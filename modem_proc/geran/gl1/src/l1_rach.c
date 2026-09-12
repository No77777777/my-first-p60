/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1   R A C H  T R A N S M I T   P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to initiate a RACH burst.
   This procedure executes in the L1 ISR context.

EXTERNALIZED FUNCTIONS
  L1_access_ISR
    Procedure to initiate a synchronous transmit of a RACH message.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_rach.c#4 $
$DateTime: 2022/03/28 10:39:23 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
28/03/20   sal       CR3138940 Install rach handler if TA denied by higher layer in access running state.
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
10/05/17   sk        CR2133076 Retain the ACCESS priority even after tuning back from GTA 
05/06/17   br        CR2054120 Dont release tx lock in SSim case.
17/01/16   sn        CR1110699 TX chain release when we exit RACH mode.
03/08/16   cws       CR1050105 Check asdiv NV before trigger switch for AGCH failure
25/01/16   cws       CR962379 Disable/enable ASDIV in IDLE/RACH state by NV
09/08/16   rv        CR1051329 Reverting CR1038637, due to regression issue
11/07/17   rv        CR1038637 Fix for - TX_ON is not done properly, when RACH and STOP_MODE requests came back to back
25/5/15    mn        CR1002174 Intialize Init_RACH for CS in L1_ACCESS_GTA_FW_LOAD_WAIT state.
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
29/03/16   mn        CR995137 Access tune away support: Allowing other tech's paging at GPRS access
08/06/16   bg        CR1021727 Before disabling Tx, Checking that if RF_TX_ENABLE_CNF is received from RF
18/05/16   sn        CR1007565 PACKET Transfer DTF Events/Handlers Phase1
30/11/15   sp        CR944730 In RACH TA gap, stay in L1_ACCESS_GTA_TRM_WAIT even after inactive timer expiry.
13/10/15   sk        CR919071 Ensure the global, initial_rach_type is updated per SUB 
16/09/15   cjl       CR908527 improve system procedures used for TRM access
26/05/15   sk        CR840250 Increase the RACH disable timer 1000ms for SMS
14/08/14   ws        CR889273 Fix Static declarations of variables which are not Dual data spaced
31/07/15   hd        CR882159 Do not abort the RACH state if there was RACH tx in the previous frame.
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons 
27/07/15   nm        CR877001 do retain lock after access init to allow TA for TDS
07/07/15   nm        CR866681 Handle trm grant in L1_ACCESS_GTA_INACTIVITY_TMR_EXPIRED state
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types
13/07/15   smd       CR870632 Release TRM lock at access abort 
14/07/15   cws       CR845909 Switch antenna when AGCH decode fails
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
02/07/15   nm        CR864676 do retain lock after changing reason to BG TRAFFIC in RACH state
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
30/06/15   cjl       CR863267 Treat GL1_TRM_RETAINED_FOR_ACCESS as GL1_TRM_GRANTED in *_TRM_WAIT states
22/06/15   smd       CR857943 GL1 to release and request diversity chain at GTA and RACH tune away
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/05/15   ws        CR834838 Correct as_id,gas_id calling fn parameter bugs
15/04/15   pa        CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
28/04/15   mko       CR828316 Enable G to TD tuneaway during Rach process by default
30/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
30/03/15   nm        CR765891 Allowing TA for TDS during PS RACH
12/02/15   cja       CR794676 Only set tx band iss to GFW false if RF set Tx band is sent to RF
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs. 
16/01/14   js        CR781582 Setting the re calculate initial rach in TRM is denied in decoupled state.
03/12/14   pg        CR765847: Add support of Idle RxD for AGCH and NCELL SCH 
07/10/14   pa        CR733749:Handling of MPH_DSC_THRESHOLD_IND in the RACH state.  
20/05/14   dg        CR613013 Long Term fix on GL1 for TS 34.123 & TC 8.3.11.12 GCF Testcase
                     failure due to RACH failure on GSM cell
28/08/14   mko       CR706626: Update the serving cell pwr in agch_metrics_callback()
25/07/14   dg        CR699913 IN RACH mode , WTR hopping machine should be called only when handlers for
                     of the current frame are executed
22/05/14   nk        CR667961 Make ab_tsc crash code FTM_MODE proof
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
09/07/14   pjr       CR685199 Added extra check to ensure WTR hop is not triggered after ACCESS MODE is aborted
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
29/04/14   pjr       CR600612 Dedicated WTR Hop initial code drop
01/05/14   nk        CR656822 Debug prints/crash to catch when ab_tsc is greater than 2
07/04/14   cja       CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
12/02/14   am        CR611265 RACH timing update to GARB
07/01/14   dg        CR593574 GL1 to send Block Quality Ind to RR for the paging
                     blocks which belong to its group in RACH mode
20/12/13   sk        CR590776 Ensure TX commands not issued before Tx config issued
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
01/10/13   sk        CR552259 CR sync from 1.0/1.1
03/10/13   ws        CR499846 - Bug fix for RACH PWR reduction feature
16/09/13   pa        CR539506: Bolt: Added support for GPLT dedicated command via DIAG 
23/05/13   pg        CR460555: Enable 3GPP Rel10 RACH Power Reduction feature
07/16/13   zc        CR470197 Replace memcpy with memscpy
10/06/13   cs        TSTS Updates
08/06/12   pa        GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
13/03/12   ws        CR 330862 - Change dedicated mode F3 level's and add sacch burst
                     metrics and compact message metrics log packets.
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
14/06/10   ap        CR: 235190 - 3GPP: Enhanced control of maximum output power in
                      a common BCCH cell
07/08/09   og        CR188813. Send MPH_BLOCK_QUALITY_IND to RR in access mode.
20/01/10   cs        CR221032 21.3.1 failure with SAIC enabled as AFC/TT not updating
31/08/07   agv       CR127468 Fix for SAIC TT handling in AGCH.
05/03/07   ws        Fixed filtered lint warnings
14/07/06   dv        Fix for CR96966. Had to take care not to break CR86620
06/04/06   av        Feature SAIC MIMO elements added to the burst metrics.
08/02/05   og        Added missing frame tick notification in l1 mode stop.
01/31/06   ip        CR86620 added RACH intialisation if RACH type chnages
10/02/05   gfr       Use new gl1_msg_cfg_ul_rach API
26/08/05   og        Removal of DEBUG_TRANSFER compiler switch and associated
                     code.
08/17/05   gfr       Move NPL1 RACH function calls to ISR context
07/29/05   gfr       Support for NPL1 DTM interface changes
06/16/05   sv        Fixed lint error due to header file include.
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
01/24/05   pp        Added transmit burst metrics logging
01/10/05   dp        Lint cleanup
09/12/04   og        Fix to tx power control by tracking c value on ccch.
08/26/04   sv        Lint cleanup
07/21/04   bk        Merged in changes to RACH to support EGPRS
06/15/04   gfr       Renamed gl1_msg_cfg_dl_ccch to gl1_msg_cfg_ccch
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
03/18/04   gsc       Some more Lint clean up.
02/25/04   gw        Lint clean up.
01/28/04   gw        Changes to support reselection optimizations.
11/05/03   gfr       Support for quad-band.
08/29/03   gw        Addded code to output F3 message with SNRs for each
                     received block.
07/11/03   gfr       Lint cleanup.
05/01/03   mk        Added AFC/TT support.
02/04/03   drl       Added FEATURE_GSM_GPRS_L1 around gprs file include
01/02/03   gw        Modified call to gl1_msg_cfg_dl_ccch() to reflect addition
                     of another parameter.
12/02/02   ws        Modified DEBUG code under DEBUG_TRANSFER switch for GPRS
11/13/02   mk        Updated GPRS baseline.
11/01/02   pjr       Added gprs serving cell measurements under switch
                     FEATURE_GPRS_MEAS.
