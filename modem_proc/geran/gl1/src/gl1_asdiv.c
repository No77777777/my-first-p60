/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     GERAN L1 ASDIV FILE

GENERAL DESCRIPTION
This module handles ASDIV specific functions.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_asdiv.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when      who       what, where, why
--------  --------  ---------------------------------------------------------- 
03/04/17   br        CR2027088 Band based ASDIV support during ACQ
02/06/17   sk        CR1116408 Do not order RESCAN as a part of ASDIV probe if GL1 is indicated to abort or deactivate stack.
06/09/15   br        CR1063442 FR 38050: AsDiv in Acquisition for CSFB in multisimode.
03/11/15   ss        CR916273 FR 29852: AsDiv in Acquisition
===========================================================================*/

/*===========================================================================
           INCLUDE FILES FOR MODULE
===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "ms.h"
#include "geran_multi_sim.h"
#include "gl1_asdiv.h"
#include "l1_isr.h"
#include "gpl1_dual_sim.h"
#include "l1_sc_int.h"
#include "l1_sc.h"
#include "l1_task.h"
#include "gl1_mutex.h"
#include "gmutex.h"
#include "gpl1_dual_sim.h"
#include "gl1_sys_params.h"
#include "gl1_msgi.h"


#ifdef FEATURE_GL1_ASDIV_ACQ 

#define L1_SC_PWR_MAX_MEAS_PER_CALL        100

/*================Externs=======================*/
extern volatile ISRTIM_CMD_BLK   *l1_isrtsk_blk;
extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];
extern BCCH_List_T l1_BCCH_List[];
extern int16 tx_div_min_delta_btw_antenna;

/** Structure controlling Probe Antenna Selection during power
 *  meas.*/
extern uint32  antenna_to_use[L1_SC_PWR_MAX_MEAS_PER_CALL]; /*L1_SC_PWR_MAX_MEAS_PER_CALL = 100*/

extern void l1_pscan_reset_freq_completed(gas_id_t gas_id);


/*================Static Vars==================*/

/** Structure holding info reg AsDiv for Acquisition*/
static gl1_asdiv_acq_buff_t gl1_asdiv_acq_buff[NUM_GERAN_DATA_SPACES];
/** Structure holding Power Meas. AGC params for Monitors done
 *  in AsDiv Acq.*/
static gl1_asdiv_acq_pwr_meas_buff_t gl1_asdiv_acq_pwr_meas_buff[NUM_GERAN_DATA_SPACES];

/* =========Function Definitions==========*/
/**===========================================================================

FUNCTION  gl1_asdiv_acq_enabled

DESCRIPTION
  This function tells if AsDiv in Acquisition state of GL1 is enabled or not

DEPENDENCIES
 As Set in NV
 At Present We check for Type 1 is enabled or Not
 
RETURN VALUE
  TRUE if enabled
  FALSE if Not

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_asdiv_acq_enabled( gas_id_t gas_id)
{
   /*At Present get the AsDiv Type 1 Enable NV 70245, 
     This NV is a Bit Mask, but for now we see if its set or not for Acq.*/
   boolean ret = antenna_swith_tx_div_nv_enable;
   MSG_GERAN_HIGH_1_G(GL1_ASDIV_HDR"gl1_asdiv_acq_enabled:%d", ret);

   /*If in MSim Mode, ret False*/
   if (gl1_msg_get_multi_sim_mode() && (grm_get_resource_lock_state(gas_id) == FALSE))
   {
     ret = FALSE;
   }
   return ret; 
}
/**===========================================================================

FUNCTION  gl1_asdiv_acq_band_supported

DESCRIPTION
  This function tells if AsDiv is supported for that band or not

DEPENDENCIES
 As Set in NV
 At Present We check for Type 1 is enabled or Not
 
RETURN VALUE
  TRUE if enabled
  FALSE if Not

SIDE EFFECTS
  None

===========================================================================*/


boolean gl1_acq_asdiv_band_supported(ARFCN_T arfcn,gas_id_t gas_id)
{
  boolean asdiv_enable = FALSE;
  sys_band_T curr_band = arfcn.band ;

   if((curr_band != INVALID_BAND) && (curr_band != SYS_BAND_NONE))
  {	
    asdiv_enable = (((1 << curr_band) & antenna_swith_tx_div_nv_enable) != 0);
    if (asdiv_enable)
   {
    MSG_GERAN_HIGH_2_G(GL1_ASDIV_HDR"ASDIV : curr_band = %d, band_mask = %d",  curr_band, antenna_swith_tx_div_nv_enable);
   }
  }
  return asdiv_enable;
}


