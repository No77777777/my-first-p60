/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    L A Y E R  1  - M D S P   C O M P R E S S E D   M O D E    I N T E R F A C E

GENERAL DESCRIPTION

    This module handles communications to and from the mDSP for compressed mode
    activities.  It operates in conjunction with mdsp_intf.c  Commands that are
    only used for compressed mode reside within this module.  Commands that are
    used by GSM as well will reside in mdsp_intf.c




EXTERNALIZED FUNCTIONS
    mdsp_cm_cleanup
    mdsp_cm_startup
    mdsp_cm_power_measure
    mdsp_cm_get_power_measure_results
    mdsp_cm_init
    mdsp_build_fw_burst_buffer
    mdsp_cm_active


INITIALIZATION AND SEQUENCING REQUIREMENTS
   mdsp_switch_command_buffers() must be istalled as a frame tick handler on the
   gsm frame tick irq.  mdsp_cm_init() should be called prior to using any of the
   other functions in this module.

  Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmdsp6/src/mdsp_cm.c#2 $
$DateTime: 2019/11/27 05:34:59 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ------- ------------------------------------------------------
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
18/01/16     zf      CR961805: Set first burst flag in x2g_startup cmd
27/04/17     dg      CR2038208 Print current Qs value during L2G/W2G startup and during CXM register command sent to GFW
06/10/16   sk       CR1074515  Set x2gStartupCmd.readrfalpha flag FALSE in case of IRAT 
11/08/16   sk       CR1048655  FW is ordered to read RF Alpha values from RF without which RSSI was 
                    getting floored leading to xCCH failure, if it's first QTA after Stack activation
21/04/17      br     CR1041084 FR 36219: GDET – GMSK Modulation Detector
30/09/15     cws     CR913773 Use TRM allocated device to get cxm channel ID for CM preload/startup
07/09/15     npt     CR898493 GL1 NV control of T2G acquisition GFW error recovery 
19/08/15     mc      CR874662 CXM Freq ID support for X2G startup and X2G preload
14/08/15     cja     CR890072 Dual data space mdsp_cmd_buf_ptr for delayed T2G gaps on GS2
31/07/15     zf      CR881498: Do x2g_startup during X2G_Preload
24/07/15     mc      CR878346 : Source RAT cannot be mapped to CXM Tech Type for NULL2X IRAT activities leading to MCS Channel ID assert
17/07/15     mc      CR871205 : Incorrect CXM Channel ID assigned to W2G activities in W+W mode
03/07/15     jj      CR  865562 Remove featurisation introduced for CR839611
18/05/15     jj      CR 839611 New skip last measurement in T2G if command is late 
19/05/15     zf      CR840672: New WL1 GL1 interface to indicate IRAT in QTA operation
31/03/15     zf      CR811937: Cleanup CM preload API.
16/04/15     smd     CR822202 Dual space data for IRAT and QTA
16/03/15     pg      CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
27/02/15     cja     CR768169 Add RF Seq Number
09/02/15     jj      CR 793229 making debug f3 for crash in the middle of the burst under NV control 
17/11/14     jj      CR 752202 GL1 - bolt - Debug support for crashing in the 
                      middle of rx burst for a given range of rssi value
11/02/15     am      CR719452 Addressing KW GL1 P1 errors
23/01/15     npt     CR697236 Mainline FEATURE_GERAN_VSTMR
09/01/15     dg      CR779348 SKip index 0 while retrieving power meas. results if the flag skipfirstburst is set
05/01/14     ca      CR774877 initialising isTuneAway in single SIM mode
28/11/14     jj      CR 764201 power measurement ready mask change 
08/12/14     og      CR766189: buffer overflow problem in mdsp_cm_get_power_measure_results( ).
14/11/14     cja     CR754529 when prebuilding scripts, have preload offset as immediate
27/10/14     cja     CR746760 In X2G power measure tie results buffer index to state machine
07/10/14     cja     CR730201 Send rf meas exit after cleanup rather than waiting inline for RF scripts
09/09/14     jk      CR697308:GL1 changes to support HLin LLin sawless feature
09/09/14     jk      CR722274:Device ID to be passed in FW enter mode command 
28/08/14     dp      CR716947: Make us crash for debug when GFW doesn't return RSSI results
23/08/14     mc      CR707745: [BOLT2.0][SG+G]: Channel ID assignment in X2G for IRAT measurements
28/07/14     zf      CR700730: Removing RXLM buf idx in set GFW app mode
27/07/14     ap      CR678067: Race condition between APP mode as ACTIVE and Device mode change as DSDA leads to crash
18/07/14     pg/pa	 CR694601:GERAN L1 support for dedicated WTR hopping with FED architecture
03/07/14     og      CR652720. L2G DSDA bring-up updates.
02/07/14     mc      CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
24/06/14     og      CR638097. L2G DSDA support.
28/05/14     cja     CR657368 Fix Klockwork issue of NULL data space pointer
21/05/14     ssh     CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub
15/05/14     ap      CR635898 Enable 3Way FW Cxm
02/05/14     cja     CR658621 Optimise QTA and T2G F3. 
07/04/14     cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
26/03/14     ab      CR639078 : COEX : CXM Priority and Freq ID for cm_power_meas
12/03/14     pa      CR630228:Bolt: Changes due to obsolete FW API cleanup
27/02/14     og      CR606569. Correction to T2G CXM logging.
27/02/14     ap      CR600310 Enable Channel id support
23/02/14     mc      CR621753 Back out 3WCXM changes due to stability issues
19/02/14     ap	     CR600310 GSM L1 to send channel Id to GSM FW corresponding to the primary RF device 
07/02/14     cja     CR612831 Always do mdsp_init as part of gl1_hw_init
14/01/14     og      CR589868. T+G DSDA support.
23/12/13     cja     CR593115 Add passing RxLM index in preload command to FW
09/12/13     us      CR562238 Partial T2G gap usage for RSSI measurement
06/12/13     ap      CR585801 uninitialized variable being used when FEATURE_CXM_QTA is not enabled 
12/11/13     cja     CR575034 Add IRAT preload script for X2G.
22/11/13     cja     CR455527 Remove featurisation from files in API directory
15/11/13     ap      CR575548 FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS
17/09/13     cja     CR546121 Send generic config for x2g start alpha before first x2g startup.
13/09/13     dv      CR544094 CCS crash due to rfBufIndex not being incremented 
16/09/13     sk      CR545664 Ensure to set the isTuneAway flag correctly inside the partial QBTA gap
10/09/13     og      CR529940. No QTA tuneaway.
21/08/13     npt     CR531633 - Remove FEATURE_DIME_MODEM dependencies 
23/08/13     sk      CR533467 Partial QBTA bringup on 2.0 in DSDS
20/08/13     cs      CR531664 Remove FTSM checks in cm_startup/cleanup during QTA as always set to zero
15/08/13     sk      CR524039 GBTA bringup changes
07/08/13     cja     CR524892 QTA uses immediate cmd after startup/cleanup, but should not inc current buffer mod3
05/08/13     cja     CR514498 Only increment current_buffer_mod_3 once per frame, plus for T2G async
31/07/13     cja     CR514255 Only add generic config commands when issue message to FW
10/06/13     cja     CR498279 Do not increment current_buffer_mod_3 twice in G2W only frame
02/08/13     sk      CR522533 Ensure CM cleanup offset is 0qs inside the partial QBTA gap
29/07/13     sk      CR519663 Partial QBTA bringup changes
21/06/13     sk      CR502900 Ensure GSTMR2 uses correct host interface segment to populate the commands
20/06/13     cs      Major Triton TSTS Syncup
02/06/13     sai     CR 481896: Ensure GL1-GFW state coherence for W2G BPLMN searches
21/02/13     dv      CR448609 Handle T2G HO GFW transition from MEAS mode to ACTIVE mode
23/05/13     ap      CR491318: Reverted GFW state transitions regression 485912
22/05/13     ap      CR485912: Revised GFW state transitions
25/04/13     zf      Change funcation get_private_dataspace_ptr to static.
20/04/13     ws      CR 478028 Remove TD=IRAT feature around mdsp_async_intf_send_immediate_cmd()
15/04/13     cja     CR475568 Correct T2G data metrics
13/04/13     mc      CR471759 DSDS: QTA Support : Initial code changes & Triton Bring-Up
28/03/13     ss      CR474761 Populate cmdType and nBursts for T2G pwr measure command
28/03/13     ss      CR468974 Return correct cm power measure results
25/03/13     cja     CR466302 Don't increment current_buf_mod3 if already done by immediate command
27/03/13     ss      CR467967 Populate cmdType and nBursts for X2G pwr measure command
20/02/13     cja     CR455032 Changes for build without TD feature
20/02/13     cja     CR443834 Extend mdsp_awake to indicate FW enable/disable
05/02/13     cja     CR440314. Don't issue commands using DPC in compressed mode init.
29/01/13     cja     CR446356 Set msgr_init_hdr 'from' parameter as MSGR_GERAN_GL1
23/01/13     cja     CR443889 Tidy up rat_info.mode
22/01/13     cja     CR443834 Extend mdsp_awake to indicate FW enable/disable
09/01/13     cja     CR438712 Set FN to match command FN/offset for mdsp immediate cmd
04/01/13     cja     CR437603 Remove use of AsyncImmediateCmd, always use AsyncCmd
10/12/12     cja     CR411308 Dime T2G changes (msg router to GFW)
07/12/12     cja     CR429173 Remove Dime compiler warnings
05/12/12     pg      CR427361: GFW/RF shared memory interface cleanup
19/11/12     cja     CR422874 Add pseudo Hw FN for Dime
31/10/12     pg      CR409233: updates for G2W on Dime modem
04/09/12     og      CR391191. Prevent T2G RSSI 0 results.
15/08/12     cja     CR385484 Add flag for async interrupt in num cmds
07/08/12     dv      CR371181 GFW CMDS picked up twice due to race condition
                     between ftick and async int
