/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         M O B I L E   D E C O D E R   ( S V D )    S E R V I C E S

GENERAL DESCRIPTION
  Library of routines to initialize, and operate on, the Qualcomm CDMA
  Serial Viterbi Decoder Chip.

EXTERNALIZED FUNCTIONS
 dec_init
   Initialize the circuit to Idle. Load the polynomials for  1/2, 1/3,
   1/4 code rate.
 dec_version
   Return the hardware version number for this circuit.
 dec_read_data
   Read data from the decoder output buffer.
 dec_sc_init
   Initialize the circuit for decoding the Sync Channel.
 dec_pc_init
   Initialize the circuit for decoding the Paging Channel.
 dec_tc_init
   Initialize the circuit for decoding the Traffic Channel.
 dec_read_sc_status
   Read the frame status and symbol error rates of sync channel  from the
   Decoder.
 dec_read_pc_status
   Read the frame status and symbol error rates of Paging channel  from the
   Decoder.
 dec_read_fch_status
   Read the frame status and symbol error rates of Fundamental channel  from the
   Decoder Output Buffer.
 dec_read_sch_status
   Read the frame status and symbol error rates of Suplemental Channel
  (SCH, IS95C) from the Decoder Output Buffer.
 dec_read_sc_data
   Read data from the Decoder as Sync Channel data.
 dec_read_pc_data
   Read data from the Decoder as Paging Channel data.
 dec_read_fch_data
   Read data from the Decoder as Fundamental Traffic Channel data.
 dec_read_sch_data
   Read data from the Decoder as Supplemental Traffic Channel data
  (SCH, IS95C).
 dec_clk_select
   Select between TCXO or Chipx8 for decoder clock.

  Copyright (c) 1999 - 2020 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/drivers/src/dec5000.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
11/20/17   ab      1x2GTA : 0x18CD log packet changes.
05/12/16   srk     Use locally stored/passed in RC instead of global variable
                   to avoid race condition between RX Task and TC ISR.
11/06/15   srk     Add F3 prints for SER, energy and other decoder metrics.
06/12/15   pk      Changes to enable HW demback for CSFB traffic
02/12/15   jh      Changes for SW Demback
                   Support muxmdsp_set_demback_mode() call after demback clks
04/17/14   cjb     Clean-up related to using 1x MSG macros for printing F3s
02/14/14   dkb     Add demback null ptr checks.
12/07/13   cjb     Centralized shadow register writes
12/07/13   cjb     Changes to support DEMBACK_1
11/21/13   vks     Use muxmdsp api to obtain tdec decob address
11/13/14   cjb     Added common function to get HWIO address to read decode o/p
11/09/13   bb      Changes to set correct MEMPOOL address for
                   SCH turbo
11/08/13   eye     Making HWIO pointers volatile to shut compiler optimization.
10/15/13   cjb     Fixed F3s that printout RAW decode status and data
10/10/13   cjb     DEMBACK0 vs DEMBACK1: Use DEMBACK ID from FW shared memory
10/03/13   cjb     Hard-coding DEMBACK0 SVD for bringup and related cleanup
09/16/13   cjb     Dime revisit cleanup
08/30/13   sst     Remove 1xA high voice request
08/22/13   cjb     Removed FEATURE_MODEM_1X_DIME_REVISIT
04/02/13   pap     Feature cleanup
02/15/13   srk     Replace mcs hwio with m1x hwio
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
09/12/12   jtm     Add a check to detect bad status words.
09/10/12   jtm     Use RC11 specific SVD buffer offsets and frame lengths.
08/16/12   jtm     Added SCH TDEC and CONV support for DIME.
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
05/31/12   srk     Removed unnecessary F3s for optimized logging.
04/16/12   jtm     Added handling for DEC_CLK_VOICE_TRAFFIC_1XA clock voting.
02/24/12   srk     Mainlined FEATURE_IS2000_P2
02/09/12   trc     Init all states of SVD in dec_pc_init()
10/25/11   trc     Work around unexpected demback reset in SVLTE
09/28/11   srk     Mainlined Always On features and removed Always Off Features
09/02/11   srk     Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
06/16/11   trc     Convert to dec_set_mode() from dec_1x_set_svd_turbo_mode()
05/18/11   vks     Replace feature MSMHW_TURBO_CODING with
                   FEATURE_MODEM_1X_SUPPORTS_TURBO_CODING
05/17/11   trc     Add dec_1x_set_svd_turbo_mode() API
04/26/11   adw     Changes to handle additional deint status word on NikeL.
04/20/11   adw     Cleanup NikeL "revisit" featurization.
03/30/11   adw     Register setting updates for NikeL.
01/21/11   adw     Added initial M1X_RM support.
01/19/11   vks     More register featurization for nikel.
12/15/10   vks     Temporarily featurize register accesses for Nikel.
12/14/10   jtm     Mainlined FEATURE_SRCH_CLKREGIM_RM and cleaned up unneeded
                   legacy clock code. Created decoder clk resource function to
                   handle enabling/disabling appropriate clocks.
12/03/10   jtm     Removed MDR/SCCH support.
10/30/10   jtm     Modem 1x SU API changes.
10/22/10   jtm     Enable TD_CLK prior to writing to TD_RESET. Disable the
                   clock if no longer needed.
08/10/10   vks     Clean up Genesis revisit feature.
08/04/10   vks     Remove FEATURE_MODEM_1X_GENESIS_1_0 support. Replace
                   FEATURE_MODEM_1X_GENESIS_1_1 with FEATURE_MODEM_1X_GENESIS.
07/20/10   jtm/adw Replaced "is on" clock querries with clkrgm_rm equivalents
                   in VD and TD clk enable functions to prevent resets which
                   can otherwise occur if clk status regs are unclocked.
07/07/10   jtm     Change rxc_dcch_status to use the same type as FCH.
07/06/10   jtm     Added default case to switch statements in
                   dec_dtx_thresh_init().
04/07/10   vks     Clean up Genesis 1.1 register changes
03/25/10   vks     Featurize for register changes between Genesis 1.0 and 1.1
02/04/10   jtm     Removed FEATURE_PLT.
01/20/10   mca     Tweaked some stuff for the Genesis modem
10/21/09   jtm     Removed redundant typecasting.
09/24/09   jtm     Klocwork fixes.
08/24/09   mca     Fixed Q6 link error
08/05/09   jtm     Lint fixes.
07/06/09   jtm     Mainlined FEATURE_IS2000_1X_ADV for tables sized using the
                   number of available RCs.
06/06/09   vlc     Added support for 1x Advanced (RC11.)
04/17/09   jtm     Added includes to fix compiler warnings.
04/01/09   jtm     Eliminated implicit includes.
03/30/09   jtm     Lint fixes.
03/26/09   jtm     Mainlined T_MSM7600. Replaced T_POSEIDON2 with
                   FEATURE_MODEM_1X_POSEIDON. Replaced T_MSM7800 with
                   FEATURE_MODEM_1X_ARTEMIS || FEATURE_MODEM_1X_POSEIDON.
                   Removed MSMHW_MANAGED_DEM_MAC_CLK.
03/26/09   jtm     Added include for customer.h and modem_1x_defs.h
03/19/09   jtm     Updated included headers.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to Drivers.
03/06/09   jtm     Cleaned up compiler warnings
02/03/09   jtm     Added support for T_POSEIDON2
01/28/09   vlc     Moved Turbo decoder setting under MSMHW_TURBO_CODING.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
11/12/08   vlc     Added changes specific to MSM7800 turbo decoder.
10/01/08   vlc/jtm Changed dec_ccch_init() to avoid stomping on HDR traffic in
                   simultaneous 1X/HDR mode. Removed T_QUASAR
07/16/08   jtm     Removed CCCH_DEBUG. Reduced debug string sizes to reduce ROM
                   for ULC effort.
07/01/08   vlc     Merged code changes from old tip archive.
04/10/08   trc     Switch clock mgmt strategy via FEATURE_SRCH_CLKREGIM_RM
03/31/08   mca     Changed FEATURE_CLKREGIM_RM to FEATURE_SRCH_CLKREGIM_RM
03/14/08   trc     Only reset VD & TD after turning ON clocks, not prior to
                   turning OFF clocks
03/05/08   vlc     User clock resource manager to manage RX clocks
                   (FEATURE_CLKREGIM_RM.)
09/11/07   vlc     Added shift_bits_size in function dec_read_sch_data().
08/09/07   vlc     Modified the rel 0 branch of functions dec_read_sch_data()
                   and dec_read_sch_status() to fix 1x F-SCH high CRC failure.
12/21/06   trc     MSM7600 support, part II
10/17/06   trc     Add MSM7600 support
09/19/06   vlc     Fixed lint errors.
04/19/06   trc     Correct several enum types in dec_dtx_thresh_init()
04/19/06   vlc     Fixed compiler warning.
                   In function dec_read_mux_pdu5(), adjust DSM item size
                   accordingly when pointer to data buffer is moved.
04/12/06   bn      Add code in dec_tc_init to re-initialize dec 1x in case HDR messed it up
01/03/05   rkc     Move FCCCH channel data from Long Buffer to an offset in the
                   Short Buffer RAM in Decoder Output Buffer.  Offset since BCCH
                   also uses short buffer; move to short buffer since HDR uses
                   long buffer affecting simultaneous 1x/HDR.
08/01/05   grl      Turned off only viterbi decoder clk gating during PC.
04/21/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
02/28/05   bn/vlc  Modified function dec_read_mux_pdu5(), changed words_read
                   to words_need in memcpy statement.
02/28/05   bn/vlc  Merged code changes to fix MUX PDU5 decoder read function to
                   6700 archive.  Same code changes for earlier targets were done
                   in Mux layer.  The code was moved to Dec layer for 6700.
02/17/05   bn      Please featurize the use of clk_regime_sel_clk_src() for the decoder clock
                   to not be called for T_MSM6700
01/26/05   vlc     Removed obsolete ds.h inclusion.
11/30/04   vlc     In function dec_read_mux_pdu5_sch_data(), start reading SCH
                   data from location 0 of decoder output buffer (long buffer.)
11/17/04   jyw     Removed the 0x1fff covering for the 1st scch status word
08/23/04   vlc     Switched source and dest ptrs for memcpy operations in
                   functions dec_read_mux_pdu5() and dec_read_sch_data().
07/30/04   jyw     Featurized the debuging msg for ccch
07/27/04   jyw     Fixed the status distance in dec_read_ccch_status
06/24/04   sr      Fixed TD CRC mask in dec_read_sch_status()
05/06/04   sr      Moved out DV driver code to separate file
09/10/03   dnn     Changed FEATURE_TC_FRAMES_INTEGRATION to
                   FEATURE_MSM6500_TC_FRAMES_INTEGRATION.
09/10/03   dnn     Added FEATURE_TC_FRAMES_INTEGRATION and its functionalities.
09/03/03   dna     Ported for MSM6500
08/08/03   vlc     Added changes to move output buffer address of DCCH task to
                   decouple FCH/DCCH rate sets.
10/22/02   vlc     In function dec_read_sch_status(), added SVD code under
                   feature MSMHW_EXTENDED_ENERGY_MATRIX.
07/23/02   dlb2    Add EXTENDED_ENERGY_MATRIX support to CDMA2000 Rel. A code.
07/19/02   dlb2    Turn off DEM_MAC clock to save power except when in traffic.
06/27/02   dlb2    Add workaround for MSM6050 DEC1 problem.  Use new macros
                   defined in msm_drv.h to turn on/off dynamic clock gating.
