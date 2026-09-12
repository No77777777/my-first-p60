/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            L 1   B C C H  R E C E I V E   S C H E D U L I N G

GENERAL DESCRIPTION
   This module contains the procedures to schedule BCCH message decodes.
   This procedure executes in the L1 ISR context.

EXTERNALIZED FUNCTIONS
  L1_get_bcch
    Procedure to schedule a synchronous receive of the BCCH message.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_bcch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
18/01/16   zf        CR961805: Set first burst flag in x2g_startup cmd
10/4/17    rc        CR2030561 To enable early decode even during camping/reselection
10/03/17  ksb        CR2016325 Do not abort ccch in progress while in QTA gap  
26/02/16   js        CR980305 Issue X2G startup while configuring RX/ Sending blcok IND to RR when TRM is not granted
01/02/15   js        CR909145 FR 27391: GL1 should allow QBTA/QTA gaps in find_BCCH state to avoid LTA gaps
27/11/15   cjl       CR942511 Check TRM available before extending BCCH activity
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
29/05/15   am        CR843362 Remove CR823102
26/05/15   am        CR840857 Ensure GL1 is awake to schedule SYS INFO REQ
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
30/04/15   og        CR823102. Set the afc freq before decoding every BCCH radio block.
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX
10/02/15   am        CR793750 Correcting compilation warning
30/01/15   am        CR785921 Do TRM reserve & request for bcch readings in cell reselection state
06/01/15   am        CR390109 Improved DSDS cell reselection
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
07/16/13   zc        CR470197 Replace memcpy with memscpy
15/07/13   sp        CR508123: Update panic reset to use multi-sim data space
21/06/13   cs        TSTS Updates
08/06/12   pa        GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
15/05/11   jj        CR357981 Abort aquicition earlier if panic reset triggered
13/03/12   ws        CR 330862 - Change dedicated mode F3 level's and add sacch burst
                     metrics and compact message metrics log packets.
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
24/09/10   cja       Lint error fix
03/07/09   sjw       Support Extended BCCH parameter for l1_compute_tc()
11/15/05   sv        Lint cleanup.
09/07/05   sv        Fix panic reset due to stuck in FIND_BCCH mode
08/17/05   gfr       Use gl1_msg_init_frame_tick_info to init the ftn structure
08/16/05   sv        Clean up the abort scenario during BCCH receive.
07/29/05   gfr       Support for NPL1 DTM interface changes
06/15/04   gfr       Renamed gl1_msg_cfg_dl_ccch to gl1_msg_cfg_ccch
06/11/04   dp        Fix to correctly use the BCC (derived form BSIC) to
                     configure the BCCH
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
02/25/04   gw        Lint clean up.
01/28/04   gw        Changes to support reselection optimizations.
01/09/04   gw        Merged in support for new SI reading API from branch.
11/05/03   gfr       Support for quad-band.
08/29/03   gw        Added code to output F3 message with SNRs for each
                     received block.
07/11/03   gfr       Lint cleanup.
06/23/03   gfr       Compiler warning cleanup.
01/02/03   gw        Modified call to gl1_msg_cfg_dl_ccch() to reflect addition
                     of another parameter.
12/05/02   gw        Added support for decoding SI 2quater on extended BCCH.
10/01/02   ATM       Added central get and set FN routines
06/06/02   mk        Support for aborts on all channels.
05/24/02   JC        Added BCCH burst and message logging.
04/30/02   gw        Replaced call to L1_send_PH_DATA_IND with call to
                     L1_send_sysinfo_PH_DATA_IND so we can send TC to RR.
03/04/02   JC        Removed legacy SCH decoding only support. Also, added
                     gw's support for L1_ACQUIRE_COMMAND in L1_get_bcch().
02/14/02   gw        Added call to gl1_msg_reset() to support AGC.
02/05/02   JC        Changes to support AGC, removed write_sig_gains()
01/24/02   ATM       Added calls to l1_change_state() for logging
12/21/01   gw        Changes to support DRX scheduling.  Removed references to
                     gl1_msg_tag_type.
