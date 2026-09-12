/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 M D S P   G P R S   I N T E R F A C E
                    mDSP GPRS Interface Module

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmdsp6/src/mdsp_gprs.c#2 $
$DateTime: 2019/11/27 05:34:59 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ------- ------------------------------------------------------ 
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
08/04/16      og     CR993589. Stack usage reduction.
21/04/17      br     CR1041084 FR 36219: GDET – GMSK Modulation Detector
04/11/15      ab     CR919479 : EDGE IR memory should not be allocated during IR RELEASE(MOD13 update).
07/09/15      npt    CR898493 GL1 NV control of T2G acquisition GFW error recovery
24/08/15      mc     CR874662 : CXM Freq ID support for X2G startup and X2G preload
01/06/15      ab     CR846442 : GL1 issue GFW_CONFIGURE_IR_WINDOW_CMD in All states.
20/08/15      npt    CR878988 GL1 NV control of GFW recovery features
                     GFW behaviour modified GFW CR:836872
29/05/15      pa     CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD. 
13/05/15      npt    CR837344 Change f3 logging for RF timeline error recovery feature 
29/04/15      npt    CR804870 RF timeline error recovery mechanism
17/04/15      ak     CR813711 Debug code for SCE power monitors
16/04/15      ak     CR820145 Problems with empty BA list during ded mode and with HMSC tests
07/04/15      og     CR814507. Further stack reduction in the GSM L1 ISR.
27/02/15      cja    CR768169 Add RF Seq Number
18/02/15      am     CR775652 Mismatch in monitor reading
09/02/15      jj     CR 793229 making debug f3 for crash in the middle of the burst under NV control 
17/11/14      jj     CR 752202 GL1 - bolt - Debug support for crashing in the 
                      middle of rx burst for a given range of rssi value
28/11/14      jj     CR 764201 power measurement ready mask change
23/09/14      cja    CR728667 Fix compile error in promotion of CR725605
22/09/14      cja    CR725605 Add GFW command for configure IR window release
09/09/14      jk     CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14      nm     CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
08/08/14      mc     CR703283 Insane no-zone calculation triggers gfw_burst_scheduler.cc:801
                     Assertion (0 <= burstRxTxInfo.lastRx && burstRxTxInfo.lastRx <= TS7) failed
06/08/14      df     CR536059 Allow Asynchronous sleep for all idle activities
01/08/14      pjr    CR703206 Revert CR700772
28/07/14      pjr    CR700772 Changed mdsp_notify_power_mon_info to support two no_zones
25/07/14      pjr    CR700017 do not send GFW_PWR_MSR_DSP_SCHEDULED to fw until monitor placement
                     is fixed
22/05/14      cah    Bolt 2.0: FEATURE_GERAN_DRDSDS: Scell measurement changes
28/05/14      cja    CR657368 Fix Klockwork issue of NULL data space pointer and RSSI array size
19/06/14      ws     DATA+MMS feature - command FW to schedule monitors if GL1 can't find a space
14/11/13      js     CR549415 - Do not do reselection meas on yielded arfcns
28/11/13      cja    CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13      cja    CR455527 Remove featurisation from files in API directory
14/10/13      pa     CR539505:Bolt: Added support for Dynamic Power Scheduling
17/09/13      pjr    CR538893 Changes to enable CXM logging
21/08/13      npt    CR531633 - Remove FEATURE_DIME_MODEM dependencies 
15/08/13      sk     CR524039 GBTA bringup changes
29/07/13      sk     CR519663 Partial QBTA bringup changes
21/06/13      cs     Major Triton TSTS Syncup
16/05/13      ab     CR487396 - Support EGPRS IR Reduced Window in GFW
09/05/13      cs     TSTS Updates
19/02/13      mc     CR453970 Hard coding of FW interface COEX parameters for Rx, Tx and Monitors
06/02/13      jk     CR449735: Resolving KW errors
05/12/12      pg     CR427361: GFW/RF shared memory interface cleanup
29/06/12      jj     CR374950 GL1 changes for 38 symbol HMSC monitor
25/05/12      pg     Add support for RF Task
30/01/12      cja    CR333023 Clear pending flags when command issued
28/01/12      cja    CR333023 Send IR configure command from ISR not task
12/01/12      jj     CR329722 add indication for  scheduleTimeError
07/11/11      jj     315253 IQ capture capability for GSM via QRCT
31/10/11      jj     CR315402 fix for interference measurement crash on HMSC
08/09/11      jj     CR302378  Added GL1 changes for RSB along with spectral inversion
20/04/11      cja    Nikel GFW interface change
08/02/11      cja    Initial changes for Nikel
07/10/10      ky     254466:Removed fw unused shared variables from the sw code
19/07/10      ky     Klocwork fixes
13/07/10      ky     Fixed 9k warnings
17/03/10      ws     Added featurisation to seperate Data mover code from FEATURE_GSM_MDSP_IR
11/02/10      ky     Correct pdchVsH array (uint32 instead of uint16 array) is passed
                     to mdsp_update_shared_varilable function to send this array correctly to fw
23/12/09      ws     Added volatile type for accessing pMdspHostIfBaseStore
30/09/09      ws     Renamed mdsp_issue_command2 to mdsp_issue_qdsp6_command
23/09/09      ws     Removed setting of external memory address for QDSP6
                     target since data mover is not required
05/08/09      ws     Removed FEATURE_GSM_MDSP_DTM it's now mainlined
23/07/09      ws     Added FEATURE_GSM_GPRS_QDSP6 to remove QDSP4 references as part
                     of GERAN DSP migration to QDSP6
06/06/09      ab     Feature FEATURE_GSM_EGPRS_IR_OPT for LCU
09/10/08      cs     Allow the RX Sweep to schedule shorter monitor duration
29/07/08      ws     mdsp Stucture initialisations to resolve Klockwork warnings
19/12/07      cs     Fix the previous checkin for builds that don't have DM fix
13/12/07      agv    CR 131982 Fix for EGPRS DM causing sleep nacks.
19/07/07      og     UOOS changes, reduce number of measured samples.CR119147.
30/03/07      og     Prevent local pdch array over index when power measurements
                     are higher than 4. Resolves CR115683.
05/18/06      gfr    Support for clock regime resource manager
11/07/05      gfr    Minor cleanup for QLint
10/11/05      gfr    Added mdsp_dm_pdtch_decode_done to sanity check the counter
09/30/05      gfr    Allow ARM HALT during IR since HW issue has been resolved
09/30/05      gfr    Move ACI detection control to mDSP driver
09/23/05      gfr    Optimize DM active period to just during PDTCH decoding
09/23/05      gfr    Removed power measure schedule shift (offset 4999) since
                     we now suppress that warning anyway.
08/30/05      gfr    Fixed avoid scheduling power measures that end on offset
                     4999 code to deal with the channel filter stop delta
08/26/05      gfr    Update power measure interface, sanity check power
                     measure results
08/19/05      gfr    Use mdsp_burst_current_buffer
08/17/05      gfr    QCLint cleanup - shorten lines to less than 90 chars
08/05/05      gfr    ACI detection is now a bit in the command word.
07/29/05      gfr    DTM support
07/23/05      gfr    Inform clkregime of DM active state
07/05/05      gfr    Avoid scheduling power measures that end on offset 4999
06/10/05      gfr    Remove unused power measure code
05/19/05      gfr    Prevent ARM halt when IR is on and the DM active
                     interrupt interface is not in use.
05/18/05      gfr    Rename mdsp_pdch_id to mdsp_timeslot_type
05/13/05      gfr    Replace INC_MODx macros with INC_MOD(,x)
05/03/05      gfr    Mainline FEATURE_GSM_MDSP_EER
04/26/05      gfr    Set the metrics length to zero for invalid bursts
04/08/05      gfr    Allow caller to specify whether to discard untxed data
03/29/05      gfr    Always send up data, even if the decode fails.
03/22/05      gfr    Use tx tag to keep track of encoded data.
03/11/05      gfr    Lint cleanup
03/07/05      gfr    Add mdsp_gprs_IR_set_tfi
03/02/05      gfr    Do not overwrite PS when changing from USF to PDCH
02/11/05      gfr    Reduce shared memory required for PTCCH bursts
02/10/05      gfr    Minor IR updates
01/31/05      gfr    Finalized support for incremental redundancy
01/25/05      gfr    Use PA transition profile provided by RF driver
01/24/05      gfr    General cleanup and restructure.
01/11/05      yhong  Lint
12/16/04      gfr/kt Support for ACI detection and transition ramp for polar RF
12/14/04      gfr    Always reload the PA profile as a temporary workaround
                     for a firmware bug.