18/07/12     cja     CR376909 For non TD builds ensure earlyTaskqRelease is FALSE
24/09/12     ky      T2G Async message router changes
03/07/12     ky      Async message router sleep, app mode, enter mode interface changes
27/06/12     ky      Added support of using new CCS interface and passing new CCS
                     buffer pointers in RF task messages
30/05/12     og      CR371570. Set the early TQ release for T2G cleanup.
11/06/12     og      CR369985. Wrong RSSI values during T2G dedicated and idle modes.
11/06/12     cja     Fix compiler error for non TDS builds
08/06/12     cja     CR363269 Do not clear IRAT SBI/NonSBI buffers
25/05/12     pg      Add support for RF Task
21/05/12     og      CR361191.
28/04/12     og      CR357019. GSM and MDSP drivers changes to reduce stack usage.
05/04/12     pa      CR349568:T2G Incorrect reading of power measurement result
21/03/12     ws      CR 345606 Improve stability of X2G Rxfe stuck recovery
02/04/12     dv      T2G:CR348512 remove FTSM restriction for startup/cleanup values
30/03/12     ws      CR345370 - remove unnecessary call to mdsp_awake()
14/03/12     pg      Add support for local HWIO macros (msm.h deprecated)
14/03/12     pg      Add support for DAL interrupt controller (tramp deprecated)
29/02/12     dv      T2G:CR339879 Incorrect interpretation of Timing info sent to GFW
22/02/12     og      CR338781. Added async interrupt logging to the mdsp_cmd_trace_buf.
20/02/12     dv      T2G:Remove abberrations from bringup check-in
15/02/12     dv      Overwrite changes with that from T2G FCCH/SCH bringup
10/02/12     ws      CR 336194 Enable TD-SCDMA IRAT for GERAN
06/02/12     cja     CR331207 Send generic config command after mdsp_wakeup
02/02/12     cja     CR329327 Ensure mdsp init complete before using command buffer
07/11/11     jj      CR315253 IQ capture capability for GSM via QRCT
08/09/11     jj      CR302378  Added GL1 changes for RSB along with spectral inversion
28/04/11     cja     Add invalid RxLM for measurement mode
08/04/11     ky      Brought SPI feature changes under FEATURE_GSM_RAFT_SPI_CONTROL
07/04/11     ky      Changed sbi_infor pointer to rtr_sbi_infor pointer in SPI feature
20/04/11     cja     Nikel GFW interface changes
06/04/11     cja     Nikel IRAT
27/01/11     cja     Changes for RxLM and TxLM for NikeL
08/11/10     nf      9K suspend resume
03/11/10     ky      Fix the compiler warnings for 9k
25/10/10     ky      259645: Installed gstmr handler for sending pending commands is not working.
                     Hardcoded the sending these commands in gstmr isr function.
18/10/10     ky      259645: Installed gstmr handler for sending pending commands. This handler
                     sends the generic config command and app mode command to fw if they are
                     pending
09/10/10     nf      9K added FWS suspend/resume
26/08/10     ky      251151: Before sending gsm app mode command we are waiting
                     for a frame tick.
19/07/10     ky      Klocwork fixes
14/05/10     pg      Add missing FEATURE_GSM_GPRS_GENESIS around
                     call to mdsp_set_gsm_app_mode()
17/03/10     og      Further x2g and g2x updates.
10/03/10     ws      Further featurisation of G2X and X2G
05/03/10     og      Ensure the correct grfc and sbi pointers are filled in
                     the cm startup and cleanup commands.
01/03/10     og      g2x and x2g updates.
25/02/10     og      Added cm fw enable/disable and set app mode.
23/02/10     og      Added x2g startup and cleanup commands.
22/02/10     og      Increase the maximum number of compressed mode power
                     monitors to 10 per frame.
04/02/09     ws      Removed leagcy Q4 MDSP services and image for Q6 targets
25/11/09     ws      Removed mdsp_read's and old Q4 version numbers for Q6 build
23/07/09     ws      Added FEATURE_GSM_GPRS_QDSP6 to remove QDSP4 references as part
                     of GERAN DSP migration to QDSP6
29/07/08     ws      mdsp Stucture initialisations to resolve Klockwork warnings
12/01/05     gfr     Change FEATURE_GSM_MDSP_APPS_ENABLED to
                     FEATURE_GSM_MDSP_APPS_ENABLED_CM to disable in CM
11/07/05     gfr     Use mdsp_ftsm() instead of global variable, enable
                     mDSP apps logging in CM, attempt to recover from invalid
                     cm_startup/cleanup offsets instead of error fatalling
