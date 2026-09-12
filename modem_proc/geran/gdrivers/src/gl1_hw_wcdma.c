/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
   This file contains the wcdma calls from the gl1_hw.c.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_wcdma.c#5 $
$DateTime: 2021/02/07 22:09:36 $


when       who      what, where, why
--------   ---      ----------------------------------------------------------
05/02/20   sal      CR2870865 Populate source_sub_id during G2W RF enter ind.
24/07/20   sal      CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
16/03/18   ksb      CR2207319 GL1 fix for KW errors 
20/02/18   shm      Change Request 2192580 : KW P1 errors on TA.3.0 fix 
29/04/16   dp       CR1000807 Remove useless check from G2T gap-calc code
15/12/17   km       CR1049586 Send G2X cleanup through ASYNC to avoid executing before G2X startup
05/12/17   km       CR987605 G2X immediate cleanup
29/10/17   km       CR2133917 GL1 support for TA.3.0 L+L
28/08/15   cja      CR892470 G2X to cope with overlapping init/deinit/init 
26/08/15   br       CR896169 In G2T dedicated mode reset irat state machine to null only after reciving the search results.
17/08/15   cja      CR890396 In G2T dedicated abort do cleanup before setting state to NULL
29/07/15   dp/dv    CR878618: Changes to wait for rf exit confirmation in gl1 for X2W srch
03/07/15   cja      CR866307 G2T and G2L in QTA gap
30/06/15   ws       CR852519 - Resolve GL1 enum conversion compile warnings
25/06/15   zf       CR861092: Do not schedule NULL2X IRAT in transfer mode
11/06/15   zf       CR847878: Using PRE_SRCH_REQ and CNF for G2W
02/06/15   cja      CR844096 RF meas gsm parameters to use xtoy param type
20/05/15   aga      CR840984 NULL2X IRAT changes for G2W IRAT
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
06/05/15   aga      CR833610 dual data space gl1_wcdma_rxlm_buf_idx and gl1_hw_g2w_rf_enter_called_last
24/04/15   cja      CR800657 QTA gap for G2W IRAT
09/04/15   dp       CR819956 Thor 2.x RF API change, G2W use MSGR
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
24/02/15   js       CR749012: GL1 changes for LTE removal
20/02/15   js       CR693771: W+T removal. Resolving compilation errors
20/01/14   zf       CR772571: Build G2W RF scripts one frame earlier
17/02/15   br       CR777272 Parallelize the execution of RF RX script from RF task and G2X start up from GSTMR IST.
23/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
15/12/14   ws       CR771244 GL1: Compile warnings for LTE,1X,GERAN build flavour
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
17/11/14   dp       CR700052 Move g2w drivers into W filetree
07/10/14   sjv      CR734254  G2L rf devices retrieval for primary and diversity
29/09/14   cgc      CR727137 LTE api additions add gl1_hw_get_lte_irat_rf_device()
26/09/14   sjv      CR731259 Correct device id for G2X IRAT
06/05/14   dv       CR660054 RXLM enhancement to ensure that no tech de-allocates another techs buffers
11/07/14   dp       CR674070 - Use new G2W drivers interface (phase 1)
04/07/14   cs       CR688003 GRM functions moved to gpl1_grm_intf.c
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
19/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
07/05/14   cja      CR657218 In dedicated mode delay G2W gap start by 1250qs
03/10/14    sc      CR628645 : Print F3 after successful rxlm buffer allocation
05/03/14   cja      CR619911 For G2X set source param for rf meas enter/exit
03/03/14   cja      CR622705 Add rxlm_handles parameter to G2L rf meas exit
10/02/14   cja      CR612218 For G2L add meas enter and meas exit for LTE RF
02/12/14    js      CR595142 :  T+G compilation errors removed
17/12/13   cja      CR590767 Bolt G2W - add gap start time to g2w startup
26/11/13   sai      CR572198: X2W mode cfg cmd issued after startup/cleanup scripts built,move G2W gap further into GSM frame
                    to avoid overrun.
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13   cja      CR455527 Remove featurisation from files in API directory
14/10/13   pa       CR536820:Added GL1 Msgr IF Task.
16/09/13   cs       CR503042 Updated XO RGS handling for Triton
03/09/13   npt      CR524770 - GL1 changes to support RFLM FED
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
08/13/13   sbr      CR527070 fix TDS related compile errors
08/08/13   dv       CR525765 Changes for DSDS T/G IRAT bringup
05/08/13   cja      CR514498 Only increment current_buffer_mod_3 once per frame, plus for T2G async
31/07/13   cja      CR498800 Use TRM to get IRAT RF ID
10/06/13   cja      CR498279 Do not increment current_buffer_mod_3 twice in G2W only frame
19/06/13   cs       Major Triton TSTS Syncup
18/06/13   cja      CR498800 Use TRM to get IRAT RF ID
10/06/13   cja      CR498279 Do not increment current_buffer_mod_3 twice in G2W only frame
28/05/13   cja      CR492493 Only initialise G2W driver when W neighbour in list
22/05/13   ap       CR485912: Revised GFW state transitions
21/05/13   pg       CR486329: send X2G commands before calling RF to populate CCS events
20/05/13   pg       CR484522: service the MSGR Qs whilst waiting to start G2X
17/05/13   cs       TSTS Updates
24/04/13   pg       CR459509: Reduce the RF Task complete polling period for G2X
19/04/13   cja      CR477380 Ensure mutex lock/unlock always paired.
29/01/13   cja      CR446356 Turn off FEATURE_GSM_RFA_TASK_DEBUG
10/12/12   cja      CR411308 Dime T2G changes (msg router to GFW)
26/11/12    pg      CR423852: G2W startup can be scheduled in next frame on Dime modem
22/11/12   cja      CR414098 Tidy up LM allocation/deallocation
31/10/12   pg       CR409233: updates for G2W on Dime modem
21/10/12   dv       CR397922: Fix for 6.2.2.3a G2T TC
21/09/12   cja      CR401869 Make G2T cleanup margin bigger as FW Rx Beta has increased.
19/09/12   rc       CR401496 - Back out fix for CR382976,398725 & 400225
10/09/12   cja      CR382976 Correct TD timing for G2T dedicated gap
03/08/12    og      CR383632. Don't request a tuneback to GSM when the G2L startup request
                    is not sent to LTE L1.
04/07/12   pa       CR376123: G2T ensure GL1 shoudl not send extra cleanup while leaving GSM mode.
03/07/12   pa       CR371273: Don't schedule G2T if TS reconfigure before idle frame.
22/06/12   pa       CR371580: Delay TBF release during G2T activity.
18/06/12   pa       CR371123: Remove G2X gap event. Fixed in CR370133.
18/06/12   pa       CR371123: In G2T sdcch, allocate max 15G frames and correct gap calc.
13/06/12   dv       CR370133:Ensure that pwr monitors resumed after G2T Gap
12/06/12   dv       CR369770:Ensure no pwr measures during G2T gap in xfer
08/06/12   pa       GPLT: Replaced FEATURE_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
06/06/12   cja      CR368070 Correct time for G2X gap schedule
29/05/12   cja      CR365748 Resume power measurements on g2x clean-up
25/05/12   cja      CR363431 Suspend G power measurements during T measurements
18/05/12   pa       CR361192,CR362129,CR362562: Ensure UE doesn't sleep during G2T MEAS
12/04/12   pa       CR351597: G2T: UE goes into sleep mode during cleanup
14/03/12   pg       Add support for local HWIO macros (msm.h deprecated)
14/03/12   pg       Add support for DAL interrupt controller (tramp deprecated)
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
12/10/11   ws       CR312154 - Featurise remaining clkrgm API calls uder FEATURE_MCPM
04/10/11   cja      CR310265 Disable WFW after suspend WFW
20/08/11   cja      Fix compiler error.
16/08/11   cja      CR301096 Pass index to RF driver for startup and cleanup buffers
16/08/11   cja      CR301307 Changes following code review.
12/08/11   cja      CR301307 Correct Rx path for W RxLM in G2W
08/06/11   cja      Nikel G2W changes (so WFW does startup)
12/05/11   cja      Nikel remove manual setting of W freq correction
03/05/11   cja      Nikel W2G use cycle swallower
28/04/11   cja      Add Nikel freq correction in GFW
26/04/11   ws       NikeL CMI change to support wl1api.h
20/04/11   cja      Nikel GFW interface changes
05/04/11   cja      L1 I/F change for Nikel IRAT
30/03/11   cja      Initial Nikel IRAT
21/03/11   cja      Initial changes for Nikel
18/03/11   ws       Added Initial MCPM updates
02/02/11   ws       Changes to add rfwcdma_core_convert_chan_to_band()
01/02/11   ws       Commented out HWIO front end settings for gl1_hw_g2w_set_frontend_rot() until IRAT BU
12/01/11   ws       Fixed compile errors for XO support on QDSP6
29/11/10   og       Adding G2L support.
20/07/11   pg       CR296954: Remove write to MDSP_xoMndFreqGain for G2W search
21/06/11   pg       CR282927: Remove write to MDSP_xoAdjAccum for G2W search
29/03/11   pg       CR279078: Update to WCDMA RxF rotator calc when supporting SWC
05/01/11   pg       Add feature FEATURE_WCDMA_G2W_SWC to support SWC based clock control
                    for WCDMA on Pegasus
