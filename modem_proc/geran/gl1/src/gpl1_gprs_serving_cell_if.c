/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1  S E R V I N G  C E L L  M E A S U R E M E N T S

GENERAL DESCRIPTION
    This module contains the processing required to perform serving cell
    measurements in both IDLE and TRANSFER modes. The modules in this file
    are the task related functions.


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who       what, where, why
--------   --------  ----------------------------------------------------------
21/08/17   sk        CR2093261 Dual space IMS meas data and report meas as state if TRM not available during previous DRX period
17/04/17   nm        CR2032010 make rf power class variable dual data spaced
07/06/16   sk        CR1028080 Utility to get GSM Serving cell measurements for IMS use
13/05/16   db        CR937805. DL BLER for GPRS and EGPRS DL TBF
12/08/15   cws       CR864883 FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
21/07/15   jk        CR864645:Data RxD ON/OFF changes for GCF TC 14.18.3 & 14.18.1
04/06/15   jk        CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
20/04/15   zc        CR826075 Band Specific ASDIV Enable
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs. 
29/01/15   cws       CR788745 [ASDIV]clear antenna measurments when turning off ASDIV algorithm
17/12/13   jj        CR 590806 correcting pointer overflows 
30/05/14   ws        CR657674 Data + MMS Phase 1
02/11/12   jj        CR 402579 Support of Tx Diversity in GERAN.
27/09/13   sp        CR551580: Add NV logging of outgoing L1 messages
26/03/13   ws        CR466973 - DSDS BU, Removed Async/SYNC code in
                     gpl1_gprs_serving_cell_meas_if()
15/02/13   ws        CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
11/06/12   tjw       CR367302 mainline FEATURE INTERRAT_CELL_RESELECTION_GTOW_GPRS52
09/09/11   ab        CR303214 Request for Configurable option for changing GSM Power Class value
06/08/10   ab        CR: 333225 - GCF RF 51.010-1: 22.8_8PSK. one step alpha
                     change results in lower PCL.
20/05/11   ws        Compile warnings fixed for Nikel
06/08/10   og        CR: 254291 - GPRS L1 Compliance to spec for ALPHA - Power 
                     Control Parameters
14/06/10   ap        CR: 235190 - 3GPP: Enhanced control of maximum output power in 
                     a common BCCH cell
11/06/10   dv        fix syntax error for previous check-in
08/06/10   dv        GP-031925/CR235566 - For DTM use SACCH TX power if C 
                     value not valid.
02/12/09   ws        Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                     Improvements
17/08/09   ap        CR190902 BER support needed for PS in 2G
26/01/09   og        Ensure rxqual calculations ignore CS4 radio blocks. Resolves
                     CR170214.
28/07/98   hv        Added KxMutex support
20/02/08   og        Fix TC22.8 v443 (R&S). Ensure invalid C values are filtered
                     out and that NDRX and cn_1 are set correctly. Resolves CR133528.
11/11/05   og        Lint 8.0 AMSS_std_critical_errors removal.
31/08/05   nt        Added the check for L1_DTM_MODE to the PROCESS_C_VALUE handler
                     to make sure that this code is executed in DTM mode.
02/06/05   og        Removal of header-header includes.
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
17/1/05    og        Extra interference/bep message debug added.
17/01/05   ws        Lint updates
30/11/04   og        Enable interference measurements when cuta is ON.
25/11/04   og        Add setting of invalid sign var when in egprs pkt transfer.
28/10/04   ws        Lint tidy up
27/09/04   ws        added ptcch_bad_snr_valid flag - Fix for CR 48942
17/09/04   ws        set alpha_cn to zero if we don't have a valid C value
26/05/04   sv        Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
23/06/04   og        Egprs Qual measurements handling.
28/04/04   rm        Added PTCCH function calls for accessing PTCCH params and
                     cleanup lint warnings
01/04/04   ws        fixed compile warnings
19/03/04   ws        Added support for bad snr detection on PTCCH/D decodes
27/01/04   kf        setting of band for ul power control
26/11/03   ws        Lint cleanup
11/06/03   gfr       Support for quad-band.
18/09/03   pjr       Lint tidy
17/09/03   pjr       Added trap in gpl1_gprs_build_quality_report for case when
                     quality_report_count is zero.
12/09/03   pjr       Lint tidy
20/08/03   pjr       Ensure gpl1_sc->gamma_gsm always valid in gpl1_gprs_calc_rf_pwr
                     function.
22/07/03   pjr       Modified Alpha values to pass RF power tests.
09/07/03   pjr       Further modifications to use of ALPHA value.
08/07/03   pjr       Modified resolution of ALPHA value.
04/07/03   npr       Lint warning cleanup.
23/06/03   gfr       Compiler warning cleanup.
19/06/03   pjr       Added rrbp frame number to the quality report.
14/05/03   pjr       Modified gpl1_gprs_build_quality_report to only add
                     interference measurements when in transfer mode.
14/05/03   pjr       Modified QUALITY REPORT TIME to handle multiple timeslots
16/04/03   sw        Added initial changes for WCDMA neighbour cell search
                     during packet idle on PCCCH
20/03/03   pjr       Minor update the debug message to make more meaningful
12/03/03   pjr       Removed local function gprs_calc_rxqual_lev and changed call
                     to use function gpl1_gprs_calc_rxqual_lev.
27/02/03   pjr       Added setting of l1_idle_data.campedon_cell_agc.pwr_dBm_x16
05/02/03   ws        Adjusted for scheduling change and data cb frame change
10/01/03   pjr       Additional rf power calculation changes for dual band.
08/01/03   pjr       Modified rf power calculation for dual band.
12/06/02   pjr       Removed debug.
11/25/02   pjr       Added missing case 1 to gpl1_gprs_metrics_retrival function
11/22/02   pjr       Modified convert_pwr_lvl to use GSM5.05 values
11/11/02   pjr       Changed max power level.
11/01/02   pjr       Created.


===========================================================================*/

#include "geran_variation.h"
#include "customer.h"


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "l1_task.h"
#include "gl1_msg_g.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "l1_isr.h"
#include "gpl1_gprs_isr.h"
#include "task.h"
#include "gpl1_gprs_task.h"
#include "ms.h"
#include "l1_utils.h"
#include "gmacl1.h"
#include "l1_sc.h"
#include "gpl1_gprs_ptcch.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif

#ifdef PL1_FW_SIM

/* set test harness signal */
#include "pl1_test_harness.h"

#endif

#include "l1_ds.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/* Local Function prototypes */
static uint8  convert_pwr_lvl( int32       pwr_out,
                               sys_band_T  band,
                               gas_id_t gas_id);
static uint8 translate_mean_bep_to_rxqual(uint8 mean_bep);
static uint8 calculate_mean_bep( gas_id_t gas_id );

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern void gl1_hw_clear_antenna_meas (gas_id_t gas_id);
#endif

/* Arrays for converting powers as per 5.05, values are dBm x 16 */
/* GSM 900 band */
static int16 convert_pmax_900[] =
  {624,624,624,592,560,528,496,464,432,400,368,336,304,272,240,208,
   176,144,112,80,80,80,80,80,80,80,80,80,80,80,80,80};