10/04/05     gfr     Improve debug msg in mdsp_build_fw_setup_buffer
09/22/05     gfr     Removed FEATURE_SBI_MDSP_CONTROLLER
08/19/05     gfr     Use mdsp_burst_current_buffer
08/16/05     gfr     Change int to uint8 to type compatibility, make all lines
                     less than 90 characters long
07/29/05     gfr     DTM support
07/01/05     gfr     Set cmd length properly for DTM
04/28/05     gfr     Added mdsp_cm_inact, register gstmr handler here
04/08/05     gfr     Add version message at init.
04/03/05     gfr     Check CM_CLEANUP semaphore earlier if possible
02/11/05     gfr     Added missing include file.
02/09/05     yh      Fix the sbi/nonsbi buffers in mdsp_cm_power_measure
01/24/05     gfr     General cleanup and restructure.
12/06/04     gfr     Featurize the mDSP SBI controller
09/29/04     gfr     Save GSM RAM version to a global variable for debugging.
08/09/04     gfr     Sanity check startup and cleanup offsets.
06/28/04     gfr     Fixed missing third async data buffer pointer.
06/09/04     gfr     Added third async data buffer.
04/27/04      jc     Use CM_STARTUP_BURST. Same functionality.
04/26/04      bm     Mainlined FEATURE_MDSP_NO_STATIC_BUFFERS
04/26/04      bm     Initialize debug_buffer with different offsets than in
                     idle mode. Otherwise this causes writting into some one
                     elses buffer as the allocated buffer space is different in
                     GSM image and WCDMA image
04/13/04      bm     Call mdsp_gsm_drv_wakeup instead of mdsp_wakeup
03/05/04      gfr    Lint cleanup.
08/15/03      jc     Make mon metrics a buffer of 3. Needed for ftsm problems.
07/07/03      jc     Remove kovuramversion.
04/27/03      jc     Remove L1_get_FN().
04/02/03      jc     Use CM_CLEANUP_BURST enum.
12/18/02      jc     Removed sbii.h
11/01/02      jc     Add include file. Init static buffer pointers that
                     were missing re: FEATURE_MDSP_NO_STATIC_BUFFERS
10/28/02      gw     Added support for back-to-back async receives.
10/18/02      jc     Correct array too small - stack crash when calling
                     mdsp_cm_get_power_measure_results().
10/14/02      jc     Added mdsp_cm_get_power_measure_results().
10/03/02      jc     Wakeup the driver in init.
10/01/02      jc     Removed sbi_status/sbi_req buffers to mdsp_sbi.c
09/28/02      jc     Fix cleanup command to clear semaphore later.
08/07/02      jc     Added sbi buffer to cm_startup command
08/02/02      jc     Change to support mdsp services.
06/26/02      tb     Change MDSP_RAM_VERSION to MDSP_kovuRamVersion.
06/02/02      jc     Add support for up to 9 power measure commands.
06/02/02      jc     Add support for async_rx.
04/05/02      jc     Initial version.
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"
#include "mdsp_debug.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_async_intf.h"
#include "mdsp_cmd.h"
#include "msg.h"
#include "err.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "vtmrs_g.h"
#include "l1_drx.h"
#include "l1_utils.h"
#include "gfw_sw_intf.h"
#include "fws.h"
#include "gl1_hw.h"

#include "gl1_hw_g.h"
#include "geran_dual_sim_g.h"

#include "gl1_arbitrator_cxm.h"

#include "modem_fw_memmap.h"
#ifdef FEATURE_GSM_TDS
#include "mcs_hwio.h"

#include "DDIInterruptController.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#endif
/* Quarter-symbols per frame */
#define QS_PER_FRAME 5000

#if defined (FEATURE_QTA ) && defined (FEATURE_CXM_QTA)
extern garb_arbitrated_gas_t arbitrated_gas[NUM_GERAN_DATA_SPACES];
#endif

/*==========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

==========================================================================*/


/* Which burst types to use */
mdsp_burst_type mdsp_cm_mon_metrics_burst_types[MAX_CM_RSSI_MEASUREMENTS] =
{
   CM_MON_BURST_1,
   CM_MON_BURST_2,
   CM_MON_BURST_3,
   CM_MON_BURST_4,
   CM_MON_BURST_5,
   CM_MON_BURST_6,
   CM_MON_BURST_7,
   CM_MON_BURST_8,
   CM_MON_BURST_9
#if defined  FEATURE_LTE
   ,CM_MON_BURST_10
#endif
};


#ifdef FEATURE_GSM_TDS
#define MDSP_ASYNC_CMD_RSP_DELAY  50

#if  !defined(UNICORE_MODEM)
DalDeviceHandle* gsmCM_hInterruptHandle;
/* Interrupt vector number - in legacy tramp usage this was the tramp id */
#define TRAMP_Q6B_7_IRQ 285
#endif
#endif
/*===========================================================================
  MULTI-DATASPACED PRIVATE DATA
===========================================================================*/
typedef struct
{
  /* Sequence numbers used by various commands - used to correlate results  */
  uint16 mdsp_power_measure_seq_num[GFW_X2G_PWR_MON_BUFFERING];
  GfwX2GPwrMsrCmd               x2gPwrMsrCmd;
  mdsp_cm_power_metrics_struct  data;

  /* Indicate if we are in compressed mode or not */
  boolean mdsp_compressed_mode;
} mdsp_cm_private_data_t;

/*===========================================================================
  PUBLIC AND PRIVATE DATASPACES
===========================================================================*/
static mdsp_cm_private_data_t mdsp_cm_private_data[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION get_private_dataspace_ptr

DESCRIPTION
  This function gets the private data space for a specified GAS ID

===========================================================================*/
static mdsp_cm_private_data_t* get_private_dataspace_ptr(const gas_id_t gas_id )
{
 mdsp_cm_private_data_t *data_ptr;

 data_ptr =
      &(mdsp_cm_private_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(check_gas_id(gas_id))]);

 return ( data_ptr );
}

/*===========================================================================

FUNCTION init_private_data_space

DESCRIPTION
  This function initialises a public data space

===========================================================================*/
static void init_private_data_space( gas_id_t gas_id )
{
  int i;
  mdsp_cm_private_data_t* data = get_private_dataspace_ptr( gas_id);

  data->data.len                = 0;
  data->data.seq_num            = 0;

  for(i=0;i<MAX_CM_RSSI_MEASUREMENTS;i++)
  {
    data->data.rssi[i][0] = 0;
    data->data.rssi[i][0] = 1;
  }

  data->mdsp_compressed_mode = FALSE;
}

