/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
   This file contains the DTM section of the GL1 frame layer.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2003-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_dtm.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ---------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
30/01/17   mk       CR1113152 Keep RxD always enabled in FTM mode based on RxDForcedON NV
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
08/12/16   mk       CR1097591 Enhancement for DRX: DRx and PRx difference will be applied to neighbor cell measurements, if DRx power level is greater than PRx
23/08/16   bg       CR1045621 Updating Rx Div Ant parameters in Engineering mode
08/02/16   mn       CR872002 GL1 changes for VSWR feature
22/10/15   zf       CR923858 Rework Sleep error recovery in QTA 
28/09/15   pg       CR909305 Update dynamic RxD switching and remove pwr_meas suspension  
18/09/15   cws      CR907895 Change unnecessary F3 to LOW 
01/09/15   jk       CR899990:Enable LIF by default in FTM mode 
12/08/15   pa       CR887572: RxD: Schedule DRX wakeup after Rx/Tx and request TRM in task context. 
14/02/15   smd      CR794015 Added support for RF self test
08/07/15   cja      CR868167 For dynamic EGPRS AEq reduce clock bump after block is complete
29/05/15   pa       CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD. 
09/04/15   cgc      CR819956 G2T changes to support new RF API's  FEATURE_NEW_RF_API
02/04/15   pg       CR810444 Adjust AEQ/RxD dynamic switching and add support for DRx IQ scaling 
26/03/15   df       CR811822 Prop error in CR701894 - EPD always disabled
12/03/15   pa       CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
11/03/15   pa       CR805850: Validate Rx signal strength before averaging expected power and signal strength. 
07/01/15   jj       CR777805 Add probe burst flag for diversity rx
27/02/15   cja      CR768169 Add RF Seq Number
23/02/15   jk       CR793098:LIF additional logging changes
27/02/15   am       CR646584 WLAN coex
26/01/15   pa       CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs. 
21/01/15   jk       CR782488:Use updated gain tags for PRx and DRx in DTM mode
21/01/15   pa       CR758792:RxD support for multi-sim.
07/01/15   zc       CR776264 GL1 updating gain_ptr while rf is processing rx burst
16/12/14   jk       CR753423:LIF for Idle,voice and data
15/12/14   pg       CR630281 implementation of RxD support for data/MSRD 
03/12/14   pg       CR765847: Add support of Idle RxD for AGCH and NCELL SCH 
13/11/14   pa       CR755557:Support for granted band/rf device in Idle mode RxD.
13/11/14   jk       CR735025:Remove unncecessary LIF debug F3s
05/11/14   fan      CR751146 Fix wakeup error recovery mechanism on Thor
31/10/14   xz       CR748816 Extend dual data space for building RF script variable 
24/09/14   jk       CR729382:Enable LIF feature by default
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
21/10/14   fan      CR734490 Rework the wake up recovery mechanism
10/10/14   cos      CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes 
07/10/14   pg       CR730132 F3 cleanup 
01/10/14   df       CR611845 SBD interface updates for missed wakeup
12/09/14   fan      CR636420: Run Sleep commit after GSTMR ISR at wakeup
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14   nm       CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
09/09/14   cos      CR720965 adding support for DR-DSDS - rfdev 2 and 3 handling wtr to RxD and calibration
04/09/14   am       CR682760 Addressing KW GL1 P1 errors
29/08/14   cos      CR 716812 - Adding missing changes for imrd framework
14/04/14   pjr      CR641577 Replaced FN with Seq Number in cxm log table
07/08/14   cja      CR643648 Desense for channel adjacent to spur
11/02/14   jk        CR614254:GL1 changes to support IP2 calibration
11/07/14   pjr      CR 692177 Added support for RFACCH 2.0
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
22/05/14   nk       CR667961 Make ab_tsc crash code FTM_MODE proof
24/07/14   jk       CR699271:GL1 LIF changes for RxD
15/05/14   jj       CR 664355 Memclr rx_chan_params to avoid junk values in rx params send to rf 
16/07/14   jk       CR695230:Reset enable_rxdiversity variable while exiting DRx mode
02/07/14   cos      CR 688739 - Residual BER in RxD mode when PRx is disconnected
27/06/14   cos      CR686719: Cleaning up AEQPH4 GL1 control info at call exit
18/06/14   cos      CR682031 - GL1 control algorithm and changes for supporting AEQ PHASE 4
11/06/14   npt      CR630396 Wake-up error recovery mechanism
01/05/14   nk       CR656822 Debug prints/crash to catch when ab_tsc is greater than 2
17/04/14   cos      CR636543 - Changes for enabling Rx Diversity on Bolt - GL1/GRF intf updates
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
21/03/14   ws       CR622270 FTM specific - Don't Tx if num_tn = 0
17/03/14   jk       CR629569:Incorrect gain values used in DTM
19/02/14   jk       CR608290:GERAN RxD ON/OFF adaptive algorithm 
03/02/14   cos      CR475069 GL1 to indicate rxlev to firmware; merged on Bolt 
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity 
02/01/13   dv       CR592228 RxAGC flooring after few QTAs 
10/12/13   ws       CR 587586 - Remove DUAL_SIM featurisation in API
09/11/13   mc       CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
12/11/13   pjr      CR571992 Log correctly coex_backoff_desense into CXM log table
18/10/10   pjr      CR563534 Fix incorrect channel type shown in CXM logs packets
22/11/13   cja      CR455527 Remove featurisation from files in API directory
11/18/13   pjr      CR563534 Fix incorrect channel type shown in CXM logs packets
17/09/13   pjr      CR538893 Changes to enable CXM logging
03/10/13   ws       CR499846 - Bug fix for RACH PWR reduction feature
23/05/13   pg       CR460555: Enable 3GPP Rel10 RACH Power Reduciton feature  
13/09/13   jk       CR413653:Set flag spurChanFlag for desense channel
15/08/13   sk       CR524039 GBTA bringup changes
29/07/13   sk       CR519663 Partial QBTA bringup changes
12/07/13   cs       CR509747 Apply frequency drift to TRM_DENIED SUB(s)
18/06/13   cs       Major TSTS Triton Syncup
17/05/13   cs       TSTS Updates
10/04/13   ap       472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
17/05/13   kb/npt   CR385548 - Decode PCH after sleep wakeup error recovery.
02/05/13   jk       CR482767:Fixed missing rxgain pointer initialization in gl1_hw_dtm_process_rx( )
01/05/13   jk       CR466056:Changes to support Rx busrt on any time slot in FTM mode
05/04/13   mc       CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc       CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc       CR.459487 : COEX Support - GERAN arbitrator framework
06/02/13   jk       CR449735: Resolving KW errors
10/12/12   cja      CR411308 Dime G2T changes (msg router to GFW)
05/12/12   pg       CR427361: GFW/RF shared memory interface cleanup
26/11/12   pg       CR424194: updates for AFC and AGC when using RF Task
29/06/12   jj       CR374950 GL1 changes for 38 symbol HMSC monitor
20/06/12   ky       Added support of using new CCS interface and passing new CCS
                    buffer pointers in RF task messages
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
25/05/12   pg       Add support for RF Task
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
13/01/12   ws       Added Vamos Feature
29/09/11   jj       CR308254 GL1 Changes for GSM DC spur removal
20/05/11   ws       Fixed compile warnings for Nikel
10/05/11   ky       Enhancing the handler logging for rf tx function
03/05/11   cja      For Nikel change to use rfm_set_tx_band
28/04/11   cja      Add Nikel freq control by GFW
28/04/11   pg       Fix compiler warnings
20/04/11   cja      Nikel - add spectral inversion setting
11/03/11   pg       CR278827: Add RSSI signal info to Eng Mode IF
23/02/11   pg       CR276800 Add RSSI DIAG logging functionality
18/01/11   pg       CR270330 Add FEATURE_GSM_DYNAMIC_AEQ_8PSK
24/11/10   cja      Integrate HMSC
17/11/10   cja      Fix Klocwork issues
14/10/10   ws       FEATURE_GSM_MDSP_AEQ_8PSK - support AEQ Ph2
03/09/10   ws       Added FEATURE_GSM_MDSP_AEQ_SCH for QDSP6 targets
29/06/10   ky       Added the performace logging for rf driver apis
27/04/10   cs       CR236460 Latest AFC\XO handling updates
17/03/10   ws       Added featurisation to seperate Data mover code from FEATURE_GSM_MDSP_IR
04/02/09   ws       Removed leagcy Q4 MDSP services and image for Q6 targets
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
02/06/09   ws       Added missing setting of params.dedicated in gl1_hw_dtm_process_rx()
29/05/09   ws       FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
22/01/09   cs       Fix the WPLT build which fails due to previous change
07/01/09   nt       Added featurisation around the variable iq_sample_force_disable
                    for compiler warning
08/12/08   cs       Add in new gl1_hw_debug.h from split gl1_hw.h
03/12/08   ws       Added FEATURE_GSM_DYNAMIC_SAIC
20/11/08   ws       CR164614 - Stop access to MDSP_dmComplete when in FTM in case GSM clock is off
30/10/08   ws       CR 162112 - ESAIC Only reset IIR filter on PCH and NCELL BCCH
02/10/08   cs       Lint fixes
21/04/08   ws       CR 144248 Enable and disable ESAIC in gl1_msg_set_egprs_mode() and
                    improve F3 for SAICS status
19/03/08   agv      LCU RF Spur Interface Support.
14/02/08   nf       Added feature enabling other Tech L1's to find if GSM is in a TX state
14/01/08   cs       Allow for separate IQ logging and saving
24/12/07   nt       Added the GCF flag to the mDSP interface for async/sync_rx cmds
13/12/07   agv      CR 131982 Fix for EGPRS DM causing sleep nacks.
09/11/07   nf       Added FEATURE_GSM_R_SACCH
17/09/07   agv      Added I/Q logging to code base.
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
26/07/07   ws       Resolve bad merge in previous checkin
17/07/07   ws       CR123746 Add LNA_GAIN_RANGE to rfm_set_rx_gain() API
06/07/07   agv      CR123023 - Update fix for AFC/AGC corruption.
27/06/07   agv      CR123023 Fix for AGC corruption, and TT.
29/06/07   cs       Fix WPLT build for CR122077
26/06/07   nt       CR122077 - Use IS_FTM_IN_TEST_MODE() to qualify if we are in
                    FTM mode and therefore should set zero values to AGC as this
                    is GAIN_STAGE 0
12/06/07   nt       CR118485: Reverted the previous checkin, for new fix
23/05/07   nt       CR118485: Stops the SDCCH rx/tx whilst aborting, and alligns
                    on block boundary.
29/05/07   njt      CR118309 - Added missed merged lines to RSSI variation initial fix
23/05/07   agv      CR118309 - Large RSSI variation initial fix.
15/05/07   cs       Fix possible buffer overruns with bad slot configs
27/03/07   dv       Added FEATURE_GSM_R_FACCH
23/01/07   agv      Switch SAIC off if Idle Mode to reduce current draw.
19/12/06   cs       Extra fix for IQ logging, avoids disabling active channel
28/11/06   ws       High Lint warning fixes
13/10/06   cjs      CR101170 Fix the IQ logging for MDSP_SAIC and IQ_LOGGING
31/07/06   agv      CR85810 NB AB TSC, and TA fix.
29/03/06   agv      CR88298 - Temp fix for SAIC logging bug wrt TS and seq error.
03/23/06   ws       Added Support for SDA algorithm SAIC Disable Algorithm
16/03/06   og       Take timeslot gaps into account when deciding on Tta/Trb
                    or Ttb/Tra. Resolves CR 88935 and CR 88975.
02/17/06   gfr      Added modulation for power backoff to RF Driver.
01/13/06   gfr      Support for new RFM API (FEATURE_RFM_INTERFACE)
10/12/05   gfr      Add support for GSTMR tracing
09/29/05   gfr      Optimize tx clock on period
08/17/05   gfr      QCLint cleanup - shorten lines to 90 chars or less

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "gl1_hwi.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "rfm.h"
#include "geran_msgs.h"
#include "ftm.h"
#include "gl1_msg_pdch.h"
#include "mcfg_nv_api.h"
#include "l1_utils.h"
#include "l1_task.h"
#include "l1_log.h"

#include "gl1_hw_arbitration.h"

#include "gl1_hw_sleep_ctl.h"
#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "ftm.h"
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/
/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/

boolean gl1_hw_async_rx_saic[NUM_GERAN_DATA_SPACES];

/* If mDSP sends 0 RSSI. Initialise to -70 dBm */
static int16 last_good_agc[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE(GL1_RXD_AGC_PWR_LEVEL) };

#ifdef FEATURE_GSM_RX_DIVERSITY
static int16 last_good_agc_divrx[NUM_GERAN_DATA_SPACES] =  
               { INITIAL_VALUE(GL1_RXD_AGC_PWR_LEVEL) };
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
static int16 last_good_data_agc_divrx[NUM_GERAN_DATA_SPACES] =  
               { INITIAL_VALUE(GL1_RXD_AGC_PWR_LEVEL) };
