/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      L 1   P O W E R   S C A N

GENERAL DESCRIPTION
   This module contains the procedures to perform power measurement
   scanning.  This procedure executes in the ISR context and
   signals the L1 task of any status.

EXTERNALIZED FUNCTIONS
  L1_power_scan
    Procedure to initiate a power measurement scan

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_pscan.c#2 $
$DateTime: 2019/12/30 04:21:22 $ $Author: pwbldsvc $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
24/12/19   sc        CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
17/08/17   br        CR2092505 NV control for logging GDET related F3s
07/08/17   sn        CR2084716 changes to fix memory heap starvation issue with GDET FR
06/06/17   br        CR2052392 Reallocate the memory for decstat, if the decstat pointer is NULL
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
03/01/16   mn        CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
20/10/14   nm        CR740938 calculate the meas duration for pscan correctly when resumed
20/05/14   sk        CR659786 Ensure yielded monitor not considered for averaging
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
25/04/14   og        CR650586: COEX : X+G DSDA: Huge PSCAN results seen if monitors are YIELDED
11/04/14   sp        CR645132: Configure PSCAN handler for maximum possible frames required for complete scan, as arbitrator decides later on max_meas_per_frames.
                               Regression after CR/583282.
08/04/14   gk        CR641603 Ignore yielded power monitor results
05/03/14   mc        CR.583282 : Limit the number of PSCAN monitors per frame to that which can be COEX protected in X+G DSDA
27/11/13   cja/npt   CR555678 Speed up clock during power scan for RF processing load
18/11/13   js        CR566406 Correction for Check for null frequency list
07/11/13   nm        CR565664 - fix for regression from CR CR548047
07/11/13   og        CR565881. Reduce the number of power scan monitors per frame
                     from 7 to 2 when WCDMA is in dedicated mode.
07/11/13   nm        CR565664 - fix for regression from CR CR548047
06/11/13   js        CR566406 Check for null frequency list
23/10/13   ws        CR548047 in Pscan state machine where we are checking wrong array index
14/10/13   cja       CR555678 Speed up clock during power scan for RF processing load
05/10/13   gk        CR516242:Reset the number of frequencies completed to 0, after receiving the power scan request
04/10/13   nm        CR552803 tighten power scan threshold from -107dBm to -102dBm for background power scan only
03/10/12   sk        CR553848 Bring back the CR548047 and ensure power scan doesnt run indefinitely
01/10/13   sk        CR552836 backout the CR548047
25/09/13   ap        CR548047 DSDS: Power scan is completing and sending confirmation to RR prematurely
26/08/13   sk        CR534005 Use correct API to get the dual/triple standby mode
15/07/13   sp        CR508123: Update panic reset to use multi-sim data space
25/06/13   cs        Major Triton TSTS Syncup
06/06/13   ap        CR491896 Delay the release of TRM when pscan ends for MPLMN
13/05/13   ap        471917 DSDS G+G: SUB1 and SUB2 stuck in Roaming Indefinitely
06/06/13   cs        TSTS Updates
08/06/12   pa        GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
15/05/11   jj        CR357981 Abort aquicition earlier if panic reset triggered
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
11/04/11   og        CR280835. Add two frames delay between stopping the power meas
                     handler and issuing the suspend command.
19/10/10   cja       CR260187 Check for L1 Frequency List being null.
11/01/10   ap        CR222956 - Incorrect minimum power scan measurment length used.
29/05/09   ps        Zero Initialise memory relocation to Heap for selected data structure
20/04/09   cs        CR145322 Fix the tav_ticks calculation to avoid negative numbers
09/10/08   cs        Allow the RX Sweep acquisition to be restarted
04/09/08   dv        CR153510: Fix for Manual NW search failure
28/08/08   cs        Implement the Fast Rx Sweep functionality for FTM
19/07/07   og        UOOS changes, reduce number of measured samples.CR119147.
30/03/07   og        Prevent a div by zero when power scan meas per carrier = 1.
01/27/06   sv        Abort power scan when deactivating GSM mode.
01/16/06   sv        Fix CR85766 - UE reset while Stability testing for Abort specific
                     BPLMN testing
01/05/06   sv        Set the main command to SUSPEND_COMMAND only when suspending for GSM
                     and not when stopping GSM mode.