10/26/02   dp        Extern declaration for rach_cnt to fix Leopard
                     compiler errors with ADS1.2  Has now been declared in
                     gl1_msg_rach.c
10/14/02   ATM       Added central get and set FN routines
10/10/02   mk        Merged GPRS release 1 phase1 code to baseline.
09/03/02   dp        Made AGCH decoded debug msgs MSG_MED
06/06/02   mk        Support for aborts on all channels.
02/14/02   gw        Added call to gl1_msg_reset() to support AGC.
02/12/02   thh       Added a parm to l1_determine_rach().  The new boolean
                     parm specifies whether the current channel combination is
                     of type CCCH&SDCCH.
                     Updated copyright date.
02/05/02   JC        Changes to support AGC.
12/18/01   dp        Changes to use the new acq API for SC acquisition
12/11/01   gw        Changes to match new interface for getting debug (logging)
                     data.
11/27/01   mk        Added L1_ACCESS_INIT, ...RUNNING substates. Added
                     L1_MODE_STOP support. Misc cleanup.
11/9/01    bk        Updated NPL1 FTN block info to conform to the interface change
10/31/01   dp        Incorporated changes due to new CCCH decode_rpt format
10/10/01   gw        Added support for message/frame layer Ncell reception.
09/27/01   jc/gw     Added support for message/frame layer interface in RACH.
08/31/01   JC        Changed rach_ARFCN type from ARFCN_T to uint16 and
                     removed SPINCO_SIMULATION & KOVU compile switches.
06/14/01   thh       Call hw_tx_rach() with the BSIC.  Removed the obsolete
                     call to hw_set_bsic().
05/16/01   JC        Added hw_gsm prototypes and initialized idle flag to
                     FALSE to appease compiler.
05/05/01   thhuynh   Initial Version. (header block added by chatlani)

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
/* Alphabetical Order */
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1_isr.h"
#include "l1_log.h"      /* for l1_log_message_metrics() */
#include "l1_utils.h"    /* for gl1_get_FN()              */
#include "geran_msgs.h"
#include <stringl/stringl.h>

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "l1_task.h"

#include "geran_eng_mode_info.h"
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gpl1_dual_sim.h"
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */


#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT */

#include "mdsp_intf_g.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include "gl1_msg_wtr_hop.h"
#include "geran_multi_sim.h"
#include "gl1_hw_g.h"
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define L1_RACH_DISABLE_TIMEOUT  200 /* ms */
#define L1_RACH_DISABLE_FOR_SMS_TIMEOUT  1000 /* ms */
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

extern gl1_msg_rach_type  initial_rach_type[NUM_GERAN_DATA_SPACES];
extern boolean re_calculate_initial_rach[NUM_GERAN_DATA_SPACES];