#endif
static boolean enable_rx_diversity [NUM_GERAN_DATA_SPACES] =  
               { INITIAL_VALUE( FALSE ) };
static gl1_rxd_control_type gl1_rxd_control_flag[NUM_GERAN_DATA_SPACES] =  
               { INITIAL_VALUE_S( 0 ) };
static uint16 gl1_total_burst_cnt[NUM_GERAN_DATA_SPACES] =  
               { INITIAL_VALUE( 0 ) };
static uint16 gl1_divrx_sel_cnt[NUM_GERAN_DATA_SPACES] =  
               { INITIAL_VALUE( 0 ) };
static uint8 gl1_last_divrx_stream_avg[NUM_GERAN_DATA_SPACES] =  
               { INITIAL_VALUE( 0 ) };
#endif

#ifdef FEATURE_AEQ_PHASE4
static gl1_hw_dtm_rx_aeq4_type mdsp_aeq4_ctl[NUM_GERAN_DATA_SPACES] =  
                            { INITIAL_VALUE_S ( 0 ) };
#endif /*FEATURE_AEQ_PHASE4 */

#if defined (FEATURE_GSM_MDSP_IR)&& !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
/* Needed to sync drivers and mDSP for EDGE Rx. */
static boolean dm_complete[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
#endif

/* Storage for information for single DTM RX slot */
typedef struct
{
   boolean                       configured;
   mdsp_dtm_rx_slot_params_type  mdsp_params;
   gl1_hw_rx_tags_type           *tags;
   gl1_hw_channel_type           channel_type;
   /* co-ex params for this slot */
   uint32                        coex_desense_id;
   uint32                        coex_priority;
} gl1_hw_dtm_rx_slot_info_type;


/* Storage for DTM RX parameters */
typedef struct
{
   boolean                        saic;        /* SAIC On/Off */
   boolean                        dedicated;
   boolean                        idle;
   boolean                        configured;
   ARFCN_T                        arfcn;
   int16                          expected_power;
#ifdef FEATURE_GSM_RX_DIVERSITY
   int16                          expected_power_divrx;
#endif
   uint8                          expected_gain_range; /* Only use in FTM mode or calibration */
#ifdef FEATURE_GSM_RX_DIVERSITY
   uint8                          expected_gain_range_divrx;
#endif
   uint8                          tsc;
   uint16                         search_width;
   gl1_hw_dtm_rx_slot_info_type   slot_info[GL1_DEFS_SLOTS_IN_FRAME];
   uint8                          num_of_page_bursts_missed;
#ifdef FEATURE_GSM_RX_DIVERSITY
   boolean                        is_block_start;
#endif
} gl1_hw_dtm_rx_params_type;

/* Storage for information for single DTM TX slot */
typedef struct
{
   boolean                  configured;
   int16                    power_index;
   /* Modulation needed for power backoff. */
   gl1_defs_modulation_type modulation;
   gl1_hw_channel_type      channel_type;
   /* co-ex params for this slot */
   uint32                   coex_desense_id;
   uint32                   coex_priority;
   int16                    coex_backoff_desense;
   int16                    coex_backoff_vbatt;
   uint8                    burst_num;
   geran_pdu_priority_t     priority;
} gl1_hw_dtm_tx_slot_info_type;


/* Storage for DTM TX parameters */
typedef struct
{
   boolean                        configured;
   ARFCN_T                        arfcn;
   uint8                          bsic;
   mdsp_dtm_tx_params_type        mdsp_params;
   gl1_hw_dtm_tx_slot_info_type   slot_info[GL1_DEFS_SLOTS_IN_FRAME];
} gl1_hw_dtm_tx_params_type;


/* Storage for DTM parameters */
typedef struct
{
   gl1_hw_dtm_rx_params_type rx;
   gl1_hw_dtm_tx_params_type tx;
} gl1_hw_dtm_params_type;

typedef struct	
{
   rf_gain_T                     *gain_tags[GL1_DEFS_SLOTS_IN_FRAME];
#ifdef FEATURE_GSM_RX_DIVERSITY
   rf_gain_T                     *gain_tags_divrx[GL1_DEFS_SLOTS_IN_FRAME];
#endif
#ifdef GERAN_L1_HLLL_LNASTATE
   uint16                        *sawless_lnastate[GL1_DEFS_SLOTS_IN_FRAME];
#endif
   uint8                         num_of_slots;
}gl1_hw_dtm_gain_tags_type;


static gl1_hw_dtm_params_type gl1_hw_dtm_params[NUM_GERAN_DATA_SPACES];

static gl1_hw_dtm_gain_tags_type gl1_hw_dtm_gain_tags[NUM_GERAN_DATA_SPACES];

extern uint8 gl1_hw_num_assigned_tx_slots[NUM_GERAN_DATA_SPACES];

extern gl1_hw_arbitration_t gl1_hw_arbitration[NUM_GERAN_DATA_SPACES];

/*===========================================================================

                             LOCAL MACROS

===========================================================================*/
#define DTM_PARAM_CHECK(param_gsm,param_gprs,param_name) \
   if (param_gsm != param_gprs)  \
   {                     \
      MSG_GERAN_ERROR_3_G("Param \"" param_name "\" mismatch: GSM (%d) != GPRS (%d), FN=%d", \
                param_gsm,param_gprs,GSTMR_GET_FN_GERAN(gas_id));   \
   }

#define DTM_ARFCN_PARAM_CHECK(param_gsm,param_gprs,param_name) \
   if (!ARFCNS_EQUAL(param_gsm, param_gprs))      \
   {                                              \
      MSG_ERROR("Param \"" param_name "\" mismatch: GSM (%d) != GPRS (%d), FN=%d", \
                param_gsm.num,param_gprs.num,GSTMR_GET_FN_GERAN(gas_id));   \
   }

/* Invalidate the rx tags so that if the burst is aborted the message layer
   does not mess up other receives */
#define MARK_RX_TAGS_INVALID(t) { (t)->data_tag = (t)->metrics_tag = MDSP_INVALID_TAG; }

/* The maximum difference in expected rx power between GSM and GPRS receives
   before printing a warning */
#define MAX_CS_PS_RX_POWER_DIFF  (20*16)  /* 20 dBm */

#ifdef GERAN_L1_IP2CAL
#define GL1_HW_IP2SCHEDULE_GUARD_PERIOD 56
#endif

/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/
#ifdef FEATURE_GSM_RX_DIVERSITY
static gl1_rxd_channel_type gl1_hw_check_channel_type(gl1_hw_channel_type chan_type);
#endif


/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION gl1_hw_get_rxd_enable_status

DESCRIPTION
  Used to return value of rxd mode to TRUE or FALSE

===========================================================================*/
boolean gl1_hw_get_rxd_enable_status(gas_id_t gas_id)
{
  return(enable_rx_diversity[gas_id]);
}


/*===========================================================================

FUNCTION gl1_hw_dm_complete

DESCRIPTION
  This function reads the exported value of dm_complete which tells
  the Host that Data Mover (DM) is complete.

DEPENDENCIES
  None

RETURN VALUE
  boolean         DM finished or not.

SIDE EFFECTS
  None
===========================================================================*/
#if defined (FEATURE_GSM_MDSP_IR)&& !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
boolean gl1_hw_dm_complete (gas_id_t gas_id)
{
  return (dm_complete[gas_id]);
}
#endif
/*===========================================================================
FUNCTION gl1_hw_set_ho_pwr

DESCRIPTION
   This signals the Handover AGC power to use.

===========================================================================*/
void gl1_hw_set_ho_pwr( int16 agc_pwr ,gas_id_t gas_id)
{
  /* Here we need to override the last good AGC value with */
  /* the Handover value as timing of the first sync_rx can */
  /* be overwritten with our -70 dBm default causing fails */
  last_good_agc[gas_id] = gl1_hw_dtm_params[gas_id].rx.expected_power = agc_pwr;
#ifdef FEATURE_GSM_RX_DIVERSITY
  if(gl1_rxd_control_flag[gas_id].enableRxd)
    {
    last_good_agc_divrx[gas_id] = gl1_hw_dtm_params[gas_id].rx.expected_power_divrx = agc_pwr;
    }
#endif
}

/*===========================================================================

FUNCTION gl1_hw_get_idle_mode

DESCRIPTION
  Used to return value of IDLE mode to TRUE or FALSE

===========================================================================*/
boolean gl1_hw_get_idle_mode(gas_id_t gas_id)
{
  return(gl1_hw_dtm_params[gas_id].rx.idle);
}
/*===========================================================================

FUNCTION gl1_hw_set_idle_mode

DESCRIPTION
  Used to set IDLE mode to TRUE or FALSE

===========================================================================*/
void gl1_hw_set_idle_mode(boolean state,gas_id_t gas_id)
{
   gl1_hw_dtm_params[gas_id].rx.idle = state;
 }
/*===========================================================================

FUNCTION gl1_hw_set_dedicated

DESCRIPTION
   This signals if the MS is in Dedicated.

===========================================================================*/
void gl1_hw_set_dedicated( boolean dedicated,gas_id_t gas_id )
{
  gl1_hw_dtm_params[gas_id].rx.dedicated = dedicated;
}

/*===========================================================================

FUNCTION gl1_hw_get_dedicated

DESCRIPTION
   This checks if the MS is in Dedicated.

===========================================================================*/
boolean gl1_hw_get_dedicated( gas_id_t gas_id )
{
  return(gl1_hw_dtm_params[gas_id].rx.dedicated);
}

/*===========================================================================

FUNCTION gl1_hw_dtm_init

DESCRIPTION
   Initialized DTM related parameters.

===========================================================================*/
void gl1_hw_dtm_init (gas_id_t gas_id )
{
   /* Clear out all DTM data */
   memset(&gl1_hw_dtm_params[gas_id], 0, sizeof(gl1_hw_dtm_params[gas_id]));
   memset(&gl1_hw_dtm_gain_tags[gas_id], 0, sizeof(gl1_hw_dtm_gain_tags[gas_id]));   

   /* Set these correctly for init config */
   gl1_hw_set_idle_mode( TRUE, gas_id );
   gl1_hw_set_dedicated( FALSE, gas_id );
}

/*===========================================================================

FUNCTION gl1_hw_dtm_terminate

DESCRIPTION
   Terminate DTM processing

===========================================================================*/
void gl1_hw_dtm_terminate (void)
{
   /* Nothing to do */
}



/*===========================================================================

FUNCTION gl1_hw_rx_dtm_gprs

DESCRIPTION
  Schedule a GPRS receive.

===========================================================================*/
void gl1_hw_rx_dtm_gprs
(
   const gl1_hw_rx_signal_type       signal_info[GL1_DEFS_MAX_ASSIGNED_DL_TS],
   uint8                             num_tn,
   const gl1_hw_rx_pkt_burst_type    *burst_info,
   gl1_hw_rx_tags_type               tags[GL1_DEFS_MAX_ASSIGNED_DL_TS],
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   boolean                           block_start,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   gas_id_t                          gas_id
)
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
  // In case of GPLT and no RF, return default/canned result
  MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_rx_dtm_gprs: Return immediately");
  return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   mdsp_dtm_rx_slot_params_type *slot_params;
   uint8 i, tn;

   /* If we are already configured, check that we agree - note that any
      parameters configured by GSM have priority */
   if (gl1_hw_dtm_params[gas_id].rx.configured)
   {
      DTM_PARAM_CHECK(gl1_hw_dtm_params[gas_id].rx.tsc,burst_info->tsc,"Rx TSC");
      DTM_ARFCN_PARAM_CHECK(gl1_hw_dtm_params[gas_id].rx.arfcn,burst_info->arfcn,"Rx ARFCN");

      /* Warn if expected power is way off */
      if (abs(gl1_hw_dtm_params[gas_id].rx.expected_power - signal_info[0].rx_signal_strength)
           > MAX_CS_PS_RX_POWER_DIFF)
      {
         MSG_GERAN_HIGH_3_G("Large rx power diff (PS=%d, CS=%d), FN=%d",
                  signal_info[0].rx_signal_strength / 16,
                  gl1_hw_dtm_params[gas_id].rx.expected_power / 16, GSTMR_GET_FN_GERAN(gas_id));
      }

      /* Average GSM and GPRS expected power */
      if (signal_info[0].rx_signal_strength != 0) 
      {
        gl1_hw_dtm_params[gas_id].rx.expected_power += signal_info[0].rx_signal_strength; 
        gl1_hw_dtm_params[gas_id].rx.expected_power /= GL1_EXPECTED_RX_PWR_AVG_LEN;
      }
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
      if(gl1_get_MSRD_ctl_nv(gas_id))
      {
        if(signal_info[0].rx_signal_strength_divrx !=0)
        {
          /* Store last good value in case we need it later */
          last_good_data_agc_divrx[gas_id] = signal_info[0].rx_signal_strength_divrx;

          /* Average GSM and GPRS expected power */
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx += signal_info[0].rx_signal_strength_divrx;
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx /= GL1_EXPECTED_RX_PWR_AVG_LEN;
        }
        else
        {   
          MSG_GERAN_HIGH_2_G(GL1_RXD"AGC DivRx Bad Val=0,Used=%d,FN=%d",
                last_good_data_agc_divrx[gas_id]/16,GSTMR_GET_FN_GERAN(gas_id));
          /* Average GSM and GPRS expected power */
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx += last_good_data_agc_divrx[gas_id];
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx /= GL1_EXPECTED_RX_PWR_AVG_LEN;
        }
      }
#endif
   }
   else
   {
      gl1_hw_dtm_params[gas_id].rx.tsc = burst_info->tsc;
      gl1_hw_dtm_params[gas_id].rx.arfcn = burst_info->arfcn;
      if (signal_info[0].rx_signal_strength != 0) 
      {
        gl1_hw_dtm_params[gas_id].rx.expected_power = signal_info[0].rx_signal_strength;
      }

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
      if(gl1_get_MSRD_ctl_nv(gas_id))
      {
        if(signal_info[0].rx_signal_strength_divrx !=0)
        {
          /* Store last good value in case we need it later */
          last_good_data_agc_divrx[gas_id] = signal_info[0].rx_signal_strength_divrx;
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx = signal_info[0].rx_signal_strength_divrx;
        }
        else
        {   
          MSG_GERAN_HIGH_2_G(GL1_RXD"AGC DivRx Bad Val=0,Used=%d,FN=%d",
                             last_good_data_agc_divrx[gas_id]/16,GSTMR_GET_FN_GERAN(gas_id));
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx = last_good_data_agc_divrx[gas_id];
        }
      }
#endif
      gl1_hw_dtm_params[gas_id].rx.search_width = burst_info->search_width;
      gl1_hw_dtm_params[gas_id].rx.configured = TRUE;
   }

   /* Setup Gain range in case we are in FTM mode */
   gl1_hw_dtm_params[gas_id].rx.expected_gain_range = signal_info[0].lna_range;
#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_hw_dtm_params[gas_id].rx.expected_gain_range_divrx = signal_info[0].lna_range_divrx;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   gl1_hw_dtm_params[gas_id].rx.is_block_start = block_start;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
#endif /*FEATURE_GSM_RX_DIVERSITY*/

   /* Catch overflow */
   if (num_tn> GL1_DEFS_MAX_ASSIGNED_DL_TS)
   {
      MSG_GERAN_ERROR_2_G("Max number of DL time slots per frame exceeded %d > %d",
                num_tn, GL1_DEFS_MAX_ASSIGNED_DL_TS);
      num_tn = GL1_DEFS_MAX_ASSIGNED_DL_TS;
   }

   /* Fill in the slot information */
   for (i = 0; i < num_tn; i++)
   {
      tn = (uint8)signal_info[i].tn;

      /* Sanity */
      if (tn >= GL1_DEFS_SLOTS_IN_FRAME)
      {
         MSG_GERAN_ERROR_3_G("Invalid TN (%d) for chan=%d, FN=%d",
                   tn, burst_info->channel_type, GSTMR_GET_FN_GERAN(gas_id));
         MARK_RX_TAGS_INVALID(&tags[i]);
         continue;
      }

      /* Make sure we are not trying to overwrite data */
      if (gl1_hw_dtm_params[gas_id].rx.slot_info[tn].configured)
      {
         MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "Rx TN %d used by chan=%d (aborting chan=%d), FN=%d",
               tn, gl1_hw_dtm_params[gas_id].rx.slot_info[tn].mdsp_params.channel_type,
               burst_info->channel_type, GSTMR_GET_FN_GERAN(gas_id));
         MARK_RX_TAGS_INVALID(&tags[i]);
         continue;
      }

      /* Fill in params */
      slot_params = &gl1_hw_dtm_params[gas_id].rx.slot_info[tn].mdsp_params;
      slot_params->timeslot = (mdsp_timeslot_type)tn;
      slot_params->channel_type = mdsp_channel_type_lut[burst_info->channel_type];
#ifdef FEATURE_GSM_RX_DIVERSITY
      slot_params->rxd_channel_type = gl1_hw_check_channel_type(burst_info->channel_type);
#endif
      slot_params->decode = burst_info->decode_flag;
      slot_params->early_decoding = FALSE;
      slot_params->burst_num = burst_info->burst_num;
      slot_params->force_facch = FALSE;
      slot_params->look_for_facch = FALSE;
#ifdef FEATURE_GSM_AMR
      slot_params->amr_rx_phase = 0;
#endif

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
      slot_params->amr_partial_decode = FALSE;
      slot_params->dtx_detected = FALSE;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

      gl1_hw_dtm_params[gas_id].rx.slot_info[tn].configured = TRUE;
      gl1_hw_dtm_params[gas_id].rx.slot_info[tn].channel_type = burst_info->channel_type;

      /* co-ex params for this slot */
      gl1_hw_dtm_params[gas_id].rx.slot_info[tn].coex_priority = 0;
      gl1_hw_dtm_params[gas_id].rx.slot_info[tn].coex_desense_id = 0;

      /* Save the address to store the tags */
      gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags = &tags[i];
   }

}

