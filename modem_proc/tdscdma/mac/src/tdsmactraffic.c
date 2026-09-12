/*================================================================================================

              MAC TRAFFIC VOLUME MEASUREMENTS

GENERAL DESCRIPTION

This module contains functions to

- Process config requests from RRC (setup, modify, delete, delete_all).
- Handle the 10ms timer for traffic volume reporting.
- Handle periodic and event triggered mode.
- Compute  raw rlc payload, average rlc payload, variance rlc payload.
- Compute transport channel traffic volume.
- Block DTCH transmissions on RACH

Copyright (c) 2010-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


================================================================================================*/

/*================================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmactraffic.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who   what, where, why
--------  ---   ----------------------------------------------------------------------------------
06/02/14  sarao   Removed the inclusion of tmc.h as its being discontinued in core.
03/03/11   cyx  add function for HUPA TVM
10/24/10   guo  Initial release Upmerged WCDMA to TDS
================================================================================================*/
/*================================================================================================
                     INCLUDE FILES
================================================================================================*/
#include "tdscdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "task.h"
#include  "tdsmacrrcif.h"
#include  "tdsmacinternal.h"
#include  "tdsl2ultask.h"
#include  "tdsmactraffic.h"
#include  "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include  "tdsmacul.h"
#include  <string.h>
#include "tdsmacrlcif.h"

#include "tdsmathutil.h"

#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
  #include "tdsmacullog.h"
#endif

#include "tdsmaculsubtask.h"
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#ifdef FEATURE_TDSCDMA_HSUPA
#include "tdsmaculhs.h"
#endif

#ifdef FEATURE_TDSCDMA_E4A_OPT
#include "tdsl1macif.h"
#endif
/*================================================================================================
                         D A T A    D E C L A R A T I O N S
================================================================================================*/


/*The number of traffic volume measurements so far */
static uint16  tdsnum_event_measurements=0;
static uint16  tdsnum_periodic_measurements = 0;


static boolean tdsmac_tvm_event_timer_started = FALSE;
static boolean tdsmac_tvm_periodic_timer_started = FALSE;
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
static boolean tdsmac_tvm_block_timer_started = FALSE;
#endif



/*This is the index at which the raw_rlc_payload information for the rb's */
/* in the current timeout would be placed */
static uint16  tdscur_index=0;

/* Traffic volume measurement Config table */
tdsmaci_ul_traffic_cfg_index_type   tdsul_traffic_info_table[TDSUE_NUM_TRAFFIC_MEAS];

/* Traffic volume timer*/
rex_timer_type                   tdsmac_ul_tvm_event_tmr_blk;
rex_timer_type                   tdsmac_ul_tvm_periodic_tmr_blk;
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
rex_timer_type                   tdsmac_ul_tvm_block_tmr_blk;
tdsmaci_ul_event_tvm_blk_index_type   tdsmac_ul_event_tvm_block_table[TDSUE_NUM_TRAFFIC_MEAS];
#endif

/* UL Buffer status obtained from RLC */
tdsrlc_ul_buf_status_type           tdstraffic_ul_buf_status;

/* This array contains the raw payload information for all the radio bearers*/
uint32                           tdsrb_raw_payload[TDS_MAX_RB][TDSTRAFFIC_MAX_SAMPLES];

/* TRUE indicates that the Tx interruption timer is active currently. FALSE*/
/* indicates that the DTCH transmissions on RACH are not blocked now.*/
boolean                          tdsblock_transmission=FALSE;

/* Measurement id which caused the blocking of DTCH transmissions on RACH*/
uint16                           tdstx_interrupt_tmr;

/* Counter to keep track of the Tx interruption timer */
uint16                           tdstx_interrupt_cnt;

tdsmac_ul_traffic_trch_info_type    tdsevt_trch_info[TDSUE_MAX_TRCH];

/* This will indicate if the circular buffer has rolled over */
boolean                          tdsrolled_over = FALSE;

/* This is the first measurement */
//boolean                          evt_first_meas;

/* This structure would contain the results of theresults for a particular
measurement identity at a time */
tdsmac_ul_traffic_meas_ind_type    tdsmeas_ind;

boolean tdsprepare_addtl_meas_incl = FALSE;


/*---------------------------------------------------------------------------
  Contains the list of logical channels for which MAC needs the buffer status
---------------------------------------------------------------------------*/
extern tdsrlc_ul_lc_info_type              tdsmac_ul_lc_info;


#if defined (FEATURE_TDSCDMA_HSUPA)
extern tdsmac_ulhs_state_e_type            tdsmac_ulhs_state;
extern tdsmac_ulhs_lcid_macd_mapping_type  tdsmac_ulhs_rlc_id_mac_dflow_lc_id_mapping[TDSUE_MAX_UL_LOGICAL_CHANNEL];
extern tdsmac_ulhs_config_info_type        tdsmaculhs_config_info;
extern tdsrlc_ul_lc_info_type              tdsmac_hs_ul_lc_info;
tdsrlc_ul_lc_info_type	                   tdstraffic_ul_lc_info;
#endif


// This indicates the minimum transport block size for
// each transport channel. THis will be used for computing
// the exact buffer status for UM for traffic volume
// measurements. In case of UM, the PDU sizes can vary. We
// will pick up the minimum size corresponding to the
// transport channel to which the LC is mapped to and use the
// minimum TB size for computation of headers.
// For AM PDU size is fixed and is configured by RRC to RLC
// Hence this field will NOT be used for AM. TVM is not done
// for TM channels.
extern uint32  tdstrch_min_tb_size[TDSUE_MAX_TRCH];

/* Uplink RLC buffer status buffer */
//extern rlc_ul_buf_status_type    tdsul_buf_status;

/*+yongh add optimization feature NV for L2+*/
uint8 tdsmac_e4b_event_cnt = 0;
uint8 tdsmac_nv_e4b_event_cnt = 0;
/*-yongh add optimization feature NV for L2-*/

/*===============================================================================

FUNCTION MAC_UL_TRAFFIC_VOLUME_INIT

DESCRIPTION Initialise the measurement id config table, intermediate measurements
table, buffer status and the raw payload information.
====================================================================================*/

void tdsmac_ul_traffic_volume_init (void)
{
  uint16 i;

  for (i=0;i<TDSUE_NUM_TRAFFIC_MEAS; i++)
  {
    /*-------------------------------------------------------------------------------
    Initialize the uplink traffic volume table. this table stores the config
    information and the intermediate measurement results. "valid" flag is set
    to FALSE to indicate that there are no valid information available now.
    -------------------------------------------------------------------------------*/
    tdsul_traffic_info_table[i].valid=FALSE;
    tdsul_traffic_info_table[i].traffic_cfg_ptr = NULL;
  }

  /*---------------------------------------------------------------------------------
  Initalize the traffic_ul_buf_status structure. Every 10ms the RLC buffer status is
  obtained in traffic_ul_buf_status.
  ---------------------------------------------------------------------------------*/
  memset(&tdstraffic_ul_buf_status,0, sizeof(tdsrlc_ul_buf_status_type));

  tdstx_interrupt_tmr = 0;
  tdstx_interrupt_cnt = 0;
  tdsnum_event_measurements = 0;
  tdsnum_periodic_measurements = 0;
  //evt_first_meas = TRUE;
  tdsmac_tvm_event_timer_started = FALSE;
  tdsmac_tvm_periodic_timer_started = FALSE;
  tdsblock_transmission = FALSE;

  /*---------------------------------------------------------------------------------
  Initalize the radio bearer raw payload information.
  ---------------------------------------------------------------------------------*/
  for (i=0;i<TDS_MAX_RB;i++)
  {
    /*------------------------------------------------------------------------------
    tdsrb_raw_payload is an array of 26 elements(maximum measurement interval is 260ms)
    and is maintained for every radio bearer (1..32).
    ------------------------------------------------------------------------------*/
    memset(tdsrb_raw_payload[i],0,TDSTRAFFIC_MAX_SAMPLES*sizeof(uint32));
  }

  // Define the traffic volume timer.
  rex_def_timer(&tdsmac_ul_tvm_event_tmr_blk,tds_l2_ul_get_tcb(), TDSMAC_UL_TVM_EVENT_TMR_EXP_SIG);
  rex_def_timer(&tdsmac_ul_tvm_periodic_tmr_blk,tds_l2_ul_get_tcb(), TDSMAC_UL_TVM_PERIODIC_TMR_EXP_SIG);
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
  rex_def_timer(&tdsmac_ul_tvm_block_tmr_blk,tds_l2_ul_get_tcb(), TDSMAC_UL_TVM_BLOCK_TMR_EXP_SIG);
  tdsmac_tvm_block_timer_started = FALSE;
  
  /*---------------------------------------------------------------------------------
  Initalize the TVM block table
  ---------------------------------------------------------------------------------*/
  for (i=0; i<TDSUE_NUM_TRAFFIC_MEAS; i++)
  {
    memset(&tdsmac_ul_event_tvm_block_table[i], 0, sizeof(tdsmaci_ul_event_tvm_blk_index_type));
  }
#endif
}/* mac_ul_traffic_volume_init */


/*===================================================================================
 FUNCTION NAME  mac_reset_periodic_event_timer_counters

 PURPOSE    This function clears the periodic timer, event triggered related timers
 when a state transition occurs.
===================================================================================*/
void tdsmac_reset_periodic_event_trigger_counters (void)
{
  tdsmaci_ul_traffic_cfg_index_type     *int_cfg_param_ptr;
  tdsmaci_ul_traffic_cfg_param          *traffic_cfg_ptr;
  uint8                              i, j, k;

  /* Indicate that this is the first measurement for RACH transport channel */
  //evt_first_meas = TRUE;

  /* If number of measurements is 0 then return */
  if ((tdsnum_event_measurements == 0) && (tdsnum_periodic_measurements == 0))
  {
    return;
  }
  // Loop through all possible Measurement Id's
  for (i=1;i<=TDSMAC_UL_MAX_MEAS_ID; i++)
  {
    // Get the information pertaining to this measurement id.
    int_cfg_param_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(i);

    // If this id has valid parameters in it, then
    if (int_cfg_param_ptr->valid)
    {
      // If non null
      if (int_cfg_param_ptr->traffic_cfg_ptr != NULL)
      {
        traffic_cfg_ptr = int_cfg_param_ptr->traffic_cfg_ptr;

        if (traffic_cfg_ptr->num_trch > TDSUE_MAX_TRCH)
        {
          TDSMAC_MSG2(MSG_LEGACY_ERROR,"NumTrch %d > %d",traffic_cfg_ptr->num_trch,TDSUE_MAX_TRCH);
        }
        else
        {
          // If the report mode is event triggerred then populate rach transport
          // id
          if (traffic_cfg_ptr->report_mode == TDSEVENT_TRIGGERED_MODE)
          {
            // Loop through the transport channels specified in the event criteria
            // information.
            if (traffic_cfg_ptr->event_trch_incl)
            {

              for (j=0;j<traffic_cfg_ptr->event_num_trch
                  && traffic_cfg_ptr->event_num_trch < TDSUE_MAX_TRCH;j++)
              {
                if (traffic_cfg_ptr->num_events[j] > TDSMAX_NUM_EVENTS)
                {
                  TDSMAC_MSG2(MSG_LEGACY_HIGH,"Number of events %d configured for TrchId %d",traffic_cfg_ptr->event_num_trch,traffic_cfg_ptr->event_trch_info[j].trch_id);
                }
                else
                {
                  for (k=0; k < traffic_cfg_ptr->num_events[j] && k < TDSMAX_NUM_EVENTS; k++)
                  {
                    traffic_cfg_ptr->trch_event_param[j][k].prohibit_tmr_cnt= 0;
                    traffic_cfg_ptr->trch_event_param[j][k].trigger_tmr_cnt = 0;
                    traffic_cfg_ptr->trch_event_param[j][k].evt_not_first_meas = FALSE;
                  }
                }

              } // end for

            } // end if
          } // End of if of report mode
          else if (traffic_cfg_ptr->report_mode == TDSPERIODIC_MODE)
          {
            traffic_cfg_ptr->periodic_tmr_cnt = 0;
          }
          else
          {
            // Do Nothing
          }
        } // End of else
      }//else
    }// cfg ptr not null
  } // Loop through all possible measurement identities.
}



/*===================================================================================
 FUNCTION NAME  tdsmaci_post_cfg_cnf_msg()

 PURPOSE    This function posts a config confirmation message to RRC
===================================================================================*/
void tdsmaci_post_cfg_cnf_msg
(
/* Acknowledgement number*/
uint16   config_ack_num,

/* Measurement identity for which ack is sent to RRC*/
uint16  tdsmeas_id,

/* Indicates if the configuration was succesful or not*/
boolean status
)
{
  tdsrrc_mac_cmd_type   *rrc_cmd_ptr;

  /* Failed to allocate memory for RRC command */
  if ((rrc_cmd_ptr = tdsrrc_get_mac_cmd_buf()) == NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Could not get RRC cmd buffer");
  }
  /* Send TDSRRC_CMAC_MEAS_CTRL_CNF message to RRC */
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CMAC_MEAS_CTRL_CNF to RRC");

    /*Note : This feature definition would be removed only when RRC also removes
    this FEATURE definition */
    /* Fill in command header */
    rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CMAC_MEAS_CTRL_CNF;

    /* Fill in command data */
    rrc_cmd_ptr->cmd.mac_meas_ctrl_cnf.config_ack_num = config_ack_num;
    rrc_cmd_ptr->cmd.mac_meas_ctrl_cnf.meas_id = tdsmeas_id;
    rrc_cmd_ptr->cmd.mac_meas_ctrl_cnf.status = status;

    /* Send command to RRC */
    tdsrrc_put_mac_cmd(rrc_cmd_ptr);

  }

} /*tdsmaci_post_cfg_cnf_msg */

/*===================================================================================
 FUNCTION NAME  tdsmaci_post_traffic_volume_meas_report

 PURPOSE    This function posts the result of traffic volume measurements for a
            measurement identity.
===================================================================================*/
void tdsmaci_post_traffic_volume_meas_report
(
/* Contains the measurement results for a particular measurement id*/
tdsmac_ul_traffic_meas_ind_type  *meas_ind_ptr
)
{
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
  tr_ch_id_type                      trch_id;
  tdsmac_ul_traffic_event_id_e_type  event_id;
  uint16                             meas_id_index;
  uint16                             ntrch;
  uint16                             i;

  if (tdsmac_tvm_block_timer_started == FALSE)
  {
#endif
    tdsrrc_mac_cmd_type   *rrc_cmd_ptr;
  
    /* Failed to allocate memory for RRC command */
    if ((rrc_cmd_ptr = tdsrrc_get_mac_cmd_buf()) == NULL)
    {
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"Could not get RRC cmd buffer");
    }
    /* Send TDSRRC_CMAC_UL_TRAFFIC_IND message to RRC */
    else
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CMAC_UL_TRAFFIC_IND to RRC");
  
      /* Fill in command header */
      rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CMAC_UL_TRAFFIC_IND;
      /* Fill in command data */
      /*memcpy(&rrc_cmd_ptr->cmd.cmac_ul_traffic_ind,meas_ind_ptr,
             sizeof(tdsmac_ul_traffic_meas_ind_type));*/
      memscpy(&rrc_cmd_ptr->cmd.cmac_ul_traffic_ind,
              sizeof(tdsmac_ul_traffic_meas_ind_type),
              meas_ind_ptr,
             sizeof(tdsmac_ul_traffic_meas_ind_type));
      /* Send command to RRC */
      tdsrrc_put_mac_cmd(rrc_cmd_ptr);
    }
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
  }
  else
  {
    if (meas_ind_ptr->report_mode == TDSEVENT_TRIGGERED_MODE)
    {
      meas_id_index = meas_ind_ptr->meas_id-TDSMAC_UL_MIN_MEAS_ID;
      trch_id = meas_ind_ptr->event_result.trch_info.trch_id;
      event_id = meas_ind_ptr->event_result.event_id;
      ntrch = tdsmac_ul_event_tvm_block_table[meas_id_index].ntrch;

      for (i=0; i<ntrch && i < TDSUE_MAX_TRCH; i++)
      {
        if (tdsmac_ul_event_tvm_block_table[meas_id_index].blk_info[i].tdstrch_id == trch_id)
        {
          tdsmac_ul_event_tvm_block_table[meas_id_index].blk_info[i].tdsevent_id = event_id;
          TDSMAC_MSG3(MSG_LEGACY_HIGH,"Update lock TDSRRC_CMAC_UL_TRAFFIC_IND, meas_id %d, event_id %d, trch_id %d", 
                meas_ind_ptr->meas_id, event_id, trch_id);
          break;
        }
      }
      if (i == ntrch && i < TDSUE_MAX_TRCH)
      {
        tdsmac_ul_event_tvm_block_table[meas_id_index].blk_info[i].tdstrch_id = trch_id;
        tdsmac_ul_event_tvm_block_table[meas_id_index].blk_info[i].tdsevent_id = event_id;
        tdsmac_ul_event_tvm_block_table[meas_id_index].ntrch++;
        TDSMAC_MSG3(MSG_LEGACY_HIGH,"Block TDSRRC_CMAC_UL_TRAFFIC_IND, meas_id %d, event_id %d, trch_id %d", 
                meas_ind_ptr->meas_id, event_id, trch_id);
      }
    }
  }
#endif
} /*tdsmaci_post_cfg_cnf_msg */

/*==================================================================================
 FUNCTION NAME  tdscheck_meas_report_qty

 PURPOSE    This function checks if measurement interval is specified in the
            case when we are asked to report the average or variance.

==================================================================================*/
boolean tdscheck_meas_report_qty
(
/* Configuration parameters recv'd from RRC */
tdsmac_ul_traffic_meas_config_type *oc_traffic_cfg_ptr
)
{
  boolean status_ind=TRUE;

  /* If the measurement report is included */
  if (oc_traffic_cfg_ptr->meas_report_incl)
  {
    /* If MAC has to report either the average or variance*/
    if (oc_traffic_cfg_ptr->meas_report_qty.avg_payload ||
        oc_traffic_cfg_ptr->meas_report_qty.var_payload)
    {
      /* If the measurement quantity information is not included*/
      if (!oc_traffic_cfg_ptr->meas_qty_incl)
      {
        status_ind=FALSE;
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"Measurement interval not specified");
      }
      else
      {
        /* If the measurement interval is 0, then it is invalid*/
        if (oc_traffic_cfg_ptr->meas_qty.meas_interval/10 ==0)
        {
          status_ind=FALSE;
          TDSMAC_MSG0(MSG_LEGACY_ERROR,"Measurement Interval not valid");
        }
      }
    }
    /*------------------------------------------------------------------------
      If every value in measurement report quantity is set to FALSE
      then it is an error since we don't have any quantity to report
    ------------------------------------------------------------------------*/
    else if (!oc_traffic_cfg_ptr->meas_report_qty.rlc_payload)
    {
      /*--------------------------------------------------------------------
      In the case of event triggered mode or periodic mode, UE can be asked to
      report only the event results (as 4A, 4B ) and NW might not ask UE to report
      raw, average, or variance. Its allowed as per spec.
      ---------------------------------------------------------------------*/
      status_ind=TRUE;

      TDSMAC_MSG0(MSG_LEGACY_HIGH,"Report quantity not specified");
    }
  }

  return status_ind;
}

/*==================================================================================
 FUNCTION NAME  tdspopulate_object_list

 PURPOSE    This function finds all the uplink currently active transport channels
            and populates the transport channel information array. It returns the number
            of transport channels.
==================================================================================*/
uint16 tdspopulate_object_list
(
/* Array of transport channel type and id information*/
tdsmac_ul_traffic_trch_info_type     tdstrch_info[]

)
{
  uint16                  i,j;     /* loop index */
  tr_ch_id_type           tdstrch_id; /*transport channel id */
  tdstrch_id_to_idx_map_type *id_to_idx_ptr; /*Transport channel id to index pointer*/

  /* Go through all the possible transport channels id */
  for (i=0,j=0;i<MAX_TRCH;i++)
  {
    /* The transport channel id ranges from 1 to 32 */
    tdstrch_id = i+TDSMAC_UL_MIN_TRCH_ID;

    /* Get the index where transport channel information for this id is stored*/
    id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);

    /* if the corresponding index has valid parameters */
    if (id_to_idx_ptr->valid)
    {
      /* Store the transport channel id and the type in the appropriate place */
      if(j < TDSUE_MAX_TRCH)
      {
        tdstrch_info[j].trch_id = tdstrch_id;
        tdstrch_info[j].trch_type = TDSGET_UL_CCTRCH_TYPE;

        /* Increment the count of valid transport channels */
        j++;
      }
      else
      {
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"Num of TVM object exceed TDSUE_MAX_TRCH ");
      }

    } /*if transport channel is valid*/

  }/*for the number of transport channels */

#if defined (FEATURE_TDSCDMA_HSUPA)
  if (j < TDSUE_MAX_TRCH)
  {
    if(TDSMAC_ULHS_ACTIVE == tdsmac_ulhs_state)
    {
     
      tdstrch_info[j].trch_id = TDSEDCH_ID;
      tdstrch_info[j].trch_type = TDSEDCH;
      j++;
    }
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Num of TVM object exceed TDSUE_MAX_TRCH ");
  }
#endif
  /* j keeps a count of the valid transport channels */
  return j;
}

/*==================================================================================
 FUNCTION NAME  tdspopulate_rach_trch_id

 PURPOSE: UTRAN can configure traffic volume measurements by specifying both the
 DCH and RACH objects in the measurement object list. In the case when the current ul
 cctrch type is DCH and they have configured measurements for RACH, the trch_id will
 not be available for RACH. Hence this function is called when UE transitions to
 cell_fach state and a RACH id is required to do the measurements.
==================================================================================*/
boolean tdspopulate_rach_trch_id(void)
{
  tdsmaci_ul_traffic_cfg_index_type     *int_cfg_param_ptr;
  tdsmaci_ul_traffic_cfg_param          *traffic_cfg_ptr;
  uint8                              i, j;
  boolean                            status = TRUE;

  /* Indicate that this is the first measurement for RACH transport channel */
  //evt_first_meas = TRUE;
  tdsblock_transmission = FALSE;

  /* If number of measurements is 0 then return */
  if ((tdsnum_event_measurements == 0) && (tdsnum_periodic_measurements == 0))
  {
    return status;
  }

  // Loop through all possible Measurement Id's
  for (i=1;i<=TDSMAC_UL_MAX_MEAS_ID; i++)
  {
    // Get the information pertaining to this measurement id.
    int_cfg_param_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(i);

    // If this id has valid parameters in it, then
    if (int_cfg_param_ptr->valid)
    {
      // If non null
      if (int_cfg_param_ptr->traffic_cfg_ptr != NULL)
      {
        traffic_cfg_ptr = int_cfg_param_ptr->traffic_cfg_ptr;

        if (traffic_cfg_ptr->num_trch > TDSUE_MAX_TRCH)
        {
          TDSMAC_MSG2(MSG_LEGACY_ERROR,"NumTrch %d > %d",traffic_cfg_ptr->num_trch,TDSUE_MAX_TRCH);
        }
        else
        {
          // Loop through the number of transport channels
          for (j=0; j<traffic_cfg_ptr->num_trch ; j++)
          {
            // populate the transport channel id for rach
            if (traffic_cfg_ptr->trch_info[j].trch_type == TDSRACH)
            {
              traffic_cfg_ptr->trch_info[j].trch_id = tdsint_ul_trch_info[0].trch_id;
              // There can be only one RACH, hence break after finding the first one.
              break;
            }
          }

          // If the report mode is event triggerred then populate rach transport
          // id
          if (traffic_cfg_ptr->report_mode == TDSEVENT_TRIGGERED_MODE)
          {
            // Loop through the transport channels specified in the event criteria
            // information.
            if (traffic_cfg_ptr->event_num_trch > TDSUE_MAX_TRCH)
            {
              TDSMAC_MSG2(MSG_LEGACY_ERROR,"Event NumTrch %d > %d",traffic_cfg_ptr->event_num_trch,TDSUE_MAX_TRCH);

            }
            else
            {
              for (j=0;j<traffic_cfg_ptr->event_num_trch;j++)
              {
                if (traffic_cfg_ptr->event_trch_info[j].trch_type == TDSRACH)
                {
                  traffic_cfg_ptr->event_trch_info[j].trch_id = tdsint_ul_trch_info[0].trch_id;
                  // There can be only one RACH, hence break after finding the first one.
                  break;
                }

              } // end for
            } // else
          } // end if

        }//else

      }// cfg ptr not null
    } // if the measurement id exists.
  }// Loop through all possible measurement identities.

  return status;

}// tdspopulate_rach_trch_id()


