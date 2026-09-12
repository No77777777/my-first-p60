/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

  M O B I L E   C H A N N E L  D E I N T E R L E A V E R    S E R V I C E S

GENERAL DESCRIPTION
  Library of routines to initialize, and operate on, the Qualcomm CDMA
 Channel Deinterleaver chip.

  Copyright (c) 1999 - 2015 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/drivers/src/deint.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/03/15   eye     Expand the SW DEMBACK check for HW registers as well.
07/07/15   eye     Dont disable data bridge if SW DEMBACK is active.
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
04/02/15   srk     Fix unused variable warning.
04/17/14   cjb     Clean-up related to using 1x MSG macros for printing F3s
03/27/14   vks     Set tdec mode register only after asking FW to setup tdec
03/21/14   vks     Initialize deint to DEINT_CDMA_STATE
12/07/13   cjb     Centralized shadow register writes
12/07/13   cjb     Changes to support DEMBACK_1
10/10/13   vks     Do not delete second slot as turbo is now using it
10/10/13   cjb     Use second 5ms slot as dummy task for turbo (FW limitation)
05/16/13   trc     Remove obsolete register DINT_CH3_CFG references
05/06/13   cjb     Resolve compiler warnings
04/02/13   pap     Feature cleanup
03/19/13   trc     Explicitly set TDEC_MODE_CTL prior to 1x TDEC F-SCH usage
02/26/13   trc     Halt deint ch2 in deint_delete_supplemental_task_list()
02/15/13   srk     Replace mcs hwio with m1x hwio
11/13/12   trc     Layering/Cleanup for Triton
11/08/12   cjb     Making previous change backward compatible with Triton.
11/07/12   cjb     Switched to new FW interface which takes care of configuring
                   DEMBACK_MODE_SEL to proper values based on the active TECHs.
10/31/12   cjb     Temporary fix: Set DEMBACK mode to "1x only" for SCH cases.
10/18/12   srk     Use deint debug F3s for unnecessary F3s.
09/12/12   jtm     Move SCH Dummy task decob buff output to RC5 SCH offset
09/10/12   jtm     Use RC11 specific SVD buffer offsets.
08/22/12   cjb     Temporarily bypass TDEC cleanup during call release.
08/16/12   jtm     Added initial TDEC support or SCH for DIME.
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
04/04/12   trc     Convert to m1x_time module
04/03/12   jtm     Load FCH packet parameters when DCCH parameters are loaded.
02/24/12   srk     Mainlined FEATURE_IS2000_P2
01/23/12   srk     Feature Cleanup
01/11/12   srk     Feature Cleanup
09/28/11   srk     Mainlined Always On features and removed Always Off Features
09/13/11   srk     Moved queue init from deint_init() into a separate function
                   deint_queue_init() to prevent memory leak.
09/02/11   srk     Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
08/15/11   jtm     Klocwork Fixes.
06/16/11   trc     Convert to dec_set_mode() from dec_1x_set_svd_turbo_mode()
05/18/11   vks     Replace feature MSMHW_TURBO_CODING with
                   FEATURE_MODEM_1X_SUPPORTS_TURBO_CODING
05/17/11   trc     Call dec_1x_set_svd_turbo_mode(TRUE) at turbo F-SCH setup
04/28/11   adw     Remove masked write causing issues in 1/8th rate fch config.
04/26/11   adw     Remove obsolete pkt config settings now invalid on NikeL.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/21/11   adw     Cleanup compile warnings.
04/20/11   adw     Added additional calls to ensure cycle wait is enforced.
04/06/11   adw     Ensure cycle wait before writing DEINT regs on NikeL.
03/03/11   adw     Changes to the deint initialization routine.
01/21/11   vks     Fix linker issue on nikel.
01/21/11   adw     Added initial M1X_RM support.
12/15/10   jtm     Feature cleanup.
12/14/10   jtm     Mainlined FEATURE_SRCH_CLKREGIM_RM and cleaned up unneeded
                   legacy clock code.
12/03/10   jtm     Removed MDR/SCCH support.
10/30/10   jtm     Modem 1x SU API changes.
08/08/10   jtm     When SVD is not enabled, delete DINT task lists for RC11
                   FCH since this is handled by FW.
07/06/10   jtm     Fixed compiler warnings.
06/17/10   adw     Set CDMA clock config during deint initialization.
03/01/10   trc     Add 2-MAP parameters for Genesis modem turbo decoder
02/19/10   trc     Add support for Genesis modem turbo decoder
02/04/10   jtm     Removed FEATURE_PLT.
09/24/09   jtm     Klocwork fixes.
09/12/09   trc     Do normal deint/decode setup in RC11, except zero task list
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's.
08/05/09   jtm     Lint fixes.
07/06/09   jtm     Mainlined FEATURE_IS2000_1X_ADV for macro definitions and
                   tables sized with NUM_SCH_RC.
06/06/09   vlc     Added support for 1x Advanced (RC11.)
04/21/09   jtm     Replaced assert.h with amssassert.h. Assert.h is being
                   phased out.
04/17/09   jtm     Added includes to fix compiler warnings.
04/01/09   jtm     Eliminated implicit includes.
03/19/09   jtm     Updated included headers.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to Drivers.
03/06/09   jtm     Cleaned up compiler warnings
03/03/09   jtm     Lint fixes.
01/16/09   vlc     Moved Turbo decoder functions under MSMHW_TURBO_CODING.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
10/23/08   jtm     Removed T_QUASAR
08/12/08   jtm     Added DEINT_UPPER_BOUNDS_CHECK for bounds checking.
07/16/08   jtm     Eliminated obsolete code and reduced debug string sizes to save
                   ROM for the ULC effort.
07/01/08   vlc     Merged code changes from old tip archive.
04/10/08   trc     Switch clock mgmt strategy via FEATURE_SRCH_CLKREGIM_RM
03/31/08   mca     Changed FEATURE_CLKREGIM_RM to FEATURE_SRCH_CLKREGIM_RM
03/05/08   vlc     User clock resource manager to manage RX clocks
                   (FEATURE_CLKREGIM_RM.)
11/22/06   vlc     Fixed RVCT compiler warnings.
09/19/06   vlc     Fixed lint errors.
06/05/06   bn/vlc  In function deint_allocate_ram_nccch(), swap RAM allocation
                   for channel 2 to fix issue with SCH in traffic state.
03/31/03   bn      Make function deint_allocate_CH0_RAM becomes public
01/03/05   rkc     In deint_set_sc_mode(), allocated channel 0 RAM to
                   fix problem when switching between 1x only/hybrid modes.
01/03/05   rkc     Change deint_allocate_ram_nccch() to use L-M deinterleaver RAM
                   instead of N-P RAM since N-P RAM is used by HDR in simultaneous
                   1/x/HDR mode.  Also turn on L0 and M0 extended RAM since original
                   768 symbols cannot handle max 3072 frame size.
01/03/05   rkc     Change DINT_TASK_LIST's start address for decoder output buffer
                   for FCCCH data since FCCCH moved to short buffer offset.
04/21/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
03/30/05   bn      Removed obsoleted debug message
01/14/05   jyw     Added the macros for DINT_CH3_CFG
12/17/04   jyw     Added the initialization to DINT_CH3_CFG
11/30/04   vlc     Added debug F3 message to show Conv SCH parameters.
06/10/04   sr      Fixed shifts for TD_PARAMS_LO to match the changes in MSM6700
                   Turned on clocks to DEINT & DEC fro QUASAR
05/06/04   sr      Moved out DV driver code to separate file
05/05/04   sr      Fixed typo in PDCH #defines & defined DEVO_REV_D_HW
04/13/04   sr      Added DINT PDCCH configuration parameters and modified
                   deint_set_pdcch_mode() to program these into DINT.
                   Replaced num_pdcch with hw_test_mode input parameter to
                   deint_set_pdcch_mode().
09/03/03   dna     Ported for MSM6500
08/08/03   vlc     In function deint_create_dcch_task_list(), changed output
                   buffer address of DCCH task to decouple FCH/DCCH rate sets.
07/10/03   vlc     Added DEC_SCH_DUMMY_FULL_OB_ADDR.
                   This address is used for Turbo SCH dummy task and is beyong
                   any critical decoder output buffer adresses.
                   Got rid of unused parameter for local Rel A function
                   deint_create_fch_task_list().
                   Added initialization for curr_deint_ch_active at start of
                   traffic channel.
03/20/03   bn,sr   Added logic to support simultaneously monitor BCCH and FCCCH
03/07/03   vlc     Changes are for Release A.  Created function
                   deint_load_dcch_pkt_params() and use deint packet F for
                   DCCH rather than deint packet A so FCH and DCCH can
                   co-exist with different RC.
02/25/03   vlc     Changes are for MSM6100, Release A:
                   Added logic to solve Turbo finite burst assignment issues
                   brought on by moving Turbo dummy SCH interrupt to 3rd bin.
                   Added new functions deint_atmr_isr2 () and
                   deint_atmr_isr2_init ().  These functions support the use
                   of a second Action Timer ISR which is set to go off at the
                   4th bin in a frame.
                   In function deint_delete_supplemental_task_list(), now zero
                   out 3rd bin task list for Turbo SCH.
01/21/03   vlc     Put Turbo SCH dummy task on 3rd bin code under release A
                   feature.
11/27/02   cr      Moved MSMHW_EXTENDED_LTU_LEN msm6100hw.h
11/26/02   sr/vlc  Moved Turbo SCH dummy task to 3rd bin for MSM6100.
11/25/02   vlc     Added MSMHW_EXTENDED_LTU_LEN feature to handle dynamic
                   LTU length in function deint_load_sch_pkt_params().
10/22/02   vlc     Added RS1 Turbo 32X parameters.  Changed 2 parameters of RS2
                   Turbo 16X.  Removed code to prohibit RS1 Turbo 32X config which
                   no longer applied.
08/26/02   sr      Added deint_bcch_seq_repetition()
07/16/02   dlb2    Resolved register differences between MSM5100 and MSM6050
                   for Rel A code.
07/17/02   vlc     Changed code (including the addition of various masks) to turn
                   off FCH interrupt when DCCH is dynamically added.
06/27/02   dlb     Added workaround for MSM6050 DEC1 problem.
06/21/02   vlc     Switched order of parameter list of
                   deint_create_fch_task_list().
06/12/02   vlc     Removed unused functions deint_delete_fch_task_list()
                   and deint_delete_dcch_task_list().
                   Changed DEINT_MAX_SEQUENCING_CMD to 3 for VP2 support.
06/04/02   vlc     Added functions deint_delete_fch_dcch_task_list(),
                   deint_delete_fch_task_list(), and deint_delete_dcch_task_list().
                   Changed code to allow DCCH and FCH channel set up while in
                   traffic state with no active channels.
05/30/02   vlc     VP2 changes are now featurized under FEATURE_IS2000_REL_A_SVD
                   and FEATURE_IS2000_P2 per request of development team.
                   Implemented comments from VP2 code review.
05/16/02   dlb     Added support for modified LTU_LEN fields for MSM6050.
                   Featurized with MSMHW_EXTENDED_LTU_LEN flag.
05/13/02   vlc     Added support for simultaneous DCCH & FCH channel set up
                   (feature FEATURE_IS2000_REL_A_VP2.)
03/15/02   vlc     No longer program deinterleaver to shift 1X SCH header bits
                   for FEATURE_IS2000_REL_A_SVD.
03/05/02   sr      Initialized deint task list with "zero" tasks in all the
                   4 5ms bins in deint_init() and removed creation of "zero"
                   tasks in other 5ms bins in deint_create_fch_task_list()
                   as this could wipe out already programmed and active SCH task
02/25/02   sr      Merged in Release A common channels from the branch
01/23/02   dlb     Resolved register differences between MSM5100 and MSM6050
11/16/01   vlc     Modified deint_init () to call dec_clk_select ().
10/04/01   sr      Merged in fix for turbo decoder sequencing from MSM5000.
                   This solves turbo code overlapping burst problems.
09/25/01   lad     Moved internal constants and macros from deint.h, made
                   tables 'static const' to save RAM, and made a few cosmetic
                   changes.
08/13/01   sr      Merged in from MSM5000 archive:
                   In deint_delete_supplemental_task_list, added removal of
                   DEINT_SCH_READ_PARAMS commands to make sure that any pending
                   SCH tasks are cleaned up.
08/10/01   lh      Added 32X support.
07/04/01   sr      Added support for mixed rate sets by defining independent
                   deinterleaver packet E parameters for 1X SCH task instead of
                   reusing packet A parameters loaded for FCH full rate task.
06/04/01   tc      Initialized HW_TD_INTLV_LEN_X2/X4/X8 to clear the bit.
02/20/01   sr      Added support for 32X
01/05/01   tc      Fixed the decoder clock source select to TCXO.
12/21/00   tc      Fixed the forward link turbo code.
12/13/00   vlc     Changed TD_MIN_LLR_WH to TD_MIN_LLR_THRESH_WH for MSM5100.
                   This change is necessary when MSMHW_TURBO_CODING is enabled.
11/14/00   tc      Added MSM5100 Decoder clock source select support.
10/24/00   rmd     Merge with MSM5000 code. Did some code clean up.
10/23/00   rmd     Included rxc.h so that we get access to the variables
                   rxc_turbo_dec_enable and rxc_sup_dec_int_enable.
10/19/00   rmd     In deint_init(), removed call to clk_regime_enable(CLK_RGM_DEC_M).
                   This call is not necessary, this clk regime is already on.
09/18/00   rm      enable clockregim after selecting clock source
09/08/00   jcw     Added include of clkregim.h to support call to clk_regime_enable
09/06/00   djd/et  Added ifdef MSMHW_TURBO_CODING around turbo coding software.
09/06/00   rm      Added clock regime changes for MSM5105.
09/06/00   rm      Enable Decoder clock regime in deint_init.
09/05/00   rm      Added sequencing support for Turbo decoder.
09/03/00   rm      Made changes to fix SCH sequencing of read/write parameters.
08/04/00   rm      Added support to sequence read parameters for convolutionally
                   encoded SCH frames. This is required for SCH Burst assigments.
07/13/00   rm      Fixed the case where FCH data was being overwriiten if
                   SCH was turbo encoded.
07/12/00   rm      Added PLT support for 40/80ms MULTIFRAME INTERLEAVING
                   for suuplemental channels.
07/11/00   rm      Added PLT support for 5ms FCH frames.
05/10/00   rm      Fixes to support 1X SCH.
04/05/00   va      Fixed bug in sending DEINT_SCCH_READ_PARAMS command.
04/15/00   rm      Created a new dummy task in the second 5ms slot when
                   Turbo Decdoing (SCH) is enabled. Turbo Decoder now
                   doesnot generate interrupts. This dummy task in the
                   second 5ms slot will generate a interrupt and this
                   should be associated with SCH interrpt processing for
                   Turbo Decoded frames.
03/23/00   bgc     Changed yamamoto threshholds for RC1 and RC2 to
                   [1, 1, 3, 3] for [full, half, quarter, eighth].  These
                   are the same thresholds as MSM3000 for RC1 and RC2
01/28/00   rm      Added in support for 1X SCH (RC3, RC4 & RC5).
01/24/00   rm      Added in support to sequence read/write parameters. The
                   write parameters are programmed one frame ahead of the read
                   parameters. This will allow us clean transiton if SCH rates
                   change or a change in RC, without loss of a frame.
11/15/99   rm      Added in support for RC5 SCH (convolutional codes).
11/09/99   rm      Added in support for RC3,RC4 & RC5 SCH (TURBO DECODER).
10/20/99   rm      Added in a new function to delete supplemental channel
                   task from deinterleaver task ram.
10/17/99   rm      Added in support for IS95B Supplemental Code Channel.
09/24/99   rm      Changes the Deinterleaver/Decoder clock to TCXO.
09/24/99   rm      Added in support for SCH (RC3 & RC4).
08/11/99   rm      Added support for RC4 and RC5 for F-FCH.
07/31/99   rm      Created file. Interfaces to support channel deinterleaver.

============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"
#include "drivers_common.h"
#include "m1x_diag.h"

//drivers
#include "m1x_rm.h"
#include "msm_drv.h"
#include "msm_drv_i.h"
#include "deint.h"
#include "deint_i.h"
#include "dec5000.h"
#include "dec5000_v.h"
#include "m1x_hwio_mpss.h"

//mux
#include "rxc_i.h"
#include "muxmdsp_i.h"

//cp
#include "cai_v.h"

//other
#include "amssassert.h"
#include "m1x_diag.h"
#include "err.h"
#include "queue.h"
#include "log.h"
#include "m1x_time_i.h"
#include "qw.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*=======================================================================

      Constants & Macros

========================================================================*/
#define DEINT_DBG_MSG( ... ) \
     M1X_MSG( MUX, DBG_DEINT, __VA_ARGS__ )

/*=======================================================================

   Macros to resolve register name differences between platforms

========================================================================*/

  /* NOTE: Keeping the masks common for both DEMBACK_0 and DEMBACK_1 */
  #define DINT_CH0_CFG_MASK                      HWIO_RMSK(DINT_CHw_CFG)
  #define DINT_CH1_CFG_MASK                      HWIO_RMSK(DINT_CHw_CFG)
  #define DINT_CH2_CFG_MASK                      HWIO_RMSK(DINT_CHw_CFG)
  #define DINT_CH0_CFG__SYNC_CHAN_80M_BND_MASK   HWIO_FMSK(DINT_CHw_CFG,SYNC_CHAN_80M_BND)
  #define DINT_CH0_CFG__NUM_SUB_CHANS_MASK       HWIO_FMSK(DINT_CHw_CFG,NUM_SUB_CHANS)
  #define DINT_CH1_CFG__NUM_SUB_CHANS_MASK       HWIO_FMSK(DINT_CHw_CFG,NUM_SUB_CHANS)
  #define DINT_CH2_CFG__NUM_SUB_CHANS_MASK       HWIO_FMSK(DINT_CHw_CFG,NUM_SUB_CHANS)
  #define DINT_CH0_CFG__NEXT_20M_BND_MASK        HWIO_FMSK(DINT_CHw_CFG,NEXT_20M_BND)
  #define DINT_CH1_CFG__NEXT_20M_BND_MASK        HWIO_FMSK(DINT_CHw_CFG,NEXT_20M_BND)
  #define DINT_CH2_CFG__NEXT_20M_BND_MASK        HWIO_FMSK(DINT_CHw_CFG,NEXT_20M_BND)
  /* <END> NOTE: Keeping the masks common for both DEMBACK_0 and DEMBACK_1 <END> */

/*=======================================================================

   Constants describing settings for deinterleaver registers.

========================================================================*/

/* Packet Values for  Packet A, B, C, D, E, and F */
#define DINT_PKT_A_V (0x00 << 0x03)
#define DINT_PKT_B_V (0x01 << 0x03)
#define DINT_PKT_C_V (0x02 << 0x03)
#define DINT_PKT_D_V (0x03 << 0x03)
#define DINT_PKT_E_V (0x04 << 0x03)
#define DINT_PKT_F_V (0x05 << 0x03)

/* since we could have both FCCCH and BCCH on at a time, we can't share
new packet is fined for FCCCH*/
#define DINT_PKT_G_V (0x06 << 0x03)

#define DINT_TASK_LIST_PACKET_A_M  (0x00 << 0x04)
#define DINT_TASK_LIST_PACKET_A_V  (0x00 << 0x04)
#define DINT_TASK_LIST_PACKET_B_M  (0x01 << 0x04)
#define DINT_TASK_LIST_PACKET_B_V  (0x01 << 0x04)
#define DINT_TASK_LIST_PACKET_C_M  (0x02 << 0x04)
#define DINT_TASK_LIST_PACKET_C_V  (0x02 << 0x04)
#define DINT_TASK_LIST_PACKET_D_M  (0x03 << 0x04)
#define DINT_TASK_LIST_PACKET_D_V  (0x03 << 0x04)
#define DINT_TASK_LIST_PACKET_E_M  (0x04 << 0x04)
#define DINT_TASK_LIST_PACKET_E_V  (0x04 << 0x04)
#define DINT_TASK_LIST_PACKET_F_M  (0x05 << 0x04)
#define DINT_TASK_LIST_PACKET_F_V  (0x05 << 0x04)

/* since we could have both FCCCH and BCCH on at a time, we can't share
new packet is fined for FCCCH*/
#define DINT_TASK_LIST_PACKET_G_M  (0x06 << 0x04)
#define DINT_TASK_LIST_PACKET_G_V  (0x06 << 0x04)

/* Offsets for packet parameters */
#define DINT_PKT_INTLV_CFG_LO_V 0x00
#define DINT_PKT_INTLV_CFG_HI_V 0x01
#define DINT_PKT_BLK_SIZE_LO_V  0x02
#define DINT_PKT_BLK_SIZE_HI_V  0x03
#define DINT_PKT_PUNCT_LO_V     0x04
#define DINT_PKT_PUNCT_HI_V     0x05
#define DINT_PKT_PL_CRC_LO_V    0x06
#define DINT_PKT_PL_CRC_HI_V    0x07

/* Timing trigger settings */
#define DINT_SYNC_CHAN_80M_BND_V  0x8
#define DINT_NEXT_20M_BND_V       0x4

/* RAM allocation settings */
#define DINT_LRAM_CHAN_ALLOC_MASK 0xC0
#define DINT_MRAM_CHAN_ALLOC_MASK 0x30
#define DINT_NRAM_CHAN_ALLOC_MASK 0x0C
#define DINT_PRAM_CHAN_ALLOC_MASK 0x03

/* Settings to assignment deinterleaver RAMs to channels */
#define DINT_CH0_RAM_L_V  0x00
#define DINT_CH0_RAM_M_V  0x00
#define DINT_CH0_RAM_N_V  0x00
#define DINT_CH0_RAM_P_V  0x00

#define DINT_CH1_RAM_L_V  0x40
#define DINT_CH1_RAM_M_V  0x10
#define DINT_CH1_RAM_N_V  0x04
#define DINT_CH1_RAM_P_V  0x01

#define DINT_CH2_RAM_L_V  0x80
#define DINT_CH2_RAM_M_V  0x20
#define DINT_CH2_RAM_N_V  0x08
#define DINT_CH2_RAM_P_V  0x02

#define DINT_L0_RAM_EXTEND_ON (0x1 << HWIO_SHFT(DINT_CFG,L0_RAM_EXTEND))
/* Enable DINT_CFG's L0_RAM_EXTEND when F-CCCH, F-BCCH and F-SCH cannot fit in original RAM size */
#define DINT_M0_RAM_EXTEND_ON (0x1 << HWIO_SHFT(DINT_CFG,M0_RAM_EXTEND))
/* Enable DINT_CFG's M0_RAM_EXTEND when F-CCCH, F-BCCH and F-SCH cannot fit in original RAM size */

/* Task list settings */
#define DINT_TASK_LIST_ACTIVE_TASK_M  (0x01 << 15)
#define DINT_TASK_LIST_ACTIVE_TASK_V  (0x01 << 15)
#define DINT_TASK_LIST_SUB_CHANS_EN_V (0x01 << 14)
#define DINT_TASK_LIST_SYNC_CHAN_EN_V (0x01 << 13)
#define DINT_TASK_LIST_DEC_INTR_EN_M  (0x01 << 12)
#define DINT_TASK_LIST_DEC_INTR_EN_V  (0x01 << 12)
#define DINT_TASK_LIST_DEC_INTR_DIS_V (0x00 << 12)
#define DINT_TASK_LIST_CH0_M          (0x00 << 10)
#define DINT_TASK_LIST_CH0_V          (0x00 << 10)
#define DINT_TASK_LIST_CH1_M          (0x01 << 10)
#define DINT_TASK_LIST_CH1_V          (0x01 << 10)
#define DINT_TASK_LIST_CH2_V          (0x02 << 10)
#define DINT_TASK_LIST_FRM_HYP_20MS_M (0x02 << 8)
#define DINT_TASK_LIST_FRM_HYP_20MS_V (0x02 << 8)
#define DINT_TASK_LIST_FRM_HYP_5MS_V  (0x00 << 8)

#define DINT_FIRST_5MS_SLOT_ADDR  0x0000
#define DINT_SECOND_5MS_SLOT_ADDR 0x0020
#define DINT_THIRD_5MS_SLOT_ADDR  0x0040
#define DINT_FOURTH_5MS_SLOT_ADDR 0x0060

/*=========================================================================
    DEINTERLEAVER PACKET PARAMETERS MACROS
==========================================================================*/

typedef struct
{
  word pkt_intlv_cfg_lo;
  /* row_width | chainback_depth | ym_thresh | smt_scalar */
  word pkt_intlv_cfg_hi;
  /* intlv_cols | intlv_rows */
  word pkt_blk_size_lo;
  /* Block size */
  word pkt_blk_size_hi;
  /* punct_length | symbol_repeat | code_rate */
  word pkt_punct_lo;
  /* lo_punct_pattern | rdcnt_en_sel | num_ltu_frm */
  word pkt_punct_hi;
  /* punct_pattern_hi */
  word pkt_pl_crc_lo;
  /* Physical Layer CRC Polynomial */
  word pkt_pl_crc_hi;
  /* LTU CRC Polynomial */

} deint_pkt_params_type;

typedef struct
{
  deint_pkt_params_type fch_full_rate_param;
  deint_pkt_params_type fch_half_rate_param;
  deint_pkt_params_type fch_quarter_rate_param;
  deint_pkt_params_type fch_eighth_rate_param;

} deint_fch_pkt_cfg_type;

typedef struct
{
  deint_pkt_params_type sc_param;
} deint_sc_pkt_cfg_type;

typedef struct
{
  deint_pkt_params_type pc_params[PRAT_COUNT];
} deint_pc_pkt_cfg_type;

typedef deint_pkt_params_type deint_sch_pkt_cfg_type;

/*=============================================================
   DEINTRELEAVER & TURBO DECODER Params for Turbo Codes
================================================================*/
typedef struct
{
  word td_block_size;
  /* Deinterleaver TD block size. Indicates the block size, N
     for de-interleaving. Programmed value should be N-1. */
  word td_intlv_cfg_lo;
  /* bits 13:15 indicate Row_width for the DeInterleaver(m=logM)
     bits 0:12   reserved */
  word td_intlv_cfg_hi;
  /* bits 8:15 indicate number of columns for deinterleaver.
     bits 0:7 indicate number of rows (M = 2 ^ (m-1) )  */
  word td_punct_lo;
  /* Punct_Pattern | Punct_Length | TD_DEC_INT_EN | Code_Rate */
  word td_punct_hi;
  /* bits 0:15 indicate the 16_upper bits of the puncturing pattern */
  word td_intlv_size_lo;
  /* bits 0:14 indicate the Turbo decoder interleaver length*/

  dword td_common_cfg0;
  /* bits   :0  IRQ_TIMING_SEL
     bits   :1  unused
     bits  3:2  CRC_WIDTH_SEL
     bits  4:31 reserved */

  dword td_common_cfg1;
  /* bits  0:12 INTL_LEN_M1
     bits 13:26 MIN_LLR
     bits 27:30 LLR_SCALE_SEL
     bits   :31 reserved */

  dword td_common_cfg2;
  /* bits  0:7  NUM_COL_M1
     bits  8:15 LAST_COL_M1
     bits 16:20 LAST_ROW_M1
     bits 21:24 MIN_ITER
     bits 25:28 MAX_ITER
     bits   :29 EARLY_TERM_EN
     bits 30:31 NUM_TD_MAP */

  dword td_common_cfg3;
  /* bits  0:4  FWD0_ITV_ROW_BND
     bits  5:9  FWD1_ITV_ROW_BND
     bits 10:14 FWD2_ITV_ROW_BND
     bits   :15 unused
     bits 16:20 FWD0_SEQ_ROW_BND
     bits 21:25 FWD1_SEQ_ROW_BND
     bits 26:30 FWD2_SEQ_ROW_BND
     bits   :31 unused */

  dword td_common_cfg4;
  /* bits  0:7  FWD0_ITV_COL_BND
     bits  8:15 FWD1_ITV_COL_BND
     bits 16:23 FWD2_ITV_COL_BND
     bits 24:31 unused */

  dword td_common_cfg5;
  /* bits  0:7  FWD0_SEQ_COL_BND
     bits  8:15 FWD1_SEQ_COL_BND
     bits 16:23 FWD2_SEQ_COL_BND
     bits 24:31 unused */

  dword td_1x_cfg;
  /* bits   :0  TDECIB_IDX
     bits  1:2  unused
     bits  3:6  CONSEC_CRC_M1
     bits  7:8  unused
     bits   :9  DEC_BIAS
     bits 10:31 reserved */

} deint_td_params_type;