12/18/01   dp        Changes to support ncell handler type name change
12/13/01   gw        Changes to support message/frame layer acquisition.
11/27/01   mk        Added L1_BCCH_INIT (= L1_BCCH_START) substate to init
                     message callbacks. Misc cleanup.
11/9/01    bk        Updated NPL1 FTN block info to conform to the interface change
10/31/01   dp        Incorporated changes due to new CCCH decode_rpt format
10/10/01   gw        Fixed bugs in bcch_rx_callback.  Added support for Ncell
                     message layer functionality.
09/27/01   gw        Changes to match new gl1_msg_rx_ccch interface allowing
                     passing of the channel type.
09/25/01   mk/gw     Converted to message/frame layer interface.
08/31/01   JC        Cleanup of SPINCO_SIMULATION and KOVU compile switches.
04/13/01   JC        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#include "comdef.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "gl1_msg_g.h"
#include "gl1_sys_algo.h"
#include "msg.h"
#include "l1_log.h"
#include "l1_utils.h"    // for l1_state_change & gl1_get_FN()

#include "geran_dual_sim.h"
#include "gl1_hw_g.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT */

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#include "gl1_arbitrator_cxm.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

extern void write_arfcns(ARFCN_T rx_arfcns[], ARFCN_T arfcn );

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/


/* Prototype for local functions */
static void bcch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void bcch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );

typedef struct
{

gl1_msg_metrics_rpt      bcch_metrics_rpt[4];
int bcch_rx_in_progress;

  /* Two sets of report arrays are declared out of paranoia      */
  /* regarding reception of back-to-back blocks (extended BCCH). */
gl1_msg_decode_rpt    data_rpts[2];
}l1_bcch_data_t;