12/09/04      yhong  Log GPRS TX into a trace buffer
12/07/04      yhong  Lint cleanup
12/06/04      yhong  Support for FEATURE_GSM_MDSP_EGPRS
11/30/04      gfr    Support for polar RF and EER programming.
08/27/04      gfr    Lint cleanup.
08/26/04      gfr    Added first_burst parameter to rx command.
07/22/04      gfr    EGPRS support.
06/13/04      gfr    Print an error message if data is not available.
06/10/04      gfr    Use nice defines for control fields.
04/22/04      gfr    Fixed resetting of sequence numbers.
04/09/04      gfr    Use current_buffer_mod3 for the debug buffer seq_num
03/05/04      gfr    Lint cleanup.
02/17/04      gfr    GSM rotators support.
01/16/04      gfr    Support for mdsp results validation.
01/12/04      gfr    Support for 6 monitors per frame.
08/04/03      jc     More Lint cleanup with array sizes.
06/21/03      jc     Lint cleanup.
06/17/03      gfr    The power measure offset is now to the start of the burst.
                     Added frame numbers to the error messages.
06/03/03      gfr    Clean up driver if rx message is never available.
05/21/03      gfr    Take offsets instead of timeslots for scheduling power
                     measures.
05/13/03      gfr    Common power handler support.
05/12/03      gfr    Host scheduling of all power measures.
05/12/03      gfr    Added tx_alpha parameter to transmit command.
05/08/03      gfr    Reset the saved pa profile table when initializing.
04/02/03      jc     Mainline RF_MULTIMODE
03/18/03      gfr    Clear decoded usf buffer on rx schedule.
02/26/03      gfr    More lint cleanup.
02/04/03     jc/ma   Lint cleanup.
11/06/02      gr     Code cleanup.  Removed usf fifo.
10/10/02      gr     Support for ping-pong debug buffer.
09/24/02      gs     Supports USF Granularity on target
09/13/02      gr     Added support for aborting dynamic PRACH
09/11/02      sd     Added support for RF driver multimode API that includes
                     GSM drivers.
09/06/02      gr     New uplink power control interface.
09/05/02      gr     Modifed get_decoded_data to gracefully handle case
                     when data is not actually available yet.
09/05/02      bk     Added support to reset DL seq nums
09/04/02      gr     Moved FEATURE_GSM_GPRS flag to include #includes
08/22/02      bk     Fixed wrong display of USF seq. number mismatch message
                     and fixed bug to handle the case when USF is not decoded
                     before a PTCCH transmit.
06/05/02      gw     Added third debug buffer.  Some general clean up.
06/04/02      bbd    Changed FEATURE_GPRS to FEATURE_GSM_GPRS
05/02/02      gw     Added facility to log function calls.
04/25/02      ih     Implemented dynamic MDSP buffer allocation. Moved
                     statically defined arrays into mdsp_gprs_init.
04/16/02      bk     Added extra word to pwr meas header. Fixed bug in tx_burst
                     to handle multiple PDCH specific transmits.
04/11/02     sg/jc   Changes to make dynamic allocation work.
03/22/02      gw     Changed references to mdsp_power_metrics_struct to
                     mdsp_gprs_power_metrics_struct.
03/14/02     bk/gw   Changes to GPRS power measure command.
02/14/02      jc     Add support for power measure cmd.  Fix circular buffers
                     that are 3 in length (cannot run ctr to 255 then wrap).
02/04/02      jc     Various fixes to get gprs tx to work.
12/12/01      jc     Corrected _get_decoded_data() to use proper offset val.
11/28/01      jc     Fixed sync_rx cmd, Also use 3 buffers not 2 for metrics
                     and burst scheduler results. In mdsp_gprs_get_decoded_data,
                     retrieve data in pdch order, no longer needing to buffer
                     up the data.
10/25/01      jc     Initial version.
==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

#ifdef FEATURE_DMOV
#include "dmov.h"
#endif


#include "comdef.h"
#include "msg.h"
#include "err.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_cmd.h"
#include "rfm.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_hw_g.h"
#include "gfw_sw_intf.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"

/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/


/* Define some shortcuts to increment a circular buffer index */
#define INC_MOD(x,m) { if (++(x) >= m) x = 0;  }


#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)

/* DM active frame counter, if we exceed this generate an error */
#define MDSP_MAX_DM_ACTIVE_FRAMES  20
#endif /* (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)*/

/* Burst types used for the up to seven power measures */
const mdsp_burst_type mdsp_gprs_mon_metrics_burst_types[] =
{
   MON_BURST_1,
   MON_BURST_2,
   MON_BURST_3,
   MON_BURST_4,
   MON_BURST_5,
   MON_BURST_6,
   MON_BURST_7 /* Added this to enable 7 pwr monitors per frame */
};


/* Lookup table for message length in words */
const uint8 mdsp_cs_to_length_lut[] =
{
   MDSP_CS1_WORDS,
   MDSP_CS2_WORDS,
   MDSP_CS3_WORDS,
   MDSP_CS4_WORDS,
#ifdef FEATURE_GSM_MDSP_EGPRS
   MDSP_MCS1_WORDS,
   MDSP_MCS2_WORDS,
   MDSP_MCS3_WORDS,
   MDSP_MCS4_WORDS,
   MDSP_MCS5_WORDS,
   MDSP_MCS6_WORDS,
   MDSP_MCS7_WORDS,
   MDSP_MCS8_WORDS,
   MDSP_MCS9_WORDS
#endif
};

typedef struct
{
uint16  set_starting_bsn;
uint16  set_ending_bsn;
uint16  clear_starting_bsn;
uint16  clear_ending_bsn;
boolean set_memory;
boolean clear_memory;
} mdsp_pending_IR_config_type;

/*===========================================================================
  MULTI-DATASPACED PRIVATE DATA
===========================================================================*/

typedef struct
{
 /* Variables associated with gprs_sync_rx command
  *
  * mdsp_sync_rx_buf_wr_index: used when issuing the command into 1 of 2 buffers
  * mdsp_sync_rx_buf_wr_num_pdch: indicates how many pdch are associated with
  *   that buffer
  * mdsp_sync_rx_buf_rd_index: indicates which buffer to read from
  * mdsp_sync_rx_buf_rd_num_pdch: indicates how many pdchs have been read from
  *   that buffer
  */

  uint8   mdsp_gprs_power_measure_burst_wr_index;
  uint8   mdsp_gprs_power_measure_burst_rd_index;
  uint8   mdsp_gprs_power_measure_burst_seq_wr_index;
  uint8   mdsp_gprs_power_measure_burst_seq_rd_index;

  /* 4 power_measure commands */
  /* Used for burst by burst comparison - results are read 3 frames later, but
     we cannot guarantee the order so we need an extra value */
  uint16  mdsp_gprs_power_measure_burst_seq_num[4];

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)

  /* Variables to handle activating and inactivating DM */
  boolean mdsp_dm_active;
  boolean mdsp_dm_required;
  uint16  mdsp_dm_active_frames;
  int16   mdsp_dm_frames_to_inactive;
#endif /* (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)*/

  boolean mdsp_ir_release;
  mdsp_pending_IR_config_type Mdsp_pending_IR_config;

} mdsp_gprs_private_data_t;

#ifdef FEATURE_DUAL_DATA   

typedef struct
{
  boolean mdsp_scheduled_pwr_mon;
  uint16 allowed_zone_start[2];
  uint16 allowed_zone_stop[2];
}  mdsp_pwr_mon_info_t;

#endif
/*===========================================================================
  PUBLIC AND PRIVATE DATASPACES
===========================================================================*/

mdsp_gprs_public_data_t mdsp_gprs_public_data[NUM_GERAN_DATA_SPACES];
static mdsp_gprs_private_data_t mdsp_gprs_private_data[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_DUAL_DATA   
static mdsp_pwr_mon_info_t mdsp_pwr_mon_info[NUM_GERAN_DATA_SPACES];
#endif

#ifdef FEATURE_GSM_SCE_PWR_MEAS_DBG
#define L1_SCE_DBG_MDSP_GPRS_LOG_IDX_MAX 49
static l1_sce_dbg_mdsp_gprs_T l1_sce_dbg_mdsp_gprs[NUM_GERAN_DATA_SPACES][L1_SCE_DBG_MDSP_GPRS_LOG_IDX_MAX+1];
static uint8 l1_sce_dbg_mdsp_gprs_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( L1_SCE_DBG_MDSP_GPRS_LOG_IDX_MAX ) };
#endif

/*===========================================================================

                             LOCAL MACROS

===========================================================================*/