21/12/10   nt       DSDS for C+G builds ensure that no WCDMA clocks are enabled
06/12/10   cs       Remove possibility of arithmetic overflow in CM rot calc
14/10/10   pg       Fix compiler warnings
13/10/10   cja      Fix LINT warnings
07/10/10   ky       254466:Removed fw unused shared variables from the sw code
14/09/10   og       CR255453 Setup the serving cell arfcn and band before WCDMA ID search.
19/07/10   ws       Added Missing msm.h to ensure MDSP_FW_ANT_CLK_CFG gets updated
13/07/10   pg       CR235492: Ensure MDSP_FW_ANT_CLK_CFG gets set via non-sbi write
13/07/10   cs       CR246313 Update the W DL band conversion constants
08/06/10   cja      Remove BBRx clock gating RF API for MDSP QDSP6.
07/06/10   cja      Add W measurement start/stop RF API for BB Rx clock gating.
20/05/10   dv       Osamas 9k irat updates.
20/05/10   og       9k irat updates.
17/03/10   og       Further x2g and g2x updates.
16/03/10   scm      Call fws_app_disable(FW_APP_WCDMA) on GSM shutdown.
01/03/10   og       g2x and x2g updates.
23/02/10   ws       Integrate G2W IRAT driver from MDM8200
11/24/09   tws      Use HWIO_PHYS macro to pass physical address to MDSP.
13/11/09   ky       Change the reg address from virtual to physical in Q6 mdsp_nonsbi_write function call
04/12/09   cs       Add changes for SCMM iRAT to program MND for XO this will
                    be updated when new clk regime API is available
06/11/09   ws       Added FEATURE_CDMA1X around rf1x_core_rf.h
09/10/22   tjw      include rfm_meas for fn prototypes
17/09/09   nf       Removed unused APIs
09/07/09   ws       Added FEATURE_GSM_GPRS_POSEIDON to protect unsupported API
12/06/09   ip       CR181369 switching of slpfast clock sclk inaccurate cal
07/04/09   cs/hy    Add support for the clkregime API for setting MND counters for XO
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
26/03/09   ws       Added CLKREGIM_HAS_GET_MDSP_ACLK_VAL from taxis modem
13/03/09   og       FEATURE_XO updates
02/02/09   cs       Fix the LCU build for previous updates
25/01/09   cs       Add latest Mustang G2W changes from rel11 dev branch
02/12/08   cs       Add in new split gl1_hw_clk_ctl.h from gl1_hw.h
21/08/08   og       Mustang bring-up changes.
28/07/08   ws       Added T_MSM8650B to setup MDSP_REG_mdspfwantclkcfg for correct Gto W operation
10/03/08   cs       Reintroduce some useful debug F3 removed in API update
11/01/08   cs       CR134941 - Add in some 6800-Dora specific featurisation for
                    clock control that is not supported on that target
05/12/07   cs       Featurise the MDSP_REG_mdspfwantclkcfg changes as this reg
                    write is required for Koel/Hawk, otherwise RSSI=0!
19/11/07   ws       CR 123029 - Removed clk_regime_set_analog_clk()
07/11/07   ws       Removed MDSP_REG_mdspfwantclkcfg for 7600, since this got
                    accidentally introduced.
21/09/07   cs/ip    CR117884 reintroduced
01/08/07   cs       Fix EDGE only build featurisation for 7205/7500-90 etc
06/03/07   agv      Removed MDSP_REG_mdspfwantclkcfg for 7600 as now set by mDSP.
06/01/06   ws       Removed T_MSM6280 switch around clk_regime_set_analog_clk() CR94943
05/18/06   gfr      Support for clock regime resource manager.
05/17/06   pp       Support for unified sleep task.
01/13/06   gfr      Support for new RFM API (FEATURE_RFM_INTERFACE)
12/19/05   gfr      Move the clock enable from gl1_hw_wcdma_inact to
                    gl1_msg_cleanup_wcdma_meas since this function is called
                    from L1 as well.
12/05/05   gfr      Use rfm_build_gtow_scripts
11/28/05   gfr      Enable g2w clocks during inact
09/22/05   gfr      Removed FEATURE_SBI_MDSP_CONTROLLER, fixed midband freqs
08/19/05   gfr      QLint cleanup
06/10/05   gfr      Mainline FEATURE_HARDWARE_CONTROLLED_CLK_REGIMES,
                    use generic clk_regimes APIs, support multiband UMTS
03/16/05   yh       Lint
02/04/05   gfr      Use g2w driver for programming sample ram
01/24/05   yhong    Cleanup for W2G build
12/06/04   gfr      Featurize the mDSP SBI controller
11/04/04   gfr      Commented out more code to get Raven to compile.
10/28/04   gfr      Clean up init functions, add support for Raven (6275)
10/04/04   gfr      Put setting MDSP_REG_mdspgsmmodectl back here since hw.c
                    is run before mdsp is initialized for nonsbi writing.
10/01/04   gfr      Moved setting MDSP_REG_mdspgsmmodectl into hw.c
08/10/04   yhong    Initial version. Code contains only wcdma calls from gl1_hw.c
============================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif


#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hwi.h"
#include "gl1_msg_g.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "msg.h"
#include "mcs_hwio.h"
#include "gpl1_dual_sim.h"

#include "gpl1_grm_intf.h"

#include "gfw_sw_intf.h"

#include "rfm.h"
#include "rfwcdma_core.h"

#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || \
    defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
#include "wl1x2wsrchapi.h"
#endif

#include "rfmeas_types.h"
#include "rfm_meas.h"


#include "gl1_mutex.h"
#include "fws.h"

#include "gl1_msg_g.h"
#include "gl1_msg_wcdma_meas.h"
#include "l1_sc_irat.h"
#include "l1_sc_lte_irat.h"


#include "rxlm_intf.h"
#include "txlm_intf.h"

#ifdef FEATURE_GSM_TDS
#include "vstmr_tds.h"
#include "gtmrs.h"  //for reference to SUB_MOD
#include "l1_task.h"
#endif

#include "mdsp_cmd.h"

#ifdef  FEATURE_GSM_TO_LTE
#include "lte_l1_types.h"
#endif

#include "gtmrs_g.h"
#include "l1_drx.h"

#include "gl1_hw_tcxo_mgr3.h"

#if (defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG))
extern volatile gap_info_debug_type gap_info_debug;
#endif

/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/
extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

extern int32 l1_get_band_frequency_mhz(gas_id_t gas_id);
extern void l1_set_band_frequency_mhz(int32 band_frequency_mhz,gas_id_t gas_id);

#ifdef  FEATURE_GSM_TO_LTE
/* LTE functions for G2L RF meas enter/exit */
extern boolean lte_ml1_common_band_get_band_from_dl_earfcn(lte_earfcn_t dl_earfcn, sys_sband_lte_e_type  * band_ptr);
extern sys_band_class_e_type lte_ml1_convert_lte_band_to_sys_band(sys_sband_lte_e_type lte_band);
#endif

/* Mid band frequencies for WCDMA, may not be exactly correct */
#define MID_FREQ_WCDMA_IMT_BAND   2140
#define MID_FREQ_WCDMA_PCS_BAND   1960
#define MID_FREQ_WCDMA_DCS_BAND   1843
#define MID_FREQ_WCDMA_AWS_BAND   2133
#define MID_FREQ_WCDMA_CLR_BAND    882
#define MID_FREQ_WCDMA_GSM_BAND    943
#define MID_FREQ_WCDMA_BC9_BAND   1862

/* Inverse of mid band frequencies (2^52 / freq in Hz) */
#define INV_MID_FREQ_WCDMA_IMT_BAND   2104486
#define INV_MID_FREQ_WCDMA_PCS_BAND   2297755
#define INV_MID_FREQ_WCDMA_DCS_BAND   2443624
#define INV_MID_FREQ_WCDMA_AWS_BAND   2111392
#define INV_MID_FREQ_WCDMA_CLR_BAND   5106122
#define INV_MID_FREQ_WCDMA_GSM_BAND   4775821
#define INV_MID_FREQ_WCDMA_BC9_BAND   2418689

#define GL1_HW_G2W_WAIT_RF_MAX_DELAY  2500 //us
#define GL1_HW_G2W_WAIT_RF_DELAY_STEP 250  //us
#define GL1_HW_G2W_WAIT_RF_MAX_START  4000 //qs

/*===========================================================================

 LOCAL STORAGE

===========================================================================*/
/* RF Link Manager buffer index */
static uint32 gl1_wcdma_rxlm_buf_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( MDSP_RXLM_BUF_IDX_INVALID) };
static uint32 gl1_lte_rxlm_buf0_idx[NUM_GERAN_DATA_SPACES]  = { INITIAL_VALUE( MDSP_RXLM_BUF_IDX_INVALID) };
static uint32 gl1_lte_rxlm_buf1_idx[NUM_GERAN_DATA_SPACES]  = { INITIAL_VALUE( MDSP_RXLM_BUF_IDX_INVALID) };

#ifdef FEATURE_GSM_TDS
static uint32 gl1_tds_rxlm_buf0_idx = MDSP_RXLM_BUF_IDX_INVALID;
extern uint32 gsm_clks_to_tds_cx8_clks( uint32 x );

boolean l1_g2t_delay_for_fw_cleanup = FALSE;
uint8   l1_g2t_delay_for_fw_startup = 0;
extern int     l1_g2t_delay_tbf_rel;
extern boolean l1_g2t_issue_tbf_rel;

