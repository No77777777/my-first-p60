/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           L 1   C N F  /  I N D   P R O C E S S I N G   # 1

GENERAL DESCRIPTION
   This module contains the procedures to indicate status and confirm
   commands.

EXTERNALIZED FUNCTIONS
  L1_send_MPH_SERVING_IDLE_MEAS_IND
  L1_send_MPH_POWER_SCAN_CNF
  L1_send_MPH_SELECT_SPECIFIC_BCCH_CNF
  L1_send_MPH_HANDOVER_IND
  L1_send_MPH_HANDOVER_CNF
  L1_send_MPH_CHANNEL_MODE_MODIFY_CNF
  L1_send_MPH_STOP_HANDOVER_ACCESS_CNF

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_cnf1.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ---------------------------------------------------------- 
05/03/18   sk        CR2199223 Address the Task and ISR concurrency while sending power scan cnf
14/06/17   mk        CR2059749 Consider Drx power instant as well for MPH_SERVING_AUX_MEAS_IND
07/08/17   sn        CR2084716 changes to fix memory heap starvation issue with GDET FR
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
18/09/15   rmn       CR892350: Mainline FEATURE_SNR_BASED_CELL_SEL_FOR_CSFB
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
27/09/15   dv        CR915710 For MPH_BECCH_DECODE_LIST_REQ ensure only 
                              required bands are registered with TRM  
14/08/15   sjw       CR884733: send MPH_ACQUISITION_STATE_IND when granted for inverted acquisition
31/07/15   cah       CR882109; FR:26893: RR is basing band prioritisation on Dr mode echoed in START_GSM_MODE_CNF
26/06/15   am        CR862168 Adding simulation support through diag
15/06/15   am        CR854340 Adding simulation support through diag
02/06/15   pjr       CR847067: Framework to support MPH_VOICE_SAMPLE_RATE_CHANGE_IND
27/05/15   sjw       CR843300: Framework to support MPH_PREEMPTION_IND
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs. 
09/07/14   am        CR691142 Update new multisim mode when all subs are inactive
20/06/14   pjr       DATA+MMS feature - USF utilisation calculation
09/04/14   am        CR639289 Report SNR in MPH_FCCH_SCH_DECODE_IND to GRR
01/10/13   sk        CR552259 CR sync from 1.0/1.1
18/09/13   sjw       CR543997 - do not allow power scan results to be shared if invalid
06/09/13   sjw       CR467545 - Pass updated RXLEV to GRR in FCCH_SCH_DECODE_IND
02/09/13   nm        propagation (from nikel) of CR465537  GL1/GRR – Introduce MPH_RA_ABORT_REQ and MPH_RA_ABORT_CNF for DSDS 
                     to resolve state transition in RR/L1. 
26/06/13   cs        CR505156 Latest TSTS Interface updates
26/06/13   cs        TSTS Updates
26/02/13   ip        DSDS CR453357, don't sleep until stop mode CNF is sent
06/02/13   cs        DSDS CR438977: merged support for CBCH in DSDS mode
08/06/12   pa        Added support for GPLT.
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
27/04/10   cs        CR236460 Latest AFC\XO handling updates
26/05/09   cs        CR167848 fix for expanded afc_freq I/F to mDSP
09/10/08   ap        CR 158238 - Complete L1 support to RR for Frequency
                     Redefinition Interface.
27/06/08   nt        CR147984 - REV2:- Final changes for TCXO reset
16/06/08   nt        CR147984:- Changes for TCXO reset.
17/01/08   cs        Fix the external def of gl1_hw_get_vctcxo_pdm_val
26/04/07   ws        CR 114617 - Added L1_send_MPH_FCCH_SCH_DECODE_IND for UOOS
11/01/07   og        Resolve RVCT2.1 compiler warnings.
23/01/07   og        Resolve RVCT2.1 compiler warnings, merge from the
                     gpl1_hummer.10.00.43.01 branch.
12/14/05   sv        Change prototype for MPH_DECODE_BCCH_LIST_CNF.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
04/21/05   gfr       Improve serving cell measure message readability
04/21/05   sv        Add support for L1 "Panic Reset"
05/26/04   sv        Lint Cleanup.
03/1704    dlh       Added L1_send_MPH_START_IDLE_MODE_CNF
02/25/04   gw        Lint clean up.
02/19/04   gfr       Compile fixes for gsm rotators.
01/28/04   gw        Changes to support reselection optimizations.
11/05/03   gfr       Support for quad-band.
08/01/03   tb        Pass trk_lo_adj for GtoW interrat.
11/07/03   gfr       Lint cleanup.
12/03/02   mk        Cleaned L1_send_MPH_SERVING_IDLE_MEAS_IND(), don't
                     need GPRS specific code anymore for RR/GRR.