/* Aim is to have a threshold midpoint of 1 db in choosing the PCL */
/* A step variation (0.1) in alpha with a fixed C value would result in
** small -alpha (C+48) change , but should not lower the PCL resulting
** transmission lower bt 2 dBm.Use 1 db above/below dynamic range to choose
** PCL
*/
static int16 convert_pmax_900_dynamic[] =
  {608,608,608,576,544,512,480,448,416,384,352,320,288,256,224,192,
   160,128,96,64,64,64,64,64,64,64,64,64,64,64,64,64};

/* DCS 1800 band */
static int16 convert_pmax_1800[] =
  {480,448,416,384,352,320,288,256,224,192,160,128,96,64,32,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,576,544,512};

/* Aim is to have a threshold midpoint of 1 db in choosing the PCL */
/* A step variation (0.1) in alpha with a fixed C value would result in
** small -alpha (C+48) change , but should not lower the PCL resulting
** transmission lower bt 2 dBm.Use 1 db above/below dynamic range to choose
** PCL
*/
static int16 convert_pmax_1800_dynamic[] =
  {464,432,400,368,336,304,272,240,208,176,144,112,80,48,16,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,560,528,496};

/* GSM 850 band is the same as 900 */
/* PCS 1900 band */
static int16 convert_pmax_1900[] =
  {480,448,416,384,352,320,288,256, 224,192,160,128,96,64,32,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,528,512};

/* Aim is to have a threshold midpoint of 1 db in choosing the PCL */
/* A step variation (0.1) in alpha with a fixed C value would result in
** small -alpha (C+48) change , but should not lower the PCL resulting
** transmission lower bt 2 dBm.Use 1 db above/below dynamic range to choose
** PCL
*/
static int16 convert_pmax_1900_dynamic[] =
  {464,432,400,368,336,304,272,240, 208,176,144,112,80,48,16,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,520,496}; /* PCL 30, 31 , mid point 0.5 db */


/* Table for max pcl for each band power class */ 
/* [num_bands][pwr_class][max_pcl] */
/*
*/
uint8  gl1_max_pcl_pwr_class[5][5][2] =
      {
         { /* SYS_BAND_EGSM_900 */
            {0,5}, /* CLASS_1 Invalid Default */
            {1,2}, /* CLASS_2 39dBm */
            {2,3}, /* CLASS_3 37dBm */
            {3,5}, /* CLASS_4 33dBm */
            {4,7}, /* CLASS_5 29dBm */
         },
         {  /* SYS_BAND_PGSM_900 */
            {0,5}, /* CLASS_1 Invalid Default */
            {1,2}, /* CLASS_2 39dBm */
            {2,3}, /* CLASS_3 37dBm */
            {3,5}, /* CLASS_4 33dBm */
            {4,7}, /* CLASS_5 29dBm */
         },
         {  /* SYS_BAND_PCS_1900 */
            {0,0},  /* CLASS_1 30dBm */
            {1,3},  /* CLASS_2 24dBm */
            {2,30}, /* CLASS_3 33dBm */
            {3,0},  /* CLASS_1 Invalid Default */
            {4,0},  /* CLASS_1 Invalid Default */
         },
         {  /* SYS_BAND_PCS_1800 */
            {0,0},  /* CLASS_1 30dBm */
            {1,3},  /* CLASS_2 24dBm*/
            {2,29}, /* CLASS_3 36dBm */
            {3,0},  /* CLASS_1 Invalid Default */
            {4,0},  /* CLASS_1 Invalid Default */
         },
         {  /* SYS_BAND_CELL_850 */
            {0,5}, /* CLASS_1 Invalid Default */
            {1,2}, /* CLASS_2 39dBm */
            {2,3}, /* CLASS_3 37dBm */
            {3,5}, /* CLASS_4 33dBm */
            {4,7}, /* CLASS_5 29dBm */
         },
      };

extern dedicated_data_T l1_dedicated_data[];

extern const uint32  GPRS_PL1_GMSK_MEAN_BEP_LOG_SCALE[ NUM_MEAN_BEP_VALUES ];
extern const uint32  GPRS_PL1_8PSK_MEAN_BEP_LOG_SCALE[ NUM_MEAN_BEP_VALUES ];

static const egprs_mean_bep_store_T egprs_mean_bep_store_reset =
{ 0,0,0,0,0,0 };

extern byte L1_compute_next_txlev( byte    demanded_power_level,
                                   byte    power_level_in_use,
                                   ARFCN_T arfcn );
extern dBx16_T serving_RLA_C;
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern gl1_hw_asdiv_type1_T gl1_hw_asdiv_type1;
extern int16 tx_diversity_threshold ;
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
static rx_qual_T pdch_rxqual_report[NUM_GERAN_DATA_SPACES];

/* RF power class per band */
extern byte   gl1_rf_power_class_per_band[NUM_GERAN_DATA_SPACES][5];

#define RXQUAL_INVALID 0xFF;

extern gpl1_serving_cell_meas_t gpl1_serving_cell_meas[];

/*===========================================================================

FUNCTION  gpl1_gprs_serving_cell_if

DESCRIPTION
  This function manages the serving cell measurements task level operation.
  It will be event driven. The events include signals generated by the isr function
  gpl1_gprs_serving_cell.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_serving_cell_meas_if(gas_id_t gas_id)
{
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

  uint8 local_task_event;
  uint8 action_to_process = 1;

  /*  Setup a local pointer to the serving cell measurements  */
  gprs_pl1_serving_cell_meas_T  *gpl1_sc = gprs_serv_cell_meas_ptr;
  pccch_power_control_params_t  *gpl1_pc = &gprs_l1_idle_data->gprs_campedon_cell_data.power_control;

  /*  This function is event driven. The events can take the form of a signal
      generated by the isr and an associated task event word indicting the
      action to take.  */
#ifdef PL1_FW_SIM
  /* set test harness signal */
  (void)rex_clr_sigs(&tcb_pl1, SERV_MEAS_ISR_SIG);
#else
  (void)rex_clr_sigs( l1_task_tcb_read(gas_id), SERV_MEAS_ISR_SIG);