static uint32 gl1_startup_decrement_time_offset[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
uint32 gl1_cleanup_decrement_time_offset[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
boolean mdsp_decrement_startup_time_offset_flag[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
boolean mdsp_decrement_cleanup_time_offset_flag[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
#endif

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/* used to store tune time parameters for G2W searches */
static wl1_x2w_rf_timing_params_struct_type g2w_rf_timing_params[NUM_GERAN_DATA_SPACES];

static boolean gl1_hw_g2w_rf_enter_called_last[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
static boolean gl1_hw_g2w_using_rf[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
#else
* ----------------------------------------------------------------------- */
/* Store whether we need to preserve WCDMA ref count during sleep.         */
/* ----------------------------------------------------------------------- */
static boolean wcdma_srch_is_on = FALSE;
#endif

/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/


/*===========================================================================

                                FUNCTIONS

===========================================================================*/

#ifndef FEATURE_WPLT
/*===========================================================================

FUNCTION  gl1_hw_wcdma_init

DESCRIPTION
  This function performs any WCDMA specific mDSP initialization.  It should
  be called after loading the firmware.

===========================================================================*/
void gl1_hw_wcdma_init (gas_id_t gas_id)
{
#if (defined(FEATURE_INTERRAT_HANDOVER_GTOW) || \
     defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW) || \
     defined(FEATURE_INTERRAT_HANDOVER_WTOG) || \
     defined(FEATURE_INTERRAT_CELL_RESELECTION_WTOG))

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* initialize G2W driver */
  gl1_hw_allocate_wcdma_rxlm_buf(gas_id);
  gl1_msg_wcdma_srchdrv_init(gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id,gas_id), gas_id);
#endif
}

#endif /* FEATURE_WPLT */


/*===========================================================================

FUNCTION  gl1_hw_wcdma_inact

DESCRIPTION
  This function performs any cleanup necessary in wcdma when leaving GSM mode.

===========================================================================*/
void gl1_hw_wcdma_inact (gas_id_t gas_id)
{
  #if defined(FEATURE_INTERRAT_HANDOVER_GTOW)            || \
      defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW)
  /* Clean up WCDMA driver */
  gl1_msg_cleanup_wcdma_meas(gas_id);

  #endif
}

#ifdef FEATURE_WCDMA
/*===========================================================================
FUNCTION gl1_hw_get_wcdma_freq

DESCRIPTION
  Get mid frequency in MHz of current WCDMA band.

RETURN

===========================================================================*/
static int32 gl1_hw_get_wcdma_freq( rfcom_wcdma_band_type rf_band, uint32* inv_freq )
{
  int32 wcdma_freq;

  /* Determine the current RF band */
  switch ( rf_band )
  {
    case RFCOM_BAND_IMT:
      wcdma_freq = MID_FREQ_WCDMA_IMT_BAND;
      *inv_freq = INV_MID_FREQ_WCDMA_IMT_BAND;
    break;

    case RFCOM_BAND_1900:
      wcdma_freq = MID_FREQ_WCDMA_PCS_BAND;
      *inv_freq = INV_MID_FREQ_WCDMA_PCS_BAND;
    break;

    case RFCOM_BAND_BC3:
      wcdma_freq = MID_FREQ_WCDMA_DCS_BAND;
      *inv_freq = INV_MID_FREQ_WCDMA_DCS_BAND;
    break;

    case RFCOM_BAND_BC4:
      wcdma_freq = MID_FREQ_WCDMA_AWS_BAND;
      *inv_freq = INV_MID_FREQ_WCDMA_AWS_BAND;
    break;

    case RFCOM_BAND_800:
      wcdma_freq = MID_FREQ_WCDMA_CLR_BAND;
      *inv_freq = INV_MID_FREQ_WCDMA_CLR_BAND;
    break;

    case RFCOM_BAND_BC8:
      wcdma_freq = MID_FREQ_WCDMA_GSM_BAND;
      *inv_freq = INV_MID_FREQ_WCDMA_GSM_BAND;
    break;

    case RFCOM_BAND_BC9:
      wcdma_freq = MID_FREQ_WCDMA_BC9_BAND;
      *inv_freq = INV_MID_FREQ_WCDMA_BC9_BAND;
    break;

    default:
      wcdma_freq = MID_FREQ_WCDMA_IMT_BAND;
      *inv_freq = INV_MID_FREQ_WCDMA_IMT_BAND;

      MSG_GERAN_ERROR_2( "Unknown UMTS band %d wdcma_freq %d",
                 rf_band, wcdma_freq);
    break;
  }

  return ( wcdma_freq );
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_WCDMA
/*===========================================================================

FUNCTION  gl1_hw_gsm_client_requires_resource

DESCRIPTION
  Called by WCDMA when it wants to know if GSM client is active and requires
  normal GSM resources.

DEPENDENCIES
  Only to be used when FEATURE_DUAL_SIM and FEATURE_WCDMA are defined

RETURN VALUE
  Boolean TRUE = GSM is active / FALSE = GSM inactive

SIDE EFFECTS

===========================================================================*/
boolean gl1_hw_gsm_client_requires_resource(gas_id_t gas_id)
{
  return(gl1_msg_get_multi_sim_mode());
}
#endif /* FEATURE_WCDMA */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || \
    defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
/*===========================================================================

FUNCTION  gl1_hw_g2w_wait_rf_buffer_completion

DESCRIPTION
  Called to ensure that all pending GSM RF CCS buffers have been populated in RF Task context
  before calling RF inline to populate G2W startup CCS buffer

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void gl1_hw_g2x_wait_rf_buffer_completion(gas_id_t gas_id)
{
   int16 total_delay = 0;

   /* if GFW commands present in command buffer, send to GFW now and do not wait for G2W commands */
   mdsp_issue_async_msg(GFW_ASYNC_CMD,gas_id);

   /* wait until all outstanding G RF buffers have been populated before allowing G2W startup script
      to be populated */
   while((gl1_hw_rf_check_active_buffers(gas_id) == TRUE)
         && (total_delay < GL1_HW_G2W_WAIT_RF_MAX_DELAY)
         && (gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)   ) < GL1_HW_G2W_WAIT_RF_MAX_START))
   {
     gl1_hw_delay(GL1_HW_G2W_WAIT_RF_DELAY_STEP,gas_id);
     total_delay += GL1_HW_G2W_WAIT_RF_DELAY_STEP;
     /* GL1 task can be waiting for the ISR to release the gl1_local_crit_sect,
        so service the RF Task and FW Async Intf MSGR queues whilst waiting */
   }

   if(total_delay >= GL1_HW_G2W_WAIT_RF_MAX_DELAY)
   {
     MSG_GERAN_ERROR_1_G("G2W RF buffer wait too long: total_delay=%d",total_delay);
   }
   else
   {
     MSG_GERAN_HIGH_1_G("G2W RF buffer wait: total_delay=%d",total_delay);
   }
}

#ifdef FEATURE_GSM_TO_WCDMA_MSGR

/*===========================================================================

FUNCTION  gl1_hw_g2w_send_rf_enter_ind

DESCRIPTION
  This function will populate the rf parameters used for the gap and will send
  an rf enter ind

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_g2w_send_rf_enter_ind(
  /* frequency the search will be on */
  uint16 search_uarfcn,
  /* GSM subscription (for multisim) */
  gas_id_t gas_id)
{
  rfcom_wcdma_band_type W_band;
  ARFCN_T  scell_arfcn;

  /* Pointer to RF common structure */
  rfm_meas_common_param_type *gl1_hw_g2w_rf_param_ptr = gl1_get_irat_rf_structure_ptr(gas_id);

  /* Sanity check - G2W's rf_enter and rf_exit should always be called in an
     alternating pattern. If not, it's time to debug why :) */
  if(gl1_hw_g2w_rf_enter_called_last[gas_id] == TRUE)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("Non-alternating call to rf_enter for G2W", 0, 0, 0);
  }

  W_band = rfwcdma_core_convert_chan_to_band(search_uarfcn);
  gl1_msg_algs_get_arfcn(&scell_arfcn.num, &scell_arfcn.band, gas_id);

  if (l1_sc_x2g_ta_irat_gap(gas_id))
  {
     if ( gl1_hw_qta_gap_active(gas_id))
     {
        /* Use QTA RF structure, source RAT parameters already complete */
        MSG_GERAN_HIGH_1_G("G2W in QTA gap, src tech %d",gl1_hw_g2w_rf_param_ptr->header.source_tech);
     }
     else /* NULL2G gap */
     {
        /* fill in rf parameters */
        rfm_meas_common_init_params(gl1_hw_g2w_rf_param_ptr);
		gl1_hw_g2w_rf_param_ptr->header.source_sub_id = geran_map_gas_id_to_nas_id(gas_id);
        gl1_hw_g2w_rf_param_ptr->script_type = RFM_MEAS_PROGRAM_NULL2X;
        MSG_GERAN_HIGH_0_G("G2W NULL2X gap");
     }
  }
  else
  {
     /* fill in rf parameters */
     rfm_meas_common_init_params(gl1_hw_g2w_rf_param_ptr);

     /* fill in header */
	 gl1_hw_g2w_rf_param_ptr->header.source_sub_id = geran_map_gas_id_to_nas_id(gas_id);
     gl1_hw_g2w_rf_param_ptr->header.source_tech = RFM_EGSM_MODE; /* enum for GSM */
     gl1_hw_g2w_rf_param_ptr->header.num_source_item = 1; /* should be 1 for GSM I think */
     gl1_hw_g2w_rf_param_ptr->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device =
          gl1_hw_get_gsm_rf_id(l1_tsk_buffer[gas_id].client_id, gas_id); /* primary rf device */
     gl1_hw_g2w_rf_param_ptr->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle =
          gl1_hw_get_gsm_rxlm_buf_idx(gas_id); /* source rxlm buffer */
     gl1_hw_g2w_rf_param_ptr->header.source_param_v2[RFM_MEAS_PCELL].band = gl1_hw_arfcn_band_to_rfcom_gsm_band(scell_arfcn.band); /* source band */
     /* TODO fill out RxD params */
     /* here's the sample W code:
     if (rxd_is_active(wsub_id))
     {
       rf_info->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = wl1_trm_get_secondary_rf_device(wsub_id);
       rf_info->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = wl1_xlm_get_rxlm_buf(WL1_XLM_RX_CHAIN1_DIVERSITY, wsub_id);
     }
     */
     gl1_hw_g2w_rf_param_ptr->script_type    = RFM_MEAS_PROGRAM_STARTUP;  /* Anything except NULL2X */
  }
  /* fill in gsm source params */
  /* (TODO maybe fill in more parameters) */
  gl1_hw_g2w_rf_param_ptr->source_param.gsm_params.sub_id = geran_map_gas_id_to_nas_id(gas_id);
  gl1_hw_g2w_rf_param_ptr->scenario_type  = RFM_MEAS_SCENARIO_IRAT;

  MSG_GERAN_HIGH_3_G("G2W: rf_enter_ind: source: device %d rxlm %d band %d",
                     gl1_hw_g2w_rf_param_ptr->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device,
                     gl1_hw_g2w_rf_param_ptr->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle,
                     gl1_hw_g2w_rf_param_ptr->header.source_param_v2[RFM_MEAS_PCELL].band);
  MSG_GERAN_HIGH_3_G("G2W: rf_enter_ind: target: device %d rxlm %d uarfcn %d",
                     gl1_hw_get_wcdma_rf_id(W_band, gas_id),
                     gl1_wcdma_rxlm_buf_idx[gas_id],
                     search_uarfcn);

  l1_send_IRAT_G2W_RF_ENTER_IND(gl1_hw_g2w_rf_param_ptr,
                                search_uarfcn,
                                gl1_wcdma_rxlm_buf_idx[gas_id],
                                gl1_hw_get_wcdma_rf_id(W_band, gas_id),
                                gas_id);

  gl1_hw_g2w_rf_enter_called_last[gas_id] = TRUE;
  gl1_hw_g2w_using_rf[gas_id] = TRUE;
}