/*=======================================================================

   Data Structures for Deinterleaver/Decoder Sequencing Commands to
   Program Read Parameters.

========================================================================*/
typedef enum
{
  DEINT_FCH_READ_PARAMS,
#ifdef FEATURE_IS2000_REL_A_SVD
  DEINT_DCCH_READ_PARAMS,
#endif /* FEATURE_IS2000_REL_A_SVD */
  DEINT_SCH_READ_PARAMS
}  deint_sequencing_cmd_name_type;

typedef struct
{
  q_link_type link;   /* Queue Link */
  q_type *done_q_ptr;
  deint_sequencing_cmd_name_type command;
}  deint_hdr_type;

typedef struct
{
  deint_hdr_type hdr;
  deint_rc_type fch_rc;
#ifdef FEATURE_IS2000_REL_A_SVD
  boolean fch_pc;
    /* Indicates if this is the power control chn */
#endif /* FEATURE_IS2000_REL_A_SVD */
}  deint_fch_read_params_cmd_type;

#ifdef FEATURE_IS2000_REL_A_SVD
typedef struct
{
  deint_hdr_type hdr;
  deint_rc_type dcch_rc;
  boolean dcch_pc;
    /* Indicates if this is the power control chn */
}  deint_dcch_read_params_cmd_type;
#endif /* FEATURE_IS2000_REL_A_SVD */

typedef struct
{
  deint_hdr_type hdr;
  deint_rc_type sch_rc;
  dec_coding_type dec_coding;
  dec_sch_rate_type  dec_sch_rate;
#ifdef FEATURE_IS2000_REL_A //32X
  uint8 num_ltu;
  uint16 ltu_len;
  dec_mux_pdu_type_type pdu_type;
#endif /* FEATURE_IS2000_REL_A */
}  deint_sch_read_params_cmd_type;

/* Union of all the sequencing commands */
typedef union
{
  deint_hdr_type   hdr;
  deint_fch_read_params_cmd_type fch_cmd;
#ifdef FEATURE_IS2000_REL_A_SVD
  deint_dcch_read_params_cmd_type dcch_cmd;
#endif /* FEATURE_IS2000_REL_A_SVD */
  deint_sch_read_params_cmd_type sch_cmd;
}  deint_sequencing_cmd_type;

#ifdef FEATURE_IS2000_REL_A_SVD
/*=============================================================
   DEINTRELEAVER values to support Simultaneous FCH and DCCH.
================================================================*/
/* Used to decide whether or not Decoder should be reset.  This is used to
** avoid resetting the decoder multiple times while the mobile is in traffic
** state.
*/
typedef enum
{
  DEINT_CDMA_STATE,
  DEINT_SYNC_STATE,
  DEINT_PAGING_IDLE_STATE,
  DEINT_TRAFFIC_STATE

}  deint_phone_state_type;

/* Used to determine how to create the FCH/DCCH task lists.
*/
typedef enum
{
  DEINT_CH_INACTIVE,
  DEINT_FCH_ACTIVE,
  DEINT_DCCH_ACTIVE,
  DEINT_FCH_DCCH_ACTIVE

}  deint_fundicated_ch_active_type;

#endif /* FEATURE_IS2000_REL_A_SVD */

/*=========================================================================

    DEINTERLEAVER PACKET PARAMETERS FOR SYNC CHANNEL

==========================================================================*/

#define DINT_PKT_SC_ROW_WIDTH_V         (0x04 << 13)
#define DINT_PKT_SC_CHAINBACK_DEPTH_V   (0x00 << 12)
#define DINT_PKT_SC_YM_THRESH_V         (0x01 << 8)
#define DINT_PKT_SC_SMT_SCALAR_V        (0xb5)
#define DINT_PKT_SC_INTLV_COLS_V        (0x01 << 8)
#define DINT_PKT_SC_INTLV_ROWS_V        (0x00 << 0)
#define DINT_PKT_SC_BLOCK_SIZE_V        (0x0180)
#define DINT_PKT_SC_PUNCT_LENGTH_V      (0x00 << 4)
#define DINT_PKT_SC_SYMBOL_REPEAT_V     (0x01 << 2)
#define DINT_PKT_SC_CODE_RATE_V         (0x00)
#define DINT_PKT_SC_LO_PUNCT_PATTERN_V  (0xff << 8)
#define DINT_PKT_SC_RDCNT_EN_SEL_V      (0x00 << 4)
#define DINT_PKT_SC_HI_PUNCT_PATTERN_V  (0xffff)
#define DINT_PKT_SC_PL_CRC_V            (0x0000)
#define DINT_PKT_SC_LTU_CRC_V           (0x0000)
#define DINT_PKT_SC_NUM_LTU_FRM_V       (0x00)


/*========================================================================

       DEINTERLEAVER PACKET PARAMETERS FOR FCH FOR RC1 - RC5

==========================================================================*/

#define DINT_PKT_FCH_RC1_ALL_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_FCH_RC1_ALL_RATE_CHAINBACK_DEPTH_V       (0x00 << 12)
#define DINT_PKT_FCH_RC1_FULL_RATE_YM_THRESH_V            (0x01 << 8)
#define DINT_PKT_FCH_RC1_HALF_RATE_YM_THRESH_V            (0x01 << 8)
#define DINT_PKT_FCH_RC1_QUARTER_RATE_YM_THRESH_V         (0x03 << 8)
#define DINT_PKT_FCH_RC1_EIGHTH_RATE_YM_THRESH_V          (0x03 << 8)
#define DINT_PKT_FCH_RC1_FULL_RATE_SMT_SCALAR_V           (0xff)
#define DINT_PKT_FCH_RC1_HALF_RATE_SMT_SCALAR_V           (0xb5)
#define DINT_PKT_FCH_RC1_QUARTER_RATE_SMT_SCALAR_V        (0x80)
#define DINT_PKT_FCH_RC1_EIGHTH_RATE_SMT_SCALAR_V         (0x5a)
#define DINT_PKT_FCH_RC1_ALL_RATE_INTLV_COLS_V            (0x06 << 8)
#define DINT_PKT_FCH_RC1_ALL_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_FCH_RC1_ALL_RATE_BLOCK_SIZE_V            (0x0180)
#define DINT_PKT_FCH_RC1_ALL_RATE_PUNCT_LENGTH_V          (0x00 << 4)
#define DINT_PKT_FCH_RC1_FULL_RATE_SYMBOL_REPEAT_V        (0x00 << 2)
#define DINT_PKT_FCH_RC1_HALF_RATE_SYMBOL_REPEAT_V        (0x01 << 2)
#define DINT_PKT_FCH_RC1_QUARTER_RATE_SYMBOL_REPEAT_V     (0x02 << 2)
#define DINT_PKT_FCH_RC1_EIGHTH_RATE_SYMBOL_REPEAT_V      (0x03 << 2)
#define DINT_PKT_FCH_RC1_ALL_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_FCH_RC1_ALL_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_FCH_RC1_ALL_RATE_RDCNT_EN_SEL_V          (0x00 << 4)
#define DINT_PKT_FCH_RC1_ALL_RATE_HI_PUNCT_PATTERN_V      (0xffff)
#define DINT_PKT_FCH_RC1_FULL_RATE_PL_CRC_V               (0xf130)
#define DINT_PKT_FCH_RC1_HALF_RATE_PL_CRC_V               (0x9b00)
#define DINT_PKT_FCH_RC1_QUARTER_RATE_PL_CRC_V            (0x0000)
#define DINT_PKT_FCH_RC1_EIGHTH_RATE_PL_CRC_V             (0x0000)
#define DINT_PKT_FCH_RC1_ALL_RATE_LTU_CRC_V               (0x0000)
#define DINT_PKT_FCH_RC1_ALL_RATE_NUM_LTU_FRM_V           (0x00)


#define DINT_PKT_FCH_RC2_ALL_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_FCH_RC2_ALL_RATE_CHAINBACK_DEPTH_V       (0x01 << 12)
#define DINT_PKT_FCH_RC2_FULL_RATE_YM_THRESH_V            (0x01 << 8)
#define DINT_PKT_FCH_RC2_HALF_RATE_YM_THRESH_V            (0x01 << 8)
#define DINT_PKT_FCH_RC2_QUARTER_RATE_YM_THRESH_V         (0x03 << 8)
#define DINT_PKT_FCH_RC2_EIGHTH_RATE_YM_THRESH_V          (0x03 << 8)
#define DINT_PKT_FCH_RC2_FULL_RATE_SMT_SCALAR_V           (0xff)
#define DINT_PKT_FCH_RC2_HALF_RATE_SMT_SCALAR_V           (0xb5)
#define DINT_PKT_FCH_RC2_QUARTER_RATE_SMT_SCALAR_V        (0x80)
#define DINT_PKT_FCH_RC2_EIGHTH_RATE_SMT_SCALAR_V         (0x5a)
#define DINT_PKT_FCH_RC2_ALL_RATE_INTLV_COLS_V            (0x06 << 8)
#define DINT_PKT_FCH_RC2_ALL_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_FCH_RC2_ALL_RATE_BLOCK_SIZE_V            (0x0180)
#define DINT_PKT_FCH_RC2_ALL_RATE_PUNCT_LENGTH_V          (0x05 << 4)
#define DINT_PKT_FCH_RC2_FULL_RATE_SYMBOL_REPEAT_V        (0x00 << 2)
#define DINT_PKT_FCH_RC2_HALF_RATE_SYMBOL_REPEAT_V        (0x01 << 2)
#define DINT_PKT_FCH_RC2_QUARTER_RATE_SYMBOL_REPEAT_V     (0x02 << 2)
#define DINT_PKT_FCH_RC2_EIGHTH_RATE_SYMBOL_REPEAT_V      (0x03 << 2)
#define DINT_PKT_FCH_RC2_ALL_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_FCH_RC2_ALL_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_FCH_RC2_ALL_RATE_RDCNT_EN_SEL_V          (0x00 << 4)
#define DINT_PKT_FCH_RC2_ALL_RATE_HI_PUNCT_PATTERN_V      (0xd7ff)
#define DINT_PKT_FCH_RC2_FULL_RATE_PL_CRC_V               (0xf130)
#define DINT_PKT_FCH_RC2_HALF_RATE_PL_CRC_V               (0xf640)
#define DINT_PKT_FCH_RC2_QUARTER_RATE_PL_CRC_V            (0x9b00)
#define DINT_PKT_FCH_RC2_EIGHTH_RATE_PL_CRC_V             (0x1c00)
#define DINT_PKT_FCH_RC2_ALL_RATE_LTU_CRC_V               (0x0000)
#define DINT_PKT_FCH_RC2_ALL_RATE_NUM_LTU_FRM_V           (0x00)


#define DINT_PKT_FCH_RC3_ALL_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_FCH_RC3_ALL_RATE_CHAINBACK_DEPTH_V       (0x00 << 12)
#define DINT_PKT_FCH_RC3_ALL_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_FCH_RC3_FULL_RATE_SMT_SCALAR_V           (0xff)
#define DINT_PKT_FCH_RC3_HALF_RATE_SMT_SCALAR_V           (0xb5)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_SMT_SCALAR_V        (0x80)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_SMT_SCALAR_V         (0x5a)
#define DINT_PKT_FCH_RC3_ALL_RATE_INTLV_COLS_V            (0x0c << 8)
#define DINT_PKT_FCH_RC3_ALL_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_FCH_RC3_ALL_RATE_BLOCK_SIZE_V            (0x0300)
#define DINT_PKT_FCH_RC3_FULL_RATE_PUNCT_LENGTH_V         (0x00 << 4)
#define DINT_PKT_FCH_RC3_HALF_RATE_PUNCT_LENGTH_V         (0x00 << 4)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_PUNCT_LENGTH_V      (0x08 << 4)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_PUNCT_LENGTH_V       (0x04 << 4)
#define DINT_PKT_FCH_RC3_FULL_RATE_SYMBOL_REPEAT_V        (0x00 << 2)
#define DINT_PKT_FCH_RC3_HALF_RATE_SYMBOL_REPEAT_V        (0x01 << 2)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_SYMBOL_REPEAT_V     (0x02 << 2)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_SYMBOL_REPEAT_V      (0x03 << 2)
#define DINT_PKT_FCH_RC3_ALL_RATE_CODE_RATE_V             (0x02)
#define DINT_PKT_FCH_RC3_ALL_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_FCH_RC3_FULL_RATE_RDCNT_EN_SEL_V         (0x00 << 4)
#define DINT_PKT_FCH_RC3_HALF_RATE_RDCNT_EN_SEL_V         (0x00 << 4)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_RDCNT_EN_SEL_V      (0x00 << 4)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_RDCNT_EN_SEL_V       (0x01 << 4)
#define DINT_PKT_FCH_RC3_FULL_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_PKT_FCH_RC3_HALF_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_HI_PUNCT_PATTERN_V  (0xff7f)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_HI_PUNCT_PATTERN_V   (0xf7ff)
#define DINT_PKT_FCH_RC3_FULL_RATE_PL_CRC_V               (0xf130)
#define DINT_PKT_FCH_RC3_HALF_RATE_PL_CRC_V               (0x9b00)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_PL_CRC_V            (0x9c00)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_PL_CRC_V             (0x9c00)
#define DINT_PKT_FCH_RC3_ALL_RATE_LTU_CRC_V               (0x0000)
#define DINT_PKT_FCH_RC3_ALL_RATE_NUM_LTU_FRM_V           (0x00)

#define DINT_PKT_FCH_RC4_ALL_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_FCH_RC4_ALL_RATE_CHAINBACK_DEPTH_V       (0x00 << 12)
#define DINT_PKT_FCH_RC4_ALL_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_FCH_RC4_FULL_RATE_SMT_SCALAR_V           (0xff)
#define DINT_PKT_FCH_RC4_HALF_RATE_SMT_SCALAR_V           (0xb5)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_SMT_SCALAR_V        (0x80)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_SMT_SCALAR_V         (0x5a)
#define DINT_PKT_FCH_RC4_ALL_RATE_INTLV_COLS_V            (0x06 << 8)
#define DINT_PKT_FCH_RC4_ALL_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_FCH_RC4_ALL_RATE_BLOCK_SIZE_V            (0x0180)
#define DINT_PKT_FCH_RC4_FULL_RATE_PUNCT_LENGTH_V         (0x00 << 4)
#define DINT_PKT_FCH_RC4_HALF_RATE_PUNCT_LENGTH_V         (0x00 << 4)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_PUNCT_LENGTH_V      (0x08 << 4)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_PUNCT_LENGTH_V       (0x04 << 4)
#define DINT_PKT_FCH_RC4_FULL_RATE_SYMBOL_REPEAT_V        (0x00 << 2)
#define DINT_PKT_FCH_RC4_HALF_RATE_SYMBOL_REPEAT_V        (0x01 << 2)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_SYMBOL_REPEAT_V     (0x02 << 2)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_SYMBOL_REPEAT_V      (0x03 << 2)
#define DINT_PKT_FCH_RC4_ALL_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_FCH_RC4_ALL_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_FCH_RC4_ALL_RATE_RDCNT_EN_SEL_V          (0x00 << 4)
#define DINT_PKT_FCH_RC4_FULL_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_PKT_FCH_RC4_HALF_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_HI_PUNCT_PATTERN_V  (0xff7f)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_HI_PUNCT_PATTERN_V   (0xf7ff)
#define DINT_PKT_FCH_RC4_FULL_RATE_PL_CRC_V               (0xf130)
#define DINT_PKT_FCH_RC4_HALF_RATE_PL_CRC_V               (0x9b00)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_PL_CRC_V            (0x9c00)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_PL_CRC_V             (0x9c00)
#define DINT_PKT_FCH_RC4_ALL_RATE_LTU_CRC_V               (0x0000)
#define DINT_PKT_FCH_RC4_ALL_RATE_NUM_LTU_FRM_V           (0x00)


#define DINT_PKT_FCH_RC5_ALL_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_FCH_RC5_ALL_RATE_CHAINBACK_DEPTH_V       (0x01 << 12)
#define DINT_PKT_FCH_RC5_ALL_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_FCH_RC5_FULL_RATE_SMT_SCALAR_V           (0xff)
#define DINT_PKT_FCH_RC5_HALF_RATE_SMT_SCALAR_V           (0xb5)
#define DINT_PKT_FCH_RC5_QUARTER_RATE_SMT_SCALAR_V        (0x80)
#define DINT_PKT_FCH_RC5_EIGHTH_RATE_SMT_SCALAR_V         (0x5a)
#define DINT_PKT_FCH_RC5_ALL_RATE_INTLV_COLS_V            (0x0c << 8)
#define DINT_PKT_FCH_RC5_ALL_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_FCH_RC5_ALL_RATE_BLOCK_SIZE_V            (0x0300)
#define DINT_PKT_FCH_RC5_ALL_RATE_PUNCT_LENGTH_V          (0x0b << 4)
#define DINT_PKT_FCH_RC5_FULL_RATE_SYMBOL_REPEAT_V        (0x00 << 2)
#define DINT_PKT_FCH_RC5_HALF_RATE_SYMBOL_REPEAT_V        (0x01 << 2)
#define DINT_PKT_FCH_RC5_QUARTER_RATE_SYMBOL_REPEAT_V     (0x02 << 2)
#define DINT_PKT_FCH_RC5_EIGHTH_RATE_SYMBOL_REPEAT_V      (0x03 << 2)
#define DINT_PKT_FCH_RC5_ALL_RATE_CODE_RATE_V             (0x02)
#define DINT_PKT_FCH_RC5_ALL_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_FCH_RC5_FULL_RATE_RDCNT_EN_SEL_V         (0x00 << 4)
#define DINT_PKT_FCH_RC5_HALF_RATE_RDCNT_EN_SEL_V         (0x00 << 4)
#define DINT_PKT_FCH_RC5_QUARTER_RATE_RDCNT_EN_SEL_V      (0x00 << 4)
#define DINT_PKT_FCH_RC5_EIGHTH_RATE_RDCNT_EN_SEL_V       (0x01 << 4)
#define DINT_PKT_FCH_RC5_ALL_RATE_HI_PUNCT_PATTERN_V      (0xd9bf)
#define DINT_PKT_FCH_RC5_FULL_RATE_PL_CRC_V               (0xf130)
#define DINT_PKT_FCH_RC5_HALF_RATE_PL_CRC_V               (0xf640)
#define DINT_PKT_FCH_RC5_QUARTER_RATE_PL_CRC_V            (0x9b00)
#define DINT_PKT_FCH_RC5_EIGHTH_RATE_PL_CRC_V             (0x9c00)
#define DINT_PKT_FCH_RC5_ALL_RATE_LTU_CRC_V               (0x0000)
#define DINT_PKT_FCH_RC5_ALL_RATE_NUM_LTU_FRM_V           (0x00)

#define DINT_PKT_SCH_RC3_1X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC3_1X_RATE_CHAINBACK_DEPTH_V        (0x00 << 12)
#define DINT_PKT_SCH_RC3_1X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC3_1X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC3_1X_RATE_INTLV_COLS_V             (0x0c << 8)
#define DINT_PKT_SCH_RC3_1X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC3_1X_RATE_BLOCK_SIZE_V             (0x0300)
#define DINT_PKT_SCH_RC3_1X_RATE_LTU_LEN_HI_V             (0x00 << 9)
#define DINT_PKT_SCH_RC3_1X_RATE_LTU_LEN_LO_V             (0x00 << 6)
#define DINT_PKT_SCH_RC3_1X_RATE_PUNCT_LENGTH_V           (0x00 << 4)
#define DINT_PKT_SCH_RC3_1X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC3_1X_RATE_CODE_RATE_V              (0x02)
#define DINT_PKT_SCH_RC3_1X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC3_1X_RATE_RDCNT_EN_SEL_V           (0x00 << 4)
#define DINT_PKT_SCH_RC3_1X_RATE_NUM_LTU_FRM_V            (0x00)
#define DINT_PKT_SCH_RC3_1X_RATE_HI_PUNCT_PATTERN_V       (0xffff)
#define DINT_PKT_SCH_RC3_1X_RATE_PL_CRC_V                 (0xf130)
#define DINT_PKT_SCH_RC3_1X_RATE_LTU_CRC_V                (0x0000)

#define DINT_PKT_SCH_RC3_2X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC3_2X_RATE_CHAINBACK_DEPTH_V        (0x00 << 12)
#define DINT_PKT_SCH_RC3_2X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC3_2X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC3_2X_RATE_INTLV_COLS_V             (0x18 << 8)
#define DINT_PKT_SCH_RC3_2X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC3_2X_RATE_BLOCK_SIZE_V             (0x0600)
#define DINT_PKT_SCH_RC3_2X_RATE_LTU_LEN_HI_V             (0x00 << 9)
#define DINT_PKT_SCH_RC3_2X_RATE_LTU_LEN_LO_V             (0x00 << 6)
#define DINT_PKT_SCH_RC3_2X_RATE_PUNCT_LENGTH_V           (0x00 << 4)
#define DINT_PKT_SCH_RC3_2X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC3_2X_RATE_CODE_RATE_V              (0x02)
#define DINT_PKT_SCH_RC3_2X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC3_2X_RATE_RDCNT_EN_SEL_V           (0x01 << 4)
#define DINT_PKT_SCH_RC3_2X_RATE_NUM_LTU_FRM_V            (0x00)
#define DINT_PKT_SCH_RC3_2X_RATE_HI_PUNCT_PATTERN_V       (0xffff)
#define DINT_PKT_SCH_RC3_2X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC3_2X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC3_4X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC3_4X_RATE_CHAINBACK_DEPTH_V        (0x01 << 12)
#define DINT_PKT_SCH_RC3_4X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC3_4X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC3_4X_RATE_INTLV_COLS_V             (0x30 << 8)
#define DINT_PKT_SCH_RC3_4X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC3_4X_RATE_BLOCK_SIZE_V             (0x0c00)
#define DINT_PKT_SCH_RC3_4X_RATE_LTU_LEN_HI_V             (0x0b << 9)
#define DINT_PKT_SCH_RC3_4X_RATE_LTU_LEN_LO_V             (0x02 << 6)
#define DINT_PKT_SCH_RC3_4X_RATE_PUNCT_LENGTH_V           (0x00 << 4)
#define DINT_PKT_SCH_RC3_4X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC3_4X_RATE_CODE_RATE_V              (0x02)
#define DINT_PKT_SCH_RC3_4X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC3_4X_RATE_RDCNT_EN_SEL_V           (0x01 << 4)
#define DINT_PKT_SCH_RC3_4X_RATE_NUM_LTU_FRM_V            (0x02)
#define DINT_PKT_SCH_RC3_4X_RATE_HI_PUNCT_PATTERN_V       (0xffff)
#define DINT_PKT_SCH_RC3_4X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC3_4X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC3_8X_RATE_ROW_WIDTH_V              (0x04 << 13)
#define DINT_PKT_SCH_RC3_8X_RATE_CHAINBACK_DEPTH_V        (0x01 << 12)
#define DINT_PKT_SCH_RC3_8X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC3_8X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC3_8X_RATE_INTLV_COLS_V             (0x30 << 8)
#define DINT_PKT_SCH_RC3_8X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC3_8X_RATE_BLOCK_SIZE_V             (0x1800)
#define DINT_PKT_SCH_RC3_8X_RATE_LTU_LEN_HI_V             (0x0b << 9)
#define DINT_PKT_SCH_RC3_8X_RATE_LTU_LEN_LO_V             (0x02 << 6)
#define DINT_PKT_SCH_RC3_8X_RATE_PUNCT_LENGTH_V           (0x00 << 4)
#define DINT_PKT_SCH_RC3_8X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC3_8X_RATE_CODE_RATE_V              (0x02)
#define DINT_PKT_SCH_RC3_8X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC3_8X_RATE_RDCNT_EN_SEL_V           (0x01 << 4)
#define DINT_PKT_SCH_RC3_8X_RATE_NUM_LTU_FRM_V            (0x04)
#define DINT_PKT_SCH_RC3_8X_RATE_HI_PUNCT_PATTERN_V       (0xffff)
#define DINT_PKT_SCH_RC3_8X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC3_8X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC3_16X_RATE_ROW_WIDTH_V             (0x04 << 13)
#define DINT_PKT_SCH_RC3_16X_RATE_CHAINBACK_DEPTH_V       (0x00 << 12)
#define DINT_PKT_SCH_RC3_16X_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_SCH_RC3_16X_RATE_SMT_SCALAR_V            (0xff)
#define DINT_PKT_SCH_RC3_16X_RATE_INTLV_COLS_V            (0x60 << 8)
#define DINT_PKT_SCH_RC3_16X_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_SCH_RC3_16X_RATE_BLOCK_SIZE_V            (0x3000)
#define DINT_PKT_SCH_RC3_16X_RATE_LTU_LEN_HI_V            (0x0b << 9)
#define DINT_PKT_SCH_RC3_16X_RATE_LTU_LEN_LO_V            (0x02 << 6)
#define DINT_PKT_SCH_RC3_16X_RATE_PUNCT_LENGTH_V          (0x00 << 4)
#define DINT_PKT_SCH_RC3_16X_RATE_SYMBOL_REPEAT_V         (0x00 << 2)
#define DINT_PKT_SCH_RC3_16X_RATE_CODE_RATE_V             (0x02)
#define DINT_PKT_SCH_RC3_16X_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_SCH_RC3_16X_RATE_RDCNT_EN_SEL_V          (0x00 << 4)
#define DINT_PKT_SCH_RC3_16X_RATE_NUM_LTU_FRM_V           (0x08)
#define DINT_PKT_SCH_RC3_16X_RATE_HI_PUNCT_PATTERN_V      (0xffff)
#define DINT_PKT_SCH_RC3_16X_RATE_PL_CRC_V                (0xc867)
#define DINT_PKT_SCH_RC3_16X_RATE_LTU_CRC_V               (0xc867)