static void agch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void agch_metrics_callback(gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static void l1_rach_disable_timer_handler( gas_id_t gas_id );
static void call_in_task_rach_timeout_handler(gas_id_t gas_id);
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

void rach_callback ( uint16 random_content, gas_id_t gas_id
#ifdef FEATURE_GSM_COEX_RACH_DELAY
, boolean yield
#endif
);

extern void write_arfcns(ARFCN_T rx_arfcns[], ARFCN_T arfcn );

extern boolean L1_determine_rach
(
  boolean ccch_sdcch_comb,    /* Is it a ccch/sdcch channel combo? */
  byte fn_mod51,               /* FN modular 51 */
  gas_id_t gas_id
);

typedef struct
{

boolean       rach_read_all_AGCH;
gl1_msg_rx_rpt      rach_rpt;
gl1_msg_decode_rpt  rach_decode_rpt;
gl1_msg_metrics_rpt rach_metrics_rpts[4];
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
rex_timer_type l1_rach_disable_timer;
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

}l1_rach_data_t;

static l1_rach_data_t  l1_rach_data[NUM_GERAN_DATA_SPACES];

void l1_set_rach_read_all_AGCH(boolean rach_read_all_AGCH,gas_id_t gas_id)
{
  l1_rach_data_t *l1_rach_data_ptr = &l1_rach_data[gas_id];
  l1_rach_data_ptr->rach_read_all_AGCH=rach_read_all_AGCH;
}

static boolean gl1_is_asdiv_switch_allowed[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(TRUE)};

/*===========================================================================

FUNCTION  L1_ACCESS_ISR

DESCRIPTION
  This function controls random access and access grant

DEPENDENCIES
  mode

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_access_ISR(l1_mode_command_T  mode, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_rach_data_t *l1_rach_data_ptr = &l1_rach_data[gas_id];

  /* Locals */
  idle_data_T               *idle_data_ptr;
  boolean                    idle=FALSE;
  sys_algo_agc_T             *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  sys_algo_agc_T   			*agc_ptr_divrx_ccch; // added CCCH temporarily for diferentiating on the other agc pointers.
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/


  gl1_msg_frame_tick_type    ftn_type;
  boolean                    dl_block_start;
  boolean                    ul_block_start;
  ARFCN_T                    arfcn, rx_arfcns[4];
  boolean                    own_paging_block = FALSE;
  gl1_msg_chan_type 		 chan_type = GL1_MSG_CCCH;
#ifdef FEATURE_GTA_DURING_PS_RACH
    static int8 inactivity_timer[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
    static boolean init_rach[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(TRUE) };
#endif

  /* Code */
  idle_data_ptr 		= l1_tskisr_blk->current_params.L1Data.pIdle_data;
  agc_ptr       		= &idle_data_ptr->campedon_cell_agc;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  agc_ptr_divrx_ccch	= &idle_data_ptr->campedon_cell_agc_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  if ( mode == L1_MODE_RUN )
  {
    switch ( l1_tskisr_blk->sub_state )
    {
    case L1_ACCESS_INIT :
      {
      gl1_msg_rx_cb_type     rx_cb;
      gl1_msg_rach_cb_type   tx_cb;
      int                    i;

      MSG_GERAN_HIGH_3_G("Access init mcpm_tx_on %d fw cmd issued %d Tx enable cnf %d",g1l_hw_is_mcpm_tx_on(gas_id),gl1_set_tx_band_cmd_iss_to_gfw[gas_id],l1_rf_tx_enable_cnf_rcv[gas_id]);
      /*If Tx config command is pending or MCPM is not turned on or TX band command is not
          issued to GFW then return*/
      if(
#ifndef FEATURE_GSM_RX_TX_SPLIT
         (mdsp_get_tx_config_cmd_pending(gas_id) ) ||
#endif
         (!g1l_hw_is_mcpm_tx_on(gas_id)) || 
         (gl1_set_tx_band_cmd_iss_to_gfw[gas_id] == FALSE) || 
         (l1_rf_tx_enable_cnf_rcv[gas_id] == FALSE))
      {
        MSG_GERAN_HIGH_0_G("TXConfig command pending, hold Tx until next frame tick");
        return idle;
      }
      /* Setup callbacks etc for meassage layer i/f */
      gl1_msg_reset(gas_id);

#ifdef FEATURE_GTA_DURING_PS_RACH
      /*configure RACH again only if there was any pending RACH before opening a GTA gap*/
      if(init_rach[gas_id] == TRUE)
#endif
      {
      /* Configure the RACH */
      tx_cb.data_cb    = rach_callback;
      tx_cb.abort_cb   = NULL;

      /* do we need to send a EGPRS Packet channel request ?
       * Also record the initial RACH type to detect a change in type
       * during Access.
       */
      if (idle_data_ptr->random_access_data.egprs_ra)
      {
        if ( ftm_get_mode() != FTM_MODE )
        {
          if(idle_data_ptr->random_access_data.egprs_tsc > 2)
          {
#ifdef FEATURE_QSH_MDUMP
            QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
            ERR_GERAN_FATAL_1_G("RACH DATA:: egprs_tsc: %d", idle_data_ptr->random_access_data.egprs_tsc);
          }
        }
        gl1_msg_cfg_ul_rach(idle_data_ptr->random_access_data.egprs_tsc,
                            idle_data_ptr->campedon_cell_BSIC,
                            GL1_MSG_RACH_EXTENDED, &tx_cb,gas_id);

        initial_rach_type[gas_id] = GL1_MSG_RACH_EXTENDED;
      }
      else
      {
        gl1_msg_cfg_ul_rach (0, /* TSC s*/
                             idle_data_ptr->campedon_cell_BSIC,
                             GL1_MSG_RACH_NORMAL, &tx_cb,gas_id);

        initial_rach_type[gas_id] = GL1_MSG_RACH_NORMAL;
      }

      /* activate RACH power reduction */
      gl1_activate_rach_pwr_reduction(idle_data_ptr->random_access_data.pwr_reduction_valid,
                                      idle_data_ptr->random_access_data.pwr_reduction_value,
                                      idle_data_ptr->random_access_data.emergency_call,
                                      gas_id);
      /* calculate txlev */
      idle_data_ptr->random_access_data.txlev = calculate_txpwr_max_cch(idle_data_ptr->campedon_cell_ARFCN
                                                                        , gl1_get_rach_pwr_reduction(gas_id)
                                                                        , gas_id
                                                                        );

#ifdef FEATURE_GSM_COEX_SW_CXM
      /* Notify the arbitrator of the transition to ACCESS */
      garb_intf_notify_access(idle_data_ptr->campedon_cell_ARFCN, idle_data_ptr->random_access_data.ra_delay, 
                               idle_data_ptr->random_access_data.txlev, gas_id );
#endif /* FEATURE_GSM_COEX_SW_CXM */

      /* Transmit the rach */
      gl1_msg_tx_rach ( idle_data_ptr->campedon_cell_ARFCN,
                        idle_data_ptr->random_access_data.txlev,
                        idle_data_ptr->random_access_data.ra_delay,
                        idle_data_ptr->random_access_data.ra_content,
                        gas_id);

      /* In decoupled state, if TRM lock is denied flag is not reset, so resetting here. */
      re_calculate_initial_rach[gas_id] = FALSE;
      }
  #ifdef FEATURE_GTA_DURING_PS_RACH
      else
      {
        MSG_GERAN_HIGH_0_G("GTA_PS_RACH: Skip configure RACH as no pending RACH before TA gap");
        init_rach[gas_id] = TRUE;
      }
#endif
      /* Configure CCCH for AGCH */
      rx_cb.data_cb    = agch_rx_callback;
      rx_cb.metrics_cb = agch_metrics_callback;
      rx_cb.log_cb     = NULL;
      rx_cb.abort_cb   = NULL;

      gl1_msg_cfg_ccch((uint8)(idle_data_ptr->campedon_cell_BSIC & 0x7),
                          FALSE, &rx_cb,gas_id );

      gl1_msg_cch_afc_tt_init( gl1_get_serving_cell_arfcn(gas_id),gas_id);

      /* Maybe this should be done for each call the gl1_msg_rx_ccch */
      l1_rach_data_ptr->rach_rpt.data    = &l1_rach_data_ptr->rach_decode_rpt;
      for ( i=0; i<4; i++ )
      {
        l1_rach_data_ptr->rach_rpt.metrics[i] = &l1_rach_data_ptr->rach_metrics_rpts[i];
      }

      l1_tskisr_blk->sub_state = L1_ACCESS_RUNNING;

#ifdef FEATURE_GTA_DURING_PS_RACH
      if(idle_data_ptr->random_access_data.rach_for_ps_service == TRUE)
      {
        MSG_GERAN_HIGH_1_G("GTA_PS_RACH : client %d Access init..retain lock", l1_tskisr_blk->client_id);
        grm_retain_lock( l1_tskisr_blk->client_id,
                         (grm_unlock_callback_t)&grm_ps_access_abort_callback,
                         gas_id );
      }
#endif

      }

      /* fall through... */

    case L1_ACCESS_RUNNING:

#ifdef FEATURE_GTA_DURING_PS_RACH
            
      /* Safety check for debugging*/
      if((idle_data_ptr->random_access_data.suspension_FN_GTA != L1_GPRS_INVALID_FN) &&
        (SUB_FN(idle_data_ptr->random_access_data.suspension_FN_GTA, 2) == GSTMR_GET_FN_GERAN(gas_id)))
      {
        MSG_GERAN_MED_1_G("GTA_PS_RACH : L1_ACCESS_RUNNING 2 frames before suspension FN rach state %d", gl1_msg_get_rach_state(gas_id));
        gl1_is_asdiv_switch_allowed[gas_id] = FALSE;
        break;
      }
      else if((idle_data_ptr->random_access_data.suspension_FN_GTA != L1_GPRS_INVALID_FN) &&
              (idle_data_ptr->random_access_data.suspension_FN_GTA == GSTMR_GET_FN_GERAN(gas_id)) && 
              (TRUE == gl1_is_geran_tuneaway_allowed(gas_id)) &&
              (FALSE == idle_data_ptr->non_updated_lai_in_nmo1))
  
      {
        /* Abort RACH Tx */
        gl1_msg_abort_rach(gas_id);
    
        /* Abort AGCH Rx */
        gl1_msg_abort_rx_ccch (gas_id);
    
        MSG_GERAN_MED_0_G("GTA_PS_RACH :Access abort for G2X TA in RACH (PS)");
   
  #ifdef FEATURE_GSM_GPRS_L1
        (void)gpl1_gprs_serving_cell_meas(0,gas_id);
  #endif
  
        /* Initialise the frame tick notification structure contents */
        ftn_type.next_frame_is_dl_block_start             = FALSE;
        ftn_type.next_frame_is_ul_block_start             = FALSE;
        ftn_type.next_frame_is_ul_pdch_block_start        = FALSE;
        ftn_type.next_frame_is_dl_pdch_block_start        = FALSE;
        ftn_type.next_frame_is_ul_ptcch_sacch_block_start = FALSE;
        ftn_type.next_frame_is_dl_ptcch_sacch_block_start = FALSE;
        ftn_type.next_frame_is_acq_ncell_block_start      = FALSE;
        ftn_type.next_frame_is_pwr_meas_block_start       = FALSE;
        ftn_type.next_frame_is_int_meas_block_start       = FALSE;
    
        /* Inform lower layers of TDMA frame */
        gl1_msg_frame_tick_notification ( &ftn_type,gas_id );
    
  #ifdef FEATURE_GSM_WTR_HOP
        /* Abort any hop in progress. Function gl1_msg_wtr_hop_abort 
            * will only return false if the RF enter mode command was issued,
            * in this case abort is not possible and hop state machine will need
            * to be ticked for the next two frames to complete the hop */
        if (!gl1_msg_wtr_hop_abort(FALSE, gas_id))
        {
          gl1_msg_wtr_hop_state_machine(NULL, gas_id);
        }
  #endif /*FEATURE_GSM_WTR_HOP*/
  
        /*move to supending state to allow one more frame for fw_unload*/
        l1_tskisr_blk->sub_state = L1_ACCESS_GTA_SUSPENDING;
        inactivity_timer[gas_id] = G2X_TUNEAWAY_DSDS_TIMEOUT_RACH;
        break;
      }
      else
      {
         if (( idle_data_ptr->random_access_data.suspension_FN_GTA != L1_GPRS_INVALID_FN ) && IS_FRAME_NUM_LATER(GSTMR_GET_FN_GERAN(gas_id), idle_data_ptr->random_access_data.suspension_FN_GTA))
         {
            MSG_GERAN_MED_3_G("GTA_PS_RACH :G2X TA not allowed suspension_FN %d, Non_Drx %d, vote %d ", idle_data_ptr->random_access_data.suspension_FN_GTA,idle_data_ptr->mm_non_drx_mode,gl1_is_geran_tuneaway_allowed(gas_id));
            idle_data_ptr->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;

            if((idle_data_ptr->random_access_data.updated_ra_delay != RACH_DELAY_INVALID) && 
               (idle_data_ptr->random_access_data.updated_ra_delay > 0) && 
               (TRUE == L1_determine_rach(idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB, frame_counters[gas_id].FNmod51,gas_id)))
            {
              MSG_GERAN_MED_1_G("GTA_PS_RACH :Rach pending to Tx after %d FN", idle_data_ptr->random_access_data.updated_ra_delay);
              /* calculate txlev */
              idle_data_ptr->random_access_data.txlev = calculate_txpwr_max_cch(idle_data_ptr->campedon_cell_ARFCN
                                                                        , gl1_get_rach_pwr_reduction(gas_id)
                                                                        , gas_id
                                                                        );
#ifdef FEATURE_GSM_COEX_SW_CXM
              /* Notify the arbitrator of the transition to ACCESS */
              garb_intf_notify_access(idle_data_ptr->campedon_cell_ARFCN, idle_data_ptr->random_access_data.updated_ra_delay, idle_data_ptr->random_access_data.txlev, gas_id );
#endif /* FEATURE_GSM_COEX_SW_CXM */
              /* Transmit the rach */
              gl1_msg_tx_rach ( idle_data_ptr->campedon_cell_ARFCN,
                                idle_data_ptr->random_access_data.txlev,
                                idle_data_ptr->random_access_data.updated_ra_delay,
                                idle_data_ptr->random_access_data.ra_content,
                                gas_id);

             }
         }

  #endif /*FEATURE_GTA_DURING_PS_RACH*/

      /* These seem to be required once we enter dedicated mode */
      l1_set_cch_ciphering_flag(FALSE, gas_id);
      dl_block_start = FALSE;

      /*Check whether the block belongs to UE's own paging group or not */ 
      (void)is_ccch_rx_block( idle_data_ptr, &own_paging_block, &chan_type, gas_id ); 

      switch ( frame_counters[gas_id].FNmod102 )
      {
      case 1:
      case 52:  /* BCCH */
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
        MSG_GERAN_HIGH_0_G("GPLT DBG: Not calling gl1_msg_rx_ccch");
#else
        if ( l1_rach_data_ptr->rach_read_all_AGCH )
        {
          arfcn = idle_data_ptr->campedon_cell_ARFCN;
          write_arfcns( rx_arfcns, arfcn );
          gl1_msg_rx_ccch( rx_arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                           agc_ptr_divrx_ccch,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                           1, own_paging_block, GL1_MSG_BCCH,
                           &l1_rach_data_ptr->rach_rpt,gas_id );

          dl_block_start = TRUE;
        }
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
        break;

      case 5:
      case 56:   /* Coming up is block 0 for the CCCH */

      case 11:
      case 62:   /* Coming up is block 1 for the CCCH */

      case 15:
      case 66:   /* Coming up is block 2 for the CCCH */
        if ( l1_rach_data_ptr->rach_read_all_AGCH )
        {
          arfcn = idle_data_ptr->campedon_cell_ARFCN;
          write_arfcns( rx_arfcns, arfcn );
          gl1_msg_rx_ccch( rx_arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                           agc_ptr_divrx_ccch,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                           1, own_paging_block, GL1_MSG_CCCH,
                           &l1_rach_data_ptr->rach_rpt,gas_id );

          dl_block_start = TRUE;
        }
        break;

      case 21:
      case 72:   /* Coming up is block 3 for the CCCH */

      case 31:
      case 82:   /* Coming up is block 5 for the CCCH */

      case 41:
      case 92:   /* Coming up is block 7 for the CCCH */

      case 25:
      case 76:   /* Coming up is block 4 for the CCCH */

      case 35:
      case 86:   /* Coming up is block 6 for the CCCH */

      case 45:
      case 96:   /* Coming up is block 8 for the CCCH */
        if ( (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE) &&
             l1_rach_data_ptr->rach_read_all_AGCH )
        {
          arfcn = idle_data_ptr->campedon_cell_ARFCN;
          write_arfcns( rx_arfcns, arfcn );
          gl1_msg_rx_ccch( rx_arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                           agc_ptr_divrx_ccch,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                           1, own_paging_block, GL1_MSG_CCCH,
                           &l1_rach_data_ptr->rach_rpt,gas_id );

          dl_block_start = TRUE;
        }
        break;

      case 100:
        /* Correct timing and frequency */
        gl1_msg_cch_tt_adjust( gas_id );
        gl1_msg_cch_afc_adjust( gas_id );
        break;

      default:
        break;

      } /* end of switch */



      /* Determine if the next frame is a RACH
      */
      ul_block_start =
         L1_determine_rach(idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB, frame_counters[gas_id].FNmod51,gas_id);

      gl1_msg_init_frame_tick_info(&ftn_type);
      ftn_type.next_frame_is_ul_block_start = ul_block_start;
      ftn_type.next_frame_is_dl_block_start = dl_block_start;

  #ifdef FEATURE_GTA_DURING_PS_RACH
        /*decrement the rach delay if rach is pending*/
        if((ul_block_start == TRUE) && 
           (idle_data_ptr->random_access_data.updated_ra_delay != RACH_DELAY_INVALID) && 
           (idle_data_ptr->random_access_data.updated_ra_delay > 0))
         {
           idle_data_ptr->random_access_data.updated_ra_delay--;
         }
  #endif
      (void)gpl1_gprs_serving_cell_meas(0,gas_id);

      gl1_msg_frame_tick_notification ( &ftn_type,gas_id );

#ifdef FEATURE_GSM_WTR_HOP
      if ( l1_rach_data_ptr->rach_read_all_AGCH )
      {
        gl1_msg_wtr_hop_state_machine(NULL, gas_id);
      }
#endif

      idle = FALSE;
        break;
#ifdef FEATURE_GTA_DURING_PS_RACH
     }

      case L1_ACCESS_GTA_SUSPENDING:
      {
        gl1_is_asdiv_switch_allowed[gas_id] = TRUE;
        grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
        grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;
        
        grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;
        grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
        grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(RANE_MIN_DURATION_MS, T_MSEC); /*To get the lock as soon as TDS releases*/
        grm_request_and_notify_enhanced_data_p->reason    = TRM_ACCESS;
        
        grm_request_and_notify_enhanced_data_p->resource  =
        gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );
        
        /*using the tbf supend tag as fo now as it only sets the trm status to granted and doesn nothing else on callback*/
        grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_TBF_SUSPEND; 
        
        grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);
        
        MSG_GERAN_HIGH_0_G("L1_ACCESS_GTA_SUSPENDING ");
        gl1_hw_multi_sim_deact_g2x(gas_id);
        
        /*release the diversity chain too*/
        grm_release_rx_sec(grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id,gas_id), gas_id);

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_TA_IN_RACH_STATE);
#endif

        grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p->client_id,
                                         grm_request_and_notify_enhanced_data_p->resource,
                                         grm_request_and_notify_enhanced_data_p->duration,
                                         grm_request_and_notify_enhanced_data_p->reason,
                                         gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id),
                                         grm_request_and_notify_enhanced_data_p->grm_tag );
        
        l1_tskisr_blk->firmware_loaded = FALSE;
  
        MSG_GERAN_HIGH_1_G("GTA_PS_RACH : SUSPENDED go to Wait for TRM %d",gl1_get_FN(gas_id));
  
        l1_tskisr_blk->sub_state = L1_ACCESS_GTA_TRM_WAIT;
        //idle_data_ptr->random_access_data.gta_ps_rach_enabled = TRUE;
  
        if((idle_data_ptr->random_access_data.updated_ra_delay != RACH_DELAY_INVALID) && 
           (idle_data_ptr->random_access_data.updated_ra_delay > 0)&& 
           (TRUE == L1_determine_rach(idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB, frame_counters[gas_id].FNmod51,gas_id)))
        {
          idle_data_ptr->random_access_data.updated_ra_delay--;
        }
  