/*===========================================================================

FUNCTION mdsp_cm_power_measure

DESCRIPTION
  This function tells the mDSP to perform power measurements.

  num - number of bursts to perform power measurements over

DEPENDENCIES
  None

RETURN VALUE
  Results are stored in the monitor buffer.

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_cm_power_measure( uint16                   num,
#ifdef  FEATURE_GSM_COEX
                            ARFCN_T                  ARFCN[MAX_CM_RSSI_MEASUREMENTS],
#endif
                            uint16                   offset[MAX_CM_RSSI_MEASUREMENTS],
                            uint32                   rf_seq_num[MAX_CM_RSSI_MEASUREMENTS],
#ifdef  FEATURE_GSM_COEX
			                      mdsp_gprs_monitor_coex_params  coex_params[MAX_CM_RSSI_MEASUREMENTS],
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
                            boolean                  sawless_support[MAX_CM_RSSI_MEASUREMENTS],
#endif /*GERAN_L1_HLLL_LNASTATE*/
                            gas_id_t gas_id
                          )
{
   mdsp_burst_type   burst_type;
   uint8             i, buf;

   mdsp_cm_private_data_t*  private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* Sanity check */
   if (num == 0 || num > MAX_CM_RSSI_MEASUREMENTS)
   {
      MSG_GERAN_ERROR_2_G("Invalid num of CM pwr meas %d, FN=%d", num, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   /* Clear out Q6 command first */
   memset( &(private_data_ptr->x2gPwrMsrCmd), 0, sizeof(GfwX2GPwrMsrCmd) );

   private_data_ptr->x2gPwrMsrCmd.cmdType        = GFW_X2G_PWR_MSR_CMD;
   private_data_ptr->x2gPwrMsrCmd.nBursts        = num;
#ifdef GL1_GMSK_MOD_DET
   private_data_ptr->x2gPwrMsrCmd.enableGmskDetect        = FALSE;
#endif
   private_data_ptr->x2gPwrMsrCmd.metricsBufferIndex = mdsp_current_buffer_mod3( gas_id);

#ifdef FEATURE_GSM_TDS_DEBUG
   MSG_GERAN_HIGH_3_G("T2G GFW_X2G_PWR_MSR_CMD Write: current_buff_mod3 %d, current_buffer %d, x2gPwrMsrCmd.nBursts %d",
            mdsp_current_buffer_mod3(gas_id), mdsp_current_buffer(gas_id), num );
#endif

   for (i = 0; i < num; i++)
   {
     burst_type = mdsp_cm_mon_metrics_burst_types[i];
     buf        = mdsp_burst_current_buffer(burst_type , gas_id);

     private_data_ptr->x2gPwrMsrCmd.ctrl[i].logIq     = mdsp_intf_data_ptr->gsm_Iq_Log. x2g_pwr_meas_iq;
     private_data_ptr->x2gPwrMsrCmd.iqBufferIndex     = mdsp_current_buffer( gas_id);
#ifdef FEATURE_GSM_RF_DEBUG_NV
     if ( mdsp_rf_debug_NV_enable_g [gas_id] )
     {
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].debugCrash = get_flag_for_fw_pwr_meas_crash(i ,gas_id );
     MSG_GERAN_HIGH_2_G("pwr meas debugCrash = %d ,i = %d ", get_flag_for_fw_pwr_meas_crash(i ,gas_id ),i); 
     }
	 else
	 {
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].debugCrash =  FALSE ;
	 }
#else
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].debugCrash = FALSE;
#endif /* FEATURE_GSM_RF_DEBUG_NV */

#ifdef  FEATURE_GSM_COEX
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].arfcn     = ARFCN[i].num;
#endif

     private_data_ptr->x2gPwrMsrCmd.ctrl[i].offset    = offset[i] + mdsp_ftsm( gas_id);

#ifdef FEATURE_GSM_TDS_DEBUG
     MSG_GERAN_HIGH_3_G("T2G GFW_X2G_PWR_MSR_CMD Write: buf %d, burst_type %d cmd.x2gPwrMsrCmd.ctrl[i].offset %d",
            buf,burst_type,cmd.x2gPwrMsrCmd.ctrl[i].offset );
#endif

#ifdef  FEATURE_GSM_COEX
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].cxmPriority      = coex_params[i].coex_priority;
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].cxmDesenseId     = coex_params[i].coex_desense_id;
#endif

     if(buf <= MDSP_MAX_CURRENT_BUFFER_INDEX)
     {
       private_data_ptr->x2gPwrMsrCmd.ctrl[i].rfBufIndex = mdsp_current_buffer_mod3( gas_id);
#ifdef FEATURE_GSM_TDS_DEBUG
   MSG_GERAN_HIGH_2_G("T2G GFW_X2G_PWR_MSR_CMD Write: i %d, grfcSbiBuffIndex %d",
            i,x2gPwrMsrCmd.ctrl[i].grfcSbiBuffIndex);
#endif
     }

#ifdef GERAN_L1_HLLL_LNASTATE
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].doJdetReading = sawless_support[i];
#endif /*GERAN_L1_HLLL_LNASTATE*/

     private_data_ptr->x2gPwrMsrCmd.ctrl[i].rfSeqNr = rf_seq_num[i];
   }

   mdsp_issue_qdsp6_command((GfwCmdBufType *) &private_data_ptr->x2gPwrMsrCmd, GFW_X2G_PWR_MSR_CMD , gas_id);


   /* retrieve the seq num for correlation later */
   private_data_ptr->mdsp_power_measure_seq_num[mdsp_current_buffer_mod3( gas_id)] = mdsp_get_current_seq_num( gas_id);

#ifdef FEATURE_GSM_TDS_DEBUG
   MSG_GERAN_HIGH_2_G("T2G GFW_X2G_PWR_MSR_CMD Write: current_buffer_mod3 %d mdsp_power_measure_seq_num[current_buffer_mod3] %d",
            mdsp_current_buffer_mod3( gas_id), private_data_ptr->mdsp_power_measure_seq_num[mdsp_current_buffer_mod3( gas_id)]);

#endif

}


void  mdsp_x2g_power_measure( uint16                         num,
                              uint8                          results_buffer,
                              ARFCN_T                        ARFCN[MAX_CM_RSSI_MEASUREMENTS],
                              uint16                         offset[MAX_CM_RSSI_MEASUREMENTS],
                              uint32                         rf_seq_num[MAX_CM_RSSI_MEASUREMENTS],
#ifdef  FEATURE_GSM_COEX
			       mdsp_gprs_monitor_coex_params  coex_params[MAX_CM_RSSI_MEASUREMENTS],
#endif
                              gas_id_t gas_id,
                              void **cmdbuf_ptr
                            )
{
   mdsp_burst_type   burst_type;
   uint8             i, buf;

   mdsp_cm_private_data_t*  private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* Sanity check */
   if (num == 0 || num > MAX_CM_RSSI_MEASUREMENTS)
   {
      MSG_GERAN_ERROR_2_G("Invalid num of CM pwr meas %d, FN=%d", num, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   /* Clear out Q6 command first */
   memset(&(private_data_ptr->x2gPwrMsrCmd),0,sizeof(GfwX2GPwrMsrCmd));

   private_data_ptr->x2gPwrMsrCmd.cmdType        = GFW_X2G_PWR_MSR_CMD;
   private_data_ptr->x2gPwrMsrCmd.nBursts        = num;
#ifdef GL1_GMSK_MOD_DET
   private_data_ptr->x2gPwrMsrCmd.enableGmskDetect        = FALSE;
#endif
   private_data_ptr->x2gPwrMsrCmd.metricsBufferIndex = results_buffer;

   for (i = 0; i < num; i++)
   {
     burst_type = mdsp_cm_mon_metrics_burst_types[i];
     buf        = mdsp_burst_current_buffer(burst_type , gas_id);

     private_data_ptr->x2gPwrMsrCmd.ctrl[i].logIq     = mdsp_intf_data_ptr->gsm_Iq_Log.x2g_pwr_meas_iq;
     private_data_ptr->x2gPwrMsrCmd.iqBufferIndex     = mdsp_current_buffer( gas_id);

#ifdef  FEATURE_GSM_COEX
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].arfcn     = ARFCN[i].num;
#endif

     private_data_ptr->x2gPwrMsrCmd.ctrl[i].offset    = offset[i] + mdsp_ftsm( gas_id);
#ifdef FEATURE_GSM_RF_DEBUG_NV
     if ( mdsp_rf_debug_NV_enable_g [gas_id] )
     {
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].debugCrash = get_flag_for_fw_pwr_meas_crash(i ,gas_id );
     MSG_GERAN_HIGH_1_G("pwr meas debugCrash = %d ", get_flag_for_fw_pwr_meas_crash(i ,gas_id )); 
     }
	 else
	 {
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].debugCrash =  FALSE ;
	 }
#else  
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].debugCrash = FALSE;
#endif /* FEATURE_GSM_RF_DEBUG_NV */