/* Macro to set the power measure command control field values */
#define MDSP_PWR_MEAS_CTL_FIELD(field,value) \
   (uint16)(((value) << gsmPwrMsrCmd##field##_POS) \
     & gsmPwrMsrCmd##field##_MASK)

/* Some defines for sanity checking the results of a power measure, at
   a minium there should be the length, sequence number and timeslots
   fields, and the max is obvious - size in words */
#define PWR_MEAS_RESULTS_HDR_SIZE  3

/* Use this to debug EDGE DM sleep nacking */
/* #define MDSP_DM_DEBUG */

/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_GSM_SCE_PWR_MEAS_DBG
static void l1_sce_dbg_mdsp_gprs_send_pwr_meas_cmd_to_fw(
   uint16             num,             /* number of measurements */
   uint16             offsets[],       /* which offsets */
   ARFCN_T            *arfcns,
   gas_id_t gas_id);

static void l1_sce_dbg_mdsp_gprs_get_pwr_meas_res_from_fw(
   boolean last_attempt,
   uint32 yield_mask,
   gas_id_t gas_id);


#define L1_SCE_DBG_MDSP_GPRS_SEND_PWR_MEAS_CMD_TO_FW(num, offsets, arfcns, gas_id) l1_sce_dbg_mdsp_gprs_send_pwr_meas_cmd_to_fw( num, offsets, arfcns, gas_id)
#define L1_SCE_DBG_MDSP_GPRS_GET_PWR_MEAS_RES_FROM_FW(last_attempt, yield_mask, gas_id) l1_sce_dbg_mdsp_gprs_get_pwr_meas_res_from_fw(last_attempt, yield_mask, gas_id)

#else /* FEATURE_GSM_SCE_PWR_MEAS_DBG */

#define L1_SCE_DBG_MDSP_GPRS_SEND_PWR_MEAS_CMD_TO_FW(num, offsets, arfcns, gas_id)
#define L1_SCE_DBG_MDSP_GPRS_GET_PWR_MEAS_RES_FROM_FW(last_attempt, yield_mask, gas_id)

#endif /* FEATURE_GSM_SCE_PWR_MEAS_DBG */


/*===========================================================================

                                FUNCTIONS

===========================================================================*/
/*===========================================================================

FUNCTION get_public_dataspace_ptr

DESCRIPTION
  This function gets the public data space for a specified GAS ID

===========================================================================*/
mdsp_gprs_public_data_t *get_mdsp_gprs_public_dataspace_ptr(const gas_id_t gas_id )
{
  mdsp_gprs_public_data_t *data_ptr;

  data_ptr =
      &(mdsp_gprs_public_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(check_gas_id(gas_id))]);

  return ( data_ptr );
}

/*===========================================================================

FUNCTION get_private_dataspace_ptr

DESCRIPTION
  This function gets the private data space for a specified GAS ID

===========================================================================*/
static mdsp_gprs_private_data_t *get_private_dataspace_ptr(const gas_id_t gas_id )
{
  mdsp_gprs_private_data_t *data_ptr;

  data_ptr =
      &(mdsp_gprs_private_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(check_gas_id(gas_id))]);

  return ( data_ptr );
}

/*===========================================================================

FUNCTION init_private_data_space

DESCRIPTION
  This function initialises a public data space

===========================================================================*/
static void init_private_data_space( gas_id_t gas_id )
{
  mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);
  private_data_ptr->mdsp_ir_release = FALSE;

  private_data_ptr->Mdsp_pending_IR_config.set_starting_bsn   = 0;
  private_data_ptr->Mdsp_pending_IR_config.set_ending_bsn     = 0;
  private_data_ptr->Mdsp_pending_IR_config.clear_starting_bsn = 0;
  private_data_ptr->Mdsp_pending_IR_config.clear_ending_bsn   = 0;
  private_data_ptr->Mdsp_pending_IR_config.set_memory         = FALSE;
  private_data_ptr->Mdsp_pending_IR_config.clear_memory       = FALSE;
}


/*===========================================================================
FUNCTION mdsp_gprs_set_usf_list

DESCRIPTION
  This function fills a USF list table that the mdsp uses for TX commands.
  The mdsp reads this list every TDMA frame where an encode flag is set.
  This function may be called in any frame in which the encode flag is not
  set. The USF list entries are formatted as :

    bit 15 - TRUE is USF is assigned, FALSE is not
    bit 14- 3 - Reserved
    bit 2 - 0 - USF value

   15___________0
   | pdch 0     |
   | pdch 1     |
   | pdch 2     |
   | pdch 3     |
   | pdch 4     |
   | pdch 5     |
   | pdch 6     |
   | pdch 7     |
    _____________

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_gprs_set_usf_list ( uint16  list[8] , gas_id_t gas_id  )
{
   uint16 index = 0;

   mdsp_gprs_public_data_t* mdsp_gprs_public_data_ptr = get_mdsp_gprs_public_dataspace_ptr( gas_id);

   for(index=0;index<8;index++)
   {
     mdsp_gprs_public_data_ptr->mdsp_usf_list[index] = list[index];
   }
}

/*===========================================================================

FUNCTION mdsp_gprs_init

DESCRIPTION
  Performs initialization of static variables for the gprs functions.  Note
  that several pointers are duplicates of mdsp_intf.c but are duplicated here
  rather than make them global variables.

===========================================================================*/
void mdsp_gprs_init(  gas_id_t gas_id  )
{
   mdsp_gprs_public_data_t* public_data_ptr = get_mdsp_gprs_public_dataspace_ptr( gas_id);

   init_private_data_space(gas_id);

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
/* Set the base address for incremental redundancy */

   /* Init up the data mover */
   mdsp_init_dm();

   /* Init local DM variables just in case things got wacked */
   public_data_ptr->mdsp_dm_active = FALSE;
   public_data_ptr->mdsp_dm_required = FALSE;
   public_data_ptr->mdsp_dm_pdtch_decodes = 0;
   public_data_ptr->mdsp_dm_frames_to_inactive = 0;
#endif

#ifdef FEATURE_GSM_MDSP_EGPRS
   /* We start out not in EGPRS mode */
   public_data_ptr->mdsp_egprs_mode = FALSE;
#endif

   /* If ACI detection is supported, turn it on by default */
   public_data_ptr->mdsp_aci_detection = TRUE;

#ifdef FEATURE_DUAL_DATA   
   memset(&mdsp_pwr_mon_info[gas_id],0,sizeof(mdsp_pwr_mon_info_t));
#endif   
}



/*===========================================================================

FUNCTION mdsp_gprs_inact

DESCRIPTION
  Shuts-down the GPRS module.

===========================================================================*/
void mdsp_gprs_inact ( gas_id_t gas_id )
{
#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
   mdsp_gprs_public_data_t* public_data_ptr = get_mdsp_gprs_public_dataspace_ptr( gas_id);
   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* Make sure that DM has been disabled - can happen if we abort and
      then immediately switch to WCDMA for example. */
   if (private_data_ptr->mdsp_dm_active)
   {
      /* Reset flags to ensure that mdsp_dm_disable does the job */
      private_data_ptr->mdsp_dm_frames_to_inactive = 0;
      public_data_ptr->mdsp_dm_pdtch_decodes = 0;
      mdsp_dm_disable( gas_id);
   }
#endif
}



/*===========================================================================

FUNCTION mdsp_gprs_process_frame_tick

DESCRIPTION
  This function performs any GPRS frame tick processing required.

===========================================================================*/
void mdsp_gprs_process_frame_tick ( gas_id_t gas_id )
{

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
   mdsp_gprs_public_data_t*  public_data_ptr  = get_mdsp_gprs_public_dataspace_ptr( gas_id);
   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* If DM is in use and not enabled and we started PDTCHD decodes we need
      to enable it now */
   if (mdsp_dm_required && !mdsp_dm_active && mdsp_dm_pdtch_decodes > 0)
   {
      #if   defined (FEATURE_DMOV)
      dmov_enable(DMOV_CLIENT_MDSP_GPRS);
      #else
      clk_regime_dm_active(
                          );
      #endif
      mdsp_dm_active = TRUE;
      mdsp_dm_frames_to_inactive = 0;
      mdsp_dm_active_frames = 0;
   }

   /* Check if it is time to flag DM as inactive */
   if (mdsp_dm_frames_to_inactive > 0)
   {
      mdsp_dm_frames_to_inactive--;
      mdsp_dm_disable( gas_id);
   }

   /* Maintain a sanity counter in case DM gets stuck on somehow */
   if (mdsp_dm_active)
   {
      mdsp_dm_active_frames++;
      if (mdsp_dm_active_frames > MDSP_MAX_DM_ACTIVE_FRAMES)
      {
         MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "DM stuck active: req=%d, dec=%d, fti=%d, FN=%d",
               mdsp_dm_required, mdsp_dm_pdtch_decodes,
               mdsp_dm_frames_to_inactive, GSTMR_GET_FN_GERAN(gas_id));

         /* If DM is not required anymore, assume we can force it off */
         if (!mdsp_dm_required)
         {
            mdsp_dm_pdtch_decodes = 0;
            mdsp_dm_frames_to_inactive = 0;
         }

         /* Try and disable in case it works now */
         mdsp_dm_disable( gas_id);
      }
   }