11/15/05   sv        Lint cleanup.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
08/17/05   gfr       Use gl1_msg_init_frame_tick_info to init the ftn structure
07/29/05   gfr       Support for NPL1 DTM interface changes
03/16/05   gfr       Force a GSM core reset if the channel filter is stuck
02/09/05   bm        Power scan optimizations
02/05/05   bm        BG PLMN scan optimizations
01/26/05   sv        Add support for Frame Manager.
06/07/04   gw        Allow reselection pcan to do more than 1 meas per ARFCN.
06/02/04   gw        Do fewer measurements for reselection powerscan.
05/26/04   sv        Lint Cleanup.
05/20/04   gfr       Mainlined FEATURE_COMMON_PWR_HANDLER
04/02/04   gw        Removed some lint errors.
03/31/04   dp        Fixed compiler warnings
03/26/04   gfr       Just report if the power scan is bad, no error fatal.
03/22/04   gfr       Validate the power scan results if flag is set.
02/25/04   gw        Lint clean up.
02/09/04   dp        Added support for background HPLMN searching
11/07/03   gw        Changes so that gl1_sys_params.h controls min pscan
                     duration but max duration is determined by the number
                     of ARFCNs to scan.  This means all ARFCNs are scanned
                     even if it takes longer than 5s.
05/14/03   gfr       Common power handler support.
01/17/02   dp        Moved gl1_msg_cfg_power_measure() for power scan from
                     l1_task (l1_null_if) to here to fix "Idle abort" errors
10/15/02   JC        Removed valid measurement check in PSCAN callback
                     since we do a single measure per MP (new systems algo).
06/14/02   mk        Added call to gl1_msg_reset() to support AGC.
06/13/02   mk        Fixes to pscan measurement period. Introduced new
                     L1_SCAN_WAIT_NEXT_MEASURE_PERIOD sub_sub_state.
05/21/02   gw        Changes to match changes in msg layer power measure API.
05/21/02   mk        Changed L1_SCAN_END state to handle only ACQ command.
03/08/02   ATM       Replace GSM_DEBUG with MSG_ macros
03/04/02   JC        Cleanup. Removed legacy code.
01/11/02   JC/GW     Added systems algorithms for AGC.
08/31/01   JC        Initial version (Sina emulaton board). Untested.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "gl1_msg_g.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "msg.h"
#include "err.h"
#include "l1_fm.h"
#include "l1_utils.h"
#include "l1i.h"

#include "geran_dual_sim.h"
#ifdef GL1_GMSK_MOD_DET
#include "gprs_mem.h"
#endif
#include "mcfg_nv_api.h"

extern void l1_set_background_HPLMN_search_suspended(boolean background_HPLMN_search_suspended, gas_id_t gas_id);

#ifdef DEBUG_PSCAN
#error code not present
#endif

extern void L1_pscan_callback( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id );
extern void L1_pscan_abort_callback( gas_id_t gas_id );

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

#define NUM_FRAMES_IN_5_SECONDS 1084
#define NUM_FRAMES_IN_3_SECONDS 650
#define NUM_FRAMES_IN_1_SECOND  (217) /* Added for UOOS */

#define MEAS_OVERHEAD           4    /* frames, a little conservative */
#define MIN_MEASUREMENTS_PER_FRAME_PSCAN  (2)

typedef struct
{

/* Timer used to make sure samples on the same ARFCN are spaced
 * sufficiently far apart.
 */
uint16   meas_period_timer;

/* Counter to count how many passes have been done.
 * Usually 5 passes are done (i.e. 5 measurements per ARFCN)
 * but for reselection pscan fewer are done.
 */
uint16   num_samples;

/* Flag set if we should validate that not all results are 0 */
boolean l1_pscan_validate_results;

uint16   numFreqsCompleted ;

uint16                       meas_duration;
gl1_msg_pwr_meas_result_type rpt;
uint32  fn_before_suspend ;
dBx16_T  pwr_threshold;
}l1_pscan_data_t;