06/17/02    rm     Support IS95B SCH for MSMHW_EXTENDED_ENERGY_MATRIX.
06/07/02   dlb2    Always use mask macros to write to VD_MODE_WB.
05/30/02   vlc     VP2 changes are now featurized under FEATURE_IS2000_REL_A_SVD
                   and FEATURE_IS2000_P2 per request of development team.
05/21/02   sr      Changed MSM_OUT to MSM_OUTM while programming VD_MODE reg
                   in dec_tc_init()
05/21/02   vlc     Removed rate from parameter list of dec_read_dcch_data ().
                   Removed SER scaling code from dec_read_dcch_status ().
05/13/02   vlc     Added support for simultaneous DCCH & FCH channel set up
                   (feature FEATURE_IS2000_REL_A_VP2.)
03/27/02   ems     Featurized dec_read_scch_data for MSMHW_EXTENDED_ENERGY_MATRIX
03/15/02   vlc     No longer adjust for bit shift in decoder read sch
                   data/status functions (for FEATURE_IS2000_REL_A_SVD.)
02/25/02   sr      Merged in Release A common channels from the branch
02/07/02   dlb2    Added Decoder Dynamic Clock Gating feature for MSM6050.
01/23/02   dlb2    Added extended energy matrix feature for MSM6050.
11/16/01   vlc     Added function dec_clk_select ().
09/12/01   bgc     Added control variables for dec_dtx_thresh_init().
08/15/01   sr      Fixed REL_A featurization
08/10/01   lh      Added 32X support.
08/07/01   hrk     Added DTX thresholds for RC5 turbo codes.
08/07/01   hrk     Removed parameter "code_type" from dec_dtx_thresh_init()
                   declaration.
06/01/01   hrk,snn Added support for DTX detection on RS2 SCH
05/25/01   sr      Changed OB address offset for RC5 1X in
                   dec_read_sch_data() from 2 words to 1 word
02/20/01   sr      Added support for 32X
02/06/01   sr      Changed OB address offset for RC5 1X in
                   dec_read_sch_status() from 2 words to 1 word
01/10/01   tc      Defined FEATURE_PLT for dec_read_sch_data().
12/04/00   tc      Added decoder initailization delay.
10/24/00   rmd     Mergued with MSM5000 code.
09/13/00   bgc     Added DTX threshholds for RC 4 and made turbo threshholds
                   the same as convolutional in dec_dtx_thresh_init().
09/06/00   djd/et  Added ifdef MSMHW_TURBO_CODING for turbo coding software.
09/06/00   rm      Added New function dec_read_sch_data for FTM.
08/25/00   rc      Added support for handling NULL returns form
                   dsm_new_buffer().
08/11/00   rmd     replaced DEC_OB_DATA_WH with DEC_OB_DATA_RH, Hld shows
                   this register to be write only but it is a read only register
07/12/00   rm      Added support for 5ms frames.
06/08/00   sr      Added #ifndef MSM5000_IRAM_FWD around:
                   dec_read_fch_data
                   dec_read_fch_status
                   dec_read_sch_data
                   dec_read_sch_status
05/22/00   bgc     Changed SCH 1X threshold to 225.
05/18/00   et      added function dec_dtx_thresh_init
05/11/00   rm      Added fixes for 1X SCH.
03/23/00   bgc     Scaled SER for compatibility to MSM3000 and changed
                   energies to signed values.
01/28/00   rm      Added in support for 1X SCH.
01/26/00   rm      Modified the dec_read_sch_data fumction to read
                   each mux pdu into the buffer pointed by the dsm_item.
11/09/99   rm      Added in support to read Turbo decoder stats.
10/21/99   rm      Added in support for SCCH
09/24/99   rm      Added in support for SCH (RC3 & RC4).
08/11/99   rm      Misc. Changes after first phase of PLT testing.
07/31/99   rm      Created file. Interfaces to support Serial Viterbi Decoder

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

//drivers
#include "m1x_rm.h"
#include "m1x_hwio_mpss.h"
#include "msm_drv.h"
#include "msm_drv_i.h"
#include "dec5000.h"
#include "dec5000_v.h"
#include "m1x_stmr_i.h"

//mux
#include "muxmdsp_i.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "rxctraffic.h"
#include "txctraffic_i.h"

//cp
#include "mccsch_i.h"

//other
#include "misc.h"
#include "err.h"
#include "m1x_diag.h"
#include "dsm.h"
#include "bit.h"
#include "stringl.h" /* for memscpy */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

LOCAL word dec_pc_size;   /* Paging Channel Data rate size */

/* DEC channels for decode output HWIO address */
typedef enum
{
  DEC_SYNC_CH,
  DEC_PCH_CH,
  DEC_FCH_CH,
  DEC_DCCH_CH,
  DEC_SCH_CONV_CH,
  DEC_SCH_TURBO_CH

} dec_channel_type;

/*=============================================================================

The following table stores the offset address  in the Decoder Output Buffer
and decoded bits rate for FULL, HALF, QTR, 8TH rate hypothesis for FCH (RC1-RC5)

=============================================================================*/


const dec_fch_ob_params_type dec_fch_ob_params[DEC_FCH_NUM_RC] = {
{
 DEC_FCH_FULL_OB_ADDR_RC1, DEC_FCH_HALF_OB_ADDR_RC1, DEC_FCH_QTR_OB_ADDR_RC1,
 DEC_FCH_8TH_OB_ADDR_RC1,
 DEC_FCH_SIZ_FULL_RC1, DEC_FCH_SIZ_HALF_RC1,
 DEC_FCH_SIZ_QTR_RC1, DEC_FCH_SIZ_8TH_RC1
},
{
 DEC_FCH_FULL_OB_ADDR_RC2_RC5, DEC_FCH_HALF_OB_ADDR_RC2_RC5, DEC_FCH_QTR_OB_ADDR_RC2_RC5,
 DEC_FCH_8TH_OB_ADDR_RC2_RC5,
 DEC_FCH_SIZ_FULL_RC2_RC5, DEC_FCH_SIZ_HALF_RC2_RC5,
 DEC_FCH_SIZ_QTR_RC2_RC5, DEC_FCH_SIZ_8TH_RC2_RC5
},
{
 DEC_FCH_FULL_OB_ADDR_RC3_RC4, DEC_FCH_HALF_OB_ADDR_RC3_RC4, DEC_FCH_QTR_OB_ADDR_RC3_RC4,
 DEC_FCH_8TH_OB_ADDR_RC3_RC4,
 DEC_FCH_SIZ_FULL_RC3_RC4, DEC_FCH_SIZ_HALF_RC3_RC4,
 DEC_FCH_SIZ_QTR_RC3_RC4, DEC_FCH_SIZ_8TH_RC3_RC4
},
{
 DEC_FCH_FULL_OB_ADDR_RC3_RC4, DEC_FCH_HALF_OB_ADDR_RC3_RC4, DEC_FCH_QTR_OB_ADDR_RC3_RC4,
 DEC_FCH_8TH_OB_ADDR_RC3_RC4,
 DEC_FCH_SIZ_FULL_RC3_RC4, DEC_FCH_SIZ_HALF_RC3_RC4,
 DEC_FCH_SIZ_QTR_RC3_RC4, DEC_FCH_SIZ_8TH_RC3_RC4
},
{
 DEC_FCH_FULL_OB_ADDR_RC2_RC5, DEC_FCH_HALF_OB_ADDR_RC2_RC5, DEC_FCH_QTR_OB_ADDR_RC2_RC5,
 DEC_FCH_8TH_OB_ADDR_RC2_RC5,
 DEC_FCH_SIZ_FULL_RC2_RC5, DEC_FCH_SIZ_HALF_RC2_RC5,
 DEC_FCH_SIZ_QTR_RC2_RC5, DEC_FCH_SIZ_8TH_RC2_RC5
},
{
 DEC_FCH_FULL_OB_ADDR_RC11, DEC_FCH_HALF_OB_ADDR_RC11, DEC_FCH_QTR_OB_ADDR_RC11,
 DEC_FCH_8TH_OB_ADDR_RC11,
 DEC_FCH_SIZ_FULL_RC11, DEC_FCH_SIZ_HALF_RC11,
 DEC_FCH_SIZ_QTR_RC11, DEC_FCH_SIZ_8TH_RC11
}
};

#ifdef FEATURE_IS2000_REL_A_SVD
const dec_dcch_ob_params_type dec_dcch_ob_params[DEC_DCCH_NUM_RC] =
{
{
 DEC_DCCH_FULL_OB_ADDR_RC3_RC4,
 DEC_DCCH_SIZ_FULL_RC3_RC4
},
{
 DEC_DCCH_FULL_OB_ADDR_RC3_RC4,
 DEC_DCCH_SIZ_FULL_RC3_RC4
},
{
 DEC_DCCH_FULL_OB_ADDR_RC5,
 DEC_DCCH_SIZ_FULL_RC5
},
};
#endif /* FEATURE_IS2000_REL_A_SVD */

const dec_sch_ob_params_type dec_sch_ob_params[NUM_SCH_RC][NUM_SCH_RATES] =  {


 /********SCH RC3 SIZE for 2X, 4X, 8X, 16X, 32X *********/
  {
    DEC_SCH_SIZ_1X_RS1,
    DEC_SCH_SIZ_2X_RS1,
    DEC_SCH_SIZ_4X_RS1,
    DEC_SCH_SIZ_8X_RS1,
    DEC_SCH_SIZ_16X_RS1,
    0
  },

 /********SCH RC4 SIZE for 2X, 4X, 8X, 16X, 32X *********/
  {
    DEC_SCH_SIZ_1X_RS1,
    DEC_SCH_SIZ_2X_RS1,
    DEC_SCH_SIZ_4X_RS1,
    DEC_SCH_SIZ_8X_RS1,
    DEC_SCH_SIZ_16X_RS1,
    DEC_SCH_SIZ_32X_RS1
  },
 /********SCH RC5 SIZE for 2X, 4X, 8X, 16X, 32X *********/
  {
    DEC_SCH_SIZ_1X_RS2,
    DEC_SCH_SIZ_2X_RS2,
    DEC_SCH_SIZ_4X_RS2,
    DEC_SCH_SIZ_8X_RS2,
    DEC_SCH_SIZ_16X_RS2,
    0
  },

 /********SCH RC11 SIZE for 2X, 4X, 8X, 16X, 32X *********/
  {
    DEC_SCH_SIZ_1X_RS1,
    DEC_SCH_SIZ_2X_RS1,
    DEC_SCH_SIZ_4X_RS1,
    DEC_SCH_SIZ_8X_RS1,
    DEC_SCH_SIZ_16X_RS1,
    DEC_SCH_SIZ_32X_RS1
  }

};

static uint32 dec_temp_sdu_data_array[DEC_SCH_SIZ_32X_RS1 / 4];
static uint16  *dec_temp_sdu_data_ptr;

/* Keep track of traffic vs non-traffic for packet vs continuous SVD mode */
static boolean dec_is_traffic_mode = 0;

/* Remember the DEMBACK that's currently in use  */
dec_demback_type dec_demback_id = DEC_DEMBACK_INVALID;

/*==========================================================================

FUNCTION DEC_SET_DEMBACK_ID

DESCRIPTION
 Set the current ID for the DEMBACK that's currently in use.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

============================================================================*/
void dec_set_demback_id
(
  uint32 demback_id
)
{
  switch( demback_id )
  {
    case 0:
      dec_demback_id = DEC_DEMBACK_0;
      break;

    case 1:
      dec_demback_id = DEC_DEMBACK_1;
      break;

    default:
      ONEX_ERR_FATAL( "Unsupported DEMBACK ID = %u", demback_id, 0, 0 );
      break;
  }

} /* dec_set_demback_id */