#endif
}


/*===========================================================================

FUNCTION mdsp_gprs_setup_power_measure

DESCRIPTION
  This function tells the mDSP to perform power measurements.
  It is called internally by the external API functions.

===========================================================================*/
static void mdsp_gprs_setup_power_measure
(
  uint16             num,             /* number of measurements */
  uint16             offsets[],       /* which offsets */
  mdsp_gprs_monitor_coex_params coex_params[], /* coex params */
  mdsp_timeslot_type pdch_id[],       /* which pdchs */
  boolean            dsp_scheduled,   /* whether dsp or host is scheduling */
  boolean            combine_adj,     /* whether adjacent slots can be combined
                                         into a single tune */
  uint16             length,          /* length of power measure */
  uint16             max_num_of_measurements,
  ARFCN_T            *arfcns,
  uint32             rf_seq_num[MAX_MEAS_PER_FRAME], /* RF Sequence Number */

  gas_id_t gas_id
)
{
   static GfwGsmPwrMsrCmd  pwrMsrCmd[3];
   uint16           i;
   uint8            wr_index;

#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
   mdsp_burst_type  burst_type;
   uint8            buf;
#endif

#ifdef FEATURE_DUAL_DATA
   GfwPwrMsrSchedlingType gfw_sched_type = GFW_PWR_MSR_HOST_SCHEDULED;
#endif

   mdsp_gprs_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t*  mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* Catch overflow. Passing max_num_of_measurements as the same function is used for GPRS */
   /* interference measurements and standard power measurements during GPRS.*/
   if (num > max_num_of_measurements)
   {
      MSG_GERAN_ERROR_2_G("Max monitors per frame exceeded %d > %d",
                num, max_num_of_measurements);
      num = max_num_of_measurements;
   }

   wr_index = private_data_ptr->mdsp_gprs_power_measure_burst_wr_index;
   INC_MOD(private_data_ptr->mdsp_gprs_power_measure_burst_wr_index,TRIPPLEBUF);

   /* Clear out Q6 command  first */
   memset( &pwrMsrCmd[gas_id],
           0,
           sizeof(GfwGsmPwrMsrCmd)
         );

#ifdef FEATURE_DUAL_DATA   
   if (mdsp_pwr_mon_info[gas_id].mdsp_scheduled_pwr_mon == TRUE )
   {
      gfw_sched_type = GFW_PWR_MSR_DSP_SCHEDULED;
      pwrMsrCmd[gas_id].allowInterval[0].start = mdsp_pwr_mon_info[gas_id].allowed_zone_start[0];
      pwrMsrCmd[gas_id].allowInterval[0].stop = mdsp_pwr_mon_info[gas_id].allowed_zone_stop[0];
      pwrMsrCmd[gas_id].allowInterval[1].start = mdsp_pwr_mon_info[gas_id].allowed_zone_start[1];
      pwrMsrCmd[gas_id].allowInterval[1].stop = mdsp_pwr_mon_info[gas_id].allowed_zone_stop[1];

      mdsp_pwr_mon_info[gas_id].mdsp_scheduled_pwr_mon = FALSE;

      MSG_GERAN_MED_4_G("DSP zone 0 start %d zone 0 end %d, DSP zone 1 start %d zone 1 end %d",
                        mdsp_pwr_mon_info[gas_id].allowed_zone_start[0],
                        mdsp_pwr_mon_info[gas_id].allowed_zone_stop[0],
                        mdsp_pwr_mon_info[gas_id].allowed_zone_start[1],
                        mdsp_pwr_mon_info[gas_id].allowed_zone_stop[1]);
   }
#endif

   /* Build the command */
   pwrMsrCmd[gas_id].cmdType = GFW_GPRS_PWR_MSR_CMD;
   pwrMsrCmd[gas_id].controlField.combine = combine_adj;
   pwrMsrCmd[gas_id].controlField.burstSize = length;
#ifdef FEATURE_DUAL_DATA   
   pwrMsrCmd[gas_id].controlField.type = gfw_sched_type;
#else
   pwrMsrCmd[gas_id].controlField.type = dsp_scheduled;
#endif

   pwrMsrCmd[gas_id].pwrMsrResultBuffIndex = wr_index;
   pwrMsrCmd[gas_id].nBursts = num;
#ifdef GL1_GMSK_MOD_DET
   pwrMsrCmd[gas_id].enableGmskDetect = TRUE;
#endif
   /* Fill in the burst information */
   for (i = 0; i < num; i++)
   {
#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
      /* Determine the burst type for the sbi/grfc buffers */
      burst_type = mdsp_gprs_mon_metrics_burst_types[i];

      /* Determine if we are using a circular or ping-pong buffer */
      buf = mdsp_burst_current_buffer(burst_type , gas_id);
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/

#ifdef FEATURE_GSM_RF_DEBUG_NV
      if ( mdsp_rf_debug_NV_enable_g [gas_id] )
      {
        pwrMsrCmd[gas_id].ctrl[i].debugCrash = get_flag_for_fw_pwr_meas_crash(i ,gas_id );
        MSG_GERAN_HIGH_1_G("pwr meas debugCrash = %d ",get_flag_for_fw_pwr_meas_crash(i ,gas_id )); 
      }
      else
      {
        pwrMsrCmd[gas_id].ctrl[i].debugCrash = FALSE;
      }
#else
      pwrMsrCmd[gas_id].ctrl[i].debugCrash = FALSE;      
#endif

      /* Fill in the command */
      /* note pdch renamed to burstno */
      pwrMsrCmd[gas_id].ctrl[i].burstNo = (uint16)pdch_id[i];
      pwrMsrCmd[gas_id].ctrl[i].offset = offsets[i];
#ifdef FEATURE_GSM_COEX
      pwrMsrCmd[gas_id].ctrl[i].arfcn = arfcns[i].num;
#endif

#ifdef FEATURE_GSM_COEX_FW_CXM
      pwrMsrCmd[gas_id].ctrl[i].cxmDesenseId = coex_params[i].coex_desense_id;
      pwrMsrCmd[gas_id].ctrl[i].cxmPriority  = coex_params[i].coex_priority;
      pwrMsrCmd[gas_id].ctrl[i].prioritySchedule  = 0; // always low priority.

#ifdef FEATURE_GERAN_DRDSDS
      pwrMsrCmd[gas_id].ctrl[i].skip = (boolean) coex_params[i].coex_skip;
#endif 

      MSG_GERAN_LOW_3_G("CXM : GMDSP monitor=%d arfcn=%d, coex_desense_id=%u",i, arfcns[i].num ,pwrMsrCmd[gas_id].ctrl[i].cxmDesenseId);
#else
      pwrMsrCmd[gas_id].ctrl[i].cxmDesenseId = 0;
      pwrMsrCmd[gas_id].ctrl[i].cxmPriority = 0;
#ifdef FEATURE_GERAN_DRDSDS
      pwrMsrCmd[gas_id].ctrl[i].skip = FALSE;
#endif
#endif

      pwrMsrCmd[gas_id].ctrl[i]. logIq = mdsp_intf_data_ptr->gsm_Iq_Log.gprs_pwr_mea_iq;
      pwrMsrCmd[gas_id].iqBufferIndex  = mdsp_current_buffer( gas_id);
#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
      if(buf <= MDSP_MAX_CURRENT_BUFFER_INDEX)
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
      {
        pwrMsrCmd[gas_id].ctrl[i].rfBufIndex      = mdsp_current_buffer_mod3( gas_id);
      }

#ifdef GERAN_L1_HLLL_LNASTATE
      pwrMsrCmd[gas_id].ctrl[i].doJdetReading = FALSE;
      if(gl1_hw_get_sawless_HLLL_support(arfcns[i].band,gas_id))
      {
      pwrMsrCmd[gas_id].ctrl[i].doJdetReading = TRUE;
      }
#endif /*GERAN_L1_HLLL_LNASTATE*/
      pwrMsrCmd[gas_id].ctrl[i].rfSeqNr = rf_seq_num[i];
   }

   mdsp_issue_qdsp6_command( (GfwCmdBufType *)&pwrMsrCmd[gas_id],
                              GFW_GPRS_PWR_MSR_CMD,
                              gas_id
                           );

   /* retrieve the seq num for checking later */
   wr_index = private_data_ptr->mdsp_gprs_power_measure_burst_seq_wr_index;
   INC_MOD(private_data_ptr->mdsp_gprs_power_measure_burst_seq_wr_index,4);
   private_data_ptr->mdsp_gprs_power_measure_burst_seq_num[wr_index] = mdsp_get_current_seq_num( gas_id);

   L1_SCE_DBG_MDSP_GPRS_SEND_PWR_MEAS_CMD_TO_FW(num, offsets, arfcns, gas_id);
}