#define DINT_PKT_SCH_RC4_1X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC4_1X_RATE_CHAINBACK_DEPTH_V        (0x00 << 12)
#define DINT_PKT_SCH_RC4_1X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC4_1X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC4_1X_RATE_INTLV_COLS_V             (0x06 << 8)
#define DINT_PKT_SCH_RC4_1X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC4_1X_RATE_BLOCK_SIZE_V             (0x0180)
#define DINT_PKT_SCH_RC4_1X_RATE_LTU_LEN_HI_V             (0x00 << 9)
#define DINT_PKT_SCH_RC4_1X_RATE_LTU_LEN_LO_V             (0x00 << 6)
#define DINT_PKT_SCH_RC4_1X_RATE_PUNCT_LENGTH_V           (0x00 << 4)
#define DINT_PKT_SCH_RC4_1X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC4_1X_RATE_CODE_RATE_V              (0x00)
#define DINT_PKT_SCH_RC4_1X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC4_1X_RATE_RDCNT_EN_SEL_V           (0x00 << 4)
#define DINT_PKT_SCH_RC4_1X_RATE_NUM_LTU_FRM_V            (0x00)
#define DINT_PKT_SCH_RC4_1X_RATE_HI_PUNCT_PATTERN_V       (0xffff)
#define DINT_PKT_SCH_RC4_1X_RATE_PL_CRC_V                 (0xf130)
#define DINT_PKT_SCH_RC4_1X_RATE_LTU_CRC_V                (0x0000)

#define DINT_PKT_SCH_RC4_2X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC4_2X_RATE_CHAINBACK_DEPTH_V        (0x00 << 12)
#define DINT_PKT_SCH_RC4_2X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC4_2X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC4_2X_RATE_INTLV_COLS_V             (0x0c << 8)
#define DINT_PKT_SCH_RC4_2X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC4_2X_RATE_BLOCK_SIZE_V             (0x0300)
#define DINT_PKT_SCH_RC4_2X_RATE_LTU_LEN_HI_V             (0x00 << 9)
#define DINT_PKT_SCH_RC4_2X_RATE_LTU_LEN_LO_V             (0x00 << 6)
#define DINT_PKT_SCH_RC4_2X_RATE_PUNCT_LENGTH_V           (0x00 << 4)
#define DINT_PKT_SCH_RC4_2X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC4_2X_RATE_CODE_RATE_V              (0x00)
#define DINT_PKT_SCH_RC4_2X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC4_2X_RATE_RDCNT_EN_SEL_V           (0x01 << 4)
#define DINT_PKT_SCH_RC4_2X_RATE_NUM_LTU_FRM_V            (0x00)
#define DINT_PKT_SCH_RC4_2X_RATE_HI_PUNCT_PATTERN_V       (0xffff)
#define DINT_PKT_SCH_RC4_2X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC4_2X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC4_4X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC4_4X_RATE_CHAINBACK_DEPTH_V        (0x01 << 12)
#define DINT_PKT_SCH_RC4_4X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC4_4X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC4_4X_RATE_INTLV_COLS_V             (0x18 << 8)
#define DINT_PKT_SCH_RC4_4X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC4_4X_RATE_BLOCK_SIZE_V             (0x0600)
#define DINT_PKT_SCH_RC4_4X_RATE_LTU_LEN_HI_V             (0x0b << 9)
#define DINT_PKT_SCH_RC4_4X_RATE_LTU_LEN_LO_V             (0x02 << 6)
#define DINT_PKT_SCH_RC4_4X_RATE_PUNCT_LENGTH_V           (0x00 << 4)
#define DINT_PKT_SCH_RC4_4X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC4_4X_RATE_CODE_RATE_V              (0x00)
#define DINT_PKT_SCH_RC4_4X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC4_4X_RATE_RDCNT_EN_SEL_V           (0x01 << 4)
#define DINT_PKT_SCH_RC4_4X_RATE_NUM_LTU_FRM_V            (0x02)
#define DINT_PKT_SCH_RC4_4X_RATE_HI_PUNCT_PATTERN_V       (0xffff)
#define DINT_PKT_SCH_RC4_4X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC4_4X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC4_8X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC4_8X_RATE_CHAINBACK_DEPTH_V        (0x01 << 12)
#define DINT_PKT_SCH_RC4_8X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC4_8X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC4_8X_RATE_INTLV_COLS_V             (0x30 << 8)
#define DINT_PKT_SCH_RC4_8X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC4_8X_RATE_BLOCK_SIZE_V             (0x0c00)
#define DINT_PKT_SCH_RC4_8X_RATE_LTU_LEN_HI_V             (0x0b << 9)
#define DINT_PKT_SCH_RC4_8X_RATE_LTU_LEN_LO_V             (0x02 << 6)
#define DINT_PKT_SCH_RC4_8X_RATE_PUNCT_LENGTH_V           (0x00 << 4)
#define DINT_PKT_SCH_RC4_8X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC4_8X_RATE_CODE_RATE_V              (0x00)
#define DINT_PKT_SCH_RC4_8X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC4_8X_RATE_RDCNT_EN_SEL_V           (0x01 << 4)
#define DINT_PKT_SCH_RC4_8X_RATE_NUM_LTU_FRM_V            (0x04)
#define DINT_PKT_SCH_RC4_8X_RATE_HI_PUNCT_PATTERN_V       (0xffff)
#define DINT_PKT_SCH_RC4_8X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC4_8X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC4_16X_RATE_ROW_WIDTH_V             (0x04 << 13)
#define DINT_PKT_SCH_RC4_16X_RATE_CHAINBACK_DEPTH_V       (0x01 << 12)
#define DINT_PKT_SCH_RC4_16X_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_SCH_RC4_16X_RATE_SMT_SCALAR_V            (0xff)
#define DINT_PKT_SCH_RC4_16X_RATE_INTLV_COLS_V            (0x30 << 8)
#define DINT_PKT_SCH_RC4_16X_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_SCH_RC4_16X_RATE_BLOCK_SIZE_V            (0x1800)
#define DINT_PKT_SCH_RC4_16X_RATE_LTU_LEN_HI_V            (0x0b << 9)
#define DINT_PKT_SCH_RC4_16X_RATE_LTU_LEN_LO_V            (0x02 << 6)
#define DINT_PKT_SCH_RC4_16X_RATE_PUNCT_LENGTH_V          (0x00 << 4)
#define DINT_PKT_SCH_RC4_16X_RATE_SYMBOL_REPEAT_V         (0x00 << 2)
#define DINT_PKT_SCH_RC4_16X_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_SCH_RC4_16X_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_SCH_RC4_16X_RATE_RDCNT_EN_SEL_V          (0x01 << 4)
#define DINT_PKT_SCH_RC4_16X_RATE_NUM_LTU_FRM_V           (0x08)
#define DINT_PKT_SCH_RC4_16X_RATE_HI_PUNCT_PATTERN_V      (0xffff)
#define DINT_PKT_SCH_RC4_16X_RATE_PL_CRC_V                (0xc867)
#define DINT_PKT_SCH_RC4_16X_RATE_LTU_CRC_V               (0xc867)

#define DINT_PKT_SCH_RC4_32X_RATE_ROW_WIDTH_V             (0x04 << 13)
#define DINT_PKT_SCH_RC4_32X_RATE_CHAINBACK_DEPTH_V       (0x00 << 12)
#define DINT_PKT_SCH_RC4_32X_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_SCH_RC4_32X_RATE_SMT_SCALAR_V            (0xff)
#define DINT_PKT_SCH_RC4_32X_RATE_INTLV_COLS_V            (0x60 << 8)
#define DINT_PKT_SCH_RC4_32X_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_SCH_RC4_32X_RATE_BLOCK_SIZE_V            (0x3000)
#define DINT_PKT_SCH_RC4_32X_RATE_LTU_LEN_HI_V            (0x00 << 9)
#define DINT_PKT_SCH_RC4_32X_RATE_LTU_LEN_LO_V            (0x00 << 6)
#define DINT_PKT_SCH_RC4_32X_RATE_PUNCT_LENGTH_V          (0x00 << 4)
#define DINT_PKT_SCH_RC4_32X_RATE_SYMBOL_REPEAT_V         (0x00 << 2)
#define DINT_PKT_SCH_RC4_32X_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_SCH_RC4_32X_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_SCH_RC4_32X_RATE_RDCNT_EN_SEL_V          (0x0 << 4)
#define DINT_PKT_SCH_RC4_32X_RATE_NUM_LTU_FRM_V           (0x0)
#define DINT_PKT_SCH_RC4_32X_RATE_HI_PUNCT_PATTERN_V      (0xffff)
#define DINT_PKT_SCH_RC4_32X_RATE_PL_CRC_V                (0xc867)
#define DINT_PKT_SCH_RC4_32X_RATE_LTU_CRC_V               (0xc867)

#define DINT_PKT_SCH_RC5_1X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC5_1X_RATE_CHAINBACK_DEPTH_V        (0x01 << 12)
#define DINT_PKT_SCH_RC5_1X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC5_1X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC5_1X_RATE_INTLV_COLS_V             (0x0c << 8)
#define DINT_PKT_SCH_RC5_1X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC5_1X_RATE_BLOCK_SIZE_V             (0x0300)
#define DINT_PKT_SCH_RC5_1X_RATE_LTU_LEN_HI_V             (0x00 << 9)
#define DINT_PKT_SCH_RC5_1X_RATE_LTU_LEN_LO_V             (0x00 << 6)
#define DINT_PKT_SCH_RC5_1X_RATE_PUNCT_LENGTH_V           (0x0B << 4)
#define DINT_PKT_SCH_RC5_1X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC5_1X_RATE_CODE_RATE_V              (0x02)
#define DINT_PKT_SCH_RC5_1X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC5_1X_RATE_RDCNT_EN_SEL_V           (0x00 << 4)
#define DINT_PKT_SCH_RC5_1X_RATE_NUM_LTU_FRM_V            (0x00)
#define DINT_PKT_SCH_RC5_1X_RATE_HI_PUNCT_PATTERN_V       (0xd9bf)
#define DINT_PKT_SCH_RC5_1X_RATE_PL_CRC_V                 (0xf130)
#define DINT_PKT_SCH_RC5_1X_RATE_LTU_CRC_V                (0x0000)

#define DINT_PKT_SCH_RC5_2X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC5_2X_RATE_CHAINBACK_DEPTH_V        (0x01 << 12)
#define DINT_PKT_SCH_RC5_2X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC5_2X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC5_2X_RATE_INTLV_COLS_V             (0x18 << 8)
#define DINT_PKT_SCH_RC5_2X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC5_2X_RATE_BLOCK_SIZE_V             (0x0600)
#define DINT_PKT_SCH_RC5_2X_RATE_LTU_LEN_HI_V             (0x00 << 9)
#define DINT_PKT_SCH_RC5_2X_RATE_LTU_LEN_LO_V             (0x00 << 6)
#define DINT_PKT_SCH_RC5_2X_RATE_PUNCT_LENGTH_V           (0x0B << 4)
#define DINT_PKT_SCH_RC5_2X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC5_2X_RATE_CODE_RATE_V              (0x02)
#define DINT_PKT_SCH_RC5_2X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC5_2X_RATE_RDCNT_EN_SEL_V           (0x01 << 4)
#define DINT_PKT_SCH_RC5_2X_RATE_NUM_LTU_FRM_V            (0x00)
#define DINT_PKT_SCH_RC5_2X_RATE_HI_PUNCT_PATTERN_V       (0xd9bf)
#define DINT_PKT_SCH_RC5_2X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC5_2X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC5_4X_RATE_ROW_WIDTH_V              (0x03 << 13)
#define DINT_PKT_SCH_RC5_4X_RATE_CHAINBACK_DEPTH_V        (0x01 << 12)
#define DINT_PKT_SCH_RC5_4X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC5_4X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC5_4X_RATE_INTLV_COLS_V             (0x30 << 8)
#define DINT_PKT_SCH_RC5_4X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC5_4X_RATE_BLOCK_SIZE_V             (0x0c00)
#define DINT_PKT_SCH_RC5_4X_RATE_LTU_LEN_HI_V             (0x11 << 9)
#define DINT_PKT_SCH_RC5_4X_RATE_LTU_LEN_LO_V             (0x02 << 6)
#define DINT_PKT_SCH_RC5_4X_RATE_PUNCT_LENGTH_V           (0x0B << 4)
#define DINT_PKT_SCH_RC5_4X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC5_4X_RATE_CODE_RATE_V              (0x02)
#define DINT_PKT_SCH_RC5_4X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC5_4X_RATE_RDCNT_EN_SEL_V           (0x00 << 4)
#define DINT_PKT_SCH_RC5_4X_RATE_NUM_LTU_FRM_V            (0x02)
#define DINT_PKT_SCH_RC5_4X_RATE_HI_PUNCT_PATTERN_V       (0xd9bf)
#define DINT_PKT_SCH_RC5_4X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC5_4X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC5_8X_RATE_ROW_WIDTH_V              (0x04 << 13)
#define DINT_PKT_SCH_RC5_8X_RATE_CHAINBACK_DEPTH_V        (0x01 << 12)
#define DINT_PKT_SCH_RC5_8X_RATE_YM_THRESH_V              (0x01 << 8)
#define DINT_PKT_SCH_RC5_8X_RATE_SMT_SCALAR_V             (0xff)
#define DINT_PKT_SCH_RC5_8X_RATE_INTLV_COLS_V             (0x30 << 8)
#define DINT_PKT_SCH_RC5_8X_RATE_INTLV_ROWS_V             (0x00 << 0)
#define DINT_PKT_SCH_RC5_8X_RATE_BLOCK_SIZE_V             (0x1800)
#define DINT_PKT_SCH_RC5_8X_RATE_LTU_LEN_HI_V             (0x11 << 9)
#define DINT_PKT_SCH_RC5_8X_RATE_LTU_LEN_LO_V             (0x02 << 6)
#define DINT_PKT_SCH_RC5_8X_RATE_PUNCT_LENGTH_V           (0x0B << 4)
#define DINT_PKT_SCH_RC5_8X_RATE_SYMBOL_REPEAT_V          (0x00 << 2)
#define DINT_PKT_SCH_RC5_8X_RATE_CODE_RATE_V              (0x02)
#define DINT_PKT_SCH_RC5_8X_RATE_LO_PUNCT_PATTERN_V       (0xff << 8)
#define DINT_PKT_SCH_RC5_8X_RATE_RDCNT_EN_SEL_V           (0x01 << 4)
#define DINT_PKT_SCH_RC5_8X_RATE_NUM_LTU_FRM_V            (0x04)
#define DINT_PKT_SCH_RC5_8X_RATE_HI_PUNCT_PATTERN_V       (0xd9bf)
#define DINT_PKT_SCH_RC5_8X_RATE_PL_CRC_V                 (0xc867)
#define DINT_PKT_SCH_RC5_8X_RATE_LTU_CRC_V                (0xc867)

#define DINT_PKT_SCH_RC5_16X_RATE_ROW_WIDTH_V             (0x04 << 13)
#define DINT_PKT_SCH_RC5_16X_RATE_CHAINBACK_DEPTH_V       (0x01 << 12)
#define DINT_PKT_SCH_RC5_16X_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_SCH_RC5_16X_RATE_SMT_SCALAR_V            (0xff)
#define DINT_PKT_SCH_RC5_16X_RATE_INTLV_COLS_V            (0x60 << 8)
#define DINT_PKT_SCH_RC5_16X_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_SCH_RC5_16X_RATE_BLOCK_SIZE_V            (0x3000)
#define DINT_PKT_SCH_RC5_16X_RATE_LTU_LEN_HI_V            (0x11 << 9)
#define DINT_PKT_SCH_RC5_16X_RATE_LTU_LEN_LO_V            (0x02 << 6)
#define DINT_PKT_SCH_RC5_16X_RATE_PUNCT_LENGTH_V          (0x0B << 4)
#define DINT_PKT_SCH_RC5_16X_RATE_SYMBOL_REPEAT_V         (0x00 << 2)
#define DINT_PKT_SCH_RC5_16X_RATE_CODE_RATE_V             (0x02)
#define DINT_PKT_SCH_RC5_16X_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_SCH_RC5_16X_RATE_RDCNT_EN_SEL_V          (0x00 << 4)
#define DINT_PKT_SCH_RC5_16X_RATE_NUM_LTU_FRM_V           (0x08)
#define DINT_PKT_SCH_RC5_16X_RATE_HI_PUNCT_PATTERN_V      (0xd9bf)
#define DINT_PKT_SCH_RC5_16X_RATE_PL_CRC_V                (0xc867)
#define DINT_PKT_SCH_RC5_16X_RATE_LTU_CRC_V               (0xc867)

/*========================================================================

     DEINTERLEAVER PACKET PARAMETERS FOR SCH FOR RC11

==========================================================================*/

#define DINT_PKT_SCH_RC11_1X_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_SCH_RC11_1X_RATE_CHAINBACK_DEPTH_V       (0x00 << 12)
#define DINT_PKT_SCH_RC11_1X_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_SCH_RC11_1X_RATE_SMT_SCALAR_V            (0xff)
#define DINT_PKT_SCH_RC11_1X_RATE_INTLV_COLS_V            (0x06 << 8)
#define DINT_PKT_SCH_RC11_1X_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_SCH_RC11_1X_RATE_BLOCK_SIZE_V            (0x0180)
#define DINT_PKT_SCH_RC11_1X_RATE_LTU_LEN_HI_V            (0x00 << 9)
#define DINT_PKT_SCH_RC11_1X_RATE_LTU_LEN_LO_V            (0x00 << 6)
#define DINT_PKT_SCH_RC11_1X_RATE_PUNCT_LENGTH_V          (0x00 << 4)
#define DINT_PKT_SCH_RC11_1X_RATE_SYMBOL_REPEAT_V         (0x00 << 2)
#define DINT_PKT_SCH_RC11_1X_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_SCH_RC11_1X_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_SCH_RC11_1X_RATE_RDCNT_EN_SEL_V          (0x00 << 4)
#define DINT_PKT_SCH_RC11_1X_RATE_NUM_LTU_FRM_V           (0x00)
#define DINT_PKT_SCH_RC11_1X_RATE_HI_PUNCT_PATTERN_V      (0xffff)
#define DINT_PKT_SCH_RC11_1X_RATE_PL_CRC_V                (0xf130)
#define DINT_PKT_SCH_RC11_1X_RATE_LTU_CRC_V               (0x0000)

#define DINT_PKT_SCH_RC11_2X_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_SCH_RC11_2X_RATE_CHAINBACK_DEPTH_V       (0x00 << 12)
#define DINT_PKT_SCH_RC11_2X_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_SCH_RC11_2X_RATE_SMT_SCALAR_V            (0xff)
#define DINT_PKT_SCH_RC11_2X_RATE_INTLV_COLS_V            (0x0c << 8)
#define DINT_PKT_SCH_RC11_2X_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_SCH_RC11_2X_RATE_BLOCK_SIZE_V            (0x0300)
#define DINT_PKT_SCH_RC11_2X_RATE_LTU_LEN_HI_V            (0x00 << 9)
#define DINT_PKT_SCH_RC11_2X_RATE_LTU_LEN_LO_V            (0x00 << 6)
#define DINT_PKT_SCH_RC11_2X_RATE_PUNCT_LENGTH_V          (0x00 << 4)
#define DINT_PKT_SCH_RC11_2X_RATE_SYMBOL_REPEAT_V         (0x00 << 2)
#define DINT_PKT_SCH_RC11_2X_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_SCH_RC11_2X_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_SCH_RC11_2X_RATE_RDCNT_EN_SEL_V          (0x01 << 4)
#define DINT_PKT_SCH_RC11_2X_RATE_NUM_LTU_FRM_V           (0x00)
#define DINT_PKT_SCH_RC11_2X_RATE_HI_PUNCT_PATTERN_V      (0xffff)
#define DINT_PKT_SCH_RC11_2X_RATE_PL_CRC_V                (0xc867)
#define DINT_PKT_SCH_RC11_2X_RATE_LTU_CRC_V               (0xc867)

#define DINT_PKT_SCH_RC11_4X_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_SCH_RC11_4X_RATE_CHAINBACK_DEPTH_V       (0x01 << 12)
#define DINT_PKT_SCH_RC11_4X_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_SCH_RC11_4X_RATE_SMT_SCALAR_V            (0xff)
#define DINT_PKT_SCH_RC11_4X_RATE_INTLV_COLS_V            (0x18 << 8)
#define DINT_PKT_SCH_RC11_4X_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_SCH_RC11_4X_RATE_BLOCK_SIZE_V            (0x0600)
#define DINT_PKT_SCH_RC11_4X_RATE_LTU_LEN_HI_V            (0x0b << 9)
#define DINT_PKT_SCH_RC11_4X_RATE_LTU_LEN_LO_V            (0x02 << 6)
#define DINT_PKT_SCH_RC11_4X_RATE_PUNCT_LENGTH_V          (0x00 << 4)
#define DINT_PKT_SCH_RC11_4X_RATE_SYMBOL_REPEAT_V         (0x00 << 2)
#define DINT_PKT_SCH_RC11_4X_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_SCH_RC11_4X_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_SCH_RC11_4X_RATE_RDCNT_EN_SEL_V          (0x01 << 4)
#define DINT_PKT_SCH_RC11_4X_RATE_NUM_LTU_FRM_V           (0x02)
#define DINT_PKT_SCH_RC11_4X_RATE_HI_PUNCT_PATTERN_V      (0xffff)
#define DINT_PKT_SCH_RC11_4X_RATE_PL_CRC_V                (0xc867)
#define DINT_PKT_SCH_RC11_4X_RATE_LTU_CRC_V               (0xc867)

#define DINT_PKT_SCH_RC11_8X_RATE_ROW_WIDTH_V             (0x03 << 13)
#define DINT_PKT_SCH_RC11_8X_RATE_CHAINBACK_DEPTH_V       (0x01 << 12)
#define DINT_PKT_SCH_RC11_8X_RATE_YM_THRESH_V             (0x01 << 8)
#define DINT_PKT_SCH_RC11_8X_RATE_SMT_SCALAR_V            (0xff)
#define DINT_PKT_SCH_RC11_8X_RATE_INTLV_COLS_V            (0x30 << 8)
#define DINT_PKT_SCH_RC11_8X_RATE_INTLV_ROWS_V            (0x00 << 0)
#define DINT_PKT_SCH_RC11_8X_RATE_BLOCK_SIZE_V            (0x0c00)
#define DINT_PKT_SCH_RC11_8X_RATE_LTU_LEN_HI_V            (0x0b << 9)
#define DINT_PKT_SCH_RC11_8X_RATE_LTU_LEN_LO_V            (0x02 << 6)
#define DINT_PKT_SCH_RC11_8X_RATE_PUNCT_LENGTH_V          (0x00 << 4)
#define DINT_PKT_SCH_RC11_8X_RATE_SYMBOL_REPEAT_V         (0x00 << 2)
#define DINT_PKT_SCH_RC11_8X_RATE_CODE_RATE_V             (0x00)
#define DINT_PKT_SCH_RC11_8X_RATE_LO_PUNCT_PATTERN_V      (0xff << 8)
#define DINT_PKT_SCH_RC11_8X_RATE_RDCNT_EN_SEL_V          (0x01 << 4)
#define DINT_PKT_SCH_RC11_8X_RATE_NUM_LTU_FRM_V           (0x04)
#define DINT_PKT_SCH_RC11_8X_RATE_HI_PUNCT_PATTERN_V      (0xffff)
#define DINT_PKT_SCH_RC11_8X_RATE_PL_CRC_V                (0xc867)
#define DINT_PKT_SCH_RC11_8X_RATE_LTU_CRC_V               (0xc867)

#define DINT_PKT_SCH_RC11_16X_RATE_ROW_WIDTH_V            (0x04 << 13)
#define DINT_PKT_SCH_RC11_16X_RATE_CHAINBACK_DEPTH_V      (0x01 << 12)
#define DINT_PKT_SCH_RC11_16X_RATE_YM_THRESH_V            (0x01 << 8)
#define DINT_PKT_SCH_RC11_16X_RATE_SMT_SCALAR_V           (0xff)
#define DINT_PKT_SCH_RC11_16X_RATE_INTLV_COLS_V           (0x30 << 8)
#define DINT_PKT_SCH_RC11_16X_RATE_INTLV_ROWS_V           (0x00 << 0)
#define DINT_PKT_SCH_RC11_16X_RATE_BLOCK_SIZE_V           (0x1800)
#define DINT_PKT_SCH_RC11_16X_RATE_LTU_LEN_HI_V           (0x0b << 9)
#define DINT_PKT_SCH_RC11_16X_RATE_LTU_LEN_LO_V           (0x02 << 6)
#define DINT_PKT_SCH_RC11_16X_RATE_PUNCT_LENGTH_V         (0x00 << 4)
#define DINT_PKT_SCH_RC11_16X_RATE_SYMBOL_REPEAT_V        (0x00 << 2)
#define DINT_PKT_SCH_RC11_16X_RATE_CODE_RATE_V            (0x00)
#define DINT_PKT_SCH_RC11_16X_RATE_LO_PUNCT_PATTERN_V     (0xff << 8)
#define DINT_PKT_SCH_RC11_16X_RATE_RDCNT_EN_SEL_V         (0x01 << 4)
#define DINT_PKT_SCH_RC11_16X_RATE_NUM_LTU_FRM_V          (0x08)
#define DINT_PKT_SCH_RC11_16X_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_PKT_SCH_RC11_16X_RATE_PL_CRC_V               (0xc867)
#define DINT_PKT_SCH_RC11_16X_RATE_LTU_CRC_V              (0xc867)

#define DINT_PKT_SCH_RC11_32X_RATE_ROW_WIDTH_V            (0x04 << 13)
#define DINT_PKT_SCH_RC11_32X_RATE_CHAINBACK_DEPTH_V      (0x00 << 12)
#define DINT_PKT_SCH_RC11_32X_RATE_YM_THRESH_V            (0x01 << 8)
#define DINT_PKT_SCH_RC11_32X_RATE_SMT_SCALAR_V           (0xff)
#define DINT_PKT_SCH_RC11_32X_RATE_INTLV_COLS_V           (0x60 << 8)
#define DINT_PKT_SCH_RC11_32X_RATE_INTLV_ROWS_V           (0x00 << 0)
#define DINT_PKT_SCH_RC11_32X_RATE_BLOCK_SIZE_V           (0x3000)
#define DINT_PKT_SCH_RC11_32X_RATE_LTU_LEN_HI_V           (0x00 << 9)
#define DINT_PKT_SCH_RC11_32X_RATE_LTU_LEN_LO_V           (0x00 << 6)
#define DINT_PKT_SCH_RC11_32X_RATE_PUNCT_LENGTH_V         (0x00 << 4)
#define DINT_PKT_SCH_RC11_32X_RATE_SYMBOL_REPEAT_V        (0x00 << 2)
#define DINT_PKT_SCH_RC11_32X_RATE_CODE_RATE_V            (0x00)
#define DINT_PKT_SCH_RC11_32X_RATE_LO_PUNCT_PATTERN_V     (0xff << 8)
#define DINT_PKT_SCH_RC11_32X_RATE_RDCNT_EN_SEL_V         (0x0 << 4)
#define DINT_PKT_SCH_RC11_32X_RATE_NUM_LTU_FRM_V          (0x0)
#define DINT_PKT_SCH_RC11_32X_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_PKT_SCH_RC11_32X_RATE_PL_CRC_V               (0xc867)
#define DINT_PKT_SCH_RC11_32X_RATE_LTU_CRC_V              (0xc867)

#ifdef FEATURE_IS2000_REL_A //32X
    #define DINT_LTU_SIZE_IN_BYTE_M     (0x7f << 9)
    #define DINT_LTU_SIZE_MAX           (0x7f)
    #define DINT_NUM_LTU_MAX            (0x0f)
    #define DINT_LTU_SIZE_IN_BYTE_LO_M  (0x07 << 5)
    #define DINT_LTU_PER_FRAME_M        (0xf)