#ifdef FEATURE_GSM_GPRS_L1
        (void)gpl1_gprs_serving_cell_meas(0,gas_id);
#endif
  
        idle_data_ptr->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
        break;
      }
  
      case L1_ACCESS_GTA_TRM_WAIT:
      {
        gl1_trm_state_t gl1_trm_state = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

        /* since we dont give FTN in this state , we need to decement the RACH delay so that 
               * when we resume we have the correct delay to use*/
        if((idle_data_ptr->random_access_data.updated_ra_delay != RACH_DELAY_INVALID) && 
           (idle_data_ptr->random_access_data.updated_ra_delay > 0)&& 
           (TRUE == L1_determine_rach(idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB, frame_counters[gas_id].FNmod51,gas_id)))
        {
          idle_data_ptr->random_access_data.updated_ra_delay--;
        }
  
        MSG_GERAN_MED_0_G("GTA_PS_RACH : Access L1_ACCESS_GTA_TRM_WAIT state");
  
        /*check if inactivity timer expired in this state*/
        if(--inactivity_timer[gas_id] <= 0)
        {
          MSG_GERAN_MED_0_G("GTA_PS_RACH : INACTIVITY timer expired in L1_ACCESS_GTA_TRM_WAIT state");
          
          idle_data_ptr->random_access_data.rach_for_ps_service = FALSE;
          
          /*send random acces cnf only if there was any pendign rach before suspension*/
          if(idle_data_ptr->random_access_data.updated_ra_delay != RACH_DELAY_INVALID)
          {
            MSG_GERAN_MED_0_G("GTA_PS_RACH : Sending PH_RANDOM_ACCESS_CNF_with_result GERAN_ACCESS_FAIL_NO_RETRY");
            L1_send_PH_RANDOM_ACCESS_CNF_with_result((gas_id_t)gas_id, GERAN_ACCESS_FAIL_NO_RETRY);
          }
  
          /*Stay in GTA TRM Wait state until lock grant / rach procedure abort from RR*/
          inactivity_timer[gas_id] = G2X_TUNEAWAY_DSDS_TIMEOUT_RACH;
        }
        else if ( (gl1_trm_state == GL1_TRM_GRANTED) || (gl1_trm_state == GL1_TRM_RETAINED_FOR_ACCESS) )
        {
          if( l1_tskisr_blk->firmware_loaded == FALSE )
          {
            gl1_multi_sim_fw_rf_load(l1_tskisr_blk->client_id, gas_id);
          }
          l1_tskisr_blk->sub_state = L1_ACCESS_GTA_FW_LOAD_WAIT;
        }
  
#ifdef FEATURE_GSM_GPRS_L1
        (void)gpl1_gprs_serving_cell_meas(0,gas_id);
#endif
  
      }
      break;
  
      case L1_ACCESS_GTA_FW_LOAD_WAIT:
      {
        /* we wont give FTN , we need to decement the RACH delay so that 
               * when we resume we have the correct delay to use*/
        if((idle_data_ptr->random_access_data.updated_ra_delay != RACH_DELAY_INVALID) && 
           (idle_data_ptr->random_access_data.updated_ra_delay > 0)&& 
           (TRUE == L1_determine_rach(idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB, frame_counters[gas_id].FNmod51,gas_id)))
        {
          idle_data_ptr->random_access_data.updated_ra_delay--;
        }
        
        if( l1_tskisr_blk->firmware_loaded == TRUE )
        {
          if(idle_data_ptr->random_access_data.updated_ra_delay != RACH_DELAY_INVALID)
          {
            idle_data_ptr->random_access_data.updated_ra_delay = 4;
            MSG_GERAN_MED_0_G("GTA_PS_RACH: RA delay set forcefully **** ");
          }
          
          /* initiate RF Tx band change - only gets actioned if band has actually changed */
          gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(idle_data_ptr->campedon_cell_ARFCN),gas_id);
          gl1_hw_turn_tx_on(gas_id);
          
          l1_tskisr_blk->sub_state = L1_ACCESS_INIT;
          
          /*when access init, use this delay*/
          if(idle_data_ptr->random_access_data.updated_ra_delay != RACH_DELAY_INVALID)
          {
            /*check if we have got any unlock_cb for suspension by this time*/
            if(idle_data_ptr->random_access_data.suspension_FN_GTA != L1_GPRS_INVALID_FN)
            {
              uint32 new_rach_delay =0;
              boolean is_rach_after_gap = FALSE;
              
              /*if collision*/
              if(TRUE == gl1_check_if_rach_delay_collides_with_gap(idle_data_ptr->random_access_data.updated_ra_delay,
                                                                    idle_data_ptr->random_access_data.suspension_FN_GTA,
                                                                    5,/*margin in the beginning*/
                                                                    G2X_PAGE_DUR_FRAMES,
                                                                    &new_rach_delay,
                                                                    &is_rach_after_gap,
                                                                    gas_id))
              {
                idle_data_ptr->random_access_data.updated_ra_delay = new_rach_delay;
                init_rach[gas_id] = TRUE;
              }
              /*if rach is after gap*/
              else if (is_rach_after_gap == TRUE)
              {
                /*dont need to do anything in this case*/
                MSG_GERAN_MED_1_G("GTA_PS_RACH : rach is after gap rach delay %d",idle_data_ptr->random_access_data.updated_ra_delay);
                idle_data_ptr->random_access_data.ra_delay = idle_data_ptr->random_access_data.updated_ra_delay;
                init_rach[gas_id] = TRUE;
              }
              /*rach is before the gap*/
              else
              {
                MSG_GERAN_MED_1_G("GTA_PS_RACH : rach is before gap rach delay %d",idle_data_ptr->random_access_data.updated_ra_delay);
                init_rach[gas_id] = TRUE;
              }
            }
            else
            {
              idle_data_ptr->random_access_data.ra_delay = idle_data_ptr->random_access_data.updated_ra_delay;
              init_rach[gas_id] = TRUE;
            }
          }
		  else if (FALSE == idle_data_ptr->random_access_data.rach_for_ps_service)
          {
               /*do init RACH if rach is for Cs*/
			   MSG_GERAN_MED_0_G("GTA_PS_RACH :recieved rach for CS");
               init_rach[gas_id] = TRUE;
		  }
          else
          {
            /*dont init RACH if no rach was pending before suspending*/
            init_rach[gas_id] = FALSE;
          }
          
          MSG_GERAN_MED_2_G("GTA_PS_RACH : L1_ACCESS_GTA_FW_LOAD_WAIT - fw loaded, move to access init rach_delay %d init_rach %d", 
                             idle_data_ptr->random_access_data.updated_ra_delay,init_rach[gas_id] );
        }
        else
        {
          MSG_GERAN_MED_0_G("GTA_PS_RACH : L1_ACCESS_GTA_FW_LOAD_WAIT");
        }
  