/*==================================================================================
 FUNCTION NAME  tdscheck_n_copy_event_trigger_params

 PURPOSE    This function copies the event trigger parameters for the respective
            transport channels after performing the check to see if the transport
            channels specified are valid. It copies the parameters only if they
            are valid. If the transport channel list is not specified then there
            is only one transport channel specific event criteria specified and
            it is applicable to all the  transport channels as indicated by the
            measurement object list.
==================================================================================*/

boolean tdscheck_n_copy_event_trigger_params
(
/* Pointer to Config parameters internally maintained by MAC*/
tdsmaci_ul_traffic_cfg_param             *int_tmp_cfg_ptr,

/* Pointer to Config parameters sent by RRC to MAC */
tdsmac_ul_traffic_event_trigger_cfg_type *oc_event_cfg_ptr

)
{
  uint16                                    i,j;
  uint16                                    ntrch=1;
  tdsmac_ul_traffic_trch_event_trigger_cfg     *oc_trch_event_param_ptr;
  boolean                                   event_trch_incl;
  tdsmaci_ul_traffic_trch_event_param          *int_trch_param_ptr;
  tdsmac_ul_traffic_trch_event_trigger_param   *oc_trch_cfg_param_ptr;
  boolean                                   status=TRUE;
  tdsul_trch_info_type                         *trch_ptr;

  /*-------------------------------------------------------------------------------
   True indicates, the event parameters are specified for all transport channels
   as indicated. False implies that only one set of event parameters are specified
   and this is applied to all the uplink transport channels currently active.
  --------------------------------------------------------------------------------*/
  event_trch_incl = oc_event_cfg_ptr->trch_info_incl;

  if (event_trch_incl)
  {
    /*-----------------------------------------------------------------------------
    Copy the number of transport channels for which the event criteria are specified.
    -----------------------------------------------------------------------------*/
    ntrch = oc_event_cfg_ptr->num_trch;
  }
  /*-------------------------------------------------------------------------------
  MAC currently  supports only TDSUE_MAX_TRCH number of transport channels.
  -------------------------------------------------------------------------------*/
  if (ntrch>TDSUE_MAX_TRCH)
  {
    TDSMAC_MSG1(MSG_LEGACY_ERROR,"Currently support %d trch's",TDSUE_MAX_TRCH);
    status=FALSE;
  }
  else
  {
    /*-------------------------------------------------------------------------------
    The following piece of code checks if the transport channel for which the event
    criteria are specified are valid. Even if one transport channel id or type is
    invalid it breaks out setting status to FALSE.
    --------------------------------------------------------------------------------*/
    /* Go through the transport channels specified */
    for (i=0;i<ntrch;i++)
    {
      oc_trch_event_param_ptr=&oc_event_cfg_ptr->trch_event_cfg[i];

      /* The number of events should be >= 1 */
      if ((oc_trch_event_param_ptr->num_events==0) ||
          (oc_trch_event_param_ptr->num_events > TDSMAX_NUM_EVENTS))
      {
        /*---------------------------------------------------------------------------
         If the number of events is 0 then set the status flag to FALSE
        ---------------------------------------------------------------------------*/
        status=FALSE;
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"Number of events can't be 0");
        break;
      }

    } /* for number of transport channels */

    /*-------------------------------------------------------------------------------
     We copy the transport channel id & type to local memory only if all the checks
     have been done If the transport channel id's and types are valid
    -------------------------------------------------------------------------------*/
    if (status)
    {
      int_tmp_cfg_ptr->event_trch_incl = event_trch_incl;

      if (event_trch_incl)
      {
        /*---------------------------------------------------------------------------
        Copy the number of transport channels information to the local
        table internally maintained by MAC
       ---------------------------------------------------------------------------*/
        int_tmp_cfg_ptr->event_num_trch = oc_event_cfg_ptr->num_trch;

      }
      else
      {
        /*---------------------------------------------------------------------------
        When the set of transport channels on which an event trigger is not defined
        explicitly, the UE shall apply it on the transport channels specified by the
        measurement object list.
        ---------------------------------------------------------------------------*/

      }

      for (i=0;i<ntrch;i++)
      {
        oc_trch_event_param_ptr=&oc_event_cfg_ptr->trch_event_cfg[i];
        /*---------------------------------------------------------------------------
        Copy the number of events for each of these transport channels
        ---------------------------------------------------------------------------*/
        int_tmp_cfg_ptr->num_events[i] = oc_trch_event_param_ptr->num_events;

        if (event_trch_incl)
        {
          /*------------------------------------------------------------------------
          Copy the transport channel id and the type to the table internally
          maintained by MAC
          -------------------------------------------------------------------------*/
          if (oc_trch_event_param_ptr->trch_info.trch_type==TDSRACH)
          {
            /* Get the transport channel infor pointer */
            trch_ptr = TDSGET_UL_TRCH_INFO_PTR(0);

            /* Store the tdstrch_id along with the config information*/
            int_tmp_cfg_ptr->event_trch_info[i].trch_id = trch_ptr->trch_id;
          }
          else
          {
            int_tmp_cfg_ptr->event_trch_info[i].trch_id=
            oc_trch_event_param_ptr->trch_info.trch_id;
          }

          int_tmp_cfg_ptr->event_trch_info[i].trch_type=
          oc_trch_event_param_ptr->trch_info.trch_type;
        }
        else
        {
          /*---------------------------------------------------------------------------
          When the set of transport channels on which an event trigger is not defined
          explicitly, the UE shall apply it on the transport channels specified by the
          measurement object list.
          ---------------------------------------------------------------------------*/
        }

        int_trch_param_ptr = int_tmp_cfg_ptr->trch_event_param[i];
        oc_trch_cfg_param_ptr = oc_trch_event_param_ptr->event_param;

        /* Copy the event specific parameters */
        for (j=0;j<int_tmp_cfg_ptr->num_events[i];j++)
        {
          /* copy the number of events information */
          int_trch_param_ptr[j].event_id = oc_trch_cfg_param_ptr[j].event_id;

          /* Event is 4A */
          if (int_trch_param_ptr[j].event_id==TDSEVENT_4A)
          {
            /*---------------------------------------------------------------------
             If the tdsevent_id is event_4A then initialize the previous buffer
             occupancy to the highest value possible
            ----------------------------------------------------------------------*/
            int_trch_param_ptr[j].prev_bo = 0xFFFF;
          }
          /* if the event is 4B */
          else if (int_trch_param_ptr[j].event_id==TDSEVENT_4B)
          {
            /*---------------------------------------------------------------------
             if the event is 4B then initialze the previous buffer occupancy
             to the lowest value ie 0
            ----------------------------------------------------------------------*/
            int_trch_param_ptr[j].prev_bo = 0;
          }
          else
          {
            TDSMAC_MSG1(MSG_LEGACY_ERROR,"Event ID not specified for trch %d",i);
          }
          /*----------------------------------------------------------------------
          Copy all the timer values froom the config information rec'd from RRC
          ----------------------------------------------------------------------*/
          int_trch_param_ptr[j].trigger_prohibit_tmr =
          oc_trch_cfg_param_ptr[j].trigger_prohibit_tmr;
          int_trch_param_ptr[j].tx_interrupt_tmr =
          oc_trch_cfg_param_ptr[j].tx_interrupt_tmr;
          int_trch_param_ptr[j].time_to_trigger =
          oc_trch_cfg_param_ptr[j].time_to_trigger;

          /*----------------------------------------------------------------------
          Copy the report threshold.
          ----------------------------------------------------------------------*/
          int_trch_param_ptr[j].report_threshold =
          oc_trch_cfg_param_ptr[j].report_threshold;

          /*----------------------------------------------------------------------
            Initialize all the counters. All the timers for traffic volume are
            implemented in the form of counters. There is one base timer which
            expires every 10ms.
          ----------------------------------------------------------------------*/
          int_trch_param_ptr[j].trigger_tmr_cnt=0;
          int_trch_param_ptr[j].prohibit_tmr_cnt=0;

        } /* for number of events*/

      } /*for number of transport channels*/

    }/* if status */

  } /* if ntrch <= TDSUE_MAX_TRCH */

  return status;

}

/*==================================================================================
 FUNCTION NAME  tdscheck_n_copy_modify_event_trigger_params

 PURPOSE    This function copies the event trigger parameters based on the
            modified parameters received from UTRAN for the respective
            transport channels after performing the check to see if the transport
            channels specified are valid. It copies the parameters only if they
            are valid. If the transport channel list is not specified then there
            is only one transport channel specific event criteria specified and
            it is applicable to all the  transport channels as indicated by the
            measurement object list.
==================================================================================*/

boolean tdscheck_n_copy_modify_event_trigger_params
(
/* Pointer to Config parameters internally maintained by MAC*/
tdsmaci_ul_traffic_cfg_param             *int_tmp_cfg_ptr,

/* Pointer to Config parameters sent by RRC to MAC */
tdsmac_ul_traffic_event_trigger_cfg_type *oc_event_cfg_ptr
)
{
  uint16                                    i,j,k,l;
  uint16                                    ntrch=1;

  tdsmac_ul_traffic_trch_event_trigger_cfg     *oc_trch_event_param_ptr;
  boolean                                   event_trch_incl, found, found_trch;
  //maci_ul_traffic_trch_event_param          *int_trch_param_ptr;
  //mac_ul_traffic_trch_event_trigger_param   *oc_trch_cfg_param_ptr;
  boolean                                   status=TRUE;
  tdsul_trch_info_type                         *trch_ptr;

  /*-------------------------------------------------------------------------------
   True indicates, the event parameters are specified for all transport channels
   as indicated. False implies that only one set of event parameters are specified
   and this is applied to all the uplink transport channels currently active.
  --------------------------------------------------------------------------------*/
  event_trch_incl = oc_event_cfg_ptr->trch_info_incl;

  if (event_trch_incl)
  {
    /*-----------------------------------------------------------------------------
    Copy the number of transport channels for which the event criteria are specified.
    -----------------------------------------------------------------------------*/
    ntrch = oc_event_cfg_ptr->num_trch;
  }
  else
  {
    status = FALSE;
  }

  /*-------------------------------------------------------------------------------
  MAC currently  supports only TDSUE_MAX_TRCH number of transport channels.
  -------------------------------------------------------------------------------*/
  if ((ntrch>TDSUE_MAX_TRCH)||(int_tmp_cfg_ptr->event_num_trch > TDSUE_MAX_TRCH))
  {
    TDSMAC_MSG1(MSG_LEGACY_ERROR,"Currently support %d trch's",TDSUE_MAX_TRCH);
    status=FALSE;
  }
  else
  {
    /*-------------------------------------------------------------------------------
    The following piece of code checks if the transport channel for which the event
    criteria are specified are valid. Even if one transport channel id or type is
    invalid it breaks out setting status to FALSE.
    --------------------------------------------------------------------------------*/
    /* Go through the transport channels specified */
    for (i=0;i<ntrch;i++)
    {
      oc_trch_event_param_ptr=&oc_event_cfg_ptr->trch_event_cfg[i];

      /* The number of events should be >= 1 */
      if ((oc_trch_event_param_ptr->num_events==0) ||
          (oc_trch_event_param_ptr->num_events > TDSMAX_NUM_EVENTS))
      {
        /*---------------------------------------------------------------------------
         If the number of events is 0 then set the status flag to FALSE
        ---------------------------------------------------------------------------*/
        status=FALSE;
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"Number of events can't be 0");
        break;
      }
    } /* for number of transport channels */

    /*-------------------------------------------------------------------------------
    We copy the transport channel id & type to local memory only if all the checks
    have been done If the transport channel id's and types are valid
    -------------------------------------------------------------------------------*/
    if (status)
    {
      // Go through the transport channels mentioned by RRC in modify command
      for (i=0;i<ntrch;i++)
      {
        // Get the pointer to the transport channel config information
        oc_trch_event_param_ptr=&oc_event_cfg_ptr->trch_event_cfg[i];

        // Initialise the boolean to FALSE. This indicates that we have not yet found
        // a transport channel that matches the one given in setup command.
        found_trch = FALSE;

        // Go through the transport channels that are already present in the MAC
        // database.
        for (k=0;k<int_tmp_cfg_ptr->event_num_trch;k++)
        {
          // Check if the transport channel id and transport channel type matches.
          if (((oc_trch_event_param_ptr->trch_info.trch_type == TDSRACH ) && 
               (int_tmp_cfg_ptr->event_trch_info[k].trch_type == oc_trch_event_param_ptr->trch_info.trch_type)) ||
              ((int_tmp_cfg_ptr->event_trch_info[k].trch_id == oc_trch_event_param_ptr->trch_info.trch_id) &&
               (int_tmp_cfg_ptr->event_trch_info[k].trch_type == oc_trch_event_param_ptr->trch_info.trch_type)))
          {

            TDSMAC_MSG2(MSG_LEGACY_HIGH,"TVM:Replacing with modify MCM events TR_CH Id = %d,cctrch_type = %d ",int_tmp_cfg_ptr->event_trch_info[k].trch_id,int_tmp_cfg_ptr->event_trch_info[k].trch_type);

            // Go through the events that are being modified fiven to MAC by RRC
            for (j=0;j<oc_trch_event_param_ptr->num_events;j++)
            {
              // Initialise the boolean found to FALSE to indicate that we have  not
              // yet found a event matching the one already in the database for thie
              // transport channel id.
              found = FALSE;

              // Go through the events that are already in the MAC database for
              // this transport channel
              for (l=0;l<int_tmp_cfg_ptr->num_events[k];l++)
              {
                // If the event id matches.
                if (int_tmp_cfg_ptr->trch_event_param[k][l].event_id ==
                    oc_trch_event_param_ptr->event_param[j].event_id )
                {
                  // Then some event specific parameters for this event are being
                  // modified.
                  TDSMAC_MSG3(MSG_LEGACY_HIGH,"TVM:Modify num_events %d,evt %d, tdstrch_id %d",
                  oc_trch_event_param_ptr->num_events,l,int_tmp_cfg_ptr->event_trch_info[k].trch_id);

                  /*----------------------------------------------------------------------
                  Copy all the timer values froom the config information rec'd from RRC
                  ----------------------------------------------------------------------*/
                  if (oc_trch_event_param_ptr->event_param[j].trigger_prohibit_tmr)
                  {
                    int_tmp_cfg_ptr->trch_event_param[k][l].trigger_prohibit_tmr =
                    oc_trch_event_param_ptr->event_param[j].trigger_prohibit_tmr;
                  }

                  if (oc_trch_event_param_ptr->event_param[j].tx_interrupt_tmr)
                  {
                    int_tmp_cfg_ptr->trch_event_param[k][l].tx_interrupt_tmr =
                    oc_trch_event_param_ptr->event_param[j].tx_interrupt_tmr;
                  }

                  if (oc_trch_event_param_ptr->event_param[j].time_to_trigger)
                  {
                    int_tmp_cfg_ptr->trch_event_param[k][l].time_to_trigger =
                    oc_trch_event_param_ptr->event_param[j].time_to_trigger;
                  }

                  /*----------------------------------------------------------------------
                  Copy the report threshold.
                  ----------------------------------------------------------------------*/
                  if (oc_trch_event_param_ptr->event_param[j].report_threshold)
                  {
                    int_tmp_cfg_ptr->trch_event_param[k][l].report_threshold =
                    oc_trch_event_param_ptr->event_param[j].report_threshold;
                  }

                  /*----------------------------------------------------------------------
                    Initialize all the counters. All the timers for traffic volume are
                    implemented in the form of counters. There is one base timer which
                    expires every 10ms.
                  ----------------------------------------------------------------------*/
                  int_tmp_cfg_ptr->trch_event_param[k][l].trigger_tmr_cnt=0;
                  int_tmp_cfg_ptr->trch_event_param[k][l].prohibit_tmr_cnt=0;
                  int_tmp_cfg_ptr->trch_event_param[k][l].evt_not_first_meas=FALSE;

                  found = TRUE;
                  break;
                }
              } /* for number of events actually modified */

              // Add this new event that was configured.
              if (found == FALSE)
              {
                 int_tmp_cfg_ptr->trch_event_param[k][l].event_id =
                  oc_trch_event_param_ptr->event_param[j].event_id;
                 int_tmp_cfg_ptr->trch_event_param[k][l].report_threshold =
                    oc_trch_event_param_ptr->event_param[j].report_threshold;
                 int_tmp_cfg_ptr->trch_event_param[k][l].time_to_trigger =
                    oc_trch_event_param_ptr->event_param[j].time_to_trigger;
                 int_tmp_cfg_ptr->trch_event_param[k][l].trigger_prohibit_tmr =
                    oc_trch_event_param_ptr->event_param[j].trigger_prohibit_tmr;
                 int_tmp_cfg_ptr->trch_event_param[k][l].trigger_tmr_cnt=0;
                 int_tmp_cfg_ptr->trch_event_param[k][l].prohibit_tmr_cnt=0;
                 int_tmp_cfg_ptr->trch_event_param[k][l].evt_not_first_meas=FALSE;
                 int_tmp_cfg_ptr->num_events[k]++;
              }

            } /* Iterate through J */

            found_trch = TRUE;
            break;
          }
        }

        if(((int_tmp_cfg_ptr->event_num_trch = TDSUE_MAX_TRCH)||(k = TDSUE_MAX_TRCH)) &&
          found_trch == FALSE)
        {
          status=FALSE;
          TDSMAC_MSG3(MSG_LEGACY_HIGH,"TVM:Cannot find suitable Trch and cannot establish a new one"
                                        "local event_num_trch = %d,cctrch_type = %d, k= %d ",
                                        int_tmp_cfg_ptr->event_num_trch,oc_trch_event_param_ptr->trch_info.trch_type,k);
          break;
        }

        if (found_trch == FALSE)
        {
          int_tmp_cfg_ptr->event_num_trch++;

          /*------------------------------------------------------------------------
          Copy the transport channel id and the type to the table internally
          maintained by MAC
          -------------------------------------------------------------------------*/
          if (oc_trch_event_param_ptr->trch_info.trch_type==TDSRACH)
          {
            /* Get the transport channel infor pointer */
            trch_ptr = TDSGET_UL_TRCH_INFO_PTR(0);

            /* Store the tdstrch_id along with the config information*/
            int_tmp_cfg_ptr->event_trch_info[k].trch_id = trch_ptr->trch_id;
          }
          else
          {
            int_tmp_cfg_ptr->event_trch_info[k].trch_id =
              oc_trch_event_param_ptr->trch_info.trch_id;
          }

          int_tmp_cfg_ptr->event_trch_info[k].trch_type =
               oc_trch_event_param_ptr->trch_info.trch_type;
          int_tmp_cfg_ptr->num_events[k] = oc_trch_event_param_ptr->num_events;

          //int_trch_param_ptr = int_tmp_cfg_ptr->trch_event_param[i];
          //oc_trch_cfg_param_ptr = oc_trch_event_param_ptr->event_param;

          /* Copy the event specific parameters */
          for (j=0;j<oc_trch_event_param_ptr->num_events;j++)
          {
            TDSMAC_MSG1(MSG_LEGACY_HIGH,"TVM:Appending Modify num_events %d",oc_trch_event_param_ptr->num_events);

            int_tmp_cfg_ptr->trch_event_param[k][j].event_id =
            oc_trch_event_param_ptr->event_param[j].event_id;
            /*----------------------------------------------------------------------
            Copy all the timer values froom the config information rec'd from RRC
            ----------------------------------------------------------------------*/
            int_tmp_cfg_ptr->trch_event_param[k][j].trigger_prohibit_tmr =
            oc_trch_event_param_ptr->event_param[j].trigger_prohibit_tmr;

            int_tmp_cfg_ptr->trch_event_param[k][j].tx_interrupt_tmr =
            oc_trch_event_param_ptr->event_param[j].tx_interrupt_tmr;

            int_tmp_cfg_ptr->trch_event_param[k][j].time_to_trigger =
            oc_trch_event_param_ptr->event_param[j].time_to_trigger;

            /*----------------------------------------------------------------------
            Copy the report threshold.
            ----------------------------------------------------------------------*/
            int_tmp_cfg_ptr->trch_event_param[k][j].report_threshold =
            oc_trch_event_param_ptr->event_param[j].report_threshold;

            /*----------------------------------------------------------------------
              Initialize all the counters. All the timers for traffic volume are
              implemented in the form of counters. There is one base timer which
              expires every 10ms.
            ----------------------------------------------------------------------*/
            int_tmp_cfg_ptr->trch_event_param[k][j].trigger_tmr_cnt=0;
            int_tmp_cfg_ptr->trch_event_param[k][j].prohibit_tmr_cnt=0;
            int_tmp_cfg_ptr->trch_event_param[k][j].evt_not_first_meas=FALSE;
          } /* for number of events*/
        }
      } /*for number of transport channels*/
    }/* if status */
  } /* if ntrch <= TDSUE_MAX_TRCH */
  return status;
}

/*==================================================================================
 FUNCTION NAME  tdscheck_n_copy_periodic_params

 PURPOSE  This function copies the periodic parameters only if the transport channels
          specified are valid. If the transport channel list is not specified then
          it populates the object list too.
==================================================================================*/
boolean tdscheck_n_copy_periodic_params
(
/* Pointer to the config information internally stored by MAC */
tdsmaci_ul_traffic_cfg_param                 *int_tmp_cfg_ptr,

/* Pointer to the config information received from RRC */
tdsmac_ul_traffic_meas_config_type           *oc_traffic_cfg_ptr

)
{
  tdsmac_ul_traffic_periodic_report_cfg_type   *oc_periodic_cfg_ptr;
  boolean   status_ind=TRUE;

  oc_periodic_cfg_ptr = &oc_traffic_cfg_ptr->meas_report_criteria.periodic_cfg;

  if (oc_periodic_cfg_ptr->report_interval/10 < 2)
  {
    /* The minimum valid report interval is 20ms */
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Report interval not valid");
    status_ind = FALSE;
  }

  if (status_ind)
  {
    /*------------------------------------------------------------------------------
     meas_obj_list_incl=TRUE indicates that measurement is to be done for the
     transport channels specified in the object list FALSE implies apply
     measurement to all active uplink transport channels
    -------------------------------------------------------------------------------*/
    int_tmp_cfg_ptr->meas_obj_list_incl = oc_traffic_cfg_ptr->meas_obj_incl;

    /*-------------------------------------------------------------------------------
    True indicates that there is no limit on the number of reports that we send RRC
    False indicates, send only the number of reports as specified by utran
    --------------------------------------------------------------------------------*/
    int_tmp_cfg_ptr->report_infinity = oc_periodic_cfg_ptr->report_infinity;

    /*-------------------------------------------------------------------------------
       Copy the periodic config information in the local memory.These include the
        number of times the report has to be sent and the report interval.
    -------------------------------------------------------------------------------*/
    int_tmp_cfg_ptr->num_reports     = oc_periodic_cfg_ptr->num_reports;
    int_tmp_cfg_ptr->report_interval = oc_periodic_cfg_ptr->report_interval;

    /*------------------------------------------------------------------------------
    Initialise the periodic timer count to 0. All timers in MAC are implemented in
    the form of counters.
    -------------------------------------------------------------------------------*/
    int_tmp_cfg_ptr->periodic_tmr_cnt = 0;

    /*------------------------------------------------------------------------------
          Initialise the number of reports sent so far to 0.
    -------------------------------------------------------------------------------*/
    int_tmp_cfg_ptr->nreports_sent = 0;

  }

  return status_ind;

}

/*==================================================================================
 FUNCTION NAME  tdsmac_ul_setup_traffic_volume_meas

 PURPOSE    This function populates the traffic volume measurement table and starts
            traffic volume measurements for the corresponding id.
            This function returns a boolean. True indicates the setup was sucessful.
            It goes through all the parameters specified by RRC and checks to see
            if they are valid. If it is valid then it copies them to the local
            memory.
==================================================================================*/