extern void L1_background_HPLMN_search_suspend_cb( gas_id_t gas_id );
extern void L1_suspend_background_HPLMN_search( gas_id_t gas_id );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
extern void l1_ds_set_bplmn_state_to_null(gas_id_t gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

void l1_pscan_complete(gas_id_t gas_id);

void l1_pscan_reset_freq_completed(gas_id_t gas_id);

static l1_pscan_data_t  l1_pscan_data[NUM_GERAN_DATA_SPACES];
#ifdef GL1_GMSK_MOD_DET
uint16 *decStat_pscan[NUM_GERAN_DATA_SPACES] = {NULL};
extern boolean gl1_hw_is_gdet_debug_enabled(void);
#endif

void l1_set_pscan_validate_results(boolean l1_pscan_validate_results, gas_id_t gas_id)
{
  l1_pscan_data_t *l1_pscan_data_ptr = &l1_pscan_data[gas_id];
  l1_pscan_data_ptr->l1_pscan_validate_results=l1_pscan_validate_results;
}


/*===========================================================================

FUNCTION  L1_POWER_SCAN

DESCRIPTION
  This function handles the L1 power scan for idle mode.

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_power_scan ( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_pscan_data_t *l1_pscan_data_ptr = &l1_pscan_data[gas_id];
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];
  /* locals */
   gl1_msg_frame_tick_type             ftn;
   uint16 tav_ticks_min,  num_frames;
   uint8  min_meas_per_frame = SYS_PARAM_NUM_MEASUREMENTS_PER_FRAME_PSCAN;
   /* Init frame tick info for NPL1 */
   gl1_msg_init_frame_tick_info(&ftn);
    
   if ((l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )||panic_reset_occured[gas_id])
   {
      gl1_msg_abort_pwr_meas (gas_id);   /* abort any power scans in progress */

      return;
   }

   /* Code starts */
   switch ( l1_tskisr_blk->pscan_state )
   {

      case L1_SCAN_START:

         if(mcfg_gcf_nv_get_status())
         {
             /* Do not use optimized values */
             tav_ticks_min = SYS_PARAM_Tav_TICKS_MIN;
             num_frames = NUM_FRAMES_IN_5_SECONDS;
         }
         else
         {
             /* Use the optimized values */
             tav_ticks_min = SYS_PARAM_Tav_TICKS_MIN_OPT;

             /* Changes for UOOS, reduced from 3 seconds */
             num_frames = NUM_FRAMES_IN_1_SECOND;
         }

         /* Configure the message layer */
         gl1_msg_reset(gas_id);

         l1_pscan_data_ptr->num_samples = l1_Frequency_List->meas_per_ARFCN;

         MSG_GERAN_HIGH_3_G( "PScan Start (%d meas per ARFCN) tav_ticks_min %d num_frames %d",
                   l1_pscan_data_ptr->num_samples, tav_ticks_min, num_frames );

         /* Reset numFreqsCompleted */
         l1_pscan_data_ptr->numFreqsCompleted = 0;

         /* Establish the interval, in ticks, between consecutive meaurements
          * on the same carrier.  This calcuLation is designed to pick the
          * smallest interval that will make the total power scan duration
          * (form 1st meas on 1st carrier to last meas on last carrier) at
          * least SYS_PARAM_Tav_TICKS_MIN.
          */

          if ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
          {
            /*In DSDA Mode, arbitrator can choose to perform only 2 monitors per frame, when X-RAT is agressing*/
          min_meas_per_frame = MIN_MEASUREMENTS_PER_FRAME_PSCAN;
          }
         /* work out how long each set of measurements will take */
         l1_pscan_data_ptr->meas_duration  = ((l1_Frequency_List->length-1) / min_meas_per_frame) + 1;

         l1_pscan_data_ptr->meas_duration += MEAS_OVERHEAD;

         /* Do not use optimised values if Anite GCF flag is set */

         /* make meas duration the max of how long the measurements will take and how
          * long the duration needs to be to get the required total duration
          */
         if ( l1_pscan_data_ptr->meas_duration < l1_Frequency_List->min_meas_period )
         {
            l1_pscan_data_ptr->meas_duration = l1_Frequency_List->min_meas_period;
         }
         else
         {
            uint32 dur = ( l1_pscan_data_ptr->meas_duration * l1_Frequency_List->meas_per_ARFCN );

            if (dur > tav_ticks_min)
            {
               if ( dur < num_frames )
               {
                  MSG_GERAN_HIGH_3_G("Power scan extended. %d -> %d frames num %d", tav_ticks_min, dur, num_frames);
               }
               else
               {
                  MSG_GERAN_ERROR_3_G("Power scan too long. %d frames tav_ticks_min %d num %d", dur, tav_ticks_min,num_frames);
               }
            }
         }

         /* end of this interrupt - change state for next time */
         l1_tskisr_blk->pscan_state = L1_SCAN_NEXT_MEAS_PERIOD;   /* start sampling */
#ifdef GL1_GMSK_MOD_DET
         if(decStat_pscan[gas_id] == NULL)
         {
           decStat_pscan[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(RR_MAX_PSCAN_FREQ_SCAN+1));
         }
#endif
         break;

      case L1_SCAN_NEXT_MEAS_PERIOD:       /* start of new measurement period */

         MSG_GERAN_HIGH_3_G( "Power Scan next meas period duration %d per frame %d",
                   l1_pscan_data_ptr->meas_duration,
                   SYS_PARAM_NUM_MEASUREMENTS_PER_FRAME_PSCAN,
                   0 );

         if (l1_pscan_data_ptr->num_samples > 0)
         {

            /* Start the timer for this set of measurements.  This timer ensures that
             * the gap between any two measuerments on the same ARFCN is at least
             * meas_duration.
             */
            l1_pscan_data_ptr->meas_period_timer = l1_pscan_data_ptr->meas_duration;

            if (gl1_msg_cfg_pwr_meas(L1_pscan_callback,L1_pscan_abort_callback,gas_id))
            {
               /* Reduce the count of samples that need to be collected for each carrier */
               l1_pscan_data_ptr->num_samples--;

               /* We do not care about the raw data */
               l1_pscan_data_ptr->rpt.pwr_dBm_x16 = NULL;
               l1_pscan_data_ptr->rpt.yielded = l1_Frequency_List->yielded;
#ifdef GL1_GMSK_MOD_DET
			  if( decStat_pscan[gas_id] ==   NULL)
			  {
			    decStat_pscan[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(RR_MAX_PSCAN_FREQ_SCAN+1));
			  }
			   l1_pscan_data_ptr->rpt.decstat= decStat_pscan[gas_id];
#endif
               gl1_msg_do_pwr_meas
               (
                  l1_Frequency_List->ARFCN,
                  l1_Frequency_List->agc_ptrs,
                  l1_Frequency_List->length,
                  SYS_PARAM_NUM_MEASUREMENTS_PER_FRAME_PSCAN,
                  l1_pscan_data_ptr->meas_duration,
                 &l1_pscan_data_ptr->rpt,
                  gas_id
               );

               /* Set the block start flag for NPL1 */
               ftn.next_frame_is_pwr_meas_block_start = TRUE;

               l1_tskisr_blk->pscan_state = L1_SCAN_WAIT;   /* get a sample of each carrier */
            }
            else
            {
               MSG_GERAN_ERROR_0_G("Pwr meas config not succesfull, pwr meas not intialised");
            }
         }
         else
         {
           l1_pscan_complete(gas_id);
           l1_pscan_mcpm_change (FALSE,gas_id);
         }
         break;

      case L1_SCAN_WAIT :   /* Scan all frequencies during a single measurement period */
         if (l1_pscan_data_ptr->meas_period_timer > 0)
         {
            l1_pscan_data_ptr->meas_period_timer--;
         }
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
         // In case of GPLT and no RF, return default/canned result
         MSG_GERAN_HIGH_0_G("GPLT DBG: L1_power_scan: making num_samples = 0 while waiting for results");
         num_samples = 0;
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
         break;

      case L1_SCAN_WAIT_NEXT_MEAS_PERIOD :
         /* Wait for start of the next measurement period */
         if (l1_pscan_data_ptr->meas_period_timer > 0)
         {
            l1_pscan_data_ptr->meas_period_timer--;
         }
         else
         {
            /* done waiting - change state to start next measure period */
            l1_tskisr_blk->pscan_state = L1_SCAN_NEXT_MEAS_PERIOD;
         }
         break;

      case L1_SCAN_WAIT_TO_RESUME_MEAS_PERIOD:
         if(l1_Frequency_List !=NULL)
         {
           if (l1_Frequency_List->no_of_measurement[l1_Frequency_List->length - 1] == l1_Frequency_List->meas_per_ARFCN)
           {
             l1_pscan_complete(gas_id);
           }
         }
         break;

      case L1_SCAN_RESUME_MEAS_PERIOD:
         /* Resume interrupted measurement period */
         MSG_GERAN_MED_0_G("Resuming interrupted power scan");

         if (l1_pscan_data_ptr->numFreqsCompleted == 0)
         {
            MSG_GERAN_MED_1_G("num_samples %d",l1_pscan_data_ptr->num_samples);
            if(l1_pscan_data_ptr->num_samples > 0)
            {
              l1_pscan_data_ptr->num_samples--;
            }
         }
         if (gl1_msg_cfg_pwr_meas(L1_pscan_callback,L1_pscan_abort_callback,gas_id))
         {
            uint32 dur = 0;


            if ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
            {
              /*In DSDA Mode, arbitrator can choose to perform only 2 monitors per frame, when X-RAT is agressing*/
            min_meas_per_frame = MIN_MEASUREMENTS_PER_FRAME_PSCAN;
            }

            if(((l1_Frequency_List->length - l1_pscan_data_ptr->numFreqsCompleted)% min_meas_per_frame) != 0) 
              dur = (l1_Frequency_List->length - l1_pscan_data_ptr->numFreqsCompleted)/min_meas_per_frame +  1;
            else
            dur = (l1_Frequency_List->length - l1_pscan_data_ptr->numFreqsCompleted)/min_meas_per_frame;

            /* We do not care about the raw data */
            l1_pscan_data_ptr->rpt.pwr_dBm_x16 = NULL;
            l1_pscan_data_ptr->rpt.yielded = l1_Frequency_List->yielded;
#ifdef GL1_GMSK_MOD_DET
			  if( decStat_pscan[gas_id] ==   NULL)
			  {
			    decStat_pscan[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(RR_MAX_PSCAN_FREQ_SCAN+1));
			  }
			l1_pscan_data_ptr->rpt.decstat= decStat_pscan[gas_id];
#endif
            MSG_GERAN_MED_2_G("Freqs left %d Meas dur %d",
                    (l1_Frequency_List->length - l1_pscan_data_ptr->numFreqsCompleted),dur);

            gl1_msg_do_pwr_meas
            (
               ( l1_Frequency_List->ARFCN + l1_pscan_data_ptr->numFreqsCompleted ),
               ( l1_Frequency_List->agc_ptrs + l1_pscan_data_ptr->numFreqsCompleted ),
               ( l1_Frequency_List->length - l1_pscan_data_ptr->numFreqsCompleted ),
               SYS_PARAM_NUM_MEASUREMENTS_PER_FRAME_PSCAN,
               dur,
               &l1_pscan_data_ptr->rpt,
               gas_id
            );

            /* Set the block start flag for NPL1 */
            ftn.next_frame_is_pwr_meas_block_start = TRUE;

            l1_tskisr_blk->pscan_state = L1_SCAN_WAIT;   /* get a sample of each carrier */
         }
         else
         {
            MSG_GERAN_ERROR_0_G("Pwr meas config not succesfull, pwr meas not intialised");
         }
         break;

      case L1_SCAN_END:
         /* Await further commands */

         if ( l1_tskisr_blk->command == L1_ACQUIRE_COMMAND )
         {
            l1_tskisr_blk->command       = L1_NULL_COMMAND;
            l1_tskisr_blk->sub_state     = L1_ACQUIRING;
            l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE;
            L1_transfer_new_data( gas_id );
         }
#ifdef DEBUG_PSCAN
         #error code not present
#endif
         break;

      default:
         MSG_GERAN_HIGH_0_G("Unexpected l1 sub sub state.");
         break;

   } /* of switch */

   /* Issue frame tick notification to message layer */
   gl1_msg_frame_tick_notification(&ftn,gas_id);
   ftn.next_frame_is_pwr_meas_block_start = FALSE;

}