/*==========================================================================

FUNCTION DEC_CLEAR_DEMBACK_ID

DESCRIPTION
 Clear current DEMBACK ID as it's not valid anymore.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

============================================================================*/
void dec_clear_demback_id( void )
{
  dec_demback_id = DEC_DEMBACK_INVALID;

} /* dec_clear_demback_id */


/*==========================================================================

FUNCTION DEC_INIT

DESCRIPTION
 Initialize the circuit to Idle. Load the polynomials for  1/2, 1/3,
 1/4 code rate.

DEPENDENCIES
  deint_init() calls dec_init()

RETURN VALUE
 None

SIDE EFFECTS
 None

============================================================================*/
void dec_init( void )
{
  SET_DEMBACK_REG( SVD_POLY2IJ, 0x1c75 );
  SET_DEMBACK_REG( SVD_POLY3IJ, 0x2cb7 );
  SET_DEMBACK_REG( SVD_POLY3K , 0x64   );
  SET_DEMBACK_REG( SVD_POLY4IJ, 0x2e7a );
  SET_DEMBACK_REG( SVD_POLY4KL, 0x0ea5 );

} /* dec_init */


/*===========================================================================

FUNCTION DEC_SC_INIT

DESCRIPTION
  Initialize the circuit for decoding the Sync Channel.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  The decoder is put in continous mode.

===========================================================================*/
void dec_sc_init( void )
{
   /* Put SVD in continous mode.*/
   /* For simulation purpose only initialize state 0 to 0 and saturate all other state metrics.For the real system,
    it doesn't matter since any initialization you do is likely wrong, since the sync channel is always present
    and you're always picking it up part way through a message. */

  /* NOT traffic mode */
  dec_is_traffic_mode = 0;

  SET_SHADOW_DEMBACK_REG_M( SVD_MODE,
                            HWIO_FMSK( SVD_MODE, PACKET ) |
                            HWIO_FMSK( SVD_MODE, INITMODE_2_0 ),
                            VD_CONTINUOUS_V | VD_INITMODE_NO_INIT_V );

} /* dec_sc_init */


/*===========================================================================

FUNCTION DEC_PC_INIT

DESCRIPTION
  Initialize the circuit for decoding the Paging Channel.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  The decoder is put in continous mode.

===========================================================================*/
void dec_pc_init
(
  dec_pc_rate_type pc_rate /* Indicates the Paging Cgannel rate */
)
{
  /* NOT traffic mode */
  dec_is_traffic_mode = 0;

  /*------------------------------------------------------------------*/

  switch (pc_rate) {
    case DEC_PRAT_9600:
      dec_pc_size = DEC_192_BITS / 8;
      break;
    case DEC_PRAT_4800:
      dec_pc_size = DEC_96_BITS / 8;
      break;
    default:
      ONEX_ERR_FATAL("Bad Pg Ch Rate", 0,0,0);
      break;
   }

  /* Give the SVD a clean slate at init  (slot boundary) */
  SET_SHADOW_DEMBACK_REG_M( SVD_MODE,
                            HWIO_FMSK( SVD_MODE, PACKET ) |
                            HWIO_FMSK( SVD_MODE, INITMODE_2_0 ),
                            VD_CONTINUOUS_V | VD_INITMODE_INIT_ALL_STATES_V );

} /* dec_pc_init */


/*===========================================================================

FUNCTION DEC_TC_INIT

DESCRIPTION
  Initialize the circuit for decoding the Traffic Channel.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void dec_tc_init( void )
{
  /* NOT traffic mode */
  dec_is_traffic_mode = 1;

  /* Put SVD in packet mode */
  SET_SHADOW_DEMBACK_REG_M( SVD_MODE,
                            HWIO_FMSK( SVD_MODE, PACKET ) |
                            HWIO_FMSK( SVD_MODE, INITMODE_2_0 ),
                            VD_PACKET_V | VD_INITMODE_SATURATE_STATE_V );

} /* dec_tc_init */


/*===========================================================================

FUNCTION DEC_GET_DEMBACK_PTR

DESCRIPTION
  Get the current DEMBACK pointer to read the decoder output

DEPENDENCIES
  FW should be running.

RETURN VALUE
  Demback start address

SIDE EFFECTS
  None

===========================================================================*/
static uint32* dec_get_demback_ptr
(
  dec_channel_type channel
)
{
  uint32* demback_ptr = NULL;

  /*------------------------------------------------------------------*/

  /* Get HWIO address for the input channel */
  switch( channel )
  {
    case DEC_SYNC_CH:
    case DEC_PCH_CH:
    case DEC_FCH_CH:
    case DEC_DCCH_CH:
    case DEC_SCH_CONV_CH:
    {
      demback_ptr = (uint32*)GET_DEMBACK_ADDR( DEMBACK_BUFF_SVD_DECOB_RD_MIN );
      break;
    }

    case DEC_SCH_TURBO_CH:
    {
      demback_ptr = muxmdsp_get_demback_tdec_decob_addr();
      break;
    }

    default:
    {
      ONEX_ERR_FATAL( "Unsupported channel = %u", channel, 0, 0 );
      break;
    }
  }

  return demback_ptr;

} /* dec_get_demback_ptr */


/*===========================================================================

FUNCTION DEC_READ_SC_DATA

DESCRIPTION
  Read data from the Decoder as Sync Channel data.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void dec_read_sc_data
(
  void *buf             /* Buffer for Sync Channel data */
)
{
  volatile uint32 *dem_buf_ptr;
  uint32 *sc_buf_ptr = (uint32 *) buf;
  int x=0;

  /*------------------------------------------------------------------*/

  /* Get Demback address */
  dem_buf_ptr = dec_get_demback_ptr( DEC_SYNC_CH );

  if( dem_buf_ptr )
  {
    /* SYNC Data in DECOB buffer:
      +--------------------------------------------+
      |    0   |   1    |   2    |   3    |   4    |
      +--------|--------|--------|--------|--------+
      | Status | Status | Data_0 | Data_1 | Data_2 |
      +--------------------------------------------+
       <- 2 * 32 bits ->|<------ 3 * 32 bits ------>
    */

    #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
    M1X_MSG( DEC, LEGACY_FATAL,
             "RAW Data: [0x%08X] [0x%08X] [0x%08X]",
             dem_buf_ptr[DEC_BUFF_STATUS_OFFSET],
             dem_buf_ptr[DEC_BUFF_STATUS_OFFSET+1],
             dem_buf_ptr[DEC_BUFF_STATUS_OFFSET+2] );
    #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

    /* Copy data only */
    while(x < (DEC_SC_BUF_SIZ / DEC_BUFF_WORD_ALIGNMENT))
    {
      sc_buf_ptr[x] = dem_buf_ptr[x+DEC_BUFF_STATUS_OFFSET];
      x++;
    }
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR, "Ch %u DEMBACK ptr is NULL!", DEC_SYNC_CH );
  }

} /* dec_read_sc_data */


/*===========================================================================

FUNCTION DEC_READ_PC_DATA

DESCRIPTION
  Read data from the Decoder Output Buffer as Paging Channel data.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void dec_read_pc_data
(
  void *buf             /* Buffer for Paging Channel data */
)
{
  volatile uint32 *dem_buf_ptr;
  uint32 *pc_buf_ptr = (uint32 *) buf;
  int x=0;

  /*------------------------------------------------------------------*/

  /* Get Demback address */
  dem_buf_ptr = dec_get_demback_ptr( DEC_PCH_CH );

  if( dem_buf_ptr )
  {
    /* PAGING data in DECOB buffer:
      +-----------------------------------------------------+
      |    0   |   1    |   2    |   3    |  ....  |  ....  |
      +--------|--------|--------|--------|--------|--------+
      | Status | Status | Data_0 | Data_1 |  ....  |  ....  |
      +-----------------------------------------------------+
       <- 2 * 32 bits ->|<---------- dec_pc_size ---------->
    */

    #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
    M1X_MSG( DEC, LEGACY_FATAL,
      "RAW Data: [0x%08X] [0x%08X] [0x%08X] [0x%08X] [0x%08X] [0x%08X]",
      dem_buf_ptr[DEC_BUFF_STATUS_OFFSET],
      dem_buf_ptr[DEC_BUFF_STATUS_OFFSET+1],
      dem_buf_ptr[DEC_BUFF_STATUS_OFFSET+2],
      dem_buf_ptr[DEC_BUFF_STATUS_OFFSET+3],
      dem_buf_ptr[DEC_BUFF_STATUS_OFFSET+4],
      dem_buf_ptr[DEC_BUFF_STATUS_OFFSET+5] );
    #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

    /* Copy data only */
    while(x < (dec_pc_size / DEC_BUFF_WORD_ALIGNMENT))
    {
      pc_buf_ptr[x] = dem_buf_ptr[x+DEC_BUFF_STATUS_OFFSET];
      x++;
    }
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR, "Ch %u DEMBACK ptr is NULL!", DEC_PCH_CH );
  }

} /* dec_read_pc_data */