/*===========================================================================

FUNCTION gl1_hw_rx_dtm_gsm

DESCRIPTION
  Schedule a GSM receive.

===========================================================================*/
void gl1_hw_rx_dtm_gsm
(
   const gl1_hw_rx_signal_type     *signal_info,
   const gl1_hw_rx_burst_type      *burst_info,
   gl1_hw_rx_tags_type             *tags,
#ifdef FEATURE_GSM_RX_DIVERSITY	
   boolean                          block_start,
#endif
   gas_id_t                         gas_id
)
{
   mdsp_dtm_rx_slot_params_type *slot_params;
   uint8 tn;
#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   boolean iq_sample_force_disable;
#endif

   /* If we are already configured, check that we agree */
   if (gl1_hw_dtm_params[gas_id].rx.configured)
   {
      DTM_PARAM_CHECK(burst_info->tsc,gl1_hw_dtm_params[gas_id].rx.tsc,"Rx TSC");
      DTM_ARFCN_PARAM_CHECK(burst_info->arfcn,gl1_hw_dtm_params[gas_id].rx.arfcn,"Rx ARFCN");

      /* Warn if expected power is way off */
      if (abs(gl1_hw_dtm_params[gas_id].rx.expected_power - signal_info->rx_signal_strength)
           > MAX_CS_PS_RX_POWER_DIFF)
      {
         MSG_GERAN_HIGH_3_G("Large rx power diff (CS=%d, PS=%d), FN=%d",
                  signal_info[0].rx_signal_strength / 16,
                  gl1_hw_dtm_params[gas_id].rx.expected_power / 16, GSTMR_GET_FN_GERAN(gas_id));
      }

      /* Only set non zero AGC to the RF Driver. */
      if (signal_info->rx_signal_strength !=0)
      {
        /* Store last good value in case we need it later */
        last_good_agc[gas_id] = signal_info->rx_signal_strength;

        /* Average GSM and GPRS expected power */
        gl1_hw_dtm_params[gas_id].rx.expected_power += signal_info->rx_signal_strength;
        gl1_hw_dtm_params[gas_id].rx.expected_power /= GL1_EXPECTED_RX_PWR_AVG_LEN;
      }
      else
      {
        MSG_GERAN_MED_2_G("AGC Bad Val=0,Used=%d,FN=%d",
                 last_good_agc[gas_id]/16,GSTMR_GET_FN_GERAN(gas_id));

        /* Average GSM and GPRS expected power */
        gl1_hw_dtm_params[gas_id].rx.expected_power += last_good_agc[gas_id];
        gl1_hw_dtm_params[gas_id].rx.expected_power /= GL1_EXPECTED_RX_PWR_AVG_LEN;
      }
    
#ifdef FEATURE_GSM_RX_DIVERSITY 
    if(gl1_rxd_control_flag[gas_id].enableRxd)
    {
      if(signal_info->rx_signal_strength_divrx !=0)
      {
        /* Store last good value in case we need it later */
        last_good_agc_divrx[gas_id] = signal_info->rx_signal_strength_divrx;

        /* Average GSM and GPRS expected power */
        gl1_hw_dtm_params[gas_id].rx.expected_power_divrx += signal_info->rx_signal_strength_divrx;
        gl1_hw_dtm_params[gas_id].rx.expected_power_divrx /= GL1_EXPECTED_RX_PWR_AVG_LEN;
      }
      else
      {   
        MSG_GERAN_HIGH_2_G(GL1_RXD"AGC DivRx Bad Val=0,Used=%d,FN=%d",
                           last_good_agc_divrx[gas_id]/16,GSTMR_GET_FN_GERAN(gas_id));
        /* Average GSM and GPRS expected power */
        gl1_hw_dtm_params[gas_id].rx.expected_power_divrx += last_good_agc_divrx[gas_id];
        gl1_hw_dtm_params[gas_id].rx.expected_power_divrx /= GL1_EXPECTED_RX_PWR_AVG_LEN;
      }
    }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }
   else if (gl1_hw_get_dedicated(gas_id))
   {
     /* Make sure it's valid, and if not the RF */
     /* driver will use the value last written. */
     if (signal_info->valid && signal_info->rx_signal_strength != 0)
     {
       last_good_agc[gas_id] =
         gl1_hw_dtm_params[gas_id].rx.expected_power =
              signal_info->rx_signal_strength;
     }
     else
     {
       gl1_hw_dtm_params[gas_id].rx.expected_power = last_good_agc[gas_id];
     }
   
#ifdef FEATURE_GSM_RX_DIVERSITY 
     if(gl1_rxd_control_flag[gas_id].enableRxd)
     {
       if (signal_info->valid && signal_info->rx_signal_strength_divrx != 0)
       {
         last_good_agc_divrx[gas_id] = 
         gl1_hw_dtm_params[gas_id].rx.expected_power_divrx = signal_info->rx_signal_strength_divrx;
       }
       else
       {
         gl1_hw_dtm_params[gas_id].rx.expected_power_divrx = last_good_agc_divrx[gas_id];
       }
     }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }
   else
   {
      /* Only set non zero AGC to the RF Driver. */
      if (signal_info->rx_signal_strength != 0)
      {
        last_good_agc[gas_id] = gl1_hw_dtm_params[gas_id].rx.expected_power = signal_info->rx_signal_strength;
      }
      else
      {
        MSG_GERAN_MED_2_G("AGC Bad Val=0,Used=%d,FN=%d",
                  last_good_agc[gas_id]/16,GSTMR_GET_FN_GERAN(gas_id));
        gl1_hw_dtm_params[gas_id].rx.expected_power = last_good_agc[gas_id];
      }
#ifdef FEATURE_GSM_RX_DIVERSITY 
      if(gl1_rxd_control_flag[gas_id].enableRxd)
      {
        if(signal_info->rx_signal_strength_divrx !=0)
        {
          /* Store last good value in case we need it later */
          last_good_agc_divrx[gas_id] = signal_info->rx_signal_strength_divrx;

          /* Average GSM and GPRS expected power */
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx += signal_info->rx_signal_strength_divrx;
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx /= GL1_EXPECTED_RX_PWR_AVG_LEN;
        }
        else
        {
          MSG_GERAN_HIGH_2_G(GL1_RXD"AGC DivRx Bad Val=0,Used=%d,FN=%d",
                             last_good_agc_divrx[gas_id]/16,
                             GSTMR_GET_FN_GERAN(gas_id));
          /* Average GSM and GPRS expected power */
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx += last_good_agc_divrx[gas_id];
          gl1_hw_dtm_params[gas_id].rx.expected_power_divrx /= GL1_EXPECTED_RX_PWR_AVG_LEN;
        }    
      }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }

   /* Setup Gain range in case we are in FTM mode */
   gl1_hw_dtm_params[gas_id].rx.expected_gain_range = signal_info->lna_range;
#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_hw_dtm_params[gas_id].rx.expected_gain_range_divrx = signal_info->lna_range_divrx;
   gl1_hw_dtm_params[gas_id].rx.is_block_start        = block_start;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

   /* GSM parameters have priority over GPRS */
   gl1_hw_dtm_params[gas_id].rx.tsc = burst_info->tsc;
   gl1_hw_dtm_params[gas_id].rx.arfcn = burst_info->arfcn;
   gl1_hw_dtm_params[gas_id].rx.search_width = burst_info->search_width;
   gl1_hw_dtm_params[gas_id].rx.configured = TRUE;
   gl1_hw_dtm_params[gas_id].rx.num_of_page_bursts_missed =  burst_info->num_of_page_bursts_missed;

   /* Extract the timeslot */
   tn = (uint8)signal_info->tn;

   /* Sanity */
   if (tn >= GL1_DEFS_SLOTS_IN_FRAME)
   {
      MSG_GERAN_ERROR_3_G("Invalid TN (%d) for chan=%d, FN=%d",
                tn, burst_info->channel_type, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   /* Check that GPRS has not already taken this slot */
   if (gl1_hw_dtm_params[gas_id].rx.slot_info[tn].configured)
   {
      MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
            "Rx TN %d used by chan=%d (overwriting for chan=%d), FN=%d",
            tn, gl1_hw_dtm_params[gas_id].rx.slot_info[tn].mdsp_params.channel_type,
            burst_info->channel_type, GSTMR_GET_FN_GERAN(gas_id));
      MARK_RX_TAGS_INVALID(gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags);
   }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   /* Temp fix: CR88298 to allow pdchToLog TS for SAIC logging */
   if ( !gl1_hw_is_iq_channel_enabled( burst_info->channel_type, gas_id ) )
   {
     gl1_hw_iq_samples_force_enable( burst_info->channel_type, TRUE, gas_id );
     iq_sample_force_disable = TRUE;
   }
   else
   {
     iq_sample_force_disable = FALSE;
   }

   /* Set the timeslot to log IQ samples for */
   gl1_hw_set_iq_samples_tn( burst_info->channel_type, (gl1_defs_tn_type)tn, gas_id );

   /* Temp fix: CR88298 to switch off for SAIC logging */
   if ( iq_sample_force_disable )
   {
     gl1_hw_iq_samples_force_enable( burst_info->channel_type, FALSE, gas_id );
   }
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

   /* Fill in mdsp slot parameters */
   slot_params = &gl1_hw_dtm_params[gas_id].rx.slot_info[tn].mdsp_params;
   slot_params->channel_type = mdsp_channel_type_lut[burst_info->channel_type];
#ifdef FEATURE_GSM_RX_DIVERSITY	
   slot_params->rxd_channel_type = gl1_hw_check_channel_type(burst_info->channel_type);
#endif
   slot_params->timeslot = (mdsp_timeslot_type)tn;
   slot_params->decode = burst_info->decode_flag;
   slot_params->early_decoding = burst_info->early_decoding;
   slot_params->burst_num = burst_info->burst_num;
   slot_params->force_facch = burst_info->force_facch;
   slot_params->look_for_facch = burst_info->look_for_facch;
#ifdef FEATURE_GSM_AMR
   slot_params->amr_rx_phase = burst_info->amr_phase;
#endif
   slot_params->look_for_rfacch = burst_info->look_for_rfacch;
   slot_params->rfacch_fw_buffer_index = burst_info->rfacch_fw_buffer_index;
   slot_params->look_for_rsacch = burst_info->look_for_rsacch;
   slot_params->coex_params.coex_desense_id = 0;
   slot_params->coex_params.coex_priority = 0;

   if ( gl1_msg_is_sbd_enabled( gas_id ) )
   {
     slot_params->sbd_enabled = burst_info->sbd_enabled;
     slot_params->epd_allowed = burst_info->epd_allowed;
   }
   else
   {
     slot_params->sbd_enabled = FALSE;
     slot_params->epd_allowed = FALSE;
   }

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
   slot_params->amr_partial_decode = burst_info->amr_partial_decode;
   slot_params->dtx_detected = burst_info->dtx_detected;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

   gl1_hw_dtm_params[gas_id].rx.slot_info[tn].configured = TRUE;
   gl1_hw_dtm_params[gas_id].rx.slot_info[tn].channel_type = burst_info->channel_type;

   /* co-ex params for this slot */
   gl1_hw_dtm_params[gas_id].rx.slot_info[tn].coex_priority = 0;
   gl1_hw_dtm_params[gas_id].rx.slot_info[tn].coex_desense_id = 0;

   /* Save the address to store the tags */
   gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags = tags;

#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )
   gl1_hw_set_rx_cmd_issued_in_qta(TRUE, gas_id);
#endif
}



/*===========================================================================

FUNCTION gl1_hw_dtm_setup_rx_rf

DESCRIPTION
   Setup RF for a receive burst.

===========================================================================*/
static uint32 gl1_hw_dtm_setup_rx_rf
(rf_gain_T *gain,
#ifdef FEATURE_GSM_RX_DIVERSITY
 rf_gain_T *gain_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
#ifdef GERAN_L1_HLLL_LNASTATE
 uint16 *lnastateptr,
#endif /*GERAN_L1_HLLL_LNASTATE*/
 gas_id_t gas_id
)
{
   rfgsm_rx_chan_params_type rx_chan_params[1];
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[1];
#ifdef GERAN_L1_ENHANCED_RX
   uint32 index = INVALID_INDEX;
   gl1_hw_lif_table_t gl1_hw_lif_table;
#endif
   uint16 num_bursts = 1;
   uint32 rf_seq_num;
   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)*1));
     GL1_HANDLER_PERFORMANCE_LOGGING(START_RX_RF,gas_id);

     /* package up Rx chan params - only 1 set of params required per frame for Rx Normal Burst */
     rx_chan_params[0].burst_type = RF_RX_BURST;
     rx_chan_params[0].band       = gl1_hw_arfcn_to_rfgsm_band(gl1_hw_dtm_params[gas_id].rx.arfcn);
     rx_chan_params[0].arfcn      = gl1_hw_dtm_params[gas_id].rx.arfcn.num;
     rx_chan_params[0].rx_lvl_dbm = gl1_hw_dtm_params[gas_id].rx.expected_power;
     rx_chan_params[0].lna_range  = gl1_hw_dtm_params[gas_id].rx.expected_gain_range;
     /* gain will need to become a ptr to FW interface memory */
     rx_chan_params[0].gain_ptr   = gain;
     rx_chan_params[0].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
     div_rx_chan_params[0].probe_burst = NULL;	