/*===========================================================================

FUNCTION  L1_pscan_callback

DESCRIPTION
  This function is called by the message layer once every measurement
  period.  A measurement period is defined by:

    SYS_PARAM_Tav_TICKS/(SYS_PARAM_NUM_MEASUREMENTS_PER_CARRIER-1)

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the sub_sub_state.

===========================================================================*/
void L1_pscan_callback
(
  gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id
)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_pscan_data_t *l1_pscan_data_ptr = &l1_pscan_data[gas_id];
   Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];

   uint16 i;
   uint32 res_cnt;
   Frequency_List_T *frq_list_ptr;
   sys_algo_agc_T   *agc_ptr;
   boolean pscan_valid = FALSE;

   /* Check for Null pointer (can occur if GSM deactivating before callback) */
   if(l1_Frequency_List == NULL)
   {
       MSG_GERAN_HIGH_0_G("l1_Frequency_List NULL!");
       return;
   }

   /* Assign pointer for results */
   frq_list_ptr = l1_Frequency_List;

   res_cnt = rpt->num_pwr_meas;

   /* Loop through each ARFCN and update the number of measurments */
   for (i = 0; i < res_cnt; i++)
   {
      agc_ptr = rpt->agc_ptrs[i];

#ifdef FEATURE_GSM_COEX
      if( (frq_list_ptr->yielded[l1_pscan_data_ptr->numFreqsCompleted+i]) || ( agc_ptr->pwr_dBm_x16 == RX_POWER_FLOOR ))
      {
        MSG_GERAN_HIGH_2_G("PSCAN COEX - yielded monitor ignored ARFCN %d, band %d", frq_list_ptr->ARFCN[l1_pscan_data_ptr->numFreqsCompleted+i].num, frq_list_ptr->ARFCN[l1_pscan_data_ptr->numFreqsCompleted+i].band);
        continue;
      }
#endif
      if ( frq_list_ptr->no_of_measurement[l1_pscan_data_ptr->numFreqsCompleted+i] == 0 )
      {
         frq_list_ptr->power_dBm_x16[l1_pscan_data_ptr->numFreqsCompleted+i] = agc_ptr->pwr_dBm_x16;
#ifdef GL1_GMSK_MOD_DET
		 frq_list_ptr->decstat[l1_pscan_data_ptr->numFreqsCompleted+i] = rpt->decstat[i];
		 if(gl1_hw_is_gdet_debug_enabled())
		 {
		   MSG_GERAN_HIGH_3_G(" frq list dec stat %d, rpt dec stat %d arfcn %d", frq_list_ptr->decstat[l1_pscan_data_ptr->numFreqsCompleted+i], rpt->decstat[i],frq_list_ptr->ARFCN[l1_pscan_data_ptr->numFreqsCompleted+i].num);
		 }
		  
#endif	 
      }
      else
      {
         frq_list_ptr->power_dBm_x16[l1_pscan_data_ptr->numFreqsCompleted+i] += agc_ptr->pwr_dBm_x16;
#ifdef GL1_GMSK_MOD_DET
		 frq_list_ptr->decstat[l1_pscan_data_ptr->numFreqsCompleted+i] += rpt->decstat[i];
		 if(gl1_hw_is_gdet_debug_enabled())
		 {
		   MSG_GERAN_HIGH_3_G(" frq list dec stat %d, rpt dec stat %d arfcn %d", frq_list_ptr->decstat[l1_pscan_data_ptr->numFreqsCompleted+i], rpt->decstat[i],frq_list_ptr->ARFCN[l1_pscan_data_ptr->numFreqsCompleted+i].num);
		 }
#endif		  
      }
      frq_list_ptr->no_of_measurement[l1_pscan_data_ptr->numFreqsCompleted+i]++;

      /* If any result is > -120 dBm we assume we are ok */
      if (agc_ptr->pwr_dBm_x16 > (-120*16)) pscan_valid = TRUE;
   }

   /* If every result was 0, we are in trouble, force a GSM core reset
      next time we start up */
   if (l1_pscan_data_ptr->l1_pscan_validate_results && res_cnt > 0 && !pscan_valid)
   {
      MSG_GERAN_ERROR_0_G("Bad pscan, channel filter probably stuck");
      gstmr_force_reinit(gas_id);
   }
   l1_pscan_data_ptr->l1_pscan_validate_results = FALSE;

   /*
      This saves off the index at which the next set of measurements
      should start.   Not a new measurement period but the next set
      of measurements after the interruption
   */
   l1_pscan_data_ptr->numFreqsCompleted += i;

   MSG_GERAN_HIGH_1_G("Pscan: %d freqs completed",l1_pscan_data_ptr->numFreqsCompleted);

   /* This implies that this measurement set was aborted */
   if ( rpt->stopped )
   {
      if ( l1_Frequency_List->length == l1_pscan_data_ptr->numFreqsCompleted )
      {
         MSG_GERAN_HIGH_0_G("rpt->stopped. Meas period complete");
      }
      else
      {
         MSG_GERAN_HIGH_0_G("rpt->stopped. Meas period Incomplete");
         //l1_tskisr_blk->sub_sub_state = L1_SCAN_WAIT_TO_RESUME_MEAS_PERIOD;
         return;
      }
   }

   /* Reset numFreqsCompleted if one set of measurements has been completed */
   l1_pscan_data_ptr->numFreqsCompleted = 0;

   switch( l1_tskisr_blk->l1_state )
   {
     case  L1_NULL_MODE:
     case  L1_FIND_BCCH:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     case  L1_MULTI_SIM_FIND_BCCH:
     case  L1_MULTI_SIM_NULL:
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

     {
       /* We need to void the problem of having the second/third rounds of
        * measurements becoming longer in length than the first one.
        * This needs to happen only in find_bcch and null modes of operation.
        * The meas_period_timer still needs to be followed in W2G BPLMN and
        * HPLMN searches.
        */
       MSG_GERAN_MED_0_G("resume next meas period immediately");
       l1_pscan_data_ptr->meas_period_timer = 0;
     }
     break;

     default:
     break;
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if ( rpt->stopped && ( l1_tskisr_blk->pscan_state == L1_SCAN_WAIT_TO_RESUME_MEAS_PERIOD ) )
   {
      /* If here, Meas period is complete. num_samples is normally decremented in state
       * L1_SCAN_NEXT_MEAS_PERIOD, but this state is by passed under these conditions.
       */
      MSG_GERAN_MED_1_G("PSCAN was aborted don't resume immediatly (num samples %d ) - suspension underway ",
             l1_pscan_data_ptr->num_samples);
   }
   else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   if ( (l1_pscan_data_ptr->meas_period_timer == 0) || (l1_pscan_data_ptr->num_samples == 0) )
   {
      /* Start next set of measurements now */
      l1_tskisr_blk->pscan_state = L1_SCAN_NEXT_MEAS_PERIOD;
   }
   else
   {
      /* Wait for start of the next measure period */
      l1_tskisr_blk->pscan_state = L1_SCAN_WAIT_NEXT_MEAS_PERIOD;
   }
#ifdef DEBUG_PSCAN
   #error code not present
#endif

}