#endif

  /*  Action determined by the setting of the task event work. This word is
      a bit mapped uint8  */
  /*  As this word is modified by the ISR, disable interrupts and take a local
      copy. */
  GL1_ISR_LOCK(gas_id);

    local_task_event = gpl1_sc->task_event;

  /*  Reset task_event  */
  gpl1_sc->task_event = 0;
  GL1_ISR_UNLOCK(gas_id);

  /*  As the task event word is reset all actions in the local_task_event
      have to be actioned this time round */
  do
  {
    /*  Process pending actions, bit position determines action  */
    if (local_task_event & action_to_process)
    {
      switch (action_to_process)
      {
        case  PROCESS_C_VALUE:
        {
          uint16  forgetting_factor;
          int16   cblockn;

          /*  Ssblockn or SSn has been calculated and the block has decoded correctly
              so now calculate the c_value. */
          /*  Determine which measurement and calculations need to be performed. This
              decision is based on the value of pc_meas_chan and whether the state is
              IDLE or TRANSFER. */
#ifdef FEATURE_GSM_DTM
          if ( (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) ||
               (l1_tskisr_blk->l1_state == L1_DTM_MODE)
             )
#else
          if (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
#endif
          {
            /*  Transfer state, now determine value of pc_meas_chan */
            if( PC_MEAS_CHAN_ON_PDCH == gpl1_pc->pc_meas_chan)
            {
              /*  Power measurements made on PDTCH in transfer mode so use forgetting
                  factor c, (See 05.08 sect. 10.2.3).
                  The value cblockn is determined by adding the Pb value to the
                  Ssblockn value. If frequency hopping is in progress the value
                  of Pb is reduced by 25% for every burst received on the BCCH */
              cblockn = (int16)(gpl1_sc->ssblockn + gpl1_sc->weighted_pb);

              /*  Determine forgetting factor '1/c' */
              /*  Value passed in for t_avg_t is the 'k' in 2^(k/2) / 6.
                  For k = 0, .., 25  */
              /*  Range check k */
              if (gpl1_pc->t_avg_t >= 25)
              {
                gpl1_pc->t_avg_t = 25;
              }

              /*  So forgetting factor = 12 * t_avg_t equates to */
              forgetting_factor = (1 << ((gpl1_pc->t_avg_t >> 1) + 1));

              /*  Increment sample count  */
              gpl1_sc->c_index++;

            }
            else
            {
              /*  Power measurements made on BCCH in transfer mode so use forgetting
                  factor b  */
              cblockn = gpl1_sc->power_cntrl_avg;

              /*  Determine forgetting factor '1/b' */
              /*  Value passed in for t_avg_t is the 'k' in 2^(k/2) / 6.
                  For k = 0, .., 25  */
              /*  Range check k */
              if (gpl1_pc->t_avg_t >= 25)
              {
                gpl1_pc->t_avg_t = 25;
              }

              /*  So forgetting factor = 6 * t_avg_t equates to */
              forgetting_factor = (1 << (gpl1_pc->t_avg_t >> 1));

              /*  Increment sample count  */
              gpl1_sc->c_index++;

            }
          }
          else
          {
            /*  Treat as IDLE state.  */
            int32  t_avg_w;
            int32  t_avg_w_ndrx;

            /*  Measurements made on PDTCH in transfer mode so use forgetting
                factor a. The value cblockn is determined by adding the Pb value
                to the Ssblockn value. If frequency hopping is in progress the
                value of Pb is reduced by 25% for every burst received on the BCCH */
            cblockn = (int16)(gpl1_sc->ssblockn + gpl1_sc->weighted_pb);

            /*  Increment sample count  */
            gpl1_sc->c_index++;

            {

              /* Take the paging mode into consideration */
              switch( l1_idle_data->paging_data.paging_mode )
              {
              case  PAGING_REORGANIZATION:

                if (l1_idle_data->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE)
                {
                   gpl1_sc->N_DRX = (9 - l1_idle_data->campedon_cell_data.BS_AG_BLKS_RES);
                }
                else
                {
                   gpl1_sc->N_DRX = (3 - l1_idle_data->campedon_cell_data.BS_AG_BLKS_RES);
                }

                break;

              default:
                /* No intention to use fixed point arithmetic here and hence dividing by
                 * bs_pa_mfrms is avoided.
                 * It is easier to cap the minimum value at 1 instead.
                 */
                gpl1_sc->N_DRX = 1;

                break;
              }

            }

            /*  Determine t_avg_w in Q8 format, t_avg_w = 2^(k/2) / 6
                For k = 0, .., 25  */
            /*  Range check k */
            if (gpl1_pc->t_avg_w >= 25)
            {
              gpl1_pc->t_avg_w = 25;
            }

            /*  Determine forgetting factor '1/a', which is given by:
                    MIN(n, MAX(5, t_avg_w * N_DRX)  */
            t_avg_w = (1 << ((gpl1_pc->t_avg_w >> 1) + 8)) / 6;

            /*  Now multiply by N_DRX and then
                shift to format as normal number (No Q-format) */

            /*lint -e(704) sign not lost during shift */
            t_avg_w_ndrx = (t_avg_w * gpl1_sc->N_DRX) >> 8;

            forgetting_factor = (uint16)(MAX(5, t_avg_w_ndrx));
            forgetting_factor = (uint16)(MIN(gpl1_sc->c_index, forgetting_factor));
          }

          if( 0 == cblockn )
          {
            /* Check cn_1, if it has a valid value then use it instead of
             * the zero cblockn value.
             */
            if( 0 != gpl1_sc->cn_1 )
            {
              cblockn = (int16)(gpl1_sc->cn_1 >> 8);
            }
            else
            {
              /* Do not update the c value filter at all.
               * Remove process c value bitmap and continue
               * with the while( ) loop.
               */
              local_task_event ^= PROCESS_C_VALUE;

              /* Decrement the sample count and cap it at zero
               * since it is an unsigned value
               */
              if( 0 != gpl1_sc->c_index )
              {
                gpl1_sc->c_index -= 1;
              }

              MSG_GERAN_ERROR_0_G(" zero cblockn and cn_1 ");

              /* Warning, do not replace with a break statement. */
              continue;
            }
          }
          /* If cn_1 is zero then we should not set it because we want
           * the 3GPP 45.008 filter to take effect correctly as in
           * C1 = (1-a)xC(0) + (a)x(Cblockn)
           * Where C(0) is zero.
           */

          /*  Obtain value of Cn. Note:- within the function a greater precision
              record of cn is kept to be used as Cn-1 next time round. */
          gpl1_sc->cn = gpl1_gprs_divide_ff(cblockn, &(gpl1_sc->cn_1), forgetting_factor);

          /*  Use the cn value to calculate the RF output power.  */
          /*  Power calculation:- Pch = MIN(GAMMA_GSM - GAMMA_TN - alpha * (Cn + 48), PMAX)
              GAMMA_0 is 39dBm for GSM and 36dBm for DCS.
              If no valid GAMMA_TN is present it will be set to zero.
              PMAX is set to GPRS_MS_TXPWR_MAX_CCH if PBCCH exists,
              or MS_TXPWR_MAX_CCH otherwise.  */
          gpl1_gprs_calc_rf_pwr( gas_id);

          /*  Clear action  */
          local_task_event ^= PROCESS_C_VALUE;

          break;
        }

        case  PROCESS_RXQUAL_SIGN_VAR:
        {
          /*  Calculate the BL_VAR and update SIGN_VAR  */
          /*  If frequency hopping is used, BL_VAR is only calculated if
              either at least 3 of the 4 bursts are not received on the BCCH
              frequency, or, at least 3 of the four bursts are received on the
              BCCH frequency. */
          /*  If two bursts have been received on the BCCH then do not perform
              variance calculation. This will be indicated by the metrics
              retrival function returning NULL */
          block_rssi_t *rssi_buffer_ptr;

          if( gpl1_sc->rssi_buffer_ptr->last_burst_received < L1_BURST_4 )
          {
              /* Use the previous buffer */
              if( &(gpl1_sc->block_rssi[BUFFER_1]) == gpl1_sc->rssi_buffer_ptr )
              {
                  rssi_buffer_ptr = &(gpl1_sc->block_rssi[(NUM_RSSI_BUFFERS - 1)]);
              }
              else
              {
                  rssi_buffer_ptr = gpl1_sc->rssi_buffer_ptr - 1;
              }
          }
          else
          {
              /* Use the pointer to the current block rssi buffer */
              rssi_buffer_ptr = gpl1_sc->rssi_buffer_ptr;
          }

          switch( rssi_buffer_ptr->num_on_bcch )
          {
          case  0:
          case  1:
          {
              int32  ssblockn = 0;
              int32  sum      = 0;
              int8  index_j   = (int8) (GL1_DEFS_FRAMES_IN_BLK_PERIOD - rssi_buffer_ptr->num_on_bcch);

              int8 index_k = 0;
              while( index_k < GL1_DEFS_FRAMES_IN_BLK_PERIOD )
              {
                if( FALSE == rssi_buffer_ptr->burst_on_bcch[ index_k ] )
                {
                    ssblockn += rssi_buffer_ptr->burst_ss[ index_k ];
                }
                index_k++;
              }

              ssblockn /= index_j;

              index_k = 0;
              while( index_k < index_j )
              {
                if( FALSE == rssi_buffer_ptr->burst_on_bcch[ index_k ] )
                {
                  int32  temp_sum = rssi_buffer_ptr->burst_ss[ index_k ] - ssblockn;

                  temp_sum *= temp_sum;
                  temp_sum >>= 4; /*lint !e704 sign not lost during shift */

                  sum += temp_sum;
                }
                index_k++;
              }

              if( index_j > 1 )
              {
              sum /= (index_j - 1);
              }
              else
              {
                MSG_GERAN_ERROR_0_G(" division by zero avoided ");
              }

              if( gpl1_sc->sign_var )
              {
                gpl1_sc->sign_var += sum;

                gpl1_sc->sign_var >>= 1; /*lint !e704 sign not lost during shift */
              }
              else
              {
                gpl1_sc->sign_var = sum;
              }
          }
              break;

          case  2:
              /* Two bursts received on BCCH. Discard the block */
              break;

          case  3:
          case  4:
          {
              int32  ssblockn = 0;
              int32  sum      = 0;
              int8   index_j  = (int8) rssi_buffer_ptr->num_on_bcch;

              int8  index_k = 0;
              while( index_k < GL1_DEFS_FRAMES_IN_BLK_PERIOD )
              {
                if( FALSE != rssi_buffer_ptr->burst_on_bcch[ index_k ] )
                {
                    ssblockn += rssi_buffer_ptr->burst_ss[ index_k ];
                }
                index_k++;
              }

              ssblockn /= index_j;

              index_k = 0;
              while( index_k < GL1_DEFS_FRAMES_IN_BLK_PERIOD )
              {
                if( FALSE != rssi_buffer_ptr->burst_on_bcch[ index_k ] )
                {
                  int32  temp_sum = rssi_buffer_ptr->burst_ss[ index_k ] - ssblockn;

                  temp_sum *= temp_sum;
                  temp_sum >>= 4; /*lint !e704 sign not lost during shift */

                  sum += temp_sum;
                }
                index_k++;
              }

              sum /= (index_j - 1);

              if( gpl1_sc->sign_var )
              {
                gpl1_sc->sign_var += sum;

                gpl1_sc->sign_var >>= 1; /*lint !e704 sign not lost during shift */
              }
              else
              {
                gpl1_sc->sign_var = sum;
              }
          }
              break;

          default:
              break;
          }

          /*  Clear action  */
          local_task_event ^= PROCESS_RXQUAL_SIGN_VAR;

          break;
        }

        case  QUALITY_REPORT_TIME:
        {

          gpl1_gprs_build_quality_report(gas_id);
          gpl1_calc_gprs_egprs_dl_bler( TBF_MODE_GPRS, gas_id );

          /*  Clear action  */
          local_task_event ^= QUALITY_REPORT_TIME;


          break;
        }

        case  RLA_P_REPORT_TIME:
        {
          mph_serving_meas_ind_t  message_buffer, *message;
          uint32                  message_length;
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING 
          uint8 rxlev_average;
#endif
          gprs_pl1_serving_cell_meas_T*  gpl1_sc_ptr = gprs_serv_cell_meas_ptr;
          rx_qual_T*              rxqual_ptr = &( gpl1_sc_ptr->rx_qual );
          /*  Time to report the RLA_P values collected in the receive callback
              to GRR  */
            

#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
            if (gl1_get_IMRD_ctl_nv(gas_id) 
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                || gl1_get_MSRD_ctl_nv(gas_id)
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
                ) 
            {
              gpl1_sc->rla_p_reported_value = MAX(gpl1_sc->rla_p_isr_avg, gpl1_sc->rla_p_isr_avg_divrx);              
            }
            else
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
            {
              gpl1_sc->rla_p_reported_value = gpl1_sc->rla_p_isr_avg;
            }

          /*  WCDMA measurement scheduler checks "serving_RLA_C"
              against some network-supplied paramaters to determine whether
              WCDMA measurements should be performed or not.

              ==> Copy in from RLA_P during Packet Idle/52     */
          l1_set_serving_RLA_C(gpl1_sc->rla_p_reported_value, gas_id);

          /*  Message format :-
                              IMH_T    message_header
                              byte     rxlev_average
                              boolean  i_level_valid
                              byte     i_level        */

          /* Align the pointers */
          message = &message_buffer;

          message->message_header.message_set = MS_RR_L1;
          message->message_header.message_id  = (uint8)MPH_SERVING_MEAS_IND;
          message_length = sizeof(mph_serving_meas_ind_t) - sizeof(IMH_T);
          PUT_IMH_LEN(message_length,&message->message_header);
          message->gas_id = gas_id;

          /* Load the mapped RXLEV value */
          message->rxlev_average = L1_map_dBm_x16_to_rxlev(gpl1_sc->rla_p_reported_value);
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING 
          MSG_GERAN_HIGH_3_G(" type1_enable =%d ", antenna_swith_tx_div_nv_enable,0,0);
          if (  gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE1) )
          { 
            rxlev_average = L1_map_dBm_x16_to_rxlev(gpl1_sc->rla_p_isr_avg);
            if ( rxlev_average < tx_diversity_threshold )
            {
              gl1_hw_asdiv_type1.start_alg = TRUE;  
            }
            else
            {
              gl1_hw_asdiv_type1.start_alg = FALSE;
		      gl1_hw_clear_antenna_meas (gas_id);
              MSG_GERAN_HIGH_2_G("start asdiv type1 algorithm = %d message->rxlev_average =%d ",gl1_hw_asdiv_type1.start_alg,message->rxlev_average);
            } 
          }         
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING     */ 
          if(gpl1_gprs_get_ptcch_snr_valid(gas_id))
          {
            /* Load SNR low level detection if TRUE RR may use this to gate reslection */
            message->SNR_is_bad = gpl1_gprs_get_ptcch_snr_status(gas_id);
            message->SNR_is_bad_valid = TRUE;

            /* now clear out ptcch_snr_bad valid flag */
            gpl1_gprs_reset_ptcch_snr_valid(gas_id);

            if (message->SNR_is_bad)
            {
              MSG_GERAN_ERROR_0_G("++SNR is bad++");
            }

          }else
          {
            /* SNR flag is not valid so clear SNR_is_bad_valid flag */
            message->SNR_is_bad_valid = FALSE;
            message->SNR_is_bad = FALSE;

          }




          if( gpl1_sc->serv_cell_int )
          {
            /* Map the interference signal level into an RXLEV (0-63) value
             * since it is used in building the Packet Measurement Report.
             * Warning: The size reserved for the i-level is 6-bits and hence
             * it should not be reported relative to C value (0-15).
             */
            message->i_level = (uint8)
                L1_map_dBm_x16_to_rxlev( (dBx16_T) (gpl1_sc->serv_cell_int) );

            /* Mark measurement as present */
            message->i_level_valid = TRUE;

            /* Clear the current serving cell interference measurement */
            gpl1_sc->serv_cell_int = 0;
          }
          else
          {
            /* Mark measurement as not present */
            message->i_level_valid = FALSE;
          }


          message->rxqual = RXQUAL_INVALID;
          gpl1_serving_cell_meas[gas_id].rxqual = RXQUAL_INVALID;
          if( ( L1_TRANSFER_MODE == l1_tskisr_blk->l1_state )
            &&( TBF_MODE_GPRS == l1_transfer_data[gas_id].egprs_or_gprs_tbf ) )
          {
            /*  Rxqual   */
            /*  Trap for divide by zero */
            if( 0 != pdch_rxqual_report[gas_id].count )
            {
              pdch_rxqual_report[gas_id].value+=rxqual_ptr->value;
              pdch_rxqual_report[gas_id].count+=rxqual_ptr->count;
              message->rxqual = gpl1_gprs_calc_rxqual_lev( pdch_rxqual_report[gas_id].value,
                                                 pdch_rxqual_report[gas_id].count,gas_id );
              /* reset reporting values*/
              pdch_rxqual_report[gas_id].value =0;
              pdch_rxqual_report[gas_id].count =0;
            }else if( 0 != rxqual_ptr->count )
            {
               message->rxqual = gpl1_gprs_calc_rxqual_lev( rxqual_ptr->value,
                                                 rxqual_ptr->count,gas_id );
            }
          }
          else if( ( L1_TRANSFER_MODE == l1_tskisr_blk->l1_state )
            &&( TBF_MODE_EGPRS == l1_transfer_data[gas_id].egprs_or_gprs_tbf ) )
          {
              message->rxqual = translate_mean_bep_to_rxqual( calculate_mean_bep(gas_id) );
          }

          gpl1_serving_cell_meas[gas_id].rxqual = message->rxqual;

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
         /*Data RxD will be turned OFF if PRx only stream selection for 70% of the bursts received in a measurement period.
          * Once Data RxD is OFF due to above reason, it will be OFF for 20 measurement periods. At the end of 20th measurement 
          * period  gl1_disable_rxdiversity_data will be reset and BEP based dynamic RxD algorithm will run. If in any measurement period 
          * PRx only stream selection is reported for 70% of the bursts, Data RxD will be tunred OFF for another 20 measurement periods 
          * and the cycle repeates. These changes were mainly done to help single antenna GCF TCs 14.18.3 & 14.18.1 to pass. */

         if(gl1_get_MSRD_ctl_nv(gas_id))
         {
           if(gl1_get_disable_data_diversity(gas_id) == FALSE)
           {
             if(gl1_get_ps_divrx_stream_sel_rate(gas_id))
             {
               gl1_set_disable_data_diversity(TRUE, gas_id);
               gl1_packet_meas_period_cnt(TRUE,gas_id);
             }
             else
             {
               gl1_set_disable_data_diversity(FALSE, gas_id);
             }
           }
           else
           {
             if(gl1_get_packet_meas_period_cnt(gas_id) >= GL1_PACKET_MEAS_PERIOD_THRESHOLD)
             {
               gl1_set_disable_data_diversity(FALSE, gas_id);
               gl1_packet_meas_period_cnt(FALSE,gas_id);
             }
             else
             {
               gl1_packet_meas_period_cnt(TRUE,gas_id);
             }
           }
           gl1_reset_ps_divrx_sel_avg(gas_id);
         }
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
 
          /* Send the message */
          (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

          MSG_GERAN_MED_3_G("PL1 - Sent MPH_SERVING_MEAS_IND%d %d %d RXL, tx pwr", gpl1_sc->rla_p_reported_value, message->rxlev_average,gpl1_sc->pch[4]);
          /*  Clear action  */
          local_task_event ^= RLA_P_REPORT_TIME;


          break;
        }

        default:
        {
          /*  Unhandled event detected  */
          MSG_GERAN_ERROR_1_G("PL1: Unhandled Serv Cell Meas event %d",action_to_process);

          /*  Clear event bit */
          local_task_event ^= action_to_process;

          break;
        }

      } /*  End of event switch */

    } /*  End of if event */

    /*  Shift to next action.  */
    action_to_process <<= 1;

  } while (local_task_event);

}