boolean tdsmac_ul_setup_traffic_volume_meas
(
/* Pointer to traffic volume config information */
tdsmac_ul_traffic_meas_config_type *oc_traffic_cfg_ptr
)
{
  uint16                                    tdsmeas_id;
  uint8                                     i;
  tdsmaci_ul_traffic_cfg_index_type            *int_cfg_param_ptr;
  tdsmaci_ul_traffic_cfg_param                 *int_tmp_cfg_ptr;
  boolean                                   status_ind=TRUE;
  tdsmac_ul_traffic_report_mode_e_type         tdsreport_mode;
  tdsmac_ul_traffic_meas_report_qty_type       *report_qty;

  tdsmeas_id = oc_traffic_cfg_ptr->meas_id;

  /*--------------------------------------------------------------------------------
  Measurement identity not valid. tdsmeas_id can range from 1-16
  --------------------------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    status_ind=FALSE;
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else if ( (oc_traffic_cfg_ptr->meas_start_incl == TRUE) && 
            (TDSGET_UL_CCTRCH_TYPE==TDSINVALID_UL_CCTRCH))
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC UL transport channels not configured");
    status_ind=FALSE;
  }
  else
  {
    /*-----------------------------------------------------------------------------
    Get the appropriate entry for this tdsmeas_id from the config table internally
    maintained by MAC.
    ------------------------------------------------------------------------------*/
    int_cfg_param_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);

    /*-----------------------------------------------------------------------------
    If the traffic_cfg_ptr is not NULL then we have already received a setup cmd
    for this measurement identity. "valid=TRUE" flag indicates if the correspoding
    structures have valid parametrs in them.
    -----------------------------------------------------------------------------*/
    if (int_cfg_param_ptr->traffic_cfg_ptr != NULL)
    {
      TDSL2_MSG1(MSG_LEGACY_HIGH,"Another setup command recvd for meas_id %d",tdsmeas_id);
      int_tmp_cfg_ptr = int_cfg_param_ptr->traffic_cfg_ptr;

      if ((int_tmp_cfg_ptr->report_mode == TDSEVENT_TRIGGERED_MODE) && (--tdsnum_event_measurements == 0))
      {
        (void)rex_clr_timer(&tdsmac_ul_tvm_event_tmr_blk);
         tdsmac_tvm_event_timer_started = FALSE;
         TDSL2_MSG1(MSG_LEGACY_HIGH,"Num_event_meas = %d",tdsnum_event_measurements);
      }
      else if ((int_tmp_cfg_ptr->report_mode == TDSPERIODIC_MODE) && (--tdsnum_periodic_measurements == 0))
      {
        (void)rex_clr_timer(&tdsmac_ul_tvm_periodic_tmr_blk);
        tdsmac_tvm_periodic_timer_started = FALSE;
        TDSL2_MSG1(MSG_LEGACY_HIGH,"tdsnum_periodic_measurements = %d",tdsnum_periodic_measurements);
      }

      memset(int_tmp_cfg_ptr,0x00, sizeof(tdsmaci_ul_traffic_cfg_param));
    }
    else
    {
      /*---------------------------------------------------------------------------
      Allocate memory from tmc_heap.
      ----------------------------------------------------------------------------*/
      int_tmp_cfg_ptr = modem_mem_alloc(sizeof(tdsmaci_ul_traffic_cfg_param), MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);

      if (int_tmp_cfg_ptr==NULL)
      {
        status_ind = FALSE;
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
      }
      else
      {
        memset(int_tmp_cfg_ptr,0x00, sizeof(tdsmaci_ul_traffic_cfg_param));
      }
    }
    if (status_ind)
    {
      if (oc_traffic_cfg_ptr->meas_obj_incl)
      {

        int_tmp_cfg_ptr->num_trch = oc_traffic_cfg_ptr->meas_obj_list.num_trch; /*lint !e613*/
        /*--------------------------------------------------------------------------
           If the transport channels exist then copy them to the local memory
          --------------------------------------------------------------------------*/
        /*memcpy(int_tmp_cfg_ptr->trch_info,oc_traffic_cfg_ptr->meas_obj_list.trch_info,
               TDSUE_MAX_TRCH*sizeof(tdsmac_ul_traffic_trch_info_type));*/

        memscpy(int_tmp_cfg_ptr->trch_info,
                TDSUE_MAX_TRCH*sizeof(tdsmac_ul_traffic_trch_info_type),
                oc_traffic_cfg_ptr->meas_obj_list.trch_info,
               TDSUE_MAX_TRCH*sizeof(tdsmac_ul_traffic_trch_info_type));

        for (i=0;i<int_tmp_cfg_ptr->num_trch && i < TDSUE_MAX_TRCH;i++) /*lint !e613*/
        {
          if ((int_tmp_cfg_ptr->trch_info[i].trch_type==TDSRACH)&&
              ((TDSGET_UL_CCTRCH_TYPE==TDSRACH) || (oc_traffic_cfg_ptr->meas_start_incl == FALSE)))
          {
            int_tmp_cfg_ptr->trch_info[i].trch_id = tdsint_ul_trch_info[0].trch_id; /*lint !e613*/
          }
        }
      }/*meas_obj_list_incl */ /*lint !e613 */
      else
      {
        /*----------------------------------------------------------------------------
        Populate the transport channel id and the type information from all the
        uplink transport channels currently configured in MAC.
        ----------------------------------------------------------------------------*/
        int_tmp_cfg_ptr->num_trch= tdspopulate_object_list(int_tmp_cfg_ptr->trch_info); /*lint !e613*/
      }

      /*-------------------------------------------------------------------------
       Checks if the measurement interval is specified when we are
       asked to report the average or variance of rlc bufferpayload
      --------------------------------------------------------------------------*/
      /* If all the results of the checks are valid */
      if (status_ind && tdscheck_meas_report_qty(oc_traffic_cfg_ptr))
      {
        /*-----------------------------------------------------------------------
        Copy the report quantity(indicates if we have to report average, raw,
        variance buffer payload. Copy the measuremen to calculate the average or
        variance
        -----------------------------------------------------------------------*/
        report_qty = &oc_traffic_cfg_ptr->meas_report_qty;
        int_tmp_cfg_ptr->avg_payload = report_qty->avg_payload; /*lint !e613*/
        int_tmp_cfg_ptr->rlc_payload = report_qty->rlc_payload; /*lint !e613*/
        int_tmp_cfg_ptr->var_payload = report_qty->var_payload; /*lint !e613*/

        /*---------------------------------------------------------------------
        Number of samples is given by dividing the measurement interval by 10.
        We take measurement every 10ms.
        ---------------------------------------------------------------------*/
        int_tmp_cfg_ptr->nsamples=(uint16)oc_traffic_cfg_ptr->meas_qty.meas_interval/10; /*lint !e613*/

        /*---------------------------------------------------------------------
        Copy the report mode. this indicates if we have to report periodic or
        event triggered.
        ---------------------------------------------------------------------*/
        tdsreport_mode = oc_traffic_cfg_ptr->meas_report_mode.report_mode;

        /*---------------------------------------------------------------------
        Copy the relevant mode related parameters to the local memory.
        ---------------------------------------------------------------------*/
        if (tdsreport_mode==TDSPERIODIC_MODE)
        {
          /*---------------------------------------------------------------------
          If the mode is periodic then check the periodic mode related parameters
          and then copy them to the local memory.
          ---------------------------------------------------------------------*/
          status_ind= tdscheck_n_copy_periodic_params(int_tmp_cfg_ptr,oc_traffic_cfg_ptr);
        }
        else if (tdsreport_mode==TDSEVENT_TRIGGERED_MODE)
        {
          /*---------------------------------------------------------------------
          If the mode is event triggered then check the mode related parameters
          and then copy them to the local memory.
          ---------------------------------------------------------------------*/
          status_ind = tdscheck_n_copy_event_trigger_params(int_tmp_cfg_ptr,
                       &oc_traffic_cfg_ptr->meas_report_criteria.event_cfg);
        }
      }
      /*------------------------------------------------------------------------
       All the necessary config validations are done above. Now if the status
       indication is FALSE then free the memory. Otherwise store the pointer in
       the config index table internally by MAC
      -------------------------------------------------------------------------*/
      if (!status_ind)
      {
        /*Free the memory */
        modem_mem_free(int_tmp_cfg_ptr, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"Setup configuration error");
      }
      else
      {
        /*----------------------------------------------------------------------
         Copy the tranfer(UM or AM) & report mode (periodic or event triggered)
        ----------------------------------------------------------------------*/
        int_tmp_cfg_ptr->report_mode=
        oc_traffic_cfg_ptr->meas_report_mode.report_mode; /*lint !e613*/
        int_tmp_cfg_ptr->transfer_mode=
        oc_traffic_cfg_ptr->meas_report_mode.transfer_mode;/*lint !e613*/

        /*----------------------------------------------------------------------
         Initialize the first_meas flag to TRUE indicating that this is the
         first measurement taken for this measurement identity. This boolean
         is used when we have the measurement interval greater than the
         report interval and we have to compute average or variance.
        ----------------------------------------------------------------------*/
        int_tmp_cfg_ptr->first_meas = TRUE; /*lint !e613*/

        /*---------------------------------------------------------------------
          Assign the appropriate pointers and set the valid flag to TRUE
          indicating that all the parameters are valid.
        ----------------------------------------------------------------------*/
        int_cfg_param_ptr->traffic_cfg_ptr = int_tmp_cfg_ptr;
        int_cfg_param_ptr->valid = TRUE;

        int_tmp_cfg_ptr->meas_obj_list_incl = oc_traffic_cfg_ptr->meas_obj_incl; /*lint !e613*/

        /*---------------------------------------------------------------------
        The 10ms traffic volume timer is started when there is atleast one
        measurement to make. This timer is cleared when there are no
        measurements to make.
        ----------------------------------------------------------------------*/
        /*-----------------------------------------------------------------
        start the 10ms timer for the traffic volume measurements if there is
        atleast  one traffic volume measurement to make.
        ---------------------------------------------------------------------*/

        if (int_cfg_param_ptr->traffic_cfg_ptr->report_mode == TDSPERIODIC_MODE)
        {
          tdsnum_periodic_measurements++;
        }
        else if (int_cfg_param_ptr->traffic_cfg_ptr->report_mode == TDSEVENT_TRIGGERED_MODE)
        {
          tdsnum_event_measurements++;
        }

        if (oc_traffic_cfg_ptr->meas_start_incl)
        {
          int_cfg_param_ptr->suspend_meas = FALSE;

          if ((tdsnum_event_measurements > 0) && (!tdsmac_tvm_event_timer_started))
          {
            (void)rex_set_timer(&tdsmac_ul_tvm_event_tmr_blk,(uint32)TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE);
            tdsmac_tvm_event_timer_started = TRUE;
            TDSL2_MSG1(MSG_LEGACY_HIGH,"Starting tdsmac_ul_tvm_event_tmr_blk timer %d ms",TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE);
          }
          if ((tdsnum_periodic_measurements > 0) && (!tdsmac_tvm_periodic_timer_started))
          {
            (void)rex_set_timer(&tdsmac_ul_tvm_periodic_tmr_blk,
                   (uint32)(TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE));
            tdsmac_tvm_periodic_timer_started = TRUE;
            TDSL2_MSG1(MSG_LEGACY_HIGH,"Starting tdsmac_ul_tvm_periodic_tmr_blk timer %d ms",(TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE));
          }
          
          TDSL2_MSG2(MSG_LEGACY_HIGH,"Starting measurements num_event_meas %d,num_periodic_meas %d",tdsnum_event_measurements,tdsnum_periodic_measurements);
        }
        else
        {
          int_cfg_param_ptr->suspend_meas = TRUE;
          tdsmac_tvm_event_timer_started = FALSE;
          tdsmac_tvm_periodic_timer_started = FALSE;
          TDSMAC_MSG0(MSG_LEGACY_HIGH,"Setup recvd but suspended for now ");
        }

      }/*status_ind==TRUE */

    }/*int_tmp_cfg_ptr !=NULL */

  }/* Measurement identity is valid */

  return status_ind;
}
/*========================================================================================
 FUNCTION NAME  tdsmac_ulhs_update_raw_payload_for_eul()

 PURPOSE        This function calculates the raw rlc payload for all the radio bearers
                mapping on e-dch.
=========================================================================================*/
void tdsmac_ulhs_update_raw_payload_for_eul
(
  void
)
{
  uint8                           lc_id;
  uint16                          i;
  rlc_lc_id_type                  rlc_id;
  tdsrlc_ul_lc_buf_status_type   *lc_buf_ptr;

  for (i=0;i< tdsmac_hs_ul_lc_info.nchan;i++)
  {
    rlc_id = tdsmac_hs_ul_lc_info.rlc_id[i];
     
    /*Get LC-ID provoded by the N\W*/
    lc_id = tdsmac_ulhs_rlc_id_mac_dflow_lc_id_mapping[rlc_id].lc_id;
     
    /*Check if the LC id and MAc D flow are valid */
    
    lc_buf_ptr = &tdstraffic_ul_buf_status.chan_info[rlc_id];

   /*-----------------------------------------------------------------------
      hupa pdu type is either unacknowledged or acknowledged mode
    ------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------
    "idx" gives the correct location at which the raw rlc payload
    data has to be stored. we maintain an array of 26 elements.This
    array is used as a circular array.
    ----------------------------------------------------------------------*/

    tdsmac_ulhs_update_total_bytes_for_eul_lc(lc_buf_ptr,lc_id);

    tdsrb_raw_payload[tdsmaculhs_config_info.maculhs_ul_logical_ch_mac_d_flow_mapping[lc_id].rb_id][tdscur_index] = lc_buf_ptr->nbytes;

  }/* loop tdstraffic_ul_lc_info.nchan */
}
/*========================================================================================
 FUNCTION NAME  tdscompute_obj_list_raw_payload()

 PURPOSE        This function calculates the raw rlc payload for all the radio bearers.

=========================================================================================*/
void tdscompute_obj_list_raw_payload(void  )
{
  uint16                      i,j;
  tdsul_trch_info_type           *trch_ptr;
  tdsul_lc_info_type             *lcinfo_ptr;
  rlc_lc_id_type              rlc_id;
  tr_ch_id_type               tdstrch_id;
  tdsrlc_ul_lc_buf_status_type   *lc_buf_ptr;
  tdstrch_id_to_idx_map_type     *id_to_idx_ptr;

  uint32                      pdu_size;
  uint16                      npdus;
  uint16                      hdr_len;

  REX_ISR_LOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  /* Clear the previous buffer status */
  memset(&tdstraffic_ul_buf_status,0, sizeof(tdsrlc_ul_buf_status_type));

  if (tdstraffic_ul_lc_info.nchan > 0 )
  {
    tds_rlc_buffer_call_from_ISR = FALSE;
    tdsrlc_ul_enh_buffer_status(&tdstraffic_ul_lc_info,&tdstraffic_ul_buf_status);
  }

  for (i = 0 ; i<MAX_TRCH; i++)
  {
    tdstrch_id = i + TDSMAC_UL_MIN_TRCH_ID ;

    /*------------------------------------------------------------------------------
      Get the transport channel information for this transport channel index given by
      tdstrch_id
    ------------------------------------------------------------------------------*/
    id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);

    /*------------------------------------------------------------------------------
    If the transport channel is valid.
    ------------------------------------------------------------------------------*/
    if (id_to_idx_ptr->valid)
    {
      /*----------------------------------------------------------------------------
      Get the transport channel information pertaining to this tarnsport channel id.
      "idx" gives the index into the table where the information is stored.
      ----------------------------------------------------------------------------*/
      trch_ptr = TDSGET_UL_TRCH_INFO_PTR(id_to_idx_ptr->idx);

      /*----------------------------------------------------------------------------
      Go through the logical channels which are mapped onto this transport
      channels
      ----------------------------------------------------------------------------*/
      for (j=0; j<trch_ptr->nlchan &&
          j < TDSUE_MAX_UL_LOGICAL_CHANNEL ;j++)
      {
        /*-------------------------------------------------------------------------
        Get the rlc id's which are mapped to this transport channel.
        --------------------------------------------------------------------------*/
        rlc_id = trch_ptr->rlc_id[j];

        /*-------------------------------------------------------------------------
          Get the logical channel information for this RLC ID given by rlc_id.
        --------------------------------------------------------------------------*/
        lcinfo_ptr = TDSGET_UL_LC_INFO_PTR(rlc_id);

        /*-------------------------------------------------------------------------
        Get the buffer status for this logical channel
        --------------------------------------------------------------------------*/
        lc_buf_ptr = &tdstraffic_ul_buf_status.chan_info[rlc_id];

        /*-------------------------------------------------------------------------
          if the Logical channel is enabled
        --------------------------------------------------------------------------*/
        if (lcinfo_ptr->enable)
        {
          switch (lc_buf_ptr->type)
          {
          /*-----------------------------------------------------------------------
          If the pdu type is either unacknowledged or acknowledged mode
          ------------------------------------------------------------------------*/
          case TDSFLEX_PDUS:
            /*---------------------------------------------------------------------
            "idx" gives the correct location at which the raw rlc payload
            data has to be stored. we maintain an array of 26 elements.This
            array is used as a circular array.
            ----------------------------------------------------------------------*/

            if (lcinfo_ptr->rlc_mode == UE_MODE_UNACKNOWLEDGED)
            {
              pdu_size = tdstrch_min_tb_size[id_to_idx_ptr->idx];
              hdr_len = 1; // number of bytes of header
            }
            else
            {
              pdu_size = lc_buf_ptr->pdu_size;
              hdr_len = 2; // number of bytes of header
            }

            lc_buf_ptr->nbytes = (lc_buf_ptr->new_data_bytes+lc_buf_ptr->retx_data_bytes+lc_buf_ptr->ctrl_data_bytes);

            if ((lc_buf_ptr->new_data_bytes) && (pdu_size > 0))
            {
              npdus = tdsmath_ceil(((lc_buf_ptr->new_data_bytes+lc_buf_ptr->retx_data_bytes)<<3), (pdu_size-lcinfo_ptr->rlc_hdr_length));
              npdus += tdsmath_ceil((lc_buf_ptr->ctrl_data_bytes<<3), (pdu_size-lcinfo_ptr->rlc_hdr_length));

              lc_buf_ptr->nbytes += (npdus * hdr_len);

              MSG_9(MSG_SSID_TDSCDMA_MAC, MSG_LEGACY_MED,
              "Rlc_id %d, new_data %d bytes, retx_data %d bytes,ctrl_data %d bytes, npdus %d, pdu_size %d, hdr_len %d bytes",
              rlc_id, lc_buf_ptr->new_data_bytes, lc_buf_ptr->retx_data_bytes,
              lc_buf_ptr->ctrl_data_bytes,npdus, pdu_size, hdr_len, 0, 0);
            }

            tdsrb_raw_payload[lcinfo_ptr->rb_id][tdscur_index] = lc_buf_ptr->nbytes;

            if (lc_buf_ptr->nbytes > 0)
            {
              TDSMAC_MSG2(MSG_LEGACY_HIGH,"rlc_id %d,buf_status %d",rlc_id,lc_buf_ptr->nbytes);
            }

            break;

          default:
            TDSMAC_MSG0(MSG_LEGACY_LOW,"Traffic volume not computed for TM pdus");
            break;

          } /* switch pdu type */

        } /* if logical channel enabled */

      }/* for number of logical channels */

    }/* IF the transport channel is valid */

  } /* for number of transport channels*/

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsmac_ulhs_update_raw_payload_for_eul();
#endif /* FEATURE_TDSCDMA_HSUPA */

  tdscur_index++;
  if (tdscur_index >= TDSTRAFFIC_MAX_SAMPLES)
  {
    tdsrolled_over = TRUE;
  }
  tdscur_index = tdscur_index%TDSTRAFFIC_MAX_SAMPLES;
  REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
}

/*=============================================================================
 FUNCTION NAME  tdscompute_obj_list_raw_payload_cu()

 PURPOSE        Calculates the raw rlc payload for all the radio bearers.
=============================================================================*/
void tdscompute_obj_list_raw_payload_cu
(
tdsmac_ul_rach_cctrch_config_type    *oc_rachptr,

tdsrlc_ul_buf_status_type            *int_traffic_ul_buf_status_ptr
)
{
  uint16                       i = 0, j = 0;
  rlc_lc_id_type               rlc_id;
  tdsrlc_ul_lc_buf_status_type    *lc_buf_ptr;
  tdstrch_id_to_idx_map_type      *id_to_idx_ptr;
  tdsmac_ul_ded_logch_config_type *oc_dlcptr;
  tdsrlc_ul_lc_info_type           int_mac_ul_lc_info;

  uint32                      pdu_size;
  uint16                      npdus;
  uint16                      hdr_len_bytes;

  REX_ISR_LOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  /* Clear the previous buffer status */
  memset(int_traffic_ul_buf_status_ptr, 0, sizeof(tdsrlc_ul_buf_status_type));
  
  int_mac_ul_lc_info.nchan = 0;
  
  /* udpate the internal structure with CCCH info, if present */
  if (oc_rachptr->ccch_enable)
  {
    int_mac_ul_lc_info.rlc_id[i] = oc_rachptr->ccch_id;
    int_mac_ul_lc_info.nchan += 1;
    i++;
  }

  if(oc_rachptr->ndlchan < MAX_DED_LOGCHAN_PER_TRCH)
  {
    while ((j < oc_rachptr->ndlchan) && (j < MAX_DED_LOGCHAN_PER_TRCH) && ((i+j) < TDSUE_MAX_UL_LOGICAL_CHANNEL))
    {
      int_mac_ul_lc_info.rlc_id[i+j] = oc_rachptr->dlchan_info[j].rlc_id;
      j++;
    }
    int_mac_ul_lc_info.nchan += oc_rachptr->ndlchan;

    if (int_mac_ul_lc_info.nchan > 0 )
    {
      tdsrlc_ul_enh_buffer_status(&int_mac_ul_lc_info, int_traffic_ul_buf_status_ptr);
    }

    /*----------------------------------------------------------------------------
      Get the transport channel information for this transport channel index given by
      tdstrch_id
    ------------------------------------------------------------------------------*/
    id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(oc_rachptr->trch_id);
    
    if (id_to_idx_ptr->valid)
    {
      for (j = 0; ((j < oc_rachptr->ndlchan) && (i < MAX_DED_LOGCHAN_PER_TRCH)); j++)
      {
        rlc_id = oc_rachptr->dlchan_info[j].rlc_id;
        
        /* Get the buffer status for this logical channel */
        lc_buf_ptr = &(int_traffic_ul_buf_status_ptr->chan_info[rlc_id]);

        /* Pointer to ordered config parameters of DCCH/DTCH */
        oc_dlcptr = &(oc_rachptr->dlchan_info[i]);
        
        switch (lc_buf_ptr->type)
        {
          /* If the pdu type is either unacknowledged or acknowledged mode */
          case TDSFLEX_PDUS:
          if (oc_dlcptr->rlc_mode == UE_MODE_UNACKNOWLEDGED)
          {
            pdu_size = tdstrch_min_tb_size[id_to_idx_ptr->idx];
            hdr_len_bytes = 1; // number of bytes of header
          }
          else
          {
            pdu_size = lc_buf_ptr->pdu_size;
            hdr_len_bytes = 2; // number of bytes of header
          }

          lc_buf_ptr->nbytes = (uint32) (lc_buf_ptr->new_data_bytes + 
                                         lc_buf_ptr->retx_data_bytes + 
                                         lc_buf_ptr->ctrl_data_bytes);

          if ((lc_buf_ptr->new_data_bytes) && (pdu_size > 0))
          {
            npdus = tdsmath_ceil(((lc_buf_ptr->new_data_bytes + lc_buf_ptr->retx_data_bytes) << 3), 
                              (pdu_size - (hdr_len_bytes << 3)));
            npdus += tdsmath_ceil((lc_buf_ptr->ctrl_data_bytes << 3), 
                               (pdu_size - (hdr_len_bytes << 3)));
            
            lc_buf_ptr->nbytes += (npdus * hdr_len_bytes);

            /*        Need to commented out For TASK LOCK           */
             MSG_9(MSG_SSID_TDSCDMA_MAC, MSG_LEGACY_MED,
            "Rlc_id %d, new_data %d bytes, retx_data %d bytes, ctrl_data %d bytes, npdus %d, pdu_size %d, hdr_len_bytes %d bytes",
            rlc_id, lc_buf_ptr->new_data_bytes, lc_buf_ptr->retx_data_bytes,
            lc_buf_ptr->ctrl_data_bytes, npdus, pdu_size, hdr_len_bytes, 0, 0);
            
          }

          TDSMAC_MSG2(MSG_LEGACY_HIGH,"TVM:rlc_id %d,buf_status %d",rlc_id,lc_buf_ptr->nbytes);
          break;
          
          default:
          TDSMAC_MSG0(MSG_LEGACY_LOW,"Traffic volume not computed for TM pdus");
          break;
        } /* switch pdu type */
      } /* for number of logical channels */
    } /* IF the transport channel is valid */
    else
    {
      TDSMAC_MSG1(MSG_LEGACY_ERROR,"MAC_ERR: Invalid RACH TrCh Id %d in MAC data base",oc_rachptr->trch_id);
    }
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Number of dedicated logical channels per transport channel exceed MAX_DED_LOGCHAN_PER_TRCH");
  }

  REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
}