static l1_bcch_data_t  l1_bcch_data[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION  L1_GET_BCCH

DESCRIPTION
  This function reads the BCCH data off a carrier where the SCH has just
  been decoded.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean L1_get_bcch( uint32 *frames_until_active_ptr, gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_bcch_data_t *l1_bcch_data_ptr = &l1_bcch_data[gas_id];
  /* Locals */
  sys_algo_agc_T                *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  sys_algo_agc_T               *agc_ptr_divrx_ccch; // added CCCH temporarily for diferentiating on the other agc pointers.
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
  boolean                      leaving_bcch_mode;
  boolean                      extbcch = FALSE;
  /* Two sets of report arrays are declared out of paranoia      */
  /* regarding reception of back-to-back blocks (extended BCCH). */
  gl1_msg_rx_rpt               rpt;

  ARFCN_T                      ARFCNs[4];

  gl1_msg_rx_cb_type           rx_cb;
  gl1_msg_frame_tick_type      ftn;
  /* This indicates the start of a block _we_want_to_receive_ */
  boolean                      dl_block_start;
  uint8                        tag = 0;

  *frames_until_active_ptr = 0;

  leaving_bcch_mode = ( l1_tskisr_blk->main_command != L1_NULL_COMMAND );

  if ( leaving_bcch_mode||panic_reset_occured[gas_id] )
  {
     if(!gl1_hw_qta_gap_active(gas_id)) 
     { 
        /* Abort BCCH Rx */
        if(l1_bcch_data_ptr->bcch_rx_in_progress)
        {
           gl1_msg_abort_rx_ccch(gas_id);
        }
        MSG_GERAN_MED_0_G("FindBcch abort");
        return (TRUE);
     }
     else
     {
        if(!l1_bcch_data_ptr->bcch_rx_in_progress)
        {
           MSG_GERAN_MED_0_G("FindBcch abort");
           return (TRUE);
        }
        else
        {
           MSG_GERAN_HIGH_0_G("QTA is active-do not abort ccch in progress");
        }
     }
  }

  agc_ptr = &(l1_idle_data->campedon_cell_agc);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  agc_ptr_divrx_ccch = &(l1_idle_data->campedon_cell_agc_divrx);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  /* Code */
  switch ( l1_tskisr_blk->sub_sub_state )
  {
  case L1_BCCH_INIT:
  case L1_BCCH_START:
    {
      /* Configure the message layer */
      gl1_msg_reset(gas_id);

      rx_cb.data_cb    = bcch_rx_callback;
      rx_cb.metrics_cb = bcch_metrics_callback;
      rx_cb.log_cb     = NULL;
      rx_cb.abort_cb   = NULL;

      /* -------------------------------------------------------- */
      /* DESCRIPTION: For broadcast channels the TSC = BCC        */
      /* Ensure that TSC is valid by masking all but the 3 LSBs   */
      /* REF: (GSM 05.02, v. 4.10.1, 5.2.3)                       */
      /* -------------------------------------------------------- */

      MSG_GERAN_HIGH_1_G("Configuring BCCH BCC = %d",(l1_tskisr_blk->current_params.BSIC) & 0x0007);
      gl1_msg_cfg_ccch ((l1_tskisr_blk->current_params.BSIC) & 0x0007, TRUE, &rx_cb, gas_id );

      l1_bcch_data_ptr->bcch_rx_in_progress = 0;

      MSG_GERAN_MED_0_G("FindBcch init ...");

      l1_tskisr_blk->sub_sub_state = L1_BCCH_RUNNING;
    }
    /* fall through */
    /*lint !e616*/ /* valid fall through case */

  case L1_BCCH_RUNNING:
    dl_block_start = FALSE;

    /* controlled by the    mod51 frame number */
    switch ( frame_counters[gas_id].FNmod102 )
    {
    case 1:
    case 52: /* 1 */
      {
#if defined (FEATURE_QTA_IN_FINDBCCH)
        uint32 current_TC_mask = (1UL << ((gl1_get_FN(gas_id) / 51) % 8));
      /* Coming up is the BCCH - always read */
        if (l1_idle_data->BCCH_mask & current_TC_mask)
        {
          tag            = 0;
          dl_block_start = TRUE;
        }
#else
      tag            = 0;
      dl_block_start = TRUE;
#endif
      }
      break;

    case 5:
    case 56:
      /* Check for BCCHext */
      if (l1_tskisr_blk->current_params.extended_BCCH_mask != SERV_EXT_BCCH_MASK_NONE )
      {
        /* ------------------------------------------------- */
        /* DESCRIPTION: This checks for SI messages on the   */
        /* extended BCCH.  Currently SI 7, 8, and 2quater    */
        /* are supported on the extended BCCH (by this       */
        /* software).                                        */
        /* Not sure why FNmod42432 is used instead of FN     */
        /*                                                   */
        /* REF: (GSM 05.02, 6.3)                             */
        /* ------------------------------------------------- */

        uint32 BCCH_ext_mframe_mask;

        BCCH_ext_mframe_mask = (1UL << ((gl1_get_FN(gas_id) / 51) % 8));

        if (BCCH_ext_mframe_mask & l1_tskisr_blk->current_params.extended_BCCH_mask)
        {
#if defined (FEATURE_DUAL_SIM)
          /* During cell selection we only reserve enough frames for BCCH so check that we have a large enough grant 
             to perform EBCCH read before continuing and try to extend the grant if we do not */
          boolean frames_available = TRUE;
          if ( gl1_msg_get_multi_sim_mode() )
          {
            uint32 grant_fn = grm_get_trm_grant_fn(l1_tskisr_blk->client_id,gas_id);
            uint32 grant_frames = grm_get_trm_grant_frames(l1_tskisr_blk->client_id,gas_id);
            if ((grant_fn != NOT_A_FRAME_NUMBER) && (grant_frames > 0))
            {
              uint32 fn = gl1_get_FN(gas_id);
              uint32 remaining_frames = SUB_FN(ADD_FN(grant_fn,grant_frames),fn);

              /* 4 frames for EBCCH block receive + 1 each for the read pipeline and TRM cleanup */
              if ( remaining_frames < (FRAMES_PER_BLOCK + 2) )
              {
                frames_available = grm_extend_duration (l1_tskisr_blk->client_id,
                                        gpl1_multi_sim_convert_frames_to_sclcks ( grant_frames + FRAMES_PER_BLOCK ),
                                        gas_id);
                if (!frames_available)
                {
                  MSG_GERAN_HIGH_0_G("Extend TRM grant for EBCCH denied");
                }
              }
            }
          }
          if  (frames_available)
#endif
          {
            tag            = 1;
            dl_block_start = TRUE;
            extbcch        = TRUE;
          }
        }

      }
      break;

    default:
      break;

    } /* End of Switch (frame no.) */

    if ( dl_block_start )
    {
      if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) )
      {
        /* For a PCH/BCCH setup the asynch int possible flag */
        l1_set_asynch_int_possible( gas_id );
      }

      /* Fill up the parameter and report arrays */
      write_arfcns( ARFCNs,  l1_tskisr_blk->current_params.ARFCN );

      rpt.metrics[0] = &l1_bcch_data_ptr->bcch_metrics_rpt[0];
      rpt.metrics[1] = &l1_bcch_data_ptr->bcch_metrics_rpt[1];
      rpt.metrics[2] = &l1_bcch_data_ptr->bcch_metrics_rpt[2];
      rpt.metrics[3] = &l1_bcch_data_ptr->bcch_metrics_rpt[3];

      rpt.data = &l1_bcch_data_ptr->data_rpts[tag];

      MSG_GERAN_HIGH_1_G("BCCH pwr_dBm_x16 = %d",agc_ptr->pwr_dBm_x16);
#if defined (FEATURE_DUAL_SIM)
#ifdef FEATURE_QTA
    if ( l1_tskisr_blk->qta_in_progress )
	{
	  boolean interband_monitors = l1_sc_monscan_is_next_idle_scan_interband(gas_id);

	  MSG_GERAN_HIGH_1_G("FN = %d Init NCELL BCCH - QTA in progress", gl1_get_FN(gas_id));

	  if(!gl1_hw_qta_fw_rf_init(FALSE,GL1_FW_QBTA_CM_START_UP_OFFSET,FALSE,l1_tskisr_blk->current_params.ARFCN,interband_monitors,TRUE,gas_id))
	  {
		l1_tskisr_blk->firmware_loaded = FALSE;
	  }
	}
#endif /* FEATURE_QTA*/
#endif
      gl1_msg_rx_ccch ( ARFCNs,
                        agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                        agc_ptr_divrx_ccch,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                        tag,
                        FALSE,
                        GL1_MSG_BCCH,
                        &rpt,
                        gas_id);

      l1_bcch_data_ptr->bcch_rx_in_progress++;
      rpt.data->ccch.extbcch = extbcch;
    }
    else if ( l1_bcch_data_ptr->bcch_rx_in_progress == 0 )
    {
      /* keeping it aligned with the current bcch readings which does normal bcch
       * read for all TCs and extended bcch read for the given TCs only
       */
      *frames_until_active_ptr = L1_gap_to_next_bcch( gl1_get_FN( gas_id ), SERV_BCCH_MASK_ALL, l1_tskisr_blk->current_params.extended_BCCH_mask, gas_id );
    }

    gl1_msg_init_frame_tick_info(&ftn);
    ftn.next_frame_is_dl_block_start = dl_block_start;
    gl1_msg_frame_tick_notification ( &ftn,gas_id );
    break;

  default:
    MSG_GERAN_ERROR_0_G("FindBcch: Bad sub_sub_state");
    break;

  } /* end of switch (subsubstate) */


  return(FALSE);
}


