/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             M O B I L E    E N C O D E R    S E R V I C E S

GENERAL DESCRIPTION
  Library of routines to initialize, and operate on, the Qualcomm CDMA
  encoder circuit.  Note that this circuit also performs interleaving and
  deinterleaving.  It is referred to in the ASIC documentation as the
  Interleaver.

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

EXTERNALIZED FUNCTIONS
  enc_set_btf
    Set the back-to-the-future value loaded by enc_init().
  enc_set_frame_off
    Set the forward/reverse channel frame offset
  enc_set_mode
    Sets the operating mode of the driver: sleep, cdma, sync, page, traffic.
  enc_tx_enable
    Enable/disable the transmit path.
  enc_user_pn_state
    Set the User PN long code state.
  enc_user_pn_mask
    Set the User PN long code mask.
  enc_iq_pn_state
    Set the short code state.
  enc_bozo_mode
    Set the "bozo mode" bit, so that the User PN long code state is reloaded
    on each 80 ms Transmit Frame boundary.
  enc_rate_set
    Set the rate set to use for encoding data on the Traffic Channel.
  enc_data_out
    Send data to the encoder.
  enc_status
    Read the status of the encoder.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  Enc_set_btf() and enc_init() should be called before any of the rest of the
  Encoder routines in order to get the chip into a known state.  Enc_set_mode()
  needs to be called before enc_data_out() is called, in order to transmit the
  data in the specific channel mode.

  On the Portable an Epoch must be given by the Demodulator initialization
  before the Interleaver can totally recover from chipx8 runt pulses from
  during the period when the chipx8 synthisizer is being programmed.

  Copyright (c) 1992 - 2020 Qualcomm Technologies, Inc.
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

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/1x.mpss/8.0/drivers/src/enc.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
01/19/18   eye     F3 logging reduction.
08/30/16   ab      1x2GTA : 0x18CD log packet changes.
05/16/16   eye     Remove the Triton workaround for MOD not running case.
04/21/16   srk     1x2GTA cleanup and TX power shoot fixes.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
03/28/16   pk      Fixed RUMI compilation error
03/21/16   eye     Adjust tx trigger for RTC rollover scenario.
12/24/15   eye     Added extra debug F3's for IQ and UL PN state.
07/06/15   eye     Add extra 4ms wait after FO update to a/c msgr delays.
05/29/15   eye     Debug code for UTX MOD crash.
05/11/15   jh      RXTX split - FR 21976
04/19/15   eye     Handle overflowing of LMEM in CH3 for Non-UTX targets.
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
03/18/15   eye     Clear muxmdsp_dac_trigger in Tx disable part of FTM mode.
01/06/15   eye     Add online mode and UTX check on wait for frame_off update.
12/29/14   ab      Wait 20ms after a frame offset update before sending
                   frame config
10/31/14   eye     Added overflow check for LMEM input buffers for CH3.
10/30/14   jh      Fix merge error
10/27/14   srk     Fix the BTF logic and TX trigger value on UTX targets.
10/08/14   srk     FEATURE_MODEM_1X_UTX related cleanup.
10/04/14   pap     Resolving KW errors reported.
07/11/14   as      Clean up Jolokia revisits
05/28/14   as      Jolokia compile clean-up
05/20/14   ssh     Removed encoding trigger to reduce time for RF cal.
04/17/14   cjb     Clean-up related to using 1x MSG macros for printing F3s
03/04/14   dkb     Remove RxLM and TxLM hardcoded BTF delays.
02/03/14   eye     Increasing the RTC trigger time to 1ms.
02/13/14   dkb     Add support for optimized Tx trigger.
02/03/14   dkb     Feature guard Triton UTX code under FEATURE_MODEM_1X_UTX
01/24/14   dkb     Change RC1/2 mod config to BPSK I only symbol mapping.
                   Send RC information to FW in 1x test waveform.
01/15/14   dkb     Changes to fix RC3 data issue.
01/10/14   dkb     Create separate ch3 write data function for UTX code.
01/09/14   dkb     Update Bolt CH3 lmem writing algorithm.
                   Account for RxFE and TxFE digital delays to BTF.
09/24/13   eye     Handling of data read, when it straddles LTU boundaries.
11/25/13   cjb     Switched logic for FEATURE_MODEM_1X_BOLT_OBSOLETE feature.
                   Mainlined FEATURE_MODEM_1X_TRITON.
11/14/13   dkb     Update support for full rate convolutional SCH.
11/14/13   dkb     Change ovsf value for 2X SCH to correct value.
11/14/13   dkb     Clean up enum issues and correct erroneous comments.
11/12/13   bb      Changes to fix word alignment issues for higher SCH data
                   rates
11/05/13   cjb     RF is still sending 0xD20 for BTF. Override BTF for now.
11/04/13   dkb     Clean up 1x RL and remove unsupported FW messages.
11/01/13   dkb     Clean up enc test waveform and reduce the frequency of
                   sending frame config to FW.
10/24/13   dkb     Add RL clean up and flag to disable bit reversing LC.
10/24/13   dkb     Add changes to address FL/RL discrepencies.
10/23/13   dkb     Update 1x waveform API to use optimized FW tx trigger.
10/21/13   dkb     Add 1x waveform API changes to init fw smem pointer.
10/16/13   dkb     Add changes for R-FCH functionality without FL dependency.
10/08/13   pap     Removing warnings for LLVM.
10/09/13   dkb     Add additional RL changes.
10/07/13   dkb     Update 1x Tx waveform for new FW interface.
09/30/13   pap     Updating the design of the module to have more manageable
                   functions.
09/30/13   dkb     Initial RL changes for new FW interface.
08/28/13   cjb     Mainline FEATURE_MODEM_1X_TRITON_REVISIT
08/09/13   dkb     Fix UTX stop sequence
07/26/13   dkb     Verify UTX mod is in a valid state for TX configuration.
07/08/13   dkb     Cleanup TX teardown
07/03/13   trc     Fix handling of contiguous non-dword-multiple ch3 payloads
06/14/13   eye     Add handling of odd number of bytes in data buffer.
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
04/22/13   trc     Allow enc_is95c_enable() to return failure
04/19/13   trc     Add ENC_SLEEP_MODE for cleaner enc/mod/dac teardown API
04/18/13   trc     Add prefill compensation for Triton
04/15/13   trc     Add critical timing handling to IS95A modulator mode toggling
                   Made algorithm paranoid, fatals upon any hint of trouble
04/12/13   trc     Ensure modulator is in IS2000 mode by default at init setup
03/28/13   trc     Increase LTU CRC FIFO size, validated perf on target
03/25/13   trc     Correct LTU CRC polynomial and insertion endianness
03/18/13   trc     Added LTU CRC insertion for convolutional SCH on Triton
                   Cleaned-up handling of encoder SCH ERAM critical section
03/14/13   trc     API cleanup for Triton/Dime, + R-SCH incr writes on Triton
03/05/13   cjb     Fixed long code mask configuration.
03/04/13   trc     Always zero UTX Tx sample count prior to triggering Tx
02/15/13   srk     Replace mcs hwio with m1x hwio
02/01/13   bph     KW fixes
12/11/12   trc     Triton UTX support
11/13/12   trc     Layering/Cleanup for Triton
10/02/12   jtm     Update TXC Sample count register name for DIME.
09/27/12   trc     Compensate Tx timing by HW UT in enc_tx_1x_test_waveform()
09/26/12   jtm     DIME_REVISIT clean up.
09/13/12   jtm     Mainlined FEATURE_MODEM_1X_NIKEL.
09/13/12   cjb     Featurized code which is not supported by TRITON modem.
09/13/12   jtm     Compiler warning clean up.
08/28/12   cjb     Added 1us delay after writing to LATCH registers.
08/22/12   cjb     Added debug variable to control delay before TXDAC trigger.
08/20/12   jtm     Allow bypassing of dac stop/start and use of a test btf value
                   Remove RUMI feature around busywait in enc_sync_tx_timing()
06/15/12   trc     Add immediate TxDAC triggering capability
05/25/12   jtm     Added encoder trigger for enc_write_data_done.
04/26/12   trc     Compensate Tx timing by FW's setting of TXC_1X_VALID_UT
03/11/12   trc     Adjust Tx DAC trigger for PN randomization delay
02/23/12   srk     Mainlined FEATURE_IS2000_SCH
02/23/12   jtm     Protect against invalid FCH rates.
02/22/12   jtm     Change Tx clock voting to use additional states.
02/08/12   trc     Compensate for Tx fifo depth at prefill
02/08/12   srk     Removed FEATURE_HWTC
01/13/12   srk     Feature Cleanup: Mainlined Always ON features
01/09/12   trc     Remove NikeL v1 HW support
01/04/12   srk     Feature Cleanup.
11/30/11   trc     Notify FW of Tx test waveform RC plus some feature cleanup
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
11/22/11   srk     Updated hdrenc_eram_lock to hdrl1_enc_eram_lock() and
                   hdrenc_eram_unlock() to hdrl1_enc_eram_unlock().
11/04/11   trc     Add RL frame offset debug msg
10/11/11   trc     Align RTC_MOD_TX and MOD_TX timing for test waveform
09/28/11   srk     Mainlined Always On features and removed Always Off Features
09/13/11   trc     Modify Tx timing sync to support PN roll granularity
09/02/11   srk     Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
08/29/11   trc     Add HDR interlock support around R-FCH ERAM writes for SVDO
08/18/11   adw     Mainline 1x message router support.
08/17/11   trc     Mainline RF SVDO API, remove RF->1xL1 enc_init_pa() API
08/11/11   trc     Modifications to have enc_tx_1x_test_waveform more reliable.
07/01/11   jtm     Cleaning up register access in enc_tx_1x_test_waveform.
06/28/11   jtm     Wrapped MOD_1X_BAND_EN and RTC_MOD_STMR_CMD access with
                   FEATURE_MODEM_1X_NIKEL_REVISIT for now.
06/28/11   vks     Changes to get 1xA working on nikel.
06/13/11   adw     Send Tx DAC stop message to FW.
06/08/11   trc     Add Tx DAC stop support to test waveform function
06/03/11   adw     SV support.
05/26/11   trc     NikeL Tx support
05/18/11   trc     Tx online mode updates based upon HW discussions
05/17/11   trc     Tx test mode update for NikeL
05/12/11   adw     Fixed compile error.
05/09/11   trc     NikeL support and cleanup
04/29/11   adw     Remove extraneous enc_set_mode() calls.
04/28/11   trc     NikeL Tx support
04/26/11   jtm     Use enc_get_tx_warmup to update TX_WARMUP.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/20/11   adw     Removed unnecessary modulator config from SYNC state.
04/12/11   jtm     Do not clear PA_ON_EN and TX_ON_EN in enc_tx_enable(FALSE).
03/30/11   adw     Register setting updates for NikeL.
03/28/11   jtm     Removed deprecated RF includes.
03/03/11   adw     Changes to the encoder initialization routine.
02/02/11   vks     Some more RF SVDO cleanup.
01/28/11   vks     Featurize RF interface with FEATURE_MODEM_1X_SUPPORTS_RF.
01/24/11   vks     Update to new RF api for RF SVDO feature development.
01/21/11   vks     Use 1X specific RF SVDO feature - FEATURE_1X_RF_SVDO_API.
01/21/11   adw     Added initial M1X_RM support.
01/19/11   vks     More register featurization for nikel.
01/18/11   jtm     Lint clean up.
12/15/10   jtm     Feature cleanup.
12/15/10   vks     Temporarily featurize register accesses for Nikel.
12/09/10   jtm     Mainlined FEATURE_SRCH_CLKREGIM_RM and cleaned up unneeded
                   legacy clock code.
12/08/10   jtm     Removed enc_set_tx_polarity and enc_set_rx_polarity.
12/02/10   jtm     Add zero-ing of Cx64 LCG in enc_init.
11/18/10   jtm     Removed Cx96 support for Pegasus and added MODEM_TXDAC_EN
                   configuration for Pegasus.
11/15/10   vks     Have SVDO ON/OFF macros only for Poseidon.
11/01/10   jtm     Modem 1x SU API changes.
11/01/10   jtm     Featurized TXDAC access for Pegasus.
10/07/10   jtm     Replaced clk_regime*() calls to enable/disable TX with
                   enc_update_tx_clk_resources().
08/25/10   jtm     Removed TxDAC programming for Cx96 since it is handled by RF.
08/10/10   vks     Clean up Genesis revisit feature.
08/06/10   jtm     Mainlining TXC-related clocks.
08/04/10   vks     Remove FEATURE_MODEM_1X_GENESIS_1_0 support. Replace
                   FEATURE_MODEM_1X_GENESIS_1_1 with FEATURE_MODEM_1X_GENESIS.
07/22/10   jtm     Fixed merge errors.
07/22/10   trc     enc_tx_enable(FALSE) now clears PA_ON_EN and TX_ON_EN to
                   be symmetric with the TRUE case.
07/09/10   trc     Move setting of A0TXR* and TXC_CGC* to na_go_to_cdma() from
                   enc_set_mode()
07/06/10   jtm     Added missing default case to switch statements.
06/25/10   jtm     Added back in necessary HWIO settings for Genesis1.1.
06/17/10   adw     Set CDMA clock config during enc initialization.
06/09/10   jtm     Fixed merge and compiler errors.
06/05/10   vks     Fix compiler error.
06/05/10   vks     mainline FEATURE_MDSP_SUPPORTS_TX_AGC.
06/02/10   trc     Genesis 1.1 cleanup
05/25/10   jtm     Fix ENC CH2 Punct pattern programming (DCCH) and refactor
                   enc_dcch_data_out() to remove FCH related code.
05/14/10   vks     No need to set MOD_PHASE_ROTATOR with MSGR interface (FW
                   is defaulting the value to 0 always)
05/13/10   trc     Add TX Conditioning clock support for Genesis 1.1
                   Remove obsolete test_enc_init()
05/10/10   vks     Remove un-necessary register settings for Genesis 1.1 call.
05/08/10   vks     Fill up missed registers for Genesis 1.1 call.
05/07/10   sst     Updates for 1.1 first call
04/22/10   vks     Cleanup register usage for 1.1.
04/22/10   trc/vks Cleanup moving TX_ON_EN control to L1.
04/15/10   trc     Update FEATURE_ENC_CONTROL_TX_ON_EN for Poseidon2
04/13/10   jtm     New HW registers settings added for Genesis 1.1
04/10/10   vks     Order the features correctly.
04/07/10   vks     Clean up Genesis 1.1 register changes
04/02/10   vks     More featurization changes between Genesis 1.0 and 1.1
03/25/10   vks     Featurize for register changes between Genesis 1.0 and 1.1
03/04/10   trc     Use more granular clk strategy disabling TXC in enc_init()
03/04/10   vks     Call RF-FW update macro everytime a FW register is touched
                   using RF macros
02/05/10   trc     Clean up logic in enc_wrt_btf_reg() to be more portable
02/08/10   vks     Temporarily bring back hardcoding btf value.
02/04/10   jtm     Removed FEATURE_PLT_*.
01/22/10   trc     Mainlined FEATURE_ENC_BTF_IN_NV.
01/05/10   vks     Set RF registers temporarily for 9K bringup.
11/30/09   vks     Group together mdsp writes for 1xADV channel gain ratios.
11/07/09   vks     Delete enc_<channel_type>_gain() functions. Use the data
                   structure channel_gains and api provided by muxmdsp.h.
08/05/09   jtm     Lint fixes.
08/04/09   jtm     Removed unused bio.h include.
07/28/09   mca     Fix compiler errors for Genesis
07/21/09   jtm     Replaced incorrect use of rf_get_tx_warmup() with
                   rfm_get_tx_warmup().
07/15/09   mca     Added more RF SVDO API support
07/06/09   jtm     Mainlined FEATURE_IS2000_1X_ADV in tables sized using
                   ENC_NUM_SCH_RCS and ENC_NUM_FCH_RCS.
06/11/09   jtm     Corrected CRC's for RC8 R-FCH and created a new set of
                   enc_fch_punct_0_tab and enc_fch_punct_1_tab specific to
                   Poseidon2.
05/15/09   jtm     Enabled cx96 modulator clk in enc_init().
04/30/09   jtm     Wrapped CX96 initializtion with FEATURE_MODEM_1X_POSEIDON
04/29/09   jtm/adw Added MOD_HW_CLK_CTL reg MOD_CX96_HW_LCG_ENA init.
04/28/09   jtm     Initial 1X Advanced feature support.
04/21/09   jtm     Replaced assert.h with amssassert.h. Assert.h is being
                   phased out.
04/17/09   jtm     Added includes to fix compiler warnings.
04/15/09   adw     Replaced MSM_OUTM with OUT to support SCMM hwio changes.
04/14/09   jtm     For SCMM/Poseidon2, default the TxConditioning blocks
                   (TX_CONDITIONING_ARM_CFG and TX_CONDITIONING_ARM_CFG1) to 0.
                   Clock DAC at cx96 for Poseidon2 targets.
04/06/09   mca     Added RF SVDO API support
04/02/09   jtm     Added msm_drv.h since it is needed for GET_RTF_ADJUST.
04/01/09   jtm     Eliminated implicit includes.
03/26/09   jtm     Mainlined T_MSM7600 and T_MSM6800B. Replaced T_POSEIDON2 with
                   FEATURE_MODEM_1X_POSEIDON. Replaced T_MSM6050 with
                   FEATURE_MODEM_1X_ARTEMIS. Replaced T_MSM7800 with
                   FEATURE_MODEM_1X_ARTEMIS || FEATURE_MODEM_1X_POSEIDON.
                   Removed T_IO_CARD, T_QUASAR_BB, and T_RUMI.
03/26/09   jtm     Added include for customer.h and modem_1x_defs.h
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to Drivers.
03/10/09   vlc     Added function enc_hw_reset() which resets encoder HW block.
02/03/09   jtm     Added support for T_POSEIDON2
10/23/08   jtm     Removed T_QUASAR
08/15/08   jtm     Restored test_enc_init for FTM testing.
07/16/08   jtm     Removed unused code. Also reduced debug string sizes to reduce
                   ROM for ULC effort.
07/01/08   vlc     Merged code changes from old tip archive.
04/03/08   trc     Let only clkrgm handle clocks if FEATURE_SRCH_CLKREGIM_RM
03/31/08   mca     Changed FEATURE_CLKREGIM_RM to FEATURE_SRCH_CLKREGIM_RM
03/05/08   vlc     User clock resource manager to manage TX clocks
                   (FEATURE_CLKREGIM_RM.)
05/21/08   vlc     Added changes for 7800 modem to mainline.
06/29/07   vm      Made the change so that Encoder software controls
                   the enabling of Tx_ON_EN
04/17/07   trc     MSM7600 support for cx32 modulator clock override
03/22/07   rkc     Changed QLIC_GET_RTF_ADJUST() to GET_RTF_ADJUST()
                   under FEATURE_SRCH_SUPPORTS_QLIC.
02/27/07   vlc     In enc_set_mode(), check if 1X has primary chain before
                   switching encoder to 1x mode (case ENC_CDMA_MODE.)
02/23/07   sst     Add QLIC support
01/09/07   vlc     Merged the following change to main line from SC2X branch.
                   Mainly, adding support for FEATURE_MDSP_SUPPORTS_TX_AGC.
           vm      Added the support for Firmware TX AGC
12/07/06   vlc     In enc_init(), replaced call to enc_tx_clk_enable() with
                   in line code.  Reverted changes for TX DAC sleep below to
                   fix R-EACH.  Since R-EACH frame can be 5ms in stead of 20ms,
                   ENC must be ready much earlier than R-ACH.
                   Removed enc_codec_clk_state, not used.
11/22/07   vlc     Fixed RVCT compiler warnings.
10/25/06   vlc     Updated to the correct register for TX DAC sleep.
10/24/06   vlc     In enc_init(), put TX DAC to sleep until it is needed.  Moved
                   TX DAC sleep control code from enc_tx_clk_enable() to
                   enc_tx_enable() to avoid waking up TX DAC during power up.
10/09/06   trc     Add MSM7600 support
09/19/06   vlc     Fixed lint errors.
06/05/06   bn      Only turn of the TX path if 1x is the only client that uses the it
04/19/06   vlc     Fixed compiler warning.
03/24/06   bn      Used the defined TXC_IS95A_FCH_GAIN instead of the hardcode value
03/08/06   bn      Applied feature T_MSM6800B
12/02/05   rkc     Replace T_MSM7500_ASIC20 with T_MSM7500B.
09/27/05   rkc     In enc_tx_clk_enable(), for feature T_MSM7500_ASIC20, use
                   MODEM_TX_DAC_SLEEP instead of MODEM_MISC_CFG0.
09/12/05   rkc     Add logic in enc_tx_clk_enable() to set or reset TX_DAC_SLEEP
07/26/05   grl     Fixed code that affected the HDR modulator during SHDR.
05/31/05   ejv     Remove include of dmod.h.
05/11/05   lp/vlc  In function enc_init(), fixed writing a whole value into the
                   MODEM_MISC_CFG0 register just to set one bit, overwriting the
                   value that was there before.  This change is under T_MSM7500.
04/25/05   db/vlc  In function enc_tx_enable(), modified code which was overwriting the Band
                   Selection field of the PA_ON_CTL register.  This change is under
                   FEATURE_RUMI only.
04/22/05   vlc     Added calls to MUXMSM_SET_MOD_MODE_1X() in enc_set_mode()
                   (merged this change from MSMSHARED archive.)