#ifdef  FEATURE_GSM_COEX
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].cxmPriority      = coex_params[i].coex_priority;
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].cxmDesenseId     = coex_params[i].coex_desense_id;
  //   private_data_ptr->x2gPwrMsrCmd.ctrl[i].prioritySchedule = 0x1; //!< DSDA: 1=schedule; 0=blank
#endif

     if(buf <= MDSP_MAX_CURRENT_BUFFER_INDEX)
     {
       private_data_ptr->x2gPwrMsrCmd.ctrl[i].rfBufIndex = mdsp_current_buffer_mod3( gas_id);
     }
     
#ifdef GERAN_L1_HLLL_LNASTATE
     private_data_ptr->x2gPwrMsrCmd.ctrl[i].doJdetReading = FALSE;
     if(gl1_hw_get_sawless_HLLL_support(ARFCN[i].band,gas_id))
       {
       private_data_ptr->x2gPwrMsrCmd.ctrl[i].doJdetReading = TRUE;
       }
#endif /*GERAN_L1_HLLL_LNASTATE*/

     private_data_ptr->x2gPwrMsrCmd.ctrl[i].rfSeqNr = rf_seq_num[i];
   }

   mdsp_issue_qdsp6_command((GfwCmdBufType *) &private_data_ptr->x2gPwrMsrCmd, GFW_X2G_PWR_MSR_CMD , gas_id);
    *cmdbuf_ptr=mdsp_getcmd_buf_address(gas_id);

   /* retrieve the seq num for correlation later */
   private_data_ptr->mdsp_power_measure_seq_num[results_buffer] = mdsp_get_current_seq_num( gas_id);

}

/*===========================================================================

FUNCTION mdsp_cm_get_power_measure_results

DESCRIPTION
  This function retrieves the montior data buffer and ensures that it is a
  power measure result by comparing the seq num in the result with the seq
  num in the command request.  The data pointer is filled with the results
  of the monitor buffer even if the seq num does not match.

  rssi_values - pointer to store results


DEPENDENCIES
  None

RETURN VALUE
  TRUE  - results were available
  FALSE - results were not available

SIDE EFFECTS
  This function is similar to mdsp_get_power_measure_results().
===========================================================================*/
boolean  mdsp_cm_get_power_measure_results( uint32    rssi_values[MAX_CM_RSSI_MEASUREMENTS],
                                            boolean   yield[MAX_CM_RSSI_MEASUREMENTS],
                                            boolean   ScheduleError[MAX_CM_RSSI_MEASUREMENTS],
#ifdef GERAN_L1_HLLL_LNASTATE 
                                            uint16    jdetvalues[MAX_CM_RSSI_MEASUREMENTS],
#endif /*GERAN_L1_HLLL_LNASTATE*/
                                            gas_id_t  gas_id  )
{

   mdsp_cm_private_data_t*   private_data_ptr    = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t*  mdsp_intf_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id);
   uint16  i=0; uint8 ScheduleErrorCount= 0;
   
   boolean                   results             = FALSE;
   boolean                   results_ready       = FALSE;
   const uint8               local_buffer_mod3   = mdsp_current_buffer_mod3( gas_id);

   memset( &private_data_ptr->data, 0, sizeof(mdsp_cm_power_metrics_struct) );

   private_data_ptr->data.len     = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[local_buffer_mod3].length;
   if (private_data_ptr->data.len > MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS )
   {
     private_data_ptr->data.len = MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS ;
   }

   /* Check if the results are valid */
   for( i =0 ; i < private_data_ptr->data.len;i++)
   {
   
    if( TRUE == mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[mdsp_current_buffer_mod3( gas_id)].readyMask[i]) 
      {
        results_ready = TRUE;
      }
    else
      {
        results_ready = FALSE;
        MSG_GERAN_ERROR_3_G("Power measure len= %d readymask[%d]= %d", private_data_ptr->data.len,i, mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[mdsp_current_buffer_mod3( gas_id)].readyMask[i]);
        break;
      }      
    }
   /**
    * CR 349568 Check bit15 or msb to find whether results are
    * ready or not
    */
   if( results_ready )
   {
      /* Read the sequence number */
      private_data_ptr->data.seq_num = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[local_buffer_mod3].seqNum;

 
      /* The results are only valid if the seq num in the request matches */
      if ( private_data_ptr->data.seq_num != private_data_ptr->mdsp_power_measure_seq_num[local_buffer_mod3] )
      {
         MSG_GERAN_ERROR_2_G("Power measure: bad seq num, %d!=%d",
                             private_data_ptr->data.seq_num,
                             private_data_ptr->mdsp_power_measure_seq_num[local_buffer_mod3]
                            );
      }
      else if( private_data_ptr->data.len == 0 )
      {
         MSG_GERAN_ERROR_2_G("Power measure: seq num, %d len %d",
                             private_data_ptr->data.seq_num,
                             private_data_ptr->data.len
                            );
      }
      else
      {
         #define  YIELD_MASK (TX_KNOCK_RX_BIT | RX_KNOCK_MON_BIT | TX_KNOCK_MON_BIT | HOST_ERROR_3 | HOST_ERROR_2)

         uint16  num_meas    = private_data_ptr->data.len ;

         /* Copy the results into the array */
         for (i = 0; i < num_meas; i++)
         {
           if(mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[local_buffer_mod3].scheduleTimeError[i] == TRUE)
           {
             ScheduleError[i] = TRUE;
	     ScheduleErrorCount++; 
           }
	   else
           {
	     ScheduleError[i] = FALSE;
	   }
           rssi_values[i] = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[local_buffer_mod3].rssi[i];

           yield[i]       = ((mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[local_buffer_mod3].reason[i] & YIELD_MASK) != 0);

#ifdef  GERAN_L1_HLLL_LNASTATE 
           jdetvalues[i]  = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[local_buffer_mod3].jdetReading[i];
#endif
         }

         results = TRUE;

         /*Run Error Recovery algorithm even if one burst dropped because of RF script building delay*/
         gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_MON, (ScheduleErrorCount != 0), gas_id);
      }
   }
   else
   {
      MSG_GERAN_ERROR_1_G("Power measure len= %d", private_data_ptr->data.len);

      /* Initialise data len to 0*/
      private_data_ptr->data.len = 0;
   }

   return  results;
}

#ifdef FEATURE_GSM_TDS