#endif /* FEATURE_IS2000_REL_A */


/*========================================================================

       DEINTERLEAVER PACKET PARAMETERS FOR TURBO SCH FOR ALL RCs

==========================================================================*/

/* 0 selects the DECOB interrupt, i.e. the output buffer has been emptied
   1 selects the DECODE interrupt, i.e. the decoding is done,                                                                   .
    but output buffer may not have been completely emptied yet.
   Programming this bit to ’1’ allows triggering the next code block decoding                                                   .
    while the output buffer is still being drained.
*/
#define DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL  (0x0 << 0)

/* Configure hard-coded CRC polynomial for given standard
   0x0: CRC16 (CRC16 select for 1X or HDR) */
#define DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL   (0x0 << 2)

/* Minimum LLR threshold used for early termination condition check. */
#define DINT_TD_COMMON_CFG1_MIN_LLR         (0x1 << 13)

/* Select the correct LUT for the min* function: Range 3 bits (2:0)
  0x0: BYPASS_LUT (BYPASS_LUT)
  0x1: SEL_1_5 (SEL_1_5 (select 1.5))
  0x2: SEL_3_0 (SEL_3_0 (select 3.0))
  0x3: SEL_4_5 (SEL_4_5 (select 4.5))
  0x4: SEL_6_0 (SEL_6_0 (select 6.0)) */
#define DINT_TD_COMMON_CFG1_LLR_SCALE_SEL   (4 << 27)

/* This specifies the minimum number of full iterations turbo decoder could
   ever terminate. Such as the condition that CONSECUTIVE_CRC has been satisfied.*/
#define DINT_TD_COMMON_CFG2_MIN_ITER        (0x02 << 21)

/* This specifies the maximum number of full iterations when turbo decoder
   should terminate no matter what. Since turbo decoder always terminate on
   even half iteration, one half iteration is added to the value programmed.*/
#define DINT_TD_COMMON_CFG2_MAX_ITER        (0x0A << 25)

/* Enables or disables early termination checking. */
#define DINT_TD_COMMON_CFG2_EARLY_TERM_EN   (0x01 << 29)

/* Sets MAP configuration for core operation.
   0x3: TD_1_MAP_SEQ (TD_1_MAP_SEQ for C2K only) */
#define DINT_TD_COMMON_CFG2_NUM_TD_MAP      (0x03 << 30)
#define DINT_TD_COMMON_CFG2_NUM_TD_MAP_2K   (0x00 << 30)

/* Expected as Pong-only (program as 1) for HDR concurrency. */
#define DINT_TD_1X_CFG_TDECIB_IDX           (0x1 << 0)

/* This specifies the number of consecutive CRC pass iterations
minus 1 before early termination kicks in. */
#define DINT_TD_1X_CFG_CONSEC_CRC_M1        (0x01 << 3)

/* Program ’0’ for 1x, program ’1’ for HDR.) */
#define DINT_TD_1X_CFG_DEC_BIAS             (0x1 << 9)

/* Maximum number of iterations for all rates = 10 */
#define DINT_TD_MAX_ITER_NUM                (0x09 << 10)

/* Minimum of 1 iteration for all rates */
#define DINT_TD_MIN_ITER_NUM                (0x00 << 4)

/*========================================================================

       DEINTERLEAVER PACKET PARAMETERS FOR TURBO SCH FOR RC3

==========================================================================*/

#define DINT_TD_SCH_RC3_2X_RATE_BLOCK_SIZE_V          (0x5ff)
#define DINT_TD_SCH_RC3_2X_RATE_ROW_WIDTH_V           (0x03)
#define DINT_TD_SCH_RC3_2X_RATE_INTLV_COLS_V          (0x17  << 8)
#define DINT_TD_SCH_RC3_2X_RATE_INTLV_ROWS_V          (0x3f)
#define DINT_TD_SCH_RC3_2X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC3_2X_RATE_PUNCT_LENGTH_V        (0x17  << 3)
#define DINT_TD_SCH_RC3_2X_RATE_CODE_RATE_V           (0x02)
#define DINT_TD_SCH_RC3_2X_RATE_HI_PUNCT_PATTERN_V    (0xffff)
#define DINT_TD_SCH_RC3_2X_RATE_TD_INTLV_LEN_V        (0x179)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC3_2X       (0x179 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC3_2X         (0x0F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC3_2X        (0x09  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC3_2X        (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC3_2X   (0x00  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC3_2X   (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC3_2X   (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC3_2X   (0x00  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC3_2X   (0x00  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC3_2X   (0x00  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC3_2X   (0x00  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC3_2X   (0x00  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC3_2X   (0x00  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC3_2X   (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC3_2X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC3_2X   (0x00  << 16)

#define DINT_TD_SCH_RC3_4X_RATE_BLOCK_SIZE_V          (0xbff)
#define DINT_TD_SCH_RC3_4X_RATE_ROW_WIDTH_V           (0x03)
#define DINT_TD_SCH_RC3_4X_RATE_INTLV_COLS_V          (0x2f  << 8)
#define DINT_TD_SCH_RC3_4X_RATE_INTLV_ROWS_V          (0x3f)
#define DINT_TD_SCH_RC3_4X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC3_4X_RATE_PUNCT_LENGTH_V        (0x17  << 3)
#define DINT_TD_SCH_RC3_4X_RATE_CODE_RATE_V           (0x02)
#define DINT_TD_SCH_RC3_4X_RATE_HI_PUNCT_PATTERN_V    (0xffff)
#define DINT_TD_SCH_RC3_4X_RATE_TD_INTLV_LEN_V        (0x2f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC3_4X       (0x2F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC3_4X         (0x1F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC3_4X        (0x19  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC3_4X        (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC3_4X   (0x00  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC3_4X   (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC3_4X   (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC3_4X   (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC3_4X   (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC3_4X   (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC3_4X   (0x18  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC3_4X   (0x10  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC3_4X   (0x08  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC3_4X   (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC3_4X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC3_4X   (0x00  << 16)

#define DINT_TD_SCH_RC3_8X_RATE_BLOCK_SIZE_V          (0x17ff)
#define DINT_TD_SCH_RC3_8X_RATE_ROW_WIDTH_V           (0x04)
#define DINT_TD_SCH_RC3_8X_RATE_INTLV_COLS_V          (0x2f  << 8)
#define DINT_TD_SCH_RC3_8X_RATE_INTLV_ROWS_V          (0x7f)
#define DINT_TD_SCH_RC3_8X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC3_8X_RATE_PUNCT_LENGTH_V        (0x17  << 3)
#define DINT_TD_SCH_RC3_8X_RATE_CODE_RATE_V           (0x02)
#define DINT_TD_SCH_RC3_8X_RATE_HI_PUNCT_PATTERN_V    (0xffff)
#define DINT_TD_SCH_RC3_8X_RATE_TD_INTLV_LEN_V        (0x5f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC3_8X       (0x5F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC3_8X         (0x3F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC3_8X        (0x39  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC3_8X        (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC3_8X   (0x04  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC3_8X   (0x02  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC3_8X   (0x01  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC3_8X   (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC3_8X   (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC3_8X   (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC3_8X   (0x30  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC3_8X   (0x20  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC3_8X   (0x10  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC3_8X   (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC3_8X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC3_8X   (0x00  << 16)

#define DINT_TD_SCH_RC3_16X_RATE_BLOCK_SIZE_V         (0x2fff)
#define DINT_TD_SCH_RC3_16X_RATE_ROW_WIDTH_V          (0x04)
#define DINT_TD_SCH_RC3_16X_RATE_INTLV_COLS_V         (0x5f  << 8)
#define DINT_TD_SCH_RC3_16X_RATE_INTLV_ROWS_V         (0x7f)
#define DINT_TD_SCH_RC3_16X_RATE_LO_PUNCT_PATTERN_V   (0xff  << 8)
#define DINT_TD_SCH_RC3_16X_RATE_PUNCT_LENGTH_V       (0x17  << 3)
#define DINT_TD_SCH_RC3_16X_RATE_CODE_RATE_V          (0x02)
#define DINT_TD_SCH_RC3_16X_RATE_HI_PUNCT_PATTERN_V   (0xffff)
#define DINT_TD_SCH_RC3_16X_RATE_TD_INTLV_LEN_V       (0xbf9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC3_16X      (0xBF9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC3_16X        (0x7F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC3_16X       (0x79  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC3_16X       (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC3_16X  (0x04  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC3_16X  (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC3_16X  (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC3_16X  (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC3_16X  (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC3_16X  (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC3_16X  (0x60  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC3_16X  (0x40  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC3_16X  (0x20  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC3_16X  (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC3_16X  (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC3_16X  (0x00  << 16)

/*========================================================================

       DEINTERLEAVER PACKET PARAMETERS FOR TURBO SCH FOR RC4

==========================================================================*/

#define DINT_TD_SCH_RC4_2X_RATE_BLOCK_SIZE_V          (0x2ff)
#define DINT_TD_SCH_RC4_2X_RATE_ROW_WIDTH_V           (0x03)
#define DINT_TD_SCH_RC4_2X_RATE_INTLV_COLS_V          (0x0b  << 8)
#define DINT_TD_SCH_RC4_2X_RATE_INTLV_ROWS_V          (0x3f)
#define DINT_TD_SCH_RC4_2X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC4_2X_RATE_PUNCT_LENGTH_V        (0x17  << 3)
#define DINT_TD_SCH_RC4_2X_RATE_CODE_RATE_V           (0x00)
#define DINT_TD_SCH_RC4_2X_RATE_HI_PUNCT_PATTERN_V    (0xffff)
#define DINT_TD_SCH_RC4_2X_RATE_TD_INTLV_LEN_V        (0x179)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_2X       (0x179 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_2X         (0x0F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_2X        (0x09  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_2X        (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_2X   (0x00  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_2X   (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_2X   (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_2X   (0x00  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_2X   (0x00  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_2X   (0x00  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_2X   (0x00  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_2X   (0x00  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_2X   (0x00  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_2X   (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_2X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_2X   (0x00  << 16)

#define DINT_TD_SCH_RC4_4X_RATE_BLOCK_SIZE_V          (0x5ff)
#define DINT_TD_SCH_RC4_4X_RATE_ROW_WIDTH_V           (0x03)
#define DINT_TD_SCH_RC4_4X_RATE_INTLV_COLS_V          (0x17  << 8)
#define DINT_TD_SCH_RC4_4X_RATE_INTLV_ROWS_V          (0x3f)
#define DINT_TD_SCH_RC4_4X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC4_4X_RATE_PUNCT_LENGTH_V        (0x17  << 3)
#define DINT_TD_SCH_RC4_4X_RATE_CODE_RATE_V           (0x00)
#define DINT_TD_SCH_RC4_4X_RATE_HI_PUNCT_PATTERN_V    (0xffff)
#define DINT_TD_SCH_RC4_4X_RATE_TD_INTLV_LEN_V        (0x2f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_4X       (0x2F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_4X         (0x1F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_4X        (0x19  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_4X        (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_4X   (0x00  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_4X   (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_4X   (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_4X   (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_4X   (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_4X   (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_4X   (0x18  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_4X   (0x10  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_4X   (0x08  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_4X   (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_4X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_4X   (0x00  << 16)

#define DINT_TD_SCH_RC4_8X_RATE_BLOCK_SIZE_V          (0xbff)
#define DINT_TD_SCH_RC4_8X_RATE_ROW_WIDTH_V           (0x03)
#define DINT_TD_SCH_RC4_8X_RATE_INTLV_COLS_V          (0x2f  << 8)
#define DINT_TD_SCH_RC4_8X_RATE_INTLV_ROWS_V          (0x3f)
#define DINT_TD_SCH_RC4_8X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC4_8X_RATE_PUNCT_LENGTH_V        (0x17  << 3)
#define DINT_TD_SCH_RC4_8X_RATE_CODE_RATE_V           (0x00)
#define DINT_TD_SCH_RC4_8X_RATE_HI_PUNCT_PATTERN_V    (0xffff)
#define DINT_TD_SCH_RC4_8X_RATE_TD_INTLV_LEN_V        (0x5f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_8X       (0x5F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_8X         (0x3F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_8X        (0x39  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_8X        (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_8X   (0x04  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_8X   (0x02  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_8X   (0x01  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_8X   (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_8X   (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_8X   (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_8X   (0x30  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_8X   (0x20  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_8X   (0x10  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_8X   (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_8X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_8X   (0x00  << 16)

#define DINT_TD_SCH_RC4_16X_RATE_BLOCK_SIZE_V         (0x17ff)
#define DINT_TD_SCH_RC4_16X_RATE_ROW_WIDTH_V          (0x04)
#define DINT_TD_SCH_RC4_16X_RATE_INTLV_COLS_V         (0x2f  << 8)
#define DINT_TD_SCH_RC4_16X_RATE_INTLV_ROWS_V         (0x7f)
#define DINT_TD_SCH_RC4_16X_RATE_LO_PUNCT_PATTERN_V   (0xff  << 8)
#define DINT_TD_SCH_RC4_16X_RATE_PUNCT_LENGTH_V       (0x17  << 3)
#define DINT_TD_SCH_RC4_16X_RATE_CODE_RATE_V          (0x00)
#define DINT_TD_SCH_RC4_16X_RATE_HI_PUNCT_PATTERN_V   (0xffff)
#define DINT_TD_SCH_RC4_16X_RATE_TD_INTLV_LEN_V       (0xbf9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_16X      (0xBF9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_16X        (0x7F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_16X       (0x79  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_16X       (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_16X  (0x04  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_16X  (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_16X  (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_16X  (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_16X  (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_16X  (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_16X  (0x60  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_16X  (0x40  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_16X  (0x20  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_16X  (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_16X  (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_16X  (0x00  << 16)

#define DINT_TD_SCH_RC4_32X_RATE_BLOCK_SIZE_V         (0x2fff)
#define DINT_TD_SCH_RC4_32X_RATE_ROW_WIDTH_V          (0x04)
#define DINT_TD_SCH_RC4_32X_RATE_INTLV_COLS_V         (0x5f  << 8)
#define DINT_TD_SCH_RC4_32X_RATE_INTLV_ROWS_V         (0x7f)
#define DINT_TD_SCH_RC4_32X_RATE_LO_PUNCT_PATTERN_V   (0xff  << 8)
#define DINT_TD_SCH_RC4_32X_RATE_PUNCT_LENGTH_V       (0x17  << 3)
#define DINT_TD_SCH_RC4_32X_RATE_CODE_RATE_V          (0x00)
#define DINT_TD_SCH_RC4_32X_RATE_HI_PUNCT_PATTERN_V   (0xffff)
#define DINT_TD_SCH_RC4_32X_RATE_TD_INTLV_LEN_V       (0x17f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_32X      (0x17F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_32X        (0xFF  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_32X       (0xF9  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_32X       (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_32X  (0x08  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_32X  (0x08  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_32X  (0x08  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_32X  (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_32X  (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_32X  (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_32X  (0xC0  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_32X  (0x80  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_32X  (0x40  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_32X  (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_32X  (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_32X  (0x00  << 16)

/*========================================================================

       DEINTERLEAVER PACKET PARAMETERS FOR TURBO SCH FOR RC5

==========================================================================*/

#define DINT_TD_SCH_RC5_2X_RATE_BLOCK_SIZE_V          (0x5ff)
#define DINT_TD_SCH_RC5_2X_RATE_ROW_WIDTH_V           (0x03)
#define DINT_TD_SCH_RC5_2X_RATE_INTLV_COLS_V          (0x17  << 8)
#define DINT_TD_SCH_RC5_2X_RATE_INTLV_ROWS_V          (0x3f)
#define DINT_TD_SCH_RC5_2X_RATE_LO_PUNCT_PATTERN_V    (0x00  << 8)
#define DINT_TD_SCH_RC5_2X_RATE_PUNCT_LENGTH_V        (0x0b  << 3)
#define DINT_TD_SCH_RC5_2X_RATE_CODE_RATE_V           (0x02)
#define DINT_TD_SCH_RC5_2X_RATE_HI_PUNCT_PATTERN_V    (0xdda0)
#define DINT_TD_SCH_RC5_2X_RATE_TD_INTLV_LEN_V        (0x239)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC5_2X       (0x239 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC5_2X         (0x1F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC5_2X        (0x19  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC5_2X        (0x11  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC5_2X   (0x06  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC5_2X   (0x04  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC5_2X   (0x01  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC5_2X   (0x0D  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC5_2X   (0x09  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC5_2X   (0x04  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC5_2X   (0x18  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC5_2X   (0x10  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC5_2X   (0x08  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC5_2X   (0x10  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC5_2X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC5_2X   (0x10  << 16)

#define DINT_TD_SCH_RC5_4X_RATE_BLOCK_SIZE_V          (0xbff)
#define DINT_TD_SCH_RC5_4X_RATE_ROW_WIDTH_V           (0x03)
#define DINT_TD_SCH_RC5_4X_RATE_INTLV_COLS_V          (0x2f  << 8)
#define DINT_TD_SCH_RC5_4X_RATE_INTLV_ROWS_V          (0x3f)
#define DINT_TD_SCH_RC5_4X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC5_4X_RATE_PUNCT_LENGTH_V        (0x0b  << 3)
#define DINT_TD_SCH_RC5_4X_RATE_CODE_RATE_V           (0x02)
#define DINT_TD_SCH_RC5_4X_RATE_HI_PUNCT_PATTERN_V    (0xddaf)
#define DINT_TD_SCH_RC5_4X_RATE_TD_INTLV_LEN_V        (0x479)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC5_4X       (0x479 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC5_4X         (0x3F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC5_4X        (0x39  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC5_4X        (0x11  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC5_4X   (0x08  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC5_4X   (0x04  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC5_4X   (0x02  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC5_4X   (0x0D  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC5_4X   (0x09  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC5_4X   (0x04  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC5_4X   (0x30  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC5_4X   (0x20  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC5_4X   (0x10  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC5_4X   (0x20  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC5_4X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC5_4X   (0x20  << 16)

#define DINT_TD_SCH_RC5_8X_RATE_BLOCK_SIZE_V          (0x17ff)
#define DINT_TD_SCH_RC5_8X_RATE_ROW_WIDTH_V           (0x04)
#define DINT_TD_SCH_RC5_8X_RATE_INTLV_COLS_V          (0x2f  << 8)
#define DINT_TD_SCH_RC5_8X_RATE_INTLV_ROWS_V          (0x7f)
#define DINT_TD_SCH_RC5_8X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC5_8X_RATE_PUNCT_LENGTH_V        (0x0b  << 3)
#define DINT_TD_SCH_RC5_8X_RATE_CODE_RATE_V           (0x02)
#define DINT_TD_SCH_RC5_8X_RATE_HI_PUNCT_PATTERN_V    (0xddaf)
#define DINT_TD_SCH_RC5_8X_RATE_TD_INTLV_LEN_V        (0x8f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC5_8X       (0x8F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC5_8X         (0x7F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC5_8X        (0x79  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC5_8X        (0x11  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC5_8X   (0x0A  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC5_8X   (0x06  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC5_8X   (0x02  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC5_8X   (0x0D  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC5_8X   (0x09  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC5_8X   (0x04  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC5_8X   (0x60  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC5_8X   (0x40  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC5_8X   (0x20  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC5_8X   (0x40  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC5_8X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC5_8X   (0x40  << 16)

#define DINT_TD_SCH_RC5_16X_RATE_BLOCK_SIZE_V         (0x2fff)
#define DINT_TD_SCH_RC5_16X_RATE_ROW_WIDTH_V          (0x04)
#define DINT_TD_SCH_RC5_16X_RATE_INTLV_COLS_V         (0x5f  << 8)
#define DINT_TD_SCH_RC5_16X_RATE_INTLV_ROWS_V         (0x7f)
#define DINT_TD_SCH_RC5_16X_RATE_LO_PUNCT_PATTERN_V   (0xff  << 8)
#define DINT_TD_SCH_RC5_16X_RATE_PUNCT_LENGTH_V       (0x0b  << 3)
#define DINT_TD_SCH_RC5_16X_RATE_CODE_RATE_V          (0x02)
#define DINT_TD_SCH_RC5_16X_RATE_HI_PUNCT_PATTERN_V   (0xddaf)
#define DINT_TD_SCH_RC5_16X_RATE_TD_INTLV_LEN_V       (0x11f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC5_16X      (0x11F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC5_16X        (0xFF  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC5_16X       (0xF9  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC5_16X       (0x11  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC5_16X  (0x06  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC5_16X  (0x04  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC5_16X  (0x01  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC5_16X  (0x0D  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC5_16X  (0x09  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC5_16X  (0x04  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC5_16X  (0xC0  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC5_16X  (0x80  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC5_16X  (0x40  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC5_16X  (0x80  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC5_16X  (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC5_16X  (0x80  << 16)

/*========================================================================

       DEINTERLEAVER PACKET PARAMETERS FOR TURBO SCH FOR RC11

==========================================================================*/

#define DINT_TD_SCH_RC11_2X_RATE_BLOCK_SIZE_V           (0x2ff)
#define DINT_TD_SCH_RC11_2X_RATE_ROW_WIDTH_V            (0x03)
#define DINT_TD_SCH_RC11_2X_RATE_INTLV_COLS_V           (0x0b  << 8)
#define DINT_TD_SCH_RC11_2X_RATE_INTLV_ROWS_V           (0x3f)
#define DINT_TD_SCH_RC11_2X_RATE_LO_PUNCT_PATTERN_V     (0xff  << 8)
#define DINT_TD_SCH_RC11_2X_RATE_PUNCT_LENGTH_V         (0x17  << 3)
#define DINT_TD_SCH_RC11_2X_RATE_CODE_RATE_V            (0x00)
#define DINT_TD_SCH_RC11_2X_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_TD_SCH_RC11_2X_RATE_TD_INTLV_LEN_V         (0x179)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_2X        (0x179 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_2X          (0x0F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_2X         (0x09  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_2X         (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_2X    (0x00  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_2X    (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_2X    (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_2X    (0x00  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_2X    (0x00  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_2X    (0x00  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_2X    (0x00  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_2X    (0x00  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_2X    (0x00  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_2X    (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_2X    (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_2X    (0x00  << 16)

#define DINT_TD_SCH_RC11_4X_RATE_BLOCK_SIZE_V           (0x5ff)
#define DINT_TD_SCH_RC11_4X_RATE_ROW_WIDTH_V            (0x03)
#define DINT_TD_SCH_RC11_4X_RATE_INTLV_COLS_V           (0x17  << 8)
#define DINT_TD_SCH_RC11_4X_RATE_INTLV_ROWS_V           (0x3f)
#define DINT_TD_SCH_RC11_4X_RATE_LO_PUNCT_PATTERN_V     (0xff  << 8)
#define DINT_TD_SCH_RC11_4X_RATE_PUNCT_LENGTH_V         (0x17  << 3)
#define DINT_TD_SCH_RC11_4X_RATE_CODE_RATE_V            (0x00)
#define DINT_TD_SCH_RC11_4X_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_TD_SCH_RC11_4X_RATE_TD_INTLV_LEN_V         (0x2f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_4X        (0x2F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_4X          (0x1F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_4X         (0x19  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_4X         (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_4X    (0x00  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_4X    (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_4X    (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_4X    (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_4X    (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_4X    (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_4X    (0x18  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_4X    (0x10  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_4X    (0x08  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_4X    (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_4X    (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_4X    (0x00  << 16)

#define DINT_TD_SCH_RC11_8X_RATE_BLOCK_SIZE_V           (0xbff)
#define DINT_TD_SCH_RC11_8X_RATE_ROW_WIDTH_V            (0x03)
#define DINT_TD_SCH_RC11_8X_RATE_INTLV_COLS_V           (0x2f  << 8)
#define DINT_TD_SCH_RC11_8X_RATE_INTLV_ROWS_V           (0x3f)
#define DINT_TD_SCH_RC11_8X_RATE_LO_PUNCT_PATTERN_V     (0xff  << 8)
#define DINT_TD_SCH_RC11_8X_RATE_PUNCT_LENGTH_V         (0x17  << 3)
#define DINT_TD_SCH_RC11_8X_RATE_CODE_RATE_V            (0x00)
#define DINT_TD_SCH_RC11_8X_RATE_HI_PUNCT_PATTERN_V     (0xffff)
#define DINT_TD_SCH_RC11_8X_RATE_TD_INTLV_LEN_V         (0x5f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_8X        (0x5F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_8X          (0x3F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_8X         (0x39  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_8X         (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_8X    (0x04  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_8X    (0x02  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_8X    (0x01  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_8X    (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_8X    (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_8X    (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_8X    (0x30  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_8X    (0x20  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_8X    (0x10  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_8X    (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_8X    (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_8X    (0x00  << 16)

#define DINT_TD_SCH_RC11_16X_RATE_BLOCK_SIZE_V          (0x17ff)
#define DINT_TD_SCH_RC11_16X_RATE_ROW_WIDTH_V           (0x04)
#define DINT_TD_SCH_RC11_16X_RATE_INTLV_COLS_V          (0x2f << 8)
#define DINT_TD_SCH_RC11_16X_RATE_INTLV_ROWS_V          (0x7f)
#define DINT_TD_SCH_RC11_16X_RATE_LO_PUNCT_PATTERN_V    (0xff << 8)
#define DINT_TD_SCH_RC11_16X_RATE_PUNCT_LENGTH_V        (0x17 << 3)
#define DINT_TD_SCH_RC11_16X_RATE_CODE_RATE_V           (0x00)
#define DINT_TD_SCH_RC11_16X_RATE_HI_PUNCT_PATTERN_V    (0xffff)
#define DINT_TD_SCH_RC11_16X_RATE_TD_INTLV_LEN_V        (0xbf9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_16X       (0xBF9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_16X         (0x7F  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_16X        (0x79  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_16X        (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_16X   (0x04  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_16X   (0x00  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_16X   (0x00  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_16X   (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_16X   (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_16X   (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_16X   (0x60  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_16X   (0x40  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_16X   (0x20  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_16X   (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_16X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_16X   (0x00  << 16)

#define DINT_TD_SCH_RC11_32X_RATE_BLOCK_SIZE_V          (0x2fff)
#define DINT_TD_SCH_RC11_32X_RATE_ROW_WIDTH_V           (0x04)
#define DINT_TD_SCH_RC11_32X_RATE_INTLV_COLS_V          (0x5f  << 8)
#define DINT_TD_SCH_RC11_32X_RATE_INTLV_ROWS_V          (0x7f)
#define DINT_TD_SCH_RC11_32X_RATE_LO_PUNCT_PATTERN_V    (0xff  << 8)
#define DINT_TD_SCH_RC11_32X_RATE_PUNCT_LENGTH_V        (0x17  << 3)
#define DINT_TD_SCH_RC11_32X_RATE_CODE_RATE_V           (0x00)
#define DINT_TD_SCH_RC11_32X_RATE_HI_PUNCT_PATTERN_V    (0xffff)
#define DINT_TD_SCH_RC11_32X_RATE_TD_INTLV_LEN_V        (0x17f9)
#define DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_32X       (0x17F9 << 0)
#define DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_32X         (0xFF  << 0)
#define DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_32X        (0xF9  << 8)
#define DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_32X        (0x17  << 16)
#define DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_32X   (0x08  << 0)
#define DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_32X   (0x08  << 5)
#define DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_32X   (0x08  << 10)
#define DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_32X   (0x12  << 16)
#define DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_32X   (0x0C  << 21)
#define DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_32X   (0x06  << 26)
#define DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_32X   (0xC0  << 0)
#define DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_32X   (0x80  << 8)
#define DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_32X   (0x40  << 16)
#define DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_32X   (0x00  << 0)
#define DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_32X   (0x00  << 8)
#define DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_32X   (0x00  << 16)