#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[0].rx_lvl_dbm = gl1_hw_dtm_params[gas_id].rx.expected_power_divrx;
     div_rx_chan_params[0].gain_ptr = gain_divrx;
     div_rx_chan_params[0].lna_range = gl1_hw_dtm_params[gas_id].rx.expected_gain_range_divrx;
  
     if(enable_rx_diversity[gas_id])
     {
       MSG_GERAN_HIGH_3_G(GL1_RXD"expected_power_prx:%d,expected_power_divrx:%d,enable_rx_diversity:%d",
                           gl1_hw_dtm_params[gas_id].rx.expected_power,
                           gl1_hw_dtm_params[gas_id].rx.expected_power_divrx,
                           enable_rx_diversity[gas_id]);
     }
#else
     div_rx_chan_params[0].rx_lvl_dbm = 0;
     div_rx_chan_params[0].gain_ptr = NULL;
     div_rx_chan_params[0].lna_range = NULL;
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[0].high_lin_mode_active = lnastateptr;
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
     /*LIF needs to be enabled by default in FTM mode for single slot and multi slot Cal. And in non-signalling mode GL1 algorithm
           *to turn LIF ON/OFF should work similar to the online mode. 
           */
     if((ftm_get_mode() == FTM_MODE)&& (gl1_hw_get_enable_lif_in_ftm_cal(gas_id) == TRUE))
     {
       rx_chan_params[0].enh_rx_params.enh_rx_enable_flag  = TRUE;
       rx_chan_params[0].enh_rx_params.enh_rx_offset_freq  = GL1_LIF_FREQ_OFFSET;
     }
     else
     {
     gl1_hw_lif_table.is_lif_enabled = FALSE;
     gl1_hw_lif_table.lif_freq_offset_sign = FALSE;

     index = gl1_hw_get_arfcn_index(gl1_hw_dtm_params[gas_id].rx.arfcn,gas_id);
     gl1_hw_get_lif_status(&gl1_hw_lif_table,index,gas_id);

     rx_chan_params[0].enh_rx_params.enh_rx_enable_flag  = gl1_hw_lif_table.is_lif_enabled;
     rx_chan_params[0].enh_rx_params.enh_rx_offset_freq  = (gl1_hw_lif_table.lif_freq_offset_sign == TRUE)?GL1_LIF_FREQ_OFFSET:-(GL1_LIF_FREQ_OFFSET);
     }

     if(rx_chan_params[0].enh_rx_params.enh_rx_enable_flag == TRUE)
         {
       MSG_GERAN_LOW_3_G("enh_rx_on_flag:%d,enh_offset_freq:%d,FN:%d",rx_chan_params[0].enh_rx_params.enh_rx_enable_flag,
                          rx_chan_params[0].enh_rx_params.enh_rx_offset_freq,GSTMR_GET_FN_GERAN(gas_id));
       }
#endif /*GERAN_L1_ENHANCED_RX*/

     if((gl1_hw_sleep_get_entered_recovery(gas_id)==FALSE)
         && (gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE)
         && (gl1_hw_is_forced_wakeup(gas_id)==FALSE)
#ifdef FEATURE_QTA
         && (gl1_hw_qta_gap_active(gas_id)==FALSE)
#endif
       )
     {
       gl1_hw_sleep_set_commit_mask (RX_BURST_COMMIT, gas_id);
     }

    rx_chan_params[0].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
    rf_seq_num = rx_chan_params[0].rf_sequence_number;

     /* create array of burst types - only 1 required for Rx Normal Burst*/

     gl1_hw_rf_rx_burst(num_bursts,
                        rx_chan_params
                        ,div_rx_chan_params
#ifdef FEATURE_GSM_RX_DIVERSITY
                        ,enable_rx_diversity[gas_id]
#else
                        ,FALSE
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                        ,RX_MODE
                        ,gas_id
                        );

     return(rf_seq_num);
}


/*===========================================================================

FUNCTION gl1_hw_dtm_process_rx

DESCRIPTION
   Issue any scheduled receive commands to the RF and mDSP.

===========================================================================*/
static void gl1_hw_dtm_process_rx (gas_id_t gas_id)
{
  mdsp_dtm_rx_tags_type    mdsp_tags;
  mdsp_dtm_rx_params_type  params;
  rf_gain_T                *rf_gain;
  uint32                   current_fn = GSTMR_GET_FN_GERAN(gas_id);
  uint8 tn,i;
  int32 phase_incrmt = 0;

#ifdef GERAN_L1_HLLL_LNASTATE
  uint16 *lnastate_ptr;
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  boolean imrd_channel = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

#ifdef FEATURE_GSM_RX_DIVERSITY
  rf_gain_T  *rf_gain_divrx;

  enable_rx_diversity[gas_id] = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef GERAN_L1_HLLL_LNASTATE
  if(gl1_is_current_state_dtm(gas_id))
  {
    gl1_hw_send_dtm_burst_metrics(GSTMR_GET_FN_GERAN(gas_id),gas_id);
  }
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
  gl1_hw_populate_lif_metrics(GSTMR_GET_FN_GERAN(gas_id),gas_id);
#endif /*GERAN_L1_ENHANCED_RX*/

  /* Check if we have anything to do */
  if (!gl1_hw_dtm_params[gas_id].rx.configured)
  {
    return;
  }

  /* Prepare rx command */
  params.tsc = gl1_hw_dtm_params[gas_id].rx.tsc;
  params.num_slots = 0;
  params.gl1_hw_gprs_anite_gcf = mcfg_gcf_nv_get_status();
  params.num_of_page_bursts_missed = gl1_hw_dtm_params[gas_id].rx.num_of_page_bursts_missed;
  /* Set the SAIC state for the mDSP. */
  params.saic = gl1_hw_dtm_params[gas_id].rx.saic;
  /* Set IDLE mode state for the mDSP */
  params.idle = gl1_hw_dtm_params[gas_id].rx.idle;
  /* Set dedicated mode state for the mDSP */
  params.dedicated = gl1_hw_dtm_params[gas_id].rx.dedicated;
  /* Rx level in dB (required by GFW for truncation at high Rx power) */
  params.rx_lev_dBm = gl1_hw_dtm_params[gas_id].rx.expected_power;

#ifdef FEATURE_GSM_RX_DIVERSITY
  params.rx_lev_drx_dBm = gl1_hw_dtm_params[gas_id].rx.expected_power_divrx;
#endif

#ifdef FEATURE_GSM_COEX
  /* co-ex params for this frame */
  gl1_hw_arbitration[gas_id].rx.arfcn = gl1_hw_dtm_params[gas_id].rx.arfcn;
  gl1_hw_arbitration[gas_id].rx.search_width = gl1_hw_dtm_params[gas_id].rx.search_width;
#endif

  /* Go through the list of slots and add any that are configured */
  for (tn = 0; tn < GL1_DEFS_SLOTS_IN_FRAME; tn++)
  {
#ifdef FEATURE_GSM_COEX
    gl1_hw_arbitration[gas_id].rx.slots[tn].configured = gl1_hw_dtm_params[gas_id].rx.slot_info[tn].configured;
#endif

    if (!gl1_hw_dtm_params[gas_id].rx.slot_info[tn].configured)
    {
      continue;
    }

    /* Sanity check */
    if (params.num_slots >= GL1_DEFS_MAX_ASSIGNED_DL_TS)
    {
      MSG_GERAN_ERROR_3_G( "Too many configured rx slots (%d) ts %d FN %d",
                           params.num_slots, tn, current_fn );
      MARK_RX_TAGS_INVALID(gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags);
      params.num_slots = GL1_DEFS_MAX_ASSIGNED_DL_TS;
    }
    else
    {
      /* Copy the slot info to the parameter buffer */
      params.slot_params[params.num_slots] =
        gl1_hw_dtm_params[gas_id].rx.slot_info[tn].mdsp_params;

#if defined (FEATURE_GSM_RX_DIVERSITY_IDLE)
      params.slot_params[params.num_slots].rxd_channel_type = 
        gl1_hw_check_channel_type(gl1_hw_dtm_params[gas_id].rx.slot_info[tn].channel_type);
      if (GL1_RXD_IDLE_CHANNEL == params.slot_params[params.num_slots].rxd_channel_type) 
      {
        imrd_channel = TRUE;
      }
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

#ifdef FEATURE_GSM_COEX
      /* co-ex params for this slot */
      gl1_hw_arbitration[gas_id].rx.slots[tn].channel_type = gl1_hw_dtm_params[gas_id].rx.slot_info[tn].channel_type;
      gl1_hw_arbitration[gas_id].rx.slots[tn].priority = &params.slot_params[params.num_slots].coex_params.coex_priority;
      gl1_hw_arbitration[gas_id].rx.slots[tn].desense_id = &params.slot_params[params.num_slots].coex_params.coex_desense_id;
      gl1_hw_arbitration[gas_id].rx.slots[tn].burst_num  = gl1_hw_dtm_params[gas_id].rx.slot_info[tn].mdsp_params.burst_num;

#ifdef FEATURE_GSM_LOG_COEX
      /* Log rx burst in cxm_log_table */
      l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), gl1_hw_dtm_params[gas_id].rx.arfcn.num,
                              gl1_hw_dtm_params[gas_id].rx.slot_info[tn].channel_type, CXM_LOG_UNKNOWN_VALUE,
                              tn, 0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif
#endif
      params.num_slots++;
    }

    gl1_hw_dtm_params[gas_id].rx.slot_info[tn].configured = FALSE;
  }