04/21/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
03/15/05   vlc     Fixed typo in feature name.
03/14/05   vlc     Merged in changes for MSM7500.  (lp/dlb')
01/07/05   jyw     Added the featurization of T_IO_CARD
12/03/05   bn      Fixed complilation errors due to feature MSM6700
07/30/04   bn      Featurized the specific code for QUASAR-BB
07/29/04   jrp     Featurized hardcoded btf delay.
07/20/04   bn      Store the actual rev frame offset into the txcmodint variable
06/29/04   jrp     Removed obsolete register writes.  Added pass-through gain
                   writes.  Intialized new 6700 registers.  Write IS95A gain
                   to mdsp.
06/18/04   bn      Renamed ENC chan enum type and fix the bug in 16-bit transfer
05/04/04   bn      Corrected the write to ERAM error in enc_write_data
05/04/04   bn      Modified the enc_write_data and enc_write_done to use the real HW registers
03/05/04   bn      Checked in encoder driver changes for MSM6700
09/03/03   dna     Ported for MSM6500
08/08/03   vlc     Fixed typo in enc_dcch_data_out().
08/01/03   dyc     Added support for FEATURE_MULTI_RX_CHAIN
07/31/03   yll     Added the BTF compensation for Frame Offset.
06/25/03   vlc     Added support for Rel B.  Created enc_current_dcch_rc
                   and changed enc_dcch_data_out to use the RC of DCCH channel.
05/20/03   bmg     Forced modulator to 1x mode in enc_init()
05/19/03   bmg     Some initial Jaguar porting in enc_init()
01/06/03   bn      Added new function to handle encoder's writing for MUXPDU5
11/27/02   cr      Delete init of TX_VERY_EARLY_FRAME_CTL and
                   TX_2_EARLY_PCG_CTL in conjunction with these having
                   been added to RF VU cagc.h.
11/05/02   et/dyc  Use rf_get_tx_warmup() for Tx puncture.
04/25/02   sr      Called enc_set_btf() for both Cellular and PCS in
                   enc_init_pa()
03/22/02   rjr     cleaned up comments in test_enc_init.
03/06/02   dlb     Added support for RF FTM.
02/22/02   tc      Supported FEATURE_IS2000_REL_A_CC.
02/11/02   sr      Removed enc_version()
02/08/02   tc      Supported Reverse Enhance Access Channel.
02/06/02   ems     Merged in changes for MSM6050.  Added register initialization
                   for TX_WARMUP, MOD_PRMBL_GAIN, MOD_TEST_CTL_WH and featurized
                   enc_voc_ctl and enc_codec_clk_ctl.  Added Rx polarity switching
                   for T_IO_CARD.  Removed ENC_CHIPX8_CLK_DISABLE and
                   ENC_CHIPX8_CLK_ENABLE.
11/09/01   hrk     Using Macro ENC_OUTHM() to program 16-bit reg ENC_TST_CTL.
11/08/01   hrk     Added function enc_pcbit_test_mask() to program register
                   MOD_PCBIT_TEST_MASK.
07/12/01   vlc     Changed FEATURE_RC4_SCH to FEATURE_DS_IS2000_RS2 in order
                   to eliminate FEATURE_RC4_SCH.
06/17/01   tc      Cleaned up encoder initialization and set mode.
06/06/01   tc      Supported reverse pilot gating for control hold.
05/03/01   tc      Supported reverse link DCCH.
04/26/01   vlc     No longer need to include debug.h.  Added PLT Reverse Link
                   SCH RC4 support.
03/23/01   sr      Added support for 32X
03/16/01   tc      Cleaned up encoder driver.
03/14/01   tc      Load enc_stmr_cmd at enc_user_pn_mask and enc_user_pn_state.
03/09/01   tn      Added new function to support programmable PA WARMUP delay.
02/28/01   tn      Added support for FEATURE_ENC_BTF_IN_NV.
02/23/01   tc      Fixed RC3 R-FCH 1/4 rate.
02/14/01   tn      Added enc_set_rx_polarity() and enc_set_tx_polarity().
01/17/01   tc      Fixed enc_pcbit_enable() and moved agc contol to rf_init.
01/16/01   tc      Replaced DEBUG_IO_CARD to T_IO_CARD.
01/10/01   tc      Cleaned up encoder feature definition code.
01/03/01   tc      Initialized modulator system timer command.
12/20/00   tc      Initialized modulator preamble control register.
12/15/00   tc      Fixed encoder mask function.
12/05/00   vlc     Added #include "debug.h".
11/09/00   tc      Defined feature T_MSM_5100.
11/07/00   tc      Added MSM5100 modulator driver support.
10/31/00   et      fixed the clock problem in reverse link
10/24/00   rmd     Fixed the masking of the value written to MOD_MISC_CTL.
10/22/00   rmd     Clean up code and merged with MSM5000 code.
09/29/00   jcw     Added ENC_PA_ON/OFF macros to support MSM5105
09/07/00   jcw     Added temp changes for VERY_EARLY_FRAME register in MSM5105
09/06/00   djd/et  Added ifdef MSMHW_TURBO_CODING around turbo coding software.
07/31/00   sr      Marked several functions with #ifndef MSM5000_IRAM_REV
07/14/00   sr      Changed ENC_NO_GATING_V to ENC_NO_GATING_PATTERN
06/22/00   sr      Added boolean generate_crc to enc_sch_data_out_cfg()
05/08/00   sr      Added support for MSM5000B
                   Added function enc_sch_data_out_cfg()
                   Added macro ENC_SCH_DATA_OUT()
04/25/00   sr      Added function definition for enc_pcbit_test_data()
04/11/00   sr      Merged in rel 0.3 changes:
                      Added function descriptions and removed one warning
                      as part of code cleanup for release 0.3
11/19/99   sr      Changes to support Supplemental Channels
10/13/99   lcc     Removed a debugging message.
10/07/99   lcc     Merged from PLT 2.0 (with SCH support).
09/23/99   lcc     Merged in MSM5000 and PLT support from PLT archive.
01/27/99   udp     Initialize the deinterleaver for sync channel mode from power-up
11/05/98   dlb     Added support for MSM3000.
12/10/97   aks     Added enc_wrt_btf_reg().  Adjusts for MSM23 BTF shift and for
                   modulo-8 BTF problem observed with MSM23 rev A3.
12/03/97   mk      Removed ENC_TEST_POINT_SEL, intitialized in dec_init().
06/16/97   aks     Added CRC disable.  Added ENC_TEST_POINT_SEL.  Changes are
                   related to MSM2.3.
01/28/97   jah     Removed CODEC clock override from enc_set_mode() for the
                   entry into CDMA mode.  It was overriding Voc's request
                   for the CODEC clocks, and causing muting.
05/14/96   jah/gb  Move PA-off responsibility to TXC to avoid an ordering
                   dependency with enc_codec_clk_ctl().
04/08/96   jah/gb  changed enc_tx_enable() to not turn off the PA, so that
                   it could turn off nicely at the end of the frame.
02/05/96   jah     Add ISS-2 to phones with leave the CODEC clock on during
                   the transition to CDMA state.
12/22/95   jah     Added hook to enc_init_pa() for BTF configuration.
06/23/95   jah     Really moved enc reset from MC to enc_init().
05/16/95   jah     Moved enc reset from MC to enc_init(), fixed enc_data_out().
03/20/95   jah     Added support for 14400 rate set and MSM2p encoder mods.
01/11/95   jah     Made enc_pn_rand_delay() available for Beta 1 (non-MSM).
10/20/94   jah     Changed enc_pn_rand_delay() to return previous delay.
10/11/94   jah     Added enc_pn_rand_delay() for Access Channel PN Rand.
06/27/94   jah     Added interlock between CODEC and Tx clocks.
03/07/94   jah     Improve some function comments.
12/21/93   jah     Have enc_init() preserve any selected interrupt timing.
09/28/93   jah     Updated header comments
06/23/93   jah     Changed enc_tx_enable() to put MASK_DATA low during the
                   time when the transmitter is disabled.  This is needed for
                   Beta II support.
05/11/93   jah     Added enc_chipx8_clk_enable/disable.
04/26/93   jah     Added changes for Beta II mobile.
03/15/93   jah     Added separate setting for the PCN/AMPS PA_WARMUP values.
03/11/93   jah     Changed enc_set_mode() and enc_tx_enable() for tx clk off.
01/14/93   jah     Have enc_set_mode() (T_P) not touch voc_frame_ref.
01/08/93   jah     Have enc_init() and enc_set_mode(ENC_CDMA_MODE) init the
                   vocoder selects and clocks as tri-stated (T_P-only).
12/23/92   jah     Changed enc_tx_enable(), and enc_set_mode() to turn PA_ON
                   'on' in enc_tx_enable(), as TX_PUNCT is goes off 1 frame
                   after it is 'set' to 'off'.
12/08/92   jah     Changed enc_init() to be portable-friendly.  This changed
                   the init state to have PA_ON low and TX_PUNCT tri-stated
                   in idle to allow the Portable to enable the PA during
                   FM mode when chipx8 is not available.
10/25/92   jah     Removed ENC_RESET for enc_init, with the note that the
                   Demod reset sends Epoch to straighten out the states that
                   get trashed by runt pulses during chipx8 programming.
10/13/92   jah     Updated enc_version() to use the version from the Demod
                   when using the MSM (portable and stretch targets).
09/01/92   jah     New PCN PA control needs no special processing.
08/31/92   jah     Added enc_init_pa() to configure PA control for PCN/not
08/07/92   jah     Fix bug in enc_init() which was turning on the PA.
08/04/92   jah     Inverted order of tx_punct/PA_ON for mobile RAS.
07/31/92   jah     Added enc_acp_pa_ctl() to control the PA for Analog calls.
07/28/92   jah     Added enc_voc_ctl() and enc_codec_clk_ctl(), fixed enc_init
                   to not trash the baud clock setting.
07/27/92   jah     Made enc_image[] private, added BIO init and PA_ON calls
01/30/92   jah     Ported from the brassboard enc.c/tx.c

===========================================================================*/


/*===========================================================================
Porting Issues:
        ENC_INIT_BTF should be calibrated to the hardware
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
//common
#include "1x_variation.h"
#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"
#include "drivers_common.h"

//drivers
#include "enc.h"                /* Encoder typedefs and prototypes      */
#include "enc_v.h"
#include "enc_i.h"
#include "enci.h"               /* Encoder register & macro definitions */
#include "enci_i.h"             /* Encoder register & macro definitions */
#include "m1x_rm.h"
#include "msm_drv.h"
#include "msm_drv_i.h"
#include "m1x_hwio_mpss.h"
#include "m1x_stmr_i.h"
#include "onex_non_sig.h"

//mux
#include "muxmsm.h"
#include "muxmdsp_i.h"
#include "rxctraffic.h"
#include "txccommon_i.h"
#include "txctraffic_i.h"
#include "txc.h"
#include "tx_v.h"
#include "txctraffic_i.h"

//srch
#include "srch_comb.h"
#include "srch_mux.h"
#include "srch_rx.h"

//cp
#include "mccsch_i.h"

//other
#include "misc.h"               /* Miscellaneous utilities prototypes   */
#include "err.h"                /* ERR error logging macro              */
#include "m1x_diag.h"
#include "qw.h"

#include "hdrl1_api.h"

//feature dependent

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
#include "rfm_1x.h"
#include "rfm_device_types.h"
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

#include "DALSys.h"
#include "qurt.h"
#include "cdmafw_msg.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

#ifdef FEATURE_MODEM_1X_UTX
/* Are we holding the HDR ERAM lock for R-SCH? */
static boolean enc_sch_has_hdr_eram_mutex = FALSE;
#endif /* FEATURE_MODEM_1X_UTX */

/* Local copy of LTU size, for on the fly computation in SCH ERAM writes.
   0 == no LTU CRC to be computed, size does not include the CRC16 appended.
   Only applicable to Triton modem, zero always for Dime. */
static uint32 enc_sch_ltu_size_bytes = 0;

/* BTF value in cx8 */
#define ENC_MAX_RF_BTF_CX8                300          /* Max RF Sanity Value */
#define ENC_MIN_RF_BTF_CX8                200          /* Min RF Sanity Value */
#define ENC_DEFAULT_RF_BTF_CX8            250          /* Default Value       */

/* 1.2288e6 cx1/sec * 0.02 sec/frame * 8 cx8/cx1 */
#define ENC_20MS_FRAME_CX8          196608

/* PN Rand Delay in cx8 */
LOCAL dword enc_pn_rand_delay_chipx8 = 0;

/* Setting for forward and reverse channel frame offsets */
LOCAL byte enc_frame_off   = 0x0;

/* LC Mask */
static qword enc_lc_mask;

/*
** Full rate frame size in bytes
*/
LOCAL const word enc_fch_full_rate_size[ ENC_NUM_FCH_RCS ] =
  {
    24,   /* RC 1 */
    36,   /* RC 2 */
    24,   /* RC 3 */
    36,   /* RC 4 */
    24    /* RC 8 */
  };

/* Frame size in bytes for all data rates (SCH RC3 and RC4). */
LOCAL const word enc_sch_frm_size_tab
            [ ENC_NUM_SCH_RCS ][ ENC_NUM_SCH_RATES ] =
            {
              { /* RC 3 */
                3, 6, 12, 24, 48, 96, 192, 384, 768
              },
              { /* RC 4 */
                5, 9, 18, 36, 72, 144, 288, 576, 0
              },
              { /* RC 8 */
                3, 6, 12, 24, 48, 96, 192, 384, 768
              }
            };

LOCAL const enc_crc_info_type enc_fch_crc_info_tab
            [ ENC_NUM_FCH_RCS ][ ENC_NUM_FRAC_RATES ] =
            {
               { {0,0},       {0,0},       {8,0x9B00},  {12,0xF130} }, /* RC 1 */
               { {6,0x1C00},  {8,0x9B00},  {10,0xF640}, {12,0xF130} }, /* RC 2 */
               { {6,0x9C00},  {6,0x9C00},  {8,0x9B00},  {12,0xF130} }, /* RC 3 */
               { {6,0x9C00},  {8,0x9B00},  {10,0xF640}, {12,0xF130} }, /* RC 4 */
               { {12,0xF130}, {12,0xF130}, {12,0xF130}, {12,0xF130} }  /* RC 8 */
            };

LOCAL const enc_crc_info_type enc_sch_crc_info_tab
            [ ENC_NUM_SCH_RCS ][ ENC_NUM_SCH_RATES ] =
          {
            { /* RC 3 */
              {6,  0x9C00}, {6,  0x9C00}, {8,  0x9B00}, {12, 0xF130},
              {16, 0xC867}, {16, 0xC867}, {16, 0xC867}, {16, 0xC867},
              {16, 0xC867}
            },
            { /* RC 4 */
              {6,  0x9C00}, {8,  0x9B00}, {10, 0xF640}, {12, 0xF130},
              {16, 0xC867}, {16, 0xC867}, {16, 0xC867}, {16, 0xC867},
              {0,  0     }
            },
            { /* RC 8 */
              {6,  0x9C00}, {6,  0x9C00}, {8,  0x9B00}, {12, 0xF130},
              {16, 0xC867}, {16, 0xC867}, {16, 0xC867}, {16, 0xC867},
              {16, 0xC867}
            }
          };

/* A 24th bit was added for Poseidon2 modems to accommodate
   additional puncture patterns for RC8.
   Reference Phy RevE standards tables 2.1.3.1.2.1-12 through -20
   and modem SWI document #80-V7864-40B, Ch. 11
*/
LOCAL const word enc_fch_punct_0_tab
            [ ENC_NUM_FCH_RCS ][ ENC_NUM_FRAC_RATES ] =
            {
              /* 1/8,    1/4,    1/2,     full */
              { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF }, /* RC 1 */
              { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF }, /* RC 2 */
              { 0x0000, 0x0000, 0xFFFF, 0xFFFF }, /* RC 3 */
              { 0x75D4, 0x75D4, 0x75D4, 0x75D4 }, /* RC 4 */
              { 0x75D4, 0x0000, 0xFFFE, 0xFFFF }  /* RC 8 */
            };

LOCAL const word enc_fch_punct_1_tab
            [ ENC_NUM_FCH_RCS ][ ENC_NUM_FRAC_RATES ] =
            {
              /* 1/8,    1/4,    1/2,     full */
              { 0x01FF, 0x01FF, 0x01FF, 0x01FF }, /* RC 1 */
              { 0x01FF, 0x01FF, 0x01FF, 0x01FF }, /* RC 2 */
              { 0x01E0, 0x01FE, 0x01FF, 0x01FF }, /* RC 3 */
              { 0x01D7, 0x01D7, 0x01D7, 0x01D7 }, /* RC 4 */
              { 0x01D7, 0x01E0, 0x01FF, 0x01FF }  /* RC 8 */
            };

LOCAL const word enc_sch_punct_0_tab
            [ ENC_NUM_SCH_RCS ][ ENC_NUM_SCH_RATES ] =
            {
              /* RC 3 */
              { 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                0xFFFF
              },
              /* RC 4 */
              { 0xBAEA, 0xBAEA, 0xBAEA, 0xBAEA, 0xB000, 0xB000, 0xB000, 0xB000,
                0xFFFF
              },
              /* RC 8 */
              { 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                0xFFFF
              }
            };

LOCAL const word enc_sch_punct_1_tab
            [ ENC_NUM_SCH_RCS ][ ENC_NUM_SCH_RATES ] =
            {
              /* RC 3 */
              { 0x00F0, 0x00FF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                0xFFFF
              },
              /* RC 4 */
              { 0x00EB, 0x00EB, 0x00EB, 0x00EB, 0x00D9, 0x00D9, 0x00D9, 0x00D9,
                0xFFFF
              },
              /* RC 8 */
              { 0x00F0, 0x00FF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                0xFFFF
              }
            };

LOCAL const enc_code_rate_type enc_code_rate_tab[ ENC_NUM_FCH_RCS ]  =
                                          {
                                             ENC_CODE_1_BY_3_RATE,  /* RC 1 */
                                             ENC_CODE_1_BY_2_RATE,  /* RC 2 */
                                             ENC_CODE_1_BY_4_RATE,  /* RC 3 */
                                             ENC_CODE_1_BY_4_RATE,  /* RC 4 */
                                             ENC_CODE_1_BY_4_RATE   /* RC 8 */
                                          };

LOCAL const enc_rate_set_type enc_rate_set_tab[ ENC_NUM_FCH_RCS ] =
                                          {
                                             ENC_RATE_9600,   /* RC 1 */
                                             ENC_RATE_14400,  /* RC 2 */
                                             ENC_RATE_9600,   /* RC 3 */
                                             ENC_RATE_14400,  /* RC 4 */
                                             ENC_RATE_9600,   /* RC 8 */
                                          };

#ifndef FEATURE_MODEM_1X_UTX
/*-----------------------------------------------------------------------------
  576 interleaver symbols
  RC1 and RC2 modulates with 64-ary Walsh per 6 symbols
  RC1 and RC2 total interleaver symbols = 576 * 64 / 6 = 6144

  For RC > 3, use orthogonal spreading with 16-ary Walsh 4
  RC > 3 total interleaver symbols = 576 * 16 / 6 = 1536
-----------------------------------------------------------------------------*/
LOCAL const word enc_fch_num_intl_sym
            [ ENC_NUM_FCH_RCS ][ ENC_NUM_FRAC_RATES ] =
            {
              /* 1/8,  1/4,  1/2, full */
              { 6144, 6144, 6144, 6144 }, /* RC 1 */
              { 6144, 6144, 6144, 6144 }, /* RC 2 */
              { 1536, 1536, 1536, 1536 }, /* RC 3 */
              { 1536, 1536, 1536, 1536 }, /* RC 4 */
              { 1536, 1536, 1536, 1536 }  /* RC 8 */
            };

typedef struct
{
  uint32 schIntlvSize;
  uint32 numIntlvSymbols;
  uint32 lowRepeat;
} enc_sch_intlv_info_type;

LOCAL const enc_sch_intlv_info_type enc_sch_intl_info[ ENC_NUM_SCH_RATES ] =
            {
              { /* 1/8 Rate  */
                .schIntlvSize = 0,
                .numIntlvSymbols = 1536,
                .lowRepeat = 4,
              },
              { /* 1/4 Rate  */
                .schIntlvSize = 0,
                .numIntlvSymbols = 1536,
                .lowRepeat = 3,
              },
              { /* 1/2 Rate  */
                .schIntlvSize = 0,
                .numIntlvSymbols = 1536,
                .lowRepeat = 2,
              },
              { /* Full Rate */
                .schIntlvSize = 0,
                .numIntlvSymbols = 1536,
                .lowRepeat = 1,
              },
              { /* 2X Rate   */
                .schIntlvSize = 0,
                .numIntlvSymbols = 1536,
                .lowRepeat = 0,
              },
              { /* 4X Rate   */
                .schIntlvSize = 1,
                .numIntlvSymbols = 3072,
                .lowRepeat = 0,
              },
              { /* 8X Rate   */
                .schIntlvSize = 2,
                .numIntlvSymbols = 6144,
                .lowRepeat = 0,
              },
              { /* 16X Rate  */
                .schIntlvSize = 3,
                .numIntlvSymbols = 12288,
                .lowRepeat = 0,
              },
              { /* 32X Rate  */
                .schIntlvSize = 3,
                .numIntlvSymbols = 12288,
                .lowRepeat = 0,
              }
            };
#endif /* !FEATURE_MODEM_1X_UTX */

/* Current rate set configured.  Initially 9600. */
LOCAL enc_fch_radio_config_type enc_current_fch_rc = ENC_FCH_RC_1;
LOCAL enc_sch_radio_config_type enc_current_sch_rc = ENC_SCH_RC_3;


#ifdef FEATURE_IS2000_REL_B
LOCAL enc_fch_radio_config_type enc_current_dcch_rc = ENC_FCH_RC_3;
#endif /* FEATURE_IS2000_REL_B */

static boolean enc_pilot_gating = FALSE;

#ifdef FEATURE_IS2000_REL_A_CC
//static boolean enc_reach_on = TRUE;

#define ENC_SCH_INTLV_SIZE_768_V     0x0020
#define ENC_SCH_INTLV_SIZE_1536_V    0x0040
#define ENC_SCH_INTLV_SIZE_3072_V    0x0060
#endif


static const struct {
  enc_code_rate_type code_rate;
  byte frame_size;
  word crc_poly;
  byte crc_len[2];
  byte data_size[2];
  word punct_patn_0[2];
  word punct_patn_1[2];
  byte enc_rate[2];
}
enc_dcch_config =
{
  ENC_CODE_1_BY_4_RATE,
  2,
  0xF130,

  /* CRC OFF, ON */
  {0,      12    },

  /* RC3,  RC4   */
  {24,     36    },
  {0xFFFF, 0xBAEA},
  {0x00FF, 0x00EB},
  {0,      1     }
};

#ifndef FEATURE_MODEM_1X_UTX
static enc_gating_pattern_type current_tx_pattern = ENC_NO_GATING_PATTERN;
static cdmafw_rl_enc_ch1_cfg_t rl_ch1_cfg;
static cdmafw_rl_enc_ch2_cfg_t rl_ch2_cfg;
static cdmafw_rl_enc_ch3_cfg_t rl_ch3_cfg;

static cdmafw_rl_enc_ch2_cfg_t default_ch2_cfg =
{
  .achEn                  = 1,          /* Default R-ACH  */
  .ch2CrcPoly             = 0,
  .ch2FrameSize           = 2,          /* 20ms Frames    */
  .ch2RadioCfg            = 1,          /* RC1 IS95A      */
  .fchCodeRate            = 0,          /* Code Rate 1/2  */
  .fchCrcLength           = 0,
  .fchEncRate             = 2,          /* Enc Rate 1/2   */
  .fchIs95cEn             = 0,
  .fchPunctPattern15to0   = 0xFFFF,     /* No puncture    */
  .fchPunctPattern24to16  = 0x1FF,
  .fchRate                = 2,          /* Intlv Rate 1/2 */
  .numIntlvSymbols        = 576
};
#endif /* !FEATURE_MODEM_1X_UTX */

#ifdef FEATURE_MODEM_1X_RUMI
static volatile uint64 enc_ftm_default_ulpn_state = 0;
#endif /* FEATURE_MODEM_1X_RUMI */

/* PN advance table for polynomial=0xA3A1, degree=15 */
static const uint16 i_pn_poly = 0xA3A1u;
static const uint16 i_pn_adv_table[ 15 ] =
{
  0x0002u, /* 2^00 advance multiplicand */
  0x0004u, /* 2^01 advance multiplicand */
  0x0010u, /* 2^02 advance multiplicand */
  0x0100u, /* 2^03 advance multiplicand */
  0x4742u, /* 2^04 advance multiplicand */
  0x75E8u, /* 2^05 advance multiplicand */
  0x47B1u, /* 2^06 advance multiplicand */
  0x20EDu, /* 2^07 advance multiplicand */
  0x1022u, /* 2^08 advance multiplicand */
  0x6C20u, /* 2^09 advance multiplicand */
  0x229Eu, /* 2^10 advance multiplicand */
  0x5F6Du, /* 2^11 advance multiplicand */
  0x0390u, /* 2^12 advance multiplicand */
  0x5C08u, /* 2^13 advance multiplicand */
  0x0A89u, /* 2^14 advance multiplicand */
};

/* PN advance table for polynomial=0x9C79, degree=15 */
static const uint16 q_pn_poly = 0x9C79u;
static const uint16 q_pn_adv_table[ 15 ] =
{
  0x0002u, /* 2^00 advance multiplicand */
  0x0004u, /* 2^01 advance multiplicand */
  0x0010u, /* 2^02 advance multiplicand */
  0x0100u, /* 2^03 advance multiplicand */
  0x38F2u, /* 2^04 advance multiplicand */
  0x20A3u, /* 2^05 advance multiplicand */
  0x1FA6u, /* 2^06 advance multiplicand */
  0x771Cu, /* 2^07 advance multiplicand */
  0x6E4Bu, /* 2^08 advance multiplicand */
  0x69A8u, /* 2^09 advance multiplicand */
  0x20A1u, /* 2^10 advance multiplicand */
  0x1FA2u, /* 2^11 advance multiplicand */
  0x770Cu, /* 2^12 advance multiplicand */
  0x6F4Bu, /* 2^13 advance multiplicand */
  0x515Au, /* 2^14 advance multiplicand */
};

static void short_pn_multiply( uint16 *pn_1,
                         const uint16 *pn_2,
                         const uint16 *poly )
{
  const unsigned int degree = 15;
  const uint16 msb_mask = (uint16)1 << degree;
  const uint16 valid_mask = msb_mask - 1;

  unsigned int index;
  uint16 product = 0;

  /* Compute product = pn_1 * pn_2 % poly */
  for ( index = 0; index <= degree; ++index)
  {
    product <<= 1;
    product ^= ( product & msb_mask ) ? *poly : 0;
    product ^= ( *pn_1 & msb_mask ) ? *pn_2 : 0;
    *pn_1 <<= 1;
}

  *pn_1 = product & valid_mask;
}

static void m1x_short_pn_advance( uint16 *pn_state,
                                  const uint16 *adv_table,
                                  const uint16 *pn_poly,
                                  int64 advance_cx1 )
{
  /* Do nothing if pn_state is NULL or advance is 0 */
  if ( pn_state && advance_cx1 )
  {
    const int64 period = 1LL<<15;
    uint16 advance_adjusted;
    unsigned int index = 0;

    /* Limit the advance amount to the PN space (handle wraparounds) */
    advance_cx1 %= period;
    advance_adjusted = (uint16)( advance_cx1 & ( period - 1 ) );

    /* Advance the PN code by walking the bits of the advance amount */
    for ( index = 0; advance_adjusted ; ++index, advance_adjusted >>= 1 )
    {
      /* Only multiply if there's a 1 in the bit position of the advance */
      if ( advance_adjusted & 1 )
      {
        short_pn_multiply( pn_state, &adv_table[ index ], pn_poly );
      }
    }
  }
}

void m1x_short_i_pn_advance( uint16 *pn_state, int64 advance_cx1 )
{
  m1x_short_pn_advance(pn_state,i_pn_adv_table,&i_pn_poly,advance_cx1);
}
void m1x_short_q_pn_advance( uint16 *pn_state, int64 advance_cx1 )
{
  m1x_short_pn_advance(pn_state,q_pn_adv_table,&q_pn_poly,advance_cx1);
}

void m1x_iq_pn_advance( uint16 *i_pn_state,
                        uint16 *q_pn_state,
                        int64  advance_cx1 )
{
  /* Advance I & Q PNs by the same amount */
  m1x_short_i_pn_advance( i_pn_state, advance_cx1 );
  m1x_short_q_pn_advance( q_pn_state, advance_cx1 );
}

/*===========================================================================

            LOCAL FUNCTIONS

===========================================================================*/
static boolean isValidRadioConfigType(enc_sch_radio_config_type type);
static boolean isValidRateType(enc_sch_rate_type);
static void enc_write_data
(
  enc_chan_number_type  chan_num,     /* which channel will be written to */
  byte                  *src_data_ptr, /* pointer points to source data */
  uint16                total_num_of_byte, /* Total number of byte */
  boolean               last_write /* is last write for a given channel */
);

#ifndef FEATURE_MODEM_1X_UTX
/*===========================================================================

FUNCTION ENC_LOAD_DEFAULT_PARAMS

DESCRIPTION
  This function loads default encoder and modulator ch2 parameters to prevent
  a WAV from entering an undefined state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void enc_load_default_params( void )
{
  muxmdsp_set_enc_ch2_params(&default_ch2_cfg);
  muxmdsp_set_mod_ch_cfg(ENC_CHAN_2, ENC_OVSF_0, ENC_SF_4, MULTI_BPSK_ON_IQ);
  muxmdsp_set_enc_input_len( ENC_CHAN_2, 12 );
  muxmdsp_enc_ch_en(ENC_CHAN_2, TRUE);
  muxmdsp_configure_rl_rc( CAI_INIT_RC );
}

/*===========================================================================

FUNCTION ENC_SET_MOD_CH2_CFG

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
void enc_set_mod_ch2_cfg
(
   enc_fch_radio_config_type rc
)
{
  enc_ovsf_type ovsf_num;
  enc_sprd_factor_type sprd_factor;
  enc_modulation_type mod;

  switch( rc )
  {
    case ENC_FCH_RC_1:
    case ENC_FCH_RC_2:
      ovsf_num    = ENC_OVSF_0;
      sprd_factor = ENC_SF_4;
      mod         = MULTI_BPSK_ON_I;
      break;
    case ENC_FCH_RC_3:
    case ENC_FCH_RC_4:
    case ENC_FCH_RC_8:
      ovsf_num    = ENC_OVSF_2;
      sprd_factor = ENC_SF_16;
      mod         = MULTI_BPSK_ON_Q;
      break;
    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid RC %d - default to RC1 configuration",
        rc );
      ovsf_num    = ENC_OVSF_0;
      sprd_factor = ENC_SF_4;
      mod         = MULTI_BPSK_ON_IQ;
      break;
  }

  muxmdsp_set_mod_ch_cfg(ENC_CHAN_2, ovsf_num, sprd_factor, mod);
}

/*===========================================================================

FUNCTION ENC_SET_MOD_CH3_CFG

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
void enc_set_mod_ch3_cfg
(
   enc_sch_rate_type rate
)
{
  enc_ovsf_type ovsf_num;
  enc_sprd_factor_type sprd_factor;
  enc_modulation_type mod = MULTI_BPSK_ON_Q;   /* CH3 modulation is always MULTI_BPSK_ON_Q */

  switch( rate )
  {
    case ENC_SCH_8TH_RATE:
    case ENC_SCH_QTR_RATE:
    case ENC_SCH_HALF_RATE:
    case ENC_SCH_FULL_RATE:
    case ENC_SCH_2X_RATE:
      ovsf_num    = ENC_OVSF_4;
      sprd_factor = ENC_SF_16;
      break;
    case ENC_SCH_4X_RATE:
      ovsf_num    = ENC_OVSF_2;
      sprd_factor = ENC_SF_8;
      break;
    case ENC_SCH_8X_RATE:
      ovsf_num    = ENC_OVSF_1;
      sprd_factor = ENC_SF_4;
      break;
    case ENC_SCH_16X_RATE:
    case ENC_SCH_32X_RATE:
      ovsf_num    = ENC_OVSF_1;
      sprd_factor = ENC_SF_2;
      break;
    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid rate %d - default to Full Rate",
        rate );
      ovsf_num    = ENC_OVSF_4;
      sprd_factor = ENC_SF_16;
      break;
  }

  muxmdsp_set_mod_ch_cfg(ENC_CHAN_3, ovsf_num, sprd_factor, mod);
}
#endif /* !FEATURE_MODEM_1X_UTX */