#ifdef FEATURE_GSM_GPRS_L1
        (void)gpl1_gprs_serving_cell_meas(0,gas_id);
#endif
  
      }
      break;
  
      case L1_ACCESS_GTA_INACTIVITY_TMR_EXPIRED:
      {
        gl1_trm_state_t gl1_trm_state = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        MSG_GERAN_MED_0_G("GTA_PS_RACH : L1_ACCESS_GTA_INACTIVITY_TMR_EXPIRED state");

        if( (gl1_trm_state == GL1_TRM_GRANTED) || (gl1_trm_state == GL1_TRM_RETAINED_FOR_ACCESS) )
        {
          MSG_GERAN_MED_0_G("GTA_PS_RACH : TRM granted back..move to init again");

          if( l1_tskisr_blk->firmware_loaded == FALSE )
          {
            gl1_multi_sim_fw_rf_load(l1_tskisr_blk->client_id, gas_id);
          }
          l1_tskisr_blk->sub_state = L1_ACCESS_GTA_FW_LOAD_WAIT;
        }
  
#ifdef FEATURE_GSM_GPRS_L1
        (void)gpl1_gprs_serving_cell_meas(0,gas_id);
#endif
  
        break;
      }
      
#endif /*FEATURE_GTA_DURING_PS_RACH*/


    default:
      /* Initialise the frame tick notification structure contents */
      gl1_msg_init_frame_tick_info( &ftn_type );

      /* Inform lower layers of TDMA frame */
      gl1_msg_frame_tick_notification ( &ftn_type,gas_id );