/*===========================================================================

FUNCTION  gl1_hw_g2w_send_rf_exit_req

DESCRIPTION
  This function will populate the rf parameters used for the gap and will send
  an rf enter ind

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_g2w_send_rf_exit_req(
  /* set this to TRUE if rf_enter_ind has been sent but the scripts were not
     used for any gap. (No tuning occured when gap was aborted) */
  boolean abort,
  /* GSM subscription (for multisim) */
  gas_id_t gas_id)
{
  /* Sanity check - G2W's rf_enter and rf_exit should always be called in an
     alternating pattern. If not, it's time to debug why :) */
  if(gl1_hw_g2w_rf_enter_called_last[gas_id] == FALSE)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("Non-alternating call to rf_exit for G2W", 0, 0, 0);
  }

  MSG_GERAN_HIGH_1_G("G2W: rf_exit_req: gap_aborted %d", abort);

  l1_send_IRAT_G2W_RF_EXIT_REQ(gl1_get_irat_rf_structure_ptr(gas_id),
                               abort,
                               gas_id);

  gl1_hw_g2w_rf_enter_called_last[gas_id] = FALSE;
}


/*===========================================================================

FUNCTION  gl1_hw_g2w_check_if_using_rf

DESCRIPTION
  This function allows upper layers to query whether G2W is still busy using RF

DEPENDENCIES
  None

RETURN VALUE
  True if rfm_meas_exit has not completed for this gap
  False otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_g2w_check_if_using_rf(
  /* GSM subscription (for multisim) */
  gas_id_t gas_id)
{
  return gl1_hw_g2w_using_rf[gas_id];
}

/*===========================================================================

FUNCTION  gl1_hw_g2w_done_with_rf

DESCRIPTION
  This function should be called when g2w msg layer has completed rfm_meas_exit

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_g2w_set_done_with_rf(
  /* GSM subscription (for multisim) */
  gas_id_t gas_id)
{
  /* Sanity check - G2W's rf_enter should have been called before this */
  if(gl1_hw_g2w_using_rf[gas_id] == FALSE)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("Received call to g2w_done_with_rf when rf wasn't in use", 0, 0, 0);
  }

  MSG_GERAN_HIGH_0_G("G2W: rf_exit_cnf");

  gl1_hw_g2w_using_rf[gas_id] = FALSE;
}


/*===========================================================================

FUNCTION  gl1_hw_g2x_mdsp_start_wcdma_rf_cmd

DESCRIPTION
  This function triggers GL1 to send (just) the startup command to GFW for the
  an upcoming G2X gap.  There should be another call later to do the cleanup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_g2x_mdsp_start_wcdma_rf_cmd(
  /* where the GSM gap will start in the next G frame (in qs) */
  uint16 start_time,
  /* GSM subscription (for multisim) */
  gas_id_t gas_id)
{
  MSG_GERAN_HIGH_2_G("G2W: start time:%d mdsp_ftsm:%d",
                     start_time, mdsp_get_ftsm(gas_id));

  /* Send command to GFW for mutex release */
  mdsp_start_wcdma_rf_cmd(start_time, FALSE, MDSP_RXLM_BUF_IDX_INVALID, gas_id);

  mdsp_issue_async_msg(GFW_ASYNC_CMD, gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_g2x_mdsp_rf_sample_ram_cmd

DESCRIPTION
  This function triggers GL1 to send the startup and cleanup commands to GFW
  for the upcoming sample ram.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_g2x_mdsp_rf_sample_ram_cmd(
  /* where the GSM gap will start in the next G frame (in qs) */
  uint16 start_time,
  /* GSM subscription (for multisim) */
  gas_id_t gas_id)
{
  uint16 stop_time;

  stop_time = start_time + g2w_rf_timing_params[gas_id].startup_time + g2w_rf_timing_params[gas_id].pll_settling_time + MDSP_FILL_SAMPLE_RAM_DURATION;
  /* Send commands to GFW for mutex release and cleanup */
  MSG_GERAN_HIGH_3_G("G2W gap start time %d, stop time %d, cleanup time %d",
                      start_time,
                      stop_time,
                      g2w_rf_timing_params[gas_id].cleanup_time);

  mdsp_rf_sample_ram_cmd(start_time,
                         stop_time,
                         FALSE,
                         gl1_hw_get_gsm_rxlm_buf_idx(gas_id),
                         gl1_wcdma_rxlm_buf_idx[gas_id],
                         gas_id);

  mdsp_issue_async_msg(GFW_ASYNC_CMD, gas_id);

  gl1_hw_schedule_rf_sample_ram(start_time, stop_time, gas_id);
}


/*===========================================================================
FUNCTION  gl1_hw_g2w_send_pre_search_req

DESCRIPTION
  This function triggers GL1 to send the PRE_SEARCH_REQ message to WL1 for an
  upcoming idle ID, idle reconfirm, or dedicated search.  WL1 will build rf
  scripts and start WFW.

DEPENDENCIES
  gl1_hw_g2w_send_rf_enter_ind MUST HAVE BEEN CALLED FIRST before this function

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_g2w_send_pre_search_req(
  /* which subscription (for multisim) */
  gas_id_t gas_id)
{
  /* Sanity check - G2W's rf_enter should be called before this function.
     If not, it's time to debug why :) */
  if(gl1_hw_g2w_rf_enter_called_last[gas_id] == FALSE)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("Can't send PRE_SEARCH_REQ without RF_ENTER_IND first.", 0, 0, 0);
  }

  l1_send_IRAT_G2W_PRE_SRCH_REQ(gl1_get_irat_rf_structure_ptr(gas_id), gas_id);
}