/*===========================================================================

FUNCTION  gpl1_gprs_build_quality_report

DESCRIPTION
  This function is called when the quality report needs to be built and sent
  to MAC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_build_quality_report(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  gprs_pl1_serving_cell_meas_T*  gpl1_sc_ptr = gprs_serv_cell_meas_ptr;

  rx_qual_T*     rxqual_ptr = &( gpl1_sc_ptr->rx_qual );
  uint8          reported_rxqual = 7;

  i_level_t*     int_lev_ptr;
  uint8          index;

  l1_mac_sig_t   message_buffer;
  l1_mac_sig_t*  message = &( message_buffer );

  /*  Collect measurements required for the Quality report  */

  /*  SIGN VAR  */

  /*  Convert measured sign_var into the value to be reported. Reported
      values in steps of 0.25db. So to calculate the value to report,
      determine the number of 0.25 steps in sign_var. Note sign_var
      in Q4 format  */

  if( ( L1_TRANSFER_MODE == l1_tskisr_blk->l1_state )
    &&( TBF_MODE_EGPRS == l1_transfer_data[gas_id].egprs_or_gprs_tbf ) )
  {
    /* Sign var is not reported during EGPRS UL packet transfer */
    message->msg.qual_measurements.sign_var = GMAC_INVALID_SIGN_VAR;
  }
  else
  {

    int32  sign_var        = gpl1_sc_ptr->sign_var;
    uint8  report_sign_var = 0;

    do
    {
      /*  0x04 in Q4 format equates to 0.25 */
      sign_var -= 0x04;
      report_sign_var++;

      /*  Reported sign var has a maximum value of 0x3f */
    } while ((sign_var > 0) && (report_sign_var < 0x40));

    /*  Report_sign_var will be one too many. */
    report_sign_var--;

    message->msg.qual_measurements.sign_var = report_sign_var;

  }

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = L1_MAC_QUAL_MEASURMENTS;

  /*  Rxqual  */
  /*  Trap for divide by zero */
  if( 0 != rxqual_ptr->count )
  {
    reported_rxqual = gpl1_gprs_calc_rxqual_lev( rxqual_ptr->value,
                                                 rxqual_ptr->count,gas_id );
    pdch_rxqual_report[gas_id].value += rxqual_ptr->value;
    pdch_rxqual_report[gas_id].count += rxqual_ptr->count;
    rxqual_ptr->count = 0;
  }
  else
  {
    /* This case means that either:
     * All the radio blocks received were CS4 blocks.
     * Or
     * None of the radio blocks received were decoded correctly.
     * In either case the reported value must be set to 7.
     */
    
  }

  /*  Reset sum and count values  */
  rxqual_ptr->value = 0;

  /*  C-VALUE */
  /*  Convert C-value into rxlev 6-bit values */
  gpl1_sc_ptr->reported_rxlev = L1_map_dBm_x16_to_rxlev( (dBx16_T) (gpl1_sc_ptr->cn) );

  message->msg.qual_measurements.c_value = (uint8) gpl1_sc_ptr->reported_rxlev;
  message->msg.qual_measurements.rxqual = reported_rxqual;

  /*  Interference Measurements.  */
  int_lev_ptr = &(message->msg.qual_measurements.i_lev_meas.i_lev_tn0);

  for (index = 0; index < GL1_DEFS_SLOTS_IN_FRAME; index++)
  {
    dBx16_T   int_dbm;
    int8      rxlev_diff;

    /*  Mark measurement as not present and over write as neccessary  */
    int_lev_ptr->i_lev_ind = FALSE;

    /*  Determine which channels have had at least one measurement on */
    /*  Currently only add interference measurements when in transfer mode. */
    if(  ( gpl1_sc_ptr->int_array[index].valid_ind )
       &&( l1_tskisr_blk->l1_state == L1_TRANSFER_MODE ) )
    {
      /*  Build message */
      int_lev_ptr->i_lev_ind = TRUE;

      /*  Convert the measured C-value into the report value. */

      /*lint -e(704) sign not lost during shift */
      int_dbm = L1_map_dBm_x16_to_rxlev( (dBx16_T) ( (gpl1_sc_ptr->int_array[index].meas_1 >> 8) ) );

      /*  Clear the measurement values and count to reset after reporting */
      gpl1_sc_ptr->int_array[index].meas_1 = 0;
      gpl1_sc_ptr->int_array[index].measurement_count = 0;
      gpl1_sc_ptr->int_array[index].valid_ind = FALSE;

      /*  Reported value is a 4 bit representation of the relative value
          of the intereference measurement and the C-value  */
      rxlev_diff = (int8) (gpl1_sc_ptr->reported_rxlev - int_dbm);

      if (rxlev_diff >= 0)
      {
        /*  Divide by two and add 1 to give reported value  */
        rxlev_diff >>= 1; /*lint !e702 sign not lost during shift */
        rxlev_diff += 1;

        /*  Range check, max. value 15  */
        if (rxlev_diff > 15)
        {
          rxlev_diff = 15;
        }
      }
      else
      {
        /*  Interference is greater than C - value  */
        rxlev_diff = 0;
      }

      /*  Include in message  */
      int_lev_ptr->i_lev_tn = (uint8)rxlev_diff;

#if  DEBUG_GPL1_INTERF_MEAS
      #error code not present
#endif

    }

    if(index < (GL1_DEFS_SLOTS_IN_FRAME -1))
    {
    int_lev_ptr++;
  }
  }

  /*  Add frame number of associated rrbp */
  message->msg.qual_measurements.rrbp_fn = gpl1_sc_ptr->qrt;

  message->msg.qual_measurements.gas_id = gas_id;

  /*  It is possible for a message to be sent by the interrupt and for
      the task to try and re-send. This is avoided by ensuring the
      quality_report_count is non-zero.  */
  if( 0 != gpl1_sc_ptr->quality_report_count )
  {
    MSG_GERAN_MED_3_G( "PL1 - Sent QUAL REPORT %d %d",
             gl1_get_FN(gas_id),
             gpl1_sc_ptr->quality_report_count,
             0 );
    do
    {
      /* Send the message, NULL used for MAC_PH_DATA_IND */
      L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);

    }while( --(gpl1_sc_ptr->quality_report_count) );
  }
  else
  {
    MSG_GERAN_MED_3_G( "Tried to send quality report with zero count QRFN %d",
             gpl1_sc_ptr->qrt,
             0,
             0);
  }

}