/*===========================================================================

FUNCTION mdsp_gprs_interference_measure

DESCRIPTION
  This function tells the mDSP to perform interference type
  power measurements.
  The software specifies which pdch ids to perform the power measurement.

===========================================================================*/
void mdsp_gprs_interference_measure
(
  uint16              num,       /* number of measurements */
  mdsp_timeslot_type  pdch_id[],  /* which pdchs */
  mdsp_gprs_monitor_coex_params coex_params[],
  ARFCN_T *arfcns,
  uint32  rf_seq_num[MAX_MEAS_PER_FRAME], 
  gas_id_t gas_id
)
{
   uint16 i;
   uint16 offsets[GL1_DEFS_MAX_ASSIGNED_DL_TS];

   /* Calculate the offsets */
   for (i = 0; i < num; i++)
   {
      offsets[i] = mdsp_ftsm( gas_id) + (uint16)((uint16)pdch_id[i] * QS_PER_TS);
   }

   mdsp_gprs_setup_power_measure
    (
      num,
      offsets,
      coex_params,
      pdch_id,
      FALSE,   /* not dsp scheduled */
      TRUE,    /* combine adjacent slots into a single tune */
      MDSP_INT_MEAS_LEN,
      GL1_DEFS_MAX_ASSIGNED_DL_TS,
      arfcns,
      rf_seq_num, 
      gas_id
    );
}


#ifdef FEATURE_DUAL_DATA 
/*===========================================================================

FUNCTION mdsp_notify_power_mon_info

DESCRIPTION
  This function notifies mdsp driver about power monitor info to determine
  Allowed ranges for scheduling power monitors for DSDA where GL1 is unable
  to find a position for the power monitor

===========================================================================*/

void mdsp_notify_power_mon_info
(
   boolean data_valid,
   uint16 nozone_0_start,
   uint16 nozone_0_end,
   uint16 nozone_1_start,
   uint16 nozone_1_end,
   gas_id_t gas_id 
)
{
   if (data_valid)
   {
      mdsp_pwr_mon_info[gas_id].mdsp_scheduled_pwr_mon = TRUE;

      if ( nozone_0_start == 0 )
      {
         mdsp_pwr_mon_info[gas_id].allowed_zone_start[0] = nozone_0_end + mdsp_ftsm(gas_id);
         if ( nozone_1_start > nozone_0_end)
         {
            mdsp_pwr_mon_info[gas_id].allowed_zone_stop[0] = nozone_1_start + mdsp_ftsm(gas_id);
         }
         else
         {
            mdsp_pwr_mon_info[gas_id].allowed_zone_stop[0]  = QS_PER_FRAME+ mdsp_ftsm( gas_id);
         }
         mdsp_pwr_mon_info[gas_id].allowed_zone_start[1] = 0;
         mdsp_pwr_mon_info[gas_id].allowed_zone_stop[1]  = 0;
      }
      else
      {
         mdsp_pwr_mon_info[gas_id].allowed_zone_start[0] = mdsp_ftsm( gas_id);
         mdsp_pwr_mon_info[gas_id].allowed_zone_stop[0]  = (nozone_0_start + mdsp_ftsm( gas_id));
         mdsp_pwr_mon_info[gas_id].allowed_zone_start[1] = (nozone_0_end + mdsp_ftsm( gas_id));
         mdsp_pwr_mon_info[gas_id].allowed_zone_stop[1]  = (QS_PER_FRAME+ mdsp_ftsm( gas_id));
      }
   }
   else
   {
      mdsp_pwr_mon_info[gas_id].mdsp_scheduled_pwr_mon = FALSE;
      mdsp_pwr_mon_info[gas_id].allowed_zone_start[0]  = 0;
      mdsp_pwr_mon_info[gas_id].allowed_zone_stop[0]   = 0;
      mdsp_pwr_mon_info[gas_id].allowed_zone_start[1]  = 0;
      mdsp_pwr_mon_info[gas_id].allowed_zone_stop[1]   = 0;
   }
}

#endif /* FEATURE_DUAL_DATA  */

/*===========================================================================

FUNCTION mdsp_gprs_power_measure

DESCRIPTION
  This function tells the mDSP to perform power measurements.
  The software specifies the offsets to use.

===========================================================================*/
void mdsp_gprs_power_measure
(
  uint16       num,        /* number of measurements */
  uint16       offsets[],  /* which offsets */
  mdsp_gprs_monitor_coex_params coex_params[], /* coex params */
  boolean reduced_length,
  ARFCN_T *arfcns,
  uint32  rf_seq_num[MAX_MEAS_PER_FRAME], 
  gas_id_t gas_id
)
{
   /* Use an array size which potentially cover all TDMA timeslots
    * in a frame, prevents array over indexing problems.
    *
    * Please note this is effectively a power monitor array
    * and is not necessarily tied to pdch channels.
    */
   mdsp_timeslot_type  pdch_id[MAX_GPRS_RSSI_MEASUREMENTS];
   uint16 mon_length = MDSP_PWR_MEAS_LEN;
   uint16 i;

   /* Catch overflow */
   if (num > MAX_GPRS_RSSI_MEASUREMENTS)
   {
      MSG_GERAN_ERROR_2_G("Max monitors per frame exceeded %d > %d",
                num, MAX_GPRS_RSSI_MEASUREMENTS);
      num = MAX_GPRS_RSSI_MEASUREMENTS;
   }

   if( TRUE == reduced_length )
   {
      mon_length = MDSP_PWR_MEAS_REDUCED_LEN;
   }

   /* Calculate the PDCH ids and add the FTSM to the offset */
   for (i = 0; i < num; i++)
   {
      pdch_id[i] = (mdsp_timeslot_type)(offsets[i] / QS_PER_TS);
      offsets[i] += mdsp_ftsm( gas_id);
   }

   mdsp_gprs_setup_power_measure
    (
      num,
      offsets,
      coex_params,
      pdch_id,
      FALSE,   /* not dsp scheduled */
      FALSE,   /* no combining */
      mon_length,
      MAX_GPRS_RSSI_MEASUREMENTS,
      arfcns,
      rf_seq_num,
      gas_id
    );
}


/*===========================================================================

FUNCTION mdsp_gprs_power_measure_auto

DESCRIPTION
  This function tells the mDSP to perform power measurements.
  The mdsp will look for available timeslots to perform this action.

===========================================================================*/
void mdsp_gprs_power_measure_auto
(
  uint16  num,       /* number of measurements  */
  uint16  offset,    /* scheduling gap offset for mDSP */
  mdsp_gprs_monitor_coex_params coex_params[],
  ARFCN_T *arfcns,
  uint32  rf_seq_num[MAX_MEAS_PER_FRAME], 
  gas_id_t gas_id
)
{
   mdsp_timeslot_type pdch_id[MAX_GPRS_RSSI_MEASUREMENTS];
   uint16 offsets[MAX_GPRS_RSSI_MEASUREMENTS], i;

   /* Catch overflow */
   if (num > MAX_GPRS_RSSI_MEASUREMENTS)
   {
      MSG_GERAN_ERROR_2_G("Max monitors per frame exceeded %d > %d",
                num, MAX_GPRS_RSSI_MEASUREMENTS);
      num = MAX_GPRS_RSSI_MEASUREMENTS;
   }

   /* Clear out the offsets and pdch ids */
   for (i = 0; i < num; i++)
   {
      pdch_id[i] = MDSP_TN_0;
      offsets[i] = offset;
   }

   mdsp_gprs_setup_power_measure
    (
      num,
      offsets,
      coex_params,
      pdch_id,
      TRUE,   /* dsp scheduled */
      FALSE,  /* do not combine adjacent slots */
      MDSP_PWR_MEAS_LEN,
      MAX_GPRS_RSSI_MEASUREMENTS,
      arfcns,
      rf_seq_num,
      gas_id
    );
}

/*===========================================================================

FUNCTION mdsp_gprs_get_power_measure_results

DESCRIPTION
  This function retrieves the rssi values returned from a gprs power measure
  command.
  If the mdsp_gprs_power_measure_auto() or mdsp_gprs_power_measure()
  functions were called in frames N-1, then the results may be available in
  frame N+1, and will be available by N+2.

  rssi_values - pointer to store results - max = 4 values
  num_rssi     - pointer to store number of results
  last_attempt - whether or not to give up on this particular measure,
    should be set to TRUE in frame N+2 if subsequent power measures have
    been scheduled to keep the read index in sync.

DEPENDENCIES
  None

RETURN VALUE
  Whether data was available.  Note that even if this is TRUE num_rssi may
  be zero.

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_gprs_get_power_measure_results
(
   uint32 *num_rssi,
   uint32 *rssi_values,
   boolean * yield_status,
   boolean *ScheduleError,
   boolean last_attempt
#ifdef GERAN_L1_HLLL_LNASTATE 
   ,uint16  *jdetvalues
#endif /*GERAN_L1_HLLL_LNASTATE*/
#ifdef GL1_GMSK_MOD_DET
   ,boolean *energytest_flag
   ,uint16  *decstat
   ,uint16  *decstat2