/*===========================================================================

FUNCTION  BCCH_RX_CALLBACK

DESCRIPTION
  Callback function called when a BCCH block has been received.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void bcch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  l1_bcch_data_t *l1_bcch_data_ptr = &l1_bcch_data[gas_id];

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
  // In case of GPLT and no RF, return default/canned result
  MSG_GERAN_HIGH_0_G("GPLT DBG: bcch_rx_callback: Preparing Si3/SI4");

  rpt->ccch.hdr.good_data = TRUE;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

  /* send reports up the stack */
  if ( rpt->ccch.hdr.good_data ) /* rpt->good_data is 2 if true */
  {
    uint8 si_tc;

    si_tc = l1_compute_tc( gl1_get_FN(gas_id),
                           rpt->ccch.extbcch );

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
    // In case of GPLT and no RF, return default/canned result
     switch (si_tc)
     {
     case 2:
     case 6:
       {
           MSG_GERAN_HIGH_0_G("GPLT DBG: Making SI3");
           memscpy(rpt->ccch.data,sizeof(rpt->ccch.data), &gplt_si3_result, sizeof(gplt_si3_result));
           break;
       }
     case 3:
     case 7:
       {
         //si4
         MSG_GERAN_HIGH_0_G("GPLT DBG: Making SI4");
         memscpy(rpt->ccch.data,sizeof(rpt->ccch.data),&gplt_si4_result, sizeof(gplt_si4_result));
         break;
       }
     case 4:
     case 5:
       {
         MSG_GERAN_HIGH_0_G("GPLT DBG: Making SI2Quater");
         memscpy(rpt->ccch.data,sizeof(rpt->ccch.data), &gplt_si2quater_result, sizeof(gplt_si2quater_result));
         break;
       }
     default:
       {
         break;
       }
     }
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

    L1_send_sysinfo_PH_DATA_IND(TRUE,
                                BCCH,  /* we only ever receive BCCH */
                                FALSE, /* BCCH can never be own paging block */
                                rpt->ccch.arfcn,
                                si_tc,
                                rpt->ccch.data,
                                MAX_BCCH_OCTETS, gas_id);

    MSG_GERAN_MED_3_G("BCCH(tag=%d) decoded (arfcn=%d TC=0x%02X)", rpt->ccch.tag, rpt->ccch.arfcn.num, si_tc);
  }
  else
  {
    L1_send_PH_DATA_IND(FALSE,
                        BCCH,
                        FALSE,
                        rpt->ccch.arfcn,
                        NULL,
                        0, gas_id);

    MSG_GERAN_ERROR_2_G("BCCH(%d) decode failed (arfcn=%d)", rpt->ccch.tag, rpt->ccch.arfcn.num);
  }

  /* LOG MESSAGE METRICS */
  l1_log_message_metrics ( GL1_MSG_BCCH, rpt, gas_id );
  l1_log_message_metrics_compact( GL1_MSG_BCCH, rpt, gas_id);

  l1_bcch_data_ptr->bcch_rx_in_progress--;
}