/*===========================================================================

FUNCTION ENC_GET_BTF_CX8

DESCRIPTION
  Load the back-to-the-future value from RF. It adds combiner delay
  (offset between finger and combiner) in chipx8. It also caps the
  PN randomization delay to the BTF value.

DEPENDENCIES
  None

RETURN VALUE
  BTF value in cx8.

SIDE EFFECTS
  None

===========================================================================*/
int16 enc_get_btf_cx8( void )
{
  int16  btf_value_cx8 = 0;

  /* Subtract out randomization delay adjusted BTF value in cx8 */
  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  btf_value_cx8 = rfm_1x_get_btf_v2( txc_get_tx_device(), srch_rx_get_prim_rx_device() );
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  if( ( btf_value_cx8 < ENC_MIN_RF_BTF_CX8 ) ||
      ( btf_value_cx8 > ENC_MAX_RF_BTF_CX8 ) )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "RF BTF (%u) out of range, Min: %u, Max: %u, defaulting to %u",
      btf_value_cx8,
      ENC_MIN_RF_BTF_CX8,
      ENC_MAX_RF_BTF_CX8,
      ENC_DEFAULT_RF_BTF_CX8 );

    btf_value_cx8 = ENC_DEFAULT_RF_BTF_CX8;
  }

  M1X_MSG( ENC, LEGACY_HIGH,
    "BTF cx8 = RF(%u) + DEMOD(%u)",
    btf_value_cx8,
    srch_mux_get_combiner_delay_cx8() );

  btf_value_cx8 += srch_mux_get_combiner_delay_cx8();

  if( enc_pn_rand_delay_chipx8 > btf_value_cx8 )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "PN randomization delay %u > total BTF %u",
      enc_pn_rand_delay_chipx8,
      btf_value_cx8 );

    /* Max delay = Max advance */
    enc_pn_rand_delay_chipx8 = btf_value_cx8;
  }

  return btf_value_cx8;

} /* enc_get_btf_cx8 */

/*===========================================================================

FUNCTION ENC_SET_FRAME_OFF

DESCRIPTION
  Specify Frame stagger offset for forward/reverse traffic frames
  This is the offset from the system frame in power control groups.
  This offset has no effect while in Sync Channel mode.  The value
  is reset to 0 offset by enc_init().

  Takes effect at the next SYNC80M.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_set_frame_off
(
  byte fwd_off,         /* Forward channel frame stagger */
  byte rev_off          /* Reverse channel frame stagger */
)
{
  enc_frame_off = rev_off & 0x0f;

  #ifdef FEATURE_MODEM_1X_UTX
  HWIO_OUT( TX_UNIFIED_FRAME_OFFSET, enc_frame_off );
  HWIO_OUT( TX_UNIFIED_FRAME_OFF_LD, 1 );
  #else /* !FEATURE_MODEM_1X_UTX */
  /* Update the frame offset in m1x_stmr. */
  m1x_stmr_set_frame_offset( enc_frame_off );
  #endif /* FEATURE_MODEM_1X_UTX */

  /* Update the mDSP copy of frame offset */
  muxmdsp_wr_fundFrameOffset( enc_frame_off );

  #ifndef FEATURE_MODEM_1X_UTX
  if( !onex_nonsig_is_ftm_mode() )
  {
    uint32 current_frame_num = 0;
    uint32 next_frame_num    = 0;
    uint32 loop_counter      = 0;

    m1x_stmr_get_frame_and_pcg_num_with_frame_offset( &current_frame_num, NULL );
    do
    {
      /* Do a busy wait for 0.5ms and then check if frame count has increased */
      DALSYS_BusyWait( 500 );
      m1x_stmr_get_frame_and_pcg_num_with_frame_offset( &next_frame_num, NULL );
      loop_counter++;
      if ( loop_counter > TXC_SHUTDOWN_COUNTER )
      {
        break;
      }
    } while( current_frame_num == next_frame_num );
  }
  #endif /* FEATURE_MODEM_1X_UTX */

  M1X_MSG( ENC, LEGACY_HIGH,
    "1x RL frame offset set to %u",
    enc_frame_off );

} /* enc_set_frame_off */

/*===========================================================================

FUNCTION ENC_SET_MODE

DESCRIPTION
  Sets the operating mode of the driver: sleep, cdma, sync, paging, traffic.

DEPENDENCIES
  CDMA mode  - must not be entered before 20ms after enc_init().
  Sleep mode - must not be entered before 20ms after enc_tx_enable(FALSE).

RETURN VALUE
  None

SIDE EFFECTS
  Changes the operating mode of the circuit.
  Sleep mode - masks the transmit path, turns off the PA, and disables
               the CHIPX8 clocks to the circuit.  PN state is lost.
  CDMA mode  - masks the transmit path, turns ON the PA, and enables the
               CHIPX8 clocks to the circuit.  If previously in sleep mode,
               the circuit will not re-synchronize until the next SYNC80M.
  SC mode    - initialize the circuit for the Sync Channel
  PC mode    - initialize the circuit for the Paging Channel
  TC mode    - initialize the circuit for the Traffic Channel

===========================================================================*/
void enc_set_mode
(
  enc_mode_type channel_mode    /* Mode in which the circuit is to run */
)
{
  M1X_MSG( ENC, LEGACY_MED,
    "Enc mode: %d",
    channel_mode );

  /* This switch selects between the various modes to initialize into.
  */
  switch (channel_mode) {
    case ENC_AC_MODE:           /* Paging/Access Channel mode */
      #ifdef FEATURE_MODEM_1X_UTX
      /* Make sure 1x mode is enabled */
      HWIO_OUTF( TX_C2K_MODE, MOD_1X_EN, 1 );

      /* Enable R-ACH, disable R-PICH & gating */
      HWIO_OUT( TX_1X_MISC_CTL,
                HWIO_FMSK(TX_1X_MISC_CTL, C2K_IS95A_ACCESS_CH) );
      #else /* !FEATURE_MODEM_1X_UTX */
      /* Enable R-ACH, disable R-PICH & gating */
      muxmdsp_get_enc_ch2_params(&rl_ch2_cfg);
      rl_ch2_cfg.achEn = TRUE;
      muxmdsp_set_enc_ch2_params(&rl_ch2_cfg);
      muxmdsp_enc_ch_en(ENC_CHAN_2, TRUE);
      #endif /* FEATURE_MODEM_1X_UTX */
      break;

    case ENC_TC_MODE:           /* Traffic Channel mode */

      #ifdef FEATURE_MODEM_1X_UTX
      /* Make sure 1x mode is enabled */
      HWIO_OUTF( TX_C2K_MODE, MOD_1X_EN, 1 );

      /* Disable R-ACH, disable R-PICH & gating */
      HWIO_OUT( TX_1X_MISC_CTL, 0 );
      #else /* !FEATURE_MODEM_1X_UTX */
      /* Disable R-ACH, disable R-PICH & gating */
      muxmdsp_get_enc_ch2_params(&rl_ch2_cfg);
      rl_ch2_cfg.achEn = FALSE;
      muxmdsp_set_enc_ch2_params(&rl_ch2_cfg);
      muxmdsp_enc_ch_en(ENC_CHAN_2, TRUE);
      #endif /* FEATURE_MODEM_1X_UTX */
      break;

    case ENC_SLEEP_MODE:        /* Put the encoder/modulator to sleep */
      #ifdef FEATURE_MODEM_1X_UTX
      {
        uint32 samp_cnt;

        /* Stop the 1x modulator and disable 1x mode, in that order */
        HWIO_OUTF( TX_UNIFIED_OFFL_EN, EN, 0 );

        /* spin wait until UTX HW has stopped */
        do
        {
          samp_cnt = HWIO_IN( TX_UNIFIED_SAMPLE_CNT_READ );
          DALSYS_BusyWait( 1000 );
          muxmdsp_print_tx_timing_info( TRUE );
        } while ( samp_cnt != HWIO_IN( TX_UNIFIED_SAMPLE_CNT_READ ) );

        /* Disable c2k mode */
        HWIO_OUTF( TX_C2K_MODE, MOD_1X_EN, 0 );
      }

      /* Send FW a cmd to stop the DAC to allow for next triggering */
      muxmdsp_stop_txdac();
      #else /* !FEATURE_MODEM_1X_UTX */
      /* Send FW a cmd to stop the TXFE to allow for next triggering */
      muxmdsp_stop_txfe();
      muxmdsp_enc_ch_en(ENC_CHAN_1, FALSE);
      muxmdsp_enc_ch_en(ENC_CHAN_2, FALSE);
      muxmdsp_enc_ch_en(ENC_CHAN_3, FALSE);
      #endif /* FEATURE_MODEM_1X_UTX */
      break;

    default:                    /* Illegal Mode, we're in trouble! */
      ONEX_ERR_FATAL("Bad ch mode %x", (uint16) channel_mode, 0, 0);
  }
} /* enc_set_mode */


/*===========================================================================

FUNCTION ENC_TX_ENABLE

DESCRIPTION
  Enable/disable the transmit path by masking and unmasking it.
  The TX_PUNCT change takes effect on the next transmit frame boundary.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_tx_enable
(
  boolean enable_tx     /* Enable/disable (mask/unmask) the transmitter */
)
{
  /* Set Tx frame valid accordingly, which controls what PA/TX valid will
     be set to by FW at next frame boundary.  This means enc_tx_enable()
     could be a bit spammy wrt. channel gain message. Intentionally do
     not touch gains here, so as to lower possible side-effects from
     legacy use of this call. */
  channel_gains.frame_valid = enable_tx ? TRUE:FALSE;
  muxmdsp_set_channel_gains();

  #ifdef FEATURE_IS2000_1X_ADV
  rc8_frame_config_parms.frame_valid = enable_tx ? TRUE:FALSE;
  #endif

} /* enc_tx_enable */

/*===========================================================================

FUNCTION ENC_USER_PN_STATE

DESCRIPTION
  Set the User PN long code state.  The User PN state takes effect at the next
  80 ms Transmit Frame Boundary (which proceeds SYNC80M by the count in the
  Back-to-the-Future counter).  Loading 0's into the user PN state inhibits
  spreading from the User PN Generator.  This operation is non-atomic.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_user_pn_state
(
  qword user_pn_state   /* User PN state to load into User PN Generator */
)
{
  uint32 hi_mask = qw_hi( user_pn_state );
  uint32 lo_mask = qw_lo( user_pn_state );

  #ifdef FEATURE_MODEM_1X_UTX
  /* Load the 42-bit user PN state */
  HWIO_OUT( TX_1X_U_PN_STATE_0, lo_mask );
  HWIO_OUT( TX_1X_U_PN_STATE_1, hi_mask & 0x3ff );
  HWIO_OUT( TX_UNIFIED_PN_LD, 1 );
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_set_rl_lc_state( ( (uint64)user_pn_state[1] << 32 ) |
                             (uint64)user_pn_state[0] );
  #endif /* FEATURE_MODEM_1X_UTX */

  M1X_MSG( ENC, LEGACY_HIGH,
    "LC STATE: High - 0x%03X, Low - 0x%08X",
    hi_mask,
    lo_mask );

} /* enc_user_pn_state */

/*===========================================================================

FUNCTION ENC_USER_PN_MASK

DESCRIPTION
  Set the User PN long code mask.  The User PN mask takes effect at the next
  20 ms Transmit Frame Boundary.  Loading 0's into the user PN mask inhibits
  spreading from the User PN Generator.  This operation is non-atomic.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_user_pn_mask
(
  qword user_pn_mask    /* User PN mask to load into User PN Generator */
)
{
  uint32 hi_mask = qw_hi( user_pn_mask );
  uint32 lo_mask = qw_lo( user_pn_mask );

  #ifdef FEATURE_MODEM_1X_UTX
  /* Load the 42-bit User PN mask */
  HWIO_OUT( TX_1X_U_PN_MASK_0, lo_mask );
  HWIO_OUT( TX_1X_U_PN_MASK_1, hi_mask & 0x3ff );

  /* Execute the load command (one-shot), happens on next sync80 */
  HWIO_OUT( TX_UNIFIED_PN_MASK_LD, 1 );
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_set_rl_lc_mask( ( (uint64)user_pn_mask[1] << 32 ) |
                            (uint64)user_pn_mask[0] );
  #endif /* FEATURE_MODEM_1X_UTX */

  qw_set( enc_lc_mask, hi_mask, lo_mask );

  M1X_MSG( ENC, LEGACY_HIGH,
    "LC MASK: High - 0x%03X, Low - 0x%08X",
    hi_mask,
    lo_mask );

} /* enc_user_pn_mask */

/*===========================================================================

FUNCTION ENC_IQ_PN_STATE

DESCRIPTION
  Set the I & Q PN states into the I & Q PN Generator.  Load 0's to inhibit
  spreading from the I & Q PN Generator.  This takes effect at the next
  80 ms Transmit Frame Boundary (which proceeds SYNC80M by the count in the
  Back-to-the-Future counter).  This operation is non-atomic.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_iq_pn_state
(
  word i_pn_state,      /* I PN state */
  word q_pn_state       /* Q PN state */
)
{
  #ifdef FEATURE_MODEM_1X_UTX
  /* Load the 42-bit User PN mask */
  HWIO_OUT( TX_1X_I_PN_STATE, (uint32)i_pn_state & 0x7FFF );
  HWIO_OUT( TX_1X_Q_PN_STATE, (uint32)q_pn_state & 0x7FFF );
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_set_rl_spn_state( i_pn_state, q_pn_state );
  #endif /* FEATURE_MODEM_1X_UTX */

  M1X_MSG( ENC, LEGACY_HIGH,
    "Short PN STATE: I - 0x%04X, Q - 0x%04X",
    (uint32)i_pn_state & 0x7FFF,
    (uint32)q_pn_state & 0x7FFF );

} /* enc_iq_pn_state */

/*===========================================================================

FUNCTION ENC_BOZO_MODE

DESCRIPTION
  Set the "bozo mode" bit, so that the User PN long code state is reloaded
  on each 80 ms Transmit Frame boundary.  Useful when debugging time sync
  problems between the Cel and the mobile.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_bozo_mode
(
  boolean enable_bozo_mode      /* Whether to enable/disable "bozo" mode */
)
{
  /* Set the "bozo" mode bit accordingly */
  #ifdef FEATURE_MODEM_1X_UTX
  HWIO_OUT( TX_1X_LPN_BOZO, enable_bozo_mode ? 1 : 0 );
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_bozo_mode */

/*===========================================================================

FUNCTION ENC_RATE_SET

DESCRIPTION
  Set the rate set to use for encoding data on the Traffic Channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_rate_set
(
  enc_rate_set_type rate_set    /* Rate set to use for encoding data */
)
{
  /*-----------------------------------------------------------------*/

  if ( rate_set != ENC_RATE_9600 && rate_set != ENC_RATE_14400 ) {
     ONEX_ERR_FATAL( "Bad Rate Set", 0, 0, 0 );
  }
} /* enc_rate_set */

#ifdef FEATURE_MODEM_1X_UTX
/*===========================================================================

FUNCTION ENC_SET_ERAM_BANK

DESCRIPTION
  Set the ERAM bank and reset the write pointer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void enc_set_eram_bank( enc_chan_number_type chan_num )
{
  /* With Triton, ensure default eram ptrs in HW */
  HWIO_OUT( TX_UNIFIED_MEM_PTR_EN , 0 );

  /* Triton ERAM banks map to channel # 0 = ch1, 1 = ch2, 2 = ch3 */
  HWIO_OUT( TX_UNIFIED_ERAM_CTL, (uint32)chan_num );
  HWIO_OUT( TX_UNIFIED_ENC_ERAM_INCRACC_INIT, 1 );
  HWIO_OUT( TX_UNIFIED_ENC_ERAM_INCRACC_INIT, 0 );
} /* enc_set_eram_bank */
#endif /* FEATURE_MODEM_1X_UTX */