#endif
   , gas_id_t gas_id
)
{
   uint16                         data_len;
   uint16                         i;
   boolean                        ready_mask =FALSE;
   boolean                        results_avail = FALSE, data_ready = FALSE;
   uint8                          rd_index, seq_rd_index, ScheduleErrorCount=0;
   uint32                         yield_mask = (RX_KNOCK_MON_BIT | TX_KNOCK_MON_BIT | HOST_ERROR_3 | HOST_ERROR_2);

   mdsp_gprs_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t*  mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
   gtmrs_data_t *gtmrs_data_p;
   int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   gtmrs_data_p = &gtmrs_data[as_idx];
#endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */

   /* Calculate the seq number index */
   rd_index = private_data_ptr->mdsp_gprs_power_measure_burst_rd_index;

   /* Read the size */
   data_len = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].length;

   /* Check if the results are valid */
   if (data_len > MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS  ) 
   {
      data_len = MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS;
   }

   L1_SCE_DBG_MDSP_GPRS_GET_PWR_MEAS_RES_FROM_FW(last_attempt, yield_mask, gas_id);

   for ( i=0 ;i< data_len;i++ ) 
   {
       if ( TRUE ==  mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].readyMask[i] ) 
       {
         ready_mask = TRUE;
       }
       else 
       {
         ready_mask = FALSE;
         break;
       }
   }  
   
   if (ready_mask)
   {


      if (data_len > 0)
      {

         *num_rssi = data_len ;
      }
      else
      {
         *num_rssi = 0;
      }

      /* Sanity check the length */
      if (*num_rssi == 0 || *num_rssi > MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS)
      {
         MSG_GERAN_ERROR_3_G("Invalid pwr meas results len %d, num_rssi %d, FN=%d", data_len, *num_rssi, GSTMR_GET_FN_GERAN(gas_id));
      }
      else
      {
         data_ready = TRUE;
      }

      /* Clear the length word, used as a flag */
      mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].length = 0;
   }

   /* If the data is ready then read it out */
   if (data_ready)
   {
      /* Now go and read the rest of the data */
      /* The results are only valid if the seq num in the request matches */
      seq_rd_index = private_data_ptr->mdsp_gprs_power_measure_burst_seq_rd_index;
      if (mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].seqNum != private_data_ptr->mdsp_gprs_power_measure_burst_seq_num[seq_rd_index])
      {
         uint8 recover_rd_index;

         /* Possibly, we have received results from last pwr msr cmd which was aborted */
         MSG_GERAN_ERROR_3_G("Power meas: bad seq_num %d exp %d FN=%d", mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].seqNum,
                   private_data_ptr->mdsp_gprs_power_measure_burst_seq_num[seq_rd_index],
                   GSTMR_GET_FN_GERAN(gas_id));

         /* try finding the right seq number */
         recover_rd_index = rd_index;
         INC_MOD(recover_rd_index,3);
         while(recover_rd_index != rd_index)
         {
            if(mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[recover_rd_index].seqNum == private_data_ptr->mdsp_gprs_power_measure_burst_seq_num[seq_rd_index])
            {
               /* seq num found. L1 would fetch results in the next frame */
               MSG_GERAN_MED_2_G("Power meas: seq num recovered (rd_index changed from %d to %d)",
                         rd_index,
                         recover_rd_index);

               /* Set the length back to original value */
               mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].length = data_len;

               /* expected seq number from fw is here */
               private_data_ptr->mdsp_gprs_power_measure_burst_rd_index = recover_rd_index;

               break;
            }
            INC_MOD(recover_rd_index,3);
         } /* while(recover_rd_index != rd_index) */
      }
      else
      {
         results_avail = TRUE;
      }
   }

   /* Process the results if they are ready */
   if (results_avail)
   {
      /* Copy the results into the array */
      for (i = 0; i < *num_rssi; i++)
      {
         if (mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].scheduleTimeError[i] == TRUE )
         {
           ScheduleError[i] = TRUE;
           ScheduleErrorCount++; 
         }
         else
         {
           ScheduleError[i] = FALSE;
         }
         rssi_values[i] = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].rssi[i];  /*lint !e644*/
         yield_status[i] = ((mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].reason[i] & yield_mask) != 0);
#ifdef GERAN_L1_HLLL_LNASTATE 
         jdetvalues[i] = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].jdetReading[i];
#endif /*GERAN_L1_HLLL_LNASTATE*/
#ifdef GL1_GMSK_MOD_DET
         energytest_flag[i] = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].gfwGdetResultBuffer[i].failedEngyTest;
         decstat[i] = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].gfwGdetResultBuffer[i].decStat1;
		 decstat2[i] = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].gfwGdetResultBuffer[i].decStat2;
#endif
         if ( mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].reason[i] & ENTER_MODE_LATE_CANCEL_BURST )
         {
            gl1_hw_enter_mode_late_cancel_burst (gas_id);
         }
      }
      (void) mdsp_validate_power_measure(*num_rssi, rssi_values , gas_id);
	  
      /*Run Error Recovery algorithm even if one burst dropped because of RF script building delay*/
      gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_MON, (ScheduleErrorCount != 0), gas_id);
#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
      if((gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
         && (ScheduleErrorCount == 0))
      {
        gtmrs_data_p->geran_rf_timeline_recovery.number_of_success_pow_mon++;
      }
#endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */
   }
   else if (last_attempt)
   {
      MSG_GERAN_ERROR_1_G("Power meas results not available, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
   }

   /* Increase read indices if we got results or are giving up */
   if (results_avail || last_attempt)
   {
      INC_MOD(private_data_ptr->mdsp_gprs_power_measure_burst_rd_index,3);
      INC_MOD(private_data_ptr->mdsp_gprs_power_measure_burst_seq_rd_index,4);
   }

   return results_avail;
}

/*===========================================================================

FUNCTION mdsp_gprs_is_power_measure_results_available

DESCRIPTION
  Checks the ready mask and match the sequence number for the current read index,
  if both matches then monitor result is available to fetch.

DEPENDENCIES
  None

RETURN VALUE
  Whether data is available

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_gprs_is_power_measure_results_available ( gas_id_t gas_id )
{
   uint16                         data_len;
   uint16                         i;
   boolean                        ready_mask =FALSE;
   boolean                        results_avail = FALSE;
   uint8                          rd_index, seq_rd_index;

   mdsp_gprs_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t*  mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* Calculate the seq number index */
   rd_index = private_data_ptr->mdsp_gprs_power_measure_burst_rd_index;

   /* Read the size */
   data_len = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].length;

   /* Check if the results are valid */
   if (data_len > MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS  ) 
   {
      data_len = MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS;
   }

   for ( i=0 ;i< data_len;i++ ) 
   {
       if ( TRUE ==  mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].readyMask[i] ) 
       {
         ready_mask = TRUE;
       }
       else 
       {
         ready_mask = FALSE;
         break;
       }
   }

   /* If the data is ready then match the seq number */
   if (ready_mask)
   {
      /* Now go and read the rest of the data */
      /* The results are only valid if the seq num in the request matches */
      seq_rd_index = private_data_ptr->mdsp_gprs_power_measure_burst_seq_rd_index;
      if (mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].seqNum == private_data_ptr->mdsp_gprs_power_measure_burst_seq_num[seq_rd_index])
      {
         results_avail = TRUE;
      }
   }

   return results_avail;
}

/*===========================================================================

FUNCTION mdsp_gprs_abort_power_measure

DESCRIPTION
  This function cleans up any pending power measures.

===========================================================================*/
void mdsp_gprs_abort_power_measure ( gas_id_t gas_id )
{
   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   private_data_ptr->mdsp_gprs_power_measure_burst_wr_index = 0;
   private_data_ptr->mdsp_gprs_power_measure_burst_rd_index = 0;
   private_data_ptr->mdsp_gprs_power_measure_burst_seq_wr_index = 0;
   private_data_ptr->mdsp_gprs_power_measure_burst_seq_rd_index = 0;
}

/*===========================================================================

FUNCTION mdsp_gprs_set_medium_access_mode

DESCRIPTION
  Sets the mdsp mode on the fly.

  mode -  indicates fixed or dynamic allocation
===========================================================================*/
void mdsp_gprs_set_medium_access_mode(mdsp_access_mode_type mode , gas_id_t gas_id )
{
   uint8 mac_mode = (uint16)mode;

   get_mdsp_intf_public_dataspace_ptr( gas_id)->pMdspHostIfBaseStore->gfwSharedVariables.edge.mediaAccMode = mac_mode;
   mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE , gas_id);
}