/*==================================================================
 FUNCTION NAME tdsproc_event_trigger_criteria_cu

 PURPOSE  For all the TrCh which are configured for meas Id 4A,
          TrCh BO is calculated and checked against the threshold
          True - BO > Threshold, FALSE - BO < Threshold
===================================================================*/
boolean tdsproc_event_trigger_criteria_cu
(
/* Measurement identity */
uint16                          tdsmeas_id,

/* pointer to Rach config */
tdsmac_ul_rach_cctrch_config_type          *oc_rachptr,

/*Internal pointer to  UL Buffer status obtained from RLC */
tdsrlc_ul_buf_status_type                           *int_traffic_ul_buf_status_ptr
)
{
  tdsmaci_ul_traffic_trch_event_param         *event_cfg_ptr;
  uint32                                   tdstrch_bo=0; /* Buffer occupancy */
  uint16                                   i, j,nlchan;
  tdsmaci_ul_traffic_cfg_param                *traffic_param_ptr = NULL;
  tdsmaci_ul_traffic_cfg_index_type           *traffic_cfg_idx_ptr;
  tdsmac_ul_traffic_event_id_e_type           tdsevent_id;
  tr_ch_id_type                            tdstrch_id;
  boolean                                  trch_bo_crossed_threshold = FALSE;
  tdsmac_ul_cctrch_e_type                     tdstrch_type;
  uint16                                   ntrch = 0;
  tdstrch_id_to_idx_map_type                  *id_to_idx_ptr;
  uint8                                    nevents=0;
  rlc_lc_id_type                           rlc_id;
  tdsrlc_ul_lc_buf_status_type                *lc_buf_ptr;
  
  /*-----------------------------------------------------------------------------------
  Get the traffic config pointer and the intermediate measurement results corresponding
  to this measurement identity.
  ------------------------------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
    traffic_param_ptr = traffic_cfg_idx_ptr->traffic_cfg_ptr;
  
    if ((traffic_cfg_idx_ptr->valid == FALSE) || (traffic_param_ptr == NULL))
    {
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"Entry with invalid parameters in TVM info table");
      return FALSE;
    }  
    /* If the event transport channel list is included */
    if (traffic_param_ptr->event_trch_incl)
    {
      ntrch = traffic_param_ptr->event_num_trch;
    }
    else
    {
      /*--------------------------------------------------------------------------------
      If the event transport channel list is not included then apply the criteria to
      the transport channles listed in the measurement object list.
      --------------------------------------------------------------------------------*/
      ntrch = traffic_param_ptr->num_trch;
    }
  }
  
  /* go through the list of transport channels */
  for (i = 0; ((i < ntrch) && (i < TDSUE_MAX_TRCH) && (traffic_param_ptr!=NULL)); i++)
  {
    /* Initialise */
    tdstrch_bo = 0;
    trch_bo_crossed_threshold = FALSE;
    
    /* if the transport channel list was specified by RRC */
    if (traffic_param_ptr->event_trch_incl)
    {
      /* TRUE implies the event criteria was specified for each TrCh in the list */
      event_cfg_ptr = traffic_param_ptr->trch_event_param[i];

      /* Get the transport channel id and type*/
      tdstrch_id = traffic_param_ptr->event_trch_info[i].trch_id;
      tdstrch_type = traffic_param_ptr->event_trch_info[i].trch_type;
      nevents = (uint8)traffic_param_ptr->num_events[i];
    }
    else
    {
      /* 
       * FALSE implies only one event criteria is specified and it is applicable 
       * to all TrChs specified in the object list 
       */
       
      event_cfg_ptr = traffic_param_ptr->trch_event_param[0];
      nevents = (uint8)traffic_param_ptr->num_events[0];

      /*-------------------------------------------------------------------------------
      When the set of Transport channels on which the event trigger is defined is not
      provided explicitly, the UE shall apply it on the Transport channels specified
      by the measurement object.
      -------------------------------------------------------------------------------*/
      /* Get the transport channel id */
      tdstrch_id = traffic_param_ptr->trch_info[i].trch_id;
      tdstrch_type = traffic_param_ptr->trch_info[i].trch_type;
    }
    
    /* Check the TVM only for RACH */
    if ((tdstrch_type == TDSGET_UL_CCTRCH_TYPE) && (tdstrch_type == TDSRACH) 
         && (!TDSTRCH_ID_TO_IDX_IS_INVALID(tdstrch_id)))
    {
      id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);
      if (id_to_idx_ptr->valid)
      {
        for (nlchan = 0; 
             (nlchan < oc_rachptr->ndlchan) && (nlchan < MAX_DED_LOGCHAN_PER_TRCH); 
             nlchan++)
        {
          rlc_id = oc_rachptr->dlchan_info[nlchan].rlc_id;
          lc_buf_ptr = &(int_traffic_ul_buf_status_ptr->chan_info[rlc_id]);
          
          /* PDU type is either acknowledged mode or unacnowledged mode */
          if (lc_buf_ptr->type == TDSFLEX_PDUS)
          {
            tdstrch_bo += lc_buf_ptr->nbytes;
          }  /* if UM or AM PDU */
        } /* for number of logical channels */

        /*      commented out For TASK LOCK      */
        //TDSMAC_MSG2(MSG_LEGACY_HIGH,"TrChId %d - BO %d (bytes)",tdstrch_id,tdstrch_bo);
        
        /* for the number of events corresponding to the transport channel.could be 1 or 2 */
        for (j = 0; (j < nevents) && (j < TDSMAX_NUM_EVENTS); j++)
        {
          tdsevent_id = event_cfg_ptr[j].event_id;
          if (tdsevent_id == TDSEVENT_4A)
          {
            /*----------------------------------------------------------------------------
             If the tdsevent_id is 4A and if the transport channel BO is greater than the 
             threshold then trch_bo_crossed_threshold is set to TRUE.
             Return, if any one TrCh crosses the threshold
            ----------------------------------------------------------------------------*/
            trch_bo_crossed_threshold = ((tdstrch_bo > event_cfg_ptr[j].report_threshold) ? TRUE: FALSE);

            TDSMAC_MSG1(MSG_LEGACY_HIGH,"TVM: Trch_bo_crossed_threshold %d",trch_bo_crossed_threshold);
            
            if (trch_bo_crossed_threshold)
            {
              return trch_bo_crossed_threshold;
            }                 
          }
        }/*for event list */
      }
      else
      {
        TDSMAC_MSG1(MSG_LEGACY_ERROR,"Invalid RACH TrCh Id %d by RRC",tdstrch_id);
      }
    }
  }/* for number of transport channels */

  return trch_bo_crossed_threshold;
} /* proc_event_trigger_criteria_cu */

/*================================================================
 FUNCTION NAME  mac_compute_traffic_volume_above_threshold_cu

 PURPOSE  This function will be called by RRC when it needs to set the IE 
          "Traffic volume indicator", 
          if the IE "Cell update cause" is set to "uplink data transmission" 
          and Traffic volume event identity = "4a".
          TRUE - If BO > Threshold
          FALSE - If BO < Threshold
===================================================================*/
boolean tdsmac_compute_traffic_volume_above_threshold_cu
(
/* measurement id */
uint16                             tdsmeas_id,

/* pointer to Rach config */
tdsmac_ul_rach_cctrch_config_type    *oc_rachptr
)
{
  boolean                    ret_value = FALSE;
  tdsrlc_ul_buf_status_type     int_traffic_ul_buf_status; 
  
  /* In case of TVM */
  if (tdsmeas_id == 4)
  {
    if (TDSGET_UL_CCTRCH_TYPE == TDSINVALID_UL_CCTRCH)
    {
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"MAC UL transport channels not configured");
      return ret_value;
    }     
    /* Compute the raw payload for all the configured RBs */
    tdscompute_obj_list_raw_payload_cu(oc_rachptr, &int_traffic_ul_buf_status);
    
    /* Check whether the BO of any TrCh is greater than Threshold */
    ret_value = tdsproc_event_trigger_criteria_cu(tdsmeas_id, oc_rachptr, &int_traffic_ul_buf_status);
  }
  
  return (ret_value);
}

/*========================================================================================
 FUNCTION NAME tdscompute_trch_traffic_volume

 PURPOSE    This function computes the Trasnport channel traffic volume which is sum of
            buffer occupancies of RBs multiplexed onto the transport channel. The function
            returns the number of bytes so computed for the transport channel.
            Returns Buffer occupancy of all rbs multiplexed onto the same transport
            channel
=========================================================================================*/

uint32 tdscompute_trch_traffic_volume
(
/* Compute transport channel traffic volume for this transport channel id */
tr_ch_id_type     tdstrch_id,
tdsmac_ul_cctrch_e_type  tdstrch_type
)
{
  tdsul_trch_info_type                   *ul_trch_info_ptr;
  uint16                              nlchan;
  rlc_lc_id_type                      rlc_id;
  tdsrlc_ul_lc_buf_status_type           *lc_buf_ptr;
  tdsul_lc_info_type                     *lc_info_ptr;
  tdstrch_id_to_idx_map_type             *id_to_idx_ptr;
  uint32                              bo=0;

#if defined (FEATURE_TDSCDMA_HSUPA)
  uint8                                         i;
  uint8                                         lc_id;
  tdsmac_ulhs_logical_ch_mac_d_flow_info_type*  tds_ulhs_lch_info_ptr;

 
  

  if(tdstrch_type == TDSEDCH)
  {
    for (i=0;i< tdsmac_hs_ul_lc_info.nchan;i++)
    {
       rlc_id = tdsmac_hs_ul_lc_info.rlc_id[i];

       lc_id = tdsmac_ulhs_rlc_id_mac_dflow_lc_id_mapping[rlc_id].lc_id;

       tds_ulhs_lch_info_ptr = &tdsmaculhs_config_info.
                      maculhs_ul_logical_ch_mac_d_flow_mapping[lc_id];

       if(TRUE == tds_ulhs_lch_info_ptr->valid)
       {
          bo += tdstraffic_ul_buf_status.chan_info[rlc_id].nbytes;
       }
    }
  }
  else
  { /* none edch type */
#endif

  /*------------------------------------------------------------------------------
  Get the index into the table where the transport channel information for this
  tdstrch_id is stored.
  ------------------------------------------------------------------------------*/
  if (!TDSTRCH_ID_TO_IDX_IS_INVALID(tdstrch_id))
  {
    id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);
    
    if (!id_to_idx_ptr->valid)
    {
      TDSMAC_MSG1(MSG_LEGACY_LOW,"Transport channel invalid,tdstrch_id %d",tdstrch_id);
    }
    else
    {
      /*----------------------------------------------------------------------------
      Get the transport channel information corresponding to this transport channel
      id.
      ----------------------------------------------------------------------------*/
      ul_trch_info_ptr = TDSGET_UL_TRCH_INFO_PTR (id_to_idx_ptr->idx);
  
      /*----------------------------------------------------------------------------
      Go through all the logical channels that are mapped to this transport channel
      ----------------------------------------------------------------------------*/
      for (nlchan=0; nlchan < ul_trch_info_ptr->nlchan &&
          nlchan < TDSUE_MAX_UL_LOGICAL_CHANNEL; nlchan++)
      {
        /* Get the logical channel id */
        rlc_id = ul_trch_info_ptr->rlc_id[nlchan];
  
        /* Get the logical channel information */
        lc_info_ptr = TDSGET_UL_LC_INFO_PTR(rlc_id);
  
        /* Logical channel enabled */
        if (lc_info_ptr->enable)
        {
          lc_buf_ptr = &tdstraffic_ul_buf_status.chan_info[rlc_id];
  
          /* PDU type is either acknowledged mode or unacnowledged mode */
          if (lc_buf_ptr->type == TDSFLEX_PDUS)
          {
            bo += lc_buf_ptr->nbytes;
  
          }  /* if UM or AM PDU */
  
        }  /*logical channel enabled */
  
      } /* for number of logical channels */
  
    }/*if transport channel valid */
  }
#if defined (FEATURE_TDSCDMA_HSUPA)
  }/* none edch type */
#endif
  return bo;
}
/*========================================================================================
 FUNCTION NAME  tdsprepare_eul_meas_report()

 PURPOSE        This function prepares the measurement report for rbs mapping on edch. 
                This function calculates the average, variance or raw payload as 
                specified in the report quantity.

=========================================================================================*/
void tdsprepare_eul_meas_report(
 tdsmaci_ul_traffic_cfg_param       *int_traffic_param_ptr,
 tdsmac_ul_traffic_meas_ind_type    *tdsmeas_ind_ptr,
 uint16                             nsamples
                                )
{
  uint32                          sum,square_sum,var,raw_payload;
  uint32                          avg;
  rb_id_type                      rb_id;
  uint16                          fr_idx, cnt,idx;
  uint16                          from_index,meas_index;
  uint16                          j,rb_idx;
  rlc_lc_id_type                  rlc_id;
  uint8                           lc_id;

  if(TDSMAC_ULHS_ACTIVE == tdsmac_ulhs_state)
  {
    for(j = 0; j < tdsmac_hs_ul_lc_info.nchan; j++)
    {
      rlc_id = tdsmac_hs_ul_lc_info.rlc_id[j];

      lc_id  = tdsmac_ulhs_rlc_id_mac_dflow_lc_id_mapping[rlc_id].lc_id;

      /* Intialize them to 0 */
      sum=avg=square_sum = var= raw_payload=0;
      
      rb_id = tdsmaculhs_config_info.maculhs_ul_logical_ch_mac_d_flow_mapping[lc_id].rb_id;
      
      /*-----------------------------------------------------------------------
       Get the radio bearer index where we need to store the results of the
       computations.
      ------------------------------------------------------------------------*/
      
      rb_idx = tdsmeas_ind.num_rbs;
    
      /*-----------------------------------------------------------------------
        Store the radio bearer identity.
      ------------------------------------------------------------------------*/
      if (int_traffic_param_ptr->rlc_payload || int_traffic_param_ptr->avg_payload
        || int_traffic_param_ptr->var_payload)
      {
        tdsmeas_ind_ptr->rb_meas[rb_idx].rb_id = rb_id;

        /*--------------------------------------------------------------------------
          Increment the number of radio bearers.
        ---------------------------------------------------------------------------*/
        tdsmeas_ind_ptr->num_rbs++;
      }
    
      meas_index = int_traffic_param_ptr->current_idx;
      
      /*-----------------------------------------------------------------------
        Initialize raw, average, variance to highest possible value. This
        variables are populated depending on which quantity is required.
      ------------------------------------------------------------------------*/
      tdsmeas_ind.rb_meas[rb_idx].raw_rlc_payload = 0xFFFFFFFF;
      tdsmeas_ind.rb_meas[rb_idx].variance_rlc_payload = 0xFFFFFFFF;
      tdsmeas_ind.rb_meas[rb_idx].average_rlc_payload  = 0xFFFFFFFF;
    
      /*-----------------------------------------------------------------------
        If the raw rlc payload information is requested
       ------------------------------------------------------------------------*/
      if (int_traffic_param_ptr->rlc_payload)
      {
        /*---------------------------------------------------------------------
          Obtain the information stored in the last entry in the array. This
          entry is given subtracting 1 from the tdscur_index (
          tdscur_index - is the location where a new payload entry has to be stored).
         ----------------------------------------------------------------------*/
        idx = TDSTRAFFIC_IDX_DIFF(meas_index,1);
        if ((idx < TDSTRAFFIC_MAX_SAMPLES) && (rb_id < TDS_MAX_RB))
        {
          tdsmeas_ind.rb_meas[rb_idx].raw_rlc_payload = tdsrb_raw_payload[rb_id][idx];
          TDSMAC_MSG2(MSG_LEGACY_LOW,"RB_ID RAW_PAYLOAD %d %d",rb_id,tdsrb_raw_payload[rb_id][idx]);
        }
      }
    
      /*-----------------------------------------------------------------------
        If the average or variance rlc payload information is requested
      ------------------------------------------------------------------------*/
      if (int_traffic_param_ptr->avg_payload || int_traffic_param_ptr->var_payload)
      {
        /*----------------------------------------------------------------------
          obtain the "from_index" from where we need to start calculations
          for obtaining the average or variance.
        -----------------------------------------------------------------------*/
        from_index = TDSTRAFFIC_IDX_DIFF(meas_index, nsamples);
      
        /*----------------------------------------------------------------------
          Compute average.
        -----------------------------------------------------------------------*/
        if (nsamples==0)
        {
          TDSMAC_MSG0(MSG_LEGACY_LOW,"Interval is zero");
        }
        else
        {
          /*-------------------------------------------------------------------
            tdsrb_raw_payload is used as a circular array containing
            TDSTRAFFIC_MAX_SAMPLES. from_index gives the index from where we should
            start doing the calculations. interval gives
            me the number of elements needed to do the average.
          --------------------------------------------------------------------*/
          for (cnt=0, fr_idx=from_index ; ((cnt<nsamples) && (rb_id < TDS_MAX_RB));
                cnt++,fr_idx++)
          {
            fr_idx = fr_idx%TDSTRAFFIC_MAX_SAMPLES;
      
            raw_payload = tdsrb_raw_payload[rb_id][fr_idx];
            sum += raw_payload;
            TDSMAC_MSG2(MSG_LEGACY_LOW,"RAW PAYLOAD %d RB_ID %d",raw_payload,rb_id);
            if (int_traffic_param_ptr->var_payload)
            {
              square_sum += raw_payload*raw_payload;
            }
          }
          /*-----------------------------------------------------------------------
          Calculate the average rlc payload. Average is given by dividing the
          sum_rlc_payload by the number of samples. Number of samples is calculate
          by dividing the nsamples
          -----------------------------------------------------------------------*/
          avg = sum/nsamples;
      
          /*----------------------------------------------------------------------
          Compute variance.
          -----------------------------------------------------------------------*/
          if (int_traffic_param_ptr->var_payload)
          {
            var = (square_sum/nsamples) - (avg*avg);
            tdsmeas_ind.rb_meas[rb_idx].variance_rlc_payload = var;
            TDSMAC_MSG2(MSG_LEGACY_LOW,"RB_ID VAR_PAYLOAD %d %d",rb_id,var);
          }
          /*----------------------------------------------------------------------
          Compute average.
          -----------------------------------------------------------------------*/
          if (int_traffic_param_ptr->avg_payload)
          {
            tdsmeas_ind.rb_meas[rb_idx].average_rlc_payload = avg;
            TDSMAC_MSG2(MSG_LEGACY_LOW,"RB_ID AVG_PAYLOAD %d %d",rb_id,avg);
          }
        }/* nsamples>0*/
    
      }/*if we are asked to report either avergae or variance */

    }/* loop tdsmac_hs_ul_lc_info.nchan */

  }/* TDSMAC_ULHS_ACTIVE == tdsmac_ulhs_state */
}

/*========================================================================================
 FUNCTION NAME  tdsprepare_n_send_traffic_meas_report()

 PURPOSE        This function prepares the measurement report for the measurement id
                specified. This function calculates the average, variance or raw payload
                as specified in the report quantity.

=========================================================================================*/