10/14/02   ATM       FEATURE_MULTIMODE_GSM is mainlined - removed flag per jchao
10/10/02   mk        Merged GPRS release 1 phase1 code to baseline.
08/23/02   plm       Removed the declaration of the variable i in the functions
                     L1_send_MPH_RESELECTION_CNF() and L1_send_MPH_DECODE_BCCH_LIST_FAILURE_IND()
                     to eliminate compiler warnings. Also initialized local variable signal_strength
                     to zero in function L1_send_MPH_SERVING_IDLE_MEAS_IND().
08/13/02   dp        Added cnf functions for parallel decodes
08/12/02   gw        Added function L1_send_MPH_POWER_SCAN_CNF() to support
                     cell selection/re-selection optimizations.
06/27/02   dp        Added support to send back MPH_RESELECTION_CNF
05/10/02   jc        Add support for FEATURE_MULTIMODE_GSM
03/22/02   JC        Bug fix, reset no_of_measure to 1 instead of 0 and
                     added MSG_ macro for debugging.
02/19/02   JC        Added RXLEV support to L1_send_MPH_SERVING_IDLE_MEAS_IND.
02/01/02   JC        Cleanup and AGC support.
09/25/01   mk        Serving cell metrics report is sent to rr.
08/31/01   JC        Obsoleted unnecessary compile switches & added std QCT
                     fcn hdr blocks.
08/17/01   mk/gw     Added Ncell support, featurized via MONITOR_DEBUG.
05/22/01   JC        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gs.h"
#include "l1_task.h"
#include "l1_utils.h" /* L1_map_dBm_x16_to_rxlev() */
#include "l1_isr.h"
#include "l1_drx.h"
#include "ms.h"
#include "msg.h"
#include "memory.h"   /* memset() */
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "sys_type.h" /* dBx16_T  */

#ifdef FEATURE_GL1_GPLT
#include "gplt_cmdi.h"
#include "gplt_ctl.h"
#endif /*FEATURE_GL1_GPLT*/
#ifdef GL1_GMSK_MOD_DET
#include "gprs_mem.h"
#endif
#include "gl1_hw.h"
#include "gl1_hw_g.h"

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
#include "l1_smscb.h"
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */
#ifdef GL1_GMSK_MOD_DET
extern uint16 *decStat_pscan[NUM_GERAN_DATA_SPACES] ;
#endif
#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#endif /* FEATURE_WLAN_COEX_SW_CXM */

/*===========================================================================

FUNCTION  L1_SEND_MPH_SERVING_IDLE_MEAS_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  l1_serving_cell_meas struct must be loaded with the latest meas values.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_SERVING_IDLE_MEAS_IND(byte BSIC, gas_id_t gas_id)
{
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_SERVING_IDLE_MEAS_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->serving_idle_meas_ind.param.RXLEV_average =
        L1_map_dBm_x16_to_rxlev( (int16)l1_serving_cell_meas_ptr->rx_power );
      gplt_cmd_ptr->serving_idle_meas_ind.param.RXLEV_instant =
        L1_map_dBm_x16_to_rxlev( l1_serving_cell_meas_ptr->rx_power_instant );
      gplt_cmd_ptr->serving_idle_meas_ind.param.SNR_is_bad = l1_serving_cell_meas_ptr->SNR_is_bad;

      MSG_GERAN_HIGH_3_G("GSM Serving Cell Measure: avg=%d, now=%d, bad_snr=%d",
           l1_serving_cell_meas_ptr->rx_power/16,
           l1_serving_cell_meas_ptr->rx_power_instant/16,
           l1_serving_cell_meas_ptr->SNR_is_bad );

   gplt_cmd_ptr->serving_idle_meas_ind.param.trk_lo_adj = (word)gl1_hw_get_vctcxo_pdm_val( gas_id );

      gplt_cmd_ptr->serving_idle_meas_ind.param.SCH_block[0] = (byte)(BSIC & 0x3f) * 4;
      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_SERVING_IDLE_MEAS_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   mph_serving_idle_meas_ind_T   message_buffer,*message;
   word                          message_length;
   uint8                         prx_rxlev;

   /* Align the pointers */
   message = &message_buffer;

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = MPH_SERVING_IDLE_MEAS_IND;   /*lint !e641 */
   message_length = sizeof(mph_serving_idle_meas_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* This code relies on the fact that the ISR populates
    * l1_serving_idle_meas from a running average buffer
    * and no averaging is required here.
    * Also assumes that this function is called from the
    * ISR context.
    */
   prx_rxlev = L1_map_dBm_x16_to_rxlev( (int16)l1_serving_cell_meas_ptr->rx_power );
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   if(TRUE == gl1_get_IMRD_ctl_nv(gas_id))
   {
     uint8 divrx_rxlev = L1_map_dBm_x16_to_rxlev( (int16)l1_serving_cell_meas_ptr->divrx_power );
     message->RXLEV_average  = MAX(prx_rxlev, divrx_rxlev);
     MSG_GERAN_MED_5_G("GSM Serving Cell Measure: prx_rxlev=%d, divrx_rxlev=%d "
                     "RXLEV_average=%d, rx_power=%d, divrx_power=%d",
                     prx_rxlev,
                     divrx_rxlev,
                     message->RXLEV_average,
                     l1_serving_cell_meas_ptr->rx_power,
                     l1_serving_cell_meas_ptr->divrx_power );
   }
   else
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
   {
     message->RXLEV_average  = prx_rxlev;
   }
   message->RXLEV_instant  = L1_map_dBm_x16_to_rxlev( l1_serving_cell_meas_ptr->rx_power_instant );
   message->SNR_is_bad     = l1_serving_cell_meas_ptr->SNR_is_bad;
   message->gas_id =  gas_id;

#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
   #error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */
   MSG_GERAN_MED_3_G("GSM Serving Cell Measure: avg=%d, now=%d, bad_snr=%d",
           l1_serving_cell_meas_ptr->rx_power/16,
           l1_serving_cell_meas_ptr->rx_power_instant/16,
           l1_serving_cell_meas_ptr->SNR_is_bad );

   message->trk_lo_adj = (word)gl1_hw_get_vctcxo_pdm_val( gas_id );


   /* create a SCH block */
   message->SCH_block[0] = (byte)(BSIC & 0x3f) * 4;

   /* Send the message */
   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}