/*===========================================================================

FUNCTION DEC_READ_FCH_DATA

DESCRIPTION
  Read data from the Decoder as Fundamental(Traffic) Channel data.  Note that the data
  for the specified rate is loaded from the beginning of the buffer.  Any
  space at the end of the buffer, which was not requested to be filled,
  is indeterminant.

DEPENDENCIES
  Dec_tc_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void dec_read_fch_data
(
  dec_fch_type      *buf_ptr,    /* Buffer for Traffic Channel data */
  dec_rc_type       rc,          /* Radio Configuration */
  dec_fch_rate_type fch_rate     /* Rate decode buffer to read */
)
{
  uint8 fch_ob_params_index = DEC_FCH_RC_INDEX(rc);
  volatile uint32 *dem_buf_ptr;
  uint32 *data_ptr;
  uint32 dem_buff_offset = 0;
  uint32 data_buff_limit = 0;
  uint32 data_buff_index = 0;

  /*------------------------------------------------------------------*/

  /* Get Demback address */
  dem_buf_ptr = dec_get_demback_ptr( DEC_FCH_CH );

  if( dem_buf_ptr )
  {
    if( fch_ob_params_index >= DEC_FCH_NUM_RC )
    {
      M1X_MSG( DEC, LEGACY_ERROR,
               "Invalid RC for F-FCH, rc=%d",
               (int) rc );
      return;
    }

    if( ( fch_rate == DEC_RATE_ALL ) || ( fch_rate == DEC_RATE_FULL ) )
    {
      data_ptr = (uint32 *) buf_ptr->buf.bits1;
      data_buff_index = 0;

      dem_buff_offset =
        (dec_fch_ob_params[fch_ob_params_index].dec_ob_addr_full_rate /
         DEC_BUFF_WORD_ALIGNMENT ) + DEC_BUFF_STATUS_OFFSET;

      data_buff_limit = dec_fch_ob_params[fch_ob_params_index].dec_full_rate_siz /
                        DEC_BUFF_WORD_ALIGNMENT;

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      //assumes RC3
      M1X_MSG( DEC, LEGACY_FATAL,
        "RAW Data: FULL [0x%08X] [0x%08X] [0x%08X] "
        "[0x%08X] [0x%08X] [0x%08X]",
        dem_buf_ptr[dem_buff_offset],
        dem_buf_ptr[dem_buff_offset+1],
        dem_buf_ptr[dem_buff_offset+2],
        dem_buf_ptr[dem_buff_offset+3],
        dem_buf_ptr[dem_buff_offset+4],
        dem_buf_ptr[dem_buff_offset+5] );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

      while( data_buff_index < data_buff_limit )
      {
        data_ptr[data_buff_index] = dem_buf_ptr[data_buff_index + dem_buff_offset];
        data_buff_index++;
      }
    }

     /* Read Half Rate Data */
    if( ( fch_rate == DEC_RATE_ALL ) || ( fch_rate == DEC_RATE_HALF ) )
    {
      data_ptr = (uint32 *) buf_ptr->buf.bits2;
      data_buff_index = 0;

      dem_buff_offset =
        (dec_fch_ob_params[fch_ob_params_index].dec_ob_addr_half_rate /
         DEC_BUFF_WORD_ALIGNMENT ) + DEC_BUFF_STATUS_OFFSET;

      data_buff_limit = dec_fch_ob_params[fch_ob_params_index].dec_half_rate_siz /
                        DEC_BUFF_WORD_ALIGNMENT;

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      //assumes RC3
      M1X_MSG( DEC, LEGACY_FATAL,
               "RAW Data: HALF [0x%08X] [0x%08X] [0x%08X]",
               dem_buf_ptr[dem_buff_offset],
               dem_buf_ptr[dem_buff_offset+1],
               dem_buf_ptr[dem_buff_offset+2] );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

      while( data_buff_index < data_buff_limit )
      {
        data_ptr[data_buff_index] = dem_buf_ptr[data_buff_index + dem_buff_offset];
        data_buff_index++;
      }
    }

     /* Read Quarter Rate Data */
    if( ( fch_rate == DEC_RATE_ALL ) || ( fch_rate == DEC_RATE_QTR ) )
    {
      data_ptr = (uint32 *) buf_ptr->buf.bits4;
      data_buff_index = 0;

      dem_buff_offset =
        (dec_fch_ob_params[fch_ob_params_index].dec_ob_addr_qtr_rate /
         DEC_BUFF_WORD_ALIGNMENT ) + DEC_BUFF_STATUS_OFFSET;

      data_buff_limit = dec_fch_ob_params[fch_ob_params_index].dec_qtr_rate_siz /
                        DEC_BUFF_WORD_ALIGNMENT;

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      //assumes RC3
      M1X_MSG( DEC, LEGACY_FATAL,
               "RAW Data: QTR  [0x%08X] [0x%08X]",
               dem_buf_ptr[dem_buff_offset],
               dem_buf_ptr[dem_buff_offset+1] );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

      while(data_buff_index < data_buff_limit)
      {
        data_ptr[data_buff_index] = dem_buf_ptr[data_buff_index + dem_buff_offset];
        data_buff_index++;
      }
    }
     /* Read Eighth Rate Data */

    if( ( fch_rate == DEC_RATE_ALL ) || ( fch_rate == DEC_RATE_8TH ) )
    {
      data_ptr = (uint32 *) buf_ptr->buf.bits8;
      data_buff_index = 0;

      dem_buff_offset =
        (dec_fch_ob_params[fch_ob_params_index].dec_ob_addr_8th_rate /
         DEC_BUFF_WORD_ALIGNMENT ) + DEC_BUFF_STATUS_OFFSET;

      data_buff_limit = dec_fch_ob_params[fch_ob_params_index].dec_8th_rate_siz /
                        DEC_BUFF_WORD_ALIGNMENT;

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      //assumes RC3
      M1X_MSG( DEC, LEGACY_FATAL,
               "RAW Data: 8TH  [0x%08X]",
               dem_buf_ptr[dem_buff_offset] );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

      while(data_buff_index < data_buff_limit)
      {
        data_ptr[data_buff_index] = dem_buf_ptr[data_buff_index + dem_buff_offset];
        data_buff_index++;
      }
    }
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR, "Ch %u DEMBACK ptr is NULL!", DEC_FCH_CH );
  }

} /* dec_read_fch_data */

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION DEC_READ_DCCH_DATA

DESCRIPTION
  Read data from the Decoder as DCCH data.

DEPENDENCIES
  Dec_tc_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void dec_read_dcch_data
(
  dec_fch_type      *buf_ptr,    /* Buffer for Traffic Channel data      */
  dec_rc_type       rc           /* Radio configuration                    */
)
{
  uint8 dcch_ob_param_index = DEC_DCCH_RC_INDEX(rc);

  volatile uint32 *dem_buf_ptr;
  uint32 *data_ptr;
  uint32 dem_buff_offset = 0;
  uint32 data_buff_limit = 0;
  uint32 data_buff_index = 0;

  /*------------------------------------------------------------------*/

  /* Get Demback address */
  dem_buf_ptr = dec_get_demback_ptr( DEC_DCCH_CH );

  if( dem_buf_ptr )
  {
    if( dcch_ob_param_index < DEC_DCCH_NUM_RC )
    {
      data_ptr = (uint32 *) buf_ptr->buf.bits1dcch;
      data_buff_index = 0;

      dem_buff_offset =
        (dec_dcch_ob_params[dcch_ob_param_index].dec_ob_addr_full_rate /
         DEC_BUFF_WORD_ALIGNMENT ) + DEC_BUFF_STATUS_OFFSET;

      data_buff_limit = dec_dcch_ob_params[dcch_ob_param_index].dec_full_rate_siz /
                        DEC_BUFF_WORD_ALIGNMENT;

      while( data_buff_index < data_buff_limit )
      {
        data_ptr[data_buff_index] = dem_buf_ptr[data_buff_index + dem_buff_offset];
        data_buff_index++;
      }
    }
    else
    {
      M1X_MSG( DEC, LEGACY_ERROR,
               "Invalid RC for F-DCCH, rc=%d",
               (int) rc );
    }
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR, "Ch %u DEMBACK ptr is NULL!", DEC_DCCH_CH );
  }

} /* dec_read_dcch_data */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION DEC_READ_SCH_DATA

DESCRIPTION
  Read data from the Decoder as Supplemental(Traffic) Channel data (IS95C SCH).
  This function reads each mux pdu (single/double) into the buffer pointed
  to by the dsm_item.
DEPENDENCIES
  Dec_tc_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void dec_read_sch_data
(
  dec_sch_cfg_type *sch_rec_ptr,  /* Pointer to rxc_sch record pointer */
  dsm_item_type **dsm_item_ptr,   /* array of dsm_item pointers */
  byte *mux_pdu_count             /* return the number of MUX PDU read */
)
{
  uint8 sch_ob_param_index = 0;
  byte shift_bits_size = 0;
  byte double_mux_pdu_size = 0;
  word dec_sch_size = 0;
  word dec_sch_ob_addr = 0;
  byte dsm_item_index=0;
  byte cnt;
  uint16 *pdu3_data_ptr;
  volatile uint32 *dem_buf_ptr;
  uint32 buff_offset = 0;
  uint32 decob_offset = 0;
  uint32 data_buff_limit = 0;
  uint32 data_buff_index = 0;

  /* Indicates the number of double sized mux pdu. If the mux pdu type
     is single then the total number of mux pdu's is 2 times of double_mux_pdu */
  word double_mux_pdu_cnt;

  /*------------------------------------------------------------------*/

  /* --------------------------------------------------------------------
                      NULL CHECK THE SCH PTR
     -------------------------------------------------------------------- */
  if(sch_rec_ptr == NULL)
  {
    M1X_MSG( DEC, LEGACY_FATAL,
             "dec_read_sch_data: sch_rec_ptr is NULL!" );
    *mux_pdu_count = 0;
    return;
  }

  /* --------------------------------------------------------------------
               RANGE CHECK THE PARAM INDEXES DERIVED FROM RC AND RATE
     -------------------------------------------------------------------- */
  sch_ob_param_index = DEC_SCH_RC_INDEX(sch_rec_ptr->rc);

  if( (sch_ob_param_index >= NUM_SCH_RC) ||
      ( sch_rec_ptr->sch_rate >= NUM_SCH_RATES ) )
  {
    M1X_MSG( DEC, LEGACY_FATAL,
             "dec_read_sch_data: Index out of bounds! [%d] [%d]",
             sch_ob_param_index,
             (uint32) sch_rec_ptr->sch_rate);
    *mux_pdu_count = 0;
    return;
  }

  #ifndef FEATURE_IS2000_REL_A_SVD
  if(sch_rec_ptr->sch_rate == DEC_SCH_RATE_1X)
  {
    /* The shifted bits are stored in a separate 32bit word for rate 1X. */
    shift_bits_size = 4;
  }
  #endif /* !FEATURE_IS2000_REL_A_SVD */

  /* ----------------------------------------------------------------
     Determine dem_buff offsets based on RC.
     Determine the size of the SCH data expected in the buff.
     ---------------------------------------------------------------- */

  switch( sch_rec_ptr->rc )
  {
    case DEC_RC3:
    case DEC_RC4:
      double_mux_pdu_size = 44;
      decob_offset = DEC_SCH_FULL_OB_ADDR_RC3_RC4;
      break;

    #ifdef FEATURE_IS2000_1X_ADV
    case DEC_RC11:
      double_mux_pdu_size = 44;
      decob_offset = DEC_SCH_FULL_OB_ADDR_RC11;
      break;
    #endif /* FEATURE_IS2000_1X_ADV */

    case DEC_RC5:
      double_mux_pdu_size = 68;
      decob_offset = DEC_SCH_FULL_OB_ADDR_RC5;
      break;

    default:
      M1X_MSG( DEC, LEGACY_FATAL,
               "dec_read_sch_data: Bad Radio Config %d",
               (uint32) sch_rec_ptr->rc );
      *mux_pdu_count = 0;
      return;
  }

  dec_sch_size = dec_sch_ob_params[sch_ob_param_index][sch_rec_ptr->sch_rate];

  /* --------------------------------------------------------------------
            READ SCH DATA FROM DEM_BUFF (CONV) or TDEC (TURBO)
     -------------------------------------------------------------------- */

  if(sch_rec_ptr->code_type == DEC_TURBO)
  {
    /* ----------------------------- SCH Turbo ----------------------------- */
    /* Get Demback address */
    dem_buf_ptr = dec_get_demback_ptr( DEC_SCH_TURBO_CH );

    buff_offset = DEC_BUFF_STATUS_OFFSET;
  }
  else
  {
    /* ------------------------- SCH Convolutional ------------------------- */
    /* Get Demback address */
    dem_buf_ptr = dec_get_demback_ptr( DEC_SCH_CONV_CH );

    buff_offset = ( decob_offset / DEC_BUFF_WORD_ALIGNMENT )
                                                  + DEC_BUFF_STATUS_OFFSET;
  }

  data_buff_limit = ( dec_sch_size + shift_bits_size + 3 )
                                                / DEC_BUFF_WORD_ALIGNMENT;

  if( dem_buf_ptr )
  {
    /* Read out SDU data into a temporary array */
    while(data_buff_index < data_buff_limit)
    {
      dec_temp_sdu_data_array[data_buff_index] =
                              dem_buf_ptr[data_buff_index + buff_offset];
      data_buff_index++;
    }

    /* --------------------------------------------------------------------
                    PROCESS MUXPDU5 DATA STORED LOCALLY
       -------------------------------------------------------------------- */

    dec_temp_sdu_data_ptr = (uint16 *) dec_temp_sdu_data_array;

    /* Special case if SCH Rate = 1X */
    if(sch_rec_ptr->sch_rate == DEC_SCH_RATE_1X)
    {
      if (dsm_item_ptr[dsm_item_index] != NULL)
      {
        #ifdef FEATURE_IS2000_REL_A_SVD
        /* Do not adjust the output buffer address since no shifting is
        ** done to preserve the MM bit.
        **
        ** This change also affects the deinterleaver programming and the
        ** processing of rx frames in the rxdemux layer.
        */
        pdu3_data_ptr = dec_temp_sdu_data_ptr;
        #else
        /* For RC3 & RC4 there is MM bit. For RC5 there is MM bit and EIB bit,
        ** so we shift by 2 bits.  In either case the shifted data is offset by
        ** 1 32 bit word in the output buffer.  Since pdu3_data_ptr is word
        ** aligned, add 2 words (32 bits) to the OB address.
        */
        pdu3_data_ptr = dec_temp_sdu_data_ptr + 2;
        #endif /* FEATURE_IS2000_REL_A_SVD */

        dsm_item_ptr[dsm_item_index]->used = double_mux_pdu_size/2; /* bytes */

        /* copy single side PDU 3, size = double_mux_pdu_size /2 */
        memscpy(dsm_item_ptr[dsm_item_index++]->data_ptr,
                double_mux_pdu_size/2,
                pdu3_data_ptr,
                double_mux_pdu_size/2);
        pdu3_data_ptr = pdu3_data_ptr + double_mux_pdu_size/4;

        *mux_pdu_count=1;
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "RXC has too few DSM items!!!");
        *mux_pdu_count=0;
      }
    }
    else /* SCH RATE 2X AND ABOVE */
    {
      double_mux_pdu_cnt = dec_sch_size / double_mux_pdu_size;

      for(cnt=0; cnt < double_mux_pdu_cnt; cnt++)
      {
        if (dsm_item_ptr[dsm_item_index] != NULL)
        {
          pdu3_data_ptr = dec_temp_sdu_data_ptr + dec_sch_ob_addr;
          if(sch_rec_ptr->mux_pdu_double)
          {
            /* Double Sized MUX PDU  Option */
            dsm_item_ptr[dsm_item_index]->used = double_mux_pdu_size;
            /* copy double-sided PDU 3, size = double_mux_pdu_size*/
            memscpy(dsm_item_ptr[dsm_item_index++]->data_ptr,
                    double_mux_pdu_size,
                    pdu3_data_ptr,
                    double_mux_pdu_size);
          }
          else
          {
            /* Single sized MUX PDU Option */
            /* Read 2 single mux pdu */

            dsm_item_ptr[dsm_item_index]->used = (double_mux_pdu_size / 2);
            /* copy the first single-sided PDU 3, size = double_mux_pdu_size / 2*/
            memscpy(dsm_item_ptr[dsm_item_index++]->data_ptr,
                    double_mux_pdu_size/2,
                    pdu3_data_ptr,
                    double_mux_pdu_size/2);

            pdu3_data_ptr = pdu3_data_ptr + double_mux_pdu_size / 4;

            dsm_item_ptr[dsm_item_index]->used = (double_mux_pdu_size / 2);
            /* copy the second single-sided PDU 3, size = double_mux_pdu_size / 2*/
            memscpy(dsm_item_ptr[dsm_item_index++]->data_ptr,
                    double_mux_pdu_size/2,
                    pdu3_data_ptr,
                    double_mux_pdu_size/2);
          }

          dec_sch_ob_addr += (double_mux_pdu_size / 2);
          if(sch_rec_ptr->code_type == DEC_CONVOLUTIONAL)
          {
             /* Convolutional codes have LTU (16 bits of CRC) */
             /* Skip over 1 word (1word = 16 bits) */
             dec_sch_ob_addr += 1;
          }
            /* dec_sch_ob_addr now points to the next mux pdu */
        }
        else
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "RXC - too few DSM items");
          break; /* get out of this loop */
        }
      } /* for */

      *mux_pdu_count = dsm_item_index; /* Total number of mux pdu's */
    }
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR, "SCH DEMBACK ptr is NULL!" );
  }

} /* dec_read_sch_data */