void tdsprepare_n_send_traffic_meas_report
(
/* Measurement identity */
uint16                          tdsmeas_id
)
{
  uint16                          i,j,rb_idx,nsamples;
  uint16                          fr_idx, cnt,idx;
  uint32                          avg;
  uint32                          sum,square_sum,var,raw_payload;
  uint16                          ntrch = 0;
  tdstrch_id_to_idx_map_type         *id_to_idx_ptr;
  tdsmaci_ul_traffic_cfg_param       *int_traffic_param_ptr;
  tdsul_trch_info_type               *trch_info_ptr;
  rlc_lc_id_type                  rlc_id;
  tdsul_lc_info_type                 *lcinfo_ptr;
  rb_id_type                      rb_id;
  uint16                          from_index,meas_index;
  tr_ch_id_type                   tdstrch_id;
  tdsmaci_ul_traffic_cfg_index_type  *traffic_cfg_idx_ptr;

  memset (&tdsmeas_ind,0,sizeof(tdsmac_ul_traffic_meas_ind_type));

  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    /*--------------------------------------------------------------------------------
    Get the traffic config pointer and the intermediate measurement results
    corresponding to this measurement identity.
    --------------------------------------------------------------------------------*/
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
    int_traffic_param_ptr   = traffic_cfg_idx_ptr->traffic_cfg_ptr;
  
    tdsmeas_ind.meas_id = tdsmeas_id;
    /*--------------------------------------------------------------------------------
    Copy the report mode. This can be either periodic or event triggered.
    --------------------------------------------------------------------------------*/
    tdsmeas_ind.report_mode = int_traffic_param_ptr->report_mode;
  
    /*--------------------------------------------------------------------------------
    Copy the report transfer mode. This can be either unacknowledged or acknowledged
    --------------------------------------------------------------------------------*/
    tdsmeas_ind.report_transfer_mode = int_traffic_param_ptr->transfer_mode;
  
    /*--------------------------------------------------------------------------------
    Initialize number of rb's to 0.
    --------------------------------------------------------------------------------*/
    tdsmeas_ind.num_rbs = 0;
  
    /*--------------------------------------------------------------------------------
    Initialize num_reports_ind to FALSE. This is set to true when we have reached
    the maximum number of reports that is required to send.
    --------------------------------------------------------------------------------*/
    tdsmeas_ind.num_reports_ind = FALSE;
  
    /*--------------------------------------------------------------------------------
    If the object list is included then send report for only those transport channels
    which are specified in the object list.
    --------------------------------------------------------------------------------*/
    ntrch = int_traffic_param_ptr->num_trch;
  
    /*-----------------------------------------------------------------------
    If the mode is periodic and the measurement interval (given by nsamples*10)
    is greater than the report interval, then the first time we send a meas
    report is treated as a special case.
    -----------------------------------------------------------------------*/
    /*--------------------------------------------------------------------
    This will take care of the special case when the measurement
    interval is greater than the report interval and this is the first
    time it is happening since start of the measurements for this ID.
    --------------------------------------------------------------------*/
    /*---------------------------------------------------------------------
    If an event was triggered and we were asked to report the average or
    variance based on nsamples and if we don't have nsamples information
    (this can happen when an event is triggered in the first 20 to 30 ms
    since we start doing measurements) then we compute average or variance
    based on the number of samples we currently have.
    ----------------------------------------------------------------------*/
  
    if (int_traffic_param_ptr->first_meas)
    {
      if (tdsrolled_over || (int_traffic_param_ptr->current_idx>=
                          int_traffic_param_ptr->nsamples))
      {
        int_traffic_param_ptr->first_meas = FALSE;
        nsamples = int_traffic_param_ptr->nsamples;
      }
      else
      {
        nsamples = int_traffic_param_ptr->current_idx;
      }
    }
    else
    {
      /*----------------------------------------------------------------------
      Get the number of samples that needs to be taken for computing the
      average or variance
      -----------------------------------------------------------------------*/
      nsamples = int_traffic_param_ptr->nsamples ;
    }
  
    for (i=0; (i < ntrch) && (i < TDSUE_MAX_TRCH)  ; i++)
    {
      tdstrch_id = int_traffic_param_ptr->trch_info[i].trch_id;
      /*----------------------------------------------------------------------------
      Get the index into the transport channel info table for this transport channel
      identity.
      -----------------------------------------------------------------------------*/
      if (!TDSTRCH_ID_TO_IDX_IS_INVALID(tdstrch_id))
      {
#if defined (FEATURE_TDSCDMA_HSUPA)
        if(TDSEDCH == int_traffic_param_ptr->trch_info[i].trch_type)
        {
          tdsprepare_eul_meas_report(int_traffic_param_ptr, &tdsmeas_ind,nsamples);
          continue;
        }
#endif
        id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);
    
        if ((int_traffic_param_ptr->trch_info[i].trch_type==TDSGET_UL_CCTRCH_TYPE)
            &&(id_to_idx_ptr->valid))
        {
          /*---------------------------------------------------------------------------
          Get the transport channel information pertaining to this transport channel id.
          ----------------------------------------------------------------------------*/
          trch_info_ptr = TDSGET_UL_TRCH_INFO_PTR(id_to_idx_ptr->idx);
    
          /*---------------------------------------------------------------------------
          Go through the list of logical channels which are mapped onto this transport
          channel.
          ----------------------------------------------------------------------------*/
          for (j=0;j<trch_info_ptr->nlchan && j < TDSUE_MAX_UL_LOGICAL_CHANNEL;j++)
          {
            /* Intialize them to 0 */
            sum=avg=square_sum = var= raw_payload=0;
            rlc_id = trch_info_ptr->rlc_id[j];
    
            /*-------------------------------------------------------------------------
            Get the logical channel information for this rlc_id.
            --------------------------------------------------------------------------*/
            lcinfo_ptr = TDSGET_UL_LC_INFO_PTR(rlc_id);
    
            /*-------------------------------------------------------------------------
            If the logical channel is enabled.
            --------------------------------------------------------------------------*/
            if (lcinfo_ptr->enable)
            {
              /*-----------------------------------------------------------------------
              Get the radio bearer id to which this logical channel is mapped.
              ------------------------------------------------------------------------*/
              rb_id = lcinfo_ptr->rb_id;
    
              if (rb_id==0) continue;
    
              /* If this a TM radio bearer then do not attach this RB in the measurement
              report list */
              if (lcinfo_ptr->rlc_mode == UE_MODE_TRANSPARENT)
              {
                continue;
              }
    
              /*-----------------------------------------------------------------------
              Get the radio bearer index where we need to store the results of the
              computations.
              ------------------------------------------------------------------------*/
              rb_idx = tdsmeas_ind.num_rbs;
    
              /*-----------------------------------------------------------------------
              Store the radio bearer identity.
              ------------------------------------------------------------------------*/
              if (int_traffic_param_ptr->rlc_payload || int_traffic_param_ptr->avg_payload
                  || int_traffic_param_ptr->var_payload)
              {
                if (rb_idx < TDS_MAX_RB)
                {
                tdsmeas_ind.rb_meas[rb_idx].rb_id = rb_id;
                }
                else
                {
                  TDSMAC_MSG1(MSG_LEGACY_ERROR,"num of RBs %d exceeded max rb_meas bound",tdsmeas_ind.num_rbs);
                }
    
                /*--------------------------------------------------------------------------
                Increment the number of radio bearers.
                ---------------------------------------------------------------------------*/
                tdsmeas_ind.num_rbs++;
              }
    
              meas_index = int_traffic_param_ptr->current_idx;
    
              /*-----------------------------------------------------------------------
              Initialize raw, average, variance to highest possible value. This
              variables are populated depending on which quantity is required.
              ------------------------------------------------------------------------*/
              if (rb_idx < TDS_MAX_RB)
              {
                tdsmeas_ind.rb_meas[rb_idx].raw_rlc_payload = 0xFFFFFFFF;
                tdsmeas_ind.rb_meas[rb_idx].variance_rlc_payload = 0xFFFFFFFF;
                tdsmeas_ind.rb_meas[rb_idx].average_rlc_payload  = 0xFFFFFFFF;
              }
              else
              {
                TDSMAC_MSG2(MSG_LEGACY_ERROR,"MAC_ERR: num of RBs %d exceeded max rb_meas bound,rb_idx %d",tdsmeas_ind.num_rbs,rb_idx);
              }    
              /*-----------------------------------------------------------------------
              If the raw rlc payload information is requested
              ------------------------------------------------------------------------*/
              if (int_traffic_param_ptr->rlc_payload)
              {
                /*---------------------------------------------------------------------
                Obtain the information stored in the last entry in the array. This
                entry is given subtracting 1 from the tdscur_index (
                tdscur_index - is the location where a new payload entry has to be stored).
                ----------------------------------------------------------------------*/
                idx = TDSTRAFFIC_IDX_DIFF(meas_index,1);
                if ((idx < TDSTRAFFIC_MAX_SAMPLES) && (rb_id < TDS_MAX_RB) && (rb_idx < TDS_MAX_RB))
                {
                  tdsmeas_ind.rb_meas[rb_idx].raw_rlc_payload = tdsrb_raw_payload[rb_id][idx];
                  TDSMAC_MSG2(MSG_LEGACY_LOW,"RB_ID RAW_PAYLOAD %d %d",rb_id,tdsrb_raw_payload[rb_id][idx]);
                }
              }
    
              /*-----------------------------------------------------------------------
              If the average or variance rlc payload information is requested
              ------------------------------------------------------------------------*/
              if (int_traffic_param_ptr->avg_payload || int_traffic_param_ptr->var_payload)
              {
                /*----------------------------------------------------------------------
                  obtain the "from_index" from where we need to start calculations
                  for obtaining the average or variance.
                -----------------------------------------------------------------------*/
                from_index = TDSTRAFFIC_IDX_DIFF(meas_index, nsamples);
    
                /*----------------------------------------------------------------------
                  Compute average.
                -----------------------------------------------------------------------*/
                if (nsamples==0)
                {
                  TDSMAC_MSG0(MSG_LEGACY_LOW,"Interval is zero");
                }
                else
                {
                  /*-------------------------------------------------------------------
                  tdsrb_raw_payload is used as a circular array containing
                  TDSTRAFFIC_MAX_SAMPLES. from_index gives the index from where we should
                  start doing the calculations. interval gives
                  me the number of elements needed to do the average.
                  --------------------------------------------------------------------*/
                  for (cnt=0, fr_idx=from_index ; ((cnt<nsamples) && (rb_id < TDS_MAX_RB));
                        cnt++,fr_idx++)
                  {
                    fr_idx = fr_idx%TDSTRAFFIC_MAX_SAMPLES;
    
                    raw_payload = tdsrb_raw_payload[rb_id][fr_idx];
                    sum += raw_payload;
                    TDSMAC_MSG2(MSG_LEGACY_LOW,"RAW PAYLOAD %d RB_ID %d",raw_payload,rb_id);
                    if (int_traffic_param_ptr->var_payload)
                    {
                      square_sum += raw_payload*raw_payload;
                    }
                  }
                  /*-----------------------------------------------------------------------
                  Calculate the average rlc payload. Average is given by dividing the
                  sum_rlc_payload by the number of samples. Number of samples is calculate
                  by dividing the nsamples
                  -----------------------------------------------------------------------*/
                  avg = sum/nsamples;
    
                  /*----------------------------------------------------------------------
                  Compute variance.
                  -----------------------------------------------------------------------*/
                  if ((int_traffic_param_ptr->var_payload) && (rb_idx < TDS_MAX_RB))
                  {
                    var = (square_sum/nsamples) - (avg*avg);
                    tdsmeas_ind.rb_meas[rb_idx].variance_rlc_payload = var;
                    TDSMAC_MSG2(MSG_LEGACY_LOW,"RB_ID VAR_PAYLOAD %d %d",rb_id,var);
                  }
                  /*----------------------------------------------------------------------
                  Compute average.
                  -----------------------------------------------------------------------*/
                  if ((int_traffic_param_ptr->avg_payload) && (rb_idx < TDS_MAX_RB))
                  {
                    tdsmeas_ind.rb_meas[rb_idx].average_rlc_payload = avg;
                    TDSMAC_MSG2(MSG_LEGACY_LOW,"RB_ID AVG_PAYLOAD %d %d",rb_id,avg);
                  }
                }/* nsamples>0*/
    
              }/*if we are asked to report either avergae or variance */
    
            }/*logical channel enabled */
    
          }/* for number of logical channels */
    
        }/* if the transport channel is valid */
      
      }
      
    }/* for number of transport channels */
  
    if (tdsprepare_addtl_meas_incl == TRUE)
    {
      tdsprepare_addtl_meas_incl = FALSE;
      return;
    }
    if (int_traffic_param_ptr->report_mode==TDSPERIODIC_MODE)
    {
      /*-------------------------------------------------------------------------------
      If there is atleast one radio bearer for which results have to be reported then
      report. Else don't send the report.
      -------------------------------------------------------------------------------*/
      /*if (tdsmeas_ind.num_rbs>0) */
      {
        /*----------------------------------------------------------------------------
        If we have sent the number of reports that RRC has requested us to send then
        indicate to RRC that this will be the last report that we would eb sending
        for this measurement id in the case of periodic mode.
        -----------------------------------------------------------------------------*/
        int_traffic_param_ptr->nreports_sent++;
  
        if ((!int_traffic_param_ptr->report_infinity) &&
            (int_traffic_param_ptr->nreports_sent == int_traffic_param_ptr->num_reports))
        {
          tdsmeas_ind.num_reports_ind = TRUE;
        }
        /*----------------------------------------------------------------------------
        Post the measurement results to RRC.
        -----------------------------------------------------------------------------*/
        tdsmaci_post_traffic_volume_meas_report(&tdsmeas_ind);
  #ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
  #ifdef FEATURE_TDSCDMA_TVM_BLOCK
        if (tdsmac_tvm_block_timer_started == FALSE)
        {
  #endif
          tdsmac_ul_send_tvm_report_packet(&tdsmeas_ind, int_traffic_param_ptr->nreports_sent);
  #ifdef FEATURE_TDSCDMA_TVM_BLOCK
        }
  #endif
  #endif
      }
    }
    else if (int_traffic_param_ptr->report_mode == TDSEVENT_TRIGGERED_MODE) /*event_triggered_mode */
    {
      /*if (tdsmeas_ind.num_rbs>0)*/
      {
        /*memcpy(&tdsmeas_ind.event_result, &int_traffic_param_ptr->event_result,
               sizeof(tdsmac_ul_traffic_event_result));*/

	    memscpy(&tdsmeas_ind.event_result, sizeof(tdsmac_ul_traffic_event_result), &int_traffic_param_ptr->event_result,
               sizeof(tdsmac_ul_traffic_event_result));
        /*-----------------------------------------------------------------------------
        Post the measurement results to RRC.
        ------------------------------------------------------------------------------*/
        tdsmaci_post_traffic_volume_meas_report(&tdsmeas_ind);
  
  #ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
  #ifdef FEATURE_TDSCDMA_TVM_BLOCK
        if (tdsmac_tvm_block_timer_started == FALSE)
        {
  #endif
          tdsmac_ul_send_tvm_report_packet(&tdsmeas_ind, 0);
  #ifdef FEATURE_TDSCDMA_TVM_BLOCK
        }
  #endif
  #endif
        }
    }
  
    /*--------------------------------------------------------------------------------
     Clear any old measurements associated with this measurement id.
    ---------------------------------------------------------------------------------*/
    memset (&int_traffic_param_ptr->event_result,0,sizeof(tdsmac_ul_traffic_event_result));
  }
  
}

/*-------------------------------------------------------------------
FUNCTION tdsmac_get_additional_traffic_vol_msmt_results

DESCRIPTION
This function would be called by RRC when it needs to append a TVM report
to one of intrafrequency or interfrequency measurement reports.

DEPENDENCIES
  None

RETURN VALUE
addtl_meas_ind contains the measurement results. Usually the RLC payload
information is sent back for all the RB's mapped onto a particular
transport channel.

SIDE EFFECTS
  None
--------------------------------------------------------------------*/
boolean
tdsmac_get_additional_traffic_vol_msmt_results
(
uint16 tdsmeas_id,
tdsmac_ul_traffic_meas_ind_type *addtl_meas_ind
)
{
  tdsmaci_ul_traffic_cfg_index_type    *int_cfg_param_ptr;
  boolean                           status=FALSE;

  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    int_cfg_param_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
  
    if (int_cfg_param_ptr->valid)
    {
      status = TRUE;
      tdsprepare_addtl_meas_incl = TRUE;
      tdsprepare_n_send_traffic_meas_report(tdsmeas_id);
      //memcpy(addtl_meas_ind, &tdsmeas_ind, sizeof(tdsmac_ul_traffic_meas_ind_type));	
      memscpy(addtl_meas_ind, sizeof(tdsmac_ul_traffic_meas_ind_type),&tdsmeas_ind, sizeof(tdsmac_ul_traffic_meas_ind_type));
    }
  }
  
  return status;
}

/*====================================================================================
 FUNCTION NAME tdsproc_event_trigger_criteria

 PURPOSE  For each transport channel listed in the report criteria for this
          measurement identity this function checks , if the transport channel traffic
          volume is greater than or lesser than the threshold value corresponding to
          the transport channel, based on the event (4A, 4B). It then sets a trigger
          and generates a report to be sent to RRC.

=======================================================================================*/

void tdsproc_event_trigger_criteria
(
/* Measurement identity */
uint16                          tdsmeas_id
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
,
boolean                         blk_flg
#endif
)
{
  tdsmaci_ul_traffic_trch_event_param         *event_cfg_ptr, *cur_evt_cfg_ptr;
  uint32                                   tdstrch_bo=0; /* Buffer occupancy */
  uint16                                   i, j;
#ifdef FEATURE_TDSCDMA_E4B_OPT
  uint16                                   k;  
#endif
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
  uint16                                   m=0;
  uint16                                   ntrch_blk;
#endif
  tdsmaci_ul_traffic_cfg_param                *traffic_param_ptr;
  tdsmaci_ul_traffic_cfg_index_type           *traffic_cfg_idx_ptr;
  boolean                                  send_status, prohibit_status;
  tdsmac_ul_traffic_event_id_e_type           tdsevent_id;
  tr_ch_id_type                            tdstrch_id;
  boolean                                  trch_bo_crossed_threshold=FALSE;
  boolean                                  prev_bo_crossed_threshold=FALSE;
  tdsmac_ul_cctrch_e_type                     tdstrch_type;
  uint16                                   ntrch;
  tdstrch_id_to_idx_map_type                  *id_to_idx_ptr;
  uint8                                    nevents=0;
  /*-----------------------------------------------------------------------------------
  Get the traffic config pointer and the intermediate measurement results corresponding
  to this measurement identity.
  ------------------------------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
    traffic_param_ptr = traffic_cfg_idx_ptr->traffic_cfg_ptr;
  
    /* If the event transport channel list is included */
    if (traffic_param_ptr->event_trch_incl)
    {
      ntrch = traffic_param_ptr->event_num_trch;
    }
    else
    {
      /*--------------------------------------------------------------------------------
      If the event transport channel list is not included then apply the criteria to
      the transport channles listed in the measurement object list.
      --------------------------------------------------------------------------------*/
      ntrch = traffic_param_ptr->num_trch;
    }
  
    /* go through the list of transport channels */
    for (i=0 ; i < ntrch && i < TDSUE_MAX_TRCH ; i++)
    {
      /* Initialise */
      tdstrch_bo=0;
      trch_bo_crossed_threshold=FALSE;
      prev_bo_crossed_threshold=FALSE;
  
      /* if the transport channel list was specified by RRC */
      if (traffic_param_ptr->event_trch_incl)
      {
        /*-------------------------------------------------------------------------------
        TRUE implies the event criteria was specified for each transport channel
        in the list.
        -------------------------------------------------------------------------------*/
        cur_evt_cfg_ptr = event_cfg_ptr = traffic_param_ptr->trch_event_param[i];
  
        /* Get the transport channel id and type*/
        tdstrch_id = traffic_param_ptr->event_trch_info[i].trch_id;
        tdstrch_type = traffic_param_ptr->event_trch_info[i].trch_type;
        nevents = (uint8)traffic_param_ptr->num_events[i];
      }
      else
      {
        /*-------------------------------------------------------------------------------
        FALSE implies only one event criteria is specified and it is applicable to all
        transport channels specified in the object list.
        -------------------------------------------------------------------------------*/
        event_cfg_ptr = traffic_param_ptr->trch_event_param[0];
        nevents = (uint8)traffic_param_ptr->num_events[0];
  
        cur_evt_cfg_ptr = traffic_param_ptr->trch_event_param[i];
  
        /*-------------------------------------------------------------------------------
        When the set of Transport channels on which the event trigger is defined is not
        provided explicitly, the UE shall apply it on the Transport channels specified
        by the measurement object.
        -------------------------------------------------------------------------------*/
        /* Get the transport channel id */
        tdstrch_id = traffic_param_ptr->trch_info[i].trch_id;
        tdstrch_type = traffic_param_ptr->trch_info[i].trch_type;
      }

      if (!TDSTRCH_ID_TO_IDX_IS_INVALID(tdstrch_id))
      {
        id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);
#if defined (FEATURE_TDSCDMA_HSUPA)

        if (((tdstrch_type==TDSGET_UL_CCTRCH_TYPE)&&(id_to_idx_ptr->valid))
          ||((TDSEDCH == tdstrch_type)&&(TDSMAC_ULHS_ACTIVE == tdsmac_ulhs_state)))
#else
        if ((tdstrch_type==TDSGET_UL_CCTRCH_TYPE)&&(id_to_idx_ptr->valid))
#endif
        {
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
          /* Check if TVM on this TrCH has been blocked or not */
          if (blk_flg == TRUE)
          {
            ntrch_blk = tdsmac_ul_event_tvm_block_table[tdsmeas_id-TDSMAC_UL_MIN_MEAS_ID].ntrch;
            
            for (m=0; m<ntrch_blk && m<TDSUE_MAX_TRCH; m++)
            {
              if (tdsmac_ul_event_tvm_block_table[tdsmeas_id-TDSMAC_UL_MIN_MEAS_ID].blk_info[m].tdstrch_id == tdstrch_id)
              {
                break;
              }
            }            
            if (m == ntrch_blk)
            {
              continue;
            }
          }
#endif 
          /*---------------------------------------------------------------------------------
           Compute the transport channel traffic volume.
          ---------------------------------------------------------------------------------*/
          tdstrch_bo = tdscompute_trch_traffic_volume(tdstrch_id,tdstrch_type);
    
          if (tdstrch_bo > 0)
          {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"Trch Id %d,Trch BO %d",tdstrch_id,tdstrch_bo);
          }
    
          /* for the number of events corresponding to the transport channel. could be 1 or 2 */
          for (j=0;j< nevents && j < TDSMAX_NUM_EVENTS;j++)
          {
            /*------------------------------------------------------------------------------
             Initialize send_status to FALSE at the start of processing of each event.
            ------------------------------------------------------------------------------*/
            send_status = FALSE;
    
            /*------------------------------------------------------------------------------
           Initialize prohobit_status to TRUE at the start of processing of each event.
            ------------------------------------------------------------------------------*/
            prohibit_status=TRUE;
    
            /*------------------------------------------------------------------------------
             Get the event id.
            ------------------------------------------------------------------------------*/
            tdsevent_id = event_cfg_ptr[j].event_id;
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
            if ((blk_flg == TRUE) &&
                 (tdsevent_id != tdsmac_ul_event_tvm_block_table[tdsmeas_id-TDSMAC_UL_MIN_MEAS_ID].blk_info[m].tdsevent_id))
            {
              continue;
            }
#endif
    
            /*------------------------------------------------------------------------------
            If the trigger prohibit timer is active then check if prohibit_tmr_cnt has
            expired.
            ------------------------------------------------------------------------------*/
            if (event_cfg_ptr[j].prohibit_tmr_cnt
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
                 && (blk_flg == FALSE)
#endif
              )
            {
              /* Check if the timer has expired */
              if ((cur_evt_cfg_ptr[j].prohibit_tmr_cnt>0) &&((++cur_evt_cfg_ptr[j].prohibit_tmr_cnt*10)>
                                                             event_cfg_ptr[j].trigger_prohibit_tmr))
              {
                /*-------------------------------------------------------------------------
                If the prohibit timer has expired then we can send a report if other
                conditions relating to buffer occupancy and trigger timer are met.
                --------------------------------------------------------------------------*/
                cur_evt_cfg_ptr[j].prohibit_tmr_cnt=0;
                TDSMAC_MSG2(MSG_LEGACY_HIGH,"PendingTmr exp'd evtid %d trid %d",tdsevent_id,(uint32)tdstrch_id);
                send_status=TRUE;
                prohibit_status = FALSE;
              }
            }
            else
            {
              send_status=TRUE;
            }
    
            /*-----------------------------------------------------------------------------
            If the prohibit timer has expired then we can send a report if other
            conditions related to buffer occupancy and time to trigger are met.
            -----------------------------------------------------------------------------*/
            if (send_status
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
                 && (blk_flg == FALSE)
#endif
              )
            {
              /*----------------------------------------------------------------------------
              If the tdsevent_id is 4A and if the transport channel buffer occupancy is
              greater than the threshold. Likewise if the tdsevent_id is 4B and if
              the buffer occupancy is lesser than the threshold then
              trch_bo_crossed_threshold is set to TRUE;
              ----------------------------------------------------------------------------*/
              trch_bo_crossed_threshold = (((tdsevent_id==TDSEVENT_4A)&&
                                            (tdstrch_bo > event_cfg_ptr[j].report_threshold)) ||((tdsevent_id==TDSEVENT_4B)&&
                                                                                              (tdstrch_bo<event_cfg_ptr[j].report_threshold)));
    
              /*----------------------------------------------------------------------------
              If the tdsevent_id is 4A and if the previous transport channel buffer occupancy is
              lesser than the threshold. Likewise if the tdsevent_id is 4B and if the previous
              buffer occupancy is greater  than the threshold then
              prev_bo_crossed_threshold is set to TRUE; An event is triggered only if both
              trch_bo_crossed_threshold and prev_bo_crossed_threshold is set to true.
              ----------------------------------------------------------------------------*/
              prev_bo_crossed_threshold = (((tdsevent_id==TDSEVENT_4A) &&
                                            (cur_evt_cfg_ptr[j].prev_bo <= event_cfg_ptr[j].report_threshold)) ||
                                           ((tdsevent_id==TDSEVENT_4B)&&( cur_evt_cfg_ptr[j].prev_bo >= event_cfg_ptr[j].report_threshold )));
    
              /* transport channel buffer occupancy has crossed threshold value */
              if (trch_bo_crossed_threshold)
              {
                /*---------------------------------------------------------------------------
                If the pending time after trigger timer expires and if the current transport
                channel traffic volume is greater/event_4A or lesser/event_4B threshold then
                it is time to send a report. or if the previous buffer occupancy has crossed
                threshold value then send_status is set to true depending on the value of the
                time to trigger.
                ----------------------------------------------------------------------------*/
                if (prev_bo_crossed_threshold)
                {
                  /*-------------------------------------------------------------------------
                  If the time_to_trigger value is sepcified and > 0 then start the trigger
                  timer count.
                  -------------------------------------------------------------------------*/
                  if (event_cfg_ptr[j].time_to_trigger > 0)
                  {
                    /* Increment the trigger_tmr_cnt */
                    ++cur_evt_cfg_ptr[j].trigger_tmr_cnt;
                    TDSMAC_MSG2(MSG_LEGACY_HIGH,"TimeToTrigger started evtid %d trid %d",tdsevent_id,(uint32)tdstrch_id);
                    TDSMAC_MSG3(MSG_LEGACY_HIGH,"Prev_bo = %d, cur_bo=%d, Trch_id = %d",
                             cur_evt_cfg_ptr[j].prev_bo,tdstrch_bo,tdstrch_id);
                    send_status=FALSE;
                  }
                  /* if the time_to_trigger is absent*/
                  else if (event_cfg_ptr[j].time_to_trigger==0)
                  {
                    /* All the conditions are met. We can send a report now*/
                    send_status=TRUE;
                    TDSMAC_MSG3(MSG_LEGACY_HIGH,"Prev_bo = %d, cur_bo=%d, tdstrch_id = 5d",cur_evt_cfg_ptr[j].prev_bo,tdstrch_bo,tdstrch_id);
                  }
                }
    
                /*---------------------------------------------------------------------------
                If the current transport channel buffer occupancy has crossed threshold value
                then check if the time_to_trigger value is > 0. If so then check if the
                trigger_timer_count has expired. If the timer has expired then it is time to
                send a report to RRC.
                ----------------------------------------------------------------------------*/
                else if ((cur_evt_cfg_ptr[j].trigger_tmr_cnt > 0) &&
                         ((++cur_evt_cfg_ptr[j].trigger_tmr_cnt * 10)>event_cfg_ptr[j].time_to_trigger))
                {
                  /* it is time to send a report to RRC */
                  send_status=TRUE;
    
                  TDSMAC_MSG2(MSG_LEGACY_HIGH,"TimeToTrigger exp'd evtid %d trid %d",tdsevent_id,(uint32)tdstrch_id);
    
                  /*Initialise the trigger timer count */
                  cur_evt_cfg_ptr[j].trigger_tmr_cnt=0;
                }
                /*--------------------------------------------------------------------------
                If the pending time after trigger just expired and if the buffer occupancy
                is greater than the threshold, then trigger a report. Likewise if the
                buffer occupancy is above threshold immediately after we configure then
                trigger a report.
                ----------------------------------------------------------------------------*/
                else if (!prohibit_status || !cur_evt_cfg_ptr[j].evt_not_first_meas)
                {
                  if (event_cfg_ptr[j].time_to_trigger)
                  {
                    cur_evt_cfg_ptr[j].trigger_tmr_cnt++;
                    send_status=FALSE;
                    TDSMAC_MSG3(MSG_LEGACY_HIGH,"Evt_First_Meas/PendingTmrExpd: TTT started, %d, %d, %d",
                    event_cfg_ptr[j].time_to_trigger,cur_evt_cfg_ptr[j].trigger_tmr_cnt,tdstrch_id);
                  }
                  else
                  {
                    TDSMAC_MSG2(MSG_LEGACY_HIGH,"Evt_First_Meas : tdstrch_bo %d,tdstrch_id %d",tdstrch_bo,tdstrch_id);
                    send_status=TRUE;
                  }
                  /* Set the evt_first_meas boolean to FALSE */
                  //evt_first_meas = FALSE;
                  cur_evt_cfg_ptr[j].evt_not_first_meas = TRUE;
                }
                else
                {
                  send_status=FALSE;
                }
    
              } /* trch_bo_crossed_threshold*/
    
              /*-----------------------------------------------------------------------------
              If the transport channel buffer occupancy has not crossed threshold then
              check if the time to trigger count is running. If so then initialize to zero.
              ------------------------------------------------------------------------------*/
              else if (cur_evt_cfg_ptr[j].trigger_tmr_cnt > 0)
              {
                /* Initialize count to 0 */
                cur_evt_cfg_ptr[j].trigger_tmr_cnt=0;
                TDSMAC_MSG2(MSG_LEGACY_HIGH,"TimeToTrigger stopped evtid %d trchid %d",tdsevent_id,(uint32)tdstrch_id);
    
                /* Don't send */
                send_status = FALSE;
              }
              else
              {
                send_status=FALSE;
              }
    
              /*----------------------------------------------------------------------------------
              Store the current buffer occupancy in previous_bo.
              ----------------------------------------------------------------------------------*/
              cur_evt_cfg_ptr[j].prev_bo = tdstrch_bo;
    
            } /*send_status*/
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
            else if(blk_flg == TRUE)
            {
              /*----------------------------------------------------------------------------
              If the tdsevent_id is 4A and if the transport channel buffer occupancy is
              greater than the threshold. Likewise if the tdsevent_id is 4B and if
              the buffer occupancy is lesser than the threshold then send_status is set to TRUE;
              ----------------------------------------------------------------------------*/
              send_status = (((tdsevent_id==TDSEVENT_4A)&&(tdstrch_bo > event_cfg_ptr[j].report_threshold)) ||
                             ((tdsevent_id==TDSEVENT_4B)&&(tdstrch_bo<event_cfg_ptr[j].report_threshold)));
            }
#endif
    
            /*------------------------------------------------------------------------------
            If the interrupt timer is running check to see if it has expired. We can resume
            transmission once the interrupt timer has expired.
            -------------------------------------------------------------------------------*/
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
            if ((tdstrch_type == TDSRACH)&& (blk_flg == FALSE))
#else
            if (tdstrch_type == TDSRACH)     
#endif
            
            {
              /*--------------------------------------------------------------------------------
              If the transmission is blocked then check to see if the Tx interrupt timer has
              expired. Resume transmissions when the timer has expired.
              ---------------------------------------------------------------------------------*/
              if (tdsblock_transmission)
              {
                  /*-------------------------------------------------------------------------
                  If another event 4a was triggered at the time of expiry of the tx interrupt
                  after trigger then MAC is expecting another block rach req from RRC.
                  --------------------------------------------------------------------------*/
                if(send_status && (tdsevent_id == TDSEVENT_4A))
                {
                    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Event 4A reported,expecting RRC to send block rach req");
                  /* Store this timer value which would be used to resume transmissions */
                  /* when MAC receives a BLOCK RACH Tx command from RRC */
    
                  // The timer was started because of an earlier event and has a value which
                  // is greater than the new tx interrupt timer value then reset the counters
                  // otherwise let the earlier timer continue to completion
                  if ((tdstx_interrupt_tmr-(tdstx_interrupt_cnt*10)) > event_cfg_ptr[j].tx_interrupt_tmr)
                  {
                    tdstx_interrupt_tmr = event_cfg_ptr[j].tx_interrupt_tmr;
                    tdstx_interrupt_cnt = 0;
                    TDSMAC_MSG3(MSG_LEGACY_HIGH,"Resetting the timer tx_intp_tmr %d, new_tx_tmr %d, tx_cnt %d",
                    tdstx_interrupt_tmr,event_cfg_ptr[j].tx_interrupt_tmr,tdstx_interrupt_cnt);
                  }
                  else
                  {
                    // Let the ealier timer continue to run.
                    TDSMAC_MSG3(MSG_LEGACY_HIGH,"Continuing the tx interrupt timer tx_intp_tmr %d, new_tx_tmr %d, tx_cnt %d",
                    tdstx_interrupt_tmr,event_cfg_ptr[j].tx_interrupt_tmr,tdstx_interrupt_cnt);
                  }
                }
              }
              // If the transmission was not blocked earlier then
              // wait for RRC to send the block command
              else if (send_status && (tdsevent_id == TDSEVENT_4A))
              {
                tdstx_interrupt_tmr = event_cfg_ptr[j].tx_interrupt_tmr;
                tdstx_interrupt_cnt = 0;
                TDSMAC_MSG1(MSG_LEGACY_HIGH,"e4a reported,expecting RRC to send block rach req %d",tdstx_interrupt_tmr);
              }
            }
            else
            {
              tdsblock_transmission = FALSE;
              tdstx_interrupt_tmr = tdstx_interrupt_cnt = 0;
            }
    
            /*----------------------------------------------------------------------------------
            If the event is triggered then store the value of the event ID which
            caused the trigger. Store the uplink channel type which caused the
            event trigger.
            ----------------------------------------------------------------------------------*/
            if (send_status)
            {
              traffic_param_ptr->current_idx = tdscur_index;
    
              traffic_param_ptr->event_result.event_id = event_cfg_ptr[j].event_id;
    
              /* Store the transport channel id which caused the trigger */
              traffic_param_ptr->event_result.trch_info.trch_id = tdstrch_id;
    
              /* Store the transport channel type */
              traffic_param_ptr->event_result.trch_info.trch_type = tdstrch_type;

              /*+yongh add optimization feature NV for L2+*/
              if (TDSEVENT_4B == event_cfg_ptr[j].event_id)
              {
                if (event_cfg_ptr[j].trigger_prohibit_tmr)
                {
                  tdsmac_e4b_event_cnt++;
                }
                else
                {
                  tdsmac_e4b_event_cnt = tdsmac_nv_e4b_event_cnt;
                }
                
                if (tdsmac_e4b_event_cnt >= tdsmac_nv_e4b_event_cnt)
                {
#ifdef FEATURE_TDSCDMA_E4B_OPT
                  if(tdsl2_opt_bitmask & TDSL2_OPT_FEATURE_E4B_IGNORE)
                  {
                    tdsmac_e4b_event_cnt = 0;  
										
                    if((tdstrch_type == TDSDCH)
                        && (traffic_param_ptr->trch_report_stauts[i].e4b_configured == TRUE))
                    {
                      traffic_param_ptr->trch_report_stauts[i].e4b_triggered = TRUE;
                    }
										
                    if(tdscheck_trch_bo_and_e4btrigger(tdsmeas_id))
                    {
                      /* send the measurement report immediately */
                      tdsprepare_n_send_traffic_meas_report(tdsmeas_id);

                      for (k = 0; k < TDSUE_MAX_TRCH; k++)
                      {
                        traffic_param_ptr->trch_report_stauts[k].e4b_triggered = FALSE;
                      }
                    }
                  }
                  else
#endif
                  {
                    tdsprepare_n_send_traffic_meas_report(tdsmeas_id);
                    tdsmac_e4b_event_cnt = 0;    
                  }
                }
                else
                {
                   TDSMAC_MSG2(MSG_LEGACY_HIGH,"e4b reported is ignored %d times for total %d",tdsmac_e4b_event_cnt,tdsmac_nv_e4b_event_cnt);               
                }
              }
              else
              {
#ifdef FEATURE_TDSCDMA_E4A_OPT
                if (tdsl2_opt_bitmask & TDSL2_OPT_FEATURE_E4A_IGNORE)
                {
                  if (FALSE == tdsuldpchctrl_get_e4a_barred())
                  {
                    /* send the measurement report immediately */
                    tdsprepare_n_send_traffic_meas_report(tdsmeas_id);
                  }
                  else
                  {
                    TDSMAC_MSG0(MSG_LEGACY_HIGH,"e4a report is ignored due to reach MAX power");
                  } 
                }
                else
#endif
                {
                  /* send the measurement report immediately */
                  tdsprepare_n_send_traffic_meas_report(tdsmeas_id);
                }
              }
              /*-yongh add optimization feature NV for L2-*/
    
              /*--------------------------------------------------------------------------------
                If the trigger_prohibit_timer is specified then start the prohibit_timer_count
                since we are sending a report to RRC now. Trigger prohibit timer will prohibit
                the sending of any further reports  to RRC until the timer has expired.
              ---------------------------------------------------------------------------------*/
              if (event_cfg_ptr[j].trigger_prohibit_tmr)
              {
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
                if (blk_flg == TRUE)
                {
                  cur_evt_cfg_ptr[j].prohibit_tmr_cnt = 0;
                }
#endif
                /* Start the prohibit timer count */
                ++cur_evt_cfg_ptr[j].prohibit_tmr_cnt;
    
                /* Clear off the previous buffer occupancy */
                cur_evt_cfg_ptr[j].prev_bo=0;
    
                TDSMAC_MSG2(MSG_LEGACY_HIGH,"Pending timer started for event %d trchid=%d",tdsevent_id,(uint32)tdstrch_id);
              }
    
    
              // Set the first_meas to false.
              //evt_first_meas=FALSE;
              cur_evt_cfg_ptr[j].evt_not_first_meas = TRUE;
    
            }/* send_status*/
    
          }/*for event list */
        }
      
      }
  
    }/* for number of transport channels */
  }
  
} /* tdsproc_event_trigger_criteria */