/*===========================================================================

FUNCTION  L1_SEND_MPH_SERVING_AUX_MEAS_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  l1_serving_cell_meas struct must be loaded with the latest meas values.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_SERVING_AUX_MEAS_IND( boolean own_paging_block, gas_id_t gas_id )
{
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_SERVING_AUX_MEAS_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->serving_aux_meas_ind.param.own_paging_block = own_paging_block;
      gplt_cmd_ptr->serving_aux_meas_ind.param.RXLEV_instant =
      L1_map_dBm_x16_to_rxlev( l1_serving_cell_meas_ptr->rx_power_instant );
      gplt_cmd_ptr->serving_aux_meas_ind.param.SNR_is_bad = l1_serving_cell_meas_ptr->SNR_is_bad;

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_SERVING_AUX_MEAS_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   mph_serving_aux_meas_ind_T    message_buffer,*message;
   word                          message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = MPH_SERVING_AUX_MEAS_IND;   /*lint !e641 */
   message_length = sizeof(mph_serving_aux_meas_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* This code relies on the fact that the ISR populates
    * l1_serving_idle_meas from a running average buffer
    * and no averaging is required here.
    * Also assumes that this function is called from the
    * ISR context.
    */
   message->RXLEV_instant    = L1_map_dBm_x16_to_rxlev( l1_serving_cell_meas_ptr->rx_power_instant );

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   if(TRUE == gl1_get_IMRD_ctl_nv(gas_id))
   {
     if(l1_serving_cell_meas_ptr->divrx_power_instant != GL1_INVALID_POWER_LEVEL)
     {
       uint8 divrx_rxlev_instant = L1_map_dBm_x16_to_rxlev( l1_serving_cell_meas_ptr->divrx_power_instant );
       message->RXLEV_instant  = MAX(message->RXLEV_instant, divrx_rxlev_instant);
     }
   }
#endif

   message->SNR_is_bad       = l1_serving_cell_meas_ptr->SNR_is_bad;
   message->own_paging_block = own_paging_block;
   message->gas_id =  gas_id;

#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
   #error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */
   /* Send the message */
   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_POWER_SCAN_CNF

DESCRIPTION
  Tell RR L1 has finished a power scan and the results are ready in the
  buffers provided.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_POWER_SCAN_CNF( boolean l1_suspending, gas_id_t gas_id )
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_POWER_SCAN_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->pwr_scan_cnf.param.debug_flag = 5;
      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_POWER_SCAN_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }
#else

   /* Locals */

   mph_power_scan_cnf_T      message_buffer,*message;
   int                       message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_power_scan_cnf_T));

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = MPH_POWER_SCAN_CNF;   /*lint !e641 */

   message->suspending                 = l1_suspending;
   message->invalidate                 = FALSE;
   message->gas_id                     = gas_id;

   message_length = sizeof(mph_power_scan_cnf_T) - sizeof(IMH_T); /*lint !e778 evaluates to 0 */
   PUT_IMH_LEN(message_length,message)

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#ifdef GL1_GMSK_MOD_DET
   GL1_ISR_LOCK(gas_id);
   if ( decStat_pscan[gas_id] != NULL )
   {
   MSG_GERAN_HIGH_1_G("Free Memory at scan cnf :%2x", decStat_pscan[gas_id]);   
   GPRS_MEM_FREE(decStat_pscan[gas_id]);
   decStat_pscan[gas_id] = NULL;
   }
   GL1_ISR_UNLOCK(gas_id);   