/**===========================================================================

FUNCTION  gl1_asdiv_acq_diff_thresh

DESCRIPTION
  This function Gets the threshold value set in NV
  If probe antenna returns power greater than this threshold compared to currect antenna
  then switch should happen

DEPENDENCIES
 As Set in NV 70221 (Which is same as that set for Type 1 Switch)

RETURN VALUE
  Value set in NV in dBx16

SIDE EFFECTS
  None

===========================================================================*/
static int16 gl1_asdiv_acq_diff_thresh(gas_id_t gas_id)
{
    return tx_div_min_delta_btw_antenna;
}

/** ===========================================================================

FUNCTION  gl1_asdiv_acq_check_rescan

DESCRIPTION
  This function tells if a ReScan is initiated as
  part of AsDiv Algo. for Acq.
  This is to be done when Scan was for Acq. DB
 
DEPENDENCIES
 None

RETURN VALUE
  TRUE if enabled
  FALSE if Not

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_asdiv_acq_check_rescan(gas_id_t gas_id)
{
   boolean ret;
   ret = (gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event == GL1_ASDIV_ACQ_EV_RESCAN);
   if (ret) {
       MSG_GERAN_HIGH_1_G(GL1_ASDIV_HDR"gl1_asdiv_acq_check_rescan:%d", ret); 
   }
   return ret;
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_is_mon_active

DESCRIPTION
  Checks if Probe Monitors for AsDiv Acq. are to be Active
  i.e. to have prove burst when issuing Rx Commands to RF
 
DEPENDENCIES
 None

RETURN VALUE
  TRUE if active
  FALSE if Not

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_asdiv_acq_is_mon_active(gas_id_t gas_id)
{
    boolean ret;
    ret = ((gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event >= GL1_ASDIV_ACQ_EV_PROBE)&&
           (gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event <= GL1_ASDIV_ACQ_EV_BCCH_END));
    if (ret)
    {
        MSG_GERAN_HIGH_1_G(GL1_ASDIV_HDR"gl1_asdiv_acq_is_mon_active event: %d", gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event);
    }
    return ret;
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_update_params

DESCRIPTION
  Update the params stored in the gl1_asdiv_acq_buff After Finishing Power Scan
  to decide the future course of actions
 
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_update_params(gl1_asdiv_acq_event_t event,
                                 ARFCN_T best_arfcn,
                                 uint16 num_freq_valid,
                                 uint16 freq_list_length, 
                                 gas_id_t gas_id)
{

    if(num_freq_valid &&( gl1_acq_asdiv_band_supported(best_arfcn,gas_id)== FALSE))
    {
      MSG_GERAN_HIGH_3_G(GL1_ASDIV_HDR" best_arfcn: num %d band %d nv support %d",
	  best_arfcn.num,
	  best_arfcn.band,
	  antenna_swith_tx_div_nv_enable);
	  gl1_asdiv_acq_substate_exit(gas_id);
	  return;
    }
    /*Set Event to PSCAN END*/
    gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event = event;
    gl1_asdiv_acq_buff[gas_id].best_arfcn = best_arfcn;
    /*Update the params for AsDiv decision*/
    gl1_asdiv_acq_buff[gas_id].num_freq_valid = num_freq_valid;
    gl1_asdiv_acq_buff[gas_id].freq_list_length = freq_list_length;
    
    MSG_GERAN_HIGH_5_G(GL1_ASDIV_HDR"Updated params: event: %d num_freq_valid: %d freq_list_length: %d best_arfcn: num %d band %d", 
                          gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event,
                          gl1_asdiv_acq_buff[gas_id].num_freq_valid,
                          gl1_asdiv_acq_buff[gas_id].freq_list_length,
                          gl1_asdiv_acq_buff[gas_id].best_arfcn.num,
                          gl1_asdiv_acq_buff[gas_id].best_arfcn.band);
    
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_probe_callback