/*===========================================================================

FUNCTION  L1_pscan_abort_callback

DESCRIPTION
  This function is a placeholder only.  It was introduced when the
  unified message layer power handler was created.  It may be removed in
  the future if we don't register an abort handler with the message handler.

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_pscan_abort_callback( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
  if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
  {
    if (l1_get_bplmn_suspend_in_progress(gas_id))
    {
      l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
    }
  }
  else
#endif
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH )
  {
    MSG_GERAN_MED_0_G("Dual Sim pscan stopped");
  }
  else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  {
    L1_background_HPLMN_search_suspend_cb(gas_id);
  }
}

/*===========================================================================

FUNCTION  L1_pscan_suspend

DESCRIPTION
  This function aborts a power scan in progress.  Called by the
  background HPLMN search engine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#define  GL1_PSCAN_WAIT_FRAMES_BEFORE_SUSPEND  (2U)

void L1_pscan_suspend( void* dummy, gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_pscan_data_t *l1_pscan_data_ptr = &l1_pscan_data[gas_id];


   NOTUSED( dummy );

   if ( l1_tskisr_blk->pscan_state == L1_SCAN_WAIT )
   {
      MSG_GERAN_HIGH_0_G("Power meas in progress..stopping");
      gl1_msg_stop_pwr_meas(gas_id);

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
      if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
      {
        l1_pscan_data_ptr->fn_before_suspend = GL1_PSCAN_WAIT_FRAMES_BEFORE_SUSPEND;
      }
#endif
   }
   else
   {
      MSG_GERAN_HIGH_1_G( "No Power meas in progress..halting count %d",l1_pscan_data_ptr->fn_before_suspend);

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
      if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
      {
         if (l1_pscan_data_ptr->fn_before_suspend == 0)
         {
           l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
         }
         else
         {
            l1_pscan_data_ptr->fn_before_suspend--;
         }
      }
      else
#endif
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH )
      {
        MSG_GERAN_MED_0_G("Dual Sim pscan suspended");
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      {
         L1_background_HPLMN_search_suspend_cb(gas_id);
      }
   }

#if      ( defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) )
   l1_ds_set_bplmn_state_to_null(gas_id);
#endif /* FEATURE_GSM_BACKGROUND_HPLMN_SEARCH & ( FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM  ) */

   l1_tskisr_blk->pscan_state = L1_SCAN_WAIT_TO_RESUME_MEAS_PERIOD;
}