#endif

   /*lint -esym(715, l1_suspending) not referenced */
#endif /*FEATURE_GL1_GPLT*/
}


/*===========================================================================

FUNCTION  L1_SEND_MPH_SELECT_SPECIFIC_BCCH_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  found
  Pointer to SCH block

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_SELECT_SPECIFIC_BCCH_CNF(boolean found, byte  *sch_block, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */
  int i;
    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_SELECT_SPECIFIC_BCCH_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->select_specific_cnf.param.bcch_found = found;
      if ( found  )
      {
        for ( i = 0; i < SCH_BLOCK_SIZE ; i++ )
        {
        gplt_cmd_ptr->select_specific_cnf.param.SCH_block[i] = *(sch_block + i);
        }
      }
      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_SELECT_SPECIFIC_BCCH_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }
#else
   /* Locals */
   mph_select_specific_bcch_cnf_T    message_buffer,*message;
   int                               message_length,i;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_select_specific_bcch_cnf_T));

      /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_SELECT_SPECIFIC_BCCH_CNF;  /*lint !e641 */
   message_length = sizeof(mph_select_specific_bcch_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
   #error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */

  /* Load the data */
   message->BCCH_found = found;
   if ( found  )
   {
      for ( i = 0; i < SCH_BLOCK_SIZE ; i++ )
      {
         message->SCH_block[i] = *(sch_block + i);
      }
   }
   message->gas_id = gas_id;

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
   /* new BCCH has just been selected, reset any CB-DRX schedule */
   l1_smscb_reset_schedule_info( gas_id );
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_RESELECTION_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  found
  Pointer to SCH block

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_RESELECTION_CNF(boolean found, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_RESELECTION_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_RESELECTION_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   mph_reselection_cnf_T            message_buffer,*message;
   int                              message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_reselection_cnf_T));

      /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_RESELECTION_CNF;   /*lint !e641 */
   message->gas_id = gas_id;
   message_length = sizeof(mph_reselection_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Load the data */
   message->BCCH_found = found;
   message->gas_id = gas_id;

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
   /* new BCCH has just been selected, reset any CB-DRX schedule */
   l1_smscb_reset_schedule_info( gas_id );
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_DECODE_BCCH_LIST_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_DECODE_BCCH_LIST_CNF( boolean l1_suspending, gas_id_t gas_id )
{
#if  !defined(FEATURE_GL1_GPLT)
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif

#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_DECODE_BCCH_LIST_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */
      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_DECODE_BCCH_LIST_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   mph_decode_bcch_list_cnf_T       message_buffer,*message;
   int                              message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_decode_bcch_list_cnf_T));

      /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_DECODE_BCCH_LIST_CNF; /*lint !e641 */
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
   message->suspending                 = l1_suspending;
#endif
  /* To indicate RR about the confidence level of BCCH list decoding
   * If L1 requires another round of PLMN selection procedure, it sets
   * the value of retry_required to TRUE. This decision is based upon the
   * the confidence of the RGS value and also if and SCH's have been decoded
   * in the last pass through the list.
   */
   message->retry_required = (!gl1_msg_get_pdm_confidence_factor(gas_id) && (l1_BCCH_List_ptr->sch_found == FALSE));

   if (message->retry_required)
   {
     MSG_GERAN_ERROR_3_G("L1 to RR retry_required %d l1_BCCH_List_ptr->sch_found %d",
       message->retry_required,
       l1_BCCH_List_ptr->sch_found,
       0);
   }
   message->gas_id = gas_id;
   message_length = sizeof(mph_decode_bcch_list_cnf_T) - sizeof(IMH_T); /*lint !e778 evaluates to 0 */
   PUT_IMH_LEN(message_length,message)

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

   /*lint -esym(715, l1_suspending) not referenced */

   /* In MultiSim FIND_BCCH & NULL States, we intialise our band struct to only those bands that are 
    * requested in the MPH_DECODE_BCCH_LIST_REQ. once this request has been serviced,
    * restore our default values. calling this function by default shoul dbe harmless
    */
     
   grm_set_valid_acquisition_bands( l1_tskisr_blk->client_id, gas_id );

#endif /*FEATURE_GL1_GPLT*/
}


/*===========================================================================

FUNCTION  L1_SEND_MPH_DECODE_BCCH_LIST_FAILURE_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  L1_send_MPH_DECODE_BCCH_LIST_FAILURE_IND (ARFCN_T arfcn, rr_l1_decode_bcch_failure_T failure_cause, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_DECODE_BCCH_LIST_FAILURE_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_DECODE_BCCH_LIST_FAILURE_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */

   mph_decode_bcch_list_failure_ind_T  message_buffer,*message;
   int                           message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_decode_bcch_list_failure_ind_T));

      /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_DECODE_BCCH_LIST_FAILURE_IND; /*lint !e641 */
   message_length = sizeof(mph_decode_bcch_list_failure_ind_T) - sizeof(IMH_T);

   message->frequency = arfcn;
   message->failure_cause = failure_cause;
   message->gas_id = gas_id;
   PUT_IMH_LEN(message_length,message) /*lint !e740 unusual pointer cast */

   /* Send the message */

   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