#ifdef FEATURE_GSM_WTR_HOP
      if ( l1_rach_data_ptr->rach_read_all_AGCH )
      {
        gl1_msg_wtr_hop_state_machine(NULL, gas_id);
      }
#endif

      MSG_GERAN_ERROR_1_G("Unexpected state %d", l1_tskisr_blk->sub_state);
      break;

    } /* end of switch */

  }
  else if ( mode == L1_MODE_STOP )
  {
    idle = TRUE;

    if(gl1_msg_check_rach_tx_in_prev_frame(gas_id))
    {
      MSG_GERAN_MED_0_G("RACH was transmitted in the previous frame, wait for a frame before access abort");
      return FALSE;
    }

    /* Clear the last_rach_tx_fn while aborting the Access state */
    gl1_msg_clear_last_rach_tx_fn(gas_id);

    /* Abort RACH Tx */
    gl1_msg_abort_rach(gas_id);

    /* Abort AGCH Rx */
    gl1_msg_abort_rx_ccch (gas_id);

    MSG_GERAN_MED_0_G("Access abort");

    if(l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND)
    {
      gl1_msg_reset(gas_id);
    }

    /* Initialise the frame tick notification structure contents */
    ftn_type.next_frame_is_dl_block_start             = FALSE;
    ftn_type.next_frame_is_ul_block_start             = FALSE;
    ftn_type.next_frame_is_ul_pdch_block_start        = FALSE;
    ftn_type.next_frame_is_dl_pdch_block_start        = FALSE;
    ftn_type.next_frame_is_ul_ptcch_sacch_block_start = FALSE;
    ftn_type.next_frame_is_dl_ptcch_sacch_block_start = FALSE;
    ftn_type.next_frame_is_acq_ncell_block_start      = FALSE;
    ftn_type.next_frame_is_pwr_meas_block_start       = FALSE;
    ftn_type.next_frame_is_int_meas_block_start       = FALSE;

    /* Inform lower layers of TDMA frame */
    gl1_msg_frame_tick_notification ( &ftn_type,gas_id );

#ifdef FEATURE_GSM_WTR_HOP
    /* Abort any hop in progress. Function gl1_msg_wtr_hop_abort 
        * will only return false if the RF enter mode command was issued,
        * in this case abort is not possible and hop state machine will need
        * to be ticked for the next two frames to complete the hop */
    if (!gl1_msg_wtr_hop_abort(FALSE, gas_id))
    {
      gl1_msg_wtr_hop_state_machine(NULL, gas_id);
      idle = FALSE;
    }
#endif
#ifdef FEATURE_GTA_DURING_PS_RACH

      /*reset the GTA suspension FNwhile stopping*/
      idle_data_ptr->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
      idle_data_ptr->random_access_data.rach_for_ps_service = FALSE;
      idle_data_ptr->random_access_data.updated_ra_delay = RACH_DELAY_INVALID;
      init_rach[gas_id] = TRUE;

#endif

#ifdef FEATURE_GSM_RX_TX_SPLIT
    if ( ( l1_tskisr_blk->main_command != L1_TRANSFER_COMMAND ) &&
         ( l1_tskisr_blk->main_command != L1_DEDICATED_COMMAND ) )
    {
      if ( l1_rf_tx_enable_cnf_rcv[gas_id] == TRUE ) /* Before disabling Tx, Checking that if RF_TX_ENABLE_CNF is received from RF */
      {
        /* disable tx and release TRM lock for Tx client.*/
        (void)gl1_hw_turn_tx_off( TRUE, gas_id );

        if (gl1_msg_get_multi_sim_mode())
        {
           grm_release_tx_chain(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id); 
        }
      }
      else
      {
        /* Continue to be in same state as RF_TX_ENABLE_CNF is not yet received from RF */
        idle = FALSE;
      }
    }
 #endif /*FEATURE_GSM_RX_TX_SPLIT*/
  }

  return(idle);
}

/*===========================================================================

FUNCTION  L1_DETERMINE_TIME

DESCRIPTION
  This function determines whether the starting time has occured

DEPENDENCIES
  Frame count mod 42432 , starting time

RETURN VALUE
  started

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_determine_time(word Mod42432, word starting_time)
{
  /* Locals */
  sdword    frame_diff;

  frame_diff = (sdword)(Mod42432 - starting_time);


  /* Anyone simplifying this is welcome to it*/
  if ( (( frame_diff >=0 ) &&
        (frame_diff < 21216))||
       (frame_diff < -21216) )
  {
    return(TRUE);
  }

  else
  {
    return(FALSE);
  }
}

/*===========================================================================

FUNCTION  L1_DETERMINE_RACH

DESCRIPTION
  This function determines whether the next frame is a RACH.

DEPENDENCIES
  modulo 51 frame count

RETURN VALUE
  boolean indicating RACH next

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_determine_rach
(
  boolean ccch_sdcch_comb,    /* Is it a ccch/sdcch channel combo? */
  byte fn_mod51,               /* FN modular 51 */
  gas_id_t gas_id
)
{
  boolean rach_is_next = FALSE;

  if ( ccch_sdcch_comb == FALSE )
  {
    rach_is_next = TRUE;
    gl1_msg_inc_rach_frame_cnt(gas_id);
  }
  else
  {
    if ( (( fn_mod51 > 2  ) && ( fn_mod51 < 5  )) ||
         (( fn_mod51 > 12 ) && ( fn_mod51 < 36 )) ||
         (( fn_mod51 > 43 ) && ( fn_mod51 < 46 )) )
    {
      MSG_GERAN_LOW_1_G("FN mod 51: %d", fn_mod51);
      rach_is_next = TRUE;
      gl1_msg_inc_rach_frame_cnt(gas_id);
    }
  }

  return( rach_is_next );
}

/*===========================================================================

FUNCTION  L1_is_rach_next

DESCRIPTION
  This function returns TRUE if next frame is RACH.

DEPENDENCIES

RETURN VALUE: boolean

SIDE EFFECTS

===========================================================================*/
boolean L1_is_rach_next ( uint16 fn_mod51, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  boolean rach_is_next = FALSE;

  if ( idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE )
  {
    rach_is_next = TRUE;
  }
  else
  {
    if ( (( fn_mod51 > 2 ) && ( fn_mod51 < 5 )) ||
         (( fn_mod51 > 12 ) && ( fn_mod51 < 36 )) ||
         (( fn_mod51 > 43 ) && ( fn_mod51 < 46 )) )
    {
      rach_is_next = TRUE;
    }
  }

  return( rach_is_next );
}

/*===========================================================================

FUNCTION  rach_callback

DESCRIPTION
  This callback function is invoked by the rach handler when the RACH has
  been transmitted.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void rach_callback ( uint16 random_content, gas_id_t gas_id
#ifdef FEATURE_GSM_COEX_RACH_DELAY
, boolean yield
#endif
)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_rach_data_t *l1_rach_data_ptr = &l1_rach_data[gas_id];

  uint8 i = 0;
  ARFCN_T arfcns[4];
  int16 txlev[4], txlev_temp;
  idle_data_T               *idle_data_ptr;

  /* temp place holder for TX PWR to write Eng Mode Info */
  int16 tmp_tx_pwr[8] = {0};