/*===========================================================================

FUNCTION    gpl1_gprs_divide_ff

DESCRIPTION
  This function determines the value Xn in equations of the type

    ff * Xn = (ff - 1) * Xn-1 + input_value.

  The value Xn is determined given the value ff * Xn without
  performing a divide. Assumes the value of Xn will be negative. Xn lies in
  the range -110 to -48 dBm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

int16  gpl1_gprs_divide_ff(int16 input_value, int32 *prev_xn, uint16 ff)
{
  /*  This shift value is required as the arithmetic in this file is done in
      Q8 format. i.e. 8 decimal places, so as to maintain precsion. */
  #define SHIFT_VALUE   8
  #define SIGN_EXT_SHIFT_VALUE   16
  #define MINUS_48      (int32)0xfffd0000
  #define MINUS_79      (int32)0xfffb1000
  #define MINUS_110     (int32)0xfff92000
  #define HALF_VALUE_16 (int32)0x00010000

  int32   xn = MINUS_79;
  int32   cal_value, half_value = HALF_VALUE_16;
  int32   temp_xn;
  int32   scaled_input;

  /*  Scale input values to uint32, with the most significant 24 bits
      representing the integer part and the least 8 bits the decimal part. */
  /*lint -e(701) sign not lost during shift */
  scaled_input = input_value << SIGN_EXT_SHIFT_VALUE;
  /*lint -e(701) sign not lost during shift */
  scaled_input = scaled_input >> (SIGN_EXT_SHIFT_VALUE - SHIFT_VALUE);

  /*  Determine xn  */
  temp_xn = *prev_xn * (ff - 1);
  temp_xn += scaled_input;

  /*  Trap case when forgetting factor has not been set */
  /*  Result determined by multiplying values until the result is within an
      acceptable error of the required value. Do not enter this loop if
      forgetting factor is zero. More efficient than using divide  */
  if (ff)
  {
    do
    {
      /*  ff is not scaled as always an integer and scaling and multiplying
          would require right shifting afterwards.  */
      cal_value = xn * ff;

      /*  Use binary search to converge on answer quickly */
      if (cal_value != temp_xn)
        (cal_value < temp_xn) ? (xn += half_value) : (xn -= half_value);

      /*  Halve the half value  */
      /*lint -e(704) sign not lost during shift */
      half_value >>= 1;

    }while ((cal_value != temp_xn) && (half_value));

  }
  else
  {
    MSG_GERAN_ERROR_0("PL1: Serv Cell Meas forgetting fact. 0 ");
  }


  /*  Store newly calculated value in previous store for next time.
      Stored in q format with 24msb representing the integer part */
  *prev_xn = xn;

  /*  Limit reported value to range -48 to -110 */
  if (xn < MINUS_110)
  {
    xn = MINUS_110;
  }

  if (xn > MINUS_48)
  {
    xn = MINUS_48;
  }

  /*  Return the current xn value as an integer, rounded down */
  /*lint -e(704) sign not lost during shift */
  xn >>= SHIFT_VALUE;


  return ((int16)xn);

}