#ifdef FEATURE_IS2000_REL_A_SVD
#define DEINT_MAX_SEQUENCING_CMD  3   /* FCH + DCCH + SCH */
#else
#define DEINT_MAX_SEQUENCING_CMD  2   /* FCH + SCH */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*=======================================================================

   Data Structures for Deinterleaver/Decoder Sequencing Commands to
   Program Read Parameters.

========================================================================*/
static q_type deint_sequencing_cmd_q;
static q_type deint_sequencing_free_q;
static deint_sequencing_cmd_type
  deint_sequencing_cmd_buf[DEINT_MAX_SEQUENCING_CMD];

#ifdef FEATURE_IS2000_REL_A_SVD
/* Avoid resetting decoder while in traffic state.
*/
static deint_phone_state_type curr_deint_state;

/* Indicate activity of fundicated channels FCH and DCCH.
*/
static deint_fundicated_ch_active_type curr_deint_ch_active = DEINT_CH_INACTIVE;
#endif /* FEATURE_IS2000_REL_A_SVD */

#ifdef FEATURE_IS2000_REL_A
/* Changes to Turbo SCH set up, including the new struct sch_setup, were
** made to avoid the first Turbo SCH ISR which will occur 5 ms after
** Turbo Dec parameters are loaded resulting in reading previous SCH frame
** data.  This issue was discovered during Turbo finite burst testing.
**
** The fix is to move the creation of the Turbo SCH dummy task to
** the fourth bin of the same frame as the action time which the Turbo Dec
** parameters is loaded.  This move guarantees that the first dummy
** interrupt will not happen in the frame which the ISR is set up
** (1 frame too soon).
**
** Since finite bursts may involve conversion between Turbo and Conv
** coding, the setting of the coding type is also moved.  This move
** ensures smooth transitioning between all types of finite burst
** assignments (bursts with gaps, back to back, overlapping) which may
** involve coding conversion as well.
*/
typedef struct
{
  boolean this_frame;
  boolean turbo_dec_int;
  boolean sup_dec_int;

} deint_sch_setup_type;

static deint_sch_setup_type sch_setup;

#endif /* FEATURE_IS2000_REL_A */

/*===========================================================================

  DEINTERLEAVER PACKET CONFIGURATION PARAMETERS FOR SYNC CHANNEL

============================================================================*/

static const deint_sc_pkt_cfg_type deint_sc_pkt_params =
{
  .sc_param =
  {
    .pkt_intlv_cfg_lo = DINT_PKT_SC_ROW_WIDTH_V | DINT_PKT_SC_CHAINBACK_DEPTH_V |
                        DINT_PKT_SC_YM_THRESH_V | DINT_PKT_SC_SMT_SCALAR_V,

    .pkt_intlv_cfg_hi = DINT_PKT_SC_INTLV_COLS_V | DINT_PKT_SC_INTLV_ROWS_V,

    .pkt_blk_size_lo  = DINT_PKT_SC_BLOCK_SIZE_V,

    .pkt_blk_size_hi  = DINT_PKT_SC_PUNCT_LENGTH_V |
                        DINT_PKT_SC_SYMBOL_REPEAT_V | DINT_PKT_SC_CODE_RATE_V,

    .pkt_punct_lo     = DINT_PKT_SC_LO_PUNCT_PATTERN_V | DINT_PKT_SC_RDCNT_EN_SEL_V |
                        DINT_PKT_SC_NUM_LTU_FRM_V,

    .pkt_punct_hi     = DINT_PKT_SC_HI_PUNCT_PATTERN_V,

    .pkt_pl_crc_lo    = DINT_PKT_SC_PL_CRC_V,

    .pkt_pl_crc_hi    = DINT_PKT_SC_LTU_CRC_V }
};

/*===========================================================================

  DEINTERLEAVER PACKET CONFIGURATION PARAMETERS FOR IS95 PAGING CHANNEL

============================================================================*/

static const deint_pc_pkt_cfg_type deint_pc_pkt_params = {
{
  /* PRAT_9600 */
  //TODO: Replace magic numbers with constants
  {
    0x61ff,
    0x0600,
    0x0180,
    0x0000,
    0xff00,
    0xffff,
    0x0000,
    0x0000
  },

  /* PRAT_4800 */
  {
    0x61b5,
    0x0600,
    0x0180,
    0x0004,
    0xff00,
    0xffff,
    0x0000,
    0x0000
  }
}
};

/*===========================================================================

  DEINTERLEAVER PACKET CONFIGURATION PARAMETERS FOR FCH FOR RADIO
  CONFIGURATION 1-5.

============================================================================*/
#define DEINT_NUM_FCH_PKT_CFG_PARAM_RC  5 /* RC1, RC2, RC3, RC4, RC5 */

/* NOTE: RC11 is not included since SW does not
  program deint tasks for this RC */

static const deint_fch_pkt_cfg_type deint_fch_packet_params[] = {
{

  /************ RC1 Full Rate Packet Parameters *******/

  {

    DINT_PKT_FCH_RC1_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC1_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC1_FULL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC1_FULL_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC1_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC1_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC1_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC1_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC1_FULL_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC1_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC1_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC1_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC1_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC1_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC1_FULL_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC1_ALL_RATE_LTU_CRC_V
  },

   /********* RC1 HALF Rate Packet Parameters **********/

  {

    DINT_PKT_FCH_RC1_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC1_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC1_HALF_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC1_HALF_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC1_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC1_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC1_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC1_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC1_HALF_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC1_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC1_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC1_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC1_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC1_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC1_HALF_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC1_ALL_RATE_LTU_CRC_V

  },

  /******** RC1 QUARTER Rate Packet Parameters ***********/

  {

    DINT_PKT_FCH_RC1_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC1_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC1_QUARTER_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC1_QUARTER_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC1_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC1_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC1_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC1_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC1_QUARTER_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC1_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC1_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC1_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC1_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC1_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC1_QUARTER_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC1_ALL_RATE_LTU_CRC_V

  },

  /********* RC1 EIGHTH Rate Packet Parameters ****************/

  {

    DINT_PKT_FCH_RC1_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC1_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC1_EIGHTH_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC1_EIGHTH_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC1_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC1_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC1_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC1_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC1_EIGHTH_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC1_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC1_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC1_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC1_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC1_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC1_EIGHTH_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC1_ALL_RATE_LTU_CRC_V

  },

},

/***********************RC2********************************/

{

  /***************** RC2 Full Rate Packet Parameters *******/

  {

    DINT_PKT_FCH_RC2_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC2_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC2_FULL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC2_FULL_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC2_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC2_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC2_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC2_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC2_FULL_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC2_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC2_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC2_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC2_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC2_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC2_FULL_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC2_ALL_RATE_LTU_CRC_V

},

  /*************** RC2 HALF Rate Packet Parameters **********/

  {

    DINT_PKT_FCH_RC2_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC2_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC2_HALF_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC2_HALF_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC2_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC2_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC2_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC2_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC2_HALF_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC2_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC2_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC2_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC2_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC2_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC2_HALF_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC2_ALL_RATE_LTU_CRC_V

  },

  /************ RC2 QUARTER Rate Packet Parameters ******************/

  {

    DINT_PKT_FCH_RC2_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC2_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC2_QUARTER_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC2_QUARTER_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC2_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC2_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC2_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC2_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC2_QUARTER_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC2_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC2_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC2_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC2_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC2_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC2_QUARTER_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC2_ALL_RATE_LTU_CRC_V

  },

  /************* RC2 EIGHTH Rate Packet Parameters *********************/

  {

    DINT_PKT_FCH_RC2_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC2_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC2_EIGHTH_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC2_EIGHTH_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC2_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC2_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC2_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC2_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC2_EIGHTH_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC2_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC2_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC2_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC2_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC2_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC2_EIGHTH_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC2_ALL_RATE_LTU_CRC_V

  },

},

/*****************************RC3************************************/
{


  /*********************RC3 FULL Rate Packet Parameters *************/

  {

    DINT_PKT_FCH_RC3_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC3_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC3_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC3_FULL_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC3_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC3_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC3_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC3_FULL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC3_FULL_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC3_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC3_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC3_FULL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC3_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC3_FULL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC3_FULL_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC3_ALL_RATE_LTU_CRC_V

  },

  /****************** RC3 HALF Rate Packet Parameters ************/

  {

    DINT_PKT_FCH_RC3_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC3_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC3_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC3_HALF_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC3_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC3_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC3_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC3_HALF_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC3_HALF_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC3_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC3_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC3_HALF_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC3_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC3_HALF_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC3_HALF_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC3_ALL_RATE_LTU_CRC_V

  },

  /*************** RC3 QUARTER Rate Packet Parameters ***************/

  {

    DINT_PKT_FCH_RC3_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC3_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC3_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC3_QUARTER_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC3_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC3_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC3_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC3_QUARTER_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC3_QUARTER_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC3_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC3_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC3_QUARTER_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC3_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC3_QUARTER_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC3_QUARTER_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC3_ALL_RATE_LTU_CRC_V

  },

  /************* RC3 EIGHTH Rate Packet Parameters *******************/

  {

    DINT_PKT_FCH_RC3_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC3_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC3_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC3_EIGHTH_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC3_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC3_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC3_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC3_EIGHTH_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC3_EIGHTH_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC3_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC3_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC3_EIGHTH_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC3_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC3_EIGHTH_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC3_EIGHTH_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC3_ALL_RATE_LTU_CRC_V

  },

},

/************************RC4************************************/

{


  /*****************RC4 FULL Rate Packet Parameters *************/

  {

    DINT_PKT_FCH_RC4_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC4_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC4_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC4_FULL_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC4_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC4_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC4_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC4_FULL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC4_FULL_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC4_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC4_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC4_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC4_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC4_FULL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC4_FULL_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC4_ALL_RATE_LTU_CRC_V

  },

  /**************** RC4 HALF Rate Packet Parameters *************/

  {

    DINT_PKT_FCH_RC4_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC4_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC4_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC4_HALF_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC4_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC4_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC4_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC4_HALF_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC4_HALF_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC4_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC4_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC4_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC4_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC4_HALF_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC4_HALF_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC4_ALL_RATE_LTU_CRC_V

  },

  /********** RC4 QUARTER Rate Packet Parameters ***************/

  {

    DINT_PKT_FCH_RC4_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC4_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC4_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC4_QUARTER_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC4_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC4_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC4_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC4_QUARTER_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC4_QUARTER_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC4_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC4_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC4_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC4_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC4_QUARTER_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC4_QUARTER_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC4_ALL_RATE_LTU_CRC_V

  },

  /************ RC4 EIGHTH Rate Packet Parameters ******************/

  {

    DINT_PKT_FCH_RC4_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC4_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC4_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC4_EIGHTH_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC4_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC4_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC4_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC4_EIGHTH_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC4_EIGHTH_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC4_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC4_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC4_ALL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC4_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC4_EIGHTH_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC4_EIGHTH_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC4_ALL_RATE_LTU_CRC_V

  },

},

/******************RC5*****************************************/
{


  /**************RC5 FULL Rate Packet Parameters *************/

  {

    DINT_PKT_FCH_RC5_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC5_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC5_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC5_FULL_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC5_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC5_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC5_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC5_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC5_FULL_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC5_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC5_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC5_FULL_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC5_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC5_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC5_FULL_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC5_ALL_RATE_LTU_CRC_V

  },

  /************** RC5 HALF Rate Packet Parameters *****************/

  {

    DINT_PKT_FCH_RC5_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC5_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC5_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC5_HALF_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC5_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC5_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC5_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC5_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC5_HALF_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC5_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC5_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC5_HALF_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC5_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC5_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC5_HALF_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC5_ALL_RATE_LTU_CRC_V

  },

  /************* RC5 QUARTER Rate Packet Parameters **************/

  {

    DINT_PKT_FCH_RC5_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC5_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC5_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC5_QUARTER_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC5_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC5_ALL_RATE_INTLV_ROWS_V,

    DINT_PKT_FCH_RC5_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC5_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC5_QUARTER_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC5_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC5_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC5_QUARTER_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC5_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC5_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC5_QUARTER_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC5_ALL_RATE_LTU_CRC_V

  },

  /************* RC5 EIGHTH Rate Packet Parameters ************************/

  {

    DINT_PKT_FCH_RC5_ALL_RATE_ROW_WIDTH_V |
    DINT_PKT_FCH_RC5_ALL_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_FCH_RC5_ALL_RATE_YM_THRESH_V |
    DINT_PKT_FCH_RC5_EIGHTH_RATE_SMT_SCALAR_V,

    DINT_PKT_FCH_RC5_ALL_RATE_INTLV_COLS_V |
    DINT_PKT_FCH_RC5_ALL_RATE_INTLV_ROWS_V,
    DINT_PKT_FCH_RC5_ALL_RATE_BLOCK_SIZE_V,

    DINT_PKT_FCH_RC5_ALL_RATE_PUNCT_LENGTH_V |
    DINT_PKT_FCH_RC5_EIGHTH_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_FCH_RC5_ALL_RATE_CODE_RATE_V,

    DINT_PKT_FCH_RC5_ALL_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_FCH_RC5_EIGHTH_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_FCH_RC5_ALL_RATE_NUM_LTU_FRM_V,

    DINT_PKT_FCH_RC5_ALL_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_FCH_RC5_EIGHTH_RATE_PL_CRC_V,

    DINT_PKT_FCH_RC5_ALL_RATE_LTU_CRC_V

  },

}

};

/*============================================================================

  DEINTERLEAVER PACKET CONFIGURATION PARAMETERS FOR IS95C Supplemental Channels

==============================================================================*/