/*===========================================================================

FUNCTION ENC_FCH_DATA_OUT

DESCRIPTION
  Send data to the encoder, generate a CRC (or not), and encode it at
  a specified rate. The Tiger2 definitions of (fractional) rates is
  the reverse of Panther's: 00 means 8th rate in Tiger2 whereas it
  meant full rate earlier, and so forth. Other modules use the old
  definitions of rate as defined in enc_rate_type, so we  keep the old one
  and do (3 - rate) in this function.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_fch_data_out
(
  byte *data_ptr,                /* Buffer to send to Encoder               */
  boolean generate_crc,          /* Have the Encoder generate a CRC, or not */
  enc_rate_type rate             /* Rate to encode the data as              */
)
{
  #ifdef FEATURE_MODEM_1X_UTX
  word enc_ctl;                 /* Encoder control value for this packet  */
  #else /* !FEATURE_MODEM_1X_UTX */
  cdmafw_rl_enc_ch2_cfg_t enc_ch2_cfg;
  #endif /* FEATURE_MODEM_1X_UTX */
  word data_size = 0;              /* Data size for this packet              */
  byte crc_len;
  word crc_poly = 0;
  enc_code_rate_type code_rate;

  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_IS2000_1X_ADV
  ONEX_ASSERT( ( ( enc_current_fch_rc >= ENC_FCH_RC_1 )   &&
                 ( enc_current_fch_rc <= ENC_FCH_RC_4 ) ) ||
               ( enc_current_fch_rc == ENC_FCH_RC_8 ) );
  #else
  ONEX_ASSERT( ( enc_current_fch_rc >= ENC_FCH_RC_1 )   &&
               ( enc_current_fch_rc <= ENC_FCH_RC_4 ) );
  #endif /* FEATURE_IS2000_1X_ADV */

  /*Check for invalid rates given to ENC. If invalid default to full rate. */
  if( rate >= ENC_NUM_FRAC_RATES )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid rate (%d). Default to FULL (%d)",
      rate,
      ENC_FULL_RATE);
    rate = ENC_FULL_RATE;
  }

  /* Compute the data size in bytes; round up any fractional part
  */
  data_size = (word)(enc_fch_full_rate_size[ENC_FCH_RC_INX(enc_current_fch_rc)]
                    + ((1 << ((uint16) rate)) - 1)) >> ((uint16) rate);

  // MSM5100 modulator needs to have 32 bits alignment data input.
  data_size = ((data_size + 3) >> 2) * 4 ;

  if( generate_crc )
  {
    crc_len = (enc_fch_crc_info_tab[ ENC_FCH_RC_INX( enc_current_fch_rc ) ]
               [ ENC_FCH_RATE_INX( rate ) ]).length;
    crc_poly = (enc_fch_crc_info_tab[ ENC_FCH_RC_INX( enc_current_fch_rc ) ]
                [ ENC_FCH_RATE_INX( rate ) ]).poly;
    #ifdef FEATURE_MODEM_1X_UTX
    HWIO_OUT( TX_1X_CH2_CRC_POLY, crc_poly );
    #endif /* FEATURE_MODEM_1X_UTX */
  }
  else
  {
     crc_len = 0;
  }

  code_rate = enc_code_rate_tab[ ENC_FCH_RC_INX(enc_current_fch_rc) ];

  #ifdef FEATURE_MODEM_1X_UTX
  //
  // program puncture patterns (only affects RC 3 & 4)
  //
  HWIO_OUT(
             TX_1X_CH2_PUNCT_PATN_0,
             enc_fch_punct_0_tab[ENC_FCH_RC_INX(enc_current_fch_rc)]
                                [ENC_FCH_RATE_INX( rate )]
          );
  HWIO_OUT(
             TX_1X_CH2_PUNCT_PATN_1,
             enc_fch_punct_1_tab[ENC_FCH_RC_INX(enc_current_fch_rc)]
                                [ENC_FCH_RATE_INX( rate )]
          );

  enc_ctl = (word) (
                      ( (word) ENC_FCH_RATE_INX( rate ) << ENC_FCH_RATE_POS ) |
                      ( ( (word) code_rate ) << ENC_FCH_CODE_RATE_POS ) |
                      ( ( (word) crc_len ) << ENC_FCH_CRCLEN_POS )
                   );

  /* set the rate, rate set, code rate and crc length */
  HWIO_OUTM(
             TX_1X_CH2_ENC_CTL_0,
             (
                ENC_FCH_CRCLEN_M |
                HWIO_FMSK(TX_1X_CH2_ENC_CTL_0,FCH_CODE_RATE) |
                HWIO_FMSK(TX_1X_CH2_ENC_CTL_0,FCH_ENC_RATE)
             ),
             enc_ctl
           );

  enc_ctl = (word) ( ( (word) ENC_FCH_FRAME_SIZE_V ) << ENC_FCH_FRAME_SIZE_POS );

  #ifdef FEATURE_IS2000_1X_ADV
  /* For the MOD_CH2_ENC_CTL_1 register, RC8 does not follow the convention
     of other RC's. RC8 equates to a value of 7 and not 8. We must adjust
     the value of the current rc to take this into account before writing
     to this register.
  */

  if ( enc_current_fch_rc == ENC_FCH_RC_8 )
  {
    enc_ctl |= (word) ( ( (word) 0x7 ) << ENC_FCH_RADIO_CONFIG_POS );
  }
  else
  #endif /* FEATURE_IS2000_1X_ADV */
  {
    enc_ctl |= (word) ( ( (word) enc_current_fch_rc ) << ENC_FCH_RADIO_CONFIG_POS );
  }

  /* set the radio configuration, 20 ms frame size */
  HWIO_OUT( TX_1X_CH2_ENC_CTL_1, ( (byte) enc_ctl ) );

  /* set the walshcover select */
  HWIO_OUTM( TX_1X_MOD_WCOVER_SEL,
             ENC_WALSHCOVER_SEL_FCH_M,
             ENC_WALSHCOVER_SEL_FCH_V
           );

  if( enc_current_fch_rc > ENC_FCH_RC_2 )
  {
     (void)enc_is95c_enable( TRUE ); // IS-95C
     enc_pch_enable( TRUE );
     enc_fch_enable( TRUE );
  }
  else
  {
     (void)enc_is95c_enable( FALSE ); // IS-95A
     enc_pch_enable( FALSE );
     enc_fch_enable( TRUE );
  }

  /* interlock w/ DO ERAM accesses */
  hdrl1_enc_eram_lock();

  /* Initialize the ERAM pointer and bank */
  enc_set_eram_bank( ENC_CHAN_2 );

  /* write the 1x data */
  enc_write_data(ENC_CHAN_2,data_ptr,data_size,TRUE);

  /* un-interlock w/ DO ERAM accesses */
  hdrl1_enc_eram_unlock();
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_get_enc_ch2_params(&enc_ch2_cfg);
  /* Populate Enc Ch2 Bit Field structure in FW shared memory */  /*  BITS    */
  enc_ch2_cfg.fchEncRate    = ( ENC_FCH_RATE_INX( rate ) & 0x3 ); /*  1 :  0  */
  enc_ch2_cfg.fchCodeRate   = ( code_rate & 0x3 );                /*  3 :  2  */
  enc_ch2_cfg.fchCrcLength  = ( crc_len & 0x1F );                 /*  8 :  4  */
  enc_ch2_cfg.ch2FrameSize  = ( ENC_FCH_FRAME_SIZE_V & 0x3 );     /* 10 :  9  */
  enc_ch2_cfg.ch2RadioCfg   =                                     /* 13 : 11  */
            (
            #ifdef FEATURE_IS2000_1X_ADV
              ( enc_current_fch_rc == ENC_FCH_RC_8 ) ? 0x7 :
            #endif /* FEATURE_IS2000_1X_ADV */
              ( enc_current_fch_rc & 0x7 )
            );
  enc_ch2_cfg.ch2CrcPoly    = ( crc_poly & 0xFFFF );              /* 29 : 14  */
  /* reserved bits */                                             /* 31 : 30  */
  enc_ch2_cfg.fchPunctPattern24to16 =                             /* 40 : 32  */
            ( enc_fch_punct_1_tab[ENC_FCH_RC_INX(enc_current_fch_rc)]
                                 [ENC_FCH_RATE_INX( rate )] & 0x1FF );
  enc_ch2_cfg.fchPunctPattern15to0  =                             /* 56 : 41  */
            ( enc_fch_punct_0_tab[ENC_FCH_RC_INX(enc_current_fch_rc)]
                                 [ENC_FCH_RATE_INX( rate )] & 0xFFFF );
  /* enc_ch2_cfg.achEn */                                         /* 57 : 57  */
  enc_ch2_cfg.fchIs95cEn    =                                     /* 58 : 58  */
            ( ( ( enc_current_fch_rc > ENC_FCH_RC_2 ) ? 1 : 0 ) & 0x1 );
  enc_ch2_cfg.fchRate       = ( ENC_FCH_RATE_INX( rate ) & 0x3 ); /* 60 : 59  */
  /* reserved bits */                                             /* 63 : 61  */
  enc_ch2_cfg.numIntlvSymbols =                                   /* 79 : 64  */
            ( enc_fch_num_intl_sym[ENC_FCH_RC_INX(enc_current_fch_rc)]
                                  [ENC_FCH_RATE_INX( rate )] & 0xFFFF );
  /* reserved bits */                                             /* 95 : 80  */


  M1X_MSG( ENC, LEGACY_HIGH,
    "FCH cfg: enc rate %u code rate %u intlv rate %u crc len %u RC %u "
    "is95c %u punct [%03X][%04X]",
    enc_ch2_cfg.fchEncRate,
    enc_ch2_cfg.fchCodeRate,
    enc_ch2_cfg.fchRate,
    enc_ch2_cfg.fchCrcLength,
    enc_ch2_cfg.ch2RadioCfg,
    enc_ch2_cfg.fchIs95cEn,
    enc_ch2_cfg.fchPunctPattern24to16,
    enc_ch2_cfg.fchPunctPattern15to0 );

  muxmdsp_set_frame_config_data_rate( ENC_FCH_RATE_INX( rate ) & 0x3 );
  muxmdsp_set_enc_ch2_params(&enc_ch2_cfg);
  enc_set_mod_ch2_cfg(enc_current_fch_rc);
  muxmdsp_set_enc_input_len( ENC_CHAN_2, data_size );

  /* save a copy of the most recent encoder config */
  rl_ch2_cfg = enc_ch2_cfg;

  /* write the 1x data */
  enc_write_data(ENC_CHAN_2,data_ptr,data_size,TRUE);
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_fch_data_out */

/*===========================================================================

FUNCTION ENC_SCH_DATA_OUT_CFG

DESCRIPTION
  Send data to the encoder, generate a CRC (or not), and encode it at
  a specified rate.

  SCH Encoder Rate - RC 4 enc rates are defined in FW as the same as RC 3 with
  a 1 in the MSBit.
        Register Val   : Data Rate
        -----------RC 3-----------
         00000       : 1.5 Kbps
         00001       : 2.7 Kbps
         00010       : 4.8 Kbps
         00011       : 9.6 Kbps
         00100       : 19.2 Kbps
         00101       : 38.4 Kbps
         00110       : 76.8 Kbps
         00111       : 153.6 Kbps
         01000       : 307.2 Kbps
      01001 to 01111 : Undefined
        -----------RC 4-----------
         10000       : 1.8 Kbps
         10001       : 3.6 Kbps
         10010       : 7.2 Kbps
         10011       : 14.4 Kbps
         10100       : 28.8 Kbps
         10101       : 57.6 Kbps
         10110       : 115.2 Kbps
         10111       : 230.4 Kbps
      10111 to 11111 : Undefined

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_sch_data_out_cfg
(
  enc_sch_rate_type rate,        /* Rate to encode the data as              */
  boolean turbo_code,            /* TRUE= Turbo, FALSE=Convolutional        */
  word ltu_size,                 /* LTU block size with CRC in bits         */
  boolean generate_crc           /* Have the Encoder generate a CRC, or not */
)
{
  #ifdef FEATURE_MODEM_1X_UTX
  word enc_ctl;                 /* Encoder control value for this packet  */
  #else /* !FEATURE_MODEM_1X_UTX */
  cdmafw_rl_enc_ch3_cfg_t enc_ch3_cfg;
  #endif /* FEATURE_MODEM_1X_UTX */
  byte crc_len;
  word crc_poly = 0;
  enc_rate_set_type rate_set;

  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_IS2000_1X_ADV
  ONEX_ASSERT( ( enc_current_sch_rc == ENC_SCH_RC_3 ) ||
               ( enc_current_sch_rc == ENC_SCH_RC_4 ) ||
               ( enc_current_sch_rc == ENC_SCH_RC_8 ) );
  #else
  ONEX_ASSERT( ( enc_current_sch_rc == ENC_SCH_RC_3 ) ||
               ( enc_current_sch_rc == ENC_SCH_RC_4 ) );
  #endif /* FEATURE_IS2000_1X_ADV */


  ONEX_ASSERT( rate < ENC_NUM_SCH_RATES);

  if ( ( enc_current_sch_rc == ENC_SCH_RC_3 )
        #ifdef FEATURE_IS2000_1X_ADV
        || ( enc_current_sch_rc == ENC_SCH_RC_8 )
        #endif /* FEATURE_IS2000_1X_ADV */
  )
  {
    rate_set = ENC_RATE_9600;
  }
  else
  {
    rate_set = ENC_RATE_14400;
  }

  if( generate_crc )
  {
    crc_len =
      (enc_sch_crc_info_tab [ENC_SCH_RC_INX(enc_current_sch_rc)][rate]).length;
    crc_poly =
      (enc_sch_crc_info_tab [ENC_SCH_RC_INX(enc_current_sch_rc)][rate]).poly;
    #ifdef FEATURE_MODEM_1X_UTX
    HWIO_OUT( TX_1X_CH3_CRC_POLY, crc_poly );
    #endif /* FEATURE_MODEM_1X_UTX */
  }
  else
  {
     crc_len = 0;
  }

  #ifdef FEATURE_MODEM_1X_UTX
  //
  // program puncture pattern
  //
  if ( (turbo_code) && (enc_current_sch_rc == ENC_SCH_RC_4) )
  {
    HWIO_OUT(TX_1X_CH3_PUNCT_PATN_0, ENC_SCH_RC4_TURBO_PUNCT_0_PATT);
    HWIO_OUT(TX_1X_CH3_PUNCT_PATN_1, ENC_SCH_RC4_TURBO_PUNCT_1_PATT);
  }
  else
  {
    HWIO_OUT( TX_1X_CH3_PUNCT_PATN_0,
              enc_sch_punct_0_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate] );
    HWIO_OUT( TX_1X_CH3_PUNCT_PATN_1,
              enc_sch_punct_1_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate] );
  }

  if( rate == ENC_SCH_32X_RATE )
  {
    enc_ctl = (word) (
                       ( (word) rate << ENC_SCH_RATE_POS ) |
                       ( (word) rate_set << ENC_SCH_RATE_SET_POS ) |
                       ( ( (word) ENC_CODE_1_BY_2_RATE )
                                  << ENC_SCH_CODE_RATE_POS ) |
                       ( ( (word) crc_len ) << ENC_SCH_CRCLEN_POS )
                     );
  }
  else
  {
        enc_ctl = (word) (
                                                ( (word) rate << ENC_SCH_RATE_POS ) |
                                                ( (word) rate_set << ENC_SCH_RATE_SET_POS ) |
                                                ( ( (word) ENC_CODE_1_BY_4_RATE )
                                                                   << ENC_SCH_CODE_RATE_POS ) |
                                                ( ( (word) crc_len ) << ENC_SCH_CRCLEN_POS )
                                         );
  }

  /* set the rate, rate set, code rate and crc length */
  HWIO_OUTM
  (
    TX_1X_CH3_ENC_CTL_0,
    HWIO_FMSK(TX_1X_CH3_ENC_CTL_0,SCH_CRC_LENGTH) |
    HWIO_FMSK(TX_1X_CH3_ENC_CTL_0,SCH_CODE_RATE)  |
    HWIO_FMSK(TX_1X_CH3_ENC_CTL_0,SCH_ENC_RATE),
    enc_ctl
  );

/*
  data rate 32X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_32X_V ENC_SCH_WALSHCOVER_1_V
  data rate 16X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_16X_V ENC_SCH_WALSHCOVER_1_V
  data rate  8X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_8X_V  ENC_SCH_WALSHCOVER_2_V
  data rate  4X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_4X_V  ENC_SCH_WALSHCOVER_2_V
  data rate  2X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_2X_V  ENC_SCH_WALSHCOVER_2_V
  data rate  1X  ENC_SCH_LOW_REPEAT_2_V   ENC_SCH_INTLV_SIZE_1X_V  ENC_SCH_WALSHCOVER_2_V
  data rate 1/2  ENC_SCH_LOW_REPEAT_4_V   ENC_SCH_INTLV_SIZE_1X_V  ENC_SCH_WALSHCOVER_2_V
  data rate 1/4  ENC_SCH_LOW_REPEAT_8_V   ENC_SCH_INTLV_SIZE_1X_V  ENC_SCH_WALSHCOVER_2_V
  data rate 1/8  ENC_SCH_LOW_REPEAT_16_V  ENC_SCH_INTLV_SIZE_1X_V  ENC_SCH_WALSHCOVER_2_V
*/

  switch (rate)
  {
    case ENC_SCH_8TH_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_1X_V | ENC_SCH_LOW_REPEAT_16_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;
    case ENC_SCH_QTR_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_1X_V | ENC_SCH_LOW_REPEAT_8_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );

      break;
    case ENC_SCH_HALF_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_1X_V | ENC_SCH_LOW_REPEAT_4_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;
    case ENC_SCH_FULL_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_1X_V | ENC_SCH_LOW_REPEAT_2_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;
    case ENC_SCH_2X_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_2X_V | ENC_SCH_LOW_REPEAT_1_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;
    case ENC_SCH_4X_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_4X_V | ENC_SCH_LOW_REPEAT_1_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;
    case ENC_SCH_8X_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_8X_V | ENC_SCH_LOW_REPEAT_1_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;
      /* program the same value to HWIO_ADDR(MOD_CH3_ENC_CTL_1) for rate = 16X or 32X. */
    case ENC_SCH_16X_RATE:
    case ENC_SCH_32X_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_16X_V | ENC_SCH_LOW_REPEAT_1_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;

    default:
      ONEX_ERR_FATAL("Invalid R-SCH rate (%d)", rate, 0, 0);
  }

  /* set the radio configuration, 20 ms frame size */
  HWIO_OUT( TX_1X_CH3_ENC_CTL_1, enc_ctl );

  /*
  ** Extended supplemental channel assignment message
  ** specifies REV_WALSH_ID for supplemental channel.
  ** If only one reverse supplemental channel is assigned,
  ** the base station should set the default value.
  ** Set the default walshcover select depends on the
  ** interleaver size.
  */
  /*
     Program MOD_WALSHCOVER_SEL:CH3_WALSHCOVER_SEL with 1 when rate
     is equal to 16X or 32X.
  */
  if
  (
    (rate == ENC_SCH_16X_RATE) || (rate == ENC_SCH_32X_RATE)
  )
  {
    HWIO_OUTM( TX_1X_MOD_WCOVER_SEL,
               ENC_WALSHCOVER_SEL_SCH_M,
               ENC_WALSHCOVER_SEL_SCH_1_V
             );
  }
  else
  {
    HWIO_OUTM( TX_1X_MOD_WCOVER_SEL,
               ENC_WALSHCOVER_SEL_SCH_M,
               ENC_WALSHCOVER_SEL_SCH_2_V
             );
  }

  (void)enc_is95c_enable( TRUE ); // IS-95C
  #else /* !FEATURE_MODEM_1X_UTX */
  if ( ( rate < ENC_SCH_8TH_RATE ) || ( rate >= ENC_NUM_SCH_RATES )  )
  {
    ONEX_ERR_FATAL("Invalid R-SCH rate (%d)", rate, 0, 0);
  }

  muxmdsp_get_enc_ch3_params(&enc_ch3_cfg);
  /* Populate Enc Ch3 Bit Field structure in FW shared memory */  /*  BITS    */
  enc_ch3_cfg.schEncRate   =                                      /*  4 :  0  */
    ( ( ( rate_set << ENC_SCH_RATE_SET_POS ) | rate ) & 0x1F );
  enc_ch3_cfg.schCodeRate  = ( ( ( rate == ENC_SCH_32X_RATE ) ?   /*  6 :  5  */
                                  ENC_CODE_1_BY_2_RATE :
                                  ENC_CODE_1_BY_4_RATE ) & 0x3 );
  enc_ch3_cfg.schCrcLength = ( crc_len & 0x1F );                  /* 11 :  7  */
  enc_ch3_cfg.turboEncode  = ( turbo_code & 0x1 );                /* 12 : 12  */
  enc_ch3_cfg.ch3FrameSize = ( ENC_SCH_FRAME_SIZE_20_V & 0x3 );   /* 14 : 13  */
  enc_ch3_cfg.ch3Sel       = ( ENC_SCH_SELECT_SCH_V & 0x7 );      /* 17 : 15  */
  enc_ch3_cfg.lowRepeat    =                                      /* 20 : 18  */
                          ( (uint32)enc_sch_intl_info[rate].lowRepeat & 0x7 );
  /* reserved bits */                                             /* 31 : 21  */
  enc_ch3_cfg.ch3CrcPolynomial = ( crc_poly & 0xFFFF );           /* 47 : 32  */
  enc_ch3_cfg.numIntlvSymbols  =                                  /* 63 : 48  */
                          ( enc_sch_intl_info[rate].numIntlvSymbols & 0xFFFF );

  if ( (turbo_code) && (enc_current_sch_rc == ENC_SCH_RC_4) )
  {
    enc_ch3_cfg.schPunctPattern23to16 =                           /* 71 : 64  */
                          ( ENC_SCH_RC4_TURBO_PUNCT_1_PATT & 0xFF );
    enc_ch3_cfg.schPunctPattern15to0  =                           /* 87 : 72  */
                          ( ENC_SCH_RC4_TURBO_PUNCT_0_PATT & 0xFFFF );
  }
  else
  {
    enc_ch3_cfg.schPunctPattern23to16 =
    ( enc_sch_punct_1_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate] & 0xFF );
    enc_ch3_cfg.schPunctPattern15to0  =
    ( enc_sch_punct_0_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate] & 0xFFFF );
  }

  enc_ch3_cfg.eachEn       = 0;                                   /* 88 : 88  */
  enc_ch3_cfg.schIntlvSize =                                      /* 90 : 89  */
                        ( (uint32)enc_sch_intl_info[rate].schIntlvSize & 0x3 );
  /* reserved bits */                                             /* 95 : 91  */

  muxmdsp_set_enc_ch3_params(&enc_ch3_cfg);
  enc_set_mod_ch3_cfg(rate);
  muxmdsp_set_enc_input_len(ENC_CHAN_3,
          enc_sch_frm_size_tab[ ENC_SCH_RC_INX( enc_current_sch_rc ) ][ rate ]);

  /* save a copy of the most recent encoder config */
  rl_ch3_cfg = enc_ch3_cfg;
  #endif /* FEATURE_MODEM_1X_UTX */

  enc_sch_enable( TRUE ); // IS-95C

  if( turbo_code )
  {
    #ifdef FEATURE_MODEM_1X_UTX
    /* Turn on turbo, disable LTU CRC */
    HWIO_OUTF( TX_1X_CH3_ENC_CTL_0, TURBO_ENCODE, 1 );
    #endif /* FEATURE_MODEM_1X_UTX */

    enc_sch_ltu_size( 0 );
  }
  else
  {
    #ifdef FEATURE_MODEM_1X_UTX
    /* Turn off turbo, check and set LTU CRC size accordingly */
    HWIO_OUTF( TX_1X_CH3_ENC_CTL_0, TURBO_ENCODE, 0 );
    #endif /* FEATURE_MODEM_1X_UTX */

    ONEX_ASSERT( ( ltu_size == 0 ) ||
                 (( ltu_size >= ENC_LTU_MIN ) &&
                  ( ltu_size <= ENC_LTU_MAX )) );
    enc_sch_ltu_size( ltu_size );
  }

} /* enc_sch_data_out_cfg */

/*===========================================================================

FUNCTION ENC_SCH_MUXPDU_DATA_OUT

DESCRIPTION
  Send data to the encoder for MUXPDUs 5 and 3, appending to last data written

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Note that a mutex for ERAM (w/ HDR) is grabbed in enc_sch_data_out_cfg()
  and will only be released in enc_sch_write_data_done()

===========================================================================*/
void enc_sch_muxpdu_data_out
(
  byte *data_ptr,                /* Buffer to send to Encoder */
  word total_numberofbyte
)
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  if ( !enc_sch_has_hdr_eram_mutex )
  {
    /* interlock w/ DO ERAM accesses */
    hdrl1_enc_eram_lock();

    /* note internally that we have an outstanding mutex */
    enc_sch_has_hdr_eram_mutex = TRUE;

    /* We need to init the ERAM write pointer once here, since all
       subsequent CH3 writes assume concatenation. */
    enc_set_eram_bank( ENC_CHAN_3 );
  }

  enc_write_data( ENC_CHAN_3, data_ptr, total_numberofbyte, FALSE );

  /* Note that we leave the lock hanging here, as it gets cleared
     when the SCH writing is done in enc_sch_write_data_done() */
  #else /* !FEATURE_MODEM_1X_UTX */
  enc_write_data( ENC_CHAN_3, data_ptr, total_numberofbyte, FALSE );
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_sch_muxpdu_data_out */

/*===========================================================================

FUNCTION enc_write_sch_data_done

DESCRIPTION
  This function is used to indicate the HW writing to ENCODER is done.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 None.

===========================================================================*/
void enc_sch_write_data_done ( void )
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  /* un-interlock w/ DO ERAM accesses, if we're holding the mutex */
  if ( enc_sch_has_hdr_eram_mutex == TRUE )
  {
    /* Only do the 'last write' here, since it guarantees that there
       was a 'first write' to initially grab the mutex */
    enc_write_data( ENC_CHAN_3, NULL, 0, TRUE );

    enc_sch_has_hdr_eram_mutex = FALSE;

    hdrl1_enc_eram_unlock();
  }
  #else /* !FEATURE_MODEM_1X_UTX */
  enc_write_data( ENC_CHAN_3, NULL, 0, TRUE );
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_sch_write_data_done */

/*===========================================================================

FUNCTION ENC_SCH_FTM_DATA_OUT

DESCRIPTION
  Send data to the encoder, generate a CRC (or not), and encode it at
  a specified rate.  For FTM mode only.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_sch_ftm_data_out
(
  byte *data_ptr,                /* Buffer to send to Encoder               */
  boolean generate_crc,          /* Have the Encoder generate a CRC, or not */
  enc_sch_rate_type rate,        /* Rate to encode the data as              */
  boolean turbo_code,            /* TRUE= Turbo, FALSE=Convolutional        */
  word ltu_size                  /* LTU block size with CRC in bits         */
)
{
  uint16 data_size;

  /*-----------------------------------------------------------------*/

  if (isValidRadioConfigType(enc_current_sch_rc) &&
      isValidRateType(rate))
  {
    /* Do the R-SCH write all in one shot here */
    data_size = enc_sch_frm_size_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate];
    enc_sch_data_out_cfg( rate, turbo_code, ltu_size, generate_crc );
    enc_sch_muxpdu_data_out( data_ptr, data_size );
    enc_sch_write_data_done();
  }
  else
  {
    M1X_MSG( ENC, LEGACY_ERROR,
      "Attempt to access enc_sch_frm_size_tab[%d][%d]",
      ENC_SCH_RC_INX(enc_current_sch_rc),
      rate);
  }

} /* enc_sch_ftm_data_out */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION ENC_REACH_DATA_OUT

DESCRIPTION
  Send data to the encoder, generate a CRC (or not), and encode it at
  a specified rate.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_reach_data_out