/*=========================================================================
FUNCTION DEC_READ_CH_STATUS

DESCRIPTION
  This function reads in the status of a channel from the Short Buffer

  The status for the Sync Channel, Paging Channel & Fundicated Channel
  all have the same format

  This function receives an offset into the output buffer of the status
  information, and pointers to variables in which to store the information.

  The format of the status information in the output buffer is as follows:

  +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
  | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
  +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
  | QU | CP | BC | VI |            LTU CRC (27:20)            |                                                Energy (19:0)                                      |
  +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
  |  MTVA TB     |                                  Symbol Error Count (28:16)    |                                Absolute Sum (15:0)                            |
  +---------------------------------------------------------------------------------------------------------------------------------------------------------------+


  Where:

      BC = Bad Count
      VI = Violation
      UN = Unused
      QU = Quality
      CP = CRC Pass

  All resulting values are stored in the lowest order bits except for the
  status result, which is stored as follows:

  +---------------------------------------+
  |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
  +---------------------------------------+
  | BC | VI |      Unused       | QU | CP |
  +---------------------------------------+

  The format of this result is for backward compatibility with an earlier
  version of this module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
LOCAL void dec_read_ch_status
(
  dec_channel_type channel,
  word             buffer_offset,
  byte             *pstatus,
  word             *pser,
  int4             *penergy,
  word             *pltu_crc
)
{
  uint32          temp_status   = 0;
  uint32          total_metrics = 0;
  volatile uint32 *dem_buf_ptr;

  /*------------------------------------------------------------------*/

  /* Get Demback address */
  dem_buf_ptr = dec_get_demback_ptr( channel );

  if( dem_buf_ptr )
  {
    #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
    M1X_MSG( DEC, LEGACY_FATAL,
             "RAW Status: [0x%08X] [0x%08X]",
             dem_buf_ptr[buffer_offset],
             dem_buf_ptr[buffer_offset+1] );
    #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

    /* Read the first word */
    temp_status = dem_buf_ptr[buffer_offset];

    total_metrics += temp_status;

    if( penergy )
    {
      *penergy = (int4)( temp_status & 0xFFFFF );

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      M1X_MSG( DEC, LEGACY_FATAL, "Energy: %d", *penergy );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
    }

    if( pltu_crc )
    {
      *pltu_crc = (word)( temp_status >> 20 & 0xFF );

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      M1X_MSG( DEC, LEGACY_FATAL, "LTU CRC: %d", *pltu_crc );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
    }

    /*
    +---------------------------------------+
    |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
    +---------------------------------------+
    | BC | VI |      Unused       | QU | CP |
    +---------------------------------------+  */
    if( pstatus )
    {
      *pstatus = (byte) ( ( temp_status >> 22 ) & 0xC0 ) | /* BC | VI */
                 (byte) ( temp_status >> 30 );             /* QU | CP */

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      M1X_MSG( DEC, LEGACY_FATAL, "Status: %d", *pstatus );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
    }


    /* Read the second word */
    temp_status = dem_buf_ptr[buffer_offset+1];

    total_metrics += temp_status;

    if( pser )
    {
      *pser= (word)( ( temp_status >> 16 ) & 0x1FFF );

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      M1X_MSG( DEC, LEGACY_FATAL, "SER: %d", *pser );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
    }

    /* Check for a 'dead' DECOB, as evidenced by zeroes for all params, then
       do a reinitialization of some key registers */
    if( total_metrics == 0 )
    {
      dec_init();

      SET_SHADOW_DEMBACK_REG_M( SVD_MODE,
                                HWIO_FMSK( SVD_MODE, PACKET ),
                                dec_is_traffic_mode ? VD_PACKET_V : 0 );
    }
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR, "Ch %u DEMBACK ptr is NULL!", channel );
  }

} /* dec_read_ch_status */


/*=========================================================================
FUNCTION DEC_READ_SC_STATUS

DESCRIPTION
  This function reads the Status (SER, Energy Metrics) for Sync Channel frames.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void dec_read_sc_status
(
  /* Pointer the buffer where sync channel decoding status will be stored */
  dec_sc_status_type *sc_status
)
{
  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x Sync Channel Status" );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
  dec_read_ch_status( DEC_SYNC_CH,
                      0,
                      &sc_status->status,
                      &sc_status->ser,
                      &sc_status->energy,
                      NULL );

} /* dec_read_sc_status */


/*=========================================================================
FUNCTION DEC_READ_PC_STATUS

DESCRIPTION
  This function reads the Status (SER, Energy Metrics) for Paging Channel frames.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void dec_read_pc_status
(
  /* Pointer the buffer where paging channel decoding status will be stored */
  dec_pc_status_type *pc_status
)
{
  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x Paging Channel Status" );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
  dec_read_ch_status( DEC_PCH_CH,
                      0,
                      &pc_status->status,
                      &pc_status->ser,
                      &pc_status->energy,
                      NULL );

} /* dec_read_pc_status */