DESCRIPTION
  Callback After finishing Power/Probe Monitors on best ARFCN detected after PScan
  
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_probe_callback(gl1_msg_pwr_meas_result_type *result_data, gas_id_t gas_id)
{
    uint16 tmp, num_meas;
    dBx16_T probe_mon, pow_mon;
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    num_meas = result_data->num_pwr_meas;
    pow_mon = 0;
    probe_mon = 0;
    MSG_GERAN_MED_1_G(GL1_ASDIV_HDR"acq_probe_cb: num meas:%d", num_meas);
    
    //@TODO: add further logic to find yielded results
    //or invalid results here
    for (tmp = 0; tmp < num_meas; tmp++) 
    {
        MSG_GERAN_MED_2_G(GL1_ASDIV_HDR"Idx:%d power:%d", tmp, 
                          result_data->pwr_dBm_x16[tmp]);
        if (tmp < GL1_ASDIV_ACQ_NUM_MON_PROBE/2)
        {
            pow_mon = pow_mon + result_data->pwr_dBm_x16[tmp];
        }
        else
        {
            probe_mon = probe_mon + result_data->pwr_dBm_x16[tmp];
        }
    }/*for loop*/

    MSG_GERAN_HIGH_2_G(GL1_ASDIV_HDR"Accumalated Pow Mon Res:%d Prb Mon Res:%d",
                       pow_mon, probe_mon);

    /*Make the switch decision And switch if applicable*/
    if (probe_mon > (pow_mon + gl1_asdiv_acq_diff_thresh(gas_id))) 
    {
       MSG_GERAN_HIGH_0_G(GL1_ASDIV_HDR"Probe Monitor returns better resultsby threshold value, So Switch in Next State transition");
       
    }
    else
    {
       MSG_GERAN_HIGH_0_G(GL1_ASDIV_HDR"Power Monitor returns better results, No Switch");
       gl1_asdiv_acq_reset(gas_id); //Reset the states as Switch is not to be made on next state change
    }
    gl1_asdiv_acq_substate_exit(gas_id);
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_probe_abort_callback

DESCRIPTION
  Abort Callback, to handle when AsDiv related Monitors are being performed
  This prints an F3 to indicate that the PMs were aborted
 
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_probe_abort_callback(gas_id_t gas_id)
{
    MSG_GERAN_HIGH_0_G(GL1_ASDIV_HDR"gl1_asdiv_acq_probe_abort_callback: No Switch");
    gl1_asdiv_acq_substate_exit(gas_id);   
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_substate_exit

DESCRIPTION
  Force An Exit out of AsDiv Substate; to NULL Mode
 
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_substate_exit(gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  
    /* set main command to enter NULL Mode*/
    if (l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND) 
    {
       /* ... transition to Null Mode ... */
        MSG_GERAN_HIGH_0_G(GL1_ASDIV_HDR"Main command Set to Enter Null Command");
       l1_tskisr_blk->main_command = L1_ENTER_NULL_COMMAND;
    }
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_do_pwr_meas

DESCRIPTION
  Install power Meas Handlers for AsDiv for Acquisition
  Also Configures the Number of Monitors, Meas. per frame, Frames to meas. 
 
DEPENDENCIES
  Power Meas Handler must be configured prior to calling this function

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_do_pwr_meas(gl1_asdiv_acq_event_t event, gas_id_t gas_id)
{
    uint8 tmp;
    uint8 num_mon;
    uint8 mon_per_frame;
    uint8 num_frames_to_meas;
    gl1_asdiv_acq_pwr_meas_buff_t* gl1_asdiv_acq_pwr_meas_ptr = &gl1_asdiv_acq_pwr_meas_buff[gas_id];

    switch (event)
    {
        /*Probe Monitors Along with BCCH Bursts*/
        case GL1_ASDIV_ACQ_EV_BCCH:
        {
            gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event = GL1_ASDIV_ACQ_EV_BCCH;
            num_mon            = GL1_ASDIV_ACQ_NUM_MON_BCCH;
            mon_per_frame      = GL1_ASDIV_ACQ_MPF_BCCH;
            num_frames_to_meas = GL1_ASDIV_ACQ_NUM_FRM_BCCH;
            /*Set all monitors as Probes*/
            for (tmp = 0; tmp < GL1_ASDIV_ACQ_NUM_MON_BCCH; tmp++) {
                antenna_to_use[tmp] = 1;
            }
        }/*case GL1_ASDIV_ACQ_EV_BCCH*/
        break;

        /*Normal and Probe Monitors on Best Arfcn After PScan*/
        case GL1_ASDIV_ACQ_EV_PROBE:
        {
            gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event = GL1_ASDIV_ACQ_EV_PROBE;
            num_mon            = GL1_ASDIV_ACQ_NUM_MON_PROBE;
            mon_per_frame      = GL1_ASDIV_ACQ_MPF_PROBE;
            num_frames_to_meas = GL1_ASDIV_ACQ_NUM_FRM_PROBE;
            /*Set The Last three monitors as probe*/
            for (tmp = 0; tmp < GL1_ASDIV_ACQ_NUM_MON_PROBE; tmp++) 
            {
                if ( tmp < (GL1_ASDIV_ACQ_NUM_MON_PROBE/2)) 
                {
                    antenna_to_use[tmp] = 2; 
                }
                else
                {
                    antenna_to_use[tmp] = 1;
                }/*if-Else*/
            }/*For Loop*/
        }/*case GL1_ASDIV_ACQ_EV_PROBE*/
        break;

        default:
        {
          /*for any other event, Do not initiate Power Meas. and Return immed.*/
          MSG_GERAN_ERROR_1_G(GL1_ASDIV_HDR"Error: UnExpected Acq. Power Meas Event:%d", event);
          return;
        }
    }/*switch...*/

    /*Point to the Power MEas Buffer*/
    gl1_asdiv_acq_pwr_meas_ptr->asdiv_acq_pwr_probe_cb_param.pwr_dBm_x16 = gl1_asdiv_acq_pwr_meas_ptr->result_buffer;

    /*now populate the Arfcns and Init AGC Ptrs*/
    for (tmp = 0; tmp < GL1_ASDIV_ACQ_MAX_MEAS; tmp++) 
    {
        gl1_asdiv_acq_pwr_meas_ptr->ASDIV_ARFCN[tmp] = gl1_asdiv_acq_buff[gas_id].best_arfcn;
        /*AGC Init*/
        sys_algo_agc_init(&gl1_asdiv_acq_pwr_meas_ptr->asdiv_agc[tmp], SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16);
        sys_algo_agc_disable_slew_rate_limiting(&gl1_asdiv_acq_pwr_meas_ptr->asdiv_agc[tmp]);
        gl1_asdiv_acq_pwr_meas_ptr->asdiv_agc_ptr[tmp] = &(gl1_asdiv_acq_pwr_meas_ptr->asdiv_agc[tmp]);
        /*Init power to Floor*/
        gl1_asdiv_acq_pwr_meas_ptr->result_buffer[tmp] = RX_POWER_FLOOR;
    }

    MSG_GERAN_HIGH_4_G(GL1_ASDIV_HDR"gl1_asdiv_acq_do_pwr_meas: Event: %d num_mon:%d mon_per_frame:%d num_frames_to_meas:%d",
                        event,
                        num_mon,
                        mon_per_frame,
                        num_frames_to_meas);

    /*Configure Pwr Meas Handlers, in the Power Monitor SM*/
    gl1_msg_do_pwr_meas( gl1_asdiv_acq_pwr_meas_ptr->ASDIV_ARFCN,
                         gl1_asdiv_acq_pwr_meas_ptr->asdiv_agc_ptr,
                         num_mon,
                         mon_per_frame,
                         num_frames_to_meas,
                         &gl1_asdiv_acq_pwr_meas_ptr->asdiv_acq_pwr_probe_cb_param,
                         gas_id);
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_state_tick

DESCRIPTION
  Tick AsDiv Acq. State Machine State Machine  
 
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_state_tick(gas_id_t gas_id)
{

    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    gl1_msg_frame_tick_type      ftn;
    gl1_msg_init_frame_tick_info(&ftn);

    if (gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event != GL1_ASDIV_ACQ_EV_NONE) 
    {
      MSG_GERAN_HIGH_2_G(GL1_ASDIV_HDR"ISR Tick: Event: %d Main Command:%d", gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event, l1_tskisr_blk->main_command); 
    }

    switch (gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event) 
    {
        /*AsDiv after PScan is just Starting*/
        case GL1_ASDIV_ACQ_EV_PSCAN_END:
        {
            if (gl1_asdiv_acq_buff[gas_id].num_freq_valid > 0)
            {
                gl1_msg_reset(gas_id);
                /*Set the Next Event to Probe on the Best ARFCN detected*/
                gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event = GL1_ASDIV_ACQ_EV_PROBE;
                /*Config Power Mon and Probe Mon on the best Arfcn Found*/
                MSG_GERAN_HIGH_0_G(GL1_ASDIV_HDR"Configuring and Starting Probe Monitors Now");

                if(gl1_msg_cfg_pwr_meas( gl1_asdiv_acq_probe_callback, gl1_asdiv_acq_probe_abort_callback, gas_id))
                {
                    gl1_asdiv_acq_do_pwr_meas(GL1_ASDIV_ACQ_EV_PROBE, gas_id);
                    ftn.next_frame_is_pwr_meas_block_start = TRUE;
                    gl1_msg_frame_tick_notification(&ftn,gas_id);
                }
                else
                {
                    MSG_GERAN_ERROR_0_G(GL1_ASDIV_HDR"CFG power Meas Failed: Exit AsDiv Acq. SubState"); 
                    gl1_asdiv_acq_substate_exit(gas_id);
                }/*Else of cfg pwr meas*/
            }/* Else Move to rescan after switching*/
            else
            {
               if(TRUE!=L1_setup_PSCAN_RESCAN(gas_id))
               {
                MSG_GERAN_HIGH_0_G(GL1_ASDIV_HDR"Power Meas Res. Empty, Switch And ReScan");
                gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event = GL1_ASDIV_ACQ_EV_RESCAN;
                /*Set Up to Rescan on Same Frequency Set*/
                l1_pscan_reset_freq_completed( gas_id);
               }
				else
				{
				   gl1_asdiv_acq_reset(gas_id);
				  if ( l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND)
				 {
				  /* ... transition to Null Mode ... */
				  l1_tskisr_blk->main_command = L1_ENTER_NULL_COMMAND;
				  }
				}
            }/*End Else*/
        }/*case GL1_ASDIV_ACQ_NONE*/
        break;
        
        case GL1_ASDIV_ACQ_EV_PROBE:
        {
            /* Wait for Callbacks from Pwr meas. */
            /* Issue frame tick notification to message layer */
            ftn.next_frame_is_pwr_meas_block_start = TRUE;
            gl1_msg_frame_tick_notification(&ftn,gas_id);
        }/*case GL1_ASDIV_ACQ_EV_PROBE*/
        break;
        
        case GL1_ASDIV_ACQ_EV_BCCH:
        {
          //uint32 num_mon    = gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param.num_pwr_meas;
          /*Wait for BCCH decode to Finish*/
          //MSG_GERAN_HIGH_2_G(GL1_ASDIV_HDR"Probe Mon Num:%d Pwr:%d", num_mon, gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param.pwr_dBm_x16[num_mon]);
        }
        case GL1_ASDIV_ACQ_EV_RESCAN:
        case GL1_ASDIV_ACQ_EV_NONE:
        {
            /*Do Nothing*/
        }/*case GL1_ASDIV_ACQ_EV BCCH, RESCAN, NONE*/
        break;

        case GL1_ASDIV_ACQ_EV_BCCH_END:
        {
            BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
            /*retrieve avg. bcch power measured for current index*/
            dBx16_T avg_bcch_pwr  = l1_BCCH_List_ptr->rx_power_instant[l1_BCCH_List_ptr->current_bcch_index];
            dBx16_T avg_probe_pwr = 0;
            uint32 num_mon        = gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param.num_pwr_meas;
            uint32 tmp            = 0;
            int8 cnt              = 0;

            /*Loop to add total power, and then average it with count*/
            for (tmp=0; tmp < num_mon; tmp ++)
            {
                MSG_GERAN_MED_2_G(GL1_ASDIV_HDR"Bcch Probe idx:%d Pwr:%d", tmp, gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param.pwr_dBm_x16[tmp]);
                if (gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param.pwr_dBm_x16[tmp] > RX_POWER_FLOOR) 
                {
                  avg_probe_pwr += gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param.pwr_dBm_x16[tmp]; 
                  cnt++;
                }
            }/*for loop*/
            
            if (cnt > 0) 
            {
                avg_probe_pwr = avg_probe_pwr / cnt; 
            }
           
            MSG_GERAN_HIGH_3_G(GL1_ASDIV_HDR"avg BCCH pwr:%d avg Probe pwr:%d num-mon:%d", avg_bcch_pwr, avg_probe_pwr, num_mon);

            if (avg_probe_pwr > (avg_bcch_pwr + gl1_asdiv_acq_diff_thresh(gas_id)))
            {
                MSG_GERAN_HIGH_0_G(GL1_ASDIV_HDR"Trigger Switch");
                /*Switch*/
                gl1_trm_ant_switch_div_set_config(gas_id);
            }

            /*terminate the power meas.*/
            MSG_GERAN_MED_0_G(GL1_ASDIV_HDR"terminate Pwr Meas Now!");
            gl1_msgi_pwr_meas_terminate(gas_id);
            /*Reset as a BCCH Decode has ended*/
            gl1_asdiv_acq_reset(gas_id);
        }/*case GL1_ASDIV_ACQ_EV_BCCH_END*/
        break;

        default:
          MSG_GERAN_ERROR_1_G(GL1_ASDIV_HDR"Error: Unknown AsDiv Acq Event:%d",gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event);
        
    }/*switch .. case*/
}/*End Function*/

/**===========================================================================

FUNCTION  gl1_asdiv_acq_reset

DESCRIPTION
  Reset/Re-Init vars in gl1_asdiv_acq_buff
 
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_reset(gas_id_t gas_id)
{
  gl1_asdiv_acq_buff[gas_id].gl1_acq_asdiv_event = GL1_ASDIV_ACQ_EV_NONE;
  gl1_asdiv_acq_buff[gas_id].num_freq_valid      = 0;
  gl1_asdiv_acq_buff[gas_id].freq_list_length    = 0;
  gl1_asdiv_acq_buff[gas_id].best_arfcn.num      = 0;
  memset( &gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param, 0, sizeof( gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param ) );
  //gl1_asdiv_acq_buff[gas_id].best_arfcn.band     = SYS_BAND_NONE;
  MSG_GERAN_HIGH_0_G(GL1_ASDIV_HDR"Reset AsDiv Acq params");
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_bcch_mon_cb

DESCRIPTION
  Power Meas Callback for Probe Monitors along with BCCH Bursts
  Prints F3s for Debug purposes
 
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_bcch_mon_cb(gl1_msg_pwr_meas_result_type *result_data, gas_id_t gas_id)
{
    uint16 tmp, num_meas;
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    num_meas = result_data->num_pwr_meas;    

    MSG_GERAN_MED_1_G(GL1_ASDIV_HDR"gl1_asdiv_acq_bcch_mon_cb: Num meas.:%d", num_meas);
    #if 0
    for (tmp = 0; tmp < num_meas; tmp++) 
    {
        MSG_GERAN_MED_1_G(GL1_ASDIV_HDR"gl1_asdiv_acq_bcch_mon_cb: power:%d", result_data->pwr_dBm_x16[tmp]);
    }
    #endif
}

/**===========================================================================

FUNCTION  gl1_asdiv_acq_bcch_mon_abort_cb

DESCRIPTION
  Abort Callback for Probe Monitors along with BCCH Bursts
  Prints an F3 for Debug Purposes
 
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_asdiv_acq_bcch_mon_abort_cb(gas_id_t gas_id)
{
    MSG_GERAN_HIGH_1_G(GL1_ASDIV_HDR"gl1_asdiv_acq_bcch_mon_abort_cb: Num meas.:%d", gl1_asdiv_acq_pwr_meas_buff[gas_id].asdiv_acq_pwr_probe_cb_param.num_pwr_meas);
}

/**===========================================================================
FUNCTION gl1_asdiv_reset_antenna_to_use 
  
DESCRIPTION 
  Function to Reset antenna_to_use[] Global Var.
  As this Var is directly checked in gl1_hw_pwr_meas_cmd its essential to Reset
  
DEPENDENCIES
  None
  
RETURN VALUE 
  None
  
SIDE EFFECTS 
  None 
  
===========================================================================*/
void gl1_asdiv_reset_antenna_to_use(void)
{
  int i;
  for (i = 0; i<L1_SC_PWR_MAX_MEAS_PER_CALL; i++)
  {
    antenna_to_use[i] = 0;
  }
}

#endif /*FEATURE_GL1_ASDIV_ACQ*/