/*===========================================================================
FUNCTION  gl1_hw_g2w_search

DESCRIPTION
  This function triggers GL1 to send the SEARCH_REQ message to WL1 for an
  upcoming idle ID, idle reconfirm, or dedicated search.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_g2w_search(
  /* frequency to search */
  uint16                                     search_uarfcn,
  /* how many frames from now the measurement will take place */
  uint8                                      start_time_frame_offset,
  /* The start time of the G gap in qs from the start of the frame boundary */
  uint16                                     start_time_qs,
  /* search type (idle id, idle reconf, dedicated) */
  wl1_x2w_mode_enum_type                     search_mode,
  /* a union of cell params for the different types of searches
     (can be NULL for IDLE_ID) */
  wl1_x2w_srch_input_union_type              *cell_search_params,
  /* which subscription (for multisim) */
  gas_id_t                                   gas_id)
{
  wl1_x2w_startup_params_struct_type startup_params;

  tcxomgr_vco_info_type vco_info;
  rfcom_wcdma_band_type W_band;
  int32 wcdma_freq, W_freq_err_q6_hz;
  uint32 inv_dlo_freq;
  uint32 g2w_start_time_xo;
  ARFCN_T  scell_arfcn;

  /* Sanity check - G2W's rf_enter should be called before this function.
     If not, it's time to debug why :) */
  if(gl1_hw_g2w_rf_enter_called_last[gas_id] == FALSE)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("Can't send SEARCH_REQ without RF_ENTER_IND first.", 0, 0, 0);
  }

  /* Use XO value from TCXO manager for W freq correction */
  vco_info = gl1_hw_get_initial_rgs(gas_id);
  W_band = rfwcdma_core_convert_chan_to_band(search_uarfcn);
  wcdma_freq = gl1_hw_get_wcdma_freq(W_band, &inv_dlo_freq);
  W_freq_err_q6_hz = (vco_info.rot_value * wcdma_freq) >> 4;

  /* Convert relative startup time to absolute xo time for WL1 */
  g2w_start_time_xo = gstmr_qs_in_future_frame_to_xo(start_time_qs + mdsp_ftsm(gas_id), gas_id, start_time_frame_offset);

  MSG_GERAN_HIGH_3_G("G2W: startup xo time %d, freq err %d, W RF ID %d",
                     g2w_start_time_xo,
                     W_freq_err_q6_hz,
                     gl1_hw_get_wcdma_rf_id(W_band, gas_id));

  gl1_msg_algs_get_arfcn(&scell_arfcn.num, &scell_arfcn.band, gas_id);

  startup_params.rf_action_time      = g2w_start_time_xo;
  startup_params.gap_start_time      = g2w_start_time_xo;
  startup_params.freq_err_q6_hz      = W_freq_err_q6_hz;
  startup_params.inv_dlo_freq        = inv_dlo_freq;
  startup_params.gap_end_time        = 0;    /*Initialize this to avoid KW error */
  L1_send_IRAT_G2W_SRCH_REQ(search_mode,
                            search_uarfcn,
                            cell_search_params,
                            &startup_params,
                            gl1_get_irat_rf_structure_ptr(gas_id),
                            gas_id);
}

#else /* FEATURE_GSM_TO_WCDMA_MSGR */

/*===========================================================================

FUNCTION  gl1_hw_start_wcdma_rf_cmd

DESCRIPTION
  This function sets up RF and MDSP buffers before calling the mdsp driver
  routine to issue a WCDMA rf on command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_start_wcdma_rf_cmd( uint16 W_chn, uint16 start_time,gas_id_t gas_id )
{
  tcxomgr_vco_info_type vco_info;
  rfcom_wcdma_band_type W_band;
  int32 wcdma_freq;
  int32 freq_err_q6_hz;
  uint32 inv_dlo_freq;
  uint32 g2w_start_time_xo;
  wl1_x2w_srch_g2w_startup_params_struct_type g2w_params;
  ARFCN_T  scell_arfcn;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* Use XO value from TCXO manager for W freq correction */
  vco_info = gl1_hw_get_initial_rgs( gas_id );

  W_band       = rfwcdma_core_convert_chan_to_band( W_chn );
  wcdma_freq = gl1_hw_get_wcdma_freq( W_band, &inv_dlo_freq );
  freq_err_q6_hz = ( ( vco_info.rot_value * wcdma_freq ) >> 4);

  MSG_GERAN_HIGH_3_G("start time:%d mdsp_ftsm:%d FRAME_TICK_SETUP_MARGIN_QS:%d ",
                     start_time,mdsp_get_ftsm(gas_id),FRAME_TICK_SETUP_MARGIN_QS);

  /* Send command to GFW for mutex release */
  mdsp_start_wcdma_rf_cmd( start_time, FALSE, MDSP_RXLM_BUF_IDX_INVALID,gas_id);

  /* Convert startup time to ustmr/xo time */
  g2w_start_time_xo = gstmr_qs_in_next_frame_to_xo((start_time + mdsp_ftsm( gas_id )), gas_id);

  /* if GFW commands present in command buffer, send to GFW now and do not wait for G2W commands */
   mdsp_issue_async_msg(GFW_ASYNC_CMD,gas_id);

  MSG_GERAN_HIGH_3_G("G2W: g2w_startup, startup time %d (ustmr time %d), freq err %d ",start_time,g2w_start_time_xo,freq_err_q6_hz);

  gl1_msg_algs_get_arfcn(&scell_arfcn.num, &scell_arfcn.band, gas_id);

  g2w_params.mode                =  WL1_X2W_SRCH_G2W_IDLE_MODE;
  //g2w_params.g2w_rxlm_buf_idx    = gl1_wcdma_rxlm_buf_idx[gas_id];
  //g2w_params.g_home_rxlm_buf_idx = gl1_hw_get_gsm_rxlm_buf_idx(gas_id);
  g2w_params.rf_action_time      = g2w_start_time_xo;
  //g2w_params.startup_rf_buf_idx  = G2X_STARTUP_BURST_IDX;
  //g2w_params.cleanup_rf_buf_idx  = G2X_CLEANUP_BURST_IDX;
  //g2w_params.W_chn               = W_chn;
  //g2w_params.G_chn               = scell_arfcn.num;
  //g2w_params.W_band              = W_band;
  //g2w_params.G_band              = gl1_hw_arfcn_to_rfcom_gsm_band(scell_arfcn);
  g2w_params.freq_err_q6_hz      = freq_err_q6_hz;
  g2w_params.inv_dlo_freq        = inv_dlo_freq;
  //g2w_params.source_rf_device    = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
  //g2w_params.target_rf_device    = gl1_hw_get_wcdma_rf_id(W_band, gas_id);

  wl1_x2w_srch_g2w_startup(&g2w_params, gas_id );
}

/*===========================================================================

FUNCTION  gl1_hw_rf_sample_ram_cmd

DESCRIPTION
  This function sets up RF and MDSP buffers before calling the mdsp driver
  routine to issue a rf sample ram command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_rf_sample_ram_cmd (uint16 W_chn, uint16 start_time,gas_id_t gas_id)
{
  tcxomgr_vco_info_type vco_info;
  rfcom_wcdma_band_type W_band;
  int32 wcdma_freq, freq_err_q6_hz;
  uint32 inv_dlo_freq;
  uint16 stop_time;
  uint32 g2w_start_time_xo;

  ARFCN_T  scell_arfcn;
  wl1_x2w_srch_g2w_startup_params_struct_type g2w_params;
  wl1_x2w_srch_rf_timing_params_struct_type rf_timing_params;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* Use XO value from TCXO manager for W freq correction */
  vco_info = gl1_hw_get_initial_rgs( gas_id );

  W_band       = rfwcdma_core_convert_chan_to_band( W_chn );
  wcdma_freq = gl1_hw_get_wcdma_freq( W_band, &inv_dlo_freq );
  freq_err_q6_hz = ( ( vco_info.rot_value * wcdma_freq ) >> 4);

  wl1_x2w_srch_get_rf_timing_params(&rf_timing_params, gas_id);

  stop_time = start_time + rf_timing_params.startup_time + rf_timing_params.pll_settling_time + MDSP_FILL_SAMPLE_RAM_DURATION;
  /* Send commands to GFW for mutex release and cleanup */
  MSG_GERAN_HIGH_3_G( "G2W gap start time %d, stop time %d, cleanup time %d", start_time, stop_time, rf_timing_params.cleanup_time);

  mdsp_rf_sample_ram_cmd(start_time, stop_time, FALSE, gl1_hw_get_gsm_rxlm_buf_idx(gas_id), gl1_wcdma_rxlm_buf_idx[gas_id] ,gas_id);

  /* Convert startup time to ustmr/xo time */
  g2w_start_time_xo = gstmr_qs_in_next_frame_to_xo((start_time + mdsp_ftsm( gas_id )), gas_id);

  /* if GFW commands present in command buffer, send to GFW now and do not wait for G2W commands */
   mdsp_issue_async_msg(GFW_ASYNC_CMD,gas_id);

  MSG_GERAN_HIGH_3_G("G2W: dedicated mode startup: ustmr time %d, freq err %d, W RF ID %d ",g2w_start_time_xo,freq_err_q6_hz,gl1_hw_get_wcdma_rf_id(W_band, gas_id));

  gl1_msg_algs_get_arfcn(&scell_arfcn.num, &scell_arfcn.band, gas_id);

  g2w_params.mode                = WL1_X2W_SRCH_G2W_DEDICATED_MODE;
  //g2w_params.g2w_rxlm_buf_idx    = gl1_wcdma_rxlm_buf_idx[gas_id];
  //g2w_params.g_home_rxlm_buf_idx = gl1_hw_get_gsm_rxlm_buf_idx(gas_id);
  g2w_params.rf_action_time      = g2w_start_time_xo;
  /*g2w_params.startup_rf_buf_idx  = G2X_STARTUP_BURST_IDX;
  g2w_params.cleanup_rf_buf_idx  = G2X_CLEANUP_BURST_IDX;
  g2w_params.W_chn               = W_chn;
  g2w_params.G_chn               = scell_arfcn.num;
  g2w_params.W_band              = W_band;
  g2w_params.G_band              = gl1_hw_arfcn_to_rfcom_gsm_band(scell_arfcn);*/
  g2w_params.freq_err_q6_hz      = freq_err_q6_hz;
  g2w_params.inv_dlo_freq        = inv_dlo_freq;
  //g2w_params.source_rf_device    = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
  //g2w_params.target_rf_device    = gl1_hw_get_wcdma_rf_id(W_band, gas_id);

  wl1_x2w_srch_g2w_startup(&g2w_params, gas_id );

  gl1_hw_schedule_rf_sample_ram(start_time,gas_id);
}
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
#endif