/*===========================================================================

FUNCTION    gpl1_gprs_calc_rf_pwr

DESCRIPTION
  This function determines the required rf power output.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_gprs_calc_rf_pwr(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  #define ALPHA_MAX 0xa
  uint8   tsn;
  int32   gamma_tn;
  int8    convert_alpha[] = {0, 0x03,0x06,0x0a,0x10,0x10,0x13,0x16,0x1a,0x1d,0x20}; //ifevrier/Sandiko 07-14-03

  uint8   pwr_index, pwr_lvl;
  int16   pmax;
  int8    alpha;
  ARFCN_T arfcn;


  gprs_pl1_serving_cell_meas_T  *gpl1_sc = gprs_serv_cell_meas_ptr;

  if (!gpl1_sc->gamma_gsm)
  {
    gpl1_sc->gamma_gsm = GAMMA_GSM900;

  }
  /*  Power calculation:- Pch = MIN(GAMMA_GSM - GAMMA_TN - alpha * (Cn + 48), PMAX)
      GAMMA_GSM is 39dBm for GSM and 36dBm for DCS.
      If no valid GAMMA_TN is present it will be set to zero.
      PMAX is set to GPRS_MS_TXPWR_MAX_CCH if PBCCH exists,
      or MS_TXPWR_MAX_CCH otherwise.  */

  /*  NOTE: - All values used in this function are dBm*16 */

  /*  Convert alpha from received value into 0.0 -> 1.0 as per spec * 16  */
  if (gpl1_sc->alpha > ALPHA_MAX)
  {
    gpl1_sc->alpha = ALPHA_MAX;

  }

  alpha = convert_alpha[gpl1_sc->alpha];

  /* Figure out the band we are using */

  arfcn.band = gprs_serv_cell_meas_ptr->ul_power_band;


  /* Convert pmax from received value into dBm * 16 */
  switch (arfcn.band)
    {
    case SYS_BAND_CELL_850:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_EGSM_900:
      /*  Determine the index into the power control array  */
      if( gpl1_sc->gprs_LB_MS_TXPWR_MAX_CCH_valid )
      {
        pwr_index = gpl1_sc->lb_pmax;
      }
      else
      {
        pwr_index = gpl1_sc->pmax;
      }
      /* Max power index is 31 */
      if (pwr_index > 31)
      {
        pwr_index = 31;
      }
      pmax = convert_pmax_900[pwr_index];
      break;

    case SYS_BAND_DCS_1800:
      if(gpl1_sc->pmax > 31)
      {
        pmax = convert_pmax_1800[31];
      }else
      {
        pmax = convert_pmax_1800[gpl1_sc->pmax];
      }
      break;

    case SYS_BAND_PCS_1900:
      if(gpl1_sc->pmax > 31)
      {
        pmax = convert_pmax_1900[31];
      }else
      {
        pmax = convert_pmax_1900[gpl1_sc->pmax];
      }      break;

     default:
       MSG_GERAN_ERROR_1_G("Unsupported band %d for rf pwr calculation", arfcn.band);
      /*  Determine the index into the power control array  */
      if( gpl1_sc->gprs_LB_MS_TXPWR_MAX_CCH_valid )
      {
        pwr_index = gpl1_sc->lb_pmax;
      }
      else
      {
        pwr_index = gpl1_sc->pmax;
      }
      /* Max power index is 31 */
      if (pwr_index > 31)
      {
        pwr_index = 31;
      }
      pmax = convert_pmax_900[pwr_index];
       break;
  }

  for (tsn = 0; tsn<8; tsn++)
  {
    int32 dbm_pwr_out, alpha_cn;

    /*  Convert gamma_tn from received value into dBm * 16  */
    gamma_tn = gpl1_sc->gamma_tn[tsn] << 5;



    if(gpl1_sc->cn == 0)
    {
      /* If the C value is not valid then set the alpha part of the
       * Power control alogorithm to zero.
       */

      alpha_cn = 0;

      /* As per GP-031925/CR235566, for DTM only, if C value is not valid we should
       * use Tx pwr level from SACCH
       */

      if ( l1_tskisr_blk->sub_state == L1_DTM_INIT ||
           l1_tskisr_blk->sub_state == L1_DTM_RUNNING )
      {
        if (tsn == 0)
        {
          pwr_lvl = l1_dedicated_data_ptr->ordered_ms_power_level;
      
          /* This indicates to the called function that arfcn is invalid and band info is valid*/
          arfcn.num = INVALID_ARFCN;

          /*  Convert to power control levels expected by NPL1  */
          gpl1_sc->pch[tsn] = L1_compute_next_txlev(pwr_lvl,pwr_lvl,arfcn);
        }
        else
        {
          /* No need to re-calculate - should be same value*/
          gpl1_sc->pch[tsn] = gpl1_sc->pch[tsn-1];
        }
        /* don't execute anything else further down the loop.
         * Though this is a rather coarse way of implementing
         * the functionality it keeps the code simple. Also this 
         * code will be executed very rarely so it should be fine. 
         */ 
        continue;
      }
    }else
    {
      /* C value is valid so calculate Alpha part of power control
       *
       * Divide by 32 as alpha has been premultiplied by 32
       */

      /*lint -e(702) sign not lost during shift */
      alpha_cn = (alpha * (gpl1_sc->cn + 768)) >> 5;

    }

    dbm_pwr_out = MIN((gpl1_sc->gamma_gsm - gamma_tn - alpha_cn), pmax);

    if (dbm_pwr_out < 0)
    {
      dbm_pwr_out = 0;
    }

    /*  Convert to power control levels expected by NPL1  */
    gpl1_sc->pch[tsn] = convert_pwr_lvl(dbm_pwr_out, arfcn.band, gas_id);
    
  }
}