/*===========================================================================

FUNCTION  BCCH_METRICS_CALLBACK

DESCRIPTION
  Callback function called when BCCH block metrics are available.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void bcch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
   dBx16_T snr_array[4];
#ifdef FEATURE_GSM_COEX_SW_CXM
   dBx16_T             power_instant = 0;
   uint8               valid_num_bursts=0;
#endif /* FEATURE_GSM_COEX_SW_CXM */
   uint8               num_bursts;

   /* Output a debug message showing the SNRs */
   for ( num_bursts = 0; num_bursts < 4; num_bursts++ )
   {
     if ( rpt[num_bursts]->valid )
     {
       snr_array[num_bursts] = gl1_msg_SNR_to_dBx16( rpt[num_bursts]->snr );
	   
#ifdef FEATURE_GSM_COEX_SW_CXM
        power_instant +=  rpt[num_bursts]->pwr_dBm_x16;
        valid_num_bursts++;
#endif /*  FEATURE_GSM_COEX_SW_CXM */	   
     }
     else
     {
        snr_array[num_bursts] = 0;
     }
   }
   l1_snr_msg( snr_array, 4, gas_id );

#ifdef FEATURE_GSM_COEX_SW_CXM

   if (valid_num_bursts)
   {
     power_instant = power_instant/valid_num_bursts;
     garb_intf_notify_find_bcch_update_rx_power(rpt[0]->arfcn,(int32)power_instant,gas_id);
     /* First Power Reported when registration for Freq of SCELL */
     gpl1_set_rx_pwr_avg_dbm10_coex(((((int32)power_instant + GARB_FIND_BCCH_DESENSE_BIAS) * 10 ) >> 4),gas_id);
     garb_cxm_power_indication_arfcn(rpt[0]->arfcn,
                                     ((((int32)power_instant + GARB_FIND_BCCH_DESENSE_BIAS) * 10 ) >> 4), 
                                     gas_id);
   }