#ifdef FEATURE_GSM_MDSP_EGPRS
/*===========================================================================

FUNCTION mdsp_gprs_set_egprs_mode

DESCRIPTION
  Sets the mdsp egprs mode on the fly.

  mode -  indicates if EGPRS is enabled or not
===========================================================================*/
void mdsp_gprs_set_egprs_mode (boolean enabled , gas_id_t gas_id )
{
   mdsp_intf_public_data_t*  mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);
   mdsp_gprs_public_data_t*  public_data_ptr    = get_mdsp_gprs_public_dataspace_ptr( gas_id);

   mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.edgeModeFlag = enabled;
   mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE , gas_id);
   /* Save state */
   public_data_ptr->mdsp_egprs_mode = enabled;
}

#ifdef FEATURE_GSM_EGPRS_IR_WS_REDUCTION
/*===========================================================================

FUNCTION mdsp_gprs_get_oper_win_size

DESCRIPTION
  Reads the dynamic egprs Ir window size.

  mode -  indicates if EGPRS is enabled or not
===========================================================================*/
uint16 mdsp_gprs_get_oper_win_size(gas_id_t gas_id)
{
   mdsp_intf_public_data_t*  mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   return (uint16)(mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.irVariableWindow);
}
#endif /* FEATURE_GSM_EGPRS_IR_WS_REDUCTION */

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
/*===========================================================================

FUNCTION mdsp_dm_pdtch_decode_done

DESCRIPTION
  Reduces the count of pending PDTCH decodes in a sane fashion.

===========================================================================*/
void mdsp_dm_pdtch_decode_done (boolean completed , gas_id_t gas_id )
{
   mdsp_gprs_public_data_t* public_data_ptr = get_mdsp_gprs_public_dataspace_ptr( gas_id);

   /* Sanity check */
   if (mdsp_dm_pdtch_decodes <= 0)
   {
      MSG_GERAN_ERROR_2_G("dm_pdtch_decodes out of sync (comp=%d), FN=%d",
                completed, GSTMR_GET_FN_GERAN(gas_id));
      mdsp_dm_pdtch_decodes = 0;
      return;
   }

   mdsp_dm_pdtch_decodes--;

   /* If we are aborting, we need to delay disabling DM, otherwise we can
      potentially do it immediately */
   if (completed)
   {
      mdsp_dm_disable( gas_id);
#ifdef MDSP_DM_DEBUG
      #error code not present
#endif
   }
   else
   {
      mdsp_dm_frames_to_inactive = MDSP_MAX_FRAMES_TO_DM_DONE;
   }


#ifdef MDSP_DM_DEBUG
   #error code not present
#endif
}



/*===========================================================================

FUNCTION mdsp_dm_disable

DESCRIPTION
  Disables DM if possible.  Should be called whenever a PDTCH decode is
  complete or the dm counter timer is ticked down.

===========================================================================*/
void mdsp_dm_disable ( gas_id_t gas_id )
{
   mdsp_gprs_public_data_t*  public_data_ptr  = get_mdsp_gprs_public_dataspace_ptr( gas_id);
   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* New I/F is such that MDSP_dmComplete must be */
   /* set to zero before DM clock is switched off. */
   if ( ( mdsp_dm_active && mdsp_dm_pdtch_decodes == 0 ) &&
        ( mdsp_dm_frames_to_inactive == 0 )
#if defined ( MDSP_dmComplete )
        && ( mdsp_read(MDSP_dmComplete) == 0 )
#endif
      )
   {
      #if   defined(FEATURE_DMOV)
      dmov_disable(DMOV_CLIENT_MDSP_GPRS);
      #else
      clk_regime_dm_inactive(
                          );
      #endif
      mdsp_dm_active = FALSE;

      MSG_GERAN_HIGH_1_G("DM Clock Switched Off, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
   }
#ifdef MDSP_DM_DEBUG
   #error code not present
#endif
}

#endif /* FEATURE_GSM_MDSP_IR && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM*/
#ifdef FEATURE_GSM_MDSP_IR
/*===========================================================================

FUNCTION mdsp_gprs_IR_enable

DESCRIPTION
  Sets the mdsp acknowledge mode on the fly.  Used for incremental
  redundancy.

===========================================================================*/
void mdsp_gprs_IR_enable (boolean enabled , gas_id_t gas_id )
{
   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   if (enabled == FALSE)
   {
      private_data_ptr->mdsp_ir_release = TRUE;
   }

   get_mdsp_intf_public_dataspace_ptr( gas_id)->pMdspHostIfBaseStore->gfwSharedVariables.edge.doIRFlag = enabled;
   mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE , gas_id);

#if !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
   /* Set flag indicating if DM is required or not */
   private_data_ptr->mdsp_dm_required = enabled;
#endif
}

/*===========================================================================

FUNCTION mdsp_gprs_IR_clear_memory

DESCRIPTION
  Tells the mdsp to clear the incremental redundancy memory for the
  given BSN range.

===========================================================================*/
void mdsp_gprs_IR_clear_memory (uint16 starting_bsn, uint16 ending_bsn , gas_id_t gas_id )
{
   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);
   private_data_ptr->Mdsp_pending_IR_config.clear_starting_bsn = starting_bsn;
   private_data_ptr->Mdsp_pending_IR_config.clear_ending_bsn = ending_bsn;
   private_data_ptr->Mdsp_pending_IR_config.clear_memory = TRUE;

}
/*===========================================================================

FUNCTION mdsp_gprs_IR_set_active_window

DESCRIPTION
  Sets the range of active BSNs for incremental redundancy.

===========================================================================*/
void mdsp_gprs_IR_set_active_window (uint16 starting_bsn, uint16 ending_bsn , gas_id_t gas_id )
{
   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   private_data_ptr->Mdsp_pending_IR_config.set_starting_bsn = starting_bsn;
   private_data_ptr->Mdsp_pending_IR_config.set_ending_bsn = ending_bsn;
   private_data_ptr->Mdsp_pending_IR_config.set_memory = TRUE;
}

/*===========================================================================

FUNCTION mdsp_gprs_issue_IR_conigure_memory

DESCRIPTION
  Sends pending IR configure memory commands (from ISR context).

===========================================================================*/
void mdsp_gprs_issue_IR_configure_memory ( sys_modem_as_id_e_type as_id )
{
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   GfwConfigureIrWindowCmd  configureIrWindowCmd;

   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   if (private_data_ptr->Mdsp_pending_IR_config.clear_memory == TRUE)
   {
      /* Clear out Q6 command first */
      memset(&configureIrWindowCmd,0,sizeof(GfwConfigureIrWindowCmd));

     configureIrWindowCmd.cmd          = GFW_CONFIGURE_IR_WINDOW_CMD;
     if (private_data_ptr->mdsp_ir_release == TRUE)
     {
       private_data_ptr->mdsp_ir_release = FALSE;
       private_data_ptr->Mdsp_pending_IR_config.set_memory = FALSE; /* prevents IR allocation if set */
       
       configureIrWindowCmd.config   = MDSP_CFG_IR_RELEASE;
     }
     else
     {
       configureIrWindowCmd.config     = MDSP_CFG_IR_WINDOW_CLEAR;
     }
     configureIrWindowCmd.startBsn     = private_data_ptr->Mdsp_pending_IR_config.clear_starting_bsn;
     configureIrWindowCmd.endBbsn      = private_data_ptr->Mdsp_pending_IR_config.clear_ending_bsn;

     private_data_ptr->Mdsp_pending_IR_config.clear_memory = FALSE;

#ifdef FEATURE_DUAL_SIM
     /* As per updated GFW ,command is handled in MEAS_ILDE */
     if (GFW_APP_MODE_ACTIVE != gl1_get_gfw_app_mode(gas_id) )
     {
        MSG_GERAN_HIGH_3_G("GFW_CONFIGURE_IR_WINDOW_CMD ->IR_RELEASE/IR_WINDOW_CLEAR %d sent in GFW state %d FN %d ",
                            configureIrWindowCmd.config,
                            gl1_get_gfw_app_mode(gas_id),
                            GSTMR_GET_FN_GERAN(gas_id));
     }
#endif /* FEATURE_DUAL_SIM */

     mdsp_issue_qdsp6_command((GfwCmdBufType *)&configureIrWindowCmd, GFW_CONFIGURE_IR_WINDOW_CMD , gas_id);
   }

   if (private_data_ptr->Mdsp_pending_IR_config.set_memory == TRUE)
   {
     /* Set only when GFW is ACTIVE  */
     /* during TA L1 periodically updates start/end bsn every 13 frames. 
        When GFW is not active , CMD with MDSP_CFG_IR_WINDOW_SET should not be sent to GFW.
     */
     private_data_ptr->Mdsp_pending_IR_config.set_memory = FALSE;
	 
#ifdef FEATURE_DUAL_SIM
     if (GFW_APP_MODE_ACTIVE == gl1_get_gfw_app_mode(gas_id) )
#endif /* FEATURE_DUAL_SIM */	 
     {
       /* Clear out Q6 command */
       memset(&configureIrWindowCmd,0,sizeof(GfwConfigureIrWindowCmd));

       configureIrWindowCmd.cmd          = GFW_CONFIGURE_IR_WINDOW_CMD;
       configureIrWindowCmd.config       = MDSP_CFG_IR_WINDOW_SET;
       configureIrWindowCmd.startBsn     = (private_data_ptr->Mdsp_pending_IR_config.set_starting_bsn + (MDSP_IR_BSN_MOD - 1)) % MDSP_IR_BSN_MOD;
       configureIrWindowCmd.endBbsn      = (private_data_ptr->Mdsp_pending_IR_config.set_ending_bsn + 1) % MDSP_IR_BSN_MOD;

       mdsp_issue_qdsp6_command((GfwCmdBufType *)&configureIrWindowCmd, GFW_CONFIGURE_IR_WINDOW_CMD , gas_id);
     }
#ifdef FEATURE_DUAL_SIM	 
     else
     {
        MSG_GERAN_HIGH_2_G("GFW_CONFIGURE_IR_WINDOW_CMD ->MDSP_CFG_IR_WINDOW_SET not sent in GFW state %d FN %d ",
                           gl1_get_gfw_app_mode(gas_id),
                           GSTMR_GET_FN_GERAN(gas_id));
     }
#endif /* FEATURE_DUAL_SIM */	 
   }
}