(
  byte *data_ptr,                /* Buffer to send to Encoder               */
  boolean generate_crc,          /* Have the Encoder generate a CRC, or not */
  enc_sch_rate_type rate,        /* Rate to encode the data as              */
  boolean turbo_code,            /* TRUE= Turbo, FALSE=Convolutional        */
  word ltu_size,                 /* LTU block size with CRC in bits         */
  word frame_size                /* Frame size                              */
)
{
  #ifdef FEATURE_MODEM_1X_UTX
  word enc_ctl;                 /* Encoder control value for this packet  */
  #else /* !FEATURE_MODEM_1X_UTX */
  cdmafw_rl_enc_ch3_cfg_t enc_ch3_cfg;
  #endif /* FEATURE_MODEM_1X_UTX */
  word data_size = 0;           /* Data size for this packet              */
  byte crc_len;
  word crc_poly = 0;
  enc_rate_set_type rate_set;

  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_IS2000_1X_ADV
  ONEX_ASSERT( ( enc_current_sch_rc == ENC_SCH_RC_3 ) ||
               ( enc_current_sch_rc == ENC_SCH_RC_8 ) );
  #else
  ONEX_ASSERT( enc_current_sch_rc == ENC_SCH_RC_3 );
  #endif /* FEATURE_IS2000_1X_ADV */

  ONEX_ASSERT( ( rate == ENC_SCH_FULL_RATE ) ||
               ( rate == ENC_SCH_2X_RATE   ) ||
               ( rate == ENC_SCH_4X_RATE   ) );

  /* Look up the data size in bytes.
  */
  data_size = enc_sch_frm_size_tab[ ENC_SCH_RC_INX( enc_current_sch_rc ) ][ rate ];

  rate_set = ENC_RATE_9600;

  if( generate_crc )
  {
    crc_len =
      (enc_sch_crc_info_tab [ENC_SCH_RC_INX(enc_current_sch_rc)][rate]).length;
    crc_poly =
      (enc_sch_crc_info_tab [ENC_SCH_RC_INX(enc_current_sch_rc)][rate]).poly;
    #ifdef FEATURE_MODEM_1X_UTX
    HWIO_OUT( MOD_CH3_CRC_POLY, crc_poly );
    #endif /* FEATURE_MODEM_1X_UTX */
  }
  else
  {
     crc_len = 0;
  }

  #ifdef FEATURE_MODEM_1X_UTX
  /* program puncture pattern */
  HWIO_OUT(
          MOD_CH3_PUNCT_PATN_0,
          enc_sch_punct_0_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate]
         );

  HWIO_OUT(
          MOD_CH3_PUNCT_PATN_1,
          enc_sch_punct_1_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate]
         );

  if( rate == ENC_SCH_32X_RATE )
  {
    enc_ctl = (word) (
                       ( (word) rate << ENC_SCH_RATE_POS ) |
                       ( (word) rate_set << ENC_SCH_RATE_SET_POS ) |
                       ( ( (word) ENC_CODE_1_BY_2_RATE )
                                  << ENC_SCH_CODE_RATE_POS ) |
                       ( ( (word) crc_len ) << ENC_SCH_CRCLEN_POS )
                     );
  }
  else
  {
    enc_ctl = (word) (
                       ( (word) rate << ENC_SCH_RATE_POS ) |
                       ( (word) rate_set << ENC_SCH_RATE_SET_POS ) |
                       ( ( (word) ENC_CODE_1_BY_4_RATE )
                                  << ENC_SCH_CODE_RATE_POS ) |
                       ( ( (word) crc_len ) << ENC_SCH_CRCLEN_POS )
                     );
  }

  /* set the rate, rate set, code rate and crc length */
  // enc_reach_on
  if (frame_size == ENC_SCH_FRAME_SIZE_20_V)
  {
    // 20 ms, 9600 bps, 19200 bps, 38400 bps data rate
    HWIO_OUTM
    (
      MOD_CH3_ENC_CTL_0,
      (ENC_SCH_CRCLEN_M|ENC_SCH_CODE_RATE_M|ENC_SCH_RATE_SET_M|ENC_SCH_DATA_RATE_M),
      enc_ctl
    );

  }
  else if (frame_size == ENC_SCH_FRAME_SIZE_10_V)
  {

    if (rate == ENC_SCH_FULL_RATE)
    {

      // 10 ms, 19200 bps data rate, 12 bits CRC, ENC_SCH_FULL_RATE
      HWIO_OUTM
      (
        MOD_CH3_ENC_CTL_0,
        (ENC_SCH_CRCLEN_M|ENC_SCH_CODE_RATE_M|ENC_SCH_RATE_SET_M|ENC_SCH_DATA_RATE_M),
        0x0C89
      );
    }
    else if (rate == ENC_SCH_2X_RATE)
    {
      /* 10 ms, 38400 bps data rate, 16 bits CRC, ENC_SCH_2X_RATE */
      HWIO_OUTM
      (
        MOD_CH3_ENC_CTL_0,
        (ENC_SCH_CRCLEN_M|ENC_SCH_CODE_RATE_M|ENC_SCH_RATE_SET_M|ENC_SCH_DATA_RATE_M),
        0x108B
      );

    } /* end if (rate == ENC_SCH_FULL_RATE) */
  }
  else if (frame_size == ENC_SCH_FRAME_SIZE_5_V)
  {

    // 5 ms, 38400 bps data rate, 12 bits CRC, ENC_SCH_FULL_RATE
    HWIO_OUTM
    (
      MOD_CH3_ENC_CTL_0,
      (ENC_SCH_CRCLEN_M|ENC_SCH_CODE_RATE_M|ENC_SCH_RATE_SET_M|ENC_SCH_DATA_RATE_M),
      0x0C8B
    );

  } /* end if (frame_size == ENC_SCH_FRAME_SIZE_20_V) */

  /*
    data rate 32X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_32X_V ENC_SCH_WALSHCOVER_1_V
    data rate 16X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_16X_V ENC_SCH_WALSHCOVER_1_V
    data rate  8X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_8X_V  ENC_SCH_WALSHCOVER_2_V
    data rate  4X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_4X_V  ENC_SCH_WALSHCOVER_2_V
    data rate  2X  ENC_SCH_LOW_REPEAT_1_V   ENC_SCH_INTLV_SIZE_2X_V  ENC_SCH_WALSHCOVER_2_V
    data rate  1X  ENC_SCH_LOW_REPEAT_2_V   ENC_SCH_INTLV_SIZE_1X_V  ENC_SCH_WALSHCOVER_2_V
    data rate 1/2  ENC_SCH_LOW_REPEAT_4_V   ENC_SCH_INTLV_SIZE_1X_V  ENC_SCH_WALSHCOVER_2_V
    data rate 1/4  ENC_SCH_LOW_REPEAT_8_V   ENC_SCH_INTLV_SIZE_1X_V  ENC_SCH_WALSHCOVER_2_V
    data rate 1/8  ENC_SCH_LOW_REPEAT_16_V  ENC_SCH_INTLV_SIZE_1X_V  ENC_SCH_WALSHCOVER_2_V
  */

  switch (rate)
  {
    case ENC_SCH_8TH_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_1X_V | ENC_SCH_LOW_REPEAT_16_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;
    case ENC_SCH_QTR_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_1X_V | ENC_SCH_LOW_REPEAT_8_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );

      break;
    case ENC_SCH_HALF_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_1X_V | ENC_SCH_LOW_REPEAT_4_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;
    case ENC_SCH_FULL_RATE:
      //if( enc_reach_on )
      {
        if (frame_size == ENC_SCH_FRAME_SIZE_20_V)
        {

          // 20ms, Data rate = 9600 bps use ENC_SCH_FULL_RATE information and crc
          //       txcaccess.c sets data rate ENC_SCH_FULL_RATE
          //
          enc_ctl = (word) ( ENC_SCH_SELECT_EACH_V | ENC_SCH_FRAME_SIZE_20_V |
                             ENC_SCH_INTLV_SIZE_4X_V | ENC_SCH_LOW_REPEAT_4_V |
                             ENC_SCH_MULTIFRAME_1_V
                           );
        }
        else if (frame_size == ENC_SCH_FRAME_SIZE_10_V)
        {

        // 10ms, Data rate = 19200 bps uses ENC_SCH_FULL_RATE information bits and crc
        //       txcaccess.c sets data raet ENC_SCH_FULL_RATE
        // 10ms frame size, low repeat = 2, interleave size = 1536
        // 9600 bps data rate
        enc_ctl = (word) ( ENC_SCH_SELECT_EACH_V | ENC_SCH_FRAME_SIZE_10_V |
                           ENC_SCH_INTLV_SIZE_1536_V | ENC_SCH_LOW_REPEAT_2_V |
                           ENC_SCH_MULTIFRAME_1_V
                         );

        }
        else if (frame_size == ENC_SCH_FRAME_SIZE_5_V)
        {

        // 5ms, Data rate = 38400 bps uses ENC_SCH_FULL_RATE information bits and crc
        //      txcaccess.c sets data rate ENC_SCH_FULL_RATE
        // 5ms frame size, low repeat = 1, interleave size = 768
        // 9600 bps data rate
        enc_ctl = (word) ( ENC_SCH_SELECT_EACH_V | ENC_SCH_FRAME_SIZE_5_V |
                           ENC_SCH_INTLV_SIZE_768_V | ENC_SCH_LOW_REPEAT_1_V |
                           ENC_SCH_MULTIFRAME_1_V
                         );

        } /* end if (frame_size == ENC_SCH_FRAME_SIZE_20_V) */
      }
      break;
    case ENC_SCH_2X_RATE:
      //if( enc_reach_on)
      {
        if (frame_size == ENC_SCH_FRAME_SIZE_20_V)
        {
          // 20ms, Data rate = 19200 bps use ENC_SCH_2X_RATE information and crc
          //       txcaccess.c sets data rate ENC_SCH_2X_RATE
          //
          enc_ctl = (word) ( ENC_SCH_SELECT_EACH_V | ENC_SCH_FRAME_SIZE_20_V |
                             ENC_SCH_INTLV_SIZE_4X_V | ENC_SCH_LOW_REPEAT_2_V |
                             ENC_SCH_MULTIFRAME_1_V
                           );
        }
        else if (frame_size == ENC_SCH_FRAME_SIZE_10_V)
        {
          // 10ms, Data rate = 38400 bps uses ENC_SCH_2X_RATE information bits and crc
          //       txcaccess.c sets data raet ENC_SCH_2X_RATE
          // 10ms frame size, low repeat = 1, interleave size = 1536
          // 19200 bps data rate
          enc_ctl = (word) ( ENC_SCH_SELECT_EACH_V | ENC_SCH_FRAME_SIZE_10_V |
                             ENC_SCH_INTLV_SIZE_1536_V | ENC_SCH_LOW_REPEAT_1_V |
                             ENC_SCH_MULTIFRAME_1_V
                           );
        } /* end if (frame_size == ENC_SCH_FRAME_SIZE_20_V) */
      }
      break;
    case ENC_SCH_4X_RATE:
      //if( enc_reach_on)
      {
        // 20ms, Data rate = 38400 bps use ENC_SCH_4X_RATE information and crc
        //       txcaccess.c sets data rate ENC_SCH_4X_RATE
        //
        enc_ctl = (word) ( ENC_SCH_SELECT_EACH_V | ENC_SCH_FRAME_SIZE_20_V |
                           ENC_SCH_INTLV_SIZE_4X_V | ENC_SCH_LOW_REPEAT_1_V |
                           ENC_SCH_MULTIFRAME_1_V
                         );
      }
      break;
    case ENC_SCH_8X_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_8X_V | ENC_SCH_LOW_REPEAT_1_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;

    /* program the same value to HWIO_ADDR(MOD_CH3_ENC_CTL_1) for rate = 16X or 32X. */
    case ENC_SCH_16X_RATE:
    case ENC_SCH_32X_RATE:
      enc_ctl = (word) ( ENC_SCH_SELECT_SCH_V | ENC_SCH_FRAME_SIZE_20_V |
                         ENC_SCH_INTLV_SIZE_16X_V | ENC_SCH_LOW_REPEAT_1_V |
                         ENC_SCH_MULTIFRAME_1_V
                       );
      break;

    default:
      ONEX_ERR_FATAL("Invalid R-SCH rate (%d)", rate, 0, 0);
  }

  /* set the radio configuration, 20 ms frame size */
  HWIO_OUT( MOD_CH3_ENC_CTL_1, enc_ctl );

  /* set the pcg count, encode time */
  HWIO_OUT( MOD_CH3_TIMING_CTL,
            (ENC_SCH_ENCODE_TIME_V | ENC_SCH_PCG_COUNT_V)
          );

  /*
  ** Extended supplemental channel assignment message
  ** specifies REV_WALSH_ID for supplemental channel.
  ** If only one reverse supplemental channel is assigned,
  ** the base station should set the default value.
  ** Set the default walshcover select depends on the
  ** interleaver size.
  */
  /*
     Program MOD_WALSHCOVER_SEL:CH3_WALSHCOVER_SEL with 1 when rate
     is equal to 16X or 32X.
  */
  if
  (
    (rate == ENC_SCH_16X_RATE) || (rate == ENC_SCH_32X_RATE)
  )
  {
     HWIO_OUTM( MOD_WCOVER_SEL,
                ENC_WALSHCOVER_SEL_SCH_M,
                ENC_WALSHCOVER_SEL_SCH_1_V
              );
  }
  else
  {
     HWIO_OUTM( MOD_WCOVER_SEL,
                ENC_WALSHCOVER_SEL_SCH_M,
                ENC_WALSHCOVER_SEL_SCH_2_V
              );
  }

  (void)enc_is95c_enable( TRUE ); // IS-95C
  #else /* !FEATURE_MODEM_1X_UTX */
  if ( frame_size != ENC_SCH_FRAME_SIZE_20_V )
  {
    ONEX_ERR_FATAL("Frame size other than 20ms no longer supported",0,0,0);
  }

  /* Populate Enc Ch3 Bit Field structure in FW shared memory */  /*  BITS    */
  enc_ch3_cfg.schEncRate   = ( rate & 0x1F );                     /*  4 :  0  */
  enc_ch3_cfg.schCodeRate  = ( ENC_CODE_1_BY_4_RATE & 0x3 );      /*  6 :  5  */
  enc_ch3_cfg.schCrcLength = ( crc_len & 0x1F );                  /* 11 :  7  */
  enc_ch3_cfg.turboEncode  = ( turbo_code & 0x1 );                /* 12 : 12  */
  enc_ch3_cfg.ch3FrameSize = ( frame_size & 0x3 );                /* 14 : 13  */
  enc_ch3_cfg.ch3Sel       = ( 0x3 & 0x7 ); /* b'011 : EACH */    /* 17 : 15  */
  enc_ch3_cfg.lowRepeat    =                                      /* 20 : 18  */
                          ( (uint32)enc_sch_intl_info[rate].lowRepeat & 0x7 );
  /* reserved bits */                                             /* 31 : 21  */
  enc_ch3_cfg.ch3CrcPolynomial = ( crc_poly & 0xFFFF );           /* 47 : 32  */
  enc_ch3_cfg.numIntlvSymbols  = ( 3072 & 0xFFFF );               /* 63 : 48  */
  enc_ch3_cfg.schPunctPattern23to16 =                             /* 71 : 64  */
    ( enc_sch_punct_1_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate] & 0xFF );
  enc_ch3_cfg.schPunctPattern15to0  =                             /* 87 : 72  */
    ( enc_sch_punct_0_tab[ENC_SCH_RC_INX(enc_current_sch_rc)][rate] & 0xFFFF );
  enc_ch3_cfg.eachEn       = 1;                                   /* 88 : 88  */
  enc_ch3_cfg.schIntlvSize = 1; /* always 3072 symbols */         /* 90 : 89  */
  /* reserved bits */                                             /* 95 : 91  */

  muxmdsp_set_enc_ch3_params(&enc_ch3_cfg);
  enc_set_mod_ch3_cfg(rate);
  muxmdsp_set_enc_input_len(ENC_CHAN_2,
          enc_sch_frm_size_tab[ ENC_SCH_RC_INX( enc_current_sch_rc ) ][ rate ]);

  /* save a copy of the most recent encoder config */
  rl_ch3_cfg = enc_ch3_cfg;
  #endif /* FEATURE_MODEM_1X_UTX */
  enc_sch_enable( TRUE ); // IS-95C

  if( turbo_code )
  {
    /* TODO: When the below reference code is removed, change this
       to a single condition check, i.e. if !turbo_code */
    #ifdef FEATURE_MODEM_1X_UTX
    // enc_turbo_code_enable( TRUE );
    HWIO_OUTM( MOD_CH3_ENC_CTL_0, ENC_TURBO_CODE_M, ENC_TURBO_CODE_V );
    #endif /* FEATURE_MODEM_1X_UTX */
  }
  else
  {
    #ifdef FEATURE_MODEM_1X_UTX
    // enc_turbo_code_enable( FALSE );
    HWIO_OUTM( MOD_CH3_ENC_CTL_0, ENC_TURBO_CODE_M, ENC_CONV_CODE_V );
    #endif /* FEATURE_MODEM_1X_UTX */

    if( ltu_size > 0 )
    {
      ONEX_ASSERT( ( ltu_size >= ENC_LTU_MIN ) && ( ltu_size <= ENC_LTU_MAX ) );
      enc_sch_ltu_size( ltu_size );
      // enc_sch_ltu_enable( TRUE );
      #ifdef FEATURE_MODEM_1X_UTX
      HWIO_OUTM( MOD_CH3_ENC_CTL_0, ENC_LTU_ENA_M, ENC_LTU_ENA_V );
      #endif /* FEATURE_MODEM_1X_UTX */
    }
    else
    {
      enc_sch_ltu_size( 0 );
      // enc_sch_ltu_enable( FALSE );
      #ifdef FEATURE_MODEM_1X_UTX
      HWIO_OUTM( MOD_CH3_ENC_CTL_0, ENC_LTU_ENA_M, ENC_LTU_DIS_V );
      #endif /* FEATURE_MODEM_1X_UTX */
    }
  }

  #ifdef FEATURE_MODEM_1X_UTX
  /* interlock w/ DO ERAM accesses */
  hdrl1_enc_eram_lock();

  /* Initialize the ERAM pointer and bank */
  enc_set_eram_bank( ENC_CHAN_3 );

  enc_write_data(ENC_CHAN_3,data_ptr,data_size,TRUE);

  /* un-interlock w/ DO ERAM accesses */
  hdrl1_enc_eram_unlock();
  #else /* !FEATURE_MODEM_1X_UTX */
  enc_write_data(ENC_CHAN_3,data_ptr,data_size,TRUE);
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_reach_data_out */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION ENC_STATUS_CHECK

DESCRIPTION
  Read the status of the encoder circuit, and tries to correct any faults.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_status_check( void )
{
  static uint8 tx_fr_count = 0;

  /*-----------------------------------------------------------------*/

  /* Dump out Tx stats every N frames */
  if (++tx_fr_count == 0)
  {
    /* Dump out Tx timing info every 256 frames (5.12s) */
    muxmdsp_print_tx_timing_info( FALSE );
  }

  return;
} /* enc_status_check */

/*===========================================================================

FUNCTION      ENC_CHANGE_TX_CLK_RESOURCES

DESCRIPTION
              This function is used to enable/disable the clock resources
              required by 1X for TX via the m1xrm driver. This
              function requests the necessary clock resources by voting
              for a particular clock state. Conversely, this function
              can be used to vote off the resources.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If state = ENC_TX_CLOCKS_ENABLE:  TX clock resources will be voted on.
  If state = ENC_TX_CLOCKS_DISABLE:  TX will vote for INACTIVE clocks.
===========================================================================*/
void enc_change_tx_clk_resources
(
  enc_tx_clock_states_type state
)
{
  static enc_tx_clock_states_type previous_state = ENC_TX_CLOCKS_DISABLE;

  /*-----------------------------------------------------------------*/

  switch( state )
  {
    case ENC_TX_CLOCKS_ENABLE:
      m1x_rm_set_state( M1X_RM_TX, M1X_RM_1X_VOICE );
      previous_state = state;
      break;

    case ENC_TX_CLOCKS_DISABLE:
      /* need to go through idle first, if already enabled */
      if ( previous_state == ENC_TX_CLOCKS_ENABLE )
      {
        m1x_rm_set_state( M1X_RM_TX, M1X_RM_1X_IDLE );
      }
      m1x_rm_set_state( M1X_RM_TX, M1X_RM_1X_INACTIVE );
      previous_state = state;
      break;

    default:
      ONEX_ERR_FATAL("Invalid tx clock state %d", state, 0, 0);
      break;
  }
} /* enc_change_tx_clk_resources */

/*===========================================================================

FUNCTION ENC_PN_RAND_DELAY

DESCRIPTION
  Delay the transmit time by a specified number of chips.  This value is
  loaded internally by the Interleaver at the next SYNC80M, and takes effect
  at the successive internal 80 millisecond boundary (the BTF roll-over).

  For CDMA transmit PN randomization, call this routine with the number of
  chips of delay desired.  Call it again with 0 to return to non-delayed
  transmit frame timing.

DEPENDENCIES
  None

RETURN VALUE
  Number of chips from the previous call to enc_pn_rand_delay.

SIDE EFFECTS
  None

===========================================================================*/
word enc_pn_rand_delay
(
  word chips        /* Chip delay for transmit frame timing */
)
{
  word old_delay = (word)(enc_pn_rand_delay_chipx8 >> 3);

  /*-----------------------------------------------------------------*/

  /* BTF is advancement of transmit frame timing in units of chipx8.
  ** We delay by decreasing the advancement.
  */

  /* Convert the chip delay to a chipx8 delay */
  enc_pn_rand_delay_chipx8 = (dword)(chips << 3);

  return( old_delay );

} /* enc_pn_rand_delay */

/*===========================================================================

FUNCTION ENC_SCH_LTU_SIZE

DESCRIPTION
   Set supplemental channel LTU size.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_sch_ltu_size
(
   word sch_ltu_size        /* 32 <= ltu_size < 512 bits */
)
{
  /*-----------------------------------------------------------------*/

  /* Update the LTU size variable, to be used on the fly when writing
     to ERAM for CH3.  Convert from bits to bytes and subtract 2 for the
     CRC16 bytes that will be tacked on during writing.  A size of zero
     disables LTU CRC generation. */
  if ( sch_ltu_size == 0 )
  {
    /* Disable LTU CRC generation */
    enc_sch_ltu_size_bytes = 0;
  }
  else
  {
    /* Convert from bits to bytes and subtract the 2 bytes of CRC16 itself */
    enc_sch_ltu_size_bytes = (sch_ltu_size / 8) - 2;

    /* Assert that LTU size is at least 16-bit aligned */
    if ( enc_sch_ltu_size_bytes & 1 )
    {
      ONEX_ERR_FATAL("LTU size not 16-bit aligned (%u)",enc_sch_ltu_size_bytes,0,0);
    }
  }

} /* enc_sch_ltu_size */

/*===========================================================================

FUNCTION ENC_IS95C_ENABLE

DESCRIPTION
  Turns the IS95C bit of FCH_CTL register on/off

DEPENDENCIES

RETURN VALUE
   0  ==  success
  -1  ==  failure to switch reliably

SIDE EFFECTS

===========================================================================*/
int enc_is95c_enable
(
  boolean is95c
)
{
  int failure_code = 0;

  #ifdef FEATURE_MODEM_1X_UTX
  /* Update the encoder CH2 IS95C bit first */
  HWIO_OUTF( TX_1X_CH2_ENC_CTL_0, IS_95_C, is95c ? 1 : 0 );
  HWIO_OUTF( TX_C2K_MODE, C2K_IS95A_MODE, is95c ? 0 : 1 );
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_get_enc_ch2_params(&rl_ch2_cfg);
  rl_ch2_cfg.fchIs95cEn = is95c;
  muxmdsp_set_enc_ch2_params(&rl_ch2_cfg);
  #endif /* FEATURE_MODEM_1X_UTX */

  return( failure_code );
}

/*===========================================================================

FUNCTION ENC_FCH_ENABLE

DESCRIPTION
  Turns the FCH_ENABLE bit of FCH_CTL register on/off

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_fch_enable
(
   boolean enable_fch
)
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  HWIO_OUTF( TX_1X_CH2_ENC_CTL_0, FCH_EN, enable_fch ? 1 : 0 );
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_enc_ch_en(ENC_CHAN_2, enable_fch);
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_fch_enable */

/*===========================================================================

FUNCTION ENC_SCH_ENABLE

DESCRIPTION
  Turns the SCH_ENABLE bit of SCH_CTL register on/off

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_sch_enable
(
   boolean enable_sch
)
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  HWIO_OUTF( TX_1X_CH3_ENC_CTL_0, SCH_EN, enable_sch ? 1 : 0 );
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_enc_ch_en(ENC_CHAN_3, enable_sch);
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_sch_enable */

/*===========================================================================

FUNCTION ENC_PCH_ENABLE

DESCRIPTION
  Turns the PCH_ENABLE bit of MISC_CTL register on/off

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_pch_enable
(
   boolean enable_pch
)
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  HWIO_OUTF( TX_1X_MISC_CTL, C2K_PCH_EN, enable_pch ? 1 : 0 );
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_set_tx_pcg_mask( enable_pch ?
                           current_tx_pattern : TX_PCG_MASK_DISABLED );
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_pch_enable */

