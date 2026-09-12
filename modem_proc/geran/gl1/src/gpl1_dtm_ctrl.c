/*=========================================================================
L 1   D U A L  T R A N S F E R  M O D E  C O N T R O L



GENERAL DESCRIPTION
   This module contains the procedures to handle the ISR level processing
   of DUAL TANSFER MODE

EXTERNALIZED FUNCTIONS
 gpl1_dtm_control()

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2005-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dtm_ctrl.c#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
20/03/17   mk       CR2020999 Prx Drx power difference needs to be calculated & applied to NCELLs before REP_PERIOD_IND to avoid delays
08/12/16   mk       CR1097591 Enhancement for DRX: DRx and PRx difference will be applied to neighbor cell measurements, if DRx power level is greater than PRx
08/12/16   mk       CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
11/03/15   pa       CR805850: Validate Rx signal strength before averaging expected power and signal strength. 
04/08/14   sp       CR703980: Fix Compile error on disabling feature 
24/07/13   jk       CR464982:Changes to support GEA5_4 ciphering
15/02/13   ws       CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
20/05/10   ap       CR 238752  fix problem with DTM frame inc and reconnec
02/10/09   ws       CR210956 - Divide By Zero exception
23/09/09   ps       Resolve Klocwork errors
05/03/07   cs       Making CR112989 calculation consistent with sc updates
07/12/06   og       Fix for CR107465. Ensure the tdma frame is incremented when
                    TS6(7) to TS6(7) transitions occur.
22/11/06   og       Reverse the order of stopping SCE and setting the dedicated
                    stop main command. Resolves CR105367.
31/10/06   og       Ensure the AMR multirate configuration is updated in the
                    L1_EDTM_INIT.
21/09/06   og       Reset only the cs part of lower layers when connecting DTM
                    assignment in packet transfer (EDTM route).
07/09/06   og       EDTM updates following testing on Agilent 8960.
01/09/06   og       Call l1_change_channel( ) correctly.
31/08/06   og       EDTM updates following host test simulator testing.
24/08/06   og       EDTM updates following host test simulator testing.
17/08/06   og       Basic featured-off EDTM code drop.
14/08/06   ws       Allow dedicated to be timeslot slammed to lowest received tn
                    and also to place the CS timeslot on TN 2
04/11/05   nt       Updates L1_DTM_INIT state to include L1_DYNAMIC_DOWN_ALLOC setup
02/11/05   nt       Added code to handle the DTM_ASSIGNMENT with DTM mode
15/09/05   ws       Added dummy function to avoid warning when FEATURE_GSM_DTM
                    is not defined
01/09/05   ws       Linted and code cleanup for FEATURE_GSM_DTM
12/08/05   ws       Updated for initial DTM release
08/08/05   og       DTM code changes.
05/07/05   ws       Initial Revision

===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GSM_DTM
#include "l1i.h"
#include "l1_isr.h"
#include "l1_utils.h"
#include "l1_task.h"

#include "rr_l2.h" /* for L2 reason in l1_notify_start() */
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
extern void L1_sync_to_target  ( l1_sc_bs_T *current_cell, l1_sc_bs_T *target_cell, gas_id_t gas_id );
extern void L1_sync_to_current ( l1_sc_bs_T *current_cell, gas_id_t gas_id );

extern dedicated_data_T l1_dedicated_data[];