boolean  mdsp_x2g_get_power_measure_results( uint32  rssi_values[MAX_CM_RSSI_MEASUREMENTS],
					     boolean  yield[MAX_CM_RSSI_MEASUREMENTS],
					     boolean  ScheduleError[MAX_CM_RSSI_MEASUREMENTS],
                                             uint8   results_buffer,
                                             uint8*  num_ptr     /* Returns the number of measurements processed by GFW */
#ifdef GERAN_L1_HLLL_LNASTATE 
                                             ,uint16  jdetvalues[MAX_CM_RSSI_MEASUREMENTS]
#endif /*GERAN_L1_HLLL_LNASTATE*/
                                             , gas_id_t gas_id
                       )
{
   int                           k;
   boolean                       results;
   boolean                       results_ready = FALSE;
   uint8                         ScheduleErrorCount = 0;

   mdsp_cm_private_data_t*  private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);
  
#ifdef  FEATURE_GSM_COEX
   const uint32             yield_mask         = (HOST_KNOCK_RX_BIT|RX_KNOCK_MON_BIT | TX_KNOCK_MON_BIT | HOST_ERROR_3 | HOST_ERROR_2 | RF_NOT_TUNED);
#else
   const uint32             yield_mask         = RF_NOT_TUNED;
#endif

   memset( &private_data_ptr->data, 0, sizeof(mdsp_cm_power_metrics_struct) );

   private_data_ptr->data.len     = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].length;
   private_data_ptr->data.seq_num = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].seqNum;

   if ( private_data_ptr->data.len > MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS )
   {
       private_data_ptr->data.len = MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS;
   }
   for ( k=0;k< (private_data_ptr->data.len);k++  ) 
   {
       if (TRUE == mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].readyMask[k] )
       {
           results_ready = TRUE;
       }
       else
       {
           results_ready = FALSE;
           MSG_GERAN_ERROR_3_G( " length %d  readymask[%d]= %d ",private_data_ptr->data.len,k,mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].readyMask[k]);
           /*For the case when skip_first_burst is SET for index 0, ignore it and check the rest of the indexes*/
             results_ready = FALSE;
             break;
         
       }
   }

   if (results_ready)
   {
      /* The results are only valid if the seq num in the request matches */
      if ( private_data_ptr->data.seq_num != private_data_ptr->mdsp_power_measure_seq_num[results_buffer] )
      {
         MSG_GERAN_ERROR_3_G("Power measure: bad seq num, %d!=%d length %d", &private_data_ptr->data.seq_num,
                   private_data_ptr->mdsp_power_measure_seq_num[results_buffer], ( ((private_data_ptr->data.len & 0x7fff) - 3)/2 ));

         results = FALSE;
      }
      else if( private_data_ptr->data.len  > 0 )
      {
         uint16  i;
         uint16  num_meas = private_data_ptr->data.len;

         results = TRUE;

         mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].length = 0;

        if( num_meas > MAX_CM_RSSI_MEASUREMENTS )
        {
          MSG_GERAN_ERROR_1_G(" num meas %d ", num_meas);

          num_meas = MAX_CM_RSSI_MEASUREMENTS;
        }

        /* Copy the results into the array */
        for (i = 0; i < num_meas; i++)
        {
         if ( mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].scheduleTimeError[i] == TRUE )
        {
           ScheduleError[i] = TRUE;
           ScheduleErrorCount++; 
         }
	 else
         {
	   ScheduleError[i] = FALSE;
	 }
         rssi_values[i] = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].rssi[i];
         yield[i] = ((mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].reason[i] & yield_mask) != 0);
         if (yield[i] == TRUE)
         {
            MSG_GERAN_HIGH_1_G("X2G power meas not scheduled: yield %d",mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].reason[i]);
         }
#ifdef GERAN_L1_HLLL_LNASTATE 
          jdetvalues[i] = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwX2GPwrMsrResultBuffers[results_buffer].jdetReading[i];
#endif /*GERAN_L1_HLLL_LNASTATE*/
        }

         /*Run Error Recovery algorithm even if one burst dropped because of RF script building delay*/
         gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_MON, (ScheduleErrorCount != 0), gas_id);

         if ( num_meas != *num_ptr )  /* GFW was not able to measure power on all the arfcn that were request*/
         {
             MSG_GERAN_ERROR_3_G(" num pwr meas requested = %d , num pwr measure processed  %d, seq_num =%d",*num_ptr, num_meas, &private_data_ptr->data.seq_num);
         }
         *num_ptr = num_meas;

      }
      else
      {
        results = FALSE;
      }

   }
   else
   {
      MSG_GERAN_ERROR_1_G("X2G power measure results not ready, len= %d", private_data_ptr->data.len);

      /* Initialise data len to 0*/
      private_data_ptr->data.len = 0;
      results = FALSE;
   }

   return  results;

}

#endif
/*===========================================================================

FUNCTION mdsp_cm_preload

DESCRIPTION
  This function constructs and issues the cm preload command to the mdsp.

  pretune_offset - number of quarter symbols from start of frame for start of pretune loading
  startup_offset - number of quarter symbols from start of frame for gap start
  rxlm_buf_idx - RXLM buffer index
  device - RF device
  immediate_preload - If TRUE, GFW will do preload command immediately
  is_a_qta_gap - If TRUE, this is a QTA gap
  do_register_startup - If TRUE, GFW will register CxM for Startup when handling Preload command

===========================================================================*/
void mdsp_cm_preload( uint16 pretune_offset,
                      uint16 startup_offset,
                      uint32 rxlm_buf_idx,
                      rfm_device_enum_type rf_device,
                      boolean immediate_preload,
                      boolean is_a_qta_gap,
                      boolean do_register_startup,
                      mdsp_x2g_coex_params_type coex_params,
                      gas_id_t gas_id )
{
   GfwX2GPreLoadCmd  x2gPreloadCmd;

#ifdef FEATURE_GSM_COEX_FW_CXM
   cxm_channel_in_type  cxm_channel_in  = {0};
   cxm_channel_out_type cxm_channel_out = {0};
   cxm_channel_in.rf_device = rf_device;

   cxm_channel_in.techid    = geran_map_gas_id_to_cxm_id( gas_id );
   cxm_channel_in.rf_device = rf_device;
   cxm_channel_out          = cxm_get_channel_id( cxm_channel_in );
   x2gPreloadCmd.channel_id0 = cxm_channel_out.rx_channel_id;
   MSG_GERAN_HIGH_2_G("X2G IRAT channel id = %d curr_qs = %d", x2gPreloadCmd.channel_id0,gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));

   if( is_a_qta_gap )
   {
     x2gPreloadCmd.actPriority   = coex_params.activity_priority;
/*   x2gPreloadCmd.noActPriority = coex_params.no_activity_priority;   Not part of GfwX2GPreLoadCmd ?? */
     x2gPreloadCmd.desenseId     = coex_params.desense_id;
     x2gPreloadCmd.isTuneAway    = QTA_CM_CxM;
     MSG_GERAN_HIGH_3_G("CXM : cxm actpriority=%d noAct=%d desenseid=%d", x2gPreloadCmd.actPriority, 0/* x2gPreloadCmd.noActPriority */,x2gPreloadCmd.desenseId);
   }
   else
   {
     x2gPreloadCmd.actPriority = 0;
     x2gPreloadCmd.desenseId = 0;
     x2gPreloadCmd.isTuneAway = NO_TUNE_AWAY;
   }

#else
   //Ensure that isTuneAway is set
   x2gPreloadCmd.isTuneAway = NO_TUNE_AWAY;
   MSG_GERAN_HIGH_0_G("isTuneAway set to NO_TUNE_AWAY");