#ifndef FEATURE_GSM_TO_WCDMA_MSGR

/*===========================================================================

FUNCTION  gl1_hw_set_wcdma_srch_on, gl1_hw_is_wcdma_srch_on

DESCRIPTION
  These functions are used to ensure that the WCDMA Searcher Block
  Initialization is done only once.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_set_wcdma_srch_on( boolean wsrch_on )
{
  if( wsrch_on == TRUE )
    wcdma_srch_is_on = TRUE;
  else
    wcdma_srch_is_on = FALSE;
}

boolean gl1_hw_is_wcdma_srch_on( void )
{
  return( wcdma_srch_is_on );
}

#endif  /*FEATURE_GSM_TO_WCDMA_MSGR*/

#if defined(FEATURE_GSM_TO_LTE) || defined(FEATURE_GSM_TDS)

void  gl1_hw_g2x_startup( uint16   time_offset,gas_id_t gas_id)
{
    mdsp_g2x_startup_cmd( time_offset,
                          FALSE,
                          MDSP_RXLM_BUF_IDX_INVALID , FALSE, gas_id);
}

#endif  /* FEATURE_GSM_TO_LTE  */

void  gl1_hw_g2x_cleanup( boolean  tuneback_to_gsm, boolean immed_cleanup, gas_id_t gas_id )
{
    mdsp_g2x_cleanup_cmd( 0 , gl1_hw_get_gsm_rxlm_buf_idx(gas_id), tuneback_to_gsm, immed_cleanup, gas_id  );
}

/*===========================================================================
FUNCTION  gl1_hw_g2x_cleanup_done_handler

DESCRIPTION
if we are in Xfer or DTM mode then issue rfmeas_exit immediatly on cleanup_cmd_cnf 

RETURN VALUE
  Void
===========================================================================*/
void  gl1_hw_g2x_cleanup_done_handler( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);


  gl1_irat_mutex_lock( gas_id);
  MSG_GERAN_MED_2_G("DBG: GFW G2X cleanup done in srch state %d target tech %d",
                     l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                     l1_sc_globals_ptr->active_irat);
  /*if we are in Xfer or DTM mode then issue rfmeas_exit immediatly on cleanup_cmd_cnf  */
  if (l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE && (gl1_is_l1_state_ptm(gas_id) || gl1_is_l1_state_dtm(gas_id)))
  {
    if (gl1_hw_get_g2x_rf_enter_called_last(gas_id))
    {
      g2l_irat_x2l_meas_exit(l1_sc_wcdma_data_ptr->g2x_aborted, gas_id);
      MSG_GERAN_MED_0_G("DBG: Issued rfmeas_exit on cleanup_cnf");
      gl1_hw_set_g2x_rf_enter_called_last(FALSE, gas_id);
    }
    l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;
  }

  gl1_irat_mutex_unlock( gas_id);
}


#ifdef FEATURE_GSM_TDS
void  gl1_hw_g2t_startup( uint32   time_offset,gas_id_t gas_id)
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT) )
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_g2x_startup: Return ");
   return ;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

  if(time_offset < QS_PER_FRAME)
  {
      vstmr_tdsstmr_t   current_tdsstmr;
      vstmr_tds_view_s *tdsstmr_view;

      uint16 cur_subfn_strtup, cur_pos_strtup;
#if defined(FEATURE_GSM_TO_LTE) || defined(FEATURE_GSM_TDS)
      if (!l1_sc_x2g_ta_irat_gap(gas_id))
      {
         /*Issue startup in next frame*/
         gl1_hw_g2x_startup( time_offset,gas_id);
      }
#endif
      gl1_startup_decrement_time_offset[gas_id] = 0;
      mdsp_decrement_startup_time_offset_flag[gas_id] = FALSE;

      /* debug info*/
      tdsstmr_view = vstmr_tds_get_view_handle (VSTMR_RTC_TDS_SUB0, VSTMR_TDS_WALL_TIME_VIEW);
      current_tdsstmr = vstmr_tds_get_current_stmr ( tdsstmr_view );
      cur_subfn_strtup = current_tdsstmr.subframe_no;
      cur_pos_strtup = current_tdsstmr.subframecx8;

      MSG_GERAN_HIGH_5_G("TDS startup: time_offset %d cur_subfn %d, cur_pos %d GFN %d qs %d",
                                       time_offset,
                                       cur_subfn_strtup,
                                       cur_pos_strtup,
                                       GSTMR_GET_FN_GERAN(gas_id),
                                       gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id))
                        );

      /* Suspend power measure during T measurement */
      gl1_msg_suspend_pwr_meas(gas_id);
      l1_g2t_delay_for_fw_startup = 2;

  }
  else
  {
      /* Need to decrement 5000 for every frame */
      gl1_startup_decrement_time_offset[gas_id] = time_offset - QS_PER_FRAME;
      /* Now do same decrement & check every frame */
      mdsp_decrement_startup_time_offset_flag[gas_id] = TRUE;
      l1_g2t_delay_for_fw_startup = 0;
      
      MSG_GERAN_HIGH_2_G("TDS startup: time_offset %d decrement time_offset %d",
                time_offset, gl1_startup_decrement_time_offset[gas_id]);
  }
}

void gl1_hw_g2t_startup_decrement( gas_id_t gas_id )
{
  gl1_hw_g2t_startup( gl1_startup_decrement_time_offset[gas_id],gas_id);
}

void gl1_hw_g2t_cleanup_decrement( gas_id_t gas_id )
{
  if (l1_g2t_delay_for_fw_cleanup)
  {
    l1_g2t_delay_for_fw_cleanup = FALSE;
    mdsp_decrement_cleanup_time_offset_flag[gas_id] = FALSE;
    if(l1_sc_get_tds_issue_cleanup(gas_id))
    {
       if(l1_sc_wcdma_data[gas_id].wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED)
       {
          l1_g2t_gap_end(gas_id);
       }
       l1_sc_set_tds_issue_cleanup(FALSE,gas_id);
    }
    gl1_msg_resume_pwr_meas(gas_id);

    if(l1_g2t_delay_tbf_rel != 0xff)
    {
      l1_g2t_issue_tbf_rel = TRUE;
    }

  }
  else
  {
    if(l1_sc_get_tds_issue_cleanup(gas_id))
    {
      gl1_hw_g2t_cleanup(0,gas_id);
    }
    else
    {
      gl1_hw_g2t_cleanup( gl1_cleanup_decrement_time_offset[gas_id],gas_id);
    }
  }
}

void  gl1_hw_g2t_cleanup( uint32 time_offset,gas_id_t gas_id )
{
   boolean cleanup_immediate = FALSE;

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT) )
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_g2x_cleanup: Return ");
   return ;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   /* Set startup delay for one frame to avoid cleanup gets issued immediately
      before startup */
   if (l1_g2t_delay_for_fw_startup > 0)
   {
     l1_g2t_delay_for_fw_startup--;
   }

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
   if ( l1_sc_get_tds_issue_cleanup(gas_id) &&
       (l1_g2t_delay_for_fw_startup == 0)   && 
        l1_sc_g2x_timeline_opt_enable[gas_id])
   {
      cleanup_immediate = TRUE;
   }
#endif

   if(time_offset < QS_PER_FRAME)
   {
      vstmr_tdsstmr_t   current_tdsstmr;
      vstmr_tds_view_s *tdsstmr_view;

      uint16 cur_subfn_clnup, cur_pos_clnup;

      if (!l1_sc_x2g_ta_irat_gap(gas_id))
      {
         /*Issue cleanup in next frame*/
         mdsp_g2x_cleanup_cmd( time_offset,
                               gl1_hw_get_gsm_rxlm_buf_idx(gas_id),
                               TRUE,
                               #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
                               cleanup_immediate,
                               #else
                               FALSE,
                               #endif
                               gas_id ); //NOT going to work in dedicated!!
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
         if (cleanup_immediate)
         {
            mdsp_async_intf_send_immediate_cmd(GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);
         }
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */

      }
      gl1_cleanup_decrement_time_offset[gas_id] = 0;

      /* debug info*/
      tdsstmr_view = vstmr_tds_get_view_handle (VSTMR_RTC_TDS_SUB0, VSTMR_TDS_WALL_TIME_VIEW);
      current_tdsstmr = vstmr_tds_get_current_stmr ( tdsstmr_view );
      cur_subfn_clnup = current_tdsstmr.subframe_no;
      cur_pos_clnup = current_tdsstmr.subframecx8;


      MSG_GERAN_HIGH_5_G("TDS cleanup: time_offset %d TD timing:cur_subfn %d, cur_pos %d GFN %d qs %d",
                                       time_offset,
                                       cur_subfn_clnup,
                                       cur_pos_clnup,
                                       GSTMR_GET_FN_GERAN(gas_id),
                                       gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id))
                        );

      l1_g2t_delay_for_fw_cleanup= TRUE;
      /* Ensure that even in case where there is no delay in
       * issuing of cleanup we delay resuming power meas by 1 frame
       */
      mdsp_decrement_cleanup_time_offset_flag[gas_id] = TRUE;

      gl1_drx_require_next_tick(gas_id);
   }
   else
   {
       /* Need to decrement 5000 for every frame */
       gl1_cleanup_decrement_time_offset[gas_id] = time_offset - QS_PER_FRAME;
       /* Now do same decrement & check every frame */
       mdsp_decrement_cleanup_time_offset_flag[gas_id] = TRUE;
       MSG_GERAN_HIGH_2_G("TDS cleanup: time_offset %d decrement time_offset %d",
                 time_offset, gl1_cleanup_decrement_time_offset[gas_id]);
       /* Keep power measure suspended until clean-up complete */
       gl1_msg_suspend_pwr_meas(gas_id);
      /**
        * CR351597: Ensure UE doesn't go into sleep mode
        *  */
       gl1_drx_require_next_tick(gas_id);
   }

}