/*===========================================================================

FUNCTION  L1_pscan_resume

DESCRIPTION
  This function resumes a suspended power scan

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_pscan_resume( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  if ( l1_tskisr_blk->pscan_state == L1_SCAN_WAIT_TO_RESUME_MEAS_PERIOD )
  {
     l1_tskisr_blk->pscan_state = L1_SCAN_RESUME_MEAS_PERIOD;
  }
  else
  {
     if ( l1_tskisr_blk->pscan_state == L1_SCAN_END )
     {
        /* Set this because if we suspend before we run L1_initial_acq the
         * abort_cb in NPL1 will point to the previous caller
         */
         if (gl1_msg_cfg_pwr_meas(L1_pscan_callback,L1_pscan_abort_callback,gas_id))
         {
            /* From scratch.   Kick off power scan */
            l1_tskisr_blk->pscan_state = L1_SCAN_START;
            l1_pscan_mcpm_change(TRUE, gas_id);
         }
         else
         {
            MSG_GERAN_ERROR_0_G("Pwr meas config not succesfull, pwr meas not intialised");
         }
     }
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH )
  {
    MSG_GERAN_MED_0_G("Multi-Sim pscan resume");
  }
  else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  if (l1_tskisr_blk->l1_state != L1_WTOG_BPLMN_MODE)
  {
     l1_set_background_HPLMN_search_suspended(FALSE, gas_id);
  }
}