/*=======================================================================================
 FUNCTION NAME tdsproc_periodic_mode_criteria

 PURPOSE  This function checks if it is time to send a report based on comparing
          periodic timer count and the report interval. Also it checks if there
          are any limits on the number of reports that MAC can send to RRC.

========================================================================================*/
void tdsproc_periodic_mode_criteria
(
/* Measurement identity */
uint16                          tdsmeas_id
)
{

  tdsmaci_ul_traffic_cfg_param       *traffic_param_ptr;
  tdsmaci_ul_traffic_cfg_index_type  *traffic_cfg_idx_ptr;

  /*-------------------------------------------------------------------------
  Get the traffic config pointer and the intermediate measurement results
  corresponding to this measurement identity.
  -------------------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    traffic_cfg_idx_ptr =TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
    traffic_param_ptr   =traffic_cfg_idx_ptr->traffic_cfg_ptr;
  
    /*-----------------------------------------------------------------------
    Check if this is the time to report. This is done with counters
    (periodic_tmr_cnt). If this counter is not equal to the report_interval
    then wait for the next 10ms, else it is time to send the report to RRC.
    -----------------------------------------------------------------------*/
    TDSL2_MSG3(MSG_LEGACY_HIGH," Timer_count %d, report_interval %d, meas_id %d",
             (traffic_param_ptr->periodic_tmr_cnt *TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE), 
              traffic_param_ptr->report_interval, tdsmeas_id);
    
    if ((++traffic_param_ptr->periodic_tmr_cnt * 
         (10 * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE))>=traffic_param_ptr->report_interval)
    {
      /* Intialize the timer count to 0 */
      traffic_param_ptr->periodic_tmr_cnt = 0;
  
      /*------------------------------------------------------------------------------
      If there is no limit on the number of reports that we need to send to rrc or if
      there is a limit& we have not exceeded it, then it is time to send a report to
      RRC. nreports_sent is incremented just at the time of sending a report. Its
      done after checking if there is atleast one radio bearer for which the results
      have to be reported.
      ------------------------------------------------------------------------------*/
      if ((traffic_param_ptr->report_infinity) ||
          (traffic_param_ptr->nreports_sent < traffic_param_ptr->num_reports))
      {
        traffic_param_ptr->current_idx = tdscur_index;
  
        /*---------------------------------------------------------------------------
        Time to send a report to RRC for this measurement identity. Prepare to
        send the report.
        ----------------------------------------------------------------------------*/
        tdsprepare_n_send_traffic_meas_report(tdsmeas_id);
  
      }/* if report_infinity  */
      else
      {
        /* Delete this measurement identity since we have sent the maximum number of*/
        /* reports that needs to be sent*/
        (void)tdsmac_ul_delete_traffic_volume_meas(tdsmeas_id);
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"Sent %d reports. Deleting %d",traffic_param_ptr->num_reports,tdsmeas_id);
      }
    }/*if time to send report */
  }
}/* tdsproc_periodic_mode_criteria*/

/*===============================================================================
 FUNCTION NAME  tdsmac_ul_handle_tvm_event_tmr

 PURPOSE    This function is called when MAC traffic volume timer signal
            expires every 10ms. This is used as a base timer and all the other
            timers like trigger_timer..etc are done using counters.
            This function checks for each event mode measurement identity , if the
            timer counters have expired and posts the measurement report to RRC.
================================================================================*/


void tdsmac_ul_handle_tvm_event_tmr (void)
{
  uint16                            tdsmeas_id;
  tdsmaci_ul_traffic_cfg_index_type    *traffic_cfg_idx_ptr;
  tdsmac_ul_traffic_report_mode_e_type tdsreport_mode;

  /*---------------------------------------------------------------------------------
  If the current uplink transport channel type is invalid then its an error since
  there are no transport channels configured in MAC so far. Hence can't make any
  measurements.
  ---------------------------------------------------------------------------------*/
  if (TDSGET_UL_CCTRCH_TYPE==TDSINVALID_UL_CCTRCH)
  {
    /* Set the 10ms timer tick again */
    (void)rex_set_timer(&tdsmac_ul_tvm_event_tmr_blk,(uint32)TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE);
    TDSMAC_MSG0(MSG_LEGACY_LOW,"MAC UL transport channels not configured yet");
  }
  else if (tdsnum_event_measurements==0)
  {
    TDSMAC_MSG0(MSG_LEGACY_LOW,"No measurements to make");
  }
  else
  {
    /*------------------------------------------------------------------------------
    If the interrupt timer is running check to see if it has expired. We can resume
    transmission once the interrupt timer has expired.
    -------------------------------------------------------------------------------*/
    if (TDSGET_UL_CCTRCH_TYPE == TDSRACH)
    {
      /*--------------------------------------------------------------------------------
      If the transmission is blocked then check to see if the Tx interrupt timer has
      expired. Resume transmissions when the timer has expired.
      ---------------------------------------------------------------------------------*/
      if (tdsblock_transmission)
      {
        // Increment the timer
        if ((tdstx_interrupt_tmr > 0 )&& ((++tdstx_interrupt_cnt*10) >= tdstx_interrupt_tmr))
        {
          tdsblock_transmission = FALSE;
          tdstx_interrupt_tmr = tdstx_interrupt_cnt = 0;
          TDSMAC_MSG0(MSG_LEGACY_HIGH,"TxInterruptTmr expired");
        }
      }
    }

    /* Set the 10ms timer tick again */
    (void)rex_set_timer(&tdsmac_ul_tvm_event_tmr_blk,(uint32)TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE);

    /*-----------------------------------------------------------------------------
    Compute the raw payload and store the information in the array for each
    radio bearer.
    -----------------------------------------------------------------------------*/
    tdscompute_obj_list_raw_payload();

    for (tdsmeas_id = TDSMAC_UL_MIN_MEAS_ID; tdsmeas_id <= TDSMAC_UL_MAX_MEAS_ID; tdsmeas_id++)
    {
      /*--------------------------------------------------------------------------
        Get the config, pointer corresponding to this measurement
        identity.
      --------------------------------------------------------------------------*/
      traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);

      /*--------------------------------------------------------------------------
      If it contains valid parameters and the pointer is not NULL then start
      measurements for this measurement identity.Check to see if we need to
      temporarily suspend measurements for this identity. suspend_meas=TRUE indicates
      that we need to save this configuration information and resume measurements
      only after the resume command is recvd for this identity.
      --------------------------------------------------------------------------*/
      if ((traffic_cfg_idx_ptr->valid)&&(!traffic_cfg_idx_ptr->suspend_meas)&&
          (traffic_cfg_idx_ptr->traffic_cfg_ptr!=NULL))
      {
        tdsreport_mode = traffic_cfg_idx_ptr->traffic_cfg_ptr->report_mode;

        /*-----------------------------------------------------------------------
          Based on the mode, check to see if it is time to send a report.
        -----------------------------------------------------------------------*/
        if (tdsreport_mode==TDSEVENT_TRIGGERED_MODE)
        {
          tdsproc_event_trigger_criteria(tdsmeas_id
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
                       , FALSE
#endif
                        );
        }

      }/* if traffic_cfg_ptr is valid */

    }/* for */
  } /* num_measurements>0) */
}

/*===============================================================================
 FUNCTION NAME  tdsmac_ul_handle_tvm_periodic_tmr

 PURPOSE    This function is called when MAC traffic volume timer signal
            expires every (10ms* TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE). This is used as a 
            base timer and report_interval is computed using counters.
            This function checks for each periodic mode measurement identity if the 
            timer counters have expired posts the measurement report to RRC.
================================================================================*/
void tdsmac_ul_handle_tvm_periodic_tmr (void)
{
  uint16                            meas_id;
  tdsmaci_ul_traffic_cfg_index_type    *traffic_cfg_idx_ptr;
  tdsmac_ul_traffic_report_mode_e_type report_mode;

  /*---------------------------------------------------------------------------------
  If the current uplink transport channel type is invalid then its an error since
  there are no transport channels configured in MAC so far. Hence can't make any
  measurements.
  ---------------------------------------------------------------------------------*/
  if (TDSGET_UL_CCTRCH_TYPE==TDSINVALID_UL_CCTRCH)
  {
    /* Set the (10ms* TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE) timer tick again */
    (void)rex_set_timer(&tdsmac_ul_tvm_periodic_tmr_blk,
                (uint32)(TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE));
    TDSL2_MSG0(MSG_LEGACY_HIGH,"TVM_PERIODIC: MAC UL transport channels not configured yet");
  }
  else if (tdsnum_periodic_measurements == 0)
  {
    TDSL2_MSG0(MSG_LEGACY_HIGH,"TVM_PERIODIC:No periodic measurements to make");
  }
  else
  {

    /* Set the (10ms* TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE) timer tick again */
    (void)rex_set_timer(&tdsmac_ul_tvm_periodic_tmr_blk,
                (uint32)(TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE));

    /*-----------------------------------------------------------------------------
        Compute the raw payload and store the information in the array for each RB
       -----------------------------------------------------------------------------*/
    tdscompute_obj_list_raw_payload(); 
                        //    IS IT really needed ??
        

    for (meas_id = TDSMAC_UL_MIN_MEAS_ID; meas_id <= TDSMAC_UL_MAX_MEAS_ID; meas_id++)
    {
//        Get the config, pointer corresponding to this meas id
      traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(meas_id);

//      If it contains valid parameters and if its not temporarily suspend meas id
      if ((traffic_cfg_idx_ptr->valid)&&(!traffic_cfg_idx_ptr->suspend_meas)&&
          (traffic_cfg_idx_ptr->traffic_cfg_ptr!=NULL))
      {
        report_mode = traffic_cfg_idx_ptr->traffic_cfg_ptr->report_mode;
        if (report_mode==TDSPERIODIC_MODE)
        {
          tdsproc_periodic_mode_criteria(meas_id);
        }
      }/* if traffic_cfg_ptr is valid */
    }/* for */
  } /* num_measurements>0) */
}


/*======================================================================================
 FUNCTION NAME  tdsmac_ul_delete_traffic_volume_meas

 PURPOSE    This function deletes the traffic volume measurement associated with the
            measurement identity specified.
            This function returns a boolean. true indicates that the delete operation
            was sucessful.
======================================================================================*/