/*===========================================================================

FUNCTION  convert_pwr_lvl

DESCRIPTION
  This function calculates the power control level to be sent to NPL1 from the
  calculated output power in dBm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static uint8  convert_pwr_lvl( int32       pwr_out,
                               sys_band_T  band,
                               gas_id_t gas_id)
{
  uint8 pwr_index = 0xff;

  /*  Pwr out is a dBm * 16 value and needs to be converted into the apporpriate
      power control level (as per GSM5.05). Then converted to the value required
      by NPL1. (This may be the same thing in later releases and is why the
      conversion is done in two stages. */

  /* Go through the appropriate array until we find the power we are closest to */
  switch (band)
  {
    /* GSM 850/900 band, max power is 0, min power is 19 */
    case SYS_BAND_CELL_850:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_EGSM_900:
      for (pwr_index = 0; pwr_index < 20; pwr_index++)
      {
        if (pwr_out >= convert_pmax_900_dynamic[pwr_index])
        {
          break;
        }
      }

      /* Limit the PCL as per Power Class for each band */
      if (pwr_index < gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1])
      {
        pwr_index = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
      }
      break;

    /* DCS band, max power is 29, min power is 15 */
    case SYS_BAND_DCS_1800:
      for (pwr_index = 29; pwr_index != 16; pwr_index = (pwr_index + 1) % 32)
      {
        if (pwr_out >= convert_pmax_1800_dynamic[pwr_index])
        {
          break;
        }
      }

      /* Limit the PCL as per Power Class for each band */
      if ( gl1_rf_power_class_per_band[gas_id][band] == 0 ) /* class 1 */
      {
         if (pwr_index > 28 ) /* greater than 30 dBm */
         {
           pwr_index = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
         }
      }
      else if ( gl1_rf_power_class_per_band[gas_id][band] == 1 ) /* class 2 */
      {
         if ( (pwr_index > 28) && (pwr_index < 3)  ) /* greater than 24 dBm */
         {
           pwr_index = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
         }
      }
      
      break;

    /* PCS band, max power is 30, min power is 15 */
    case SYS_BAND_PCS_1900:
      for (pwr_index = 30; pwr_index != 16; pwr_index = (pwr_index + 1) % 32)
      {
        if (pwr_out >= convert_pmax_1900_dynamic[pwr_index])
        {
          break;
        }
      }

      /* Limit the PCL as per Power Class for each band */
      if ( gl1_rf_power_class_per_band[gas_id][band] == 0 ) /* class 1 */
      {
         if (pwr_index > 29 ) /* greater than 30 dBm */
         {
           pwr_index = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
         }
      }
      else if ( gl1_rf_power_class_per_band[gas_id][band] == 1 ) /* class 2 */
      {
         if ( (pwr_index > 29) && (pwr_index < 3 ) ) /* greater than 24 dBm */
         {
           pwr_index = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
         }
      }
      break;

    case INVALID_BAND:
    default:

      MSG_GERAN_ERROR_1(" Incorrect band type %d",band);
      break;
  }

  return pwr_index;

}
/*===========================================================================

FUNCTION  translate_mean_bep_to_rxqual

DESCRIPTION
  This function translates a mean_bep value (0-31) into an rxqual value (0-7)

DEPENDENCIES
  None

RETURN VALUE
  rxqual

SIDE EFFECTS
  None

===========================================================================*/
static uint8 translate_mean_bep_to_rxqual(uint8 mean_bep)
{
  if ( mean_bep==0xFF )
  {
    // undefined, return error value
    return(0xFF);
  }
  else if (mean_bep < 24)
  {
    return(7-(mean_bep/3));
  }

  // MEAN_BEP_24..MEAN_BEP_31 all map to RXQUAL 0
  return(0);
}
/*===========================================================================

FUNCTION  calculate_mean_bep

DESCRIPTION
  This function calculates the average mean_bep during egprs tbf 

DEPENDENCIES
  None

RETURN VALUE
  average mean bep

SIDE EFFECTS
  None

===========================================================================*/
static uint8  calculate_mean_bep( gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* Pointers to read the serving cell measurement
   * structure
   */
  gprs_pl1_serving_cell_meas_T *gprs_pl1_scell_meas_ptr = gprs_serv_cell_meas_ptr;
  uint8 calc_buffer=0;
  uint8 mean_bep=0xFF;

   /* need a double buffer system as sync task
     can interrupt async */
  if (gprs_pl1_scell_meas_ptr->egprs_rxqual_calc.active_buffer == 1)
  {
    gprs_pl1_scell_meas_ptr->egprs_rxqual_calc.active_buffer =0;
    calc_buffer=1;
  }
  else
  {
    gprs_pl1_scell_meas_ptr->egprs_rxqual_calc.active_buffer =1;
  }

  if ( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->e )
  {
    egprs_mean_bep_store_T  *egprs_rxqual_calc_ptr = 
    &gprs_serv_cell_meas_ptr->egprs_rxqual_calc.egprs_mean_bep_store[calc_buffer];
    /* Can only report gmsk or 8psk so report the one 
       with the most blocks received */
    if (egprs_rxqual_calc_ptr->num_8psk_blocks < 
        egprs_rxqual_calc_ptr->num_gmsk_blocks)
    {
      /* Division results in F6.10 format
       * Only divide if accumulated r > 0.
       */
      if ( 0 != egprs_rxqual_calc_ptr->acc_gmsk_r )
      {
        /* Fill in the gmsk mean/cv bep for the
         * received blocks since last measurement
         * report.
         */
        if ( 0 != egprs_rxqual_calc_ptr->num_gmsk_blocks )
        {
          const uint16  mean_gmsk_bep =
          (uint16) ( (egprs_rxqual_calc_ptr->acc_mean_gmsk_bep_tn) / 
                     egprs_rxqual_calc_ptr->acc_gmsk_r ); /* 1/65535 units */
          /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
           * table.
           * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
           * This is the same as multiplying by 65537, optimise to not use multiplication.
           */
          const uint32  high_res_mean_gmsk_bep = (mean_gmsk_bep << 16) + mean_gmsk_bep;
          /* BEP reporting */
          mean_bep = (uint8) gpl1_egprs_meas_scale( GPRS_PL1_GMSK_MEAN_BEP_LOG_SCALE,
                                                    high_res_mean_gmsk_bep );
        }
      }
    }
    else
    {
      /* Division results in F6.10 format
       * Only divide if accumulated r > 0.
       */
      if ( 0 != egprs_rxqual_calc_ptr->acc_8psk_r )
      {
        /* Fill in the gmsk mean/cv bep for the
         * received blocks since last measurement
         * report.
         */
        if ( 0 != egprs_rxqual_calc_ptr->num_8psk_blocks )
        {
          const uint16  mean_8psk_bep =
          (uint16) ( (egprs_rxqual_calc_ptr->acc_mean_8psk_bep_tn) / egprs_rxqual_calc_ptr->acc_8psk_r ); /* 1/65535 units */
          /* Convert the mean bep inorder to pass it to the 32-bit quantization
           * table.
           * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
           * This is the same as multiplying by 65537, optimise to not use multiplication.
           */
          const uint32  high_res_mean_8psk_bep = (mean_8psk_bep << 16) + mean_8psk_bep;
          /* BEP reporting */
          mean_bep = (uint8) gpl1_egprs_meas_scale( GPRS_PL1_8PSK_MEAN_BEP_LOG_SCALE,
                                                    high_res_mean_8psk_bep );
        }
      }
    }
  }

  /* reset rxqual calc buffer */
  gprs_pl1_scell_meas_ptr->egprs_rxqual_calc.egprs_mean_bep_store[calc_buffer] = egprs_mean_bep_store_reset;
  return mean_bep;
}