/*===========================================================================

FUNCTION  L1_send_MPH_FCCH_SCH_DECODE_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_FCCH_SCH_DECODE_IND(ARFCN_T arfcn, byte bsic, byte rxlev, uint16 fcch_snr, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_FCCH_SCH_DECODE_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_FCCH_SCH_DECODE_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   mph_fcch_sch_decode_ind_T  message_buffer,*message;
   int                        message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_FCCH_SCH_DECODE_IND; /*lint !e641 */

   /* complete the payload */
   message->BCCH_ARFCN            = arfcn;
   message->BSIC.PLMN_colour_code = (bsic >> 3);
   message->BSIC.BS_colour_code   = (bsic & 0x07);
   message->RXLEV_average         = rxlev;
   message->fcch_snr              = fcch_snr;
   message->gas_id                = gas_id;

   message_length = sizeof(mph_fcch_sch_decode_ind_T) - sizeof(IMH_T);

   PUT_IMH_LEN(message_length,message) /*lint !e740 unusual pointer cast */

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_HANDOVER_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_HANDOVER_IND(byte error, gas_id_t gas_id)
{

   /* Locals */

   mph_handover_ind_T           message_buffer,*message;
   int                           message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_handover_ind_T));

      /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_HANDOVER_IND; /*lint !e641 */
   message_length = sizeof(mph_handover_ind_T) - sizeof(IMH_T);
   message->error_code = error;
   message->gas_id = gas_id;
   PUT_IMH_LEN(message_length,message)

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

#ifdef FEATURE_WLAN_COEX_SW_CXM
   /* HO completed/failed, end high priority with WLAN */
   garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif
}


/*===========================================================================

FUNCTION  L1_send_MPH_HANDOVER_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  ob_timing_diff in bits
  prev_chnl_tmng_adv in bits

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_HANDOVER_CNF(dword ob_timing_diff, byte  prev_chnl_tmng_adv, gas_id_t gas_id)
{

   /* Locals */

   mph_handover_cnf_T           message_buffer,*message;
   int                          message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_handover_cnf_T));

      /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_HANDOVER_CNF;  /*lint !e641 */
   message_length = sizeof(mph_handover_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* insert the data */
   message->observed_time_difference[0] = (byte)((ob_timing_diff/(256*256))%256);
   message->observed_time_difference[1] = (byte)((ob_timing_diff/256)%256);
   message->observed_time_difference[2] = (byte)(ob_timing_diff%256);

   message->last_timing_advance = prev_chnl_tmng_adv;
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

}


/*===========================================================================

FUNCTION  L1_SEND_MPH_CHANNEL_MODE_MODIFY_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_CHANNEL_MODE_MODIFY_CNF(byte error, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_CHANNEL_MODE_MODIFY_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->channel_mode_modify_cnf.param.error_code = error;

      MSG_GERAN_HIGH_1_G( "GPLT DBG: Sending GPLT_MPH_CHANNEL_MODE_MODIFY_CNF error %d", error);
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */

   mph_channel_mode_modify_cnf_T           message_buffer,*message;
   int                           message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_channel_mode_modify_cnf_T));

      /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_CHANNEL_MODE_MODIFY_CNF;  /*lint !e641 */
   message_length = sizeof(mph_channel_mode_modify_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* insert the data */
   message->error_code = error;
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}


/*===========================================================================

FUNCTION  L1_SEND_MPH_STOP_HANDOVER_ACCESS_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  ob_timing_diff,
  prev_chnl_tmng_adv

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_STOP_HANDOVER_ACCESS_CNF(dword ob_timing_diff,
                                               byte  prev_chnl_tmng_adv, gas_id_t gas_id)

{
   /* Locals */

   mph_stop_handover_access_cnf_T  message_buffer,*message;
   int                             message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_stop_handover_access_cnf_T));

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_STOP_HANDOVER_ACCESS_CNF;  /*lint !e641 */
   message_length = sizeof(mph_stop_handover_access_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* insert the data */
   message->observed_time_difference[0] = (byte)((ob_timing_diff/(256*256))%256);
   message->observed_time_difference[1] = (byte)((ob_timing_diff/256)%256);
   message->observed_time_difference[2] = (byte)(ob_timing_diff%256);

   message->last_timing_advance = prev_chnl_tmng_adv;
   message->gas_id = gas_id;
   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}