#ifdef FEATURE_GSM_COEX
  /* Perform arbitration of activities prior to configuring RF and sending mDSP commands */
  gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_RX, gas_id);
#endif

#ifdef FEATURE_GSM_DYNAMIC_AEQ_8PSK
  /* if AEQ PH2 is to be dynamically controlled, update the current setting before issuing the cmd for the
      first burst in the block, as switching AEQ on/off mid-block causes incompatible soft decisions for the
      channel decoder */
  if(!gl1_hw_feature_disabled_aeq_8psk(gas_id))
  {
    if(params.slot_params[0].burst_num == 0)
    {
      mdsp_enable_aeq_8psk(mdsp_get_aeq_8psk_dynamic_status( gas_id ) , gas_id );
    }
  }
#endif /* FEATURE_GSM_DYNAMIC_AEQ_8PSK */

#ifdef FEATURE_GSM_RX_DIVERSITY
  enable_rx_diversity[gas_id] = FALSE;

  if (gl1_get_rxd_ctl_nv(gas_id)) 
  {
    if(gl1_hw_dtm_process_rxd_cfg(&params,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                               imrd_channel, 
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                               gas_id))
    {
      enable_rx_diversity[gas_id] = 
                          gl1_hw_dtm_process_slot_rxd_cfg(&params, 
                                                          gl1_hw_dtm_params[gas_id].rx.arfcn.band,
                                                          gl1_hw_dtm_params[gas_id].rx.is_block_start,
                                                          gas_id);
    }
  }

    
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  if (params.num_slots != 0)       /* Sanity check */
  {
    uint32 rf_seq_num;

    /* Prepare RF */
    /* calculate the phase incriment need to appy to rotator */
    phase_incrmt = gl1_hw_calc_phase_increment( gl1_hw_dtm_params[gas_id].rx.arfcn,gas_id );

    /* pass pointer to static location for RF_TASK - do not pass a pointer
         to a location on the stack */
    /* pre-populate gain params with something sensible */
    tn = (uint8)params.slot_params[0].timeslot;
    rf_gain = &gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->gain;
    rf_gain->range  = gl1_hw_dtm_params[gas_id].rx.expected_gain_range;
    rf_gain->valx16 = gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->gain.valx16;

#ifdef FEATURE_GSM_RX_DIVERSITY
    rf_gain_divrx = &gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->gain_divrx;
    /* pre-populate gain params with something sensible */
    rf_gain_divrx->range = gl1_hw_dtm_params[gas_id].rx.expected_gain_range_divrx;
    rf_gain_divrx->valx16 = gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->gain_divrx.valx16;    
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef GERAN_L1_HLLL_LNASTATE
    lnastate_ptr = &gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->sawless_lnastate;
    *lnastate_ptr = 0;
#endif /*GERAN_L1_HLLL_LNASTATE*/

     gl1_hw_dtm_gain_tags[gas_id].num_of_slots = params.num_slots;
     for (i = 0; i < params.num_slots; i++)
       {
       tn = (uint8)params.slot_params[i].timeslot;
       gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->gain = *rf_gain;

#ifdef GERAN_L1_HLLL_LNASTATE
       gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->sawless_lnastate =  (uint16)(*lnastate_ptr);
#endif /*GERAN_L1_HLLL_LNASTATE*/

       if(gl1_is_current_state_dtm(gas_id))
        {
         //save the gain tags which will be updated when RFA_RF_GSM_RX_BURST_CNF is received only in DTM mode
         gl1_hw_dtm_gain_tags[gas_id].gain_tags[i] = &gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->gain;

#ifdef FEATURE_GSM_RX_DIVERSITY
         gl1_hw_dtm_gain_tags[gas_id].gain_tags_divrx[i] = &gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->gain_divrx;
#endif 

#ifdef GERAN_L1_HLLL_LNASTATE
         gl1_hw_dtm_gain_tags[gas_id].sawless_lnastate[i] = &gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->sawless_lnastate;
#endif
        }
      }

    rf_seq_num = gl1_hw_dtm_setup_rx_rf(rf_gain
#ifdef FEATURE_GSM_RX_DIVERSITY
                           ,rf_gain_divrx
#endif /*FEATURE_GSM_RX_DIVERSITY*/
#ifdef GERAN_L1_HLLL_LNASTATE
                            ,lnastate_ptr
#endif /*GERAN_L1_HLLL_LNASTATE*/
                            ,gas_id);

    /* SDA RSSI changes */
    params.rf_gain.range  = rf_gain->range;
    params.rf_gain.valx16 = rf_gain->valx16;

#ifdef FEATURE_GSM_RX_DIVERSITY
    /* SDA RSSI changes */
    params.rf_gain_divrx.range  = rf_gain_divrx->range;
    params.rf_gain_divrx.valx16 = rf_gain_divrx->valx16;  
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    params.DesenseArfcn = gl1_hw_is_arfcn_xo_desense(gl1_hw_dtm_params[gas_id].rx.arfcn,gas_id);

#ifdef FEATURE_AEQ_PHASE4
    params.enableAeq4 = gl1_get_enableAeq4_status(gas_id);
#endif /*FEATURE_AEQ_PHASE4*/

    /* Set the search width */
    mdsp_set_sync_rx_search_width(gl1_hw_dtm_params[gas_id].rx.search_width ,gas_id);
    /* Issue the command */
    mdsp_dtm_rx_qdsp6(&params, &mdsp_tags, phase_incrmt, gl1_hw_dtm_params[gas_id].rx.arfcn, rf_seq_num, gas_id );


    /* Save the tags */
    for (i = 0; i < params.num_slots; i++)
    {
      tn = (uint8)params.slot_params[i].timeslot;
      gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->metrics_tag = mdsp_tags.metrics_tag[i];
      gl1_hw_dtm_params[gas_id].rx.slot_info[tn].tags->data_tag = mdsp_tags.data_tag[i];
    }

    /* Inform the scheduler */
    gl1_hw_schedule_dtm_rx(&params,gas_id);

    /* Clear out config data */
    gl1_hw_dtm_params[gas_id].rx.configured = FALSE;
    if (params.DesenseArfcn == DESENSE_CHAN)
    {
      gl1_hw_rf_update_freq_error( gl1_hw_get_xo_acc_freq_err(gas_id),gas_id  );
    }
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_tx_dtm_gprs

DESCRIPTION
  Schedule a GPRS transmit.

===========================================================================*/
void gl1_hw_tx_dtm_gprs
(
   const gl1_hw_tx_signal_type       signal_info[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   uint8                             num_tn,
   const gl1_hw_tx_pkt_burst_type    *burst_info,
   gl1_hw_tx_tags_type               tags[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   uint8                             num_msgs,
   uint8                             num_ul_timeslots,
   gas_id_t                          gas_id
)
{
   uint8 i, tn, msg_cnt;

   /* If we are already configured, check that we agree - note that any
      parameters configured by GSM have priority */
   if (gl1_hw_dtm_params[gas_id].tx.configured)
   {
      DTM_PARAM_CHECK(gl1_hw_dtm_params[gas_id].tx.mdsp_params.tsc,burst_info->tsc,"Tx TSC");
      if (gl1_hw_dtm_params[gas_id].tx.bsic != NO_BSIC && burst_info->bsic != NO_BSIC)
      {
         DTM_PARAM_CHECK(gl1_hw_dtm_params[gas_id].tx.bsic,burst_info->bsic,"BSIC");
      }
      DTM_PARAM_CHECK(gl1_hw_dtm_params[gas_id].tx.mdsp_params.timing_advance,
                      burst_info->timing_advance, "TA");
      DTM_ARFCN_PARAM_CHECK(gl1_hw_dtm_params[gas_id].tx.arfcn,burst_info->arfcn,"Tx ARFCN");
#ifdef FEATURE_GSM_MDSP_EGPRS
      /* This would be pretty bad, have to give up on GPRS entirely */
      if (burst_info->loopback_flag)
      {
         MSG_GERAN_ERROR_0_G("GPRS loopback not allowed in DTM");
         return;
      }
#endif
   }
   else
   {
      gl1_hw_dtm_params[gas_id].tx.configured = TRUE;
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_msgs = 0;
      gl1_hw_dtm_params[gas_id].tx.arfcn = burst_info->arfcn;
      gl1_hw_dtm_params[gas_id].tx.bsic = burst_info->bsic;
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.tsc = burst_info->tsc;
      /* Added for AB TSC 41bit sync sequence */
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.ab_tsc = burst_info->ab_tsc;
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.timing_advance = burst_info->timing_advance;
#ifdef FEATURE_GSM_MDSP_EGPRS
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.loopback = burst_info->loopback_flag;
#endif
#ifdef FEATURE_GERAN_RF_SELF_TEST 
      if( ftm_get_mode() != FTM_MODE )
      {
        /* always set the selftest flag to FALSE when modem is not in FTM mode */
        gl1_hw_dtm_params[gas_id].tx.mdsp_params.selftest_flag_to_issue_TxCmd = FALSE;
      }
      else
      {
         /* In FTM mode, RF would set selftest flag
           when the selftest flag is set, inform FW to start selftest capture through fbrx
          */ 
        gl1_hw_dtm_params[gas_id].tx.mdsp_params.selftest_flag_to_issue_TxCmd =
                   burst_info->selftest_ready_to_issue_txcmd;
      }
#endif /* FEATURE_GERAN_RF_SELF_TEST */
   }

   /* Save the slot parameters */
   for (i = 0; i < num_tn; i++)
   {
      tn = (uint8)signal_info[i].tn;

      /* Sanity */
      if (tn >= GL1_DEFS_SLOTS_IN_FRAME)
      {
         MSG_GERAN_ERROR_2_G("Invalid GPRS tx TN (%d) for chan=%d, FN=%d", tn, GSTMR_GET_FN_GERAN(gas_id));
         continue;
      }

      if (gl1_hw_dtm_params[gas_id].tx.slot_info[tn].configured)
      {
         MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "Tx TN %d used with pwr=%d (aborting pwr=%d), FN=%d",
               tn, gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index,
               signal_info[i].tx_power_index, GSTMR_GET_FN_GERAN(gas_id));
         continue;
      }

      /* Fill in the slot info */
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index = signal_info[i].tx_power_index;
      /* Modulation needed for power backoff. */
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].modulation = signal_info[i].modulation;
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].configured = TRUE;
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].channel_type = signal_info[i].channel_type;
      /* co-ex params for this slot */
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].coex_desense_id = 0;
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].coex_priority = 0;
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].coex_backoff_desense = 0;
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].coex_backoff_vbatt = 0;
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].burst_num = burst_info->burst_num;
      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].priority =  signal_info[i].priority;
   }

   if (num_tn == 0)
   {
     /* no tx so clear the configured flag */
     MSG_GERAN_HIGH_1_G("No tn so clear the configured flag", num_tn);
     gl1_hw_dtm_params[gas_id].tx.configured = FALSE;
   }

   /* Save the tags */
   for (i = 0; i < num_msgs; i++)
   {
      msg_cnt = gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_msgs;
      if (msg_cnt >= GL1_DEFS_MAX_UL_MSGS)
      {
         MSG_GERAN_ERROR_1_G("Too many tx messages %d", msg_cnt);
         break;
      }
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.data_tags[msg_cnt] = tags[i].data_tag;
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_msgs++;
   }

   /* Set the number of uplink timeslots */
   gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_ul_timeslots = num_ul_timeslots;
}