static const deint_sch_pkt_cfg_type
  deint_sch_pkt_params[NUM_SCH_RC][NUM_SCH_RATES] = {

{
   /********** RC3 1X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC3_1X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC3_1X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC3_1X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC3_1X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC3_1X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC3_1X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC3_1X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC3_1X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC3_1X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC3_1X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC3_1X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC3_1X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC3_1X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC3_1X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC3_1X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC3_1X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC3_1X_RATE_LTU_CRC_V
  },

   /********** RC3 2X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC3_2X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC3_2X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC3_2X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC3_2X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC3_2X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC3_2X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC3_2X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC3_2X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC3_2X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC3_2X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC3_2X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC3_2X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC3_2X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC3_2X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC3_2X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC3_2X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC3_2X_RATE_LTU_CRC_V
  },

   /********** RC3 4X SCH Packet Parameters ***********/

  {
    DINT_PKT_SCH_RC3_4X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC3_4X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC3_4X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC3_4X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC3_4X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC3_4X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC3_4X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC3_4X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC3_4X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC3_4X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC3_4X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC3_4X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC3_4X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC3_4X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC3_4X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC3_4X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC3_4X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC3_4X_RATE_LTU_CRC_V

  },

   /********** RC3 8X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC3_8X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC3_8X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC3_8X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC3_8X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC3_8X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC3_8X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC3_8X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC3_8X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC3_8X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC3_8X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC3_8X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC3_8X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC3_8X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC3_8X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC3_8X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC3_8X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC3_8X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC3_8X_RATE_LTU_CRC_V

  },

   /********** RC3 16X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC3_16X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC3_16X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC3_16X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC3_16X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC3_16X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC3_16X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC3_16X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC3_16X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC3_16X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC3_16X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC3_16X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC3_16X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC3_16X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC3_16X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC3_16X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC3_16X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC3_16X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC3_16X_RATE_LTU_CRC_V

  }
}, /* end RC3 packet parameters */

{


   /********** RC4 1X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC4_1X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC4_1X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC4_1X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC4_1X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC4_1X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC4_1X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC4_1X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC4_1X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC4_1X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC4_1X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC4_1X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC4_1X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC4_1X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC4_1X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC4_1X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC4_1X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC4_1X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC4_1X_RATE_LTU_CRC_V
  },

   /********** RC4 2X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC4_2X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC4_2X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC4_2X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC4_2X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC4_2X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC4_2X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC4_2X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC4_2X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC4_2X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC4_2X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC4_2X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC4_2X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC4_2X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC4_2X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC4_2X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC4_2X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC4_2X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC4_2X_RATE_LTU_CRC_V
  },

   /********** RC4 4X SCH Packet Parameters ***********/

  {
    DINT_PKT_SCH_RC4_4X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC4_4X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC4_4X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC4_4X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC4_4X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC4_4X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC4_4X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC4_4X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC4_4X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC4_4X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC4_4X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC4_4X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC4_4X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC4_4X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC4_4X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC4_4X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC4_4X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC4_4X_RATE_LTU_CRC_V

  },

   /********** RC4 8X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC4_8X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC4_8X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC4_8X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC4_8X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC4_8X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC4_8X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC4_8X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC4_8X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC4_8X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC4_8X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC4_8X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC4_8X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC4_8X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC4_8X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC4_8X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC4_8X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC4_8X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC4_8X_RATE_LTU_CRC_V

  },

   /********** RC4 16X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC4_16X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC4_16X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC4_16X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC4_16X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC4_16X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC4_16X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC4_16X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC4_16X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC4_16X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC4_16X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC4_16X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC4_16X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC4_16X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC4_16X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC4_16X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC4_16X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC4_16X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC4_16X_RATE_LTU_CRC_V

  },

     /********** RC4 32X SCH Packet Parameters ***********/
  {

    DINT_PKT_SCH_RC4_32X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC4_32X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC4_32X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC4_32X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC4_32X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC4_32X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC4_32X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC4_32X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC4_32X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC4_32X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC4_32X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC4_32X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC4_32X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC4_32X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC4_32X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC4_32X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC4_32X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC4_32X_RATE_LTU_CRC_V
  },
}, /* end RC4 packet parameters */

{

   /********** RC5 1X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC5_1X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC5_1X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC5_1X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC5_1X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC5_1X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC5_1X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC5_1X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC5_1X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC5_1X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC5_1X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC5_1X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC5_1X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC5_1X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC5_1X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC5_1X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC5_1X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC5_1X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC5_1X_RATE_LTU_CRC_V
  },

   /********** RC5 2X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC5_2X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC5_2X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC5_2X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC5_2X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC5_2X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC5_2X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC5_2X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC5_2X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC5_2X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC5_2X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC5_2X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC5_2X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC5_2X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC5_2X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC5_2X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC5_2X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC5_2X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC5_2X_RATE_LTU_CRC_V
  },

   /********** RC5 4X SCH Packet Parameters ***********/

  {
    DINT_PKT_SCH_RC5_4X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC5_4X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC5_4X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC5_4X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC5_4X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC5_4X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC5_4X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC5_4X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC5_4X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC5_4X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC5_4X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC5_4X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC5_4X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC5_4X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC5_4X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC5_4X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC5_4X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC5_4X_RATE_LTU_CRC_V

  },

   /********** RC5 8X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC5_8X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC5_8X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC5_8X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC5_8X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC5_8X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC5_8X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC5_8X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC5_8X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC5_8X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC5_8X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC5_8X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC5_8X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC5_8X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC5_8X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC5_8X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC5_8X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC5_8X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC5_8X_RATE_LTU_CRC_V

  },

   /********** RC5 16X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC5_16X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC5_16X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC5_16X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC5_16X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC5_16X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC5_16X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC5_16X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC5_16X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC5_16X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC5_16X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC5_16X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC5_16X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC5_16X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC5_16X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC5_16X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC5_16X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC5_16X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC5_16X_RATE_LTU_CRC_V

  }
}, /* end RC5 packet parameters */

{

   /********** RC11 1X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC11_1X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC11_1X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC11_1X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC11_1X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC11_1X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC11_1X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC11_1X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC11_1X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC11_1X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC11_1X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC11_1X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC11_1X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC11_1X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC11_1X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC11_1X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC11_1X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC11_1X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC11_1X_RATE_LTU_CRC_V
  },

   /********** RC11 2X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC11_2X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC11_2X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC11_2X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC11_2X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC11_2X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC11_2X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC11_2X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC11_2X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC11_2X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC11_2X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC11_2X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC11_2X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC11_2X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC11_2X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC11_2X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC11_2X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC11_2X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC11_2X_RATE_LTU_CRC_V
  },

   /********** RC11 4X SCH Packet Parameters ***********/

  {
    DINT_PKT_SCH_RC11_4X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC11_4X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC11_4X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC11_4X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC11_4X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC11_4X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC11_4X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC11_4X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC11_4X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC11_4X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC11_4X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC11_4X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC11_4X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC11_4X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC11_4X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC11_4X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC11_4X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC11_4X_RATE_LTU_CRC_V

  },

   /********** RC11 8X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC11_8X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC11_8X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC11_8X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC11_8X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC11_8X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC11_8X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC11_8X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC11_8X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC11_8X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC11_8X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC11_8X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC11_8X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC11_8X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC11_8X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC11_8X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC11_8X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC11_8X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC11_8X_RATE_LTU_CRC_V

  },

   /********** RC11 16X SCH Packet Parameters ***********/

  {

    DINT_PKT_SCH_RC11_16X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC11_16X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC11_16X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC11_16X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC11_16X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC11_16X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC11_16X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC11_16X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC11_16X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC11_16X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC11_16X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC11_16X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC11_16X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC11_16X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC11_16X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC11_16X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC11_16X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC11_16X_RATE_LTU_CRC_V

},

     /********** RC11 32X SCH Packet Parameters ***********/
  {

    DINT_PKT_SCH_RC11_32X_RATE_ROW_WIDTH_V |
    DINT_PKT_SCH_RC11_32X_RATE_CHAINBACK_DEPTH_V |
    DINT_PKT_SCH_RC11_32X_RATE_YM_THRESH_V |
    DINT_PKT_SCH_RC11_32X_RATE_SMT_SCALAR_V,

    DINT_PKT_SCH_RC11_32X_RATE_INTLV_COLS_V |
    DINT_PKT_SCH_RC11_32X_RATE_INTLV_ROWS_V,

    DINT_PKT_SCH_RC11_32X_RATE_BLOCK_SIZE_V,

    DINT_PKT_SCH_RC11_32X_RATE_LTU_LEN_HI_V |
    DINT_PKT_SCH_RC11_32X_RATE_PUNCT_LENGTH_V |
    DINT_PKT_SCH_RC11_32X_RATE_SYMBOL_REPEAT_V |
    DINT_PKT_SCH_RC11_32X_RATE_CODE_RATE_V,

    DINT_PKT_SCH_RC11_32X_RATE_LTU_LEN_LO_V |
    DINT_PKT_SCH_RC11_32X_RATE_LO_PUNCT_PATTERN_V |
    DINT_PKT_SCH_RC11_32X_RATE_RDCNT_EN_SEL_V |
    DINT_PKT_SCH_RC11_32X_RATE_NUM_LTU_FRM_V,

    DINT_PKT_SCH_RC11_32X_RATE_HI_PUNCT_PATTERN_V,

    DINT_PKT_SCH_RC11_32X_RATE_PL_CRC_V,

    DINT_PKT_SCH_RC11_32X_RATE_LTU_CRC_V
  }

} /* end RC11 packet parameters */

};


/*=====================================================
  TURBO DECODER PARAMETERS for RC3-RC5 Supplemental
  Channels (IS95C-SCH)
=======================================================*/

static const deint_td_params_type
  deint_td_sch_params[NUM_SCH_RC][NUM_TD_SCH_RATES] = {

  {
     /**** RC3 2X Deinterleaver/Turbo Decoder Parameters ****/

     {
       DINT_TD_SCH_RC3_2X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC3_2X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC3_2X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC3_2X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC3_2X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC3_2X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC3_2X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC3_2X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC3_2X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC3_2X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC3_2X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC3_2X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC3_2X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP_2K,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC3_2X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC3_2X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC3_2X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC3_2X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC3_2X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC3_2X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC3_2X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC3_2X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC3_2X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC3_2X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC3_2X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC3_2X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /**** RC3 4X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC3_4X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC3_4X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC3_4X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC3_4X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC3_4X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC3_4X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC3_4X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC3_4X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC3_4X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC3_4X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC3_4X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC3_4X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC3_4X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC3_4X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC3_4X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC3_4X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC3_4X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC3_4X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC3_4X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC3_4X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC3_4X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC3_4X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC3_4X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC3_4X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC3_4X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC3 8X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC3_8X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC3_8X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC3_8X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC3_8X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC3_8X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC3_8X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC3_8X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC3_8X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC3_8X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC3_8X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC3_8X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC3_8X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC3_8X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC3_8X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC3_8X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC3_8X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC3_8X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC3_8X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC3_8X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC3_8X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC3_8X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC3_8X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC3_8X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC3_8X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC3_8X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC3 16X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC3_16X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC3_16X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC3_16X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC3_16X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC3_16X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC3_16X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC3_16X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC3_16X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC3_16X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC3_16X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC3_16X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC3_16X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC3_16X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC3_16X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC3_16X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC3_16X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC3_16X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC3_16X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC3_16X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC3_16X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC3_16X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC3_16X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC3_16X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC3_16X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC3_16X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     }
  }, /* end RC3 turbo packet parameters */

  {
     /**** RC4 2X Deinterleaver/Turbo Decoder Parameters ****/

     {
       DINT_TD_SCH_RC4_2X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC4_2X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC4_2X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC4_2X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC4_2X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC4_2X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC4_2X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC4_2X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC4_2X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_2X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_2X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_2X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_2X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP_2K,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_2X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_2X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_2X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_2X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_2X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_2X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_2X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_2X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_2X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_2X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_2X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_2X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /**** RC4 4X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC4_4X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC4_4X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC4_4X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC4_4X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC4_4X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC4_4X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC4_4X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC4_4X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC4_4X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_4X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_4X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_4X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_4X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_4X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_4X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_4X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_4X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_4X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_4X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_4X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_4X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_4X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_4X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_4X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_4X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC4 8X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC4_8X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC4_8X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC4_8X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC4_8X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC4_8X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC4_8X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC4_8X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC4_8X_RATE_HI_PUNCT_PATTERN_V,
       DINT_TD_SCH_RC4_8X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_8X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_8X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_8X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_8X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_8X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_8X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_8X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_8X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_8X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_8X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_8X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_8X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_8X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_8X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_8X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_8X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC4 16X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC4_16X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC4_16X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC4_16X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC4_16X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC4_16X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC4_16X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC4_16X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC4_16X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC4_16X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_16X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_16X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_16X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_16X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_16X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_16X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_16X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_16X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_16X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_16X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_16X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_16X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_16X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_16X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_16X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_16X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC4 32X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC4_32X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC4_32X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC4_32X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC4_32X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC4_32X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC4_32X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC4_32X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC4_32X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC4_32X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC4_32X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC4_32X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC4_32X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC4_32X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC4_32X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC4_32X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC4_32X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC4_32X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC4_32X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC4_32X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC4_32X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC4_32X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC4_32X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC4_32X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC4_32X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC4_32X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     }
  }, /* end RC4 turbo packet parameters */


  {
     /**** RC5 2X Deinterleaver/Turbo Decoder Parameters ****/

     {
       DINT_TD_SCH_RC5_2X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC5_2X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC5_2X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC5_2X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC5_2X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC5_2X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC5_2X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC5_2X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC5_2X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC5_2X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC5_2X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC5_2X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC5_2X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC5_2X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC5_2X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC5_2X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC5_2X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC5_2X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC5_2X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC5_2X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC5_2X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC5_2X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC5_2X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC5_2X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC5_2X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /**** RC5 4X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC5_4X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC5_4X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC5_4X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC5_4X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC5_4X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC5_4X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC5_4X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC5_4X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC5_4X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC5_4X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC5_4X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC5_4X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC5_4X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC5_4X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC5_4X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC5_4X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC5_4X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC5_4X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC5_4X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC5_4X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC5_4X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC5_4X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC5_4X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC5_4X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC5_4X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC5 8X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC5_8X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC5_8X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC5_8X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC5_8X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC5_8X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC5_8X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC5_8X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC5_8X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC5_8X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC5_8X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC5_8X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC5_8X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC5_8X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC5_8X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC5_8X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC5_8X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC5_8X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC5_8X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC5_8X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC5_8X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC5_8X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC5_8X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC5_8X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC5_8X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC5_8X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC5 16X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC5_16X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC5_16X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC5_16X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC5_16X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC5_16X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC5_16X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC5_16X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC5_16X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC5_16X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC5_16X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC5_16X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC5_16X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC5_16X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC5_16X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC5_16X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC5_16X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC5_16X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC5_16X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC5_16X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC5_16X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC5_16X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC5_16X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC5_16X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC5_16X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC5_16X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     }
  }, /* end RC5 turbo packet parameters */

  {
     /**** RC11 2X Deinterleaver/Turbo Decoder Parameters ****/

     {
       DINT_TD_SCH_RC11_2X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC11_2X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC11_2X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC11_2X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC11_2X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC11_2X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC11_2X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC11_2X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC11_2X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_2X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_2X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_2X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_2X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP_2K,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_2X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_2X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_2X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_2X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_2X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_2X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_2X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_2X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_2X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_2X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_2X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_2X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /**** RC11 4X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC11_4X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC11_4X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC11_4X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC11_4X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC11_4X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC11_4X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC11_4X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC11_4X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC11_4X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_4X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_4X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_4X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_4X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_4X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_4X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_4X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_4X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_4X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_4X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_4X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_4X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_4X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_4X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_4X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_4X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC11 8X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC11_8X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC11_8X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC11_8X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC11_8X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC11_8X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC11_8X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC11_8X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC11_8X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC11_8X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_8X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_8X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_8X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_8X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_8X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_8X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_8X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_8X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_8X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_8X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_8X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_8X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_8X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_8X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_8X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_8X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC11 16X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC11_16X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC11_16X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC11_16X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC11_16X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC11_16X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC11_16X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC11_16X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC11_16X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC11_16X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_16X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_16X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_16X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_16X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_16X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_16X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_16X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_16X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_16X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_16X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_16X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_16X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_16X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_16X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_16X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_16X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     },

     /***** RC4 32X Deinterleaver/Turbo Decoder Parameters *****/

     {
       DINT_TD_SCH_RC11_32X_RATE_BLOCK_SIZE_V,

       DINT_TD_SCH_RC11_32X_RATE_ROW_WIDTH_V,

       DINT_TD_SCH_RC11_32X_RATE_INTLV_COLS_V |
       DINT_TD_SCH_RC11_32X_RATE_INTLV_ROWS_V,

       DINT_TD_SCH_RC11_32X_RATE_LO_PUNCT_PATTERN_V |
       DINT_TD_SCH_RC11_32X_RATE_PUNCT_LENGTH_V |
       DINT_TD_SCH_RC11_32X_RATE_CODE_RATE_V,

       DINT_TD_SCH_RC11_32X_RATE_HI_PUNCT_PATTERN_V,

       DINT_TD_SCH_RC11_32X_RATE_TD_INTLV_LEN_V,

       DINT_TD_COMMON_CFG0_IRQ_TIMING_SEL |
       DINT_TD_COMMON_CFG0_CRC_WIDTH_SEL,

       DINT_TD_COMMON_CFG1_INTLV_LEN_M1_RC11_32X   |
       DINT_TD_COMMON_CFG1_MIN_LLR        |
       DINT_TD_COMMON_CFG1_LLR_SCALE_SEL,

       DINT_TD_COMMON_CFG2_NUM_COL_M1_RC11_32X     |
       DINT_TD_COMMON_CFG2_LAST_COL_M1_RC11_32X    |
       DINT_TD_COMMON_CFG2_LAST_ROW_M1_RC11_32X    |
       DINT_TD_COMMON_CFG2_MIN_ITER       |
       DINT_TD_COMMON_CFG2_MAX_ITER       |
       DINT_TD_COMMON_CFG2_EARLY_TERM_EN  |
       DINT_TD_COMMON_CFG2_NUM_TD_MAP,

       DINT_TD_COMMON_CFG3_FWD0_ITV_ROW_BND_RC11_32X   |
       DINT_TD_COMMON_CFG3_FWD1_ITV_ROW_BND_RC11_32X   |
       DINT_TD_COMMON_CFG3_FWD2_ITV_ROW_BND_RC11_32X   |
       DINT_TD_COMMON_CFG3_FWD0_SEQ_ROW_BND_RC11_32X   |
       DINT_TD_COMMON_CFG3_FWD1_SEQ_ROW_BND_RC11_32X   |
       DINT_TD_COMMON_CFG3_FWD2_SEQ_ROW_BND_RC11_32X,

       DINT_TD_COMMON_CFG4_FWD0_ITV_COL_BND_RC11_32X   |
       DINT_TD_COMMON_CFG4_FWD1_ITV_COL_BND_RC11_32X   |
       DINT_TD_COMMON_CFG4_FWD2_ITV_COL_BND_RC11_32X,

       DINT_TD_COMMON_CFG5_FWD0_SEQ_COL_BND_RC11_32X   |
       DINT_TD_COMMON_CFG5_FWD1_SEQ_COL_BND_RC11_32X   |
       DINT_TD_COMMON_CFG5_FWD2_SEQ_COL_BND_RC11_32X,

       DINT_TD_1X_CFG_TDECIB_IDX      |
       DINT_TD_1X_CFG_CONSEC_CRC_M1   |
       DINT_TD_1X_CFG_DEC_BIAS,
     }
  } /* end RC11 turbo packet parameters */

};

struct {
  /* DEMBACK 0 */
  uint32 dint_ch0_cfg;
  uint32 dint_ch1_cfg;
  uint32 dint_ch2_cfg;

  /* DEMBACK 1 */
  uint32 dint_ch0_cfg_db1;
  uint32 dint_ch1_cfg_db1;
  uint32 dint_ch2_cfg_db1;
} deint_hwio_shadow_reg;

/*--------------------------------------------------------------------------
 * Private Function Prototypes
 *--------------------------------------------------------------------------*/
static void deint_load_sc_pkt_params (void);
static void deint_create_sc_task_list (void);

static void deint_load_pc_pkt_params (deint_pc_rate_type pc_rate);
static void deint_create_pc_task_list (void);

static boolean deint_load_fch_pkt_params (deint_rc_type fch_rc);

#ifndef FEATURE_IS2000_REL_A_SVD
static void deint_create_fch_task_list (
  deint_rc_type fch_rc,
  boolean int_enable
);
#else
static void deint_create_fch_task_list (
  deint_rc_type fch_rc,
  boolean       fch_pc
);

static void deint_create_dcch_task_list
(
  deint_rc_type dcch_rc,
  boolean dcch_pc
);

static void deint_load_dcch_pkt_params (deint_rc_type dcch_rc);
#endif /* !FEATURE_IS2000_REL_A_SVD */

static void deint_load_sch_pkt_params (
  #ifdef FEATURE_IS2000_REL_A //32X
  uint8   num_ltu,
  uint16  ltu_len,
  #endif /* FEATURE_IS2000_REL_A */
  deint_rc_type sch_rc,
  dec_sch_rate_type sch_rate
);
static void deint_create_sch_task_list (
  #ifdef FEATURE_IS2000_REL_A
  dec_mux_pdu_type_type pdu_type,
  #endif
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate
);

static void deint_create_dummy_sch_task_list (void);
void deint_load_td_deint_params
(
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate
);

void deint_load_td_dec_params
(
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate
);

/*============================================================================

FUNCTION DEINT_SET_DINT_CFG

DESCRIPTION
  Configure the DINT_CFG register to point the L, M, N, and P RAMs to the
  proper physical memory pages and channels.

DEPENDENCIES
  The Dec clocks must be enabled by caller.

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void deint_set_dint_cfg ( void )
{
  SET_DEMBACK_REG( DINT_CFG, (0 << HWIO_SHFT(DINT_CFG, L_ODD_RAM_PAGE_SEL)) |
                             (0 << HWIO_SHFT(DINT_CFG, M_ODD_RAM_PAGE_SEL)) |
                             (0 << HWIO_SHFT(DINT_CFG, N_ODD_RAM_PAGE_SEL)) |
                             (1 << HWIO_SHFT(DINT_CFG, P_ODD_RAM_PAGE_SEL)) |
                             (( DINT_CH0_RAM_L_V | DINT_CH1_RAM_M_V |
                                DINT_CH2_RAM_N_V | DINT_CH2_RAM_P_V ) <<
                                       HWIO_SHFT(DINT_CFG, CHAN_RAM_ALLOC)) );

}

/*============================================================================

FUNCTION DEINT_INIT

DESCRIPTION
  Initializes the deinterleaver.
  Calls dec_init to initialize the SVD Polynomials.

DEPENDENCIES
  The Dec clocks must be enabled by caller.

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void deint_init ( void )
{
  curr_deint_state = DEINT_CDMA_STATE;

  SET_DEMBACK_REG( TST_SEL_DINT, 0x0000 );

  /* Initialize the task list */
  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_FIRST_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_SECOND_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_THIRD_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_FOURTH_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

  dec_init();
  deint_set_dint_cfg();

} /* deint_init */

/*============================================================================

FUNCTION DEINT_QUEUE_INIT

DESCRIPTION
  Initializes the queue.
  Calls q_init, q_put abd q_link to allocate memory for queue mutex.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void deint_queue_init( void )
{
  byte i;

  (void) q_init(&deint_sequencing_cmd_q);
  (void) q_init(&deint_sequencing_free_q);

  for(i=0; i< DEINT_MAX_SEQUENCING_CMD; i++)
  {
    deint_sequencing_cmd_buf[i].hdr.done_q_ptr = &deint_sequencing_free_q;
    q_put(&deint_sequencing_free_q,
          q_link(&deint_sequencing_cmd_buf[i], &deint_sequencing_cmd_buf[i].
                                                hdr.link));
   }
} /* deint_queue_init */

/*===========================================================================

FUNCTION DEINT_SET_SC_MODE

DESCRIPTION
    This function calls dec_sc_init() which puts the decoder in continous mode
    and resets the SVD.  This function loads the deinterleaver packet parameters
    for SYNC Channel.

DEPENDENCIES
    deint_init() should have been called

RETURN VALUE
  None

SIDE EFFECTS
   The deinterleaver packet parameters for SYNC Channel are loaded in PACKET
   RAM.
=============================================================================*/
void deint_set_sc_mode
(
  void
)
{
  dec_sc_init();

  #ifdef FEATURE_IS2000_REL_A_SVD
  /* Avoid resetting decoder while in traffic state.
  */
  curr_deint_state = DEINT_SYNC_STATE;
  #endif /* FEATURE_IS2000_REL_A_SVD */

  SET_DINT_CH_CFG(0, DINT_CH0_CFG__SYNC_CHAN_80M_BND_MASK
                     | DINT_CH0_CFG__NEXT_20M_BND_MASK,
                  DINT_SYNC_CHAN_80M_BND_V);

  SET_DEMBACK_REG( DINT_OTD_CFG, 0x0000 );   /* NO OTD */

  SET_DINT_CH_CFG (1, DINT_CH1_CFG__NEXT_20M_BND_MASK, 0x00);
  SET_DINT_CH_CFG (2, DINT_CH2_CFG__NEXT_20M_BND_MASK, 0x00);

  deint_load_sc_pkt_params();
  deint_create_sc_task_list();
  deint_set_dint_cfg();
}

/*==========================================================================
FUNCTION DEINT_SET_PC_MODE

DESCRIPTION
  This function calls dec_pc_init() which puts the decoder in continous
  mode and resets the SVD. This function loads the deinterleaver packet
  parameters for PAGING Channel.

DEPENDENCIES
 deint_init() should have been called.

RETURN VALUE
  None

SIDE EFFECTS
   The deinterleaver packet parameters for PAGING Channel are loaded in PACKET
   RAM.
=============================================================================*/
void deint_set_pc_mode
(
  deint_pc_rate_type pc_rate
  /* Indicates the Paging Channel Rate : 9600bps / 4800bps */
)
{
  #ifdef FEATURE_IS2000_REL_A_SVD
  /* Avoid resetting decoder while in traffic state.
  */
  curr_deint_state = DEINT_PAGING_IDLE_STATE;
  #endif /* FEATURE_IS2000_REL_A_SVD */

  dec_pc_init((dec_pc_rate_type)pc_rate);

  /* Clear The SYNC_CHAN_80_M_BND for Paging Channel */
  SET_DINT_CH_CFG( 0, DINT_CH0_CFG__NEXT_20M_BND_MASK |
                      DINT_CH0_CFG__SYNC_CHAN_80M_BND_MASK,
                   DINT_NEXT_20M_BND_V );

  SET_DEMBACK_REG( DINT_OTD_CFG, 0x0000 );   /* NO OTD  1 BANK INTERLEAVING */

  deint_load_pc_pkt_params(pc_rate);
  deint_create_pc_task_list();
  deint_set_dint_cfg();
}

/*==========================================================================
FUNCTION DEINT_SET_FCH_MODE

DESCRIPTION
    This function calls dec_tc_init() which puts the decoder in packet
    mode and resets the SVD. This function loads the deinterleaver packet
    parameters for FCH of a given Radio Configuration.

DPENEDENCIES
    deint_init should have been called.

RETURN VALUE
  None

SIDE EFFECTS


=============================================================================*/
#ifndef FEATURE_IS2000_REL_A_SVD
void deint_set_fch_mode
(
  deint_rc_type fch_rc
)
{
  deint_sequencing_cmd_type *cmd_ptr;  /* Sequencing Command Ptr */

   dec_tc_init();

   deint_set_dint_cfg ();

   SET_DINT_CH_CFG( 0, DINT_CH0_CFG__NEXT_20M_BND_MASK, DINT_NEXT_20M_BND_V );
   SET_DINT_CH_CFG( 0, DINT_CH0_CFG__NUM_SUB_CHANS_MASK, 0x0000 );

   if(fch_rc >= DEINT_RC3)
   {
     /* IS95C  2 Bank deinterleaving; NO OTD */
     SET_DEMBACK_REG_F( DINT_OTD_CFG, OTD_SEL, 1 );
   }
   else
   {
     /* IS95A 1 Bank deinterleaving ; NO OTD */
     SET_DEMBACK_REG_F( DINT_OTD_CFG, OTD_SEL, 0 );
   }

  if(fch_rc == DEINT_RC11)
  {
    /* This sequence deletes the FCH task from the deinterleaver task
       RAM.  For legacy RCs, the task list is deleted when the channel is
       is torn down.  However, for RC11 it is necessary to clear the FCH task
       when the channel is set up.  For this RC, MDSP is responsible for
       programming the FCH deinterleaver task, not SW.

       There will be no decoder interrupt for PCGs 0 to 3 after this function
       executes.
    */

    /* Select first 5ms bin of the deinterleaver task RAM */
    SET_DEMBACK_REG( DINT_TASK_OFFSET, 0x0000 );

    /* We now need to create a zero task to indicate to the task arbitartor that
    ** there is no other task in this 5ms bin.
    */
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
  }
  else
  {
    if((cmd_ptr = (deint_sequencing_cmd_type *)
                q_get(&deint_sequencing_free_q)) == NULL)
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "0 Bufs on seq_free_q");
      /* Just incase if this happens, program the read parameters here */
      deint_load_fch_pkt_params(fch_rc);
      deint_create_fch_task_list(fch_rc, TRUE);
    }
    else
    {
       cmd_ptr->hdr.command = DEINT_FCH_READ_PARAMS;
       cmd_ptr->fch_cmd.fch_rc = fch_rc;
       q_put(&deint_sequencing_cmd_q, &cmd_ptr->hdr.link);
    }
  }
}

#else
void deint_set_fch_mode
(
  deint_rc_type fch_rc,
  boolean power_control_channel
)
{
  deint_sequencing_cmd_type *cmd_ptr;  /* Sequencing Command Ptr */

  if (curr_deint_state != DEINT_TRAFFIC_STATE)
  {
    /* Only perform these init operations at the start of traffic state.
    */
    dec_tc_init();
    curr_deint_state = DEINT_TRAFFIC_STATE;
    curr_deint_ch_active = DEINT_CH_INACTIVE;
  }

  /* Update configuration of fundicated channels FCH and DCCH.
  */
  if (curr_deint_ch_active == DEINT_CH_INACTIVE)
  {
    curr_deint_ch_active = DEINT_FCH_ACTIVE;
    M1X_MSG( MUX, LEGACY_HIGH,
      "Set_fch_mode,new=FCH,fch_rc=%d",
      fch_rc);
  }
  else if (curr_deint_ch_active == DEINT_DCCH_ACTIVE)
  {
    curr_deint_ch_active = DEINT_FCH_DCCH_ACTIVE;
    M1X_MSG( MUX, LEGACY_HIGH,
      "Set_fch_mode,new=FCH_DCCH,fch_rc=%d",
      fch_rc);
  }
  else if (curr_deint_ch_active == DEINT_FCH_ACTIVE)
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Set_fch_mode called in FCH mode,fch_rc=%d",
      fch_rc);
  }
  else if (curr_deint_ch_active == DEINT_FCH_DCCH_ACTIVE)
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Set_fch_mode called in FCH_DCCH mode,fch_rc=%d",
      fch_rc);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Bad val curr_deint_ch_act: %d",
      curr_deint_ch_active);
  }

  deint_set_dint_cfg();

  if(fch_rc >= DEINT_RC3)
  {
    /* IS95C  2 Bank deinterleaving; NO OTD */
    SET_DEMBACK_REG_F( DINT_OTD_CFG, OTD_SEL, 1 );
  }
  else
  {
    /* IS95A 1 Bank deinterleaving ; NO OTD */
    SET_DEMBACK_REG_F( DINT_OTD_CFG, OTD_SEL, 0 );
  }

  if(fch_rc == DEINT_RC11)
  {
    /* This sequence deletes the FCH task from the deinterleaver task
       RAM.  For legacy RCs, the task list is deleted when the channel is
       is torn down.  However, for RC11 it is necessary to clear the FCH task
       when the channel is set up.  For this RC, MDSP is responsible for
       programming the FCH deinterleaver task, not SW.

       There will be no decoder interrupt for PCGs 0 to 3 after this function
       executes.
    */

    /* Select first 5ms bin of the deinterleaver task RAM */
    SET_DEMBACK_REG( DINT_TASK_OFFSET, 0x0000 );

    /* We now need to create a zero task to indicate to the task arbitartor that
    ** there is no other task in this 5ms bin.
    */
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
  }
  else
  {
    if((cmd_ptr = (deint_sequencing_cmd_type *)
                  q_get(&deint_sequencing_free_q)) == NULL)
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "0 Bufs on seq_free_q");

      /* Just incase if this happens, program the read parameters here */
      (void) deint_load_fch_pkt_params(fch_rc);
      deint_create_fch_task_list (fch_rc,
                                  power_control_channel
                                 );
    }
    else
    {
      cmd_ptr->hdr.command = DEINT_FCH_READ_PARAMS;
      cmd_ptr->fch_cmd.fch_rc = fch_rc;
      cmd_ptr->fch_cmd.fch_pc = power_control_channel;
      q_put(&deint_sequencing_cmd_q, &cmd_ptr->hdr.link);
    }
  }

} /* end deint_set_fch_mode */

/*==========================================================================
FUNCTION DEINT_SET_DCCH_MODE

DESCRIPTION
  This function calls dec_tc_init() which puts the decoder in packet
  mode and resets the SVD (Serial Viterbi decoder.) This function loads the
  deinterleaver packet parameters for DCCH of a given Radio Configuration.

DEPENDENCIES
  deint_init should have been called.

RETURN VALUE
  None

SIDE EFFECTS
  Reset decoders.


=============================================================================*/

void deint_set_dcch_mode
(
  deint_rc_type dcch_rc,
  boolean power_control_channel
)
{
  deint_sequencing_cmd_type *cmd_ptr;  /* Sequencing Command Ptr */

  if (curr_deint_state != DEINT_TRAFFIC_STATE)
  {
    /* Only perform these init operations at the start of traffic state.
    */
    dec_tc_init();
    curr_deint_state = DEINT_TRAFFIC_STATE;
    curr_deint_ch_active = DEINT_CH_INACTIVE;
  }

  /* Update configuration of fundicated channels FCH and DCCH.
  */
  if (curr_deint_ch_active == DEINT_CH_INACTIVE)
  {
    curr_deint_ch_active = DEINT_DCCH_ACTIVE;
    M1X_MSG( MUX, LEGACY_HIGH,
      "Set_dcch_mode,new=DCCH,dcch_rc=%d",
      dcch_rc);
  }
  else if (curr_deint_ch_active == DEINT_FCH_ACTIVE)
  {
    curr_deint_ch_active = DEINT_FCH_DCCH_ACTIVE;
    M1X_MSG( MUX, LEGACY_HIGH,
      "Set_dcch_mode,new=FCH_DCCH,dcch_rc=%d",
      dcch_rc);
  }
  else if (curr_deint_ch_active == DEINT_DCCH_ACTIVE)
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Set_dcch_mode called in DCCH mode,dcch_rc=%d",
      dcch_rc);
  }
  else if (curr_deint_ch_active == DEINT_FCH_DCCH_ACTIVE)
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Set_dcch_mode called in FCH_DCCH mode,dcch_rc=%d",
      dcch_rc);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Bad val deint_ch_act: %d",
      curr_deint_ch_active);
  }

  deint_set_dint_cfg();

  if(dcch_rc >= DEINT_RC3)
  {
    /* IS95C  2 Bank deinterleaving; NO OTD */
    SET_DEMBACK_REG_F( DINT_OTD_CFG, OTD_SEL, 1 );
  }
  else
  {
    /* IS95A 1 Bank deinterleaving ; NO OTD */
    SET_DEMBACK_REG_F( DINT_OTD_CFG, OTD_SEL, 0 );
  }

  /* FOR DCCH decoding, use deint_load_dcch_pkt_params()) to load packet
  ** parameters (need to use a different packet to co-exist with FCH.)
  */
  if((cmd_ptr = (deint_sequencing_cmd_type *)
                q_get(&deint_sequencing_free_q)) == NULL)
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "0 Bufs on seq_free_q");

    /* Just incase if this happens, program the read parameters here */
    deint_load_dcch_pkt_params(dcch_rc);
    deint_create_dcch_task_list(dcch_rc, power_control_channel);
  }
  else
  {
    cmd_ptr->hdr.command = DEINT_DCCH_READ_PARAMS;
    cmd_ptr->dcch_cmd.dcch_rc = dcch_rc;
    cmd_ptr->dcch_cmd.dcch_pc = power_control_channel;
    q_put(&deint_sequencing_cmd_q, &cmd_ptr->hdr.link);
  }
} /* end deint_set_dcch_mode () */
#endif /* !FEATURE_IS2000_REL_A_SVD */


/*==========================================================================
FUNCTION DEINT_SET_SCH_MODE

DESCRIPTION
    This function will initialize the deinterleaver for Supplemental Channels.
    This function loads the deinterleaver packet
    parameters for SCH of a given Radio Configuration.

DPENEDENCIES
    deint_init should have been called.

RETURN VALUE
  None

SIDE EFFECTS


=============================================================================*/
void deint_set_sch_mode
(

#ifdef FEATURE_IS2000_REL_A //32X
  uint8   num_ltu,
  uint16  ltu_len,
  dec_mux_pdu_type_type pdu_type,
#endif /* FEATURE_IS2000_REL_A */

  deint_rc_type sch_rc,
  dec_coding_type dec_coding,
  dec_sch_rate_type dec_sch_rate
)
{
  deint_sequencing_cmd_type *cmd_ptr;  /* Sequencing Command Ptr */

  deint_set_dint_cfg ();

  SET_DINT_CH_CFG (2, DINT_CH2_CFG__NEXT_20M_BND_MASK,
                      DINT_CH2_CFG__NEXT_20M_BND_MASK);

  if(dec_coding == DEC_CONVOLUTIONAL)
  {
    /* Turn off Turbo mode */
    SET_DEMBACK_REG_F( DINT_OTD_CFG, CH2_CODE_TYPE, 0 );

    #if defined( FEATURE_MODEM_1X_SW_DEMBACK ) && !defined( FEATURE_MODEM_1X_ATLAS_REVISIT )
    /* Dont inform FW if SW DEMBACK is in use */
    if( !muxmdsp_get_is_sw_demback_active() )
    #endif /* FEATURE_MODEM_1X_SW_DEMBACK && !FEATURE_MODEM_1X_ATLAS_REVISIT */
    {
      /* Inform FW about the 1x DATA call (no-TDEC) */
      muxmdsp_data_brdg_enable(TRUE, FALSE);
    }

    if((cmd_ptr = (deint_sequencing_cmd_type *)
            q_get(&deint_sequencing_free_q)) == NULL)
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "0 Bufs on seq_free_q");
      /* Just incase if this happens, program the read parameters here */
     if(dec_sch_rate != DEC_SCH_RATE_1X)
     {
       /* Load new packet parameters only if SCH Rate is not 1X.
          Packet parameters for 1X SCH are the same as for full
          rate FCH (Packet A). */

#ifdef FEATURE_IS2000_REL_A //32X
       deint_load_sch_pkt_params(num_ltu, ltu_len, sch_rc, dec_sch_rate);
#else
       deint_load_sch_pkt_params(sch_rc, dec_sch_rate);
#endif /* FEATURE_IS2000_REL_A */
     }
#ifdef FEATURE_IS2000_REL_A //32X
     deint_create_sch_task_list(pdu_type, sch_rc, dec_sch_rate);
#else
     deint_create_sch_task_list(sch_rc, dec_sch_rate);
#endif /* FEATURE_IS2000_REL_A */
    }
    else
    {
     cmd_ptr->hdr.command = DEINT_SCH_READ_PARAMS;
     cmd_ptr->sch_cmd.sch_rc = sch_rc;
     cmd_ptr->sch_cmd.dec_sch_rate = dec_sch_rate;
     cmd_ptr->sch_cmd.dec_coding = DEC_CONVOLUTIONAL;
#ifdef FEATURE_IS2000_REL_A //32X
     cmd_ptr->sch_cmd.ltu_len = ltu_len;
     cmd_ptr->sch_cmd.num_ltu = num_ltu;
     cmd_ptr->sch_cmd.pdu_type = pdu_type;
#endif /* FEATURE_IS2000_REL_A */
     q_put(&deint_sequencing_cmd_q, &cmd_ptr->hdr.link);
    }
  }
  else
  {
    /* Turbo Coding */
    SET_DEMBACK_REG_F( DINT_OTD_CFG, CH2_CODE_TYPE, 1 );

    #if defined( FEATURE_MODEM_1X_SW_DEMBACK ) && !defined( FEATURE_MODEM_1X_ATLAS_REVISIT )
    /* Dont inform FW if SW DEMBACK is in use */
    if( !muxmdsp_get_is_sw_demback_active() )
    #endif /* FEATURE_MODEM_1X_SW_DEMBACK && !FEATURE_MODEM_1X_ATLAS_REVISIT */
    {
      /* Ask FW to setup the TDEC bridge */
      muxmdsp_data_brdg_enable(TRUE, TRUE);
    }

    /* Ensure the TDEC is in CDMA mode prior to 1x usage */
    SET_DEMBACK_REG( TDEC_MODE_CTL, 0 );

    /* Turbo Decoding */
    dec_sch_rate = (dec_sch_rate_type)((uint16) dec_sch_rate -
                                       (uint16) DEC_SCH_RATE_2X);
    deint_load_td_deint_params(sch_rc, dec_sch_rate);

    /* We have told TD not to generate a intterupt after SCH frame is
       decoded, so we create a dummy task in the second 5ms slot that
       will generate an interrupt */
    if((cmd_ptr = (deint_sequencing_cmd_type *)
        q_get(&deint_sequencing_free_q)) == NULL)
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "0 Bufs on seq_free_q");
      deint_load_td_dec_params(sch_rc, dec_sch_rate);
      deint_create_dummy_sch_task_list();
    }
    else
    {
      cmd_ptr->hdr.command = DEINT_SCH_READ_PARAMS;
      cmd_ptr->sch_cmd.sch_rc = sch_rc;
      cmd_ptr->sch_cmd.dec_sch_rate = dec_sch_rate;
      cmd_ptr->sch_cmd.dec_coding = DEC_TURBO;
      q_put(&deint_sequencing_cmd_q, &cmd_ptr->hdr.link);
    }
   }

  M1X_MSG( MUX, LEGACY_FATAL,
           "DINT_OTD_CFG=[0x%X]",
           GET_DEMBACK_REG( DINT_OTD_CFG ) );
}