#ifdef FEATURE_GSM_COEX_RACH_DELAY
  if(yield)  
  {
     L1_send_PH_RANDOM_ACCESS_CNF_with_result(gas_id, GERAN_ACCESS_BLANKED_BY_CXM);
     return;
  }
#endif

  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

#ifdef FEATURE_GSM_COEX_RACH_DELAY
  if(gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
  {
    MSG_GERAN_HIGH_1_G("SUB_MOD(frame_counters[gas_id].FNmod42432, 1, 42432) = %d", SUB_MOD(frame_counters[gas_id].FNmod42432, 1, 42432));
    L1_send_PH_RANDOM_ACCESS_CNF(random_content, SUB_MOD(frame_counters[gas_id].FNmod42432, 1, 42432), gas_id);
  }
  else
#endif
  {
    L1_send_PH_RANDOM_ACCESS_CNF(random_content, frame_counters[gas_id].FNmod42432, gas_id);
  }

#ifdef FEATURE_GTA_DURING_PS_RACH
    idle_data_ptr->random_access_data.updated_ra_delay = RACH_DELAY_INVALID;
#endif

  l1_rach_data_ptr->rach_read_all_AGCH = TRUE;
  txlev_temp = calculate_txpwr_max_cch(idle_data_ptr->campedon_cell_ARFCN
                                       ,gl1_get_rach_pwr_reduction(gas_id)
                                       ,gas_id
                                       );
  for ( i = 0 ; i < 4; i++ )
  {
     arfcns[i] = idle_data_ptr->campedon_cell_ARFCN;
     txlev[i] =  txlev_temp;
  }

  /* Log tx burst metrics */
  l1_log_transmit_burst_metrics
  ( GL1_MSG_RACH,
    0, 1, 1,
    arfcns,
    txlev,
    idle_data_ptr->campedon_cell_BSIC & 0x0007, /* TSC = BCC = BSIC & 0x0007 */
    0, gas_id );

  tmp_tx_pwr[0] = txlev[0];
  geran_eng_mode_data_write(ENG_MODE_TX_PWR, tmp_tx_pwr, TRUE, gas_id);

#ifdef FEATURE_GSM_COEX_RACH_DELAY
  if(yield)
  {
    MSG_GERAN_MED_2_G("RACH(%d) delayed due to CXM yield (FN=%d)", random_content, frame_counters[gas_id].FNmod42432);
  }
#else
  {
    MSG_GERAN_MED_2_G("RACH(%d) sent (FN=%d)", random_content, frame_counters[gas_id].FNmod42432);
  }
#endif

}


/*===========================================================================

FUNCTION agch_rx_callback

DESCRIPTION
  This function is called when the data from the CCH has been decoded. It
  sends a message to L2 indicating if message is good or not.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void agch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* Channel type should be of AGCH, but no AGCH type CCCH is defined.
   RR suggested to use L2_RACH to distinguish CCCH block of type PCH and AGCH
   CCCH -> PCH, L2_RACH -> AGCH*/
  byte chan_type = L2_RACH;

  if (!rpt)
  {
    MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
    return;
  }

  /* Include AGCH in Idle AEQ on/off determination, so update RXqual full on this callback */
  gl1_msg_update_hard_rxqual_ave( (uint16)rpt->ccch.hdr.rx_qual, gas_id );

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
  // In case of GPLT and no RF, return default/canned result
  MSG_GERAN_HIGH_0_G("GPLT DBG: agch_rx_callback: making rpt->ccch.hdr.good_data TRUE");
  rpt->ccch.hdr.good_data = TRUE;
  MSG_GERAN_HIGH_1_G("GPLT DBG: agch_rx_callback: rpt->ccch.own_paging_block = %d", rpt->ccch.own_paging_block);
  memscpy(rpt->ccch.data, sizeof(rpt->ccch.data),gplt_imm_assign_result.data, sizeof(gplt_imm_assign_result.data));
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

  if ( rpt->ccch.hdr.good_data )
  {
#ifdef FEATURE_GL1_GPLT
    if(gplt_canned_nw_data) 
    {
      MSG_GERAN_HIGH_0_G("GPLT DBG: Canned Immediate Assignment data");
      memscpy(rpt->ccch.data, sizeof(rpt->ccch.data),gplt_imm_assign_result.data, sizeof(gplt_imm_assign_result.data));
    }
#endif
    L1_send_PH_DATA_IND(TRUE,
                        rpt->ccch.hdr.chan,
                        rpt->ccch.own_paging_block,
                        rpt->ccch.arfcn,
                        rpt->ccch.data,
                        23, gas_id);

    MSG_GERAN_MED_3_G("AGCH decoded f=%d FN102=%d FN=%d",
            rpt->ccch.arfcn.num, frame_counters[gas_id].FNmod102, gl1_get_FN( gas_id ));

    if( FALSE != gprs_serv_cell_meas_ptr->cal_c_value_51_idle )
    {
      /* If c value calculations are required on
      * the ccch logical channel then call
      * the serv cell data function.
      */
      gpl1_scell_cs_idle_data_cb( TRUE, gas_id );
    }
    /* Send block Quality indication for its own paging group*/
    if(rpt->ccch.own_paging_block)
    {
      L1_send_MPH_BLOCK_QUALITY_IND( chan_type,
                                     GL1_BLOCK_QUALITY_GOOD, 
                                     gas_id);
    }
  }
  else
  {
    /* Bad Block received */
    L1_send_PH_DATA_IND(FALSE,
                        rpt->ccch.hdr.chan,
                        rpt->ccch.own_paging_block,
                        rpt->ccch.arfcn,
                        NULL,
                        0, gas_id);

    MSG_GERAN_ERROR_3_G("AGCH failed arfcn=%d FN102=%d FN=%d",
              rpt->ccch.arfcn.num, frame_counters[gas_id].FNmod102, gl1_get_FN( gas_id ));

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
	/*if the disable access bit is set, do not do switch*/
    if( (gl1_trm_asdiv_enable(gas_id,GL1_TRM_ASDIV_TYPE1) || gl1_trm_asdiv_enable(gas_id,GL1_TRM_ASDIV_TYPE2)) && ((gl1_antenna_switch_control_mask & DISABLE_ACCESS_ANT_SWITCH) == FALSE)
        && gl1_is_asdiv_switch_allowed[gas_id])
	{
	  gl1_hw_switch_antenna_tx_div_ccch_fail(FALSE, gas_id);
        }
#endif

    if( FALSE != gprs_serv_cell_meas_ptr->cal_c_value_51_idle )
    {
      /* If c value calculations are required on
      * the ccch logical channel then call
      * the serv cell data function.
      */
      gpl1_scell_cs_idle_data_cb( FALSE, gas_id );
    }
    /* Send block Quality indication for its own paging group*/
    if(rpt->ccch.own_paging_block)
    {
      L1_send_MPH_BLOCK_QUALITY_IND(chan_type,
                                    GL1_BLOCK_QUALITY_BAD, 
                                    gas_id);
    }
  }

  /* LOG MESSAGE METRICS -- Note:AGCH is a form of CCCH */
  l1_log_message_metrics ( GL1_MSG_CCCH, rpt, gas_id );
  l1_log_message_metrics_compact(GL1_MSG_CCCH, rpt, gas_id);
}