/*=========================================================================
FUNCTION DEC_READ_FCH_STATUS

DESCRIPTION
  This function reads the Status (CRC, SER, Energy Metrics) for
  Fundamental Channel frames.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void dec_read_fch_status
(
  /* Indicates the Radio Configuration for FCH */
  dec_rc_type rc,

  /* Pointer the buffer where FCH decoding status will be stored */
  dec_fch_status_type *fch_status
)
{
  int count;
  uint8 fch_ob_params_index = DEC_FCH_RC_INDEX(rc);

  /*------------------------------------------------------------------*/

  if( fch_ob_params_index >= DEC_FCH_NUM_RC )
  {
    M1X_MSG( DEC, LEGACY_ERROR,
             "Invalid RC for F-FCH, rc=%d",
             (int) rc );
    return;
  }

  if( rxc_ta_info.f_fch_ta_log )
  {
    rxc_ta_info.f_fch_ta_comp_sclk = timetick_get();
    rxc_ta_info.f_fch_ta_log       = FALSE;

    /* Log to SRCH, if R-FCH logging is already done and if its QTA or
       FSCH and RSCH is not going to be assigned */
    if( ( txc_ta_info.r_fch_ta_log == FALSE ) &&
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

  /* To calculate the offset to the status information for each buffer, take */
  /* the starting address of the next buffer, divide by 2 to convert a byte  */
  /* offset to a word offset, then subtract the size of the status           */
  /* information.                                                            */

  /* Read FCH Status corresponding to FULL RATE HYPOTHESIS */
  count = (uint16) DEC_RATE_FULL - 1;
  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x FCH Full Rate Status" );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
  dec_read_ch_status( DEC_FCH_CH,
                      dec_fch_ob_params[fch_ob_params_index].
                      dec_ob_addr_full_rate / DEC_BUFF_WORD_ALIGNMENT,
                      &(fch_status->status[count]),
                      &(fch_status->ser   [count]),
                      &(fch_status->energy[count]),
                      NULL );

  /* Read FCH Status corresponding to HALF RATE HYPOTHESIS */
  count++;
  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x FCH Half Rate Status" );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
  dec_read_ch_status( DEC_FCH_CH,
                      dec_fch_ob_params[fch_ob_params_index].
                      dec_ob_addr_half_rate / DEC_BUFF_WORD_ALIGNMENT,
                      &(fch_status->status[count]),
                      &(fch_status->ser   [count]),
                      &(fch_status->energy[count]),
                      NULL );

  /* Read FCH Status corresponding to QTR RATE HYPOTHESIS */
  count++;
  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x FCH Qtr Rate Status" );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
  dec_read_ch_status( DEC_FCH_CH,
                      dec_fch_ob_params[fch_ob_params_index].
                      dec_ob_addr_qtr_rate / DEC_BUFF_WORD_ALIGNMENT,
                      &(fch_status->status[count]),
                      &(fch_status->ser   [count]),
                      &(fch_status->energy[count]),
                      NULL );

  /* Read FCH Status corresponding to 8TH RATE HYPOTHESIS */
  count++;
  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x FCH 8th Rate Status" );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
  dec_read_ch_status( DEC_FCH_CH,
                      dec_fch_ob_params[fch_ob_params_index].
                      dec_ob_addr_8th_rate / DEC_BUFF_WORD_ALIGNMENT,
                      &(fch_status->status[count]),
                      &(fch_status->ser   [count]),
                      &(fch_status->energy[count]),
                      NULL );

   /* Scale the ser in software for MSM5000 (this was done in hardware on
      the MSM3000) */
   fch_status->ser[(uint16) DEC_RATE_HALF - 1] *= 2;
   fch_status->ser[(uint16) DEC_RATE_QTR - 1]  *= 4;
   fch_status->ser[(uint16) DEC_RATE_8TH - 1]  *= 8;

} /* dec_read_fch_status */

#ifdef FEATURE_IS2000_REL_A_SVD
/*=========================================================================
FUNCTION DEC_READ_DCCH_STATUS

DESCRIPTION
  This function reads the Status (CRC, SER, Energy Metrics) for
  DCCH frames.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void dec_read_dcch_status
(
  /* Indicates the Radio Configuration for DCCH */
  dec_rc_type rc,

  /* Pointer to the struct where DCCH decoding status will be stored */
  dec_dcch_status_type *dcch_status
)

{
  uint8 dcch_ob_param_index = DEC_DCCH_RC_INDEX(rc);

  /*------------------------------------------------------------------*/

  if( dcch_ob_param_index < DEC_DCCH_NUM_RC )
  {
    #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
    M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x DCCH Status" );
    #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
    dec_read_ch_status( DEC_DCCH_CH,
                        dec_dcch_ob_params[dcch_ob_param_index].
                        dec_ob_addr_full_rate / DEC_BUFF_WORD_ALIGNMENT,
                        &dcch_status->status[DEC_RATE_FULL-1],
                        &dcch_status->ser[DEC_RATE_FULL-1],
                        &dcch_status->energy[DEC_RATE_FULL-1],
                        NULL );
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR,
             "Invalid RC for DCCH, rc=%d",
             (int) rc );
  }

} /* dec_read_dcch_status */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*=========================================================================
FUNCTION DEC_READ_SCH_STATUS

DESCRIPTION
  This function reads the Status (SER, Energy Metrics) for SCH Channel frames

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void dec_read_sch_status
(
  #ifdef FEATURE_IS2000_REL_A
  dec_mux_pdu_type_type pdu_type,
  #endif
  dec_rc_type sch_rc,               /* RC for SCH */
  dec_sch_rate_type sch_rate,       /* Rate Type (2X/4X/8X/16X for SCH */
  dec_sch_status_type *sch_status,
  dec_coding_type code_type         /* Convolutional or TURBO */
)
{
  word buffer_offset = 0;
  boolean skip = FALSE;

  /*------------------------------------------------------------------*/

  if(code_type == DEC_TURBO)
  {
    /* ----------------------------- SCH Turbo ----------------------------- */
    /*
    +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
    | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
    +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
    | CF | ET | UNUSED  |  ITERATIONS RUN   |    UNUSED    |                                             Energy (20:0)                                              |
    +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
    |  UNUSED  UNUSED  UNUSED  UNUSED  UNUSED    |     PACKET LENGTH [HDR ONLY] (22:14)       |                             MIN_LLR (13:0)                          |
    +---------------------------------------------------------------------------------------------------------------------------------------------------------------+


    Where:

        CF = CRC Fail
        ET = Early Terminated
    */

    volatile uint32 *buf_ptr = dec_get_demback_ptr( DEC_SCH_TURBO_CH );
    uint32 temp_status;

    if( buf_ptr )
    {
      /* FIRST STATUS WORD */
      temp_status = buf_ptr[0];

      sch_status->turbo_sch_status.energy = (int4) (temp_status & 0x001fffff);

      /* Check CRC Fail Flag (bit 31).
         Since bit = 1 means failure, invert the polarity here. */
      if( (temp_status >> 31) & 0x1 )
      {
        sch_status->turbo_sch_status.crc_pass = FALSE;
      }
      else
      {
        sch_status->turbo_sch_status.crc_pass = TRUE;
      }

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x SCH Turbo Status" );

      M1X_MSG( MUX, LEGACY_FATAL,
        "RAW Turbo Status: [0x%08X] [0x%08X], crc pass %d",
        buf_ptr[0],
        buf_ptr[1],
        sch_status->turbo_sch_status.crc_pass );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

      sch_status->turbo_sch_status.num_iteration = ((temp_status >> 24) & 0x0F);

      /* SECOND STATUS WORD */
      temp_status = buf_ptr[1];

      sch_status->turbo_sch_status.min_llr = (word)(temp_status & 0x00003fff);
    }
    else
    {
      M1X_MSG(DEC, LEGACY_ERROR,"Ch %u DEMBACK ptr is NULL!",DEC_SCH_TURBO_CH);
    }
  }
  else
  {
    /* ------------------------- SCH Convolutional ------------------------- */
    switch( sch_rc )
    {
      case DEC_RC3:
      case DEC_RC4:
        buffer_offset = DEC_SCH_FULL_OB_ADDR_RC3_RC4;
        break;

      #ifdef FEATURE_IS2000_1X_ADV
      case DEC_RC11:
        buffer_offset = DEC_SCH_FULL_OB_ADDR_RC11;
        break;
      #endif /* FEATURE_IS2000_1X_ADV */

      case DEC_RC5:
        buffer_offset = DEC_SCH_FULL_OB_ADDR_RC5;
        break;

      default:
        M1X_MSG( DEC, LEGACY_FATAL,
                 "dec_read_sch_status: Bad Radio Config %d",
                 (uint32) sch_rc );
        skip = TRUE;
        break;
    }

    if( !skip )
    {
      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      M1X_MSG( DEC, LEGACY_FATAL, "Reading 1x SCH Conv Status" );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */
      dec_read_ch_status( DEC_SCH_CONV_CH,
                          buffer_offset / DEC_BUFF_WORD_ALIGNMENT,
                          &sch_status->conv_sch_status.status,
                          &sch_status->conv_sch_status.ser,
                          &sch_status->conv_sch_status.energy,
                          &sch_status->conv_sch_status.ltu_crc );
    }
    else
    {
      /* Couldn't calculate a proper buffer offset so gracefully return 0's */
      sch_status->conv_sch_status.status = 0;
      sch_status->conv_sch_status.ser = 0;
      sch_status->conv_sch_status.energy = 0;
      sch_status->conv_sch_status.ltu_crc = 0;
    }
  }

} /* dec_read_sch_status */

/*=========================================================================
FUNCTION DEC_DTX_THRESH_INIT

DESCRIPTION
  This function initializes the dtx threshold value depending on the RC and
  the code type.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Writes into the MSM register DEFAULT_IQ_ACC_WH
=============================================================================*/
/* Engineering control variables that allow tweaks for testing */
/*lint -e552 called from rxctraffic.c */
word dec_dtx_thresh;
byte dec_dtx_debug=0;

void dec_dtx_thresh_init
(
  dec_rc_type         rc,
  dec_coding_type     code_type,
  dec_sch_rate_type   rate
)
{
  /*------------------------------------------------------------------*/

  if(dec_dtx_debug == 0)
  {
    switch(rc)
    {
      case DEC_RC3:
      {
        switch(rate)
        {
          case DEC_SCH_RATE_1X:
          {
            dec_dtx_thresh = 225;
            break;
          }
          case DEC_SCH_RATE_2X:
          {
            dec_dtx_thresh = 464;
            break;
          }
          case DEC_SCH_RATE_4X:
          {
            dec_dtx_thresh = 903;
            break;
          }
          case DEC_SCH_RATE_8X:
          {
            dec_dtx_thresh = 1804;
            break;
          }
          case DEC_SCH_RATE_16X:
          {
            dec_dtx_thresh = 3569;
            break;
          }
          default:
          {
            dec_dtx_thresh = 0;
            M1X_MSG( DEC, LEGACY_ERROR,
                     "Invalid SCH_RATE (%d) for RC%d()",
                     (uint32)rate,
                     (uint32) rc );
            break;
          }
        }
        break;
      }
      case DEC_RC4:
      {
        switch(rate)
        {
          case DEC_SCH_RATE_1X:
          {
            dec_dtx_thresh = 124;
            break;
          }
          case DEC_SCH_RATE_2X:
          {
            dec_dtx_thresh = 236;
            break;
          }
          case DEC_SCH_RATE_4X:
          {
            dec_dtx_thresh = 461;
            break;
          }
          case DEC_SCH_RATE_8X:
          {
            dec_dtx_thresh = 906;
            break;
          }
          case DEC_SCH_RATE_16X:
          {
            dec_dtx_thresh = 1769;
            break;
          }
          case DEC_SCH_RATE_32X:
          {
            dec_dtx_thresh = 1769*2;
            break;
          }
          default:
          {
            dec_dtx_thresh = 0;
            M1X_MSG( DEC, LEGACY_ERROR,
                     "Invalid SCH_RATE (%d) for RC%d()",
                     (uint32)rate,
                     (uint32) rc );
            break;
          }
        }
        break;
      }
      case DEC_RC5:
      {
        switch (rate)
        {
          case DEC_SCH_RATE_1X:
          {
            dec_dtx_thresh = 243;
            break;
          }
          case DEC_SCH_RATE_2X:
          {
            dec_dtx_thresh = 461;
            break;
          }
          case DEC_SCH_RATE_4X:
          {
            dec_dtx_thresh = 893;
            break;
          }
          case DEC_SCH_RATE_8X:
          {
            dec_dtx_thresh = 1805;
            break;
          }
          default:
          {
            dec_dtx_thresh = 0;
            M1X_MSG( DEC, LEGACY_ERROR,
                     "Invalid SCH_RATE (%d) for RC%d()",
                     (uint32)rate,
                     (uint32) rc );
            break;
          }
        } /* switch */
        break;
      }
#ifdef FEATURE_IS2000_1X_ADV
      case DEC_RC11:
      {
        switch(rate)
        {
          case DEC_SCH_RATE_1X:
          {
            dec_dtx_thresh = 124;
            break;
          }
          case DEC_SCH_RATE_2X:
          {
            dec_dtx_thresh = 236;
            break;
          }
          case DEC_SCH_RATE_4X:
          {
            dec_dtx_thresh = 461;
            break;
          }
          case DEC_SCH_RATE_8X:
          {
            dec_dtx_thresh = 906;
            break;
          }
          case DEC_SCH_RATE_16X:
          {
            dec_dtx_thresh = 1769;
            break;
          }
          case DEC_SCH_RATE_32X:
          {
            dec_dtx_thresh = 1769*2;
            break;
          }
          default:
          {
            dec_dtx_thresh = 0;
            M1X_MSG( DEC, LEGACY_ERROR,
                     "Invalid SCH_RATE (%d) for RC%d()",
                     (uint32)rate,
                     (uint32) rc );
            break;
          }
        }
        break;
      }
#endif /* FEATURE_IS2000_1X_ADV */
      default:
      {
        ONEX_ERR_FATAL( "Bad RC %d", (uint16) rc, 0, 0  );
      }
    } /* switch rc */
  } /* if (dec_dtx_debug) */

} /* dec_dtx_thresh_init */