/*==========================================================================
FUNCTION DEINT_LOAD_PKT_PARAMS

DESCRIPTION
  This function loads deinterleaver packet parameters.

DEPENDENCIES
   This routine assumes DINT_PKT_OFFSET_WH has been set to assure the
   correct packet RAM is being written.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
static void
deint_load_pkt_params (const deint_pkt_params_type *pkt_params)
{
  /* Note:::: write to DINT_PKT_CFG register causes the address pointer in
   * CFG_RAM to autoincrement by 16 bits at a time
   * So each packet configuration will be selected automatically */

  /* PKT_INTLV_CFG_LO */
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_params->pkt_intlv_cfg_lo );

  /* PKT_INTLV_PKT_HI */
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_params->pkt_intlv_cfg_hi );

  /* PKT_BLK_SIZE_LO */
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_params->pkt_blk_size_lo );

  /* PKT_BLK_SIZE_HI */
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_params->pkt_blk_size_hi );

  /* PKT_PUNCT_LO  */
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_params->pkt_punct_lo );

  /* PKT_PUNCT_HI */
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_params->pkt_punct_hi );

  /* PKT_PL_CRC_LO */
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_params->pkt_pl_crc_lo );

  /* PKT_PL_CRC_HI */
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_params->pkt_pl_crc_hi );

  DEINT_DBG_MSG( "pkt params (cfg)   0x%08X 0x%08X",
                  pkt_params->pkt_intlv_cfg_lo,
                  pkt_params->pkt_intlv_cfg_hi);
  DEINT_DBG_MSG( "pkt params (blk)   0x%08x 0x%08X",
                  pkt_params->pkt_blk_size_lo,
                  pkt_params->pkt_blk_size_hi);
  DEINT_DBG_MSG( "pkt params (punct) 0x%08X 0x%08X",
                  pkt_params->pkt_punct_lo,
                  pkt_params->pkt_punct_hi);
  DEINT_DBG_MSG( "pkt params (crc)   0x%08X 0x%08X",
                  pkt_params->pkt_pl_crc_lo,
                  pkt_params->pkt_pl_crc_hi);

}

/*==========================================================================
FUNCTION DEINT_LOAD_SC_PKT_PARAMS

DESCRIPTION
  This function loads the deinterleaver packet parameters for SYNC Channel.

DEPENDENCIES
   None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
static void deint_load_sc_pkt_params
(
  void
)
{
  /* Select Packet A to program parameters for SYNC Channel Message */

  SET_DEMBACK_REG( DINT_PKT_OFFSET, DINT_PKT_A_V | DINT_PKT_INTLV_CFG_LO_V );

  /* Note:::: write to DINT_PKT_CFG register causes the address pointer in
     CFG_RAM to autoincrement by 16 bits at a time
     So each packet configuration will be selected automatically */

  /* Program for Packet RAM A */
  deint_load_pkt_params (&deint_sc_pkt_params.sc_param);
}

/*===========================================================================
FUNCTION DEINT_LOAD_PC_PKT_PARAMS

DESCRIPTION
  This routine loads packet parameters used to configure CH0 for IS95 PCH
  operation.

DEPENDENCIES
   None

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
static void deint_load_pc_pkt_params
(
  deint_pc_rate_type pc_rate
)
{
  ASSERT (pc_rate < PRAT_COUNT);

  SET_DEMBACK_REG( DINT_PKT_OFFSET, DINT_PKT_A_V | DINT_PKT_INTLV_CFG_LO_V );

  deint_load_pkt_params(&deint_pc_pkt_params.pc_params[pc_rate]);

}

/*===========================================================================
FUNCTION DEINT_LOAD_FCH_PKT_PARAMS

DESCRIPTION
  This function programs 4 packets corresponding to each RC for FCH.
  The four packets correspond to FULL RATE, HALF RATE,QUARTER RATE & 8TH
  HYPOTHESIS.
  The values programmed into the registers take effect at the next SYNC20.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 None
===========================================================================*/
static boolean deint_load_fch_pkt_params
(
  deint_rc_type fch_rc
)
{
  if(fch_rc > DEINT_RC5)
  {
    return FALSE;
  }

  /* Note:::: write to DINT_PKT_CFG register causes the address pointer in
   CFG_RAM to autoincrement by 16 bits at a time
   So each packet configuration will be selected automatically */

  /* Select Packet A to program parameters for FULL Rate Hypothesis */

  SET_DEMBACK_REG( DINT_PKT_OFFSET, DINT_PKT_A_V | DINT_PKT_INTLV_CFG_LO_V );

  /**********************PROGRAMMING FULL RATE HYPOTHESIS *****************/

  deint_load_pkt_params (&deint_fch_packet_params[DEINT_FCH_RC_INDEX(fch_rc)].
            fch_full_rate_param);

 /**********************PROGRAMMING HALF RATE HYPOTHESIS **********************/

  /* Select Packet B to program parameters for HALF Rate Hypothesis */

  SET_DEMBACK_REG( DINT_PKT_OFFSET, DINT_PKT_B_V | DINT_PKT_INTLV_CFG_LO_V );

  deint_load_pkt_params (&deint_fch_packet_params[DEINT_FCH_RC_INDEX(fch_rc)].
            fch_half_rate_param);

  /**********************PROGRAMMING QUARTER RATE HYPOTHESIS *********************/

  /* Select Packet C to program parameters for QUARTER Rate Hypothesis */

  SET_DEMBACK_REG( DINT_PKT_OFFSET, DINT_PKT_C_V | DINT_PKT_INTLV_CFG_LO_V );

  deint_load_pkt_params (&deint_fch_packet_params[DEINT_FCH_RC_INDEX(fch_rc)].
             fch_quarter_rate_param);

   /********************PROGRAMMING EIGHTH RATE HYPOTHESIS *****************/

  /* Select Packet D to program parameters for EIGHTH Rate Hypothesis */

  SET_DEMBACK_REG( DINT_PKT_OFFSET, DINT_PKT_D_V | DINT_PKT_INTLV_CFG_LO_V );

  deint_load_pkt_params (&deint_fch_packet_params[DEINT_FCH_RC_INDEX(fch_rc)].
            fch_eighth_rate_param);

  return TRUE;
}

/*==========================================================================
FUNCTION DEINT_LOAD_TD_DEINT_PARAMS

DESCRIPTION
  This function loads the deinterleaver packet parameters for Turbo Encoded
  Supplemental Channel.
  The values programmed into the registers take effect at the next SYNC20.

DEPENDENCIES
   None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void deint_load_td_deint_params
(
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate
)
{
  M1X_MSG( MUX, LEGACY_HIGH,
    "LOADING TD DEINT,rc=%d,r_off=%d",
    sch_rc,
    dec_sch_rate);

  /* Program The Write Paramters for TD. These Parameters affect the
     deinterleaving operation for Turbo SCH Frames */
  SET_DEMBACK_REG( TD_INTLV_CFG_LO,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                   [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                   td_intlv_cfg_lo );
  SET_DEMBACK_REG( TD_INTLV_CFG_HI,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                   [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                   td_intlv_cfg_hi );

  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( MUX, LEGACY_FATAL,
    "TD_INTLV_CFG_HI [0x%X] TD_INTLV_CFG_LO [0x%X]",
    deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)][DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate,
    NUM_TD_SCH_RATES)].td_intlv_cfg_hi,
    deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)][DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate,
    NUM_TD_SCH_RATES)]. td_intlv_cfg_lo );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
}

/*==========================================================================
FUNCTION DEINT_LOAD_TD_DEC_PARAMS

DESCRIPTION
  This function loads the decoder packet parameters for Turbo Encoded
  Supplemental Channel.
  The values programmed into the registers take effect at the next SYNC20.

DEPENDENCIES
   None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void deint_load_td_dec_params
(
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate
)
{
  M1X_MSG( MUX, LEGACY_HIGH,
    "LOADING TD DEC,rc=%d,r_off=%d",
    sch_rc,
    dec_sch_rate);

  SET_DEMBACK_REG( TD_BLOCK_SIZE,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_block_size );

  SET_DEMBACK_REG( TD_PUNCT_LO,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_punct_lo );

  SET_DEMBACK_REG( TD_PUNCT_HI,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_punct_hi );

  SET_DEMBACK_REG( TD_INTLV_SIZE_LO,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_intlv_size_lo );

  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( MUX, LEGACY_FATAL,
    "TD_BLOCK_SIZE [0x%X] TD_PUNCT_LO [0x%X] TD_PUNCT_HI [0x%X] TD_INTLV_SIZE_LO [0x%X]",
    deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)][DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate,
    NUM_TD_SCH_RATES)].td_block_size,
    deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)][DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate,
    NUM_TD_SCH_RATES)].td_punct_lo,
    deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)][DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate,
    NUM_TD_SCH_RATES)].td_punct_hi,
    deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)][DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate,
    NUM_TD_SCH_RATES)]. td_intlv_size_lo );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

  SET_DEMBACK_REG( TD_COMMON_CFG0,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_common_cfg0 );

  SET_DEMBACK_REG( TD_COMMON_CFG1,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_common_cfg1 );

  SET_DEMBACK_REG( TD_COMMON_CFG2,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_common_cfg2 );

  SET_DEMBACK_REG( TD_COMMON_CFG3,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_common_cfg3 );

  SET_DEMBACK_REG( TD_COMMON_CFG4,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_common_cfg4 );

  SET_DEMBACK_REG( TD_COMMON_CFG5,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_common_cfg5 );

  SET_DEMBACK_REG( TD_1X_CFG,
                   deint_td_sch_params[DEINT_SCH_RC_INDEX(sch_rc)]
                     [DEINT_UPPER_BOUNDS_CHECK(dec_sch_rate, NUM_TD_SCH_RATES)].
                     td_1x_cfg );

  SET_DEMBACK_REG( TD_1X_DECOB_WRITE_PTR, 0 );

  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( MUX, LEGACY_FATAL,
    "TD_COMMON_CFG0 [0x%X] TD_COMMON_CFG1 [0x%X] TD_COMMON_CFG2 [0x%X] TD_COMMON_CFG3 [0x%X]"
    "TD_COMMON_CFG4 [0x%X] TD_COMMON_CFG5 [0x%X] TD_1X_CFG [0x%X] TD_1X_DECOB_WRITE_PTR [0x%X]",
    GET_DEMBACK_REG( TD_COMMON_CFG0 ),
    GET_DEMBACK_REG( TD_COMMON_CFG1 ),
    GET_DEMBACK_REG( TD_COMMON_CFG2 ),
    GET_DEMBACK_REG( TD_COMMON_CFG3 ),
    GET_DEMBACK_REG( TD_COMMON_CFG4 ),
    GET_DEMBACK_REG( TD_COMMON_CFG5 ),
    GET_DEMBACK_REG( TD_1X_CFG ),
    GET_DEMBACK_REG( TD_1X_DECOB_WRITE_PTR ) );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
}

/*==========================================================================
FUNCTION DEINT_LOAD_SCH_PKT_PARAMS

DESCRIPTION
  This function loads the deinterleaver packet parameters for Supplemental
  Channel. The values programmed into the registers take effect at the next
  SYNC20.

DEPENDENCIES
   None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
static void deint_load_sch_pkt_params
(
  #ifdef FEATURE_IS2000_REL_A //32X
  uint8   num_ltu,
  uint16  ltu_len,
  #endif /* FEATURE_IS2000_REL_A */
  deint_rc_type sch_rc,
  dec_sch_rate_type sch_rate
)
{

  #ifdef FEATURE_IS2000_REL_A //32X
  word pkt_blk_size_hi;
  /* punct_length | symbol_repeat | code_rate */
  word pkt_punct_lo;
  /* lo_punct_pattern | rdcnt_en_sel | num_ltu_frm */
  word ltu_len_in_bytes;
  word ltu_len_in_bytes_hi_value;
  word ltu_len_in_bytes_lo_value;
  #endif /* FEATURE_IS2000_REL_A */

  if(sch_rate > DEC_SCH_RATE_32X)
  {
    M1X_MSG( MUX, LEGACY_FATAL,
             "Bad SCH data rate");
  }

  M1X_MSG( MUX, LEGACY_HIGH,
    "SCH Conv DEINT,rc=%d,r_off=%d",
    sch_rc,
    sch_rate);
  /* Select Packet E to program parameters for convolutionally encoded SCH.
    Note Packets A - D are used for FCH. */

  SET_DEMBACK_REG( DINT_PKT_OFFSET, DINT_PKT_E_V | DINT_PKT_INTLV_CFG_LO_V );

  /* Note:::: write to DINT_PKT_CFG register causes the address pointer in
     CFG_RAM to autoincrement by 16 bits at a time
     So each packet configuration will be selected automatically */


   /* Program PKT_INTLV_CFG_LO for Packet E  */
  SET_DEMBACK_REG( DINT_PKT_CFG,
                   deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                     pkt_intlv_cfg_lo );

   /* Program PKT_INTLV_PKT_HI for Packet E */
  SET_DEMBACK_REG( DINT_PKT_CFG,
                   deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                     pkt_intlv_cfg_hi );

   /* Program PKT_BLK_SIZE_LO for Packet E */
  SET_DEMBACK_REG( DINT_PKT_CFG,
                   deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                     pkt_blk_size_lo );

  /****
  When 32X is supported, the upper layer would pass the LTU size, and
  number of LTU per frame to physical layer.  In this case we would use
  the LTU size and # of LTU per frame.
  ****/

  /* Prgram PKT_BLK_SIZE_HI for packet E */
  /* Bit 15:9 of pkt_blk_size_hi is the LTU length in byte */
  #ifdef FEATURE_IS2000_REL_A //32X

  pkt_blk_size_hi = deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                  pkt_blk_size_hi;

  /* Extended LTU Length HW change (from MSM6050) only accomodate LTU
  ** length in bytes.
  */
  ltu_len_in_bytes = ltu_len >> 3;
  if (ltu_len_in_bytes > DINT_LTU_SIZE_MAX)
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "LTU len (bytes) > max, %d",
      ltu_len_in_bytes);
  }
  if (num_ltu > DINT_NUM_LTU_MAX)
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Num LTU per fr > max, %d",
      num_ltu);
  }

  /* High value is most significant 5 bits of ltu_len_in_bytes */
  ltu_len_in_bytes_hi_value = ((ltu_len_in_bytes >> 2) << 9);

  /* Low value is least significant 2 bits of ltu_len_in_bytes */
  ltu_len_in_bytes_lo_value = (ltu_len_in_bytes & 3) << 6;

  /* With this operation, bits 14 and 15 are always zeros */
  pkt_blk_size_hi = (pkt_blk_size_hi & ~DINT_LTU_SIZE_IN_BYTE_M) |
                    ltu_len_in_bytes_hi_value;
  //MSG_MED_1 ("pkt_blk_size_hi=%d", pkt_blk_size_hi);
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_blk_size_hi );

  #else

  SET_DEMBACK_REG( DINT_PKT_CFG,
                   deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                     pkt_blk_size_hi );

  #endif /* FEATURE_IS2000_REL_A */

  /* Program PKT_PUNCT_LO for Packet E  */
  #ifdef FEATURE_IS2000_REL_A //32X
  pkt_punct_lo = deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                 pkt_punct_lo;
  // bits 6 & 7 is the least significant 2 bits of the ltu length in bytes.
  // bits 5 is unused.
  // bits 3:0 is number of LTU per frame.
  pkt_punct_lo = (
                  (pkt_punct_lo  & ~DINT_LTU_SIZE_IN_BYTE_LO_M  //7:5
                                 & ~DINT_LTU_PER_FRAME_M)     | //3:0
                  ltu_len_in_bytes_lo_value                   |
                  num_ltu
                 );
  //MSG_MED_1 ("pkt_punct_lo=%d", pkt_punct_lo);
  SET_DEMBACK_REG( DINT_PKT_CFG, pkt_punct_lo );

  #else

  SET_DEMBACK_REG( DINT_PKT_CFG,
                   deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                     pkt_punct_lo );
  #endif /* FEATURE_IS2000_REL_A */

  /* Program PKT_PUNCT_HI for Packet E */
  SET_DEMBACK_REG( DINT_PKT_CFG,
                   deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                     pkt_punct_hi );

  /* Program PKT_PL_CRC_LO for Packet E */
  SET_DEMBACK_REG( DINT_PKT_CFG,
                   deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                     pkt_pl_crc_lo );

  /* Program PKT_PL_CRC_HI for Packet E */
  SET_DEMBACK_REG( DINT_PKT_CFG,
                   deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].
                     pkt_pl_crc_hi );

   M1X_MSG( MUX, LEGACY_MED,
     "Deint SCH pkt params (cfg)   0x%08X 0x%08X",
     deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].pkt_intlv_cfg_lo,
     deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].pkt_intlv_cfg_hi);
   M1X_MSG( MUX, LEGACY_MED,
     "Deint SCH pkt params (blk)   0x%08X 0x%08X",
     deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].pkt_blk_size_lo,
     deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].pkt_blk_size_hi);
   M1X_MSG( MUX, LEGACY_MED,
     "Deint SCH pkt params (punct) 0x%08X 0x%08X",
     deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].pkt_punct_lo,
     deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].pkt_punct_hi);
   M1X_MSG( MUX, LEGACY_MED,
     "Deint SCH pkt params (crc)   0x%08X 0x%08X",
     deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].pkt_pl_crc_lo,
     deint_sch_pkt_params[DEINT_SCH_RC_INDEX(sch_rc)][sch_rate].pkt_pl_crc_hi);

} /* deint_load_sch_pkt_params */


/*==========================================================================
FUNCTION DEINT_CREATE_SC_TASK_LIST()

DESCRIPTION
  Create Task1 corresponding to SYNC CHANNEL HYPOTHESIS. Packet A
  contains the configuration  parameters for SYNC CHANNEL HYPOTHESIS.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void deint_create_sc_task_list
(
  void
)
{
  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_FIRST_5MS_SLOT_ADDR );

  /* Create Task1 corresponding to SYNC CHANNEL HYPOTHESIS. Packet A
    contains the configuration  parameters for SYNC CHANNEL HYPOTHESIS,
    so we select Packet A.
    Note First write is always to TASK_LIST_LO and the next write is
    to TASK_LIST_HI */

  SET_DEMBACK_REG( DINT_TASK_LIST, DINT_TASK_LIST_ACTIVE_TASK_V |
                   DINT_TASK_LIST_CH0_V | DINT_TASK_LIST_FRM_HYP_20MS_V |
                   DINT_TASK_LIST_DEC_INTR_EN_V | DINT_TASK_LIST_SYNC_CHAN_EN_V |
                   DINT_TASK_LIST_PACKET_A_V );

  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
  /* We now need to create a zero task to dicate to the task arbitartor
    that there is no other task in this 5ms slot. */

  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );


  /* We also need to create ZERO tasks for the 2,3,4  5ms slot */

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_SECOND_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_THIRD_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_FOURTH_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

}

/*===========================================================================
FUNCTION DEINT_CREATE_PC_TASK_LIST()

DESCRIPTION
  This function creates a task for decoding paging channel message in the
  first 5ms slot.

DEPENDENCIES
  deint_load_pc_pkt_params should have been called.

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
static void deint_create_pc_task_list
(
  void
)
{
  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_FIRST_5MS_SLOT_ADDR );

  /* Create Task1 corresponding to PAGING CHANNEL HYPOTHESIS. Packet A
    contains the configuration  parameters for PAGING CHANNEL HYPOTHESIS,
    Note First write is always to TASK_LIST_LO and the next write is
    to TASK_LIST_HI */

  SET_DEMBACK_REG( DINT_TASK_LIST, DINT_TASK_LIST_ACTIVE_TASK_V |
                   DINT_TASK_LIST_CH0_V | DINT_TASK_LIST_FRM_HYP_20MS_V |
                   DINT_TASK_LIST_DEC_INTR_EN_V | DINT_TASK_LIST_PACKET_A_V );

  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );  /* OB_ADDR = 0x0000 */
  /* We now need to create a zero task to indicate to the task arbitartor
    that there is no other task in this 5ms slot. */

  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );


  /* We also need to create ZERO tasks for the 2,3,4  5ms slot */

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_SECOND_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_THIRD_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_FOURTH_5MS_SLOT_ADDR );
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

}

/*=========================================================================
FUNCTION DEINT_CREATE_FCH_TASK_LIST

DESCRIPTION
  This function creates 4 tasks in the first 5ms slot of task RAM. The 4 tasks
  correspond to FULL RATE, HALF RATE, QUARTER RATE and 8TH RATE Hypothesis.
  Decoder Interrupt is enabled after the last task is completed.

DEPENDENCIES
  deint_load_fch_pkt_params should have been called.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
#ifndef FEATURE_IS2000_REL_A_SVD

static void deint_create_fch_task_list
(
  deint_rc_type fch_rc,
  boolean int_enable
)
{
  /* 1x Advanced RCs do not use the deint task list.
    This is handled by FW */
  if( fch_rc > DEC_RC5 )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
             "Invalid RC for F-FCH deint task list, rc=%d",
             (int) fch_rc );
    return;
  }

  SET_DEMBACK_REG( DINT_TASK_OFFSET, 0x0000 );

  /* Create Task1 corresponding to FULL RATE HYPOTHESIS. Packet A contains
    the configuration parameters for FULL Rate HYPOTHESIS,
    so we select Packet A */
  /* Note First write is always to TASK_LIST_LO and the next
    write is to TASK_LIST_HI */

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   DINT_TASK_LIST_ACTIVE_TASK_V |
                   DINT_TASK_LIST_CH0_V | DINT_TASK_LIST_FRM_HYP_20MS_V |
                   DINT_TASK_LIST_PACKET_A_V );

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   ((dec_fch_ob_params[DEINT_FCH_RC_INDEX(fch_rc)].
                     dec_ob_addr_full_rate) / DEC_BUFF_WORD_ALIGNMENT) );

  /* At this point the task ram address has incremented to 0x02,
    so it now points to the second task of the first 5ms slot.
    Create Task 2 corresponding to the HALF RATE HYPOTHESIS.
    Packet B contains the configuration parameters for HALF RATE
    HYPOTHESIS */

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   DINT_TASK_LIST_ACTIVE_TASK_V | DINT_TASK_LIST_CH0_V |
                   DINT_TASK_LIST_FRM_HYP_20MS_V | DINT_TASK_LIST_PACKET_B_V );

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   ((dec_fch_ob_params[DEINT_FCH_RC_INDEX(fch_rc)].
                     dec_ob_addr_half_rate) / DEC_BUFF_WORD_ALIGNMENT) );

  /* At this point the task ram address has incremented to 0x04,
    so it now points to the third task of the first 5ms slot.
    Create Task 3 corresponding to the QUARTER RATE HYPOTHESIS.
    Packet C contains the configuration parameters for QUARTER
    RATEHYPOTHESIS */
  SET_DEMBACK_REG( DINT_TASK_LIST,
                   DINT_TASK_LIST_ACTIVE_TASK_V | DINT_TASK_LIST_CH0_V |
                   DINT_TASK_LIST_FRM_HYP_20MS_V | DINT_TASK_LIST_PACKET_C_V );

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   ((dec_fch_ob_params[DEINT_FCH_RC_INDEX(fch_rc)].
                     dec_ob_addr_qtr_rate) / DEC_BUFF_WORD_ALIGNMENT) );

  /* At this point the task ram address has incremented to 0x06,
    so it now points to the fourth task of the first 5ms slot.
    Create Task 4 corresponding to the EIGHTH RATE HYPOTHESIS.
    Packet D contains the configuration parameters for EIGHTH
    RATE HYPOTHESIS */
  if(int_enable)
  {
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     DINT_TASK_LIST_ACTIVE_TASK_V | DINT_TASK_LIST_CH0_V |
                     DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_DEC_INTR_EN_V | DINT_TASK_LIST_PACKET_D_V );
  }
  else
  {
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     DINT_TASK_LIST_ACTIVE_TASK_V | DINT_TASK_LIST_CH0_V |
                     DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_PACKET_D_V );
  }

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   ((dec_fch_ob_params[DEINT_FCH_RC_INDEX(fch_rc)].
                     dec_ob_addr_8th_rate) / DEC_BUFF_WORD_ALIGNMENT) );

  /* We now need to create a zero task to dicate to the task arbitartor that
    there is no other task in this 5ms slot */

  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
}

#else /* of !FEATURE_IS2000_REL_A_SVD */