/*===========================================================================

FUNCTION  l1_pscan_mcpm_change
DESCRIPTION
  This function requeste MCPM change at start/end of power scan so that
  clock speed is increased during power scan for RF processing load.

===========================================================================*/
void l1_pscan_mcpm_change( boolean pscan_start, gas_id_t gas_id )
{
   /* Ensure higher clock speed for power scan, required by RF driver */
   MSG_GERAN_HIGH_1_G( "MCPM change for PScan %d", pscan_start);
   gl1_clkdata_speed(pscan_start, L1_PSCAN_SPEEDUP, L1_DED_DATA_NULL, L1_PSCAN, gas_id);
}


void l1_pscan_complete(gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];

   MSG_GERAN_HIGH_0_G("Power Scan Complete ...");

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
   if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
   {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( gl1_msg_get_multi_sim_mode() )
      {
         l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_MULTI_SIM_ENDING, gas_id);
      }else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
         l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_ACTIVE, gas_id);
      }
      l1_set_wtog_bplmn_task_state(NO_HPLMN_SEARCH, gas_id);
   } else
#endif
   {
      L1_suspend_background_HPLMN_search(gas_id);
      l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
   }

   L1_send_ISR_SCAN_CNF(gas_id);
   l1_tskisr_blk->pscan_state = L1_SCAN_END;   /* end of power scan */

}

dBx16_T L1_pscan_get_threshold(gas_id_t gas_id)
{
  l1_pscan_data_t *pscan_ptr = &l1_pscan_data[gas_id];

  /* check whether BPLMN override is in effect */
  if (SYS_PARAM_MIN_ACCEPTABLE_PLMN_dBm_x16 == pscan_ptr->pwr_threshold)
  {
    return(SYS_PARAM_MIN_ACCEPTABLE_PLMN_dBm_x16);
  }

  return(SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16);
}

void L1_pscan_set_threshold(dBx16_T new_threshold, gas_id_t gas_id)
{
  l1_pscan_data_t *pscan_ptr = &l1_pscan_data[gas_id];

  pscan_ptr->pwr_threshold = new_threshold;

  return;
}


void l1_pscan_reset_freq_completed(gas_id_t gas_id)
{
  l1_pscan_data_t *l1_pscan_data_ptr = &l1_pscan_data[gas_id];
  l1_pscan_data_ptr->numFreqsCompleted  = 0;
}