void gl1_hw_calc_gap_values( gsm_l1_tds_gap_info_type *gap_info_ptr,gas_id_t gas_id)
{
   uint32 gap_pos_c8, gap_length_c8;
   uint16 current_gsm_time,td_stmr_read_time_gsm;

   vstmr_tdsstmr_t   current_tdsstmr;
   vstmr_tds_view_s *tdsstmr_view;

   uint16 cur_subfn, subfn_carry=0;
   uint16 cur_pos;

   #define GSM_MAX_TDS_STMR_RD_TIME       1 //1 qymbols allowed for read
   #define GSM_GAP_POS_MARGIN             2 //in Qsymbols
   #define GSM_GAP_LENGTH_MARGIN          2 //in Qsymbols
   #define GSM_NUM_CHIP_PER_TDS_SUBFRAME  51200 // local def. for number of Cx8 per TDS subframe(6400*8)

   /* Initialise */
   l1_g2t_delay_for_fw_cleanup = FALSE;
   l1_sc_set_tds_issue_cleanup(FALSE,gas_id);

   GDRV_ISR_LOCK(gas_id);

   current_gsm_time = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));
   td_stmr_read_time_gsm = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));

   /* Add remaining time in the current frame*/
   gap_info_ptr->gap_start_pos_qs += (QS_PER_FRAME  - current_gsm_time);
   /* convert latched TDS time for both methods */
   tdsstmr_view = vstmr_tds_get_view_handle (VSTMR_RTC_TDS_SUB0, VSTMR_TDS_WALL_TIME_VIEW);
   current_tdsstmr = vstmr_tds_get_current_stmr ( tdsstmr_view );
   cur_subfn = current_tdsstmr.subframe_no;
   cur_pos = current_tdsstmr.subframecx8;

   if (SUB_MOD(td_stmr_read_time_gsm,current_gsm_time, QS_PER_FRAME )> GSM_MAX_TDS_STMR_RD_TIME)
   {
     MSG_GERAN_ERROR_3_G("TDS: TD STMR read time - start %d qs end %d qs time_taken %d",
               current_gsm_time, td_stmr_read_time_gsm,
               SUB_MOD(td_stmr_read_time_gsm,current_gsm_time, QS_PER_FRAME) );
   }

   /*convert into GSM clocks(13 Mhz) - values passed in should be in qsymbols*/
   gap_pos_c8 = gsm_clks_to_tds_cx8_clks( (gap_info_ptr->gap_start_pos_qs + GSM_GAP_POS_MARGIN) * NUM_GSM_CLKS_PER_QS );
   gap_length_c8 = gsm_clks_to_tds_cx8_clks( (gap_info_ptr->gap_length_qs - GSM_GAP_LENGTH_MARGIN) * NUM_GSM_CLKS_PER_QS );

   /*Need to re-write to account for a wrap in STMR!!!!*/

   if ( (cur_pos + gap_pos_c8) >= GSM_NUM_CHIP_PER_TDS_SUBFRAME )
   {
     subfn_carry = ( cur_pos + gap_pos_c8 )/GSM_NUM_CHIP_PER_TDS_SUBFRAME;

     MSG_GERAN_HIGH_3_G("TDS calc gap: subfn_carry %d curr_pos %d gap_pos_c8 %d,",
              subfn_carry,cur_pos,gap_pos_c8);

     gap_info_ptr->gap_start_pos = ( cur_pos + ( gap_pos_c8  - ( subfn_carry*GSM_NUM_CHIP_PER_TDS_SUBFRAME )) );
     gap_info_ptr->gap_start_subfn = cur_subfn + subfn_carry;

   }
   else
   {
     gap_info_ptr->gap_start_pos = cur_pos + gap_pos_c8;
     gap_info_ptr->gap_start_subfn = cur_subfn;

     MSG_GERAN_HIGH_2_G("TDS calc gap: subfn_carry no carry curr_pos %d gap_pos_c8 %d,",
              cur_pos,gap_pos_c8);
   }

   if ( ( gap_info_ptr->gap_start_pos + gap_length_c8) >= GSM_NUM_CHIP_PER_TDS_SUBFRAME )
   {
     subfn_carry = ( gap_info_ptr->gap_start_pos + gap_length_c8 )/GSM_NUM_CHIP_PER_TDS_SUBFRAME;

     MSG_GERAN_HIGH_3_G("TDS calc gap length: subfn_carry %d gap_start_pos %d gap_length_c8 %d,",
              subfn_carry,gap_info_ptr->gap_start_pos,gap_length_c8);

     gap_info_ptr->gap_end_pos =  (gap_info_ptr->gap_start_pos + (gap_length_c8  - ( subfn_carry*GSM_NUM_CHIP_PER_TDS_SUBFRAME )));
     gap_info_ptr->gap_end_subfn = gap_info_ptr->gap_start_subfn + subfn_carry;
   }
   else
   {
     gap_info_ptr->gap_end_pos = gap_info_ptr->gap_start_pos + gap_length_c8;
     gap_info_ptr->gap_end_subfn = gap_info_ptr->gap_start_subfn;
   }

   // Make sure that start subfn and end subfn should be within the limit of subfn returned by TDS_WALL_TIME_COUNTER
   gap_info_ptr->gap_start_subfn %= GL1_MAX_TDS_SUBFRAME_COUNT;
   gap_info_ptr->gap_end_subfn %= GL1_MAX_TDS_SUBFRAME_COUNT;

   GDRV_ISR_UNLOCK(gas_id);

#if (defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG))

   MSG_GERAN_HIGH_3_G("TDS gap:td_stmr_read_time_gsm %d gap_start_pos_qs %d gap_length_qs %d",
            td_stmr_read_time_gsm,gap_info_ptr->gap_start_pos_qs,  gap_info_ptr->gap_length_qs);

   MSG_6( MSG_SSID_GSM_L1, MSG_LEGACY_HIGH,
          "TDS: G2T cx8 gap info cur_subfn %d, cur_pos %d gap: start_pos %d start_sfn %d end_pos %d end_sfn %d",
          cur_subfn, cur_pos, gap_info_ptr->gap_start_pos,gap_info_ptr->gap_start_subfn, gap_info_ptr->gap_end_pos,
          gap_info_ptr->gap_end_subfn);

   gap_info_debug.current_FN = GSTMR_GET_FN_GERAN(gas_id);
   gap_info_debug.gap_length_qs = (gap_info_ptr->gap_length_qs - GSM_GAP_LENGTH_MARGIN);
   gap_info_debug.gap_start_pos_qs = (gap_info_ptr->gap_start_pos_qs + GSM_GAP_POS_MARGIN);

  /*gap position info in TDS Cx8*/
  gap_info_debug.gap_start_subfn = gap_info_ptr->gap_start_subfn;
  gap_info_debug.gap_start_pos = gap_info_ptr->gap_start_pos;

  gap_info_debug.gap_end_subfn = gap_info_ptr->gap_end_subfn;
  gap_info_debug.gap_end_pos = gap_info_ptr->gap_end_pos;
#endif
}
#endif /*FEATURE_GSM_TDS*/

#endif /* ..INTERRAT.. */

/*===========================================================================

FUNCTION gl1_hw set/get functions for:
         gl1_hw_g2w_rf_enter_called_last

===========================================================================*/
boolean gl1_hw_get_g2x_rf_enter_called_last(gas_id_t gas_id)
{
  return(gl1_hw_g2w_rf_enter_called_last[gas_id]);
}

void gl1_hw_set_g2x_rf_enter_called_last(boolean value, gas_id_t gas_id)
{
  gl1_hw_g2w_rf_enter_called_last[gas_id] = value;
}

/*===========================================================================

FUNCTION  gl1_hw_enable_bbrx_clock_gating

DESCRIPTION
  This function enables or disables BB Rx clock gating
  Needs to disable the MDSP control of BB Rx clock gating if W being set-up
  and remain disabled for the duration of the W measurement.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_enable_bbrx_clock_gating ( boolean enable_bbrx_clk_gating )
{
}

/*===========================================================================
FUNCTION gl1_hw_allocate_wcdma_rxlm_buf
DESCRIPTION
  Deallocates the WCDMA RxLM buffer
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_allocate_wcdma_rxlm_buf( gas_id_t gas_id )
{
   if (gl1_wcdma_rxlm_buf_idx[gas_id] != MDSP_RXLM_BUF_IDX_INVALID)
   {
      MSG_GERAN_HIGH_1("WCDMA RxLM buffer already allocated %d - deallocate now", gl1_wcdma_rxlm_buf_idx[gas_id]);
      rflm_deallocate_rx_handle(LM_GSM, LM_UMTS, gl1_wcdma_rxlm_buf_idx[gas_id]);
      gl1_wcdma_rxlm_buf_idx[gas_id] = MDSP_RXLM_BUF_IDX_INVALID;
   }

   /* Allocate RxLM buffer, get buffer index */
   if ((rflm_allocate_rx_handle(LM_GSM, LM_UMTS, &gl1_wcdma_rxlm_buf_idx[gas_id])) != LM_SUCCESS)
   {
     MSG_GERAN_ERROR_0("Unable to allocate WCDMA RxLM buffer");
   }
   else
   {
     MSG_GERAN_MED_1("WCDMA RxLM buffer %d allocated",gl1_wcdma_rxlm_buf_idx[gas_id]);
   }
}