static void agch_metrics_callback(gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id)
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
  int      i;
  dBx16_T  snr_array[4];
  dBx16_T  pwr_dBm_x16 = GL1_INVALID_POWER_LEVEL;
  dBx16_T  avg_pwr_dBm_x16 = GL1_INVALID_POWER_LEVEL;
  uint8    no_of_measurements = 0;
  gl1_power_meas_type pwr_meas_type = PRX_MEAS;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  dBx16_T  divrx_pwr_dBm_x16 = GL1_INVALID_POWER_LEVEL;
  dBx16_T  avg_divrx_pwr_dBm_x16 = GL1_INVALID_POWER_LEVEL;
  dBx16_T  divrx_power_array[GL1_DEFS_FRAMES_IN_BLK_PERIOD] = {GL1_INVALID_POWER_LEVEL};
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  if (!rpt)
  {
     MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
     return;
  }


  MSG_GERAN_HIGH_3_G("AGCH,te=%d,fe=%d,Arfcn=%d",
             rpt[0]->timing_offset,
             rpt[0]->freq_offset,
             rpt[0]->arfcn.num);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if(gl1_get_IMRD_ctl_nv(gas_id))
  {
    pwr_meas_type = PRX_DIVRX_MEAS;
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  /* Process freq. offset */
  for ( i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++ )
  {

     /* If SAIC is used in a burst the mDSP sets bit 14 of */
     /* the misc_flag true. Additionally, for AFC/TT track */
     /* the flag selects the specific GMSK SAIC threshold. */
     if ( gl1_msg_saic_in_burst( rpt[i]->misc_flags ) )
     {
        gl1_msg_cch_tt_update( rpt[i]->timing_offset,
                               gl1_msg_saic_tt_threshold_snr( rpt[i] ), TRUE ,gas_id);

        gl1_msg_cch_afc_update( rpt[i]->freq_offset,
                                gl1_msg_saic_afc_threshold_snr( rpt[i] ), TRUE ,gas_id);
     }
     else
     {
        gl1_msg_cch_tt_update( rpt[i]->timing_offset, rpt[i]->snr, FALSE ,gas_id);
        gl1_msg_cch_afc_update( rpt[i]->freq_offset, rpt[i]->snr, FALSE ,gas_id);
     }

     pwr_dBm_x16 += rpt[i]->pwr_dBm_x16;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
     if(PRX_DIVRX_MEAS == pwr_meas_type)
     {
       if (rpt[i]->pwr_dBm_x16_divrx != GL1_INVALID_POWER_LEVEL) 
       {
         divrx_power_array[i] = rpt[i]->pwr_dBm_x16_divrx;
       }
       else
       {
         divrx_power_array[i] = rpt[i]->pwr_dBm_x16;
       }
       divrx_pwr_dBm_x16 += divrx_power_array[i];
     }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
     no_of_measurements++;
     
  }
  
  avg_pwr_dBm_x16 = (pwr_dBm_x16/no_of_measurements);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if(PRX_DIVRX_MEAS == pwr_meas_type)
  {
    avg_divrx_pwr_dBm_x16 = (divrx_pwr_dBm_x16/no_of_measurements);
    l1_serving_cell_meas_ptr->rx_power = MAX(avg_pwr_dBm_x16, avg_divrx_pwr_dBm_x16);
  }
  else
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
  {
    //update the average rx pwr
    l1_serving_cell_meas_ptr->rx_power = avg_pwr_dBm_x16;
  }

  l1_serving_cell_meas_ptr->no_of_measurements = 1;

  if( FALSE != gprs_serv_cell_meas_ptr->cal_c_value_51_idle )
  {
      /* If c value calculations are required on
       * the ccch logical channel then call
       * the serv cell metrics function inorder
       * to accumulate the rssi measurements.
       */
      uint8 burst_num = L1_BURST_1;
      const gl1_msg_metrics_rpt*  burst_metrics_ptr = rpt[ L1_BURST_1 ];

      while( burst_num < GL1_DEFS_FRAMES_IN_BLK_PERIOD )
      {
          gpl1_scell_cs_idle_metrics_cb( burst_num,
                                         burst_metrics_ptr->pwr_dBm_x16, 
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
                                         divrx_power_array[burst_num],
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
                                         pwr_meas_type,
                                         gas_id );
          burst_num++;
          burst_metrics_ptr++;
      }
  }

  /* Output a debug message showing the SNRs */
  snr_array[0] = gl1_msg_SNR_to_dBx16( rpt[0]->snr );
  snr_array[1] = gl1_msg_SNR_to_dBx16( rpt[1]->snr );
  snr_array[2] = gl1_msg_SNR_to_dBx16( rpt[2]->snr );
  snr_array[3] = gl1_msg_SNR_to_dBx16( rpt[3]->snr );
  l1_snr_msg( snr_array, 4, gas_id );

  l1_log_burst_metrics ( GL1_MSG_CCCH, rpt, gas_id  );

}
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================
FUNCTION l1_rach_timer_create

DESCRIPTION
  creates/defines the rach disable timer
===========================================================================*/
void l1_rach_timer_create( gas_id_t gas_id )
{ 
  
  l1_rach_data_t *l1_rach_data_ptr = &l1_rach_data[gas_id];

  /* Create a timer with handler */
  memset(&(l1_rach_data_ptr->l1_rach_disable_timer), 0, sizeof(rex_timer_type));
  rex_def_timer_ex( &(l1_rach_data_ptr->l1_rach_disable_timer),
                    (rex_timer_cb_type)l1_rach_disable_timer_handler,
                    gas_id );
 
}
/*===========================================================================
FUNCTION l1_rach_disable_timer_handler

DESCRIPTION
  handler for the rach disable timer
===========================================================================*/
static void l1_rach_disable_timer_handler( gas_id_t gas_id )
{
  call_in_task_rach_timeout_handler(gas_id);
}

/*===========================================================================
FUNCTION l1_rach_timer_start

DESCRIPTION
  start the rach disable timer
===========================================================================*/
void l1_rach_timer_start( gas_id_t gas_id )
{
  l1_rach_data_t *l1_rach_data_ptr = &l1_rach_data[gas_id];
  MSG_GERAN_HIGH_0_G("RACH start timer");
  if(GERAN_TRAFFIC_MO_SMS == geran_get_traffic_reason(NULL, gas_id))
  {
    (void)rex_set_timer(&l1_rach_data_ptr->l1_rach_disable_timer,
                        L1_RACH_DISABLE_FOR_SMS_TIMEOUT);
  }
  else
  {
    (void)rex_set_timer(&l1_rach_data_ptr->l1_rach_disable_timer,
                        L1_RACH_DISABLE_TIMEOUT);
  } 
}
/*===========================================================================
FUNCTION l1_rach_timer_deactivate

DESCRIPTION
  deactivate the rach disable timer
===========================================================================*/
void l1_rach_timer_deactivate( gas_id_t gas_id )
{

  l1_rach_data_t *l1_rach_data_ptr = &l1_rach_data[gas_id];
  /* Disable the timer */
  timer_attrib_struct_type gsm_l1_timeout_timer_attrib;

  /* Get the params for the timer */
  timer_get_params(&(l1_rach_data_ptr->l1_rach_disable_timer), &gsm_l1_timeout_timer_attrib);

  /* Timer to use to handle any timeout in TRM/TCXO grants */
  if ( gsm_l1_timeout_timer_attrib.tcb_sigs_obj != NULL )
  {
    MSG_GERAN_HIGH_0_G("RACH deactivate timer");
    (void)rex_clr_timer(&l1_rach_data_ptr->l1_rach_disable_timer);
  }
}

/*===========================================================================

FUNCTION   call_in_task_rach_timeout_handler

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   that will handle the tiemout of rach timer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void call_in_task_rach_timeout_handler( gas_id_t gas_id )
{
  gl1_rach_timeout_callback_t msg;

  msg.message_header.message_set = MS_L1_L1;
  msg.message_header.message_id  = L1_HANDLE_RACH_TIMEOUT;
  msg.gas_id             = gas_id;


  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.message_header ), &msg ); 


  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );

}


#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