/*===========================================================================

FUNCTION gl1_hw_tx_dtm_gsm

DESCRIPTION
  Schedule a GSM transmit.

===========================================================================*/
void gl1_hw_tx_dtm_gsm
(
   const gl1_hw_tx_signal_type       signal_info[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   const gl1_hw_tx_burst_type        *burst_info,
   gl1_hw_tx_tags_type               *tags,
   gas_id_t                          gas_id
)
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
  // In case of GPLT and no RF, return default/canned result
  MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_tx_dtm_gsm: Return immediately");
  return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   uint8 tn, msg_cnt;

   /* If we are already configured, check that we agree - note that any
      parameters configured by GSM have priority */
   if (gl1_hw_dtm_params[gas_id].tx.configured)
   {
      DTM_PARAM_CHECK(burst_info->tsc,gl1_hw_dtm_params[gas_id].tx.mdsp_params.tsc,"Tx TSC");
      if (burst_info->bsic != NO_BSIC && gl1_hw_dtm_params[gas_id].tx.bsic != NO_BSIC)
      {
         DTM_PARAM_CHECK(burst_info->bsic,gl1_hw_dtm_params[gas_id].tx.bsic,"BSIC");
      }
      DTM_PARAM_CHECK(burst_info->timing_advance,
                      gl1_hw_dtm_params[gas_id].tx.mdsp_params.timing_advance, "TA");
      DTM_ARFCN_PARAM_CHECK(burst_info->arfcn,gl1_hw_dtm_params[gas_id].tx.arfcn,"Tx ARFCN");
#ifdef FEATURE_GSM_MDSP_EGPRS
      /* This would be pretty bad, have to give up on GSM entirely */
      if (gl1_hw_dtm_params[gas_id].tx.mdsp_params.loopback)
      {
         MSG_GERAN_ERROR_0_G("GSM loopback not allowed in EGPRS SRB loopback");
         return;
      }
#endif
   }
   else
   {
      gl1_hw_dtm_params[gas_id].tx.configured = TRUE;
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_msgs = 0;
      gl1_hw_dtm_params[gas_id].tx.bsic = NO_BSIC;

#ifdef FEATURE_GERAN_RF_SELF_TEST 
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.selftest_flag_to_issue_TxCmd = FALSE;
#endif /*FEATURE _GERAN_RF_SELF_TEST*/
   }

   gl1_hw_dtm_params[gas_id].tx.arfcn = burst_info->arfcn;
   if (burst_info->bsic != NO_BSIC)
   {
      gl1_hw_dtm_params[gas_id].tx.bsic = burst_info->bsic;
   }
   gl1_hw_dtm_params[gas_id].tx.mdsp_params.tsc = burst_info->tsc;
   /* Added for AB TSC Sync sequence bits. */
   if ( ftm_get_mode() != FTM_MODE )
   {
     if(burst_info->ab_tsc > 2)
     {
#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       ERR_GERAN_FATAL_1_G("ab_tsc : %d being passed for Tx is wrong", burst_info->ab_tsc);
     }
   }
   gl1_hw_dtm_params[gas_id].tx.mdsp_params.ab_tsc = burst_info->ab_tsc;
   gl1_hw_dtm_params[gas_id].tx.mdsp_params.timing_advance = burst_info->timing_advance;
#ifdef FEATURE_GSM_MDSP_EGPRS
   gl1_hw_dtm_params[gas_id].tx.mdsp_params.loopback = FALSE;
#endif

   /* Save the slot parameters */
   tn = (uint8)signal_info->tn;

   /* Sanity */
   if (tn >= GL1_DEFS_SLOTS_IN_FRAME)
   {
      MSG_GERAN_ERROR_2_G("Invalid GSM tx TN (%d), FN=%d", tn, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }
   if (gl1_hw_dtm_params[gas_id].tx.slot_info[tn].configured)
   {
      MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
            "Tx TN %d used with pwr=%d (overwriting with pwr=%d), FN=%d",
            tn, gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index,
            signal_info->tx_power_index, GSTMR_GET_FN_GERAN(gas_id));
   }

   /* Store the timeslot parameters */
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index = signal_info->tx_power_index;
   /* Modulation needed for power backoff. */
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].modulation = GL1_DEFS_MODULATION_GMSK;
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].configured = TRUE;
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].channel_type = burst_info->channel_type;

   /* if power reduction is active and the current power index is greater than the power reduction PCL
      then use the power reduction PCL */
   if (gl1_get_rach_pwr_reduction(gas_id))
   {
      if (gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index > gl1_get_rach_pwr_reduction_pwr_idx(gas_id))
      {
         MSG_GERAN_MED_3_G("GSM tx TN (%d), overwriting power_index %d with %d", 
                            tn, 
                            gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index,
                            gl1_get_rach_pwr_reduction_pwr_idx(gas_id));
         gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index = gl1_get_rach_pwr_reduction_pwr_idx(gas_id);
      }
   }
   /* co-ex params for this slot */
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].coex_desense_id = 0;
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].coex_priority = 0;
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].coex_backoff_desense = 0;
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].coex_backoff_vbatt = 0;
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].burst_num = burst_info->burst_num;
   gl1_hw_dtm_params[gas_id].tx.slot_info[tn].priority = GPDU_LOW_PRIORITY; 

   /* Save the tag */
   msg_cnt = gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_msgs;
   if (msg_cnt >= GL1_DEFS_MAX_UL_MSGS)
   {
      MSG_GERAN_ERROR_1_G("Too many tx messages %d", msg_cnt);
   }
   else
   {
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.data_tags[msg_cnt] = tags->data_tag;
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_msgs++;
   }
}



/*===========================================================================

FUNCTION gl1_hw_dtm_setup_tx_rf

DESCRIPTION
   Setup RF for a transmit burst. Pass in modulation for power backoff.

===========================================================================*/
static void gl1_hw_dtm_setup_tx_rf
(
   uint16 pa_index[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   gl1_defs_modulation_type mod[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   int16    coex_backoff_desense[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   int16    coex_backoff_vbatt[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   uint8    num_slots,
   uint32   rf_seq_num,
   gas_id_t gas_id
)
{
   GL1_HANDLER_PERFORMANCE_LOGGING(START_TX_RF,gas_id);

   /* Call set_tx_band every time - MSGR request for band change will only occur
      if passed in band value is different from current band setting - the expectation
      is that the band will have been set up in advance, so will never get set via
      the below call */
   gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(gl1_hw_dtm_params[gas_id].tx.arfcn),gas_id);

   if((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE) && (gl1_hw_is_forced_wakeup(gas_id)==FALSE))
   {
     gl1_hw_sleep_set_commit_mask (TX_BURST_COMMIT, gas_id);
   }

   gl1_hw_rf_tx_burst(gl1_hw_num_assigned_tx_slots[gas_id],
                      num_slots,
                      gl1_hw_arfcn_to_rfgsm_band(gl1_hw_dtm_params[gas_id].tx.arfcn),
                      (uint16)gl1_hw_dtm_params[gas_id].tx.arfcn.num,
                      gl1_hw_rf_get_freq_error(gas_id),
                      pa_index,
                      mod,
                      coex_backoff_desense,
                      coex_backoff_vbatt,
                      rf_seq_num,
                      gas_id);

   /* tx_alpha is now conveyed as part of RFA_RF_GSM_GET_TIMING_INFOR_REQ */
#ifdef FEATURE_VSWR
   if ( gl_hw_use_extended_tx_alpha [gas_id] ) 
   {
     gl1_hw_dtm_params[gas_id].tx.mdsp_params.tx_alpha = gl1_hw_get_rf_gsm_tx_vswr_setup_time(gas_id);
   }
   else
   {  
     gl1_hw_dtm_params[gas_id].tx.mdsp_params.tx_alpha = gl1_hw_get_rf_gsm_tx_setup_time(gas_id); 
   }
#else

gl1_hw_dtm_params[gas_id].tx.mdsp_params.tx_alpha = gl1_hw_get_rf_gsm_tx_setup_time(gas_id); 

#endif /*FEATURE_VSWR*/
   
}



/*===========================================================================

FUNCTION gl1_hw_dtm_process_tx

DESCRIPTION
   Issue any scheduled transmit commands to the RF and mDSP.

===========================================================================*/
static void gl1_hw_dtm_process_tx (gas_id_t gas_id)
{
   uint16 pa_index[GL1_DEFS_MAX_ASSIGNED_UL_TS];
   /* Modulation for power backoff. */
   gl1_defs_modulation_type mod[GL1_DEFS_MAX_ASSIGNED_UL_TS];
   int16 coex_backoff_desense[GL1_DEFS_SLOTS_IN_FRAME] = {0};
   int16 coex_backoff_vbatt[GL1_DEFS_SLOTS_IN_FRAME] = {0};
   uint32                   current_fn = GSTMR_GET_FN_GERAN(gas_id);
   uint8 num_slots, tn;
   uint32 rf_seq_num;

#ifdef FEATURE_GSM_COEX
   uint8 slot_index;
#endif

   gl1_hw_mdsp_freq_t freq_correction_data;

   /* Check if we have anything to do */
   if (!gl1_hw_dtm_params[gas_id].tx.configured) return;

#ifdef FEATURE_GSM_COEX
   /* co-ex params for this frame */
   gl1_hw_arbitration[gas_id].tx.arfcn = gl1_hw_dtm_params[gas_id].tx.arfcn;
   gl1_hw_arbitration[gas_id].tx.timing_advance = gl1_hw_dtm_params[gas_id].tx.mdsp_params.timing_advance;
#endif

   /* Go through the list of slots and add any that are configured */
   num_slots = 0;
   for (tn = 0; tn < GL1_DEFS_SLOTS_IN_FRAME; tn++)
   {
#ifdef FEATURE_GSM_COEX
      gl1_hw_arbitration[gas_id].tx.slots[tn].configured = gl1_hw_dtm_params[gas_id].tx.slot_info[tn].configured;
#endif

      if (!gl1_hw_dtm_params[gas_id].tx.slot_info[tn].configured) continue;

      /* Sanity check */
      if (num_slots >= GL1_DEFS_MAX_ASSIGNED_UL_TS)
      {
         MSG_GERAN_ERROR_3_G( "Too many configured tx slots (%d) ts %d FN %d",
                    num_slots, tn, current_fn );
         num_slots = GL1_DEFS_MAX_ASSIGNED_UL_TS;
      }
      else
      {
         pa_index[num_slots] = (uint16)gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index;
         /* Modulation for power backoff. */
         mod[num_slots] = gl1_hw_dtm_params[gas_id].tx.slot_info[tn].modulation;
         gl1_hw_dtm_params[gas_id].tx.mdsp_params.timeslots[num_slots] = (mdsp_timeslot_type)tn;
         gl1_hw_dtm_params[gas_id].tx.mdsp_params.burst_num = gl1_hw_dtm_params[gas_id].tx.slot_info[tn].burst_num;
         gl1_hw_dtm_params[gas_id].tx.mdsp_params.high_prio_ctrl[num_slots] = (gl1_hw_dtm_params[gas_id].tx.slot_info[tn].priority == GPDU_HIGH_PRIORITY) ? TRUE :FALSE;

#ifdef FEATURE_GSM_COEX
         /* co-ex params for this slot */
         gl1_hw_arbitration[gas_id].tx.power = gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index; /* Tx power constant across all slots */
         gl1_hw_arbitration[gas_id].tx.slots[tn].channel_type = gl1_hw_dtm_params[gas_id].tx.slot_info[tn].channel_type;
         gl1_hw_arbitration[gas_id].tx.slots[tn].priority = &gl1_hw_dtm_params[gas_id].tx.mdsp_params.coex_params[num_slots].coex_priority;
         gl1_hw_arbitration[gas_id].tx.slots[tn].desense_id = &gl1_hw_dtm_params[gas_id].tx.mdsp_params.coex_params[num_slots].coex_desense_id;
         gl1_hw_arbitration[gas_id].tx.slots[tn].backoff_desense = &coex_backoff_desense[num_slots];
         gl1_hw_arbitration[gas_id].tx.slots[tn].backoff_vbatt = &coex_backoff_vbatt[num_slots];
         gl1_hw_arbitration[gas_id].tx.slots[tn].burst_num  = gl1_hw_dtm_params[gas_id].tx.slot_info[tn].burst_num;
         gl1_hw_arbitration[gas_id].tx.slots[tn].pdu_prio   = gl1_hw_dtm_params[gas_id].tx.slot_info[tn].priority;
#endif
         num_slots++;
      }

      gl1_hw_dtm_params[gas_id].tx.slot_info[tn].configured = FALSE;
   }

   rf_seq_num = gl1_hw_get_next_rf_seq_num();

   gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_slots = num_slots;

#ifdef FEATURE_GSM_COEX
   /* Perform arbitration of activities prior to configuring RF and sending mDSP commands */
   gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_TX, gas_id);

   /* Log tx bursts in cxm_log_table */
   slot_index = 0;
   for (tn = 0; tn < GL1_DEFS_SLOTS_IN_FRAME; tn++)
   {
     if (gl1_hw_arbitration[gas_id].tx.slots[tn].configured)
     {
#ifdef FEATURE_GSM_LOG_COEX
       l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), gl1_hw_dtm_params[gas_id].tx.arfcn.num,
                            gl1_hw_dtm_params[gas_id].tx.slot_info[tn].channel_type, CXM_LOG_UNKNOWN_VALUE,
                            tn + 3, coex_backoff_desense[slot_index], coex_backoff_vbatt[slot_index],
                            gl1_hw_dtm_params[gas_id].tx.slot_info[tn].power_index, CXM_LOG_ACTIVITY_TX, gas_id);
#endif
       slot_index++;
     }
   }
#endif

   /* Sanity check */
   if (num_slots == 0)
   {
      MSG_GERAN_ERROR_1_G("Zero tx slots, FN=%d", current_fn);
      return;
   }

   /* Prepare RF for transmit; pass in the modulation for power backoff. */
   gl1_hw_dtm_setup_tx_rf(pa_index, mod, coex_backoff_desense, coex_backoff_vbatt, num_slots, rf_seq_num, gas_id);

   /* Set the BSIC if valid */
   if (gl1_hw_dtm_params[gas_id].tx.bsic != NO_BSIC)
   {
      mdsp_set_bsic(gl1_hw_dtm_params[gas_id].tx.bsic ,gas_id);
   }

   /* Update the freq correction parameters */
   if (gl1_hw_get_mdsp_freq_config_data( &freq_correction_data,gas_id ))
   {
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.f_hz_q6 = freq_correction_data.freq_error_hz_q6;
      gl1_hw_dtm_params[gas_id].tx.mdsp_params.inv_f_dl_lo = freq_correction_data.inv_f_dl_lo;
   }

   /* Issue the command to the mdsp driver */
   mdsp_dtm_tx(&gl1_hw_dtm_params[gas_id].tx.mdsp_params, gl1_hw_dtm_params[gas_id].tx.arfcn, pa_index, rf_seq_num, gas_id);

   /* Inform the scheduler */
   gl1_hw_schedule_dtm_tx(&gl1_hw_dtm_params[gas_id].tx.mdsp_params,gas_id);

   /* Clear out config data */
   gl1_hw_dtm_params[gas_id].tx.configured = FALSE;
   gl1_hw_dtm_params[gas_id].tx.mdsp_params.num_msgs = 0;
}