#ifdef FEATURE_IS2000_REL_A /* 32X */
/*===========================================================================

FUNCTION DEC_READ_MUX_PDU5

DESCRIPTION
  Read one MUX PDU5 data from the Decoder. This function parses the MUX PDU5
  header and chain the dsm items if necessary.
DEPENDENCIES

RETURN VALUE
  TRUE if a MuxPDU5 has been read.
  FALSE if otherwise.

SIDE EFFECTS
  None

===========================================================================*/
boolean dec_read_mux_pdu5
(
  word*          start_pos,    /* Start position of the data  */
  dsm_item_type* dsm_item_ptr, /* array of dsm_item pointers */
  word           max_pos       /* on or after this point is garbage */
)
{
  word dec_sch_ob_addr;
  /* dec_sch_ob_addr points to the address of the sch data and status in
   the decoder output buffer */
  byte offset;          /* offset when start address is odd. */
  byte junk_at_end=0;   /* Junk bytes at the end of the reading window */
  word pos;             /* current position */
  byte sr_id;           /* sr_id field */
  byte length_ind;      /* length indicator */
  word length = 0;      /* length of the pay load */
  int  num_words;
  boolean pdu_found = FALSE;  /* return value */
  word words_read;      /* # of words read from the decoder */
  word words_need;      /* # of words need to read */
  dsm_item_type *tail = NULL;
                        /* keeps track of the last dsm item */
  uint16  *pdu5_data_ptr;

  /*------------------------------------------------------------------*/

  dec_sch_ob_addr =  DEC_SCH_RAM_BASE;
  pos = *start_pos;     /* Set position pointer to the start position */

  if (dsm_item_ptr != NULL)
  {
    while( (pos + 1) < max_pos ) /* each PDU, including PDU header, must has more than 1 byte*/
    {
      /*
      ** Notice that data are read out from decoder in half word (16 bits).
      ** If the start position is odd, we will have to read one byte ahead.
      */

      dec_sch_ob_addr = DEC_SCH_RAM_BASE + pos/2;

      /*
      ** Offset indicates where the real data starts.
      ** If offset is 0, data starts from the first byte.
      ** If offset is 1, the first byte is junk.
      */
      offset = pos%2;

      /*
      ** Because the consumer of the data does not free the dsm item all
      ** the time, e.g., TDSO. So it is possible that the data_ptr is an
      ** odd number.
      ** Base on the assumption that only this function can offset that
      ** pointer by one, which makes it odd, we deduct by one if we see
      ** an odd address. Otherwise, a data abort can happen.
      ** A clean solution would be the consumer always free the dsm item
      ** so the data_ptr is reset automatically.
      */

      if (((dword)dsm_item_ptr->data_ptr)%2 != 0)
      {
        /* Print debug message */
        /* If this message is seen, it means that DSM item is not
        ** properly freed.
        */
        M1X_MSG( MUX, LEGACY_ERROR,
          "Odd data_ptr");

        dsm_item_ptr->data_ptr --;
      }


      /* Read the first 4 bytes, it should contain the MuxPDU header */
      pdu5_data_ptr = dec_temp_sdu_data_ptr + dec_sch_ob_addr - DEC_SCH_RAM_BASE;
      memscpy(dsm_item_ptr->data_ptr,
              4,
              pdu5_data_ptr,
              4); /* copy 4 bytes */
      pdu5_data_ptr = pdu5_data_ptr + 2;

      /* extract the sr_id */
      sr_id = (dsm_item_ptr->data_ptr[offset] & 0xe0) >> 5;
      length_ind = (dsm_item_ptr->data_ptr[offset] & 0x0c) >> 2;

      if ( sr_id == 0 || sr_id == 7 || length_ind == 3 )
      {
        /*
        ** If SRID field is '000' or '111', treat this mux PUD as
        ** fill pdu and skip it till the end of this data block.
        ** Notice that '000' is reserved,
        ** if BS sends it, we skip as if it is a fill PDU.
        */

        pos = max_pos;
        length = 0;
      }
      else /* Not a fill PDU */
      {
        if (length_ind == 0)
        {
          /*
          ** Treat these cases as no length feild. '11' is
          ** also reserved. We treat this case as no length.
          ** According to the MAC layer, when the length field is 0,
          ** the whole SDU( excluding the CRC, encoder tail bits) is
          ** 1 PDU. So the length of PDU is surely bigger than 2 bytes
          */

          length = max_pos - pos;  /* length of the PDU to read */

        }
        else
        {
          /* extract the length field */
          length = b_unpackw(dsm_item_ptr->data_ptr,
                             offset*8 + 6,
                             length_ind*8);

          /* payload is 2+length*8 bits, plus length field (1 or 2 bytes)
             plus 6 bits. Total length of MuxPDU in bytes can be obtained
             from the following formula */

          /* in this case, the length of PDU is also bigger than 2 bytes */
          length += length_ind + 1;  /* 1 byte header + length field */
        }

        /*
        ** Calculate how many 16 bit word needs to be read. Or
        ** the window in words that covers the data needs to
        ** be read.
        ** We could read two bytes of garbage in the worst case,
        ** with one byte at each end to round the PDU to enven
        ** byte boundary.
        ** Notice that the equation is not simplified for ease of reading.
        ** I will leave the optimizer to reduce it.
        */

        num_words = (pos+length)/2 + (pos+length)%2 - pos/2;

        /*
        ** We've already calculated garbage at the beginning of the
        ** reading window, and stored that in offset.
        ** Now we need to calculate how many bytes of garbage at the
        ** end of the reading window.
        ** the answer can be only 0, or 1.
        ** The following formula says: if the data ends on even word
        ** boundary, there should be no garbage in the end. If the
        ** data ends on odd address, there has to be one byte of
        ** garbage.
        */

        junk_at_end = (pos+length)%2;

        /* starting position of next MuxPDU */
        pos += length;

        /* Need to check if this MuxPDU crosses data block boundary */
        if ((pos <= max_pos) && (num_words > 1)) /* since the length is bigger than 2 bytes */
        {
          /* This MuxPDU is not across the boundary */

          /*
          ** Read data into the first dsm item. We never read more
          ** data than a DSM item can hold
          */
          words_read = MIN(dsm_item_ptr->size/2, num_words);

          if (words_read >= 2)
          {

            /* Since we've pulled out two words, so we need to read 2 less */
            memscpy((dsm_item_ptr->data_ptr +4),
                    (words_read-2)*2,
                    pdu5_data_ptr,
                    (words_read-2)*2);
            pdu5_data_ptr = pdu5_data_ptr + words_read-2;
          }
          else
          {
            M1X_MSG( DEC, LEGACY_FATAL,
                     "DSM Bad s %x,l %d,p %d",
                     dsm_item_ptr->size,
                     length,
                     pos );
          }

          /* Bytes read in this DSM item including garbage */

          dsm_item_ptr->used = words_read*2;

          dsm_item_ptr->app_field = TRUE;    /* Indicates a good PDU */

          /* Head of the dsm item chain is the same as tail now */
          tail = dsm_item_ptr;

          while( words_read < num_words )
          {
            /* Still words to read, we need to allocate new dsm items. */

            if( (tail->pkt_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL ))
                != NULL )
            {
              /* Now we have a new tail */
              tail = tail->pkt_ptr;

              /* Calculate how many words we need to read */
              words_need = MIN(tail->size/2, num_words-words_read);

              if (((dword)tail->data_ptr) % 2 != 0)
              {
                /* This also should not happen */
                M1X_MSG( MUX, LEGACY_ERROR,
                  "Odd data_ptr!");

              /* Adjust data pointer if it is odd */
                tail->data_ptr --;
              }

              /* Continue to read */
              memscpy(tail->data_ptr,
                      2*words_need,
                      pdu5_data_ptr,
                      2*words_need);
              pdu5_data_ptr = pdu5_data_ptr + words_need;

              /* Set used field inlcuding garbage */
              tail->used = words_need*2;

              /* Adjust number of words read */
              words_read += words_need;

              /* Indicates that this dsm item is good */
              tail->app_field = TRUE;

            }
            else
            {
              M1X_MSG( DEC, LEGACY_ERROR, "No DSM" );
              /* No dsm item available */

              break;
            }
          } /* while */

          /* If the words read out from decoder is less then the
          ** number of words expected, there has to be an unexpected
          ** exit from the loop. Clean up is done here.
          */

          if( words_read < num_words )
          {
            /* abnormal exit from the while loop */

            /* Free the chain from the 2nd dsm_item
               because the first item is always there */
            dsm_free_packet( &dsm_item_ptr->pkt_ptr );

            /* Reset fields in the first item */
            dsm_item_ptr->used = 0;
            dsm_item_ptr->app_field = 0;
            dsm_item_ptr->pkt_ptr = NULL;

          }
          else
          {
            /*
            ** Adjust the offset ONLY when the whole PDU is read, to
            ** prevent buffer leak.
            */
            dsm_item_ptr->data_ptr += offset;

            /* When the data_ptr is moved, needs to account for it
            ** by adjusting the size, otherwise data may be written
            ** across buffer boundary.
            */
            dsm_item_ptr->size -= offset;

            /*
            ** Adjust the used field of the first and the last dsm items
            ** to exclude garbage. Notice that in the case of only one
            ** dsm item, head and tail are pointing to the same dsm item.
            */

            dsm_item_ptr->used -= offset;
            tail->used -= junk_at_end;

            pdu_found = TRUE;
          }

          /* We are either found one MUXPDU5 and adjusted pos,
             time to jump out, and return */
          break;

        }
        else
        {
          /* SDU data can't be trusted, discard the whole SDU */
          pos = max_pos;
        }
      } /* not Fill PDU */
    } /* while */
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR, "no DSM" );
  }

  *start_pos = pos;  /* return the current position */
  return pdu_found;  /* return status */

} /* dec_read_mux_pdu5 */