/*===========================================================================

FUNCTION  L1_send_MPH_START_GSM_MODE_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_START_GSM_MODE_CNF ( gas_id_t gas_id )
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_START_GSM_MODE_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_START_GSM_MODE_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
  mph_start_gsm_mode_cnf_T        message_buffer,*message;
  int                             message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* clear the data */
  memset(message,'\0',sizeof(mph_start_gsm_mode_cnf_T));

  /* Set up the message header */
  message->message_header.message_set = MS_RR_L1;
  message->message_header.message_id  = MPH_START_GSM_MODE_CNF;    /*lint !e641 */
   message->gas_id = gas_id;
  message->drdsds_enabled = gl1_drdsds_enabled(gas_id);
  message_length = sizeof(mph_start_gsm_mode_cnf_T) - sizeof(IMH_T); /*lint !e778 const expression = 0 */
  PUT_IMH_LEN(message_length,message)

  /* Send the message */
  (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTION  L1_send_MPH_START_GSM_MODE_FOR_BPLMN_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_START_GSM_MODE_FOR_BPLMN_CNF ( gas_id_t gas_id )
{
  mph_start_gsm_mode_for_bplmn_cnf_T message_buffer,*message;
  int                             message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* clear the data */
  memset(message,'\0',sizeof(mph_start_gsm_mode_for_bplmn_cnf_T));

  /* Set up the message header */
  message->message_header.message_set = MS_RR_L1;
  message->message_header.message_id  = MPH_START_GSM_MODE_FOR_BPLMN_CNF;    /*lint !e641 */
  message_length = sizeof(mph_start_gsm_mode_for_bplmn_cnf_T) - sizeof(IMH_T); /*lint !e778 const expression = 0 */
  PUT_IMH_LEN(message_length,message)
  message->gas_id = gas_id;
  /* Send the message */
  (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

}

/*===========================================================================

FUNCTION  L1_send_MPH_RESUME_GSM_MODE_FOR_BPLMN_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_RESUME_GSM_MODE_FOR_BPLMN_CNF ( gas_id_t gas_id )
{
  mph_resume_gsm_mode_for_bplmn_cnf_T message_buffer,*message;
  int                             message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* clear the data */
  memset(message,'\0',sizeof(mph_resume_gsm_mode_for_bplmn_cnf_T));

  /* Set up the message header */
  message->message_header.message_set = MS_RR_L1;
  message->message_header.message_id  = MPH_RESUME_GSM_MODE_FOR_BPLMN_CNF;     /*lint !e641 */
  message_length = sizeof(mph_resume_gsm_mode_for_bplmn_cnf_T) - sizeof(IMH_T); /*lint !e778 const expression = 0 */
  PUT_IMH_LEN(message_length,message)
  message->gas_id = gas_id;
  /* Send the message */
  (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

}

/*===========================================================================

FUNCTION  L1_send_MPH_SUSPEND_GSM_MODE_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_SUSPEND_GSM_MODE_IND ( gas_id_t gas_id )
{
  mph_suspend_gsm_mode_ind_T      message_buffer,*message;
  int                             message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* clear the data */
  memset(message,'\0',sizeof(mph_suspend_gsm_mode_ind_T));

  /* Set up the message header */
  message->message_header.message_set = MS_RR_L1;
  message->message_header.message_id  = MPH_SUSPEND_GSM_MODE_IND;    /*lint !e641 */
  message_length = sizeof(mph_suspend_gsm_mode_ind_T) - sizeof(IMH_T); /*lint !e778 const expression = 0 */
  message->gas_id = gas_id;
  PUT_IMH_LEN(message_length,message)

  /* Send the message */
  (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

}
#endif //FEATURE_WTOG_BACKGROUND_PLMN_SEARCH

/*===========================================================================

FUNCTION  L1_send_MPH_STOP_GSM_MODE_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_MPH_STOP_GSM_MODE_CNF ( gas_id_t gas_id )
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_STOP_GSM_MODE_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_STOP_GSM_MODE_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
  mph_stop_gsm_mode_cnf_T         message_buffer,*message;
  int                             message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* clear the data */
  memset(message,'\0',sizeof(mph_stop_gsm_mode_cnf_T));
  message->gas_id = gas_id;
  /* Set up the message header */
  message->message_header.message_set = MS_RR_L1;
  message->message_header.message_id  = MPH_STOP_GSM_MODE_CNF;  /*lint !e641 */
  message_length = sizeof(mph_stop_gsm_mode_cnf_T) - sizeof(IMH_T); /*lint !e778 evaluates to 0 */
  PUT_IMH_LEN(message_length,message)

  MSG_GERAN_MED_0_G("Task inactive, stop sleep until cnf sent");
  gl1_drx_task_inactive(gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  l1_tsk_buffer[gas_id].stop_gsm_mode_cnf_sent = TRUE;
#endif

#ifdef GL1_GMSK_MOD_DET
   if(decStat_pscan[gas_id] != NULL)
   {
     MSG_GERAN_HIGH_1_G("Free Memory at scan cnf :%2x", decStat_pscan[gas_id]);   
     GPRS_MEM_FREE(decStat_pscan[gas_id]);
     decStat_pscan[gas_id] = NULL;
   }
#endif


  /* Send the message */
  (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif
}

/*===========================================================================

FUNCTION  L1_send_MPH_START_IDLE_MODE_CNF

DESCRIPTION
  Creates the above named message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_START_IDLE_MODE_CNF(gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_START_IDLE_MODE_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_START_IDLE_MODE_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
  mph_start_idle_mode_cnf_T       message_buffer,*message;
  int                             message_length;
  gs_status_T                     GSstatus;

  /* Align the pointers */
  message = &message_buffer;

  /* clear the data */
  memset(message,'\0',sizeof(mph_start_idle_mode_cnf_T));
  message->gas_id = gas_id;
  /* Set up the message header */
  message->message_header.message_set = MS_RR_L1;
  message->message_header.message_id  = MPH_START_IDLE_MODE_CNF;  /*lint !e641 */
  message_length = sizeof(mph_start_idle_mode_cnf_T) - sizeof(IMH_T);  /*lint !e778 const expression = 0 */
  PUT_IMH_LEN(message_length,message)

  /* Send the message */
  GSstatus = L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

  /* log any failures */

  if(GSstatus != GS_SUCCESS)
  {
#if defined(HL_DEBUG)
     #error code not present
#else
     MSG_GERAN_ERROR_0_G(" Failed to send MPH_START_IDLE_MODE_CNF message ");

#endif
  }

/*lint -esym(550, GSstatus) suppress 'GSstatus' not accessed */
#endif /*FEATURE_GL1_GPLT*/
}

/*===========================================================================

FUNCTION  L1_send_MPH_ABORT_RA_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_ABORT_RA_CNF( gas_id_t gas_id,gl1_mode_t l1_mode)
{
   /* Locals */
   mph_abort_ra_cnf_T           message_buffer,*message;
   int                          message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_abort_ra_cnf_T));

      /* Set up the message header */   
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = MPH_ABORT_RA_CNF; /*lint !e641 */
   message_length = sizeof(mph_abort_ra_cnf_T) - sizeof(IMH_T); /*lint !e778 const expression = 0 */
   
   PUT_IMH_LEN(message_length,message)

   message->gas_id = gas_id;
   message->l1_mode = l1_mode;

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_RESET_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_RESET_IND( gas_id_t gas_id )
{
   /* Locals */
   mph_reset_ind_T           message_buffer,*message;
   int                           message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_reset_ind_T));

      /* Set up the message header */
   message->gas_id = gas_id;
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = MPH_RESET_IND; /*lint !e641 */
   message_length = sizeof(mph_reset_ind_T) - sizeof(IMH_T); /*lint !e778 const expression = 0 */
   PUT_IMH_LEN(message_length,message)

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}