/*===========================================================================

FUNCTION ENC_PCBIT_TEST_MASK

DESCRIPTION
   Sets the mask in MOD_PCBIT_TEST_MASK register. When a bit in this register
   is set to 1 the corresponding bit of MOD_PCBIT_TEST_DATA register is sent
   as power control bit on the reverse pilot channel.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_pcbit_test_mask
(
  word pcbit_test_mask
)
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  HWIO_OUT( TX_1X_PICH_PCBIT_MASK, pcbit_test_mask );
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_pcbit_test_mask */

/*===========================================================================

FUNCTION ENC_PCBIT_TEST_DATA

DESCRIPTION
   Sets the contents of MOD_PCBIT_TEST_DATA

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_pcbit_test_data
(
   word pcbit_test_data
)
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  HWIO_OUT( TX_1X_PICH_PCBIT_DATA, pcbit_test_data );
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_pcbit_test_data */

/*===========================================================================

FUNCTION ENC_FCH_RC

DESCRIPTION
  Set the reverse fundamental channel radio configuration

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_fch_rc
(
   enc_fch_radio_config_type fch_rc
)
{
  /*-----------------------------------------------------------------*/

   enc_current_fch_rc = fch_rc;
} /* enc_fch_rc */

/*===========================================================================

FUNCTION ENC_DCCH_RC

DESCRIPTION
  Set the reverse fundamental channel radio configuration

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_dcch_rc
(
  enc_fch_radio_config_type dcch_rc
)
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_IS2000_REL_B
  enc_current_dcch_rc = dcch_rc;
  #else
  enc_current_fch_rc = dcch_rc;
  #endif /* FEATURE_IS2000_REL_B */
} /* enc_dcch_rc */

/*===========================================================================

FUNCTION ENC_SCH_RC

DESCRIPTION
  Set the reverse supplemental channel radio configuration

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void enc_sch_rc
(
  enc_sch_radio_config_type sch_rc
)
{
  /*-----------------------------------------------------------------*/

  enc_current_sch_rc = sch_rc;
} /* enc_sch_rc */

/*===========================================================================

FUNCTION isValidRadioConfigType

DESCRIPTION
  Check enc_srch_radio_config_type is a valid value

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static boolean isValidRadioConfigType(enc_sch_radio_config_type type)
{
  /*-----------------------------------------------------------------*/

  if (type == ENC_SCH_RC_3 ||
      type == ENC_SCH_RC_4 ||
      type == ENC_SCH_RC_8)
  {
    return TRUE;
  }
  return FALSE;
} /* isValidRadioConfigType */

/*===========================================================================

FUNCTION isValidRateType

DESCRIPTION
  Check enc_sch_rate_type is a valid value

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static boolean isValidRateType(enc_sch_rate_type type)
{
  /*-----------------------------------------------------------------*/

  if (type >= 0 && type <= ENC_NUM_SCH_RATES)
  {
    return TRUE;
  }
  return FALSE;
} /* isValidRateType */

/*===========================================================================

FUNCTION ENC_IS95C_TX_PATN

DESCRIPTION
   Program the IS95C_TX_PATN register

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_is95c_tx_patn( enc_gating_pattern_type pattern )
{
  uint32 tx_pattern = 0;

  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  switch ( pattern )
  {
    case ENC_HALF_RATE_GATING_PATTERN: /* tx in slots 1,3,5,7,9,11,13,15*/
      tx_pattern = 1;
      break;

    case ENC_QTR_RATE_GATING_PATTERN: /* tx in slots 3,7,11,15 */
      tx_pattern = 2;
      break;

    case ENC_FCH_8TH_RATE_GATING_PATTERN: /* tx in slots 7,15 */
      tx_pattern = 3;
      break;

    case ENC_NO_GATING_PATTERN: /* tx in all slots */
    default:
      tx_pattern = 0;
      break;
  }

  HWIO_OUTF( TX_1X_MISC_CTL, C2K_PCH_GATING_RATE, tx_pattern );
  #else /* !FEATURE_MODEM_1X_UTX */
  MODEM_1X_UNUSED(tx_pattern);
  muxmdsp_set_tx_pcg_mask( (uint32)pattern );
  current_tx_pattern = pattern;
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_is95c_tx_patn */

/*===========================================================================

FUNCTION ENC_GATING_MODE

DESCRIPTION
   Program the MOD_MISC_CTL register with one of 3 mutually exclusive pilot
   gating modes: PILOT_GATING_MODE, FCH_8TH_RATE_GATING_MODE or
   HHO_PREAMBLE_MODE, NO_GATING_MODE.

DEPENDENCIES
   The gating pattern must be specified using enc_is95c_tx_patn();

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_gating_mode( enc_gating_mode_type gating_mode )
{
  /*-----------------------------------------------------------------*/

  switch( gating_mode )
  {
    case ENC_NO_GATING_MODE:
      /* No pilot gating, No 1/8 rate FCH gating */
      enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );
      #ifdef FEATURE_MODEM_1X_UTX
      HWIO_OUTF( TX_1X_MISC_CTL, C2K_FCH_EIGHTH_GATE, 0 );
      #endif /* FEATURE_MODEM_1X_UTX */

      enc_pilot_gating = FALSE;
      break;

    case ENC_PILOT_GATING_MODE:
      /* Pilot gating */
      enc_pilot_gating = TRUE;
    break;

    case ENC_FCH_8TH_RATE_GATING_MODE:
      /* No pilot gating, 1/8 rate FCH gating */
      #ifdef FEATURE_MODEM_1X_UTX
      HWIO_OUTF( TX_1X_MISC_CTL, C2K_FCH_EIGHTH_GATE, 1 );
      #endif /* FEATURE_MODEM_1X_UTX */

      enc_pilot_gating = FALSE;
    break;

    case ENC_HHO_PREAMBLE_MODE:
    default:
      /* No pilot gating */
      enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );

      enc_pilot_gating = FALSE;
    break;

  }
} /* enc_gating_mode */

/*===========================================================================

FUNCTION ENC_DCCH_ENABLE

DESCRIPTION
  Turns the DCCH_ENABLE bit of MOD_MISC_CTL register on/off

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_dcch_enable( boolean dcch_en )
{
  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_UTX
  HWIO_OUTF( TX_1X_CH1_ENC_CTL, CH1_EN, dcch_en ? 1 : 0 );
  #else /* !FEATURE_MODEM_1X_UTX */
  muxmdsp_enc_ch_en(ENC_CHAN_1, dcch_en);
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_dcch_enable */

/*===========================================================================

FUNCTION ENC_DCCH_DATA_OUT

DESCRIPTION
  Send DCCH data to the encoder, generate a CRC (or not), and encode it at
  a specified rate.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_dcch_data_out
(
  byte *data_ptr,               /* Buffer to send to Encoder               */
  boolean generate_crc,         /* Have the Encoder generate a CRC, or not */
  enc_rate_type rate            /* Rate to encode the data as              */
)
{
  byte rc_index;
  #ifndef FEATURE_MODEM_1X_UTX
  word crc_poly;
  byte crc_len;
  cdmafw_rl_enc_ch1_cfg_t enc_ch1_cfg;
  #endif /* !FEATURE_MODEM_1X_UTX */

  #ifdef FEATURE_IS2000_REL_B
  enc_fch_radio_config_type enc_current_rc = enc_current_dcch_rc;
  #else
  enc_fch_radio_config_type enc_current_rc = enc_current_fch_rc;
  #endif /* FEATURE_IS2000_REL_B */

  /*-----------------------------------------------------------------*/

  MODEM_1X_UNUSED(rate);

  ONEX_ASSERT( ( enc_current_rc >= ENC_FCH_RC_3 ) &&
               ( enc_current_rc <= ENC_FCH_RC_4 ) );

  rc_index = (ENC_FCH_RC_3 == enc_current_rc) ? 0 : 1;

  #ifdef FEATURE_MODEM_1X_UTX
  if ( generate_crc )
  {
    HWIO_OUT( TX_1X_CH1_CRC_POLY, enc_dcch_config.crc_poly );
  }

  /* program puncture patterns */
  HWIO_OUT( TX_1X_CH1_PUNCT_PATN_0, enc_dcch_config.punct_patn_0[rc_index] );
  HWIO_OUT( TX_1X_CH1_PUNCT_PATN_1, enc_dcch_config.punct_patn_1[rc_index] );

  /* Program ENC_CTL for ENC CH1 */
  HWIO_OUTM( TX_1X_CH1_ENC_CTL,
             HWIO_FMSK(TX_1X_CH1_ENC_CTL, CH1_CRC_LEN)   |
             HWIO_FMSK(TX_1X_CH1_ENC_CTL, CH1_CODE_RATE) |
             HWIO_FMSK(TX_1X_CH1_ENC_CTL, CH1_RADIO_CFG) |
             HWIO_FMSK(TX_1X_CH1_ENC_CTL, CH1_ENC_RATE)  |
             HWIO_FMSK(TX_1X_CH1_ENC_CTL, CH1_FRAME_SIZE),
             HWIO_FVAL(TX_1X_CH1_ENC_CTL,
                       CH1_CRC_LEN,
                       enc_dcch_config.crc_len[(generate_crc) ? 1 : 0]) |
             HWIO_FVAL(TX_1X_CH1_ENC_CTL,
                       CH1_CODE_RATE,
                       enc_dcch_config.code_rate) |
             HWIO_FVAL(TX_1X_CH1_ENC_CTL,
                       CH1_RADIO_CFG,
                       enc_current_rc) |
             HWIO_FVAL(TX_1X_CH1_ENC_CTL,
                       CH1_ENC_RATE,
                       enc_dcch_config.enc_rate[rc_index]) |
             HWIO_FVAL(TX_1X_CH1_ENC_CTL,
                       CH1_FRAME_SIZE,
                       enc_dcch_config.frame_size) );

  /* set the walshcover select */
  HWIO_OUTM( TX_1X_MOD_WCOVER_SEL,
             ENC_WALSHCOVER_SEL_DCCH_M,
             ENC_WALSHCOVER_SEL_DCCH_V );

  (void)enc_is95c_enable( TRUE );
  enc_pch_enable( TRUE );

  /* interlock w/ DO ERAM accesses */
  hdrl1_enc_eram_lock();

  /* Initialize the ERAM pointer and bank */
  enc_set_eram_bank( ENC_CHAN_1 );

  /* Send the data to the encoder */
  /* Note: the modulator needs to have 32 bit aligned data_size. 24 or 36 */
  enc_write_data( ENC_CHAN_1,
                  data_ptr,
                  enc_dcch_config.data_size[rc_index],
                  TRUE );

  /* un-interlock w/ DO ERAM accesses */
  hdrl1_enc_eram_unlock();
  #else /* !FEATURE_MODEM_1X_UTX */
  if ( generate_crc )
  {
    crc_poly = ( enc_dcch_config.crc_poly );
    crc_len  = enc_dcch_config.crc_len[1];
  }
  else
  {
    crc_poly = 0;
    crc_len  = enc_dcch_config.crc_len[0];

  }

  muxmdsp_get_enc_ch1_params(&enc_ch1_cfg);
  /* Populate Enc Ch2 Bit Field structure in FW shared memory */  /*  BITS    */
  enc_ch1_cfg.ch1FrameSize = ( enc_dcch_config.frame_size & 0x3 );/*  1 :  0  */
  enc_ch1_cfg.ch1EncRate   =                                      /*  2 :  2  */
              ( enc_dcch_config.enc_rate[rc_index] & 0x1 );
  enc_ch1_cfg.ch1RadioCfg  = ( enc_current_rc & 0x7 );            /*  5 :  3  */
  enc_ch1_cfg.ch1CodeRate  = ( enc_dcch_config.code_rate & 0x3 ); /*  7 :  6  */
  enc_ch1_cfg.ch1CrcLen    = ( crc_len & 0x1F );                  /* 12 :  8  */
  enc_ch1_cfg.ch1CrcPolynomial = ( crc_poly & 0xFFFF );           /* 28 : 13  */
  /* reserved bits */                                             /* 31 : 29  */
  enc_ch1_cfg.dcchPunctPattern23to16 =                            /* 39 : 32  */
              ( enc_dcch_config.punct_patn_1[rc_index] & 0xFF );
  enc_ch1_cfg.dcchPunctPattern15to0  =                            /* 55 : 40  */
              ( enc_dcch_config.punct_patn_0[rc_index] & 0xFFFF );
  /* reserved bits */                                             /* 63 : 56  */
  /* DCCH interleaver size is always 1536 */
  enc_ch1_cfg.numIntlvSymbols = ( 1536 & 0xFFFF );                /* 79 : 64  */
  /* reserved bits */                                             /* 95 : 80  */

  muxmdsp_set_enc_ch1_params(&enc_ch1_cfg);

  /* From BOLT CDMA Reverse Link FW Interface - struct cdmafw_mod_channel_cfg_t
     Modulation: 31 => MULTI_BPSK_ON_I  (CH1)
     OVS Num: 1 : CH1, CH3_SF2_SCH, CH3_SF4_SCH
     Spreading Factor: 2   => SF16 */
  muxmdsp_set_mod_ch_cfg(ENC_CHAN_1, ENC_OVSF_1, ENC_SF_16, MULTI_BPSK_ON_I);
  muxmdsp_set_enc_input_len( ENC_CHAN_1, enc_dcch_config.data_size[rc_index] );

  /* save a copy of the most recent encoder config */
  rl_ch1_cfg = enc_ch1_cfg;

  /* Send the data to the encoder */
  /* Note: the modulator needs to have 32 bit aligned data_size. 24 or 36 */
  enc_write_data( ENC_CHAN_1,
                  data_ptr,
                  enc_dcch_config.data_size[rc_index],
                  TRUE );
  #endif /* FEATURE_MODEM_1X_UTX */
} /* enc_dcch_data_out */

/*===========================================================================

FUNCTION ENC_LTU_CRC16_STEP

DESCRIPTION
  This function is used to calculate the 16-bit convolution SCH LTU CRC
  specified in 3GPP2 C.S0003 2.2.1.1.1.3.4.1.1 LTU Assembly which then
  refers to C.S0002 2.1.3.1.4.1 Generation of Frame Quality Indicator Bits.
  1) CRC polynomial is 0xC867, computed bitwise big-endian (per octet)
  2) 16-bit CRC result is inserted big-endian (bytewise & bitwise)

DEPENDENCIES
  None

RETURN VALUE
  New CRC16 value, considering the initial seed value and number of bytes
  processed

SIDE EFFECTS
  None

===========================================================================*/
static uint16 enc_ltu_crc16_step( uint16 crc16, uint8 *pbuffer, uint32 size )
{

  /* Use the following perl script to generate the CRC table:

        my $poly = 0xC867;
        print "\n  static const uint16 enc_ltu_crc16_table[ 256 ] =\n    { ";
        for (my $i=0;$i<256;$i++)
        {
          my $crc16=$i << 8;
          for (my $j=0;$j<8;$j++)
          {
            $crc16 = ($crc16 & 0x8000) ?
                     ($poly ^ ($crc16 << 1)) : ($crc16 << 1);
          }
          printf("0x%04X",$crc16 & 0xFFFF);
          ($i < 255) ? print "," : print " };\n";
          print "\n      " if (($i+1)%10 == 0);
        }
  */
  static const uint16 enc_ltu_crc16_table[ 256 ] =
    { 0x0000,0xC867,0x58A9,0x90CE,0xB152,0x7935,0xE9FB,0x219C,0xAAC3,0x62A4,
      0xF26A,0x3A0D,0x1B91,0xD3F6,0x4338,0x8B5F,0x9DE1,0x5586,0xC548,0x0D2F,
      0x2CB3,0xE4D4,0x741A,0xBC7D,0x3722,0xFF45,0x6F8B,0xA7EC,0x8670,0x4E17,
      0xDED9,0x16BE,0xF3A5,0x3BC2,0xAB0C,0x636B,0x42F7,0x8A90,0x1A5E,0xD239,
      0x5966,0x9101,0x01CF,0xC9A8,0xE834,0x2053,0xB09D,0x78FA,0x6E44,0xA623,
      0x36ED,0xFE8A,0xDF16,0x1771,0x87BF,0x4FD8,0xC487,0x0CE0,0x9C2E,0x5449,
      0x75D5,0xBDB2,0x2D7C,0xE51B,0x2F2D,0xE74A,0x7784,0xBFE3,0x9E7F,0x5618,
      0xC6D6,0x0EB1,0x85EE,0x4D89,0xDD47,0x1520,0x34BC,0xFCDB,0x6C15,0xA472,
      0xB2CC,0x7AAB,0xEA65,0x2202,0x039E,0xCBF9,0x5B37,0x9350,0x180F,0xD068,
      0x40A6,0x88C1,0xA95D,0x613A,0xF1F4,0x3993,0xDC88,0x14EF,0x8421,0x4C46,
      0x6DDA,0xA5BD,0x3573,0xFD14,0x764B,0xBE2C,0x2EE2,0xE685,0xC719,0x0F7E,
      0x9FB0,0x57D7,0x4169,0x890E,0x19C0,0xD1A7,0xF03B,0x385C,0xA892,0x60F5,
      0xEBAA,0x23CD,0xB303,0x7B64,0x5AF8,0x929F,0x0251,0xCA36,0x5E5A,0x963D,
      0x06F3,0xCE94,0xEF08,0x276F,0xB7A1,0x7FC6,0xF499,0x3CFE,0xAC30,0x6457,
      0x45CB,0x8DAC,0x1D62,0xD505,0xC3BB,0x0BDC,0x9B12,0x5375,0x72E9,0xBA8E,
      0x2A40,0xE227,0x6978,0xA11F,0x31D1,0xF9B6,0xD82A,0x104D,0x8083,0x48E4,
      0xADFF,0x6598,0xF556,0x3D31,0x1CAD,0xD4CA,0x4404,0x8C63,0x073C,0xCF5B,
      0x5F95,0x97F2,0xB66E,0x7E09,0xEEC7,0x26A0,0x301E,0xF879,0x68B7,0xA0D0,
      0x814C,0x492B,0xD9E5,0x1182,0x9ADD,0x52BA,0xC274,0x0A13,0x2B8F,0xE3E8,
      0x7326,0xBB41,0x7177,0xB910,0x29DE,0xE1B9,0xC025,0x0842,0x988C,0x50EB,
      0xDBB4,0x13D3,0x831D,0x4B7A,0x6AE6,0xA281,0x324F,0xFA28,0xEC96,0x24F1,
      0xB43F,0x7C58,0x5DC4,0x95A3,0x056D,0xCD0A,0x4655,0x8E32,0x1EFC,0xD69B,
      0xF707,0x3F60,0xAFAE,0x67C9,0x82D2,0x4AB5,0xDA7B,0x121C,0x3380,0xFBE7,
      0x6B29,0xA34E,0x2811,0xE076,0x70B8,0xB8DF,0x9943,0x5124,0xC1EA,0x098D,
      0x1F33,0xD754,0x479A,0x8FFD,0xAE61,0x6606,0xF6C8,0x3EAF,0xB5F0,0x7D97,
      0xED59,0x253E,0x04A2,0xCCC5,0x5C0B,0x946C };

  /*-----------------------------------------------------------------*/

  /* Iterate on the input bytes until complete */
  for ( ; (pbuffer != NULL) && (size > 0) ; ++pbuffer, --size )
  {
    /* Look up the byte in the CRC table and mix it into the CRC */
    crc16 = enc_ltu_crc16_table[ (uint8)(crc16 >> 8) ^ *pbuffer ] ^
            (crc16 << 8);
  }

  /* Return the updated CRC */
  return( crc16 );
} /* enc_ltu_crc16_step */