static void deint_create_fch_task_list
(
  deint_rc_type fch_rc,
  boolean       fch_pc
)
{
  word channel_select = DINT_TASK_LIST_CH0_V;
  word channel_select_mask = DINT_TASK_LIST_CH0_M;
    /* Temp value to hold channel selection       */

  uint8 fch_ob_params_index = DEINT_FCH_RC_INDEX(fch_rc);

  if( fch_ob_params_index >= DEC_FCH_NUM_RC )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid RC for F-FCH, rc=%d",
      (int) fch_rc);
    return;
  }

  if ((curr_deint_ch_active != DEINT_FCH_ACTIVE) &&
      (curr_deint_ch_active != DEINT_FCH_DCCH_ACTIVE)
     )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Bad config create_fch_task_list()");
    return;
  }

  /* Select either channel 0 or channel 1 for F-DCCH depending on the Power
  ** Control indicator.  If Power Control indicator is TRUE, set F-DCCH on
  ** channel 0, otherwise, set it on channel 1.
  */
  M1X_MSG( MUX, LEGACY_HIGH,
    "FCH PowerCnt=%d",
    fch_pc);
  if (fch_pc)
  {
    channel_select_mask = DINT_TASK_LIST_CH0_M;
    channel_select = DINT_TASK_LIST_CH0_V;
  }
  else
  {
    channel_select_mask = DINT_TASK_LIST_CH1_M;
    channel_select = DINT_TASK_LIST_CH1_V;
  }

  SET_DEMBACK_REG( DINT_TASK_OFFSET, 0x0000 );

  /* Create Task1 corresponding to FULL RATE HYPOTHESIS. Packet A contains
  ** the configuration parameters for FULL Rate HYPOTHESIS,
  ** so we select Packet A.
  **
  ** Note First write is always to TASK_LIST_LO and the next
  ** write is to TASK_LIST_HI.
  */
  SET_DEMBACK_REG( DINT_TASK_LIST,
                   DINT_TASK_LIST_ACTIVE_TASK_V |
                   channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                   DINT_TASK_LIST_PACKET_A_V );

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   ((dec_fch_ob_params[fch_ob_params_index].
                     dec_ob_addr_full_rate) / DEC_BUFF_WORD_ALIGNMENT) );

  /* At this point the task ram address has incremented to 0x02,
     so it now points to the second task of the first 5ms slot.
     Create Task 2 corresponding to the HALF RATE HYPOTHESIS.
     Packet B contains the configuration parameters for HALF RATE
     HYPOTHESIS */
  SET_DEMBACK_REG( DINT_TASK_LIST,
                   DINT_TASK_LIST_ACTIVE_TASK_V |
                   channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                   DINT_TASK_LIST_PACKET_B_V );

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   ((dec_fch_ob_params[fch_ob_params_index].
                     dec_ob_addr_half_rate) / DEC_BUFF_WORD_ALIGNMENT) );

  /* At this point the task ram address has incremented to 0x04,
     so it now points to the third task of the first 5ms slot.
     Create Task 3 corresponding to the QUARTER RATE HYPOTHESIS.
     Packet C contains the configuration parameters for QUARTER
     RATEHYPOTHESIS */
  SET_DEMBACK_REG( DINT_TASK_LIST,
                   DINT_TASK_LIST_ACTIVE_TASK_V |
                   channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                   DINT_TASK_LIST_PACKET_C_V );
  SET_DEMBACK_REG( DINT_TASK_LIST,
                  ((dec_fch_ob_params[fch_ob_params_index].
                    dec_ob_addr_qtr_rate) / DEC_BUFF_WORD_ALIGNMENT) );

  /* At this point the task ram address has incremented to 0x06,
     so it now points to the fourth task of the first 5ms slot.
     Create Task 4 corresponding to the EIGHTH RATE HYPOTHESIS.
     Packet D contains the configuration parameters for EIGHTH
     RATE HYPOTHESIS */
  if (curr_deint_ch_active == DEINT_FCH_ACTIVE)
  {
    /* If just FCH is active, enable interrupt and create a zero task
    ** (zero task is the task list terminator.)
    ** Important change:  the macro used to set register DINT_TASK_LIST_WH
    ** has been changed from MSM_OUTH to MSM_OUTHM.  This change is necessary
    ** because (only) the interrupt enable field of this register may be
    ** changed later on if DCCH is added.
    */
    SET_DEMBACK_REG( DINT_TASK_LIST,
                    (DINT_TASK_LIST_ACTIVE_TASK_M |
                     channel_select_mask | DINT_TASK_LIST_FRM_HYP_20MS_M |
                     DINT_TASK_LIST_DEC_INTR_EN_M | DINT_TASK_LIST_PACKET_D_M) );
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     ((dec_fch_ob_params[fch_ob_params_index].
                       dec_ob_addr_8th_rate) / DEC_BUFF_WORD_ALIGNMENT) );

    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
  }
  else if (curr_deint_ch_active == DEINT_FCH_DCCH_ACTIVE)
  {
    /* Do not enable interrupt and do not create a zero task.  DCCH
    ** task enables interrupt and uses the next slot.
    */
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     DINT_TASK_LIST_ACTIVE_TASK_V |
                     channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_PACKET_D_V );
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     ((dec_fch_ob_params[fch_ob_params_index].
                       dec_ob_addr_8th_rate) / DEC_BUFF_WORD_ALIGNMENT) );
  }

}

/*===========================================================================
FUNCTION DEINT_LOAD_DCCH_PKT_PARAMS

DESCRIPTION
  This function programs 1 FULL rate packet corresponding to the RC for DCCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 None
===========================================================================*/
static void deint_load_dcch_pkt_params
(
  deint_rc_type dcch_rc
)
{
  uint8 fch_packet_params_index = DEINT_FCH_RC_INDEX(dcch_rc);

  if (curr_deint_ch_active == DEINT_DCCH_ACTIVE)
  {
    /* When loading DCCH packet params in the case where DCCH is the only active
       channel, we must also load FCH packet params since DCCH assumes FCH tasks
       are present and configured properly. This is a HW specific requirement
       since on NikeL HW due to power collapse the packet RAM is not guaranteed
       to be valid. When these parameters are invalid it results in the DEINT
       wedging and no decoder done ISR occurs. */
    (void)deint_load_fch_pkt_params(dcch_rc);
  }

  if( fch_packet_params_index < DEINT_NUM_FCH_PKT_CFG_PARAM_RC )
  {
    /* Select Packet F to program parameters for FULL Rate Hypothesis */
    SET_DEMBACK_REG( DINT_PKT_OFFSET, DINT_PKT_F_V | DINT_PKT_INTLV_CFG_LO_V );

   /**********************PROGRAMMING FULL RATE HYPOTHESIS *****************/

    deint_load_pkt_params (&deint_fch_packet_params[fch_packet_params_index].
              fch_full_rate_param);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid F-DCCH RC for use with F-FCH, rc=%d",
      (int) dcch_rc);
    return;
  }

} /* deint_load_dcch_pkt_params */

/*=========================================================================
FUNCTION DEINT_CREATE_DCCH_TASK_LIST

DESCRIPTION
  This function creates a task to decode DCCH in the 5th entry of the first
  5ms slot of task RAM.  The first 4 entries are reserved for FCH channel
  Full rate, Half rate, Quarter rate and Eighth rate tasks.  If FCH channel
  is not set up, this function creates 4 dummy tasks in place of the real
  FCH tasks.  Decoder interrupts are not enabled for any dummy tasks.
  Decoder interrupt is enabled after the real DCCH task is completed.

DEPENDENCIES
  deint_load_fch_pkt_params should have been called.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
void deint_create_dcch_task_list
(
  deint_rc_type dcch_rc,
  boolean       dcch_pc

)
{
  uint8 fch_ob_params_index = DEINT_FCH_RC_INDEX(dcch_rc);
  word  channel_select      = 0;   /* Temp value to hold channel selection       */

  /* 1x Advanced RCs do not support DCCH */
  if( dcch_rc > DEINT_RC5 )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid RC for F-DCCH, rc=%d",
      (int) dcch_rc);
    return;
  }

  if( fch_ob_params_index >= DEC_FCH_NUM_RC )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid F-DCCH RC for use with F-DCCH, rc=%d",
      (int) dcch_rc);
    return;
  }

  if ((curr_deint_ch_active != DEINT_DCCH_ACTIVE) &&
      (curr_deint_ch_active != DEINT_FCH_DCCH_ACTIVE)
     )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Bad config create_dcch_task_list()");
    return;
  }

  /* Select either channel 0 or channel 1 for F-DCCH depending on the Power
  ** Control indicator.  If Power Control indicator is TRUE, set F-DCCH on
  ** channel 0, otherwise, set it on channel 1.
  */
  M1X_MSG( MUX, LEGACY_HIGH,
    "DCCH PowerCnt=%d",
    dcch_pc);
  if (dcch_pc)
  {
    channel_select = DINT_TASK_LIST_CH0_V;
  }
  else
  {
    channel_select = DINT_TASK_LIST_CH1_V;
  }

  if (curr_deint_ch_active == DEINT_DCCH_ACTIVE)
  {
    /* If just DCCH is active, create 4 dummy tasks with no interrupts.
    ** Continue with real DCCH task with interrupt and create a zero task
    ** (zero task is the task list terminator.)
    */
    SET_DEMBACK_REG( DINT_TASK_OFFSET, 0x0000 );

    /* 4 dummy tasks similar to FCH configuration.
    **
    ** Note First write is always to TASK_LIST_LO and the next
    ** write is to TASK_LIST_HI.
    */
    SET_DEMBACK_REG( DINT_TASK_LIST,                // Full rate
                     DINT_TASK_LIST_ACTIVE_TASK_V |
                     channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_PACKET_A_V );
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     ((dec_fch_ob_params[fch_ob_params_index].
                       dec_ob_addr_full_rate) / DEC_BUFF_WORD_ALIGNMENT) );

    SET_DEMBACK_REG( DINT_TASK_LIST,                // Half rate
                     DINT_TASK_LIST_ACTIVE_TASK_V |
                     channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_PACKET_B_V);
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     ((dec_fch_ob_params[fch_ob_params_index].
                       dec_ob_addr_half_rate) / DEC_BUFF_WORD_ALIGNMENT) );

    SET_DEMBACK_REG( DINT_TASK_LIST,                // Quarter rate
                     DINT_TASK_LIST_ACTIVE_TASK_V |
                     channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_PACKET_C_V);
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     ((dec_fch_ob_params[fch_ob_params_index].
                       dec_ob_addr_qtr_rate) / DEC_BUFF_WORD_ALIGNMENT) );

    SET_DEMBACK_REG( DINT_TASK_LIST,                // Eigth rate
                     DINT_TASK_LIST_ACTIVE_TASK_V |
                     channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_PACKET_D_V);
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     ((dec_fch_ob_params[fch_ob_params_index].
                       dec_ob_addr_8th_rate) / DEC_BUFF_WORD_ALIGNMENT) );
  }
  else if (curr_deint_ch_active == DEINT_FCH_DCCH_ACTIVE)
  {
    /* If both DCCH and FCH are active, make sure to turn off the FCH
    ** interrupt.  The FCH interrupt is in the third slot of the first 5ms bin.
    */
    SET_DEMBACK_REG( DINT_TASK_OFFSET, 0x0006 );
    SET_DEMBACK_REG_M( DINT_TASK_LIST,
                       (DINT_TASK_LIST_DEC_INTR_EN_M),
                       (DINT_TASK_LIST_DEC_INTR_DIS_V) );

    /* Now move the task offset to get ready for DCCH task creation.
    */
    SET_DEMBACK_REG( DINT_TASK_OFFSET, 0x0008 );

  }

  /* Create DCCH Task corresponding to FULL RATE HYPOTHESIS. Packet F contains
  ** the configuration parameters for DCCH FULL Rate HYPOTHESIS,
  ** so we select Packet F.  Note that we unconditionally turn on the DCCH
  ** interrupt here because DCCH is the last task in the task list.
  */
  SET_DEMBACK_REG( DINT_TASK_LIST,
                   DINT_TASK_LIST_ACTIVE_TASK_V |
                   channel_select | DINT_TASK_LIST_FRM_HYP_20MS_V |
                   DINT_TASK_LIST_DEC_INTR_EN_V | DINT_TASK_LIST_PACKET_F_V );

  SET_DEMBACK_REG( DINT_TASK_LIST,
                   ((dec_dcch_ob_params[DEINT_DCCH_RC_INDEX(dcch_rc)].
                     dec_ob_addr_full_rate) / DEC_BUFF_WORD_ALIGNMENT) );

  /* We now need to create a zero task to dicate to the task arbitartor that
  ** there is no other task in this 5ms slot.
  */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   /* Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

} /* end deint_create_dcch_task_list () */
#endif /* !FEATURE_IS2000_REL_A_SVD */

/*=========================================================================
FUNCTION DEINT_CREATE_SCH_TASK_LIST

DESCRIPTION
  This function creates a task to decode SCH in the second 5ms slot of task RAM.
    Decoder Interrupt is enabled after the  task is completed.

DEPENDENCIES
  deint_load_sch_pkt_params should have been called.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
static void deint_create_sch_task_list
(
#ifdef FEATURE_IS2000_REL_A
  dec_mux_pdu_type_type pdu_type,
#endif
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate   /* SCH Rate */
)
{
  byte bit_shift=0;
#ifdef FEATURE_IS2000_REL_A
  if( pdu_type != DEC_MUX_PDU_TYPE_5 )
  {
#endif
#ifdef FEATURE_IS2000_REL_A_SVD
    /* Note that bit_shift is only used in case of 1X.
    **
    ** Do not shift out the MM bit if secondary traffic stream is
    ** supported.  In this case, RLP data can be carried in the primary
    ** or secondary traffic stream.  The MM bit is the only bit which
    ** indicates which stream carries RLP data.
    **
    ** So for rate set 2, also leave in the EIB bit to simplify the driver
    ** code.  Both EIB and MM bits will be discarded by the MUX layer before
    ** data is given to RLP.
    **
    ** This change also affects the decoder read SCH functions (data & status)
    ** and the processing of rx frames in the rxdemux layer.
    */
    bit_shift=0;
#else
    if((sch_rc == DEINT_RC3) || (sch_rc == DEINT_RC4) || (sch_rc == DEINT_RC11))
    {
      bit_shift=1;
    }
    else
    {
      bit_shift=2;
    }
#endif /* FEATURE_IS2000_REL_A_SVD */

#ifdef FEATURE_IS2000_REL_A
  }
#endif
  /* Note First write is always to TASK_LIST_LO and the next
  ** write is to TASK_LIST_HI
  */
  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_SECOND_5MS_SLOT_ADDR );
  if(dec_sch_rate == DEC_SCH_RATE_1X)
  {
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     DINT_TASK_LIST_ACTIVE_TASK_V |
                     DINT_TASK_LIST_CH2_V | DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_DEC_INTR_EN_V | DINT_TASK_LIST_PACKET_E_V |
                     bit_shift );
  }
  else
  {
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     DINT_TASK_LIST_ACTIVE_TASK_V |
                     DINT_TASK_LIST_CH2_V | DINT_TASK_LIST_FRM_HYP_20MS_V |
                     DINT_TASK_LIST_DEC_INTR_EN_V | DINT_TASK_LIST_PACKET_E_V );
  }

  if((sch_rc == DEINT_RC3) || (sch_rc == DEINT_RC4))
  {
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     (DEC_SCH_FULL_OB_ADDR_RC3_RC4 / DEC_BUFF_WORD_ALIGNMENT) );
  }
  else if(sch_rc == DEINT_RC11)
  {
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     (DEC_SCH_FULL_OB_ADDR_RC11 / DEC_BUFF_WORD_ALIGNMENT) );
  }
  else
  {
    SET_DEMBACK_REG( DINT_TASK_LIST,
                     (DEC_SCH_FULL_OB_ADDR_RC5 / DEC_BUFF_WORD_ALIGNMENT) );
  }

  /* We now need to create a zero task to dictate to the task arbitartor that
  ** there is no other task in this 5ms slot
  */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
}

#ifdef FEATURE_IS2000_REL_A_SVD
/*=========================================================================
FUNCTION deint_delete_fch_dcch_task_list

DESCRIPTION
    This function deletes the FCH and DCCH tasks from the deinterleaver task
    RAM.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void deint_delete_fch_dcch_task_list
(
  void
)
{
  /* Remove any FCH or DCCH task pending in deint_sequencing_cmd_q.
  */

  /* Get pointer to first item in Q.
  */
  deint_sequencing_cmd_type  *cur_ptr = q_check( &deint_sequencing_cmd_q );
  deint_sequencing_cmd_type  *next_ptr;   /* Points to next link in Q */

  for ( ; cur_ptr != NULL; cur_ptr = next_ptr )
  {
    next_ptr = q_next( &deint_sequencing_cmd_q, &cur_ptr->hdr.link );
    if (( cur_ptr->hdr.command == DEINT_DCCH_READ_PARAMS ) ||
        ( cur_ptr->hdr.command == DEINT_FCH_READ_PARAMS )
       )
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Removing remnant FCH or DCCH cmd");

      /* Found pending FCH or DCCH task, remove from sequencing queue.
      */
      q_delete( &deint_sequencing_cmd_q, &cur_ptr->hdr.link );
      q_put(cur_ptr->hdr.done_q_ptr, &cur_ptr->hdr.link);
    }
  }

  #if defined( FEATURE_MODEM_1X_SW_DEMBACK ) && !defined( FEATURE_MODEM_1X_ATLAS_REVISIT )
  /* Dont touch HW if SW DEMBACK is in use */
  if( !muxmdsp_get_is_sw_demback_active() )
  #endif /* FEATURE_MODEM_1X_SW_DEMBACK && !FEATURE_MODEM_1X_ATLAS_REVISIT */
  {
    /* Select first 5ms bin */
    SET_DEMBACK_REG( DINT_TASK_OFFSET, 0x0000 );

    /* We now need to create a zero task to indicate to the task arbitartor that
    ** there is no other task in this 5ms bin.
    */
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
  }

  /* Indicate that there's current no FCH or DCCH task active.
  */
  curr_deint_ch_active = DEINT_CH_INACTIVE;

} /* end deint_delete_fch_dcch_task_list */

#endif /* FEATURE_IS2000_REL_A_SVD */

/*=========================================================================
FUNCTION DEINT_DELETE_SUPPLEMENTAL_TASK_LIST

DESCRIPTION
    This function deletes the sch task from the deinterleaver task ram.
    The SCH0 task is the first task of the second 5ms slot.

DEPENDENCIES
  deint_load_sch_pkt_params should have been called.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
void deint_delete_supplemental_task_list
(
  void
)
{

  // Delete any SCH task pending in deint_sequencing_cmd_q

  // Get pointer to first item in Q
  deint_sequencing_cmd_type  *cur_ptr = q_check( &deint_sequencing_cmd_q );
  deint_sequencing_cmd_type  *next_ptr;   /* Points to next link in Q */

  for ( ; cur_ptr != NULL; cur_ptr = next_ptr )
  {
    next_ptr = q_next( &deint_sequencing_cmd_q, &cur_ptr->hdr.link );
    if ( cur_ptr->hdr.command == DEINT_SCH_READ_PARAMS )
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Removing remnant SCH cmd");
      // Found pending SCH task, remove from sequencing queue
      q_delete( &deint_sequencing_cmd_q, &cur_ptr->hdr.link );
      q_put(cur_ptr->hdr.done_q_ptr, &cur_ptr->hdr.link);
    }
  }

  #if defined( FEATURE_MODEM_1X_SW_DEMBACK ) && !defined( FEATURE_MODEM_1X_ATLAS_REVISIT )
  /* Dont inform FW if SW DEMBACK is in use */
  if( !muxmdsp_get_is_sw_demback_active() )
  #endif /* FEATURE_MODEM_1X_SW_DEMBACK && !FEATURE_MODEM_1X_ATLAS_REVISIT */
  {
    /* Delete Convolutional task entry in the 2nd bin */
    SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_SECOND_5MS_SLOT_ADDR );
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

    #ifdef FEATURE_IS2000_REL_A
    /* Delete Turbo task entry in the 3rd bin */
    SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_THIRD_5MS_SLOT_ADDR );
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
    SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );

    /* Reset SCH setup struct */
    deint_atmr_isr2_init();
    #endif /* FEATURE_IS2000_REL_A */

    // Make sure deint CH2 stops pumping into SVD/TDEC
    SET_DEMBACK_REG_F( DINT_OTD_CFG, CH2_CODE_TYPE, 0 );
    SET_DINT_CH_CFG( 2, DINT_CH2_CFG__NEXT_20M_BND_MASK, 0 );

    /* Ask FW to release the TDEC bridge (if enabled) */
    muxmdsp_data_brdg_enable(FALSE, FALSE);
  }

  M1X_MSG( MUX, LEGACY_HIGH,
    "Deint CH2 (F-SCH) stopped");

}

/*=========================================================================
FUNCTION DEINT_CREATE_DUMMY_SCH_TASK_LIST

DESCRIPTION
    This function creates a dummy task in the second 5ms slot.
    This function should be called only when turbo sch is enabled.

DEPENDENCIES


RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
static void deint_create_dummy_sch_task_list( void )
{
  /* Note First write is always to TASK_LIST_LO and the next
     write is to TASK_LIST_HI */
  /* FW currently supports only FIRST and SECOND 5ms slots */
  SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_SECOND_5MS_SLOT_ADDR );

  /* We now need to create a dummy task that will generate a interrupt.
   This interrupt will be used to as th SCH interrupt when turbo decoder
   is operational */


  SET_DEMBACK_REG( DINT_TASK_LIST,
                   DINT_TASK_LIST_ACTIVE_TASK_V | DINT_TASK_LIST_CH0_V |
                   DINT_TASK_LIST_FRM_HYP_20MS_V |
                   DINT_TASK_LIST_DEC_INTR_EN_V | DINT_TASK_LIST_PACKET_A_V );

  /* Worst case offset requirement. This will push the SCH dummy decode far
    enough outside the FCH/DCCH decode buffer space to avoid any overwrites. */
  SET_DEMBACK_REG( DINT_TASK_LIST,
                   (DEC_SCH_FULL_OB_ADDR_RC5 / DEC_BUFF_WORD_ALIGNMENT) );

  /* We now need to create a zero task to dicate to the task arbitartor that
    there is no other task in this 5ms slot */

  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );   // Create a ZERO Task */
  SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
}

/*=========================================================================
FUNCTION DEINT_PROCESS_SEQUENCING_CMD

DESCRIPTION
  This function checks the deint cmd_q to see if there are any pending
  commands to program the read parameters corresponding to FCH/SCH/SCCH.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
void deint_process_sequencing_cmd
(
  void
)
{
  deint_sequencing_cmd_type *cmd_ptr;

  DRV_ENTER_ATOMIC_SECTION();

  while((cmd_ptr = (deint_sequencing_cmd_type *)
           q_get(&deint_sequencing_cmd_q)) != NULL)
  {
    switch(cmd_ptr->hdr.command)
    {
      case DEINT_FCH_READ_PARAMS:
      {
        M1X_MSG( MUX, LEGACY_HIGH,
          "New FCH Deint tsk, RC= %d",
          cmd_ptr->fch_cmd.fch_rc);
       (void) deint_load_fch_pkt_params(cmd_ptr->fch_cmd.fch_rc);
       deint_create_fch_task_list(cmd_ptr->fch_cmd.fch_rc,
                                  #ifdef FEATURE_IS2000_REL_A_SVD
                                  cmd_ptr->fch_cmd.fch_pc
                                  #else
                                  TRUE
                                  #endif /* FEATURE_IS2000_REL_A_SVD */
                                 );
       break;
      }

      #ifdef FEATURE_IS2000_REL_A_SVD
      case DEINT_DCCH_READ_PARAMS:
      {
        M1X_MSG ( MUX, LEGACY_HIGH,
          "New DCCH Deint tsk, RC= %d",
          cmd_ptr->dcch_cmd.dcch_rc);
       deint_load_dcch_pkt_params (cmd_ptr->dcch_cmd.dcch_rc);
       deint_create_dcch_task_list (cmd_ptr->dcch_cmd.dcch_rc,
                                    cmd_ptr->dcch_cmd.dcch_pc
                                   );
       break;
      }
      #endif /* FEATURE_IS2000_REL_A_SVD */

      case DEINT_SCH_READ_PARAMS:
      {
        if(cmd_ptr->sch_cmd.dec_coding == DEC_CONVOLUTIONAL)
        {
          /* Sequencing is currently done for Convolutionally encoded frames */
          deint_load_sch_pkt_params(
                                    #ifdef FEATURE_IS2000_REL_A
                                    cmd_ptr->sch_cmd.num_ltu,
                                    cmd_ptr->sch_cmd.ltu_len,
                                    #endif /* FEATURE_IS2000_REL_A */
                                    cmd_ptr->sch_cmd.sch_rc,
                                    cmd_ptr->sch_cmd.dec_sch_rate);

          #ifdef FEATURE_IS2000_REL_A
          /* Delay changing SCH booleans to the 4th bin of this frame. */
          sch_setup.this_frame = TRUE;
          sch_setup.turbo_dec_int = FALSE;
          sch_setup.sup_dec_int = TRUE;
          #else
          rxc_sup_dec_int_enable=TRUE;
          rxc_turbo_dec_enabled=FALSE;
          #endif /* FEATURE_IS2000_REL_A */
          #ifdef FEATURE_IS2000_REL_A
          deint_create_sch_task_list(cmd_ptr->sch_cmd.pdu_type,
                                     cmd_ptr->sch_cmd.sch_rc,
                                     cmd_ptr->sch_cmd.dec_sch_rate);
          #else
          deint_create_sch_task_list(cmd_ptr->sch_cmd.sch_rc,
                                     cmd_ptr->sch_cmd.dec_sch_rate);
          #endif
        }
        else
        {
          deint_load_td_dec_params(cmd_ptr->sch_cmd.sch_rc,
                                   cmd_ptr->sch_cmd.dec_sch_rate);
          #ifdef FEATURE_IS2000_REL_A
          /* Delay changing SCH booleans to the 4th bin of this frame.
          ** Delay creating dummy SCH task to the 4th bin of this frame.
          */
          sch_setup.this_frame = TRUE;
          sch_setup.turbo_dec_int = TRUE;
          sch_setup.sup_dec_int = TRUE;
          #else
          /* TURBO CODE */
          deint_create_dummy_sch_task_list();
          rxc_sup_dec_int_enable=TRUE;
          rxc_turbo_dec_enabled=TRUE;
          #endif /* FEATURE_IS2000_REL_A */
          }
          break;
        }
        default:
        {
         M1X_MSG( MUX, LEGACY_ERROR,
           "Bad DEINT SEQ CMD");
          break;
        }
      }  /* switch */
    q_put(cmd_ptr->hdr.done_q_ptr, &cmd_ptr->hdr.link);
  }
  DRV_LEAVE_ATOMIC_SECTION();
}

#ifdef FEATURE_DEINT_RAM_LOGGING
#error code not present
#endif /* FEATURE_DEINT_RAM_LOGGING */

#ifdef FEATURE_IS2000_REL_A
/*=========================================================================
FUNCTION deint_atmr_isr2_init

DESCRIPTION
  This function initializes variables used in deint_atmr_isr2().

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void deint_atmr_isr2_init( void )
{
  (void) memset (&sch_setup, 0, sizeof (sch_setup));

} /* deint_atmr_isr2_init */

/*=========================================================================
FUNCTION deint_atmr_isr2

DESCRIPTION
  This function is invoked once a frame, at the 4th bin.  This function
  support changes for 6100 release A Turbo SCH.  These changes are the result
  of moving the Turbo SCH dummy task to the 3rd bin to give the Decoder more
  time to decode 32X turbo data rate.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void deint_atmr_isr2( void )
{
  if (sch_setup.this_frame)
  {
    /* Update SCH booleans from save values.  Since this ISR happens on the
    ** 4th bin of a frame, the Traffic ISR has already happened for this frame
    ** thus the changes done here won't affect SCH data read operation for
    ** this frame.
    */
    rxc_turbo_dec_enabled = sch_setup.turbo_dec_int;
    rxc_sup_dec_int_enable = sch_setup.sup_dec_int;

    if (rxc_turbo_dec_enabled == TRUE)
    {
      /* Here for Turbo SCH */
      /* Create Turbo dummy task entry */
      deint_create_dummy_sch_task_list();
    }
    else
    {
      /* Here for Convolutional SCH */
      /* Delete Turbo task entry in the 3rd bin, if any */
      SET_DEMBACK_REG( DINT_TASK_OFFSET, DINT_THIRD_5MS_SLOT_ADDR );
      SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
      SET_DEMBACK_REG( DINT_TASK_LIST, 0x0000 );
    }
  }

  /* Reset SCH setup struct every frame */
  deint_atmr_isr2_init();

} /* deint_atmr_isr2 */
#endif /* FEATURE_IS2000_REL_A */