#endif /*  FEATURE_GSM_COEX_SW_CXM */

   /* LOG BURST METRICS */
   l1_log_burst_metrics ( GL1_MSG_BCCH, rpt, gas_id );
}

/*===========================================================================

FUNCTION  L1_gap_to_next_bcch

DESCRIPTION
  Returns the gap to the first next BCCH or extended BCCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint32 L1_gap_to_next_bcch( uint32 this_fn, uint32 bcch_mask, uint32 ext_bcch_mask, gas_id_t gas_id )
{
  uint32 TC, TC_gap;
  uint32 gap_to_next_activity = GL1_DEFS_INVALID_FN;
  uint32 gap_to_next_bcch = GL1_DEFS_INVALID_FN, gap_to_next_ext_bcch = GL1_DEFS_INVALID_FN;
  uint32 local_FNmod51 = this_fn % MULTIFRAME_51;

    TC = ( this_fn / MULTIFRAME_51 ) % 8;

    /* Work out gap to next block received by adding
    * gap to next block 0 to 51 * number of mframes
    * until next TC value.
    */
    if( bcch_mask!= SERV_BCCH_MASK_NONE )
    {
      uint32 gap_to_next_block_0;

      if (local_FNmod51 > 0)
      {
        /* next block 0 is in next multiframe */
        gap_to_next_block_0 = MULTIFRAME_51 - local_FNmod51;
        TC++;
        TC %= 8;
      }
      else
      {
        /* next block 0 is in current multiframe */
        gap_to_next_block_0 = 0;
      }

      for (TC_gap=0; TC_gap<8; TC_gap++)
      {
        if ( (bcch_mask & (1uL<<TC)) != 0 )
        {
          /* we found the next multiframe to
          * receive on
          */
          break;
        }

        TC++;
        TC %= 8;
      }

      gap_to_next_bcch = gap_to_next_block_0 + ( MULTIFRAME_51 * TC_gap );
    }

    /* Work out gap to next extended BCCH block to receive
    * if any.
    */
    if ( ext_bcch_mask != SERV_EXT_BCCH_MASK_NONE )
    {
      uint32 gap_to_next_block_1;

      /* Works the same as the code for BCCH above
      * except that ext BCCH is on block 1 instead
      * of block 0.
      */
      if (local_FNmod51 <= 4)
      {
        /* next block 1 is in current multiframe */
        gap_to_next_block_1 = 4 - local_FNmod51;
      }
      else
      {
        /* next block 1 is in next multiframe */
        gap_to_next_block_1 = 55 - local_FNmod51;
        TC++;
        TC %= 8;
      }

      for (TC_gap=0; TC_gap<8; TC_gap++)
      {
        if ( (ext_bcch_mask & (1uL<<TC)) != 0 )
        {
          /* we found the next multiframe to
          * receive on
          */
          break;
        }

        TC++;
        TC %= 8;
      }

      gap_to_next_ext_bcch = gap_to_next_block_1 + ( MULTIFRAME_51 * TC_gap );
    }

    if( gap_to_next_bcch < gap_to_next_activity )
    {
      gap_to_next_activity = gap_to_next_bcch;
    }

    if( gap_to_next_ext_bcch < gap_to_next_activity )
    {
      gap_to_next_activity = gap_to_next_ext_bcch;
    }

  MSG_GERAN_HIGH_1_G("gap to next bcch %d",gap_to_next_activity);

  return gap_to_next_activity;
}