/*===========================================================================

FUNCTION  GPL1_DTM_CONTROL()

DESCRIPTION
  Controls the ISR allocation for Dual Transfer Mode
DEPENDENCIES


RETURN VALUE
  dtm mode


SIDE EFFECTS
  None

===========================================================================*/
l1_dtm_mode_T gpl1_dtm_control( gas_id_t gas_id )
{
#ifdef  FEATURE_GSM_EDTM
  idle_data_T             *l1_idle_data = &l1_idle_data_store[gas_id];
#endif /* FEATURE_GSM_EDTM */
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T        *ded_data;
  transfer_data_T         *transfer_data;
  dtm_data_T              *dtm_data;
  l1_dtm_mode_T           idle = L1_DTM_NONE;
  l2_channel_type_T       logical_channel_type;
  boolean                 apply_freq_redef = FALSE;

  /* point at the data supplied by the L1 task */
  ded_data = l1_tskisr_blk->current_params.L1Data.pDedi_data;
  transfer_data = l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  dtm_data = l1_tskisr_blk->current_params.L1Data.pDtm_data;



  /* Frequency redefinition  imminent ?? */

  if ((ded_data->freq_redef_imminent)&&
      (L1_determine_time(frame_counters[gas_id].FNmod42432,ded_data->freq_redef_starting_time)) )
  {
    /* It is now time to change the channel info */
    ded_data->channel_specification.channel_info_1_after =
      ded_data->new_channel_info;

    /* @@@@@ Need to work out how to handle PS side for Frequency desc.@@@@@*/

    ded_data->freq_redef_imminent = FALSE;

    /*
       This flag is enabled for just a frame and informs the TCH that
       the frequencies are to be switched in this frame because the
       frequency redefinition start time has elapsed
     */
     apply_freq_redef = TRUE;
  }

  switch (l1_tskisr_blk->sub_state)
  {

#ifdef  FEATURE_GSM_EDTM

    /* @@OG: EDTM design */
    case L1_EDTM_INIT:
    {

      /* Setup CS intialization */
      if( (dtm_data->l1_dtm_init_mode == L1_DTM_CS_ONLY) ||
          (dtm_data->l1_dtm_init_mode == L1_DTM_CS_PS) )
      {
          /* Transfer the AGC information from IDLE to DEDICATED */
          ded_data->c0_agc = l1_idle_data->campedon_cell_agc;

          sys_algo_agc_enable_slew_rate_limiting(&ded_data->c0_agc);

#ifdef FEATURE_GSM_RX_DIVERSITY
          if (gl1_get_IMRD_ctl_nv(gas_id)) 
          {
            ded_data->agc_divrx = l1_idle_data->campedon_cell_agc_divrx; 
            sys_algo_agc_enable_slew_rate_limiting(&ded_data->agc_divrx);
          }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

         /* initialise agc averaging over 4 bursts
         * this may be overwritten to 2 bursts later
         * if we get assigned Half rate
         */
         sys_algo_agc_avg_init(FOUR_BURST_AVG, gas_id);
         l1_ded_drx_agc_init[gas_id] = FALSE;

        /* Copy the new cs channel spec into the current
         * channel spec.
         * @@WARNING: This might not be necessary if I handle the dtm chan assign msg in pkt transfer
         * in the same way dedicated data is handled in rach_if.
         */
        ded_data->channel_specification = ded_data->new_channel_specification;

        /* Enable SACCH meas reporting when starting up a new channel.
           L2 disables it later before releasing the channel
         */
        ded_data->stop_meas_rpt = FALSE;

#ifdef FEATURE_GSM_AMR

        /* Copy the new amr configuration into the current */
        ded_data->multirate_configuration = ded_data->new_multirate_configuration;

#endif

        /* Set up CS state machine */
        l1_tskisr_blk->sub_sub_state = L1_EDTM_INIT;

        /* Slam our timebase to ensure CS slot is on TN 2 */
        L1_change_channel(ded_data,
                          &(ded_data->channel_specification.
                          channel_info_1_after),
                          TRUE, gas_id );

      } /* end of if L1_DTM_CS_ONLY, L1_DTM_CS_PS */

      /* Setup PS reconfiguration
       * The if( ) statement checks are probably un-necessary.
       */
      if( (dtm_data->l1_dtm_init_mode == L1_DTM_PS_ONLY) ||
          (dtm_data->l1_dtm_init_mode == L1_DTM_CS_PS) )
      {

        /* Setup pending PS allocation */
        (void) gpl1_gprs_handle_reconfig(transfer_data,
                                         L1_MODE_RUN,
                                         L1_DTM_MODE, gas_id );
      }

      if (
           ded_data->channel_specification.channel_info_1_after.channel_type ==
           FULL_RATE_TRAFFIC
         )
      {
        logical_channel_type = FACCH_F;
      }
      else
      {
        logical_channel_type = FACCH_H;
      }

      /*Start the SCE*/
      l1_sci_go(gas_id);

      /* Make sure this message is sent to MAC before RR knows about the
       * CS connection.
       */
      if( MPH_DTM_CHANNEL_ASSIGN_CNF == ded_data->L1_confirm_msg )
      {
        gpl1_send_mac_ph_cs_connect_ind( ded_data->channel_specification.channel_info_1_after.TN,gas_id );
      }

      /* Send confirm to RR and L2 */
      L1_notify_start(ded_data->channel_specification.
                      channel_info_1_after.channel_type,
                      logical_channel_type,
                      ded_data->L1_confirm_msg,
                      NO_ERROR, gas_id);

      /* send MAC_PH_CONNECT_IND to MAC */
      gpl1_gprs_notify_start(transfer_data, gas_id);

      /* Now schedule the PS and CS assignment */
      idle =  gpl1_dtm_control_cs_ps(dtm_data,apply_freq_redef, gas_id);

      /* Reset the DTM abort and init modes */
      dtm_data->l1_dtm_init_mode  = L1_DTM_NONE;
      dtm_data->l1_dtm_abort_mode = L1_DTM_NONE;

    } /* switch case L1_EDTM_INIT */
    break;

#endif /* FEATURE_GSM_EDTM */

    case L1_DTM_INIT:
    {
      /* Setup CS intialization */
      if( (dtm_data->l1_dtm_init_mode == L1_DTM_CS_ONLY) ||
          (dtm_data->l1_dtm_init_mode == L1_DTM_CS_PS) )
      {
        /* Enable SACCH meas reporting when starting up a new channel.
           L2 disables it later before releasing the channel
         */
        ded_data->stop_meas_rpt = FALSE;

        /* Reset the bad sacch block count
           - use this for PANIC RESET if too many SACCH decodes fail
        */
        //l1_bad_sacch_block_cnt = 0;


        /* Set up CS state machine */
        l1_tskisr_blk->sub_sub_state = L1_DED_INIT; 


        /* Slam our timebase to ensure CS slot is on TN 2 */
        L1_change_channel(ded_data,
                          &(ded_data->channel_specification.
                          channel_info_1_after),
                          TRUE, gas_id);

      } /* end of if L1_DTM_CS_ONLY, L1_DTM_CS_PS */

      /* Setup PS initialization */
      if( (dtm_data->l1_dtm_init_mode == L1_DTM_PS_ONLY) ||
          (dtm_data->l1_dtm_init_mode == L1_DTM_CS_PS) )
      {
        transfer_data->packet_transfer_agc = l1_idle_data->campedon_cell_agc;
        sys_algo_agc_enable_slew_rate_limiting(&transfer_data->packet_transfer_agc);
        MSG_GERAN_MED_1_G("AGC pwr_dBm_x16 %d ",
                             transfer_data->packet_transfer_agc.pwr_dBm_x16);

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
        if (gl1_get_IMRD_ctl_nv(gas_id))
        {
          transfer_data->packet_transfer_div_agc = l1_idle_data->campedon_cell_agc_divrx; 
          sys_algo_agc_enable_slew_rate_limiting(&transfer_data->packet_transfer_div_agc);
          MSG_GERAN_MED_1_G(GL1_RXD"DivRx AGC pwr_dBm_x16 %d ",
                             transfer_data->packet_transfer_div_agc.pwr_dBm_x16);
        }
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/


        switch (transfer_data->current_alloc_type)
        {

          /* case for doing only dynamic allocation UL */
          case L1_DYNAMIC_ALLOC:
          {

            /* set dynamic_tbf_ptr to point at pending_dynamic_tbf_ptr (current structure)*/
            transfer_data->dynamic_tbf_ptr = transfer_data->pending_dynamic_tbf_ptr;

            /* Fix Klocwork error */
            if( NULL != transfer_data->dynamic_tbf_ptr )
            {
               /* Set downlink tbf_pointer to NULL as this is a single UL TBF */
               transfer_data->dynamic_tbf_ptr->dl_tbf_ptr = NULL;
            }
            else
            {
               MSG_GERAN_ERROR_0_G(" NULL dynamic_tbf_ptr pointer ! ");
            }

            /* No need to setup frequency info ptr since in DTM we use the CS
             * frequency channel description
             */
            transfer_data->frequency_info_ptr = NULL;


            if ( TBF_MODE_EGPRS == transfer_data->egprs_or_gprs_tbf )
            {
              /* Configure the EGPRS qual meas module.
               * bep period2 is supplied in the msg, the qual
               * meas module would not pay attention to it unless bep
               * meas are enabled.
               */
              if( NULL != transfer_data->dynamic_tbf_ptr )
              {
                gpl1_egprs_scell_qual_meas_pacch_config( INVALID_MEAS_MODE,
                                                         0x0,
                                                         transfer_data->
                                                         dynamic_tbf_ptr->bep_period2, gas_id );
              }
              else
              {
                MSG_GERAN_ERROR_0_G(" NULL dynamic_tbf_ptr pointer ! ");
              }

              /* Configure the inerference measurements module */
              gpl1_egprs_scell_interf_meas_config( INVALID_MEAS_MODE, gas_id );
            }
            break;
          } /* End of case Dynamic alloc */

          case L1_DOWNLINK_ALLOC:
          {

            /* set dl_tbf_ptr to point at dl_tbf_1 (current structure)*/
            transfer_data->dl_tbf_ptr = transfer_data->pending_dl_tbf_ptr;

            /* No need to setup frequency info ptr since in DTM we use the CS
             * frequency channel description
             */
            transfer_data->frequency_info_ptr = NULL;


            /* set up dynamic pointer to access downlink allocation */
            transfer_data->dynamic_tbf_ptr  = &(transfer_data->dynamic_tbf_1);

            /* preset all USF's to unallocated since we are dl tbf only */
            memset(
                    (transfer_data->dynamic_tbf_1.usf_value),
                     GPL1_INVALID,
                     sizeof( uint8[8] )
                  );

            /* Disable USF granulairty as we are DL TBF only */
            transfer_data->dynamic_tbf_ptr->usf_granularity=FALSE;

            /* preset number of timeslots to zero to stop dynamic tx's being scheduled */
            transfer_data->dynamic_tbf_1.nts = 0;

            /* set up downlink pointer within dynamic structure */
            transfer_data->dynamic_tbf_ptr->dl_tbf_ptr = transfer_data->dl_tbf_ptr;

            /* Check that all RRBP's are in the future */
            gpl1_gprs_delete_oldest_frame_num(gl1_get_FN( gas_id ), gas_id);

            if ( TBF_MODE_EGPRS == transfer_data->egprs_or_gprs_tbf )
            {
              /* Configure the EGPRS quality measurements module.
               * This is due to a DL TBF CONFIG MSG hence both
               * link qual meas mode and bep period are supplied.
               */
              if( NULL != transfer_data->dl_tbf_ptr )
              {
                gpl1_egprs_scell_qual_meas_pacch_config
                                          (
                                            transfer_data->dl_tbf_ptr->link_qual_meas_mode,
                                            transfer_data->dl_tbf_ptr->ts_allocation,
                                            transfer_data->dl_tbf_ptr->bep_period2, gas_id
                                          );
              }
              else
              {
                MSG_GERAN_ERROR_0_G(" NULL dl_tbf_ptr ! ");
              }

              /* Configure the interference measurements module
               * - NO Interference meas in DTM
               */
              gpl1_egprs_scell_interf_meas_config(INVALID_MEAS_MODE, gas_id);

            } /* End of if tbf assignment is EGPRS */
            break;
          } /* End of case downlink allocation */

          case L1_DYNAMIC_DOWN_ALLOC:
          {

            /* set dynamic_tbf_ptr to point at pending_dynamic_tbf_ptr (current structure)*/
            transfer_data->dynamic_tbf_ptr = transfer_data->pending_dynamic_tbf_ptr;

            /* set dl_tbf_ptr to point at pending_dl_tbf_ptr (current structure)*/
            transfer_data->dl_tbf_ptr = transfer_data->pending_dl_tbf_ptr;

            /* No need to setup frequency info ptr since in DTM we use the CS
             * frequency channel description
             */
            transfer_data->frequency_info_ptr = NULL;

           /* set up downlink pointer within dynamic structure */
            transfer_data->dynamic_tbf_ptr->dl_tbf_ptr = transfer_data->dl_tbf_ptr;

            /* Check that all RRBP's are in the future */
            gpl1_gprs_delete_oldest_frame_num(gl1_get_FN( gas_id ), gas_id);

            if ( TBF_MODE_EGPRS == transfer_data->egprs_or_gprs_tbf )
            {
              /* Configure the EGPRS quality measurements module.
               * This is due to a DL TBF CONFIG MSG hence both
               * link qual meas mode and bep period are supplied.
               */
              if( NULL != transfer_data->dl_tbf_ptr )
              {
                gpl1_egprs_scell_qual_meas_pacch_config
                                          (
                                            transfer_data->dl_tbf_ptr->link_qual_meas_mode,
                                            transfer_data->dl_tbf_ptr->ts_allocation,
                                            transfer_data->dl_tbf_ptr->bep_period2, gas_id
                                          );
              }
              else
              {
                MSG_GERAN_ERROR_0_G(" NULL dl_tbf_ptr ! ");
              }

              /* Configure the interference measurements module
               * - NO Interference meas in DTM
               */
              gpl1_egprs_scell_interf_meas_config(INVALID_MEAS_MODE, gas_id);

            } /* End of if tbf assignment is EGPRS */
            break;
          } /* End of case Dynamic downlink alloc */

          default:

            MSG_GERAN_ERROR_1_G("PL1: Incorrect alloc type %d",
                                                  transfer_data->current_alloc_type);
            break;

        } /* end of switch on current allocation type */


      } /* end of if L1_DTM_PS_ONLY, L1_DTM_CS_PS */

      if (
           ded_data->channel_specification.channel_info_1_after.channel_type ==
           FULL_RATE_TRAFFIC
         )
      {
        logical_channel_type = FACCH_F;
      }
      else
      {
        logical_channel_type = FACCH_H;
      }

      /*Start the SCE*/
      l1_sci_go(gas_id);

#ifdef  FEATURE_GSM_EDTM
      /* Make sure this message is sent to MAC before RR knows about the
       * CS connection.
       */
      if( MPH_DTM_CHANNEL_ASSIGN_CNF == ded_data->L1_confirm_msg )
      {
        gpl1_send_mac_ph_cs_connect_ind( ded_data->channel_specification.channel_info_1_after.TN,gas_id );
      }

#endif

      /* Send confirm to RR and L2 */
      L1_notify_start(ded_data->channel_specification.
                      channel_info_1_after.channel_type,
                      logical_channel_type,
                      ded_data->L1_confirm_msg,
                      NO_ERROR, gas_id);

      /* send MAC_PH_CONNECT_IND to MAC */
      gpl1_gprs_notify_start(transfer_data, gas_id);

      /* Now schedule the PS and CS assignment */
      idle =  gpl1_dtm_control_cs_ps(dtm_data,apply_freq_redef, gas_id);

      /* Reset the DTM abort and init modes */
      dtm_data->l1_dtm_init_mode = L1_DTM_NONE;
      dtm_data->l1_dtm_abort_mode = L1_DTM_NONE;
      break;

    } /* end of case L1_DTM_INIT */



    case L1_TRANSFER_RECONFIG:
    {
      /* send MAC_PH_CONNECT_IND to MAC */
      gpl1_gprs_notify_start(transfer_data, gas_id);

      /* Setup pending PS allocation */
      (void)gpl1_gprs_handle_reconfig(transfer_data,
                                       L1_MODE_RUN,
                                       L1_DTM_MODE, gas_id);

      idle =  gpl1_dtm_control_cs_ps(dtm_data,apply_freq_redef, gas_id);

      break;
    }

    case L1_DTM_RUNNING:

      /* Send MAC_PH_CONNECT_IND to MAC  -  MAY NEED THIS SINCE
       * ST MAY BE 1,2 FRAMES LATER THAN RECONFIG FRAME - Make sure that
       * this cannot get sent whilst the CS PS is being aborted. This can
       * cause premature PH_CONNECT_IND's going to MAC when a DTM_ASSIGNMENT
       * is being processed
       */
      if ( dtm_data->l1_dtm_abort_mode != L1_DTM_CS_PS )
      {
        gpl1_gprs_notify_start(transfer_data, gas_id);
      }

      idle =  gpl1_dtm_control_cs_ps(dtm_data,apply_freq_redef, gas_id);

      break;

    default:

      MSG_GERAN_ERROR_1_G("PL1: Bad L1 ss %d",l1_tskisr_blk->sub_state);
      break;
  }


  /* Execute any commands that can occur during dual transfer mode */

  if (l1_tskisr_blk->command == L1_SYNCH_TO_TARGET_COMMAND)
  {
    L1_sync_to_target(&ded_data->current_cell_details,
                      &ded_data->target_cell_details, gas_id);

    l1_tskisr_blk->command = L1_NULL_COMMAND;
  }

  if ( l1_tskisr_blk->command == L1_SYNCH_TO_CURRENT_COMMAND)
  {
    L1_sync_to_current(&ded_data->current_cell_details, gas_id);

    l1_tskisr_blk->command = L1_NULL_COMMAND;
  }

  if (( l1_tskisr_blk->command == L1_SET_CIPHERING_COMMAND)||
      ( l1_tskisr_blk->command == L1_SET_HANDVR_CIPH_COMMAND))
  {
    /*enable or disable ciphering*/
    l1_set_cch_ciphering_flag(ded_data->channel_specification.cipher_flag, gas_id);

    if (l1_get_cch_ciphering_flag(gas_id))
    {
      gl1_defs_cipher_mode mode;

      mode = (gl1_defs_cipher_mode) ded_data->channel_specification.cipher_algorithm;
      if((mode == A5_1)||(mode == A5_2)||(mode == A5_3))
      {
       gl1_msg_enable_ciphering( ded_data->cipher_key, mode, gas_id);
      }
      else
      {
        gl1_msg_enable_ciphering(ded_data->cipher_key_128, mode, gas_id);
      }

      MSG_GERAN_MED_0_G("L1: Ciphering enabled");
    }
    else
    {
      gl1_msg_disable_ciphering(gas_id);
      MSG_GERAN_MED_0_G("L1: Ciphering disabled");
    }

    if ( l1_tskisr_blk->command == L1_SET_CIPHERING_COMMAND)
    {
      /*send confirmation to RR*/

      /*
       * This won't actually send the message, as it is sent directly 2 lines
       * below, but it is used to signal some other stuff...
       */

      ded_data->L1_confirm_msg = (byte)MPH_SET_CIPHERING_CNF;

      L1isr_send_MPH_SET_CIPHERING_CNF( NO_ERROR, gas_id );
    }

    l1_tskisr_blk->command = L1_NULL_COMMAND;

  } /* End of if  l1_tskisr_blk->command .. */



  return( idle );

}


#else

/* Dummy function to avoid warning when FEATURE_GSM_DTM is NOT defined */
void gpl1_dtm_ctl_dummy(void)
{
}

#endif /* FEATURE_GSM_DTM */