/*===========================================================================

FUNCTION gl1_hw_dtm_frame_tick

DESCRIPTION
   Called every frame tick after the rx and tx handlers have been executed
   to issue any scheduled receives or transmits.

===========================================================================*/
void gl1_hw_dtm_frame_tick (gas_id_t gas_id)
{
   /* Prepare any scheduled rx and tx commands */
   gl1_hw_dtm_process_rx(gas_id);

   if(!gl1_hw_sleep_get_entered_recovery(gas_id))
   {
   gl1_hw_dtm_process_tx(gas_id);
   }

  return;
}

#ifdef FEATURE_GSM_DYNAMIC_SAIC
/*===========================================================================

FUNCTION gl1_hw_get_saic_status

DESCRIPTION
   returns state of SAIC ON,OFF

===========================================================================*/
boolean gl1_hw_get_saic_state(gas_id_t gas_id)
{
  return(gl1_hw_dtm_params[gas_id].rx.saic);
}
#endif

/*===========================================================================

FUNCTION gl1_hw_cfg_saic

DESCRIPTION
   This allows SAIC to be only active for dedicated mode.

===========================================================================*/
void gl1_hw_cfg_saic( boolean state,gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_cfg_saic: Return Immediately");
   return;
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

#ifdef FEATURE_GSM_MDSP_ESAIC
  boolean esaic_used = TRUE;
#endif

  boolean aeq_used = TRUE;

  if(gl1_hw_feature_disabled_aeq(gas_id) )
  {
    aeq_used = FALSE;
  }
  /** Not sure what to do about voice aeq currently */


#ifdef FEATURE_GSM_MDSP_ESAIC
  if(gl1_hw_feature_disabled_esaic(gas_id) )
  {
    esaic_used = FALSE;
  }
#endif

  if ( gl1_hw_feature_disabled_saic(gas_id) )
  {
    gl1_hw_dtm_params[gas_id].rx.saic = FALSE;
  }
  else
  {
    gl1_hw_dtm_params[gas_id].rx.saic = state;
  }

  gl1_hw_async_rx_saic[gas_id] = gl1_hw_dtm_params[gas_id].rx.saic;

  if ( gl1_hw_dtm_params[gas_id].rx.saic )
  {
#if   defined FEATURE_GSM_MDSP_ESAIC
    if( (esaic_used) && (aeq_used) )
    {
#ifdef FEATURE_GSM_AEQ_VOICE
      MSG_GERAN_LOW_0_G( "D/L SAIC,ESAIC and AEQ ON (VOICE + DATA)" );
#else
      MSG_GERAN_LOW_0_G( "D/L SAIC,ESAIC and AEQ ON (DATA ONLY)" );
#endif
    }else if( (esaic_used) && (aeq_used == FALSE) )
    {
      MSG_GERAN_LOW_0_G( "D/L SAIC,ESAIC ON. AEQ OFF" );
    }else
    {
      MSG_GERAN_LOW_0_G( "D/L SAIC ON, ESAIC and AEQ OFF" );
    }
#else
    MSG_GERAN_LOW_0_G( "D/L SAIC ON" );
#endif
  }
  else
  {
    MSG_GERAN_LOW_0_G( "D/L SAIC OFF" );
  }

#if defined  FEATURE_GSM_MDSP_AEQ_SCH
  if(gl1_hw_feature_disabled_aeq_sch(gas_id))
  {
    MSG_GERAN_LOW_0_G( "D/L AEQ on SCH OFF" );
  }
  else
  {
    MSG_GERAN_LOW_0_G( "D/L AEQ on SCH ON" );
  }
#endif /* FEATURE_GSM_MDSP_AEQ_SCH && FEATURE_GSM_MDSP_AEQ*/

#if defined  FEATURE_GSM_MDSP_AEQ_8PSK
  if(gl1_hw_feature_disabled_aeq_8psk(gas_id))
  {
    MSG_GERAN_LOW_0_G( "D/L AEQ PH2 (8PSK) OFF" );
  }
  else
  {
    MSG_GERAN_LOW_0_G( "D/L AEQ PH2 (8PSK) ON" );

#if defined (FEATURE_GSM_DYNAMIC_AEQ_8PSK) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
    /* initialise AEQ 8PSK mean BEP buffers and buffer indices */
    gl1_msg_init_aeq_8psk_mean_bep(gas_id);
#endif
  }
#endif
}

/*===========================================================================

FUNCTION gl1_hw_get_dtm_expected_power

DESCRIPTION
   returns current expected Rx power level

===========================================================================*/
int32 gl1_hw_get_dtm_expected_power(gas_id_t gas_id)
{
  /* return signed dBm value */
  return(gl1_hw_dtm_params[gas_id].rx.expected_power);
}

/*===========================================================================

FUNCTION gl1_hw_get_dtm_band

DESCRIPTION
   returns current band

===========================================================================*/
uint8 gl1_hw_get_dtm_band(gas_id_t gas_id)
{
  /* return current band */
  return((uint8)gl1_hw_dtm_params[gas_id].rx.arfcn.band);
}

/*===========================================================================

FUNCTION gl1_hw_dtm_get_tx_channel_type

DESCRIPTION
  This the callback used by the arbitration interface to get the tx channel
  type of the arbitrated frame.

===========================================================================*/
gl1_hw_channel_type gl1_hw_dtm_get_tx_channel_type(uint8 tn,gas_id_t gas_id)
{
  return gl1_hw_dtm_params[gas_id].tx.slot_info[tn].channel_type;
}

/*===========================================================================

FUNCTION gl1_hw_dtm_get_rx_channel_type

DESCRIPTION
  This the callback used by the arbitration interface to get the rx channel
  type of the arbitrated frame.

===========================================================================*/
gl1_hw_channel_type gl1_hw_dtm_get_rx_channel_type(uint8 tn,gas_id_t gas_id)
{
  return gl1_hw_dtm_params[gas_id].rx.slot_info[tn].channel_type;
}

/*===========================================================================

FUNCTION gl1_hw_get_dtm_arfcn

DESCRIPTION
   returns current ARFCN

===========================================================================*/
uint16 gl1_hw_get_dtm_arfcn(gas_id_t gas_id)
{
  /* return current ARFCN */
  return((uint16)gl1_hw_dtm_params[gas_id].rx.arfcn.num);
}

/*===========================================================================

FUNCTION gl1_hw_update_gain_tags

DESCRIPTION
   Updates the gain tags with the gain values when RFA_RF_GSM_RX_BURST_CNF for the RX bursts scheduled is received

===========================================================================*/
void gl1_hw_update_gain_tags(rf_gain_T *gain,
#ifdef FEATURE_GSM_RX_DIVERSITY
                             rf_gain_T *gain_divrx,
#endif
#ifdef GERAN_L1_HLLL_LNASTATE
                                  uint16 *sawless_lnastate,
#endif
                                  gas_id_t gas_id)
{
  uint8 i = 0;
  for (i = 0; i < gl1_hw_dtm_gain_tags[gas_id].num_of_slots; i++)
	{
	gl1_hw_dtm_gain_tags[gas_id].gain_tags[i]->valx16              = gain->valx16;	
	gl1_hw_dtm_gain_tags[gas_id].gain_tags[i]->range               = gain->range;
	gl1_hw_dtm_gain_tags[gas_id].gain_tags[i]->range_high_limitx16 = gain->range_high_limitx16;
	gl1_hw_dtm_gain_tags[gas_id].gain_tags[i]->range_low_limitx16  = gain->range_low_limitx16;	

#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_hw_dtm_gain_tags[gas_id].gain_tags_divrx[i]->valx16 = gain_divrx->valx16;
    gl1_hw_dtm_gain_tags[gas_id].gain_tags_divrx[i]->range = gain_divrx->range;
    gl1_hw_dtm_gain_tags[gas_id].gain_tags_divrx[i]->range_high_limitx16 = gain_divrx->range_high_limitx16;
    gl1_hw_dtm_gain_tags[gas_id].gain_tags_divrx[i]->range_low_limitx16  = gain_divrx->range_low_limitx16;
#endif
 
#ifdef GERAN_L1_HLLL_LNASTATE
   gl1_hw_dtm_gain_tags[gas_id].sawless_lnastate[i]               = sawless_lnastate;
#endif 
  	}
}


#ifdef FEATURE_GSM_RX_DIVERSITY

/*===========================================================================

FUNCTION gl1_set_rxd_control_flags

DESCRIPTION
   Sets Rx diversity control flags

===========================================================================*/

void gl1_set_rxd_control_flags(gl1_rxd_control_type gl1_rxd_control,gas_id_t gas_id)
{
  gl1_rxd_control_flag[gas_id].enableRxd = gl1_rxd_control.enableRxd;
  gl1_rxd_control_flag[gas_id].enableDrx = gl1_rxd_control.enableDrx;
  gl1_rxd_control_flag[gas_id].enablePrx = gl1_rxd_control.enablePrx;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  gl1_rxd_control_flag[gas_id].enableIMRD = gl1_rxd_control.enableIMRD;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  gl1_rxd_control_flag[gas_id].enableMSRD = gl1_rxd_control.enableMSRD;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
  gl1_rxd_control_flag[gas_id].RxdForcedOn = gl1_rxd_control.RxdForcedOn;
  gl1_rxd_control_flag[gas_id].threshold = gl1_rxd_control.threshold;
}

/*===========================================================================

FUNCTION gl1_get_rxd_control_flags

DESCRIPTION
   Returns Rx diversity control flags

===========================================================================*/
gl1_rxd_control_type gl1_get_rxd_control_flags(gas_id_t gas_id)
{
  return(gl1_rxd_control_flag[gas_id]);
}