/*===========================================================================

FUNCTION  L1_send_MPH_VOICE_SAMPLE_RATE_CHANGE_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_VOICE_SAMPLE_RATE_CHANGE_IND( uint32 sample_rate, gas_id_t gas_id )
{
   /* Locals */
   mph_voice_sample_rate_change_ind_T message_buffer, *message;
   int                                message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_voice_sample_rate_change_ind_T));

   /* Set up the message header */
   message->gas_id = gas_id;
   message->sample_rate = sample_rate;

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = MPH_VOICE_SAMPLE_RATE_CHANGE_IND;
   message_length = sizeof(mph_voice_sample_rate_change_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length, message)

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR, message, TRUE, gas_id);
}

/*===========================================================================

FUNCTION  L1_send_MPH_RESET_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_RESET_CNF( gas_id_t gas_id )
{
   /* Locals */
   mph_reset_cnf_T           message_buffer,*message;
   int                           message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_reset_cnf_T));

      /* Set up the message header */
   message->gas_id = gas_id;
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = MPH_RESET_CNF; /*lint !e641 */
   message_length = sizeof(mph_reset_cnf_T) - sizeof(IMH_T); /*lint !e778 const expression = 0 */
   PUT_IMH_LEN(message_length,message)

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  L1_send_MPH_L1_DS_ABORT_IND

DESCRIPTION
  Tell RR that a Dual Standby abort has occurred

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_L1_DS_ABORT_IND(gas_id_t gas_id, rr_ds_abort_type_T abort_cause, uint32 timeout)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
   volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   gl1_trm_state_t trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
#endif
   mph_l1_ds_abort_ind_T message_buffer,*message;
   int                   message_length;

   /* Set up pointer to local message buffer */
   message = &message_buffer;

   /* Initialise the buffer */
   memset(message,0,sizeof(mph_l1_ds_abort_ind_T));

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = (byte)MPH_L1_DS_ABORT_IND;

   message_length = sizeof(mph_l1_ds_abort_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Complete the payload */
   message->gas_id     = gas_id;
   message->abort_type = abort_cause;
   message->timeout_ms = timeout;

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
   if ((abort_cause == RR_DS_ABORT_TIMEDOUT) && (trm_status == GL1_TRM_CALLBACK_PENDING))
   {
     gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TRM_LOCK_WAIT_TIMEOUT);
   }