/*===========================================================================
FUNCTION gl1_hw_deallocate_wcdma_rxlm_buf
DESCRIPTION
  Deallocates the WCDMA RxLM buffer
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_wcdma_rxlm_buf( gas_id_t gas_id )
{
   if (gl1_wcdma_rxlm_buf_idx[gas_id] == MDSP_RXLM_BUF_IDX_INVALID)
   {
     MSG_GERAN_MED_0("WCDMA RxLM buffer invalid, cannot deallocate");
   }
   else
   {
     if (!rflm_deallocate_rx_handle(LM_GSM, LM_UMTS, gl1_wcdma_rxlm_buf_idx[gas_id]))
     {
       MSG_GERAN_ERROR_1("Unable to de-allocate WCDMA RxLM buffer %d",gl1_wcdma_rxlm_buf_idx[gas_id]);
     }
     else
     {
       MSG_GERAN_HIGH_1("WCDMA RxLM buffer %d dealloacted",gl1_wcdma_rxlm_buf_idx[gas_id]);
     }
     gl1_wcdma_rxlm_buf_idx[gas_id] = MDSP_RXLM_BUF_IDX_INVALID;
   }
}

/*===========================================================================
FUNCTION gl1_hw_allocate_lte_rxlm_buf
DESCRIPTION
  Allocates the LTE RxLM buffers
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_allocate_lte_rxlm_buf( gas_id_t gas_id )
{
   if (gl1_lte_rxlm_buf0_idx[gas_id] != MDSP_RXLM_BUF_IDX_INVALID)
   {
      MSG_GERAN_HIGH_1("LTE RxLM buffer already allocated %d - deallocate now", gl1_lte_rxlm_buf0_idx[gas_id]);
      rflm_deallocate_rx_handle(LM_GSM, LM_LTE, gl1_lte_rxlm_buf0_idx[gas_id]);
      gl1_lte_rxlm_buf0_idx[gas_id] = MDSP_RXLM_BUF_IDX_INVALID;
   }

   /* Allocate RxLM buffer, get buffer index (keep trying until allocated)*/
   if ((rflm_allocate_rx_handle(LM_GSM, LM_LTE, &gl1_lte_rxlm_buf0_idx[gas_id])) != LM_SUCCESS)
   {
     MSG_GERAN_ERROR_0("Unable to allocate LTE RxLM buffer 0");
   }
   else
   {
     MSG_GERAN_MED_1("LTE RxLM buffer %d allocated",gl1_lte_rxlm_buf0_idx[gas_id]);
   }

   if (gl1_lte_rxlm_buf1_idx[gas_id] != MDSP_RXLM_BUF_IDX_INVALID)
   {
      MSG_GERAN_HIGH_1("LTE RxLM buffer already allocated %d - deallocate now", gl1_lte_rxlm_buf1_idx[gas_id]);
      rflm_deallocate_rx_handle(LM_GSM, LM_LTE, gl1_lte_rxlm_buf1_idx[gas_id]);
      gl1_lte_rxlm_buf1_idx[gas_id] = MDSP_RXLM_BUF_IDX_INVALID;
   }

   /* Allocate RxLM buffer, get buffer index (keep trying until allocated)*/
   if ((rflm_allocate_rx_handle(LM_GSM, LM_LTE, &gl1_lte_rxlm_buf1_idx[gas_id])) != LM_SUCCESS)
   {
     MSG_GERAN_ERROR_0("Unable to allocate LTE RxLM buffer 1");
   }
   else
   {
     MSG_GERAN_MED_1("LTE RxLM buffer %d allocated",gl1_lte_rxlm_buf1_idx[gas_id]);
   }
}

/*===========================================================================
FUNCTION gl1_hw_deallocate_lte_rxlm_buf
DESCRIPTION
  Deallocates the LTE RxLM buffera
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_lte_rxlm_buf( gas_id_t gas_id )
{
  if ((gl1_lte_rxlm_buf0_idx[gas_id] == MDSP_RXLM_BUF_IDX_INVALID) && (gl1_lte_rxlm_buf1_idx[gas_id] == MDSP_RXLM_BUF_IDX_INVALID))
  {
     MSG_GERAN_MED_0("LTE RxLM buffers invalid, cannot deallocate");
  }
  else
  {
    if (!rflm_deallocate_rx_handle(LM_GSM, LM_LTE, gl1_lte_rxlm_buf0_idx[gas_id]))
    {
      MSG_GERAN_ERROR_1("Unable to de-allocate LTE RxLM buffer %d",gl1_lte_rxlm_buf0_idx[gas_id]);
    }
    else
    {
      MSG_GERAN_HIGH_1("LTE RxLM buffer %d dealloacted",gl1_lte_rxlm_buf0_idx[gas_id]);
    }
    gl1_lte_rxlm_buf0_idx[gas_id] = MDSP_RXLM_BUF_IDX_INVALID;

    if (!rflm_deallocate_rx_handle(LM_GSM, LM_LTE, gl1_lte_rxlm_buf1_idx[gas_id]))
    {
      MSG_GERAN_ERROR_1("Unable to de-allocate LTE RxLM buffer %d",gl1_lte_rxlm_buf1_idx[gas_id]);
    }
    else
    {
      MSG_GERAN_HIGH_1("LTE RxLM buffer %d dealloacted",gl1_lte_rxlm_buf1_idx[gas_id]);
    }
    gl1_lte_rxlm_buf1_idx[gas_id] = MDSP_RXLM_BUF_IDX_INVALID;
  }
}
/*===========================================================================

FUNCTION gl1_hw_get_lte_rxlm_buf0_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_lte_rxlm_buf0_idx( gas_id_t gas_id )
{
   return(gl1_lte_rxlm_buf0_idx[gas_id]);
}

/*===========================================================================

FUNCTION gl1_hw_get_lte_rxlm_buf1_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_lte_rxlm_buf1_idx( gas_id_t gas_id )
{
   return(gl1_lte_rxlm_buf1_idx[gas_id]);
}

#ifdef FEATURE_GSM_TDS

/*===========================================================================
FUNCTION gl1_hw_allocate_tds_rxlm_buf
DESCRIPTION
  Dealloactes the TDS RxLM buffer
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_allocate_tds_rxlm_buf( void )
{
   if (gl1_tds_rxlm_buf0_idx != MDSP_RXLM_BUF_IDX_INVALID)
   {
      MSG_GERAN_HIGH_1("TDS RxLM buffer already allocated %d - deallocate now", gl1_tds_rxlm_buf0_idx);
      rflm_deallocate_rx_handle(LM_GSM, LM_TDSCDMA, gl1_tds_rxlm_buf0_idx);
      gl1_tds_rxlm_buf0_idx = MDSP_RXLM_BUF_IDX_INVALID;
   }

   /* Allocate RxLM buffer, get buffer index (keep trying until allocated)*/
   if ((rflm_allocate_rx_handle(LM_GSM, LM_TDSCDMA, &gl1_tds_rxlm_buf0_idx)) != LM_SUCCESS)
   {
     MSG_GERAN_ERROR_0("Unable to allocate TDS RxLM buffer 0");
   }
   else
   {
     MSG_GERAN_MED_1("TDS RxLM buffer %d buf0 allocated",gl1_tds_rxlm_buf0_idx);
   }
}

/*===========================================================================
FUNCTION gl1_hw_deallocate_tds_rxlm_buf
DESCRIPTION
  Dealloactes the TDS RxLM buffer
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_tds_rxlm_buf( void )
{
  if (gl1_tds_rxlm_buf0_idx == MDSP_RXLM_BUF_IDX_INVALID)
  {
     MSG_GERAN_MED_0("TDS RxLM buffer 0 invalid, cannot deallocate");
  }
  else
  {
    if (!rflm_deallocate_rx_handle(LM_GSM, LM_TDSCDMA, gl1_tds_rxlm_buf0_idx))
    {
      MSG_GERAN_ERROR_1("Unable to de-allocate TDS RxLM buffer %d",gl1_tds_rxlm_buf0_idx);
    }
    else
    {
      MSG_GERAN_HIGH_1("TDS RxLM buffer %d dealloacted",gl1_tds_rxlm_buf0_idx);
    }
    gl1_tds_rxlm_buf0_idx = MDSP_RXLM_BUF_IDX_INVALID;

  }
}

/*===========================================================================

FUNCTION gl1_hw_get_tds_rxlm_buf0_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_tds_rxlm_buf0_idx( void )
{
   return(gl1_tds_rxlm_buf0_idx);
}
#endif /*FEATURE_GSM_TDS */


#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/*===========================================================================

FUNCTION  gl1_hw_wcdma_save_g2w_timing_params

DESCRIPTION
  This function performs any WCDMA specific mDSP initialization.  It should
  be called after loading the firmware.

===========================================================================*/
void gl1_hw_wcdma_save_g2w_timing_params(wl1_x2w_rf_timing_params_struct_type *timing_params,
                                         gas_id_t gas_id)
{
  memscpy(&g2w_rf_timing_params[gas_id],
          sizeof(wl1_x2w_rf_timing_params_struct_type),
          timing_params,
          sizeof(wl1_x2w_rf_timing_params_struct_type));
}
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
/* EOF */