/*===========================================================================

FUNCTION gl1_reset_rxd_control_flags

DESCRIPTION
   Resets Rx diversity control flags

===========================================================================*/
void gl1_reset_rxd_control_flags(gas_id_t gas_id)
{
  gl1_rxd_control_flag[gas_id].enableRxd = FALSE;
  gl1_rxd_control_flag[gas_id].enableDrx = FALSE;
  gl1_rxd_control_flag[gas_id].enablePrx = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE 
  gl1_rxd_control_flag[gas_id].enableIMRD = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE */
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  gl1_rxd_control_flag[gas_id].enableMSRD = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE */
  gl1_rxd_control_flag[gas_id].RxdForcedOn= FALSE;
  gl1_rxd_control_flag[gas_id].threshold = 0;

}

/*===========================================================================

FUNCTION gl1_hw_check_channel_type

DESCRIPTION
 This function returns gl1_rxd_channel_type to confirm if the channel type passed is CS or PS or IDLE CHANNEL.
 We need this classification for RxDIVERSITY purposes. 
===========================================================================*/
static gl1_rxd_channel_type gl1_hw_check_channel_type(gl1_hw_channel_type chan_type)
{
  gl1_rxd_channel_type gl1_rxd_channel = GL1_RXD_CH_UNDEFINED;

  switch(chan_type)
  {
    case  GL1_HW_TCH_FS:
    case  GL1_HW_TCH_EFS:
    case  GL1_HW_TCH_F96:
    case  GL1_HW_TCH_F144:
    case  GL1_HW_SACCH:
    case  GL1_HW_SDCCH:
    case  GL1_HW_FACCH:
    case  GL1_HW_TCH_F96_AND_FACCH:
    case  GL1_HW_TCH_F144_AND_FACCH:
    case  GL1_HW_TCH_HS:
    case  GL1_HW_FACCH_HS:
    case  GL1_HW_TCH_AFS:
    case  GL1_HW_TCH_AHS:
    case  GL1_HW_FACCH_AFS:
    case  GL1_HW_FACCH_AHS:
    case  GL1_HW_TCH_WFS:
    case  GL1_HW_FACCH_WFS:
      {
        gl1_rxd_channel = GL1_RXD_CS_CHANNEL;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
        MSG_GERAN_MED_1(GL1_RXD"CS CH Type chan_type=%d",chan_type);
#endif
        break;
      }

    case  GL1_HW_PTCCHD:
    case  GL1_HW_PDTCHD:
      {
        gl1_rxd_channel = GL1_RXD_PS_CHANNEL;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG
        MSG_GERAN_MED_1(GL1_RXD"PS CH Type chan_type=%d",chan_type);
#endif
        break;
      }
	
    case  GL1_HW_BCCH:      //  6
    case  GL1_HW_PCH:        //  7
//    case  GL1_HW_PBCCH:      //  33
//    case  GL1_HW_PCCCH:      //  34
      {
        gl1_rxd_channel = GL1_RXD_IDLE_CHANNEL;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
        MSG_GERAN_MED_1(GL1_RXD"IDLE CH Type chan_type=%d",chan_type);
#endif
        break;
      }
  /* Adding all the other channel types for debugging purposes*/
    case  GL1_HW_SCH:        //  9   ---- temp added
      {
        gl1_rxd_channel = GL1_RXD_SCH;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
        MSG_GERAN_MED_1(GL1_RXD"GL1_RXD_SCH Type chan_type=%d",chan_type);
#endif
        break;
      }

    case  GL1_HW_RACH:      //  10  ---- temp added
    case  GL1_HW_PRACH_ACC:    //  22  ---- temp added
    case  GL1_HW_PRACH_EXT:    //  30  ---- temp added
      {
        gl1_rxd_channel = GL1_RXD_RACH_PRACH;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
        MSG_GERAN_MED_1(GL1_RXD"@IMRD DBG: GL1_RXD_RACH_PRACH Type chan_type=%d",chan_type);
#endif
        break;
      }

    case  GL1_HW_PDTCHU_CS1:
    case  GL1_HW_PDTCHU_CS2:
    case  GL1_HW_PDTCHU_CS3:
    case  GL1_HW_PDTCHU_CS4:
    case  GL1_HW_PDTCHU_MCS1:
    case  GL1_HW_PDTCHU_MCS2:
    case  GL1_HW_PDTCHU_MCS3:
    case  GL1_HW_PDTCHU_MCS4:
    case  GL1_HW_PDTCHU_MCS5:
    case  GL1_HW_PDTCHU_MCS6:
    case  GL1_HW_PDTCHU_MCS7:
    case  GL1_HW_PDTCHU_MCS8:
    case  GL1_HW_PDTCHU_MCS9:
    case  GL1_HW_PTCCHU_ACC:
    case  GL1_HW_PACCHU:
    case  GL1_HW_PTCCHU_EXT:
    case  GL1_HW_PACCHU_EXT:
    case  GL1_HW_RRBP_HI:
    case  GL1_HW_RRBP_LO:
      {
        gl1_rxd_channel = GL1_UL_OR_RRBP_CHANNEL;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
        MSG_GERAN_MED_1(GL1_RXD"GL1_UL_OR_RRBP_CHANNEL: chan_type=%d !",chan_type);
#endif
        break;
      }

    default:
      gl1_rxd_channel = GL1_RXD_CH_UNDEFINED;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
      MSG_GERAN_ERROR_1(GL1_RXD"GL1_RXD_CH_UNDEFINED: chan_type=%d !",chan_type);
#endif
      break;
   }

   return(gl1_rxd_channel);
}

/*===========================================================================

FUNCTION gl1_hw_get_dtm_expected_power_divrx

DESCRIPTION
   returns current expected Rx power level

===========================================================================*/
int32 gl1_hw_get_dtm_expected_power_divrx(gas_id_t gas_id)
{
  /* return signed dBm value */
  return(gl1_hw_dtm_params[gas_id].rx.expected_power_divrx);
}

/*===========================================================================
FUNCTION gl1_run_divrx_stream_sel_avg

DESCRIPTION
Maintain a running average of stream selection for DRx during one SACCH period.

===========================================================================*/
void gl1_run_divrx_stream_sel_avg (boolean divrx_stream_selected, gas_id_t gas_id)
{
    gl1_total_burst_cnt[gas_id]++;
    if(divrx_stream_selected)
        {
         gl1_divrx_sel_cnt[gas_id]++;
        }
}

/*===========================================================================

FUNCTION gl1_get_divrx_stream_sel_rate

DESCRIPTION
Returns TRUE if the percentage rate of DRx selection during the last SACCH period
 is higher than 70 %

===========================================================================*/
boolean gl1_get_divrx_stream_sel_rate (gas_id_t gas_id)
{
    gl1_last_divrx_stream_avg[gas_id] = (gl1_divrx_sel_cnt[gas_id] * 100)/gl1_total_burst_cnt[gas_id];

  MSG_GERAN_HIGH_1_G(GL1_RXD"RxDiv stream selected for %d percent", 
                     gl1_last_divrx_stream_avg[gas_id]);  

  if (gl1_last_divrx_stream_avg[gas_id] >= 70)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION gl1_reset_divrx_sel_avg

DESCRIPTION
Cleans / resets the burst cnt and divrx sel cnt at a SACCH boundary or at 
    diversity enter/exit mode requests
if clean_last_avg is set (enter/exit mode req) we reset also the last saved avg 

===========================================================================*/
void gl1_reset_divrx_sel_avg (boolean clean_last_avg,gas_id_t gas_id)
{
    gl1_total_burst_cnt[gas_id] = 0;
    gl1_divrx_sel_cnt[gas_id] = 0;
    if(clean_last_avg)
    {
      gl1_last_divrx_stream_avg[gas_id] = 0; 
    }
}
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE

/*===========================================================================

FUNCTION gl1_get_IMRD_ctl_nv

DESCRIPTION
    Returns nv set enableIMRD control flag
===========================================================================*/
boolean gl1_get_IMRD_ctl_nv(gas_id_t gas_id)
{
 return(gl1_rxd_control_flag[gas_id].enableIMRD);
}

/*===========================================================================

FUNCTION gl1_get_rxd_ctl_nv

DESCRIPTION
    Returns nv set enableRxD control flag
===========================================================================*/

boolean gl1_get_rxd_ctl_nv(gas_id_t gas_id)
{
 return(gl1_rxd_control_flag[gas_id].enableRxd);
}

#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

#ifdef FEATURE_GSM_RX_DIVERSITY

/*===========================================================================

FUNCTION gl1_get_MSRD_ctl_nv

DESCRIPTION
  Returns nv set enableMSRD control flag
===========================================================================*/
boolean gl1_get_MSRD_ctl_nv(gas_id_t gas_id)
{
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
 return(gl1_rxd_control_flag[gas_id].enableMSRD);
#else
 return FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
}
#endif /*FEATURE_GSM_RX_DIVERSITY*/


/*===========================================================================

FUNCTION gl1_get_Rxd_threshold

DESCRIPTION
  Returns nv set for Prx-Drx power difference threshold
===========================================================================*/
uint8 gl1_get_Rxd_threshold(gas_id_t gas_id)
{
    uint8 thresh;
    thresh = gl1_rxd_control_flag[gas_id].threshold;
    return thresh;
}


#ifdef FEATURE_AEQ_PHASE4

/*===========================================================================

FUNCTION gl1_get_aeq4_info

DESCRIPTION
   returns mdsp_aeq4_ctl[gas_id] for maintaining the AEQ4 control in GL1.

===========================================================================*/
gl1_hw_dtm_rx_aeq4_type gl1_get_aeq4_info(gas_id_t gas_id)
{
 return (mdsp_aeq4_ctl[gas_id]);
}

/*===========================================================================

FUNCTION gl1_get_enableAeq4_status

DESCRIPTION
   returns mdsp_aeq4_ctl[gas_id] for maintaining the AEQ4 control in GL1.

===========================================================================*/
boolean gl1_get_enableAeq4_status(gas_id_t gas_id)
{
 return (mdsp_aeq4_ctl[gas_id].enableAeq4);
}

/*===========================================================================

FUNCTION gl1_set_aeq4_info

DESCRIPTION
   used by GL1 to update and maintain mdsp_aeq4_ctl[gas_id] members for AEQ4 control.

===========================================================================*/

void gl1_set_aeq4_info (gl1_hw_dtm_rx_aeq4_type mdsp_aeq4_ctl_info, uint8 set_mask, gas_id_t gas_id)
{
        //update according to the set mask
    if (set_mask & AEQ4_UPDATE_NV)
    {
      mdsp_aeq4_ctl[gas_id].control_nv = mdsp_aeq4_ctl_info.control_nv;
    }
    if(set_mask & AEQ4_ENABLED)
    {
      mdsp_aeq4_ctl[gas_id].enableAeq4 = mdsp_aeq4_ctl_info.enableAeq4;     
    }
    if(set_mask & AEQ4_SACCH_CNT)
    {
      mdsp_aeq4_ctl[gas_id].sacch_cnt = mdsp_aeq4_ctl_info.sacch_cnt;
    }

  MSG_GERAN_HIGH_3("AEQ4 DBG: UPDATED: control_nv=%d, enableAeq4=%d, sacch_cnt=%d ",
                    mdsp_aeq4_ctl[gas_id].control_nv,mdsp_aeq4_ctl[gas_id].enableAeq4,mdsp_aeq4_ctl[gas_id].sacch_cnt);
}

/*===========================================================================

FUNCTION gl1_reset_aeq4_ctl

DESCRIPTION
   Making sure we reset aeq4 flag and counter when exiting voice call or moving to DTM.

===========================================================================*/

void gl1_reset_aeq4_ctl (gas_id_t gas_id)
{
    if(mdsp_aeq4_ctl[gas_id].enableAeq4)
    {
      mdsp_aeq4_ctl[gas_id].sacch_cnt = 0xFF;
      mdsp_aeq4_ctl[gas_id].enableAeq4 = FALSE;
      MSG_GERAN_HIGH_0_G("AEQ4 SWITCHED OFF! GL1 cleanup");
    }
}

#endif /*FEATURE_AEQ_PHASE4 */

#ifdef GERAN_L1_IP2CAL
/*===========================================================================

FUNCTION gl1_hw_send_ip2cal_cmd

DESCRIPTION
   Send IP2Cal commands to RF and FW

===========================================================================*/
void gl1_hw_send_ip2cal_cmd(uint8 num_of_ip2cal_steps,uint32 ip2Cal_StepSizeQS,gas_id_t gas_id)
{
  int32 fw_offset_to_meas_start;

  /*Sending IP2 Cal command to RF*/
  gl1_hw_rf_ip2_cal(gas_id);
  
  fw_offset_to_meas_start = gl1_hw_get_rf_gsm_ip2cal_setup_time(gas_id) + GL1_HW_IP2SCHEDULE_GUARD_PERIOD;
  
  MSG_GERAN_HIGH_1_G("fw_offset_to_meas_start:%d",fw_offset_to_meas_start);

  /*Sending IP2CAL command to FW*/
  mdsp_schedule_ip2cal(fw_offset_to_meas_start,num_of_ip2cal_steps,ip2Cal_StepSizeQS,gas_id);
}
#endif /*GERAN_L1_IP2CAL*/


/* EOF */