#endif
}

/*===========================================================================

FUNCTION  L1_send_MPH_PREEMPTION_IND

DESCRIPTION
  Advise GRR that a pre-emption notification has been received

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_PREEMPTION_IND(rr_l1_preemption_t preemption_type, gas_id_t gas_id)
{
  mph_preemption_ind_T message_buffer,*message;
  int                  message_length;

  /* Set up pointer to local message buffer */
  message = &message_buffer;

  /* Initialise the buffer */
  memset(message,0,sizeof(mph_preemption_ind_T));

  /* Set up the message header */
  message->message_header.message_set = MS_RR_L1;
  message->message_header.message_id  = (byte)MPH_PREEMPTION_IND;

  message_length = sizeof(mph_preemption_ind_T) - sizeof(IMH_T);
  PUT_IMH_LEN(message_length,message)

  /* Complete the payload */
  message->preemption_type = preemption_type;
  message->gas_id          = gas_id;

  /* Send the message */
  (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

  return;
}

/*===========================================================================

FUNCTION  L1_send_MPH_ACQUISITION_STATE_IND

DESCRIPTION
  Advise GRR that the acquisition (FIND_BCCH) state is changing

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_ACQUISITION_STATE_IND(rr_l1_acq_state_t acq_state, geran_priority_t acq_priority, gas_id_t gas_id)
{
  mph_acquisition_state_ind_T message_buffer,*message;
  int                         message_length;

  /* Set up pointer to local message buffer */
  message = &message_buffer;

  /* Initialise the buffer */
  memset(message,0,sizeof(mph_acquisition_state_ind_T));

  /* Set up the message header */
  message->message_header.message_set = MS_RR_L1;
  message->message_header.message_id  = (byte)MPH_ACQUISITION_STATE_IND;

  message_length = sizeof(mph_acquisition_state_ind_T) - sizeof(IMH_T);
  PUT_IMH_LEN(message_length,message)

  /* Complete the payload */
  message->rr_l1_acq_state  = acq_state;
  message->current_priority = acq_priority;
  message->gas_id           = gas_id;

  /* Send the message */
  (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

  return;
}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION  L1_SEND_MPH_FREQUENCY_REDEFINITION_IND

DESCRIPTION
  Tell RR L1 has finished with the frequency redefinition parameters.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_FREQUENCY_REDEFINITION_IND( boolean channels_in_same_band, boolean starting_time_elapsed, gas_id_t gas_id )
{

   /* Locals */

   mph_frequency_redefinition_ind_T      message_buffer,*message;
   int                       message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_frequency_redefinition_ind_T));

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id  = MPH_FREQUENCY_REDEFINITION_IND;
   message->channels_in_same_band      = channels_in_same_band;
   message->starting_time_elapsed      = starting_time_elapsed;
   message->gas_id = gas_id;
   message_length = sizeof(mph_frequency_redefinition_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

   
}

/*===========================================================================

FUNCTION  L1_send_MPH_COEX_MSC_CHANGE_IND

DESCRIPTION
  This function sends MPH_COEX_MSC_CHANGE_IND to GRR

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void L1_send_MPH_COEX_MSC_CHANGE_IND(uint8 ul_slot_restriction, uint8 dl_slot_restriction, gas_id_t gas_id)
{

   /* Locals */

   mph_coex_msc_change_ind_T message_buffer,*message;
   int                       message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* clear the data */
   memset(message,'\0',sizeof(mph_coex_msc_change_ind_T));

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_COEX_MSC_CHANGE_IND;
   message_buffer.ul_slot_restriction = ul_slot_restriction;
   message_buffer.dl_slot_restriction = dl_slot_restriction;
   message->gas_id = gas_id;
   message_length = sizeof(mph_coex_msc_change_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

}

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================

FUNCTION  L1_send_MPH_NV_REFRESH_CNF

DESCRIPTION
  This function sends L1_send_MPH_NV_REFRESH_CNF to GRR

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void L1_send_MPH_NV_REFRESH_CNF(gas_id_t gas_id)
{
   /* Locals */
   mph_nv_refresh_cnf_T message_buffer,*message;
   int                  message_length;

   /* Set up pointer to local message buffer */
   message = &message_buffer;

   /* Initialise the buffer */
   memset(message,'\0',sizeof(mph_nv_refresh_cnf_T));

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = (byte)MPH_NV_REFRESH_CNF;
   message_length = sizeof(mph_nv_refresh_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Complete the payload */
   message->gas_id = gas_id;
   
   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