boolean tdsmac_ul_delete_traffic_volume_meas
(
/* Delete the traffic volume measurements associated with this measurement identity */
uint16 tdsmeas_id
)
{
  boolean delete_status_ind=TRUE;
  uint16  i;
  tdsmaci_ul_traffic_cfg_index_type  *traffic_cfg_idx_ptr;

  /*----------------------------------------------------------------*/
  /* Measurement identity not valid                                 */
  /*----------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    delete_status_ind=FALSE;
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
    /*-----------------------------------------------------------------
      If the valid flag is set to FALSE, then its an error since we
      are asked to delete a measurement ID hic doesnt exist.
      If the traffic_cfg_ptr is NULL, then its an error since we
      are asked to delete a measurement ID hic doesnt exist.
    ------------------------------------------------------------------*/
    if ((!traffic_cfg_idx_ptr->valid)||
        (traffic_cfg_idx_ptr->traffic_cfg_ptr==NULL))
    {
      delete_status_ind=FALSE;

      if (traffic_cfg_idx_ptr->traffic_cfg_ptr!=NULL)
      {
        /* Release memory */
        modem_mem_free(traffic_cfg_idx_ptr->traffic_cfg_ptr, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
      }

      traffic_cfg_idx_ptr->traffic_cfg_ptr=NULL;
      TDSMAC_MSG2(MSG_LEGACY_ERROR,"Measurment ID doesn't exist %d %d",!traffic_cfg_idx_ptr->valid,traffic_cfg_idx_ptr->traffic_cfg_ptr);
    }
    else
    {


      /*-----------------------------------------------------------------------------------
      Update the number of pending measurements.
      -----------------------------------------------------------------------------------*/
      if (traffic_cfg_idx_ptr->traffic_cfg_ptr->report_mode == TDSEVENT_TRIGGERED_MODE) 
      {
        tdsnum_event_measurements--;
        
        if (tdsnum_event_measurements == 0)
      {
        /*--------------------------------------------------------------------------------
        Stop the timer as there are no traffic volume measurements to make.
        ---------------------------------------------------------------------------------*/
          (void)rex_clr_timer(&tdsmac_ul_tvm_event_tmr_blk);
          tdsmac_tvm_event_timer_started = FALSE;
          TDSL2_MSG0(MSG_LEGACY_HIGH,"Cleared event Traffic Volume Timer");
          /* Reset all the block_transmission flag */
          tdsblock_transmission = FALSE;
        }
      }
      else if (traffic_cfg_idx_ptr->traffic_cfg_ptr->report_mode == TDSPERIODIC_MODE) 
      {
        tdsnum_periodic_measurements--;

        if (tdsnum_periodic_measurements == 0)
        {
          /*--------------------------------------------------------------------------------
                      Stop the timer as there are no traffic volume measurements to make.
                    ---------------------------------------------------------------------------------*/
         (void)rex_clr_timer(&tdsmac_ul_tvm_periodic_tmr_blk);
         tdsmac_tvm_periodic_timer_started = FALSE;
         TDSL2_MSG0(MSG_LEGACY_HIGH,"Cleared periodic Traffic Volume Timer");
        }
      }

      TDSL2_MSG3(MSG_LEGACY_HIGH,"Deleted meas_id %d, num_event_meas %d, num_periodic %d",
               tdsmeas_id, tdsnum_event_measurements,tdsnum_periodic_measurements);

      /*------------------------------------------------------------
        Release the memory.
      -------------------------------------------------------------*/
      modem_mem_free(traffic_cfg_idx_ptr->traffic_cfg_ptr, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
      traffic_cfg_idx_ptr->valid=FALSE;
      traffic_cfg_idx_ptr->traffic_cfg_ptr = NULL;

      if ((tdsnum_periodic_measurements+tdsnum_event_measurements) == 0)
      {
        /*---------------------------------------------------------------------------------
        Initalize the traffic_ul_buf_status structure. Every 10ms the RLC buffer status is
        obtained in traffic_ul_buf_status.
        ---------------------------------------------------------------------------------*/
        memset(&tdstraffic_ul_buf_status,0, sizeof(tdsrlc_ul_buf_status_type));
  
        /* evt_first_meas is set to true whenever MAC is configured for measurements
        newly */
        //evt_first_meas = TRUE;
        tdscur_index=0;
        tdsrolled_over=FALSE;
  
        for (i=0;i<TDS_MAX_RB;i++)
        {
          memset(tdsrb_raw_payload[i],0,TDSTRAFFIC_MAX_SAMPLES*sizeof(uint32));
        }
      }

    }/* measurement identity is valid */
  }
  return delete_status_ind;
}

/*========================================================================================
 FUNCTION NAME  tdsmac_ul_delete_all_traffic_volume_meas

 PURPOSE    This function deletes all the traffic volume measurements that have been done
            so far. This function returns a boolean. True indicates that the delete_all
            function was sucessful.
========================================================================================*/

boolean tdsmac_ul_delete_all_traffic_volume_meas (void)
{
  uint16 i;
  boolean delete_status_ind=TRUE ;

  /* Reset this flag since all measurements are cleared up */
  tdsblock_transmission = FALSE;

  if ((tdsnum_event_measurements == 0) && (tdsnum_periodic_measurements == 0))
  {
    delete_status_ind = FALSE;
    TDSMAC_MSG0(MSG_LEGACY_MED,"There are no measurements to delete");
  }
  else
  {
    /*-----------------------------------------------------------------------------
     Stop the timer as there are no traffic volume measurements to make.
    -----------------------------------------------------------------------------*/
    (void)rex_clr_timer(&tdsmac_ul_tvm_event_tmr_blk);
    tdsmac_tvm_event_timer_started = FALSE;

    (void)rex_clr_timer(&tdsmac_ul_tvm_periodic_tmr_blk);
    tdsmac_tvm_periodic_timer_started= FALSE;

    TDSL2_MSG0(MSG_LEGACY_HIGH,"Cleared Traffic Volume Timers");

    tdsrolled_over=FALSE;

    /* evt_first_meas is set to true whenever MAC is configured for measurements
    newly */
    //evt_first_meas = TRUE;
  }
  /*-----------------------------------------------------------------------------
    Initialise the UL traffic measurement table.
  -----------------------------------------------------------------------------*/
  memset(&tdstraffic_ul_buf_status,0, sizeof(tdsrlc_ul_buf_status_type));

  /*-----------------------------------------------------------------------------
    Initialise the UL traffic volume config table
  -----------------------------------------------------------------------------*/
  for (i=0;i<TDSUE_NUM_TRAFFIC_MEAS;i++)
  {
    if (tdsul_traffic_info_table[i].traffic_cfg_ptr!=NULL)
    {
      modem_mem_free(tdsul_traffic_info_table[i].traffic_cfg_ptr, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
      tdsul_traffic_info_table[i].traffic_cfg_ptr=NULL;
    }
    tdsul_traffic_info_table[i].valid=FALSE;
  }

  for (i=0;i<TDS_MAX_RB;i++)
  {
    memset(tdsrb_raw_payload[i],0,TDSTRAFFIC_MAX_SAMPLES*sizeof(uint32));
  }

  tdscur_index=0;
  tdstx_interrupt_tmr=0;
  tdstx_interrupt_cnt=0;
  /*-----------------------------------------------------------------------------
  Update the number of traffic volume measurements,number of pending measurements
  -----------------------------------------------------------------------------*/
  tdsnum_event_measurements = 0;
  tdsnum_periodic_measurements = 0;

  return delete_status_ind;

}
/*========================================================================================
 FUNCTION NAME  tdscheck_modify_params

 PURPOSE  This function checks the config information sent by RRC in the case of
          MODIFY cmd and sees if the parameters are valid. Returns TRUE if all the
          parameters which are modified are valid.
========================================================================================*/
boolean tdscheck_modify_params
(
/* Pointer to config information sent by RRC */
tdsmac_ul_traffic_meas_config_type *oc_traffic_cfg_ptr,

/* POinter to config information internally maintained by MAC */
tdsmaci_ul_traffic_cfg_param       *int_tmp_cfg_ptr
)
{
  boolean status_ind = TRUE;
  tdsmac_ul_traffic_report_mode_e_type tdsreport_mode;

  tdsreport_mode = oc_traffic_cfg_ptr->meas_report_mode.report_mode;

  if (oc_traffic_cfg_ptr->meas_report_incl)
  {
    /*---------------------------------------------------------------------
    If MAC is asked to report either the average or variance of rlc paylaod.
    ---------------------------------------------------------------------*/
    if ((oc_traffic_cfg_ptr->meas_report_qty.avg_payload) ||
        (oc_traffic_cfg_ptr->meas_report_qty.var_payload))
    {
      /*------------------------------------------------------------------
      if the measurement quantity is included and if the interval is 0
      then its an error.
      ------------------------------------------------------------------*/
      if ((oc_traffic_cfg_ptr->meas_qty_incl)&&
          (oc_traffic_cfg_ptr->meas_qty.meas_interval/10==0))
      {
        status_ind=FALSE;
        TDSMAC_MSG1(MSG_LEGACY_ERROR,"Invalid measurement interval %d",oc_traffic_cfg_ptr->meas_qty.meas_interval);
      }
      /*------------------------------------------------------------------
      if the measurement quantity is not included and if the previous
      config, the measurement interval is zero then its an error.
      ------------------------------------------------------------------*/
      else if ((!oc_traffic_cfg_ptr->meas_qty_incl)
               &&(int_tmp_cfg_ptr->nsamples==0))
      {
        status_ind=FALSE;
        TDSMAC_MSG1(MSG_LEGACY_ERROR,"Invalid measurement interval %d",oc_traffic_cfg_ptr->meas_qty.meas_interval);
      }

    }

  }
  else if (int_tmp_cfg_ptr->avg_payload || int_tmp_cfg_ptr->var_payload)
  {
    if ((oc_traffic_cfg_ptr->meas_qty_incl) &&
        (oc_traffic_cfg_ptr->meas_qty.meas_interval/10==0))
    {
      status_ind=FALSE;
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement interval");

    }
  }

  if (status_ind && oc_traffic_cfg_ptr->meas_criteria_incl)
  {
    int_tmp_cfg_ptr->report_mode = tdsreport_mode;

    if (tdsreport_mode==TDSEVENT_TRIGGERED_MODE)
    {
      status_ind= tdscheck_n_copy_modify_event_trigger_params(int_tmp_cfg_ptr,
                  &oc_traffic_cfg_ptr->meas_report_criteria.event_cfg);
    }
    else if (tdsreport_mode == TDSPERIODIC_MODE)
    {
      status_ind=
      tdscheck_n_copy_periodic_params(int_tmp_cfg_ptr,oc_traffic_cfg_ptr);
    }
    else
    { /* No Reporting */
      int_tmp_cfg_ptr->report_mode = tdsreport_mode;
    }
  }
  return status_ind;
}

/*========================================================================================
 FUNCTION NAME  tdsmac_ul_modify_traffic_volume_meas

 PURPOSE    This function modifies the traffic volume measurement for this measurement
            identity.This function returns a boolean. True indicates that the modify
            operation was sucessful. The modify_status_ind is set to true after checking
            if the transport channels for which measurements have been requested for
            exists.
========================================================================================*/
boolean tdsmac_ul_modify_traffic_volume_meas
(
/* Pointer to traffic volume config information */
tdsmac_ul_traffic_meas_config_type *oc_traffic_cfg_ptr
)
{
  uint16                                    tdsmeas_id;
  uint8                                     i;
  tdsmaci_ul_traffic_cfg_param                 *int_tmp_cfg_ptr = NULL;
  tdsmaci_ul_traffic_cfg_index_type            *traffic_cfg_idx_ptr = NULL;
  boolean                                   modify_status_ind=TRUE;
  boolean                                   periodic_to_event_mode_change = FALSE;
  boolean                                   event_to_periodic_mode_change = FALSE;
  

  tdsmeas_id = oc_traffic_cfg_ptr->meas_id;
  /*-----------------------------------------------------------------------------------
  If the measurement identity is invalid.
  ------------------------------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    modify_status_ind=FALSE;
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Measurement ID out of range");
  }
  else if (TDSGET_UL_CCTRCH_TYPE==TDSINVALID_UL_CCTRCH)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"MAC UL transport channels not configured");
    modify_status_ind=FALSE;
  }
  else
  {
    /*--------------------------------------------------------------------------------
    Get the internal config pointer for this measurement identity.
    ---------------------------------------------------------------------------------*/
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
    int_tmp_cfg_ptr  =    traffic_cfg_idx_ptr->traffic_cfg_ptr;
  }

  if(int_tmp_cfg_ptr != NULL)
  {
    if ((int_tmp_cfg_ptr->report_mode == TDSPERIODIC_MODE) && 
        (oc_traffic_cfg_ptr->meas_report_mode.report_mode == TDSEVENT_TRIGGERED_MODE))
    {
      periodic_to_event_mode_change = TRUE;
      TDSL2_MSG1(MSG_LEGACY_HIGH," TVM_MODIFY : periodic_to_event_mode_change %d",periodic_to_event_mode_change);
    }
    else if((int_tmp_cfg_ptr->report_mode == TDSEVENT_TRIGGERED_MODE) &&
            (oc_traffic_cfg_ptr->meas_report_mode.report_mode == TDSPERIODIC_MODE))
    {
      event_to_periodic_mode_change = TRUE;
      TDSL2_MSG1(MSG_LEGACY_HIGH," TVM_MODIFY : event_to_periodic_mode_change %d",event_to_periodic_mode_change);
    }
  }

  /* if modify_status_ind=TRUE*/
  if (modify_status_ind)
  {
    /* if the config pointer is NULL or invalid */
    if (((traffic_cfg_idx_ptr != NULL) && (!traffic_cfg_idx_ptr->valid)) ||
        (int_tmp_cfg_ptr==NULL)) /*lint !e644*/
    {
      modify_status_ind=FALSE;

      if (int_tmp_cfg_ptr!=NULL)
      {
        modem_mem_free(int_tmp_cfg_ptr, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
      }

      TDSMAC_MSG0(MSG_LEGACY_ERROR,"Measurement ID does not exist");
    }
    /* Check for the validity of the modify parameters that RRC has specified */
    else if (tdscheck_modify_params(oc_traffic_cfg_ptr,int_tmp_cfg_ptr))
    {
      /*----------------------------------------------------------------------------------
       Clear any exisiting measurement results done for this measurements identity.
      ----------------------------------------------------------------------------------*/
      memset (&int_tmp_cfg_ptr->event_result,0,sizeof(tdsmac_ul_traffic_event_result));
     

      /*---------------------------------------------------------------------------------
             If There is any mode change take care of Timers and measurement count 
        ----------------------------------------------------------------------------------*/
      if (periodic_to_event_mode_change)
      {
        tdsnum_periodic_measurements--;
        tdsnum_event_measurements++;
      }
      else if (event_to_periodic_mode_change)
      {
        tdsnum_event_measurements--;
        tdsnum_periodic_measurements++;
      }
      
      if (tdsnum_event_measurements == 0)
      {
        (void)rex_clr_timer(&tdsmac_ul_tvm_event_tmr_blk);
         tdsmac_tvm_event_timer_started = FALSE;
         TDSL2_MSG1(MSG_LEGACY_HIGH,"TVM_MODIFY : Num_event_meas = %d",tdsnum_event_measurements);
      }
      else if ((tdsnum_event_measurements > 0) && (!tdsmac_tvm_event_timer_started))
      {
        (void)rex_set_timer(&tdsmac_ul_tvm_event_tmr_blk,(uint32)TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE);
        tdsmac_tvm_event_timer_started = TRUE;
        TDSL2_MSG1(MSG_LEGACY_HIGH,"TVM_MODIFY : Starting tdsmac_ul_tvm_event_tmr_blk timer %d ms",TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE);
      }

      if (tdsnum_periodic_measurements == 0)
      {
        (void)rex_clr_timer(&tdsmac_ul_tvm_periodic_tmr_blk);
        tdsmac_tvm_periodic_timer_started = FALSE;
        TDSL2_MSG1(MSG_LEGACY_HIGH,"TVM_MODIFY : tdsnum_periodic_measurements = %d",tdsnum_periodic_measurements);
      }
      else if ((tdsnum_periodic_measurements > 0) && (!tdsmac_tvm_event_timer_started))
      {
        (void)rex_set_timer(&tdsmac_ul_tvm_periodic_tmr_blk,
               (uint32)(TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE));
        tdsmac_tvm_event_timer_started = TRUE;
        TDSL2_MSG1(MSG_LEGACY_HIGH,"TVM_MODIFY : Starting tdsmac_ul_tvm_periodic_tmr_blk timer %d ms",(TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE));
      }
      
      TDSL2_MSG2(MSG_LEGACY_HIGH,"TVM_MODIFY : num_event_meas %d,num_periodic_meas %d",tdsnum_event_measurements,tdsnum_periodic_measurements);

      /*---------------------------------------------------------------------------------
        If measurement object list is included then copy the object list.
      ----------------------------------------------------------------------------------*/
      if (oc_traffic_cfg_ptr->meas_obj_incl)
      {
        int_tmp_cfg_ptr->meas_obj_list_incl = oc_traffic_cfg_ptr->meas_obj_incl;
        int_tmp_cfg_ptr->num_trch = oc_traffic_cfg_ptr->meas_obj_list.num_trch;
        /*memcpy(int_tmp_cfg_ptr->trch_info, oc_traffic_cfg_ptr->meas_obj_list.trch_info,
               TDSUE_MAX_TRCH *sizeof(tdsmac_ul_traffic_trch_info_type));*/

        memscpy(int_tmp_cfg_ptr->trch_info, 
                TDSUE_MAX_TRCH *sizeof(tdsmac_ul_traffic_trch_info_type),
                oc_traffic_cfg_ptr->meas_obj_list.trch_info,
               TDSUE_MAX_TRCH *sizeof(tdsmac_ul_traffic_trch_info_type));

        for (i=0;i<int_tmp_cfg_ptr->num_trch;i++)
        {
          if ((int_tmp_cfg_ptr->trch_info[i].trch_type==TDSRACH) &&
              (TDSGET_UL_CCTRCH_TYPE==TDSRACH))
          {
            int_tmp_cfg_ptr->trch_info[i].trch_id = tdsint_ul_trch_info[0].trch_id;
          }
        }
      }
      /*---------------------------------------------------------------------------------
        Copy the new report quantity .
      ----------------------------------------------------------------------------------*/
      if (oc_traffic_cfg_ptr->meas_report_incl)
      {
        int_tmp_cfg_ptr->avg_payload = oc_traffic_cfg_ptr->meas_report_qty.avg_payload;
        int_tmp_cfg_ptr->rlc_payload = oc_traffic_cfg_ptr->meas_report_qty.rlc_payload;
        int_tmp_cfg_ptr->var_payload = oc_traffic_cfg_ptr->meas_report_qty.var_payload;
      }

      /*---------------------------------------------------------------------------------
      If the measurement quantity is included copy the measurement interval. This is the
      interval used to calculate average or variance when the measurement report quantity
      is specified as average or vaiance.
      ---------------------------------------------------------------------------------*/
      if (oc_traffic_cfg_ptr->meas_qty_incl)
      {
        int_tmp_cfg_ptr->nsamples = (uint8)oc_traffic_cfg_ptr->meas_qty.meas_interval/10;
      }

    }/* All parameters are valid */

  } /* modify_status_ind=TRUE */

  return modify_status_ind;
}
/*=====================================================================================
 FUNCTION NAME  tdsmac_ul_block_rach_tx

 PURPOSE    This function blocks DTCH transmissions on RACH when the UE is in CELL_FACH
            state. It resumes transmissions when the IE Tx interruption after trigger
            elapses.
=====================================================================================*/
void tdsmac_ul_block_rach_tx (boolean block_rach)
{
  /*-----------------------------------------------------------------------------------
  If the cctrch type is invalid then its an error.
  ------------------------------------------------------------------------------------*/
  if (TDSGET_UL_CCTRCH_TYPE != TDSRACH)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid BLOCK command recvd");
    tdsblock_transmission = FALSE;
    tdstx_interrupt_tmr = tdstx_interrupt_cnt = 0;
  }
  else
  { /* CCTRCH_TYPE = TDSRACH */
    /* block transmission */
    tdsblock_transmission = block_rach;
    //tx_interrupt_cnt = 0; // Reset the timer count for tx interruption after trigger.

    /* The timer would be started when we get the next 10ms timer expiry signal*/
  }/* cctrch_type = TDSRACH*/
}
/*====================================================================================
 FUNCTION NAME  tdsproc_block_dtch_tx_on_rach()

 PURPOSE  If the IE is in cell_fach state then it can prohibit dtch transmissions on
        RACH. tdsblock_transmission is set to TRUE when we receive a BLOCK cmd from RRC
        The transmission is resumed when the Tx interruption timer expires
=====================================================================================*/

void tdsproc_block_dtch_tx_on_rach(tdsrlc_ul_buf_status_type *tdsul_buf_status_ptr)
{

  uint16                      i;
  tdsul_lc_info_type             *int_lc_ptr;        /* Pointer to Logical channel info */
  tdsrlc_ul_lc_buf_status_type   *lc_buf_status_ptr; /* Pointer to buffer status info */

  /* Eliminate TFCs based on logical channel buffer occupancy */
  for (i=0 ; i < TDSUE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    int_lc_ptr = TDSGET_UL_LC_INFO_PTR((rlc_lc_id_type)(i + TDSMAC_UL_MIN_RLC_ID));

    /* Get Logical Channel Buffer Status information */
    lc_buf_status_ptr = &tdsul_buf_status_ptr->chan_info[i];

    /* Logical channel is enabled */
    if (int_lc_ptr->enable == TRUE)
    {
      /* Compute Transport Channel Buffer Occupancy based on Buffer Status Type */
      switch (lc_buf_status_ptr->type)
      {

      /* Flexible PDUs type */
      case TDSFLEX_PDUS:
        if (int_lc_ptr->lc_type==UE_LOGCHAN_DTCH)
        {
          /* Set the nbytes to 0 */
          lc_buf_status_ptr->nbytes = 0;
        }
        break;

      default:
        break;
      }
    }
  }
}

/*====================================================================================
 FUNCTION NAME  tdsmac_ul_save_traffic_volume_meas()

 PURPOSE  Svae the configuration information for this measurement identity and
 temporarily suspend measurements until a resume command is received for this
 tdsmeas_id.
=====================================================================================*/
boolean tdsmac_ul_save_traffic_volume_meas(uint16 tdsmeas_id)
{
  tdsmaci_ul_traffic_cfg_index_type            *traffic_cfg_idx_ptr;
  boolean                                                                     rv=FALSE;
  uint8                                                                         i;
  /*--------------------------------------------------------------------------
 Get the config, pointer corresponding to this measurement
 identity.
  --------------------------------------------------------------------------*/

  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
  
    if (traffic_cfg_idx_ptr->valid)
    {
      traffic_cfg_idx_ptr->suspend_meas=TRUE;
      traffic_cfg_idx_ptr->traffic_cfg_ptr->first_meas = TRUE;
  
      /* Reset the counter */
      tdsmac_reset_periodic_event_trigger_counters();
  
      tdstx_interrupt_tmr = 0;
      tdstx_interrupt_cnt = 0;
      //evt_first_meas = TRUE;
  
      /*---------------------------------------------------------------------------------
      Initalize the radio bearer raw payload information.
      ---------------------------------------------------------------------------------*/
      for (i=0;i<TDS_MAX_RB;i++)
      {
        /*------------------------------------------------------------------------------
        tdsrb_raw_payload is an array of 26 elements(maximum measurement interval is 260ms)
        and is maintained for every radio bearer (1..32).
        ------------------------------------------------------------------------------*/
        memset(tdsrb_raw_payload[i],0,TDSTRAFFIC_MAX_SAMPLES*sizeof(uint32));
      }
  
      rv=TRUE;
    }
    else
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Save: Meas_Id %d not present",tdsmeas_id);
    }
  }
  
  return rv;
}

/*====================================================================================
 FUNCTION NAME  tdsmac_ul_resume_traffic_volume_meas()

 PURPOSE  Resume measurementsfor this  identity and
=====================================================================================*/

boolean tdsmac_ul_resume_traffic_volume_meas(uint16 tdsmeas_id)
{
  tdsmaci_ul_traffic_cfg_index_type            *traffic_cfg_idx_ptr;
  boolean                                   status_ind=TRUE;
  tdsmaci_ul_traffic_cfg_param                 *traffic_cfg_ptr;


  /*--------------------------------------------------------------------------
 Get the config, pointer corresponding to this measurement
 identity.
  --------------------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
  
    if (traffic_cfg_idx_ptr->valid)
    {
      traffic_cfg_idx_ptr->suspend_meas=FALSE;
  
      traffic_cfg_ptr = traffic_cfg_idx_ptr->traffic_cfg_ptr;
  
  
      if (!traffic_cfg_ptr->meas_obj_list_incl)
      {
        /*----------------------------------------------------------------------------
        Populate the transport channel id and the type information from all the
        uplink transport channels currently configured in MAC.
        ----------------------------------------------------------------------------*/
        traffic_cfg_ptr->num_trch= tdspopulate_object_list(traffic_cfg_ptr->trch_info);
      }
  
      if (traffic_cfg_ptr->report_mode == TDSEVENT_TRIGGERED_MODE)
      {
        if (!tdsmac_tvm_event_timer_started)
        {
          // Start the traffic volume timer ticking again.
          (void)rex_set_timer(&tdsmac_ul_tvm_event_tmr_blk,(uint32)TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE);
          TDSL2_MSG2(MSG_LEGACY_HIGH,"Starting event triggering TVM timer meas_id = %d,num_meas = %d",tdsmeas_id,tdsnum_event_measurements);
        }
        else
        {
          TDSL2_MSG2(MSG_LEGACY_HIGH,"TVM event triggering timer already running, meas_id = %d,num_meas = %d",tdsmeas_id,tdsnum_event_measurements);
        }
      }
      else if(traffic_cfg_ptr->report_mode == TDSPERIODIC_MODE)
      {
        if (!tdsmac_tvm_periodic_timer_started)
        {
          // Start the traffic volume timer ticking again.
            (void)rex_set_timer(&tdsmac_ul_tvm_periodic_tmr_blk,(uint32)(TDSMAC_UL_TRAFFIC_VOLUME_TMR_BASE * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE));
          TDSL2_MSG2(MSG_LEGACY_HIGH,"Starting TVM periodic timer meas_id = %d,num_meas = %d",tdsmeas_id,tdsnum_periodic_measurements);
        }
      else
      {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"TVM timer already running, tdsmeas_id = %d,num_meas = %d",tdsmeas_id,tdsnum_periodic_measurements);
      }
     }
    }// valid
    else
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Resume: Meas_Id %d not present",tdsmeas_id);
    }
  }
  
  return status_ind;

}


/*====================================================================================
 FUNCTION NAME  tdsconfig_ul_mac_traffic_volume

 PURPOSE    This function process UL MAC traffic volume Command. It sends a config
            confirmation message to RRC in the case that RRC has requested for it.
            Then it processes the commands based on the command type : setup, modify,
            delete, delete_all.
=====================================================================================*/