/*===========================================================================

FUNCTION mdsp_gprs_IR_set_tfi

DESCRIPTION
  Sets the current TFI to use when deciding to combine or not.

===========================================================================*/
void mdsp_gprs_IR_set_tfi (uint16 tfi , gas_id_t gas_id )
{
   get_mdsp_intf_public_dataspace_ptr( gas_id)->pMdspHostIfBaseStore->gfwSharedVariables.edge.tfiForIR = tfi;
   mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE , gas_id);
}

#endif /* FEATURE_GSM_MDSP_IR */
#endif /* FEATURE_GSM_MDSP_EGPRS */


/*===========================================================================

FUNCTION mdsp_gprs_set_aci_detection

DESCRIPTION
  Enable aci cci detection in mdsp

  mode -  indicates if ACI CCI detection is enabled or not
===========================================================================*/
void mdsp_gprs_set_aci_detection (boolean enabled , gas_id_t gas_id )
{
   mdsp_gprs_public_data_t* public_data_ptr = get_mdsp_gprs_public_dataspace_ptr( gas_id);

   /* Save ACI detection state */
   public_data_ptr->mdsp_aci_detection = enabled;
}

/*===========================================================================

FUNCTION mdsp_gprs_reset_rx_seq_nums

DESCRIPTION
  Resets the sequence number fifos for sync rx related seq numbers.

===========================================================================*/
void mdsp_gprs_reset_rx_seq_nums( gas_id_t gas_id  )
{
   mdsp_gprs_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   private_data_ptr->mdsp_gprs_power_measure_burst_wr_index = 0;
   private_data_ptr->mdsp_gprs_power_measure_burst_rd_index = 0;
   private_data_ptr->mdsp_gprs_power_measure_burst_seq_wr_index = 0;
   private_data_ptr->mdsp_gprs_power_measure_burst_seq_rd_index = 0;
}

#ifdef FEATURE_GSM_SCE_PWR_MEAS_DBG
/*===========================================================================

FUNCTION l1_sce_dbg_mdsp_gprs_new_entry

DESCRIPTION
  Prepares new entry for MDSP/L1-SCE debug structure

===========================================================================*/
l1_sce_dbg_mdsp_gprs_T *l1_sce_dbg_mdsp_gprs_new_entry(l1_sce_dbg_mdsp_gprs_cmd_T cmd, gas_id_t gas_id)
{
   l1_sce_dbg_mdsp_gprs_T * pl1_sce_dbg_mdsp_gprs;

   if( l1_sce_dbg_mdsp_gprs_idx[gas_id] < L1_SCE_DBG_MDSP_GPRS_LOG_IDX_MAX )
   {
      l1_sce_dbg_mdsp_gprs_idx[gas_id]++;
   }
   else
   {
      l1_sce_dbg_mdsp_gprs_idx[gas_id]=0;
   }

   pl1_sce_dbg_mdsp_gprs = &l1_sce_dbg_mdsp_gprs[gas_id][l1_sce_dbg_mdsp_gprs_idx[gas_id]];
   memset(pl1_sce_dbg_mdsp_gprs, 0, sizeof(l1_sce_dbg_mdsp_gprs_T));

   pl1_sce_dbg_mdsp_gprs->FN = GSTMR_GET_FN_GERAN(gas_id);
   pl1_sce_dbg_mdsp_gprs->cmd = cmd;
   pl1_sce_dbg_mdsp_gprs->mdsp_results_avail = mdsp_gprs_is_power_measure_results_available(gas_id);

   return pl1_sce_dbg_mdsp_gprs;
}

/*===========================================================================

FUNCTION l1_sce_dbg_mdsp_gprs_send_pwr_meas_cmd_to_fw

DESCRIPTION
  Write an entry into MDSP/L1-SCE debug structure when cmd is sent to fw

===========================================================================*/
static void l1_sce_dbg_mdsp_gprs_send_pwr_meas_cmd_to_fw(
   uint16             num,             /* number of measurements */
   uint16             offsets[],       /* which offsets */
   ARFCN_T            *arfcns,
   gas_id_t gas_id)
{
   l1_sce_dbg_mdsp_gprs_T *pl1_sce_dbg_mdsp_gprs;
   
   pl1_sce_dbg_mdsp_gprs = l1_sce_dbg_mdsp_gprs_new_entry(MDSP_SEND_PWR_CMD_SCE_DBG, gas_id);

   pl1_sce_dbg_mdsp_gprs->data_len = num;
   pl1_sce_dbg_mdsp_gprs->offset_s = offsets[0];
   pl1_sce_dbg_mdsp_gprs->offset_e = offsets[num-1];
   pl1_sce_dbg_mdsp_gprs->seq_num_exp = mdsp_get_current_seq_num( gas_id);
#ifdef FEATURE_GSM_COEX
   pl1_sce_dbg_mdsp_gprs->arfcn_s = arfcns[0].num;
   pl1_sce_dbg_mdsp_gprs->arfcn_e = arfcns[num-1].num;
#endif

}

/*===========================================================================

FUNCTION l1_sce_dbg_mdsp_gprs_get_pwr_meas_res_from_fw

DESCRIPTION
  Write an entry into MDSP/L1-SCE debug structure when results are read from fw

===========================================================================*/
static void l1_sce_dbg_mdsp_gprs_get_pwr_meas_res_from_fw(
   boolean last_attempt,
   uint32 yield_mask,
   gas_id_t gas_id)
{
   uint8                      rd_index, seq_rd_index;
   l1_sce_dbg_mdsp_gprs_T *   pl1_sce_dbg_mdsp_gprs;
   mdsp_gprs_private_data_t*  private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t*   mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   pl1_sce_dbg_mdsp_gprs = l1_sce_dbg_mdsp_gprs_new_entry(MDSP_GET_PWR_RESULTS_SCE_DBG, gas_id);

   rd_index = private_data_ptr->mdsp_gprs_power_measure_burst_rd_index;
   seq_rd_index = private_data_ptr->mdsp_gprs_power_measure_burst_seq_rd_index;

   pl1_sce_dbg_mdsp_gprs->data_len = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].length;
   pl1_sce_dbg_mdsp_gprs->seq_num_exp = private_data_ptr->mdsp_gprs_power_measure_burst_seq_num[seq_rd_index];
   pl1_sce_dbg_mdsp_gprs->seq_num_fw = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].seqNum;
   pl1_sce_dbg_mdsp_gprs->readymask = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].readyMask[0];
   pl1_sce_dbg_mdsp_gprs->rssi_values = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].rssi[0];
   pl1_sce_dbg_mdsp_gprs->yield_status = ((mdsp_intf_data_ptr->pMdspHostResultsStore->gfwPwrMsrResultBuffers[rd_index].reason[0] & yield_mask) != 0);
   pl1_sce_dbg_mdsp_gprs->last_attempt = last_attempt;
}

#endif /* FEATURE_GSM_SCE_PWR_MEAS_DBG */