#endif

   x2gPreloadCmd.cmd         = GFW_X2G_PRELOAD_CMD;
   x2gPreloadCmd.preLoadTimeoffset = pretune_offset;
   x2gPreloadCmd.startupTimeoffset = startup_offset;
   x2gPreloadCmd.rxlmBufIndex = rxlm_buf_idx;
   x2gPreloadCmd.allowExpiredOffset = immediate_preload;
   x2gPreloadCmd.doRegisterStartup = do_register_startup;

   mdsp_issue_qdsp6_command( (GfwCmdBufType *) &x2gPreloadCmd, GFW_X2G_PRELOAD_CMD , gas_id );
}

/*===========================================================================

FUNCTION mdsp_cm_startup

DESCRIPTION
  This function constructs and issues the cm startup command to the mdsp.
  Currently only one StartupCommand per frame is supported.

  offset - number of quarter symbols from start of gap

DEPENDENCIES
  mdsp image must already be loaded.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void * mdsp_cm_startup( uint16  offset, 
                        uint8  buffer_index, 
                        uint32 rxlm_buf_idx, 
                        rfm_device_enum_type tqp_device, 
                        rfm_device_enum_type rf_device,
                        boolean is_a_qta_gap,
                        rfcom_mode_enum_type serving_rat_mode,
                        mdsp_x2g_coex_params_type coex_params,
                        boolean is_first_burst,
                        gas_id_t gas_id  )
{
   GfwX2GStartupCmd  x2gStartupCmd;

#ifdef FEATURE_GSM_COEX_FW_CXM

   cxm_channel_in_type  cxm_channel_in  = {0};
   cxm_channel_out_type cxm_channel_out = {0};
   cxm_channel_in.rf_device = rf_device;

   if( is_a_qta_gap )
   {
     cxm_channel_in.techid    = geran_map_gas_id_to_cxm_id( gas_id );
     cxm_channel_in.rf_device = rf_device;
     cxm_channel_out          = cxm_get_channel_id( cxm_channel_in );
     MSG_GERAN_HIGH_1_G( "MCS channel ID %d",  cxm_channel_out.rx_channel_id);

     x2gStartupCmd.actPriority   = coex_params.activity_priority;
     x2gStartupCmd.noActPriority = coex_params.no_activity_priority;
     x2gStartupCmd.desenseId     = coex_params.desense_id;

     MSG_GERAN_HIGH_3_G("CXM : cxm actpriority=%d noAct=%d desenseid=%d", x2gStartupCmd.actPriority,x2gStartupCmd.noActPriority, x2gStartupCmd.desenseId);
     x2gStartupCmd.isTuneAway = QTA_CM_CxM;
     x2gStartupCmd.channel_id0 = cxm_channel_out.rx_channel_id;
#ifdef FEATURE_GSM_RFLM_FED
     if(gl1_hw_get_if_start_gsm_qta_sent(gas_id))
     {
        MSG_GERAN_HIGH_0_G("Read RFAlpha set TRUE");    
        gl1_hw_set_if_start_gsm_qta_sent(FALSE, gas_id);
        x2gStartupCmd.readRfAlpha = TRUE;
     }
     else
#endif
     {
        x2gStartupCmd.readRfAlpha = FALSE;
     }        
   }
   else
   {
     cxm_channel_in.techid    = geran_map_gas_id_to_cxm_id( gas_id ); 
     cxm_channel_in.rf_device = rf_device;
     cxm_channel_out          = cxm_get_channel_id( cxm_channel_in );
     MSG_GERAN_HIGH_1_G( "MCS channel ID %d",  cxm_channel_out.rx_channel_id);

     x2gStartupCmd.actPriority = 0;
     x2gStartupCmd.noActPriority = 0;
     x2gStartupCmd.isTuneAway = NO_TUNE_AWAY;
#ifdef FEATURE_GSM_COEX_FW_CXM
     x2gStartupCmd.channel_id0 = cxm_channel_out.rx_channel_id;
     MSG_GERAN_HIGH_1_G("X2G IRAT channel id = %d", x2gStartupCmd.channel_id0);
#else /* FEATURE_GSM_COEX_FW_CXM */
     x2gStartupCmd.channel_id0 = 0;
#endif /* FEATURE_GSM_COEX_FW_CXM */
     x2gStartupCmd.readRfAlpha = FALSE;
   }

#else
   //Ensure that isTuneAway is set
     x2gStartupCmd.isTuneAway = NO_TUNE_AWAY;
     MSG_GERAN_HIGH_0_G("isTuneAway set to NO_TUNE_AWAY");
#endif   

   /* The offset must be larger than the FTSM, try and recover but can ignore for QTA */
   if ( ( offset < mdsp_ftsm(gas_id) ) && !gl1_hw_qta_gap_active(gas_id) )
      {
      MSG_GERAN_ERROR_1_G("Invalid cm_startup offset %d", offset);
      offset = mdsp_ftsm(gas_id);
   }

   x2gStartupCmd.cmd         = GFW_X2G_STARTUP_CMD;
   x2gStartupCmd.time_offset = offset;
   x2gStartupCmd.firstBurstFlag = is_first_burst;

   x2gStartupCmd.rfBufStartIndex = buffer_index;
   x2gStartupCmd.rxlmBufIndex = rxlm_buf_idx;
   if (rxlm_buf_idx == MDSP_RXLM_BUF_IDX_INVALID)
   {
      x2gStartupCmd.rfTune2g = FALSE;
   }
   else
   {
      x2gStartupCmd.rfTune2g = TRUE;
   }

   x2gStartupCmd.deviceID = tqp_device;

   mdsp_issue_qdsp6_command( (GfwCmdBufType *) &x2gStartupCmd, GFW_X2G_STARTUP_CMD , gas_id );
 return mdsp_getcmd_buf_address(gas_id);
}


/*===========================================================================

FUNCTION mdsp_cm_cleanup

DESCRIPTION
  This function constructs and issues the cm cleanup command to the mdsp.

  offset - number of quarter symbols from end of last burst

DEPENDENCIES
  mdsp image must already be loaded.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void * mdsp_cm_cleanup( uint16   offset,
                      uint8   buffer_index,
                      boolean  rf_tuneback,
                      uint32 rxlm_buf_idx
                      , gas_id_t gas_id
                    )
{
  GfwX2GCleanupCmd   x2gCleanupCmd;

   /* The offset must be larger than the FTSM can ignore for QTA */
   if ( ( offset < FRAME_TICK_SETUP_MARGIN_QS ) && !gl1_hw_qta_gap_active(gas_id) )
   {
#ifdef FEATURE_GSM_TDS
      if (!gl1_hw_cm_read_t2g_active(gas_id))
      {
        MSG_GERAN_FATAL_1_G("Invalid cm_cleanup offset %d", offset);
        offset = FRAME_TICK_SETUP_MARGIN_QS;
      }
      else
      {
        MSG_GERAN_HIGH_1_G("cm_cleanup offset %d ", offset);
      }
#else
      MSG_GERAN_FATAL_1_G("Invalid cm_cleanup offset %d", offset);
      offset = FRAME_TICK_SETUP_MARGIN_QS;
#endif
   }

   x2gCleanupCmd.cmd         = GFW_X2G_CLEANUP_CMD;
   x2gCleanupCmd.time_offset = offset;