void tdsconfig_ul_mac_traffic_volume
(
/* Pointer to traffic volume config information */
tdsmac_ul_traffic_meas_config_type *oc_traffic_cfg_ptr
)
{

  boolean config_sucess = FALSE;
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
uint16    i;
#endif

  /* If oc_traffic_cfg_ptr is NULL print an error message */
  if (oc_traffic_cfg_ptr== NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Null config pointer");
  }
  else /* oc_traffic_cfg_ptr is not NULL */
  {
    switch (oc_traffic_cfg_ptr->meas_cmd)
    {
    /*------------------------------------------------------------------------------
       Start traffic volume measurement for this measurement id
    ------------------------------------------------------------------------------*/
    case TDSMAC_TRAFFIC_MEAS_START:
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Recvd TDSMAC_TRAFFIC_MEAS_START cmd for tdsmeas_id %d",oc_traffic_cfg_ptr->meas_id);
      config_sucess = tdsmac_ul_setup_traffic_volume_meas(oc_traffic_cfg_ptr);

#ifdef FEATURE_TDSCDMA_E4B_OPT
      if(config_sucess)
      {
        tdscheck_e4b_status(oc_traffic_cfg_ptr->meas_id);
      }
#endif
      break;

      /*------------------------------------------------------------------------------
           Modify traffic volume measurement for this measurement id
      ------------------------------------------------------------------------------*/
    case TDSMAC_TRAFFIC_MEAS_MODIFY:
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Recvd TDSMAC_TRAFFIC_MEAS_MODIFY cmd for tdsmeas_id %d",oc_traffic_cfg_ptr->meas_id);
      config_sucess = tdsmac_ul_modify_traffic_volume_meas(oc_traffic_cfg_ptr);

#ifdef FEATURE_TDSCDMA_E4B_OPT
      if(config_sucess)
      {
        tdscheck_e4b_status(oc_traffic_cfg_ptr->meas_id);
      }
#endif
      break;

      /*------------------------------------------------------------------------------
         Delete traffic volume measurement for this measurement id
      ------------------------------------------------------------------------------*/
    case TDSMAC_TRAFFIC_MEAS_DELETE:
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Recvd TDSMAC_TRAFFIC_MEAS_DELETE cmd for tdsmeas_id %d",oc_traffic_cfg_ptr->meas_id);
      config_sucess = tdsmac_ul_delete_traffic_volume_meas(oc_traffic_cfg_ptr->meas_id);
      break;

      /*------------------------------------------------------------------------------
         Delete all the traffic volume measurements.
      ------------------------------------------------------------------------------*/
    case TDSMAC_TRAFFIC_MEAS_DELETE_ALL:
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"Recvd TDSMAC_TRAFFIC_MEAS_DELETE_ALL cmd");
      config_sucess = tdsmac_ul_delete_all_traffic_volume_meas();
      break;

    case TDSMAC_TRAFFIC_MEAS_RESUME:
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Recvd TDSMAC_TRAFFIC_MEAS_RESUME for tdsmeas_id %d",oc_traffic_cfg_ptr->meas_id);
      config_sucess = tdsmac_ul_resume_traffic_volume_meas(oc_traffic_cfg_ptr->meas_id);
      break;

    case TDSMAC_TRAFFIC_MEAS_SAVE:
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Recvd TDSMAC_TRAFFIC_MEAS_SAVE for tdsmeas_id %d",oc_traffic_cfg_ptr->meas_id);
      config_sucess = tdsmac_ul_save_traffic_volume_meas(oc_traffic_cfg_ptr->meas_id);
      break;

    default:
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"Unknown command");
      break;
    }

#ifdef FEATURE_TDSCDMA_TVM_BLOCK
    if ((TRUE == config_sucess) && (TRUE == tdsmac_tvm_block_timer_started))
    { 
      /* Clear block timer */
      (void)rex_clr_timer(&tdsmac_ul_tvm_block_tmr_blk);
      tdsmac_tvm_block_timer_started = FALSE;
      TDSL2_MSG0(MSG_LEGACY_HIGH,"Cleared Block Traffic Volume Timer due to config change");

      /*---------------------------------------------------------------------------------
         Clear the TVM block table
  ---------------------------------------------------------------------------------*/
      for (i=0; i<TDSUE_NUM_TRAFFIC_MEAS; i++)
      {
        memset(&tdsmac_ul_event_tvm_block_table[i], 0, sizeof(tdsmaci_ul_event_tvm_blk_index_type));
      }
    }
#endif

    /*+yongh add optimization feature NV for L2+*/
    if ((tdsl2_opt_bitmask&TDSL2_OPT_FEATURE_E4B_TWICE) &&
         (tdsl2_opt_bitmask&TDSL2_OPT_FEATURE_E4B_FOURTH))
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"NV control e4b report time is 6");    
      tdsmac_nv_e4b_event_cnt = 6;
    }
    else if (tdsl2_opt_bitmask&TDSL2_OPT_FEATURE_E4B_FOURTH)
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"NV control e4b report time is 4"); 
      tdsmac_nv_e4b_event_cnt = 4;
    }
    else if (tdsl2_opt_bitmask&TDSL2_OPT_FEATURE_E4B_TWICE)
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"NV control e4b report time is 2"); 
      tdsmac_nv_e4b_event_cnt = 2;
    }
    else
    {
      tdsmac_nv_e4b_event_cnt = 0;
    }
    /*-yongh add optimization feature NV for L2-*/
    
    /*------------------------------------------------------------------------------
      If this is set to true then send a message to RRC confirming the receipt of
      the config information
    ------------------------------------------------------------------------------*/
#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
    if (log_status(TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET))
    {
      tdsmac_ul_tvm_cfg_log_packet(oc_traffic_cfg_ptr);
    }
#endif

    if (oc_traffic_cfg_ptr->config_conf_ind)
    {
      /*---------------------------------------------------------------------------
       Posts the message in the uplink RRC task queue.
      ---------------------------------------------------------------------------*/
      tdsmaci_post_cfg_cnf_msg(oc_traffic_cfg_ptr->config_ack_num,
                            oc_traffic_cfg_ptr->meas_id, config_sucess);
    }

    /*free memory : cfg info got from RRC */
    
    tdsmac_ul_free_mem_ext_cmd(oc_traffic_cfg_ptr);
  }
}

/*==================================================================================
 FUNCTION NAME  tdsmac_update_object_list

 PURPOSE    This function finds all the uplink currently active transport channels
            and populates the transport channel information array. It returns the number
            of transport channels.
==================================================================================*/
uint16 tdsmac_update_object_list
(
  uint16                            ntrch,
  /* Array of transport channel type and id information*/
  tdsmac_ul_traffic_trch_info_type     tdstrch_info[]

)
{
  uint16                  i,j;     /* loop index */
  tr_ch_id_type           tdstrch_id; /*transport channel id */
  tdstrch_id_to_idx_map_type *id_to_idx_ptr; /*Transport channel id to index pointer*/
  boolean                 found = FALSE;

  /* Go through all the possible transport channels id */
  for (i=0;i<MAX_TRCH;i++)
  {
    /* The transport channel id ranges from 1 to 32 */
    tdstrch_id = i+TDSMAC_UL_MIN_TRCH_ID;

    /* Get the index where transport channel information for this id is stored*/
    id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);

    /* if the corresponding index has valid parameters */
    if (id_to_idx_ptr->valid)
    {
      found = FALSE;

      for (j=0;j<ntrch;j++)
      {
        if ((tdstrch_id == tdstrch_info[j].trch_id) &&
            (TDSGET_UL_CCTRCH_TYPE == tdstrch_info[j].trch_type))
        {
          found = TRUE;
          break;
        }
      }

      if(found == FALSE)
      {
        /* Store the transport channel id and the type in the appropriate place */
        tdstrch_info[ntrch].trch_id = tdstrch_id;
        tdstrch_info[ntrch].trch_type = TDSGET_UL_CCTRCH_TYPE;
        ntrch++;
        TDSMAC_MSG3(MSG_LEGACY_HIGH,"Added tdstrch_id = %d, tdstrch_type = %d, ntrch = %d",
                 tdstrch_id, TDSGET_UL_CCTRCH_TYPE, ntrch);
      }

    } /*if transport channel is valid*/

  }/*for the number of transport channels */

#if defined (FEATURE_TDSCDMA_HSUPA)
  if(TDSMAC_ULHS_ACTIVE == tdsmac_ulhs_state)
  {
    found = FALSE;

    for (j=0;j<ntrch;j++)
    {
      if ((TDSEDCH_ID == tdstrch_info[j].trch_id) &&
          (TDSEDCH == tdstrch_info[j].trch_type))
      {
        found = TRUE;
        break;
      }
    }

    if(FALSE == found)
    {
      if(ntrch < TDSUE_MAX_TRCH)
      {
        tdstrch_info[ntrch].trch_id = TDSEDCH_ID;
        tdstrch_info[ntrch].trch_type = TDSEDCH;
        ntrch++;
        TDSMAC_MSG3(MSG_LEGACY_HIGH,"Added tdstrch_id = %d, tdstrch_type = %d, ntrch = %d",
                 TDSEDCH_ID, TDSEDCH, ntrch);
      }

    }/* found == FALSE */
 }/* TDSMAC_ULHS_ACTIVE == tdsmac_ulhs_state */

#endif
  return ntrch;
}

/*==================================================================================
 FUNCTION NAME  tdspopulate_object_list

 PURPOSE    This function finds all the uplink currently active transport channels
            and populates the transport channel information array. It returns the number
            of transport channels.
==================================================================================*/

void tdsmac_ul_update_trch_info(void)
{
  uint16                            tdsmeas_id;
  tdsmaci_ul_traffic_cfg_index_type    *traffic_cfg_idx_ptr;
  tdsmac_ul_traffic_report_mode_e_type tdsreport_mode;
  tdsmaci_ul_traffic_cfg_param         *traffic_cfg_ptr;

  for (tdsmeas_id = TDSMAC_UL_MIN_MEAS_ID; tdsmeas_id <= TDSMAC_UL_MAX_MEAS_ID; tdsmeas_id++)
  {
    /*--------------------------------------------------------------------------
      Get the config, pointer corresponding to this measurement
      identity.
    --------------------------------------------------------------------------*/
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);

    /*--------------------------------------------------------------------------
     If it contains valid parameters and the pointer is not NULL then start
     measurements for this measurement identity.Check to see if we need to
     temporarily suspend measurements for this identity. suspend_meas=TRUE indicates
     that we need to save this configuration information and resume measurements
     only after the resume command is recvd for this identity.
    --------------------------------------------------------------------------*/
    if ((traffic_cfg_idx_ptr->valid)&&(!traffic_cfg_idx_ptr->suspend_meas)&&
        (traffic_cfg_idx_ptr->traffic_cfg_ptr!=NULL))
    {
      traffic_cfg_ptr = traffic_cfg_idx_ptr->traffic_cfg_ptr;

      tdsreport_mode = traffic_cfg_ptr->report_mode;

      /*if (traffic_cfg_ptr->meas_obj_list_incl == FALSE)
      {
        traffic_cfg_ptr->num_trch =
        tdspopulate_object_list(traffic_cfg_ptr->trch_info);
      }*/

      /*-----------------------------------------------------------------------
        Based on the mode, check to see if it is time to send a report.
      -----------------------------------------------------------------------*/
      if (tdsreport_mode==TDSEVENT_TRIGGERED_MODE)
      {
        if (traffic_cfg_ptr->event_trch_incl == FALSE)
        {
          traffic_cfg_ptr->num_trch =
            tdsmac_update_object_list(
              traffic_cfg_ptr->num_trch,
              traffic_cfg_ptr->trch_info
            );
        }
      }

    }/* if traffic_cfg_ptr is valid */

  }/* for */

}

#ifdef FEATURE_TDSCDMA_E4B_OPT
/*====================================================================================
 FUNCTION NAME tdscheck_trch_bo_and_e4btrigger

 PURPOSE  For each transport channel listed in the report criteria for this
          measurement identity this function checks , if any of the transport channel traffic
          volume is greater than 0 or e4b is not triggered. It return false.
=======================================================================================*/
boolean tdscheck_trch_bo_and_e4btrigger
( 
	uint16           tdsmeas_id
)
{
  uint32                                   tdstrch_bo=0; /* Buffer occupancy */
  uint16                                   i;
  tdsmaci_ul_traffic_cfg_param             *traffic_param_ptr;
  tdsmaci_ul_traffic_cfg_index_type        *traffic_cfg_idx_ptr;
  tr_ch_id_type                            tdstrch_id;
  tdsmac_ul_cctrch_e_type                  tdstrch_type;
  uint16                                   ntrch;
  tdstrch_id_to_idx_map_type               *id_to_idx_ptr;
  boolean                                  report_e4b = TRUE;
 
  /*-----------------------------------------------------------------------------------
  Get the traffic config pointer and the intermediate measurement results corresponding
  to this measurement identity.
  ------------------------------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
    traffic_param_ptr = traffic_cfg_idx_ptr->traffic_cfg_ptr;
  
    /* If the event transport channel list is included */
    if (traffic_param_ptr->event_trch_incl)
    {
      ntrch = traffic_param_ptr->event_num_trch;
    }
    else
    {
      /*--------------------------------------------------------------------------------
      If the event transport channel list is not included then apply the criteria to
      the transport channles listed in the measurement object list.
      --------------------------------------------------------------------------------*/
      ntrch = traffic_param_ptr->num_trch;
    }
  
    /* go through the list of transport channels */
    for (i=0 ; i < ntrch && i < TDSUE_MAX_TRCH ; i++)
    {
      /* Initialise */
      tdstrch_bo=0;

      /* if the transport channel list was specified by RRC */
      if (traffic_param_ptr->event_trch_incl)
      {
        /* Get the transport channel id and type*/
        tdstrch_id = traffic_param_ptr->event_trch_info[i].trch_id;
        tdstrch_type = traffic_param_ptr->event_trch_info[i].trch_type;
      }
      else
      {
        /* Get the transport channel id and type*/
        tdstrch_id = traffic_param_ptr->trch_info[i].trch_id;
        tdstrch_type = traffic_param_ptr->trch_info[i].trch_type;
      }

      if (!TDSTRCH_ID_TO_IDX_IS_INVALID(tdstrch_id))
      {
        id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);

        if ((tdstrch_type==TDSDCH)&&(tdstrch_type==TDSGET_UL_CCTRCH_TYPE)&&(id_to_idx_ptr->valid))
        {
          /*---------------------------------------------------------------------------------
           Compute the transport channel traffic volume.
          ---------------------------------------------------------------------------------*/
          tdstrch_bo = tdscompute_trch_traffic_volume(tdstrch_id,tdstrch_type);
    
          /* Exist TRCH BO is non-zero, don't report e4b*/
					if (tdstrch_bo > 0)
          {
            report_e4b = FALSE;
						TDSMAC_MSG2(MSG_LEGACY_HIGH,"e4b reported is ignored in trch %d for trch bo is %d",tdstrch_id,tdstrch_bo);
            break;
          }
					
          /* Exist TRCH e4b is not triggered, don't report e4b*/
					if((traffic_param_ptr->trch_report_stauts[i].e4b_configured == TRUE)
				      && (traffic_param_ptr->trch_report_stauts[i].e4b_triggered == FALSE))
			    {
			      report_e4b = FALSE;
						TDSMAC_MSG1(MSG_LEGACY_HIGH,"e4b reported is ignored in trch %d for trch e4b is not triggered",tdstrch_id);
				    break;
			    }
        }
      }
    }
  }

  return report_e4b;
}

/*====================================================================================
 FUNCTION NAME tdscheck_e4b_status

 PURPOSE  Check e4b status for each transport channel listed in the report criteria or obj list for this
          measurement identity.
=======================================================================================*/
void tdscheck_e4b_status
( 
	uint16   tdsmeas_id
)
{
  tdsmaci_ul_traffic_trch_event_param         *event_cfg_ptr;
  uint16                                      i, j;
  tdsmaci_ul_traffic_cfg_param                *traffic_param_ptr;
  tdsmaci_ul_traffic_cfg_index_type           *traffic_cfg_idx_ptr;
  tdsmac_ul_traffic_event_id_e_type           tdsevent_id;
  tr_ch_id_type                               tdstrch_id;
  tdsmac_ul_cctrch_e_type                     tdstrch_type;
  uint16                                      ntrch;
  tdstrch_id_to_idx_map_type                  *id_to_idx_ptr;
  uint8                                       nevents=0;
  /*-----------------------------------------------------------------------------------
  Get the traffic config pointer and the intermediate measurement results corresponding
  to this measurement identity.
  ------------------------------------------------------------------------------------*/
  if (TDSTRAFFIC_MEAS_ID_IS_INVALID(tdsmeas_id))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid measurement ID");
  }
  else
  {
    traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);
    traffic_param_ptr = traffic_cfg_idx_ptr->traffic_cfg_ptr;
  
    /* If the event transport channel list is included */
    if (traffic_param_ptr->event_trch_incl)
    {
      ntrch = traffic_param_ptr->event_num_trch;
    }
    else
    {
      /*--------------------------------------------------------------------------------
      If the event transport channel list is not included then apply the criteria to
      the transport channles listed in the measurement object list.
      --------------------------------------------------------------------------------*/
      ntrch = traffic_param_ptr->num_trch;
    }
  
    /* go through the list of transport channels */
    for (i=0 ; i < ntrch && i < TDSUE_MAX_TRCH ; i++)
    {
      /* Initialize  e4b_triggered and e4b_configured of each trch to false*/
			traffic_param_ptr->trch_report_stauts[i].e4b_triggered  = FALSE;
			traffic_param_ptr->trch_report_stauts[i].e4b_configured = FALSE;
  
      /* if the transport channel list was specified by RRC */
      if (traffic_param_ptr->event_trch_incl)
      {
        /*-------------------------------------------------------------------------------
        TRUE implies the event criteria was specified for each transport channel
        in the list.
        -------------------------------------------------------------------------------*/
        event_cfg_ptr = traffic_param_ptr->trch_event_param[i];
  
        /* Get the transport channel id and type*/
        tdstrch_id = traffic_param_ptr->event_trch_info[i].trch_id;
        tdstrch_type = traffic_param_ptr->event_trch_info[i].trch_type;
        nevents = (uint8)traffic_param_ptr->num_events[i];
      }
      else
      {
        /*-------------------------------------------------------------------------------
        FALSE implies only one event criteria is specified and it is applicable to all
        transport channels specified in the object list.
        -------------------------------------------------------------------------------*/
        event_cfg_ptr = traffic_param_ptr->trch_event_param[0];
        nevents = (uint8)traffic_param_ptr->num_events[0];
  
        /*-------------------------------------------------------------------------------
        When the set of Transport channels on which the event trigger is defined is not
        provided explicitly, the UE shall apply it on the Transport channels specified
        by the measurement object.
        -------------------------------------------------------------------------------*/
        /* Get the transport channel id */
        tdstrch_id = traffic_param_ptr->trch_info[i].trch_id;
        tdstrch_type = traffic_param_ptr->trch_info[i].trch_type;
      }

      if (!TDSTRCH_ID_TO_IDX_IS_INVALID(tdstrch_id))
      {
        id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(tdstrch_id);

        if ((tdstrch_type==TDSDCH)&&(tdstrch_type==TDSGET_UL_CCTRCH_TYPE)&&(id_to_idx_ptr->valid))
        {
          /* for the number of events corresponding to the transport channel. could be 1 or 2 */
          for (j=0;j< nevents && j < TDSMAX_NUM_EVENTS;j++)
          {         
            /*------------------------------------------------------------------------------
                   Get the event id.
                   ------------------------------------------------------------------------------*/
            tdsevent_id = event_cfg_ptr[j].event_id;

						if(tdsevent_id == TDSEVENT_4B)
            {
              traffic_param_ptr->trch_report_stauts[i].e4b_configured = TRUE;
              if(tdsl2_opt_bitmask & TDSL2_OPT_FEATURE_E4B_IGNORE)
              {
                TDSMAC_MSG2(MSG_LEGACY_HIGH,"TVM: Trch id %d, Old e4b report_threshold is %d,New e4b report_threshold is 1",tdstrch_id,event_cfg_ptr[j].report_threshold);
							
                event_cfg_ptr[j].report_threshold = 1;
              }
            }
          }/*for event list */
        }
      }
    }/* for number of transport channels */
  } 
}
#endif
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
/*=====================================================================================
 FUNCTION NAME  tdsmac_ul_block_tvm_report

 PURPOSE    This function blocks TVM report when receiving the related indication from RRC
=====================================================================================*/
void tdsmac_ul_block_tvm_report (uint16 block_time_interval)
{
  /* No measurement now */
  if ((tdsnum_event_measurements + tdsnum_periodic_measurements) == 0)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"No measurement now");
  }
  else if ((block_time_interval == 0) && (tdsmac_tvm_block_timer_started == TRUE))
  {
    /* When block timer is on and receive block_time_interval=0, RRC revert, MAC can think the block timer expired */
    tdsmac_ul_handle_tvm_block_tmr();
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"block_time_interval = 0,RRC revert");
  }
  else if (block_time_interval == 0)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"initial block_time_interval = 0");
  }
  else
  {
    (void)rex_set_timer(&tdsmac_ul_tvm_block_tmr_blk, (uint32)(block_time_interval));
    tdsmac_tvm_block_timer_started = TRUE;
    TDSL2_MSG1(MSG_LEGACY_HIGH,"Starting tdsmac_tvm_block_timer_started timer %d ms",block_time_interval);
  }
}

/*===============================================================================
 FUNCTION NAME  tdsmac_ul_handle_tvm_block_tmr

 PURPOSE    This function is called when block timer signal expires.
                  MAC will check whether to send measurement report or not.
================================================================================*/
void tdsmac_ul_handle_tvm_block_tmr (void)
{
  uint16                                tdsmeas_id;
  tdsmaci_ul_traffic_cfg_index_type     *traffic_cfg_idx_ptr;
  tdsmac_ul_traffic_report_mode_e_type  tdsreport_mode;
  uint16                                i;
  
  /* Clear block timer */
  (void)rex_clr_timer(&tdsmac_ul_tvm_block_tmr_blk);
  tdsmac_tvm_block_timer_started = FALSE;
  TDSL2_MSG0(MSG_LEGACY_HIGH,"Cleared Block Traffic Volume Timer");

  /* No measurement now */
  if ((tdsnum_event_measurements + tdsnum_periodic_measurements) == 0)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"No measurement now");
  }
  /* check whether to send measurement report or not */
  else if (tdsnum_event_measurements > 0)
  {
    for (tdsmeas_id = TDSMAC_UL_MIN_MEAS_ID; tdsmeas_id <= TDSMAC_UL_MAX_MEAS_ID; tdsmeas_id++)
    {
      /*--------------------------------------------------------------------------
        Get the config, pointer corresponding to this measurement identity.
      --------------------------------------------------------------------------*/
      traffic_cfg_idx_ptr = TDSGET_UL_TRAFFIC_CFG_PARAM_PTR(tdsmeas_id);

      /*--------------------------------------------------------------------------
      If it contains valid parameters and the pointer is not NULL then start
      measurements for this measurement identity.Check to see if we need to
      temporarily suspend measurements for this identity. suspend_meas=TRUE indicates
      that we need to save this configuration information and resume measurements
      only after the resume command is recvd for this identity.
      --------------------------------------------------------------------------*/
      if ((traffic_cfg_idx_ptr->valid)&&(!traffic_cfg_idx_ptr->suspend_meas)&&
          (traffic_cfg_idx_ptr->traffic_cfg_ptr!=NULL))
      {
        tdsreport_mode = traffic_cfg_idx_ptr->traffic_cfg_ptr->report_mode;

        /*-----------------------------------------------------------------------
          Based on the mode, check to see if it is time to send a report.
        -----------------------------------------------------------------------*/
        if ((tdsreport_mode == TDSEVENT_TRIGGERED_MODE) && 
             (tdsmac_ul_event_tvm_block_table[tdsmeas_id-TDSMAC_UL_MIN_MEAS_ID].ntrch > 0))
        {
          tdsproc_event_trigger_criteria(tdsmeas_id, TRUE);
        }
      }
    }
  }
  /*---------------------------------------------------------------------------------
  Clear the TVM block table
  ---------------------------------------------------------------------------------*/
  for (i=0; i<TDSUE_NUM_TRAFFIC_MEAS; i++)
  {
    memset(&tdsmac_ul_event_tvm_block_table[i], 0, sizeof(tdsmaci_ul_event_tvm_blk_index_type));
  }
}
#endif