/*===========================================================================

FUNCTION ENC_WRITE_DATA_CH3

DESCRIPTION
  This function is used to write data into the encoder ch3.
  For Triton, it also inserts convolutional LTU CRCs as needed.
  Turbo CH3 writes ignore/skip the CRC process.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 None.

===========================================================================*/
static void enc_write_data_ch3
(
  byte                  *src_data_ptr,     /* pointer to source buffer */
  uint16                size,              /* number of bytes available */
  boolean               last_write         /* is this the final write? */
)
{
  /* LTU state variables and FIFO structure */
  static uint32 ltu_count_bytes = 0;
  /* Only Bolt/Triton needs the SW-based convolutional LTU CRC support */
  static uint16 ltu_crc16 = 0xFFFF;
  #ifndef FEATURE_MODEM_1X_UTX
  volatile static uint32 * enc_lmem_addr = NULL;
  /* Variables to handle overflow of buffers */
  static uint32 pdu_byte_count = 0;
  uint32 rem_pad_byte_count = 0;
  #endif /* !FEATURE_MODEM_1X_UTX */

  /*-----------------------------------------------------------------*/

  /* Utility macros to make the code below more readable */
  #define FIFO_INIT_CRC16_AND_LTU()                                         \
    ltu_crc16 = 0xFFFF;                                                     \
    ltu_count_bytes = 0;

  /* FIFO structure, size must be power of two sized in uint8's */
  #define FIFO_SIZE 16
  static struct fifo_s
  {
    uint8  buffer[FIFO_SIZE];
    unsigned int read_index, write_index;
  } fifo = { { 0 }, 0 , 0 };

  /* FIFO macros to make usage easy/consistent below */
  #define FIFO_SIZE8( )                                                     \
    ( (fifo.write_index - fifo.read_index) & (FIFO_SIZE - 1) )
  #define FIFO_PUSH8( data8 )                                               \
    do {                                                                    \
      fifo.buffer[ fifo.write_index++ ] = (uint8)(data8);                   \
      fifo.write_index &= (FIFO_SIZE - 1);                                  \
    } while ( 0 )
  #define FIFO_PULL8( data8 )                                               \
    do {                                                                    \
      data8 = (uint8)fifo.buffer[ fifo.read_index++ ];                      \
      fifo.read_index &= (FIFO_SIZE - 1);                                   \
    } while ( 0 )
  #define FIFO_PUSH16( data16 )                                             \
    do {                                                                    \
      fifo.buffer[ fifo.write_index++ ] = (uint8)(data16);                  \
      fifo.write_index &= (FIFO_SIZE - 1);                                  \
      fifo.buffer[ fifo.write_index++ ] = (uint8)((data16) >> 8);           \
      fifo.write_index &= (FIFO_SIZE - 1);                                  \
    } while ( 0 )
  #define FIFO_PULL16( data16 )                                             \
    do {                                                                    \
      data16 = fifo.buffer[ fifo.read_index++ ];                            \
      fifo.read_index &= (FIFO_SIZE - 1);                                   \
      data16 |= (uint16)fifo.buffer[ fifo.read_index++ ] << 8;              \
      fifo.read_index &= (FIFO_SIZE - 1);                                   \
    } while ( 0 )
  #define FIFO_PUSH32( data32 )                                             \
    do {                                                                    \
      fifo.buffer[ fifo.write_index++ ] = (uint8)(data32);                  \
      fifo.write_index &= (FIFO_SIZE - 1);                                  \
      fifo.buffer[ fifo.write_index++ ] = (uint8)((data32) >> 8);           \
      fifo.write_index &= (FIFO_SIZE - 1);                                  \
      fifo.buffer[ fifo.write_index++ ] = (uint8)((data32) >> 16);          \
      fifo.write_index &= (FIFO_SIZE - 1);                                  \
      fifo.buffer[ fifo.write_index++ ] = (uint8)((data32) >> 24);          \
      fifo.write_index &= (FIFO_SIZE - 1);                                  \
    } while ( 0 )
  #define FIFO_PULL32( data32 )                                             \
    do {                                                                    \
      data32 = fifo.buffer[ fifo.read_index++ ];                            \
      fifo.read_index &= (FIFO_SIZE - 1);                                   \
      data32 |= (uint32)fifo.buffer[ fifo.read_index++ ] << 8;              \
      fifo.read_index &= (FIFO_SIZE - 1);                                   \
      data32 |= (uint32)fifo.buffer[ fifo.read_index++ ] << 16;             \
      fifo.read_index &= (FIFO_SIZE - 1);                                   \
      data32 |= (uint32)fifo.buffer[ fifo.read_index++ ] << 24;             \
      fifo.read_index &= (FIFO_SIZE - 1);                                   \
    } while ( 0 )

  /* Note the endianness swap here, to ensure big-endian insertion */
  #define FIFO_PUSH_CRC16_AND_INIT_LTU()                                    \
    FIFO_PUSH16( (ltu_crc16 >> 8) | (ltu_crc16 << 8) );                     \
    FIFO_INIT_CRC16_AND_LTU()

  #define FIFO_PUSH_AND_CRC_DATA8( data8 )                                  \
    if ( enc_sch_ltu_size_bytes != 0 ) {                                    \
      uint8 temp = (data8);                                                 \
      ltu_crc16 = enc_ltu_crc16_step( ltu_crc16, &temp, 1 );                \
      ltu_count_bytes += 1;                                                 \
      FIFO_PUSH8( temp );                                                   \
    } else FIFO_PUSH8( (data8) )

  #define FIFO_PUSH_AND_CRC_DATA16( data16 )                                \
    if ( enc_sch_ltu_size_bytes != 0 ) {                                    \
      uint16 temp = (data16);                                               \
      ltu_crc16 = enc_ltu_crc16_step( ltu_crc16, (uint8*)&temp, 2 );        \
      ltu_count_bytes += 2;                                                 \
      FIFO_PUSH16( temp );                                                  \
    } else FIFO_PUSH16( (data16) )

  #define FIFO_PUSH_AND_CRC_DATA32( data32 )                                \
    if ( enc_sch_ltu_size_bytes != 0 ) {                                    \
      uint32 temp = (data32);                                               \
      ltu_crc16 = enc_ltu_crc16_step( ltu_crc16, (uint8*)&temp, 4 );        \
      ltu_count_bytes += 4;                                                 \
      FIFO_PUSH32( temp );                                                  \
    } else FIFO_PUSH32( (data32) )

  /* Triton specific ERAM HWIO writes */
  #ifdef FEATURE_MODEM_1X_UTX
  #define WRITE_ERAM_32( data32 )                                           \
    HWIO_OUT( TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0, (data32) )

  #define TRIGGER_ENC_CH3()                                                 \
    HWIO_OUT( TX_1X_ENC_TRIGGER, (uint32)(1 << ENC_CHAN_3) )
  #else /* !FEATURE_MODEM_1X_UTX */
  #define WRITE_ERAM_32( data32 )                                           \
    do {                                                                    \
      *enc_lmem_addr++ = data32;                                            \
    } while ( 0 )

  #define TRIGGER_ENC_CH3()                                                 \
    enc_lmem_addr = NULL

  #endif /* FEATURE_MODEM_1X_UTX */

  /* Featurized like this to make removal of legacy code easier */
  #ifndef FEATURE_MODEM_1X_UTX
  if ( enc_lmem_addr == NULL )
  {
    if ( NULL == ( enc_lmem_addr = muxmdsp_get_lmem_ch_addr(ENC_CHAN_3) ) )
    {
      ONEX_ERR_FATAL("Enc. CH %u LMEM addr is NULL", ( ENC_CHAN_3 + 1 ), 0, 0);
    }
  }

  /* check for lmem overwrite */
  if ( enc_lmem_addr > ( muxmdsp_get_lmem_ch_addr(ENC_CHAN_3) +
                         muxmdsp_get_enc_input_len(ENC_CHAN_3) ) )
  {
    ONEX_ERR_FATAL("Enc LMEM overwrite: 0x%X > 0x%X",
              (uint32)enc_lmem_addr,
              (uint32)( muxmdsp_get_lmem_ch_addr(ENC_CHAN_3) +
                        muxmdsp_get_enc_input_len(ENC_CHAN_3) ), 0);
  }

  /* Accumulate the PDU bytes */
  pdu_byte_count = pdu_byte_count + size;

  M1X_MSG( ENC, LEGACY_MED,
      "DEBUG : PDU byte %d size %d",pdu_byte_count,size);

  /* Check if PDU bytes crosses LMEM limit */
  if( pdu_byte_count > ( ( muxmdsp_get_enc_input_len(ENC_CHAN_3) )*4 ) )
  {
    /* If overflow happens, truncate the blank padding buffer */
    pdu_byte_count = pdu_byte_count - size;

    rem_pad_byte_count =
      ( ( muxmdsp_get_enc_input_len(ENC_CHAN_3) )*4 ) - pdu_byte_count;

    M1X_MSG( ENC, LEGACY_MED,
      "PDU %d REM %d size %d",pdu_byte_count,rem_pad_byte_count,size);

    /* Make sure last bytes are word aligned */
    rem_pad_byte_count = rem_pad_byte_count - (rem_pad_byte_count % 4);

    /* Update the new size */
    size = rem_pad_byte_count;

    pdu_byte_count = pdu_byte_count + size;

    M1X_MSG( ENC, LEGACY_MED,
      "PDU %d REM %d size %d",pdu_byte_count,rem_pad_byte_count,size);
  }
  #endif /* !FEATURE_MODEM_1X_UTX */

  M1X_MSG( ENC, LEGACY_HIGH,
    "1X CH3: Writing %u bytes (last=%u) FIFO=%u SW_LTU_CRC_size=%u",
    size,
    last_write,
    FIFO_SIZE8(),
    enc_sch_ltu_size_bytes);

  /* Process the input buffer data first */
  do
  {
    uint32 data_in, bytes_read;

    /* Fetch the biggest piece of data possible from src, or do nothing
       if there is nothing to read */
    if ( (size == 0) || (src_data_ptr == NULL) )
    {
      /* Guarantee that this will be the last loop iteration here */
      data_in = 0;
      bytes_read = 0;
      size = 0;
    }
    else if ( 0 == ((unsigned long)src_data_ptr & 0x3) )
    {
      data_in = *(uint32 *)src_data_ptr;
      bytes_read = 4;
    }
    else if ( 0 == ((unsigned long)src_data_ptr & 0x1) )
    {
      data_in = *(uint16 *)src_data_ptr;
      bytes_read = 2;
    }
    else
    {
      data_in = *(uint8 *)src_data_ptr;
      bytes_read = 1;
    }

    /* Need to handle the case here where we may be over-read past the total
       number of bytes */
    if ( bytes_read > size )
    {
      /* Adjust bytes_read and set size=0 to indicate no more to read */
      bytes_read = size;
      size = 0;

      /* Zero the unused MSB bits */
      data_in &= ((uint32)1 << (bytes_read * 8)) - 1;
    }
    else
    {
      size -= bytes_read;
    }

    /* Nudge the read pointer along accordingly */
    src_data_ptr += bytes_read;

    /* check for ltu stride case and calculate and insert the LTU CRC
       then update data_in and bytes_read to be processed normally */
    if ( ltu_count_bytes > ( enc_sch_ltu_size_bytes - bytes_read ) )
    {
      switch( enc_sch_ltu_size_bytes - ltu_count_bytes )
      {
        case 1: /* 1 by 3 byte stride */
          FIFO_PUSH_AND_CRC_DATA8( data_in );
          FIFO_PUSH_CRC16_AND_INIT_LTU();
          data_in >>= 8;
          bytes_read -= 1;
          break;

        case 2: /* 2 by 2 byte stride */
          FIFO_PUSH_AND_CRC_DATA16( data_in );
          FIFO_PUSH_CRC16_AND_INIT_LTU();
          data_in >>= 16;
          bytes_read -= 2;
          break;

        case 3: /* 3 by 1 byte stride */
          FIFO_PUSH_AND_CRC_DATA16( data_in );
          FIFO_PUSH_AND_CRC_DATA8( data_in >> 16 );
          FIFO_PUSH_CRC16_AND_INIT_LTU();
          data_in >>= 24;
          bytes_read -= 3;
          break;

        default: /* all cases handled */
          /* if this is hit, something is very wrong */
          ONEX_ERR_FATAL("Unexpected remainder, ltu stride - %d",
                    enc_sch_ltu_size_bytes - ltu_count_bytes, 0 , 0);
          break;
      }
    }

    /* Handle the bytes_read values (0,1,2,3,4) - this logic will do the
       tricky stuff with calculating and inserting the LTU CRC at the right
       places in the output bytestream, if LTU CRC is needed at all */
    switch ( bytes_read )
    {
      case 0: /* Nothing read, should only happen on last_write=1 commit */
        /* Do nothing if no sub-LTU CRC is needed at end of frame */
        break;

      case 1: /* One byte read, compute the CRC */
        /* and increment the LTU count */
        FIFO_PUSH_AND_CRC_DATA8( data_in );
        break;

      case 2: /* 2 bytes read, normal uint16 stride case */
        /* Always CRC the 2 bytes and increment LTU count */
        FIFO_PUSH_AND_CRC_DATA16( data_in );
        break;

      case 3: /* 3 bytes read, compute the CRC for first 2 bytes */
        /* then compute CRC for remaining one byte */
        FIFO_PUSH_AND_CRC_DATA16( data_in );
        FIFO_PUSH_AND_CRC_DATA8( data_in >> 16 );
        break;

      case 4: /* 4 bytes read, normal uint32 stride case */
          /* CRC the 4 bytes and increment LTU count */
          FIFO_PUSH_AND_CRC_DATA32( data_in );
        break;

      default: /* Should never happen */
        ONEX_ERR_FATAL("Unexpected number of bytes read (%u)",bytes_read,0,0);
        break;
    }

    if ( enc_sch_ltu_size_bytes == 0 )
    {
      /* Do nothing if there is no LTU CRC needed */
    }
    else if ( ltu_count_bytes == enc_sch_ltu_size_bytes )
    {
      /* If we've got a full LTU, push out current CRC16, zero LTU count */
      FIFO_PUSH_CRC16_AND_INIT_LTU();
      #ifndef FEATURE_MODEM_1X_UTX
      /* Increment +2 for PDU byte count for LTU CRC 16 appended */
      pdu_byte_count = pdu_byte_count + 2;
      #endif /* !FEATURE_MODEM_1X_UTX */
    }
    else if ( ltu_count_bytes > enc_sch_ltu_size_bytes )
    {
      ONEX_ERR_FATAL("Unexpected over read (%u > %u)",
                ltu_count_bytes,enc_sch_ltu_size_bytes,0);
    }

    /* Drain any data we have in 4 byte chunks */
    while ( FIFO_SIZE8() >= 4 )
    {
      uint32 data_out;
      FIFO_PULL32( data_out );
      WRITE_ERAM_32( data_out );
    }
  } while ( size > 0 );

  #ifndef FEATURE_MODEM_1X_UTX
  /* Update the total written bytes to LMEM for this ch3 call */
  txc_enc_pdu_byte_count = pdu_byte_count;
  M1X_MSG( ENC, LEGACY_MED,
    "DEBUG : PDU bytes written till now %d",txc_enc_pdu_byte_count );
  #endif /* !FEATURE_MODEM_1X_UTX */

  /* If last write, do any special handling */
  if( last_write )
  {
    uint32 data_out = 0;
    /* Handle any remaining uint8 (should be 0/1/2/3) in FIFO */
    switch ( FIFO_SIZE8() )
    {
      case 0:
        /* Nothing remaining in the last write */
        break;
      case 1:
        FIFO_PULL8( data_out );
        WRITE_ERAM_32( data_out );
        break;

      case 2:
        FIFO_PULL16( data_out );
        WRITE_ERAM_32( data_out );
        break;

      case 3:
        /* Need to push a dummy byte into MSB so we can pull a full uint32
           without underflowing the FIFO */
        FIFO_PUSH8( 0 );
        FIFO_PULL32( data_out );
        WRITE_ERAM_32( data_out );
        break;

      default:
        ONEX_ERR_FATAL("Unexpected FIFO size remaining (%u)",FIFO_SIZE8(),0,0);
        break;
    }

    /* Reset the LTU count and CRC for next time */
    FIFO_INIT_CRC16_AND_LTU();

    #ifndef FEATURE_MODEM_1X_UTX
    /* Reset the PDU accumulation count of bytes */
    pdu_byte_count = 0;
    txc_enc_pdu_byte_count = 0;
    #endif /* !FEATURE_MODEM_1X_UTX */

    /* Trigger encoder CH3 to encode the data written above */
    TRIGGER_ENC_CH3();

    /* Sanity check, the FIFO should always be empty at this point */
    if ( FIFO_SIZE8() != 0 )
    {
      ONEX_ERR_FATAL("Tx FIFO not empty: %u", FIFO_SIZE8(),0,0);
    }
  }
} /* enc_write_data_ch3() */

/*===========================================================================

FUNCTION enc_write_data

DESCRIPTION
  This function is used to write data into the encoder for a given channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 None.

===========================================================================*/
static void enc_write_data
(
  enc_chan_number_type  chan_num,     /* which channel will be written to */
  byte                  *src_data_ptr, /* pointer points to source data */
  uint16                total_num_of_byte, /* Total number of byte */
  boolean               last_write /* indicated if this is the last write for a given channel */
)
{
  #ifndef FEATURE_MODEM_1X_UTX
  uint32 * enc_lmem_addr;
  #endif /* !FEATURE_MODEM_1X_UTX */

  /*-----------------------------------------------------------------*/

  /* Assert early & hard if the src_data_ptr is not uint32 aligned */
  if (((unsigned long)src_data_ptr & 0x3) != 0)
  {
    ONEX_ERR_FATAL("Source ptr not dword aligned (0x%x)", src_data_ptr, 0, 0);
  }

  /* Vector any CH3 writes to the dedicated function */
  if ( chan_num == ENC_CHAN_3 )
  {
    enc_write_data_ch3(src_data_ptr,total_num_of_byte,last_write);
    return;
  }

  if( txc_ta_info.r_fch_ta_log )
  {
    txc_ta_info.r_fch_ta_comp_sclk = timetick_get();
    txc_ta_info.r_fch_ta_log       = FALSE;

    /* Log to SRCH, if F-FCH logging is already done and if its QTA or
       FSCH and RSCH is not going to be assigned */
    if( ( rxc_ta_info.f_fch_ta_log == FALSE ) &&
        ( ( ( rxc_ta_info.f_sch_ta_log == FALSE ) &&
            ( txc_ta_info.r_sch_ta_log == FALSE ) ) ||
          ( ( !mccsch_sch_is_active(MCCSCH_FORWARD, 0) ) &&
            ( !mccsch_sch_is_active(MCCSCH_REVERSE, 0) ) )
         )
       )
      {
        /* Update SRCH here about the stats */
        rxc_tx_ta_srch_log_update();
      }
  }

  /* check if there is something to write */
  if ((total_num_of_byte > 0) && (src_data_ptr != NULL))
  {
    #ifndef FEATURE_MODEM_1X_UTX
    if ( NULL == ( enc_lmem_addr = muxmdsp_get_lmem_ch_addr(chan_num) ) )
    {
      ONEX_ERR_FATAL("Enc. CH %u LMEM addr is NULL", ( chan_num + 1 ), 0, 0);
    }

    /* we should never hit this case since the AXI address is guaranteed to
       be word aligned, but in case, support non word aligned addresses */
    if ( (uint32)enc_lmem_addr & 0x3 )
    {
      M1X_MSG( ENC, LEGACY_HIGH,
        "Enc LMEM addr not word aligned" );

      /* align enc_lmem_addr to half word/word boundary */
      if ( (uint32)enc_lmem_addr & 0x1 )
      {
        *(uint8 *)enc_lmem_addr = *(uint8 *)src_data_ptr;
        enc_lmem_addr = (uint32 *)( (uint8 *)enc_lmem_addr + sizeof(uint8) );
        src_data_ptr = src_data_ptr + sizeof(uint8);
        total_num_of_byte -= sizeof(uint8);
      }

      /* this is the case if enc_lmem_addr is aligned to half word */
      if ( (uint32)enc_lmem_addr & 0x2 )
      {
        *(uint16 *)enc_lmem_addr = *(uint16 *)src_data_ptr;
        enc_lmem_addr = (uint32 *)( (uint16 *)enc_lmem_addr + sizeof(uint16) );
        src_data_ptr = src_data_ptr + sizeof(uint16);
        total_num_of_byte -= sizeof(uint16);
      }
    }
    #endif /* !FEATURE_MODEM_1X_UTX */

    while ( total_num_of_byte >= sizeof(uint32) )
    {
      #ifdef FEATURE_MODEM_1X_UTX
      /* Assumes little-endian buffer, deref 32-bits and write to HW */
      HWIO_OUT( TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0, *(uint32 *)src_data_ptr );
      #else /* !FEATURE_MODEM_1X_UTX */
      /* Assumes little-endian buffer, deref 32-bits and write to LMEM */
      *enc_lmem_addr++ = *(uint32 *)src_data_ptr;
      #endif /* FEATURE_MODEM_1X_UTX */

      /* Advance the pointer, decrement the bytes left */
      src_data_ptr = src_data_ptr + sizeof(uint32);
      total_num_of_byte -= sizeof(uint32);
    }

    /* Handle any remainder bytes (1, 2, or 3) */
    if ( total_num_of_byte > 0 )
    {
      /* Construct a mask to later AND off the unused MSB's */
      uint32 remainder_data = (1 << (total_num_of_byte * 8)) - 1;

      /* AND-in the remaining 4 bytes, preserving only the ones we want */
      remainder_data &= *(uint32*)src_data_ptr;

      #ifdef FEATURE_MODEM_1X_UTX
      /* Write out the last 32-bit value to HW, unused MSBs are zero */
      HWIO_OUT( TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0, remainder_data );
      #else /* !FEATURE_MODEM_1X_UTX */
      *enc_lmem_addr = remainder_data;
      #endif /* FEATURE_MODEM_1X_UTX */
    }
  }

  #ifdef FEATURE_MODEM_1X_UTX
  /* check if this is the last write, then tell HW writing ERAM bank is DONE */
  /* Can't send FRAME CONFIG here now, need to prep all channels before
     telling FW to go */
  if ( last_write )
  {
    HWIO_OUT( TX_1X_ENC_TRIGGER, (uint32)(1 << chan_num) );
  }
  #endif /* FEATURE_MODEM_1X_UTX */

} /* enc_write_data */

/*===========================================================================

FUNCTION enc_get_next_fo_adj_frame_bdry_cx8

DESCRIPTION
  This function returns the frame offset adjusted rtc (cx8) of
  the next frame boundary used for Sync20 TX Trigger.

DEPENDENCIES
  None

RETURN VALUE
  Frame offset adjusted rtc (cx8) of the next frame boundary

SIDE EFFECTS
  None

===========================================================================*/
uint32 enc_get_next_fo_adj_frame_bdry_cx8
(
  uint8   frame_off,
  uint32* enc_current_rtc_cx8,
  uint32  enc_last_sync80_rtc_cx8
)
{
  uint32 number_of_frames_cx8      = 0;
  uint32 last_sync20_frame_rtc_cx8 = 0;
  uint32 tx_trigger_rtc_cx8        = 0;

  /*-----------------------------------------------------------------*/

  /* Check for rollover of the current RTC */
  if( *enc_current_rtc_cx8 < enc_last_sync80_rtc_cx8 )
  {
    /* rollover has occurred */
    *enc_current_rtc_cx8 += VSTMR_1X_RTC_MODULO;
  }

  number_of_frames_cx8  = ( *enc_current_rtc_cx8 - enc_last_sync80_rtc_cx8 );
  number_of_frames_cx8 /= ENC_20MS_FRAME_CX8;
  number_of_frames_cx8 *= ENC_20MS_FRAME_CX8;

  last_sync20_frame_rtc_cx8 = ( number_of_frames_cx8 + enc_last_sync80_rtc_cx8 ) %
                              VSTMR_1X_RTC_MODULO;

  M1X_MSG( ENC, LEGACY_HIGH,
     "1x2GTA: Curr RTC %u, Last 20ms frame RTC %u, Last Sync80 RTC %u",
     *enc_current_rtc_cx8,
     last_sync20_frame_rtc_cx8,
     enc_last_sync80_rtc_cx8 );

  /* Check for current rtc rollover during calculation */
  if( *enc_current_rtc_cx8 < last_sync20_frame_rtc_cx8 )
  {
    *enc_current_rtc_cx8 += VSTMR_1X_RTC_MODULO;
  }

  /* set tx trigger to the next frame boundary */
  tx_trigger_rtc_cx8 = last_sync20_frame_rtc_cx8 + ENC_20MS_FRAME_CX8;

  /* Update trigger frame with frame offset value in Cx8,
     1 PCG = 1536Cx1 = 12288cx8, this take care of ULPN update as well */
  tx_trigger_rtc_cx8 += ( frame_off * 12288 );

  M1X_MSG( ENC, LEGACY_HIGH,
     "1x2GTA: Adding Frame Offset (%u) to the next Sync20 frame RTC %u",
     frame_off,
     tx_trigger_rtc_cx8 );

  /* Make sure Tx trigger time is within safety margin */
  if( ( (int32)( tx_trigger_rtc_cx8 - *enc_current_rtc_cx8 ) ) < ENC_TRIG_MARGIN_CX8 )
  {
    M1X_MSG( ENC, LEGACY_HIGH,
       "1x2GTA: Sync20 frame RTC %u is too close to the next Sync20 frame, wait 1 more frame.",
       tx_trigger_rtc_cx8 );

    tx_trigger_rtc_cx8 += ENC_20MS_FRAME_CX8;
  }

  return tx_trigger_rtc_cx8;

} /* enc_get_next_fo_adj_frame_bdry_cx8 */

/*===========================================================================

FUNCTION enc_get_frame_boundary_with_frame_offset_cx8

DESCRIPTION
  This function returns the frame boundary RTC with the frame
  offset correction in cx8.

DEPENDENCIES
  None

RETURN VALUE
  Frame Boundary with the Frame Offset in cx8

SIDE EFFECTS
  None

===========================================================================*/
uint32 enc_get_frame_boundary_with_frame_offset_cx8
(
  boolean ta_is_enabled,
  uint8   frame_off,
  uint32  tx_trigger_rtc_val_cx8
)
{
  uint32 frame_bdry_cx8 = tx_trigger_rtc_val_cx8;

  /*-----------------------------------------------------------------*/

  if( ta_is_enabled )
  {
    frame_bdry_cx8 =
    ( ( tx_trigger_rtc_val_cx8 - ( frame_off * 12288 ) ) +
                               VSTMR_1X_RTC_MODULO ) % VSTMR_1X_RTC_MODULO;

    if( tx_trigger_rtc_val_cx8 < frame_bdry_cx8 )
    {
      frame_bdry_cx8 =
       ( frame_bdry_cx8 + 786432 ) % VSTMR_1X_RTC_MODULO;
    }
  }

  return frame_bdry_cx8;

} /* enc_get_frame_boundary_with_frame_offset_cx8 */