#if defined(FEATURE_LTE)  || defined(FEATURE_GSM_TDS) || defined(FEATURE_QTA)
   x2gCleanupCmd.rfTuneback  = rf_tuneback;
#else
   x2gCleanupCmd.rfTuneback  = TRUE;
#endif


   x2gCleanupCmd.rfBufStartIndex = buffer_index;
   x2gCleanupCmd.rxlmBufIndex = rxlm_buf_idx;

   mdsp_issue_qdsp6_command( ( GfwCmdBufType *) &x2gCleanupCmd, GFW_X2G_CLEANUP_CMD , gas_id );

 return mdsp_getcmd_buf_address(gas_id);
}

/*===========================================================================

FUNCTION mdsp_set_mdsp_compressed_mode

DESCRIPTION
  This function sets mdsp_compressed_mode flag true.

 ===========================================================================*/
void mdsp_set_mdsp_compressed_mode ( gas_id_t gas_id )
{
  mdsp_cm_private_data_t*  private_data_ptr   = get_private_dataspace_ptr( gas_id);

  /* Tell mdsp_intf.c that compressed mode is running */
  private_data_ptr->mdsp_compressed_mode = TRUE;
}

/*===========================================================================

FUNCTION mdsp_cm_init

DESCRIPTION
  This function initializes data structures to enable communications with
  the mdsp.


DEPENDENCIES
  mdsp image must already be loaded.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean gl1_hw_force_set_app_mode_cmd;
void mdsp_cm_init (  gas_id_t gas_id  )
{
  init_private_data_space(gas_id);

  /* Ensure mdsp panic cb is NULL during CM activity */
  mdsp_gsm_register_panic_cb( NULL , gas_id );

  /* The frame tick setup margin required by the mdsp is handled by the wcdma L1 task */
  mdsp_set_ftsm_geran(0 , gas_id);

  /* For CM the sync rx offset is calculated by WL1 */
  mdsp_set_sync_rx_time_offset(0 , gas_id);

  /* Ensure the driver is awake */
  mdsp_gsm_drv_wakeup();

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  MSG_GERAN_HIGH_1_G("gl1_hw_force_set_app_mode_cmd:%d",gl1_hw_force_set_app_mode_cmd);

  if( gl1_hw_force_set_app_mode_cmd )
  {
    (void)gl1_get_gfw_app_mode(gas_id);
     gl1_set_gfw_app_mode_status( GFW_APP_MODE_IDLE, gas_id );
     gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_MEASUREMENT, geran_map_gas_id_to_nas_id(gas_id));

     gl1_hw_force_set_app_mode_cmd = FALSE;
  }
  else
  {
       gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_MEASUREMENT, geran_map_gas_id_to_nas_id(gas_id));
  }
#else
  gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_MEASUREMENT, geran_map_gas_id_to_nas_id(gas_id));
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}

/*===========================================================================

FUNCTION mdsp_cm_inact

DESCRIPTION
  This function shuts-down the CM mDSP driver.

DEPENDENCIES
  mdsp image must already be loaded.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_cm_inact ( boolean shutdown_fw, gas_id_t gas_id )
{
   mdsp_cm_private_data_t*  private_data_ptr   = get_private_dataspace_ptr( gas_id);

   if (shutdown_fw)
   {
     mdsp_disable( gas_id);
   }
   else
   {
     MSG_GERAN_MED_0_G("GERAN FW not shutdown");
   }

   /* Turn off compressed mode */
   private_data_ptr->mdsp_compressed_mode = FALSE;

   /* Set the sync rx offset back to the default */
   mdsp_set_sync_rx_time_offset(MDSP_DEFAULT_RX_TIME_OFFSET , gas_id);
}



/*===========================================================================

FUNCTION mdsp_build_fw_setup_buffer

DESCRIPTION
  This function is used for CM_POWER_MEASURE.  Operation is identical to
  building for nonsbi burst buffers. The difference is the data is loaded
  into a seperate buffer.  The initial intended use of this buffer is for
  position information about the monitor burst.

  Typical usage of this function is to use
    id = FW_CFG_proc_burst_posn_ctrl
    data = position of burst relative to gap start
    burst = designates which one of the monitor bursts

  id  - mdsp logical id to write to
  data - data to write - 16 bits
  burst - which of 9 burst buffers does this belong to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_build_fw_burst_buffer (uint16 id, uint16 data, mdsp_burst_type burst, gas_id_t gas_id)
{
}

/*===========================================================================

FUNCTION  mdsp_async_intf_send_immediate_cmd

DESCRIPTION
  Send MSGR message to GFW Task to run command processing immediately and execute the commands for the current frame.
  Pass FN to which the offsets in the commands are referenced

===========================================================================*/
void mdsp_async_intf_send_immediate_cmd(uint32  cmd_fn, boolean rf_activity_cmds ,gas_id_t gas_id)
{
  GfwHostAsyncCmd gfwHostAsyncCmd;
  uint32 hw_fn;
  uint32 sw_fn = GSTMR_GET_FN_GERAN(gas_id);
  mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

  mdsp_issue_generic_config_command(gas_id);

  /* initialise msgr header */
  msgr_init_hdr(&gfwHostAsyncCmd.hdr,MSGR_GERAN_GL1,GERAN_FW_ASYNC_CMD);
  /* populate payload */
  gfwHostAsyncCmd.swFrameNo   = sw_fn;

  hw_fn = get_vstmr_FN_modMaxFN(geran_map_gas_id_to_nas_id(gas_id));
  if (cmd_fn == sw_fn)
  {
     /* As offsets realtive to this frame rather than next, decrement hw_fn */
     hw_fn = (hw_fn+(GERAN_VSTMR_MAX_FN-1)) & (GERAN_VSTMR_MAX_FN-1);
  }

  gfwHostAsyncCmd.hwFrameNo   = hw_fn;
  gfwHostAsyncCmd.cmdSeqNum   = mdsp_get_current_seq_num( gas_id );
  gfwHostAsyncCmd.cmdBuffIdx  = mdsp_current_buffer( gas_id );

  /* Set sequence number and num_cmds */
  mdsp_cmd_buffer_header(gas_id);
  /* send msgr message to GFW */
  (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_ASYNC_CMD, &gfwHostAsyncCmd, NULL, gas_id);

  /* toggle command and RF buffers now that FW have picked up previous commands*/
  mdsp_switch_command_buffers(geran_map_gas_id_to_nas_id(gas_id) );

  if (rf_activity_cmds == TRUE)
  {
     mdsp_inc_current_buffer_mod3 (gas_id);
     mdsp_intf_data_ptr->mdsp_async_imm_cmd_sent = TRUE;
  }

  return;
}

/*===========================================================================

FUNCTION  mdsp_set_async_imm_cmd_sent

DESCRIPTION
  Sets state of mdsp_async_imm_cmd_sent to be TRUE or FALSE

===========================================================================*/
void mdsp_set_async_imm_cmd_sent(boolean async_imm_cmd_sent, gas_id_t gas_id)
{
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   mdsp_intf_data_ptr->mdsp_async_imm_cmd_sent = async_imm_cmd_sent;

   return;
}

boolean mdsp_cm_active ( gas_id_t gas_id )
{
   mdsp_cm_private_data_t*  private_data_ptr   = get_private_dataspace_ptr( gas_id);

   /* Report compressed mode */
   return private_data_ptr->mdsp_compressed_mode;
}