/*===========================================================================

FUNCTION DEC_READ_MUX_PDU5_SCH_DATA

DESCRIPTION
  Read data from the Decoder as Supplemental(Traffic) Channel data (IS95C SCH).
  This function reads MUX_PDU5 into the buffer pointed
  to by the dsm_item.
DEPENDENCIES
  Dec_tc_init() should have already been called.
  This function should not be called if a frame is bad and there is no LTU
  ( 1X, 2X and turbo case ).

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define DEC_SCH_MAX_FWD_PDU  8
void dec_read_mux_pdu5_sch_data
(
  dec_sch_cfg_type* sch_rec_ptr,   /* Pointer to rxc_sch record pointer */
  dsm_item_type**   dsm_item_ptr,  /* array of dsm_item pointers */
  byte*             mux_pdu_count, /* return the number of MUX PDU read */
  word              ltu_crc,       /* LTU crc from the decoder status */
  byte*             good_ltu       /* return value of number of good ltus */
)
{
  uint8 sch_ob_param_index = 0;
  word dec_sch_size =  0;
  byte dsm_item_index=0;
  byte cnt;
  word mask;
  word sdu_start_pos = 0;   /* Byte offset of the start point for the next MuxPDU */
  word curr_pos;            /* Keep the current position of parsed data block */
  byte pdu_cnt = 0;         /* Keep track of the number of MuxPDU read */
  byte good_ltu_cnt = 0;    /* Counter for good LTUs */
  volatile uint32* dem_buf_ptr;
  uint32 buff_offset = 0;
  uint32 decob_offset = 0;
  uint32 data_buff_limit = 0;
  uint32 data_buff_index = 0;

  /*------------------------------------------------------------------*/

  /* --------------------------------------------------------------------
                      NULL CHECK THE SCH PTR
     -------------------------------------------------------------------- */
  if(sch_rec_ptr == NULL)
  {
    M1X_MSG( DEC, LEGACY_FATAL,
             "dec_read_mux_pdu5_sch_data: sch_rec_ptr is NULL!" );
    *good_ltu = 0;
    *mux_pdu_count = 0;
    return;
  }

  /* --------------------------------------------------------------------
               RANGE CHECK THE PARAM INDEXES DERIVED FROM RC AND RATE
     -------------------------------------------------------------------- */
  sch_ob_param_index = DEC_SCH_RC_INDEX(sch_rec_ptr->rc);

  if( (sch_ob_param_index >= NUM_SCH_RC) ||
      ( sch_rec_ptr->sch_rate >= NUM_SCH_RATES ) )
  {
    M1X_MSG( DEC, LEGACY_FATAL,
             "dec_read_mux_pdu5_sch_data: Index out of bounds! [%d] [%d]",
             sch_ob_param_index,
             (uint32) sch_rec_ptr->sch_rate );
    *good_ltu = 0;
    *mux_pdu_count = 0;
    return;
  }

  /* --------------------------------------------------------------------
                      DETERMINE SCH DATA SIZE
     -------------------------------------------------------------------- */
  dec_sch_size = dec_sch_ob_params[sch_ob_param_index][sch_rec_ptr->sch_rate];

  /* Calculate how many information bits in the SDU */
  if (sch_rec_ptr->sch_rate == DEC_SCH_RATE_1X)
  {
    /* For 1X case, there are 12 bits frame CRC and 8 bits tail */
    dec_sch_size = ((dec_sch_size*8) - (DEC_SCH_SIZ_1X_CRC + DEC_SCH_SIZ_TAIL ))/ 8;
  }
  else
  {
    /* For 2X and up, there are 16 bits frame CRC and 8 bits tail */
    dec_sch_size =  ((dec_sch_size*8) - (DEC_SCH_SIZ_2X_CRC + DEC_SCH_SIZ_TAIL ))/ 8;
  }

  /* --------------------------------------------------------------------
            READ SCH DATA FROM DEM_BUFF (CONV) or TDEC (TURBO)
     -------------------------------------------------------------------- */

  if(sch_rec_ptr->code_type == DEC_TURBO)
  {
    /* ----------------------------- SCH Turbo ----------------------------- */
    /* Get Demback address */
    dem_buf_ptr = dec_get_demback_ptr( DEC_SCH_TURBO_CH );

    buff_offset = DEC_BUFF_STATUS_OFFSET;
  }
  else
  {
    /* ------------------------- SCH Convolutional ------------------------- */
    /* Get Demback address */
    dem_buf_ptr = dec_get_demback_ptr( DEC_SCH_CONV_CH );

    /* ----------------------------------------------------------------
       Determine dem_buff offsets based on RC.
       Determine the size of the SCH data expected in the dem_buff.
       ---------------------------------------------------------------- */
    switch( sch_rec_ptr->rc )
    {
      case DEC_RC3:
      case DEC_RC4:
        decob_offset = DEC_SCH_FULL_OB_ADDR_RC3_RC4;
        break;

      #ifdef FEATURE_IS2000_1X_ADV
      case DEC_RC11:
        decob_offset = DEC_SCH_FULL_OB_ADDR_RC11;
        break;
      #endif /* FEATURE_IS2000_1X_ADV */

      case DEC_RC5:
        decob_offset = DEC_SCH_FULL_OB_ADDR_RC5;
        break;

      default:
        M1X_MSG( DEC, LEGACY_FATAL,
                 "dec_read_mux_pdu5_sch_data: Bad Radio Config %d",
                 (uint32) sch_rec_ptr->rc );
        *mux_pdu_count = 0;
        return;
    }

    buff_offset = ( decob_offset / DEC_BUFF_WORD_ALIGNMENT )
                                                  + DEC_BUFF_STATUS_OFFSET;
  }

  data_buff_limit = ( dec_sch_size + 3 ) / DEC_BUFF_WORD_ALIGNMENT;

  if( dem_buf_ptr )
  {
    /* Read out SDU data into a temporary array */
    while(data_buff_index < data_buff_limit)
    {
      dec_temp_sdu_data_array[data_buff_index] =
                               dem_buf_ptr[data_buff_index + buff_offset];
      data_buff_index++;
    }
  }
  else
  {
    M1X_MSG( DEC, LEGACY_ERROR, "SCH DEMBACK ptr is NULL!" );
  }

  /* --------------------------------------------------------------------
                  PROCESS MUXPDU5 DATA STORED LOCALLY
     -------------------------------------------------------------------- */

  /*
     Oh , MuxPDU 5!
     Here is the assumption we have:
     1. There is already 8 dsm items allocated. If a MuxPDU 5 cannot be
        contained within one dsm item, additional dsm items will be
        allocated and chained.
  */

  curr_pos = sdu_start_pos;  /* Initialize current position */

  dec_temp_sdu_data_ptr = (uint16 *) dec_temp_sdu_data_array;

  if( sch_rec_ptr->num_ltu == 0 )
  {
    /*
       This covers the following cases:
       + Convolutional - 1X & 2X ( No LTU )
       + Turbo - 2X and up (No LTU)
    */

    for (cnt=0; cnt< DEC_SCH_MAX_FWD_PDU; cnt++)
    {
      /* Keep reading till max number of MuxPDU allowed by standard */
      if( dec_read_mux_pdu5(&curr_pos,
                        dsm_item_ptr[dsm_item_index],
                        dec_sch_size)
        )
      {
        /*
        ** Function returns TRUE, there might still be MUXPDU 5 in the
        ** data block. Move to the nex dsm item pointer and read again.
        */
        dsm_item_index++;
      }
      else
      {
        /* There is no more MuxPDU in this SDU */
        break;
      }

    }
  }
  else /* This covers LTU != 0 cases */
  {
    /*
    ** If LTU# !=0, MuxPDU header cannot be interpret unless LTU CRC passes
    */

    /* Initialize the mask for crc checking */
    mask = 0x80 >> (8 - sch_rec_ptr->num_ltu);

    for (cnt=0;
         cnt<sch_rec_ptr->num_ltu && dsm_item_index < DEC_SCH_MAX_FWD_PDU;
         cnt++)
    {
      /* Find out whether this LTU is good or bad. Skip bad ones */
      if (mask & ltu_crc)
      {
        /* Good LTU, keep going */
        good_ltu_cnt++;

        /* Set up the start point for this LTU */
        sdu_start_pos =  sch_rec_ptr->ltu_len/8 * cnt;
        curr_pos = sdu_start_pos;


        for (pdu_cnt=0;
             pdu_cnt < DEC_SCH_MAX_FWD_PDU && dsm_item_index < DEC_SCH_MAX_FWD_PDU;
             pdu_cnt++ )
        {
          /* Notice that there are two bytes of CRC at the end of each
          ** LTU. They should not be read out from decoder. So deduct
          ** 2 from the limit setting.
          */

          if (dec_read_mux_pdu5(&curr_pos,
                            dsm_item_ptr[dsm_item_index],
                            sdu_start_pos + sch_rec_ptr->ltu_len/8 - 2)
              )
          {
            /*
            ** There is a MuxPDU 5 found within this LTU, adjust the pointer
            ** and try to find the next one.
            */

            dsm_item_index++;

          }
          else
          {
            /*
            ** There is no more MuxPDU 5 in this LTU. Jump out for the next LTU
            */

            break;
          }
        } /* for */
      } /* if (mask & ltu_crc) */

      mask >>=1;  /* Shift the CRC mask */

    }   /* for (cnt=0; cnt<sch_rec_ptr->num_ltu; cnt++) */
  } /* if( sch_rec_ptr->num_ltu == 0 ) */

  *good_ltu = good_ltu_cnt;        /* Good LTU if convolutional and has LTUs */
  *mux_pdu_count = dsm_item_index; /* Total number of mux pdu's */

} /* dec_read_mux_pdu5_sch_data */
#endif /* FEATURE_IS2000_REL_A */


/*===========================================================================

FUNCTION DEC_CONFIG_DEMBACK_MODE

DESCRIPTION
  Update FW with the correct demback mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void dec_config_demback_mode( dec_clk_mode_type dec_clk_mode )
{
  /* Configure demback mode... */
  switch(dec_clk_mode)
  {
    case DEC_CLK_DISABLED:
      // Do nothing
      break;

    case DEC_CLK_SYNC:
      /* muxmdsp_set_demback_mode( FALSE, FALSE );
      ** This will happen in rxcsync.c based on HW/SW demback needs
      */
      break;

    case DEC_CLK_IDLE:
      /* muxmdsp_set_demback_mode( FALSE, TRUE );
      ** This will happen in rxcpaging.c based on HW/SW demback needs
      */
      break;

    case DEC_CLK_IDLE_CSFB:
    case DEC_CLK_VOICE_TRAFFIC:
    case DEC_CLK_DATA_TRAFFIC:
    case DEC_CLK_ENABLE_ALL:
      muxmdsp_set_demback_mode( TRUE, TRUE );
      break;

    default:
      M1X_MSG( DEC, LEGACY_ERROR,
        "Invalid Dec Clk mode (%d)",
        dec_clk_mode);
      break;
  }

} /* dec_config_demback_mode */

/*===========================================================================

FUNCTION DEC_CONFIG_CLK_RESOURCES

DESCRIPTION
  Enable/Disable the appropriate decoder and TD clocks based on the
  provided mode, as well as update FW with the correct demback mode.

DEPENDENCIES
  DEC clock regimes should have been initialized.
  TURBO DEC clock regimes should have been initialized.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dec_config_clk_resources
(
  dec_clk_mode_type dec_clk_mode
)
{
  /*------------------------------------------------------------------*/

  M1X_MSG( DEC, LEGACY_MED, "dec_clk_mode: %d", dec_clk_mode );

  /* Configure clocks... */
  switch(dec_clk_mode)
  {
    case DEC_CLK_DISABLED:
      m1x_rm_set_state( M1X_RM_RX, M1X_RM_1X_INACTIVE );
      break;

    case DEC_CLK_SYNC:
      m1x_rm_set_state( M1X_RM_RX, M1X_RM_1X_IDLE );
      break;

    case DEC_CLK_IDLE:
    case DEC_CLK_IDLE_CSFB:
      m1x_rm_set_state( M1X_RM_RX, M1X_RM_1X_IDLE );
      break;

    case DEC_CLK_VOICE_TRAFFIC:
      m1x_rm_set_state( M1X_RM_RX, M1X_RM_1X_VOICE );
      break;

    case DEC_CLK_DATA_TRAFFIC:
      m1x_rm_set_state( M1X_RM_RX, M1X_RM_1X_DATA );
      break;

    case DEC_CLK_ENABLE_ALL:
      m1x_rm_set_state( M1X_RM_RX, M1X_RM_1X_ALL );
      break;

    default:
      M1X_MSG( DEC, LEGACY_ERROR,
               "Invalid Dec Clk mode (%d)",
               dec_clk_mode );
      break;
  }

    /* Configure demback mode */
    dec_config_demback_mode(dec_clk_mode);
} /* dec_config_clk_resources */