/*===========================================================================

FUNCTION enc_sync_tx_timing

DESCRIPTION
  This function prepares the Tx prefill and TxDAC timing setup.  It can
  either trigger immediately (abort case), or at the Sync20/Sync80 (normal)

DEPENDENCIES
  Long code slam values should have already been loaded into HW.
  This needs to happen within 13ms of next SYNC80 boundary

RETURN VALUE
  None

SIDE EFFECTS
 Disables any active encoder channels!!  Caller must re-enable if needed,
 but re-enable must happen AFTER the trigger point.

===========================================================================*/
void enc_sync_tx_timing
(
  boolean trigger_now           /* Trigger now, or on next Sync20/Sync80 */
)
{
  uint8     frame_off               = enc_frame_off;
  boolean   ta_is_enabled           = FALSE;
  int16     btf_value_cx8           = 0;
  uint16    enc_init_pnstate_i      = ENC_PN_INIT_STATE;
  uint16    enc_init_pnstate_q      = ENC_PN_INIT_STATE;
  uint32    frame_num               = 0;
  uint32    sync80_phase            = 0;
  uint32    enc_cur_rtc_cx8         = 0;
  uint32    enc_last_sync80_rtc_cx8 = 0;
  uint32    tx_trigger_rtc_cx8      = 0;
  uint32    frame_bdry_wo_fo_cx8    = 0;
  uint64    sync80_lc               = 0;
  ulpn_type ulpn                    = { 0 }; /* user long PN code */
  qword     pn_state;                 /* pn state to write to ENC */

  #ifdef FEATURE_MODEM_1X_UTX
  cfw_tx_dac_start_msg_t txdac_start_m        = { { 0 } };
  const uint32           prefill_compensation = 6 * 8; /* Tx prefill fifo depth */
  #else /* !FEATURE_MODEM_1X_UTX */
  enc_spn_state_type enc_spn                  = { 0 };
  cdmafw_rl_txfe_trigger_msg_t txfe_trigger_m = { { 0 } };
  #endif /* FEATURE_MODEM_1X_UTX */

  /*-----------------------------------------------------------------*/

  M1X_MSG( ENC, LEGACY_HIGH,
    "Performing Tx timing sync" );

  #ifdef FEATURE_MODEM_1X_UTX
  {
    volatile uint32 * const pTX_C2K_MODE = (volatile uint32*)
      HWIO_ADDR( TX_C2K_MODE );
    volatile uint32 * const pTX_UNIFIED_SAMPLE_CNT_READ = (volatile uint32*)
      HWIO_ADDR( TX_UNIFIED_SAMPLE_CNT_READ );
    register uint32 samp_cnt_before, samp_cnt_after;

    /* Verify TX_UNIFIED_SAMPLE_CNT_READ is running, otherwise the workaround
       code will fail */
    samp_cnt_before = *pTX_UNIFIED_SAMPLE_CNT_READ;
    DALSYS_BusyWait( 10 );   /* delay for some chips */
    samp_cnt_after = *pTX_UNIFIED_SAMPLE_CNT_READ;

    if( samp_cnt_before == samp_cnt_after )
    {
      /* if the sample count has not changed by now UTX modulator is stopped */
      M1X_MSG( ENC, LEGACY_HIGH,
         "UTX stopped, safe to configure TX." );
    }
    else
    {
      /* UTX modulator still running, programming C2K_MODE now could cause
         UTX to get stuck */
      /* print relevant FW info */
      muxmdsp_print_tx_timing_info( TRUE );
      M1X_MSG( ENC, LEGACY_ERROR,
        "UTX still running!!! MISC_CTL=0x%08X "
        "C2K_MODE=0x%08X OFFL_EN=%0X",
        HWIO_IN(TX_1X_MISC_CTL),
        *pTX_C2K_MODE,
        HWIO_IN( TX_UNIFIED_OFFL_EN ) );
    }
  }

  /* Stop UTX (if it was enabled, shouldn't be, though) */
  HWIO_OUTF( TX_UNIFIED_OFFL_EN, EN, 0 );

  /* Enable 1x modulator, in IS2000 mode by default */
  HWIO_OUT( TX_C2K_MODE, HWIO_FMSK( TX_C2K_MODE, MOD_1X_EN ) );

  /* Send a TX DAC stop msg to FW */
  muxmdsp_stop_txdac();

  /* Disable any active encoder channels */
  HWIO_OUTF( TX_1X_CH1_ENC_CTL  , CH1_EN, 0 );
  HWIO_OUTF( TX_1X_CH2_ENC_CTL_0, FCH_EN, 0 );
  HWIO_OUTF( TX_1X_CH3_ENC_CTL_0, SCH_EN, 0 );

  #else /* !FEATURE_MODEM_1X_UTX */

  /* Disable any active encoder channels */
  muxmdsp_enc_ch_en( ENC_CHAN_1, FALSE );
  muxmdsp_enc_ch_en( ENC_CHAN_2, FALSE );
  muxmdsp_enc_ch_en( ENC_CHAN_3, FALSE );

  /* Load default encoder parameters to prevent WAV entering undefined state */
  enc_load_default_params();

  /* Set channel gains to zero for now, since there data is not valid */
  muxmdsp_set_channel_gains_zero();

  /* send FW message to commit frame configs */
  muxmdsp_send_frame_config();
  #endif /* FEATURE_MODEM_1X_UTX */

  /* Get sync80 phase (range 0~2) */
  sync80_phase    = m1x_stmr_get_scomb_phase();

  /* Get the BTF value which has RF + Combiner delay in cx8 */
  btf_value_cx8   = enc_get_btf_cx8();

  /* Get the current RTC count */
  enc_cur_rtc_cx8 = m1x_stmr_get_rtc_count_cx8();

  /* Check if TA is enabled */
  ta_is_enabled = txc_is_ta_enabled();

  /* Snapshot the last sync80 RTC value (Rx side) */
  srch_mux_get_rtc_lcstate_at_sync80( ta_is_enabled, &enc_last_sync80_rtc_cx8, &sync80_lc );

  #ifdef FEATURE_MODEM_1X_RUMI
  if( onex_nonsig_is_ftm_mode() )
  {
    /* Get current RTC cnt and advance to next SYNC80
       1 SYNC80 = 786432 cx8 */
    tx_trigger_rtc_cx8 = enc_cur_rtc_cx8;
    tx_trigger_rtc_cx8 += 786432 - m1x_stmr_get_scomb_chipx8_count();
    ulpn.low  = enc_ftm_default_ulpn_state & 0xFFFFFFFF;
    ulpn.high = ( enc_ftm_default_ulpn_state >> 32 ) & 0x3FF;
  }
  else
  #endif /* FEATURE_MODEM_1X_RUMI */
  {
    /* If immediate trigger is needed, set to now + a smidge, else
       next PN roll - Tx advance factor */
    if( trigger_now )
    {
      /* Read the ULPN state */
      srch_mux_get_ulpn_state( &ulpn );

      /* Advance ULPN by 1 PN Roll. ULPN is already advanced by Sync80 Phase */
      ulpn_adjust( &ulpn, 1 );

      /* Now + 1ms can be considered 'immediate' = 9830.4 cx8 */
      tx_trigger_rtc_cx8 = enc_cur_rtc_cx8 + 9830;
      M1X_MSG( ENC, LEGACY_HIGH,
         "Immediate TXR trigger rtc=0x%08X (now + 1ms)",
         tx_trigger_rtc_cx8 );
    }
    else
    {
      /* If TA is enabled, use Sync20 TX Trigger */
      if( ta_is_enabled )
      {
        int16  tx_gain_adj      = 0;
        uint32 enc_ulpn_adj_cx1 = 0;

        ulpn.low  = (uint32)sync80_lc;
        ulpn.high = ( sync80_lc >> 32 ) & 0x3FF;

        /* Get the new Sync20 Trigger RTC using the current Frame offset and
           the last Sync80 */
        tx_trigger_rtc_cx8 =
          enc_get_next_fo_adj_frame_bdry_cx8( frame_off,
                                              &enc_cur_rtc_cx8,
                                              enc_last_sync80_rtc_cx8 );

        /* Get the chip advance from the last Sync80 */
        enc_ulpn_adj_cx1 = ( tx_trigger_rtc_cx8 - enc_last_sync80_rtc_cx8 ) / 8;

        /* Adjust the ULPN to the next frame boundary state */
        ulpn_advance_chips( &ulpn, enc_ulpn_adj_cx1 );

        /* Advance the Short PN codes to the next frame boundary state  */
        m1x_iq_pn_advance( &enc_init_pnstate_i, &enc_init_pnstate_q, enc_ulpn_adj_cx1 );

        /* Frame # in sync80 where we want to trigger, non-offset, so
           we don't need to consider FO in this calc */
        frame_num = enc_ulpn_adj_cx1 / ( ENC_20MS_FRAME_CX8 / 8 );

        M1X_MSG( ENC, LEGACY_HIGH,
           "1x2GTA: Trigger RTC @ %u, last sync80 ulpn adj by %u cx1",
           tx_trigger_rtc_cx8,
           enc_ulpn_adj_cx1 );

        /* Set gains to 1x FW here  */
        muxmdsp_set_channel_gains();

        /* Restore OL TX Power to 1x FW */
        muxmdsp_update_tx_open_loop_override( 1 );

        /* Override Closed loop Tx power before DAC_STRAT */
        tx_gain_adj = (int16)txtc.power;
        tx_gain_adj = (tx_gain_adj << 8);

        #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
        rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                         RFM_1X_TX_CL_OVERRIDE,
                                         tx_gain_adj );
        #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

        M1X_MSG( ENC, LEGACY_HIGH,
           "1x2GTA: Tx power override Tx power %d txtc_power %d",
           tx_gain_adj,
           txtc.power );
      }
      else /* Use Legacy Sync80 TX Trigger */
      {
        /* Read the ULPN state */
        srch_mux_get_ulpn_state( &ulpn );

        /* Advance ULPN by 1 PN Roll. ULPN is already advanced by Sync80 Phase */
        ulpn_adjust( &ulpn, 1 );

        /* Advanced trigger point by (1 + phase) * 26.66ms to get next sync26 value.
           1.2288e6 chip/sec * 8 chipx8/chip * 26.66ms = 262144 cx8 */
        tx_trigger_rtc_cx8 = enc_last_sync80_rtc_cx8 + ( ( 1 + sync80_phase ) * 262144 );
      }

      /* Subtract out randomization delay adjusted BTF value in cx8 */
      tx_trigger_rtc_cx8 -= ( btf_value_cx8 - enc_pn_rand_delay_chipx8 );

      tx_trigger_rtc_cx8  =
        ( tx_trigger_rtc_cx8 + VSTMR_1X_RTC_MODULO ) % VSTMR_1X_RTC_MODULO;
    }
  }

  /* Always set Short PN before loading LC */
  enc_iq_pn_state( enc_init_pnstate_i, enc_init_pnstate_q );

  /* Load the LC State */
  qw_set( pn_state, ( dword ) ulpn.high, ulpn.low );
  enc_user_pn_state( pn_state );

  /* Load LC mask for LTA case */
  if( ta_is_enabled )
  {
    enc_user_pn_mask( enc_lc_mask );
  }

  /* Send TX_Cn_TRIG_VAL trigger value to FW, uses SYS_1X Rx-side RTC */
  #ifdef FEATURE_MODEM_1X_UTX
  if( !ta_is_enabled )
  {
    /* Always reload Tx sample count value of 0 */
    HWIO_OUT( TX_UNIFIED_SAMPLE_CNT_INIT_VAL, 0 );
    M1X_MSG( ENC, LEGACY_HIGH,
       "1x2GTA: TA is OFF" );
  }
  else
  {
    /* Sync20 TX Sample count load to non-offset frame timing
       1. Zero for Chip           - Part 1 Counter [12:0]
       2. Frame Offset == PCG NUM - Part 2 Counter [16:13]
       3. Frame #                 - Part 3 Counter [24:17] */
    frame_num %= 4;

    M1X_MSG( ENC, LEGACY_HIGH,
       "1x2GTA: TA is ON, Frame Num %u, Frame offset %u",
       frame_num,
       frame_off );

    HWIO_OUT( TX_UNIFIED_SAMPLE_CNT_INIT_VAL, ( frame_num << 17 ) | ( frame_off << 13 ) );
  }
  HWIO_OUT( TX_UNIFIED_SAMPLE_CNT_LD, 1 );

  /* Trigger pre-fill mode */
  HWIO_OUTF( TX_UNIFIED_OFFL_EN, EN, 1 );

  /* Set the initial PN states back to original values after Offline Enable */
  enc_iq_pn_state( ENC_PN_INIT_STATE, ENC_PN_INIT_STATE );

  /* Now, if the post-prefill/offl_en=1 sample count hasn't moved, it means
     the Tx fifos were already full, so we need to back that depth of
     samples out of the TXR timing triggerpoint to ensure our first valid
     sample hits the DAC when it should */
  if( HWIO_IN( TX_UNIFIED_SAMPLE_CNT_INIT_VAL ) == HWIO_IN( TX_UNIFIED_SAMPLE_CNT_READ ) )
  {
    M1X_MSG( ENC, LEGACY_HIGH,
      "TX_UNIFIED_SAMPLE_CNT_READ unchanged, compensating "
      "TXDAC trigger by -%u cx8",
      prefill_compensation );

    tx_trigger_rtc_cx8 -= prefill_compensation;
    tx_trigger_rtc_cx8  =
     ( tx_trigger_rtc_cx8 + VSTMR_1X_RTC_MODULO ) % VSTMR_1X_RTC_MODULO;
  }

  /* Get the Frame boundary without Frame Offset. For Sync80 Tx Trigger,
     this will be same as TX Trigger RTC */
  frame_bdry_wo_fo_cx8 =
    enc_get_frame_boundary_with_frame_offset_cx8( ta_is_enabled,
                                                  frame_off,
                                                  tx_trigger_rtc_cx8 );

  txdac_start_m.start_rtc_cx8       = tx_trigger_rtc_cx8;
  txdac_start_m.frame_bndry_rtc_cx8 = frame_bdry_wo_fo_cx8;

  /* Trigger TxDAC */
  muxmdsp_send_txdac_trigger( &txdac_start_m );

  #else /* !FEATURE_MODEM_1X_UTX */
  {
    /* Get the LC state from FW. This will be bit reversed */
    uint64 enc_lc_state = muxmdsp_get_rl_lc_state();

    /* Get Short PN state from FW. This will be bit reversed */
    enc_spn = muxmdsp_get_rl_spn_state();

    /* Get the Frame boundary without Frame Offset. For Sync80 Tx Trigger,
       this will be same as TX Trigger RTC */
    frame_bdry_wo_fo_cx8 =
      enc_get_frame_boundary_with_frame_offset_cx8( ta_is_enabled,
                                                    frame_off,
                                                    tx_trigger_rtc_cx8 );

    txfe_trigger_m.txTriggerRTC              = tx_trigger_rtc_cx8;
    txfe_trigger_m.frameBndryRTC             = frame_bdry_wo_fo_cx8;
    txfe_trigger_m.initPNState.I_PN_STATE_01 = enc_spn.spn_i;
    txfe_trigger_m.initPNState.Q_PN_STATE_01 = enc_spn.spn_q;
    txfe_trigger_m.initLCState0              = ( enc_lc_state & 0xFFFFFFFF );
    txfe_trigger_m.initLCState1              = ( enc_lc_state >> 32 ) & 0x3FF;

    /* Trigger TxFE */
    muxmdsp_send_txfe_trigger( &txfe_trigger_m );
  }
  #endif /* FEATURE_MODEM_1X_UTX */

  M1X_MSG( ENC, LEGACY_HIGH,
     "Last Sync80 RTC %u cx8, Current RTC %u cx8, Sync80 Phase %u, "
     "Sync20 Frame %u, frame offset %u",
     enc_last_sync80_rtc_cx8,
     enc_cur_rtc_cx8,
     sync80_phase,
     frame_num,
     frame_off );

  M1X_MSG( ENC, LEGACY_HIGH,
     "Tx trigger %u cx8, Adjusted BTF %u cx8, "
     "Frame Boundary w/o Frame Offset %u cx8",
     tx_trigger_rtc_cx8,
     btf_value_cx8-enc_pn_rand_delay_chipx8,
     frame_bdry_wo_fo_cx8 );

} /* enc_sync_tx_timing */

/*===========================================================================

FUNCTION enc_tx_1x_test_waveform

DESCRIPTION
  This function starts/stops a 1x Tx test waveform

DEPENDENCIES
  RF and FW should already be enabled and in Tx-capable mode

RETURN VALUE
  None

SIDE EFFECTS
 None.

===========================================================================*/
void enc_tx_1x_test_waveform( boolean enable )
{
  const byte zeros[ENC_9600_FULL_RATE_SIZE] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };   /* Full rate all zeros packet */

  static boolean is_enabled = FALSE;
  uint32 txdac_trigger_cx8  = 0;
  qword pn_state;                 /* pn state to write to ENC */

  #ifdef FEATURE_MODEM_1X_UTX
  cfw_tx_dac_start_msg_t txdac_start_m = { { 0 } };
  cfw_tx_dac_stop_msg_t txdac_stop_m   = { { 0 } };
  uint32 txc_update_time               = 0;
  #else /* !FEATURE_MODEM_1X_UTX */
  cdmafw_rl_txfe_trigger_msg_t txfe_trigger_m = { { 0 } };
  cdmafw_rl_txfe_stop_msg_t txfe_stop_m       = { { 0 } };
  cdmafw_rl_signaling_msg_t rl_signaling_m    = { { 0 } };
  #endif /* FEATURE_MODEM_1X_UTX */

  /*-----------------------------------------------------------------*/

  /* Put out an F3 message indicating status */
  M1X_MSG( ENC, LEGACY_HIGH,
    "1x Tx test waveform: enable=%d, enabled=%d",
    enable,
    is_enabled );

  if( enable && !is_enabled )
  {
    /* Set short codes to initial states */
    enc_iq_pn_state( ENC_PN_INIT_STATE, ENC_PN_INIT_STATE );

    /* Set ulpn to 0 to disable LC spreading */
    qw_set( pn_state, 0, 0 );
    enc_user_pn_state( pn_state );

    /* Initialize M1X STMR */
    m1x_stmr_init();

    /* Get the RTC count in cx8 */
    txdac_trigger_cx8 = m1x_stmr_get_rtc_count_cx8();

    #ifdef FEATURE_MODEM_1X_UTX
    /* Align PCG timing of RTC_MOD_TX_SYS_TIME w/ MOD_TX_SYS_TIME */
    {
      uint32 rtc_mod_tx_time,chips,pcg,diff_cx1;

      rtc_mod_tx_time = 0;
      chips = rtc_mod_tx_time & 0x7FF;
      pcg = (rtc_mod_tx_time >> 16) & 0xF;
      diff_cx1 = (1535 - chips) + 1536;
      txdac_trigger_cx8 += diff_cx1 << 3;
      txdac_trigger_cx8 += txc_update_time;

      M1X_MSG( ENC, LEGACY_HIGH,
        "Advancing RTC by %u cx1, current pcg=%u chips=%u)",
        diff_cx1,
        pcg,
        chips );
    }

    /* Stop UTX (if it was enabled, shouldn't be, though) */
    HWIO_OUTF( TX_UNIFIED_OFFL_EN, EN, 0 );

    /* Enable 1x modulator, in IS95A mode by default */
    HWIO_OUT( TX_C2K_MODE, HWIO_FMSK( TX_C2K_MODE, MOD_1X_EN ) |
                           HWIO_FMSK( TX_C2K_MODE, C2K_IS95A_MODE ) );

    /* Always reload Tx sample count value of 0 */
    HWIO_OUT( TX_UNIFIED_SAMPLE_CNT_INIT_VAL, 0 );
    HWIO_OUT( TX_UNIFIED_SAMPLE_CNT_LD, 1 );

    /* Set the encoder for RC1, traffic mode */
    enc_fch_rc( ENC_FCH_RC_1 );
    enc_set_mode( ENC_TC_MODE );

    /* Trigger pre-fill mode */
    HWIO_OUTF( TX_UNIFIED_OFFL_EN, EN, 1 );
    #else /* !FEATURE_MODEM_1X_UTX */
    /* Advance the RTC count by 1 PCG, 1536 cx1 */
    txdac_trigger_cx8 += ( 1536 << 3 );
    M1X_MSG( ENC, LEGACY_HIGH,
      "Tx Trigger RTC cnt %u cx8", txdac_trigger_cx8 );

    /* Load default ch2 (FCH) params */
    enc_load_default_params();

    /* Set the encoder for RC1, traffic mode */
    enc_fch_rc( ENC_FCH_RC_1 );
    enc_set_mode( ENC_TC_MODE );

    /* Inform FW that this is RC1 */
    rl_signaling_m.rliRCConfig = CDMAFW_RC_CONFIG__RC12;
    rl_signaling_m.rliRCConfig_Update = TRUE;

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &rl_signaling_m.hdr,
                      CDMA_FW_CMD_RL_SIGNALING,
                      sizeof(cdmafw_rl_signaling_msg_t) );

    M1X_MSG( ENC, LEGACY_HIGH,
       "muxmdsp_configure_rl_rc: RC %u rliRCConfig %u",
       ENC_FCH_RC_1,
       rl_signaling_m.rliRCConfig );
    #endif /* FEATURE_MODEM_1X_UTX */

    /* Load and trigger the encoder */
    enc_fch_data_out( (byte *)zeros, FALSE, ENC_FULL_RATE );

    /* Gain value for IS-95 waveform (RC1 & RC2)
    ** Only 1 traffic channel on RL
    ** Allocate all power to traffic channel
    ** Gain setting for traffic channel is a constant
    ** NEW (15-bit):    (255 << 7) / sqrt(2) = 23079
    **   Sqrt(2) is for gain adjustment for QPSK
    */
    channel_gains.txc_fch_gain    = 23079;
    channel_gains.txc_pch_gain    = 0;
    channel_gains.txc_sch_gain    = 0;
    channel_gains.txc_dcch_gain   = 0;
    channel_gains.txc_tx_rate_adj = 0;
    channel_gains.frame_valid     = TRUE;

    muxmdsp_set_channel_gains();

    #ifdef FEATURE_MODEM_1X_UTX
    /* Set up TxDAC trigger in FW, guaranteed to happen within 2.5ms */
    txdac_start_m.start_rtc_cx8       = txdac_trigger_cx8;
    txdac_start_m.frame_bndry_rtc_cx8 = txdac_trigger_cx8;
    muxmdsp_send_txdac_trigger( &txdac_start_m );

    /* Wait for enough time for frames to start */
    DALSYS_BusyWait( 30000 );
    #else /* !FEATURE_MODEM_1X_UTX */
    /* Set Tx PCG mask to transmit in all PCGs */
    enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );

    /* Tell FW to latch frame config in smem */
    muxmdsp_send_frame_config();

    /* Set frame boundary RTC to the same as txTriggerRTC
       for legacy pre-Bolt behavior */
    txfe_trigger_m.txTriggerRTC              = txdac_trigger_cx8;
    txfe_trigger_m.frameBndryRTC             = txdac_trigger_cx8;
    txfe_trigger_m.initPNState.I_PN_STATE_01 = ( 0x0001 & 0x7FFF );
    txfe_trigger_m.initPNState.Q_PN_STATE_01 = ( 0x0001 & 0x7FFF );
    txfe_trigger_m.initLCState0              = 0;
    txfe_trigger_m.initLCState1              = 0;
    muxmdsp_send_txfe_trigger( &txfe_trigger_m );

    /* Wait for enough time for trigger to start, 1 pcgs = 1.25ms */
    DALSYS_BusyWait( 1250 );
    #endif /* FEATURE_MODEM_1X_UTX */

    /* Set the enabled flag */
    is_enabled = TRUE;

    /* Dump out FW Tx info */
    muxmdsp_print_tx_timing_info( TRUE );
  }
  else if ( !enable && is_enabled )
  {
    /* Dump out FW Tx info */
    muxmdsp_print_tx_timing_info( FALSE );

    /* Zero out channel gains */
    channel_gains.txc_fch_gain    = 0;
    channel_gains.txc_pch_gain    = 0;
    channel_gains.txc_sch_gain    = 0;
    channel_gains.txc_dcch_gain   = 0;
    channel_gains.txc_tx_rate_adj = 0;
    channel_gains.frame_valid     = FALSE;
    muxmdsp_set_channel_gains();

    /* Disable encoder ch2 (FCH) */
    enc_fch_enable( FALSE );

    #ifdef FEATURE_MODEM_1X_UTX
    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &txdac_stop_m.hdr,
                      CDMA_FW_TX_DAC_STOP_CMD,
                      sizeof(cfw_tx_dac_stop_msg_t) );

    M1X_MSG( ENC, LEGACY_HIGH,
      "Sending CDMA_FW_TX_DAC_STOP_CMD to FW" );

    /* Wait for enough time for TxDAC to stop */
    DALSYS_BusyWait( 40000 );

    /* Stop UTX (if it was enabled, shouldn't be, though) */
    HWIO_OUTF( TX_UNIFIED_OFFL_EN, EN, 0 );

    /* Disable 1x modulator mode */
    HWIO_OUTF( TX_C2K_MODE, MOD_1X_EN, 0 );
    #else /* !FEATURE_MODEM_1X_UTX */
    /* Register for a response */
    muxmsgr_register_msg( TX_TASK, CDMA_FW_RL_TXFE_STOP_RSP );

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &txfe_stop_m.hdr,
                      CDMA_FW_RL_TXFE_STOP_CMD,
                      sizeof(cdmafw_rl_txfe_stop_msg_t) );

    M1X_MSG( ENC, LEGACY_HIGH,
      "Sending CDMA_FW_RL_TXFE_STOP_CMD to FW" );

    /* Wait for enough time for TxFE to stop */
    DALSYS_BusyWait( 40000 );
    #endif /* FEATURE_MODEM_1X_UTX */

    muxmdsp_tx_triggered = FALSE;

    /* Deinitialize M1X STMR */
    m1x_stmr_deinit();

    /* Clear the enabled flag */
    is_enabled = FALSE;
  }
  else
  {
    M1X_MSG( ENC, LEGACY_HIGH,
      "1x Tx test waveform: No state change" );
  }

} /* enc_tx_1x_test_waveform */

#ifndef FEATURE_MODEM_1X_UTX
/*===========================================================================

FUNCTION ENC_FTM_TX_1X_INIT

DESCRIPTION
  This function initializes the necessary 1x structures for FTM testing

DEPENDENCIES
  RF and FW should be enabled and pointer to FW smem should not be NULL.

RETURN VALUE
  None

SIDE EFFECTS
 None.

===========================================================================*/
void enc_ftm_tx_1x_init
(
  cdmafw_shared_memory_interface_t* smem_addr  /* FW Shared memory address */
)
{

  /*-----------------------------------------------------------------*/

  /* Update MUX MDSP smem variable */
  muxmdsp_update_fw_smem_addr( smem_addr );

} /* enc_ftm_tx_1x_init */

/*===========================================================================

FUNCTION ENC_FTM_TX_1X_DEINIT

DESCRIPTION
  This function cleans up any 1x structures when FTM testing is complete

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
 None.

===========================================================================*/
void enc_ftm_tx_1x_deinit( void )
{

  /*-----------------------------------------------------------------*/

  /* Clear MUX MDSP smem variable */
  muxmdsp_update_fw_smem_addr( NULL );

} /* enc_ftm_tx_1x_deinit */
#endif /* !FEATURE_MODEM_1X_UTX */
