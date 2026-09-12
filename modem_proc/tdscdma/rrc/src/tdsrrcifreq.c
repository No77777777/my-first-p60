/*===========================================================================
            R R C   M E A S U R E M E N T S

DESCRIPTION

   This module supports Inter Freq Measurement Control and Measurement Reporting
   for the RRC layer.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

   RRC Task must have been started. All RRC command queues must
   have been initialized. All RRC signals must have been cleared
   at task startup.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

             EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcifreq.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/17/12   zy      fix CR430249, and add the check of m_length and k in DMO
09/29/12   zy      add code of IE DMO and IE Idele Interval Info 
03/01/12   zwj     fix: inter-frequency number use TDSL1_MAX_NON_USED_FREQ 
                        instead of TDSL1_MAX_FREQ
01/05/12   zwj     fix: no delta RSCP, close v590 extension
10/20/11   yuh     Merged LTE code
03/10/11   zwj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
03/08/11   mkh     Cleanup FEATURE_WCDMA_VASET_SUPPORT for measurement tests
11/23/10   zwj     Modify meas report for TDD
11/08/10   zwj     Modify inter report quantity 14.2.0c for TDD. 
11/06/10   zwj     Delete compress mode CCSA 8.4.1.3 for TDD.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "tdsrrcmeas.h"
#include "tdsrrcmeasi.h"
#include "tdsrrcsibdb.h"
#include "msg.h"
#include "tdsrrccmd_v.h"
#include "tdsl1mtask.h"
#include "tdsrrclcm.h"
#include "tdsrrcsend.h"
#include "tdsuecomdef.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcmcm.h"
#include "tdsrrcccm.h"
#include "mm_umts.h"
#include "sys_eplmn_list.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrccsp.h"
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
#include "tdsrrcllc.h"
#endif
#include "tdsrrccspi.h"
#include "tdsrrcasn1util.h"
#ifdef FEATURE_TDSCDMA_REL9
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#endif

#include "tdsrrcnv.h"

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#include "tdsrrcfreefloating.h"
#endif
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Inter-Freq Event Type */
#define  TDSRET_INTER_FREQ_EVENT_TYPE_R6(event_type)  \
(  \
 ((event_type == T_tdsrrc_InterFreqEvent_r6_event2a) ? TDSL1_INTER_FREQ_EVENT_2A : \
  ((event_type == T_tdsrrc_InterFreqEvent_r6_event2b) ? TDSL1_INTER_FREQ_EVENT_2B : \
  ((event_type == T_tdsrrc_InterFreqEvent_r6_event2c) ? TDSL1_INTER_FREQ_EVENT_2C : \
  ((event_type == T_tdsrrc_InterFreqEvent_r6_event2d) ? TDSL1_INTER_FREQ_EVENT_2D : \
  ((event_type == T_tdsrrc_InterFreqEvent_r6_event2e) ? TDSL1_INTER_FREQ_EVENT_2E : \
  TDSL1_INTER_FREQ_EVENT_2F ))))) \
)

#define  TDSRET_INTER_FREQ_EVENT_TYPE(event_type)  \
(  \
 ((event_type == T_tdsrrc_InterFreqEvent_event2a) ? TDSL1_INTER_FREQ_EVENT_2A : \
  ((event_type == T_tdsrrc_InterFreqEvent_event2b) ? TDSL1_INTER_FREQ_EVENT_2B : \
  ((event_type == T_tdsrrc_InterFreqEvent_event2c) ? TDSL1_INTER_FREQ_EVENT_2C : \
  ((event_type == T_tdsrrc_InterFreqEvent_event2d) ? TDSL1_INTER_FREQ_EVENT_2D : \
  ((event_type == T_tdsrrc_InterFreqEvent_event2e) ? TDSL1_INTER_FREQ_EVENT_2E : \
  TDSL1_INTER_FREQ_EVENT_2F ))))) \
)


/* Converts time to trigger for TDD from ASN1 format to L1 format */
#define  TDSRET_INTER_FREQ_TIME_TO_TRIGGER(time_to_trigger)  \
(  \
 (time_to_trigger == tdsrrc_TimeToTrigger_ttt0) ? 0 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt10) ? 10 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt20) ? 20 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt40) ? 40 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt60) ? 60 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt80) ? 80 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt100) ? 100 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt120) ? 120 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt160) ? 160 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt200) ? 200 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt240) ? 240 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_tt320) ? 320 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt640) ? 640 : \
 ((time_to_trigger == tdsrrc_TimeToTrigger_ttt1280) ? 1280 : \
((time_to_trigger == tdsrrc_TimeToTrigger_ttt2560) ? 2560 : 5000 \
  )))))))))))))) \
)


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Local Object Definition
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

extern tdsrrcmeas_substate_e_type tdsrrcmeas_current_substate;
extern uint8 tdsrrcmeas_pending_rpt;
/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */

/* Maximum No of Inter RAT measurements based on additional meas */
tdsinter_freq_meas_id_list_struct_type tdsinter_freq_meas_id_list[TDSMAX_INTER_FREQ_MEAS];

tdsinter_freq_cell_list_struct_type tdsinter_freq_cell_list[TDSL1_MAX_CELL_MEAS];

tdsinter_freq_cell_list_struct_type tdsinter_freq_cell_list_tmp[TDSL1_MAX_CELL_MEAS];

tdsinter_freq_cell_list_struct_type tdstmp_inter_freq_cell_list[TDSL1_MAX_CELL_MEAS];

/* Declared to store new intra freq cell list that is being built */
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsl1_inter_freq_cell_list_struct_type tdsrrcifreq_sib12_new_cell_list;

#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
tdsl1_inter_freq_rach_rpt_type tdsrrcifreq_inter_freq_rach_rpt;
tdsrach_report_crit_e_type tdsinterf_rach_report_crit;
extern tdssib12_present_e_type tdssib12_present;
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

/*Boolean added to track if interfrequency and interrat measurements are to be processed or not*/
extern boolean tdsprocess_interfreq_interrat_meas_rel7;

/*Boolean to track if inter frequency measurements are allowed or not
Inter freq measurements are allowed if process_interfreq is TRUE 
(Even in case tdsprocess_interfreq_interrat_meas_rel7 is FALSE)

Use the same variable to unblock UE internal measurements also
*/
extern boolean tdsprocess_interfreq_uei_meas_rel7;

/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_EVENT_CRITERIA

DESCRIPTION

    This function processes the event criteria received in a Measurement Control Message
  and stores in the format that is sent to L1

DEPENDENCIES

    None

RETURN VALUE

    FAILURE: If event criteria is invalid
    SUCCESS: If event criteria is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_event_criteria
(
  tdsrrc_InterFreqReportingCriteria* rrc_event_criteria_ptr,
  tdsl1_inter_freq_report_crit_struct_type* l1_event_criteria_ptr
 ,tdsrrc_MeasurementControl_v590ext_IEs* rel5_ext_ptr
)
{
  /* Store the list in a local pointer */
  tdsrrc_InterFreqEventList * ifreq_event_list_ptr = NULL;

  uint8 cnt = 0, count = 0;
  uint32 idx=0;
  /* To satisfy LINT */
  uint32 tmp = 0;

  tdsrrc_NonUsedFreqParameterList * local_non_used_param_ptr = NULL;

  tdsrrc_Inter_FreqEventCriteriaList_v590ext *inter_f_evt_ext_ptr = NULL;

  ifreq_event_list_ptr = &rrc_event_criteria_ptr->interFreqEventList;

  if (ifreq_event_list_ptr->n  == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Event Criteria list empty");
    return FAILURE;
  }

  if((rel5_ext_ptr != NULL) && (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rel5_ext_ptr,
    measurementCommand_v590ext)) &&
  (rel5_ext_ptr->measurementCommand_v590ext.t == 
  T_tdsrrc_MeasurementControl_v590ext_IEs_measurementCommand_v590ext_inter_frequency))
  {
    inter_f_evt_ext_ptr = &rel5_ext_ptr->measurementCommand_v590ext.u.inter_frequency;
  }

  while ((ifreq_event_list_ptr->n > idx) && (cnt < TDSL1_MAX_MEAS_EVENT))
  {
    /* Get Event Id */
    l1_event_criteria_ptr->evt[cnt].evt_id =
      TDSRET_INTER_FREQ_EVENT_TYPE(ifreq_event_list_ptr->elem[idx].t);

    l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;
    l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = 0;

    switch(l1_event_criteria_ptr->evt[cnt].evt_id)
    {

    case TDSL1_INTER_FREQ_EVENT_2A:
      /* Change of best frequency. */
      l1_event_criteria_ptr->evt[cnt].w_used = (uint16)
        (ifreq_event_list_ptr->elem[idx].u.event2a->usedFreqW);

      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2a->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2a->timeToTrigger);

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2a,
        tdsrrc_Event2a,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2a->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }
      /* Get Threshold Non-Used Freq */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2a,
        tdsrrc_Event2a,nonUsedFreqParameterList))
      {
        uint32 idy=0;

        local_non_used_param_ptr =
          &ifreq_event_list_ptr->elem[idx].u.event2a->nonUsedFreqParameterList;

        while ((local_non_used_param_ptr->n > count) && (count < TDSL1_MAX_NON_USED_FREQ))
        {
          /* Get non used threshold freq */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh
            =  (int16) local_non_used_param_ptr->elem[count].nonUsedFreqThreshold;
       /*  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].delta_non_used_freq_thresh_included
           = FALSE;        */
          if((inter_f_evt_ext_ptr != NULL) && (inter_f_evt_ext_ptr->n >idx))
          {
            if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_f_evt_ext_ptr,threholdNonUsedFrequency_deltaList))
            {
              if(inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.n > idy)
              {
                if(inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].m.deltaRSCPPresent)
                {
                  /*l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].
                    delta_non_used_freq_thresh_included = TRUE;
                  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].delta_non_used_freq_thresh
                    = inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].deltaRSCP;*/
                  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh +=
                    (int16) inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].deltaRSCP;
                }
                idy++;
              }
            }
          }

          /* Non used f */
          tmp = local_non_used_param_ptr->elem[count].nonUsedFreqW;
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].w_non_used_f =
            (uint16) tmp;

          count++;
        } /* End of while */

        l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = count;
        count = 0;

      }
      break;

    case TDSL1_INTER_FREQ_EVENT_2B:
    /* The estimated quality of the currently used frequency is below a certain
    threshold and the estimated quality of a non-used frequency is above a certain
      threshold. */

      l1_event_criteria_ptr->evt[cnt].freq_thresh = (int16)
        (ifreq_event_list_ptr->elem[idx].u.event2b->usedFreqThreshold);
      /*l1_event_criteria_ptr->evt[cnt].delta_freq_thresh_included = FALSE; */
      if((inter_f_evt_ext_ptr != NULL) && (inter_f_evt_ext_ptr->n > idx))
      {
        /*l1_event_criteria_ptr->evt[cnt].delta_freq_thresh_included = TRUE;
        l1_event_criteria_ptr->evt[cnt].delta_freq_thresh = inter_f_evt_ext_ptr->elem[idx].
          threholdUsedFrequency_delta; */
        l1_event_criteria_ptr->evt[cnt].freq_thresh += (int16) inter_f_evt_ext_ptr->elem[idx].
          threholdUsedFrequency_delta;
      }
      l1_event_criteria_ptr->evt[cnt].w_used = (uint16)
        (ifreq_event_list_ptr->elem[idx].u.event2b->usedFreqW);

      /* Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2b->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2b->timeToTrigger);

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2b,
        tdsrrc_Event2b,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2b->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }
      /* Get Threshold Non-Used Freq */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2b,
        tdsrrc_Event2b,nonUsedFreqParameterList))
      {
        uint32 idy=0;
        local_non_used_param_ptr =
          &ifreq_event_list_ptr->elem[idx].u.event2b->nonUsedFreqParameterList;

        while ((local_non_used_param_ptr->n > count) && (count < TDSL1_MAX_NON_USED_FREQ))
        {
          /* Get non used threshold freq */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh =
            (int16) local_non_used_param_ptr->elem[count].nonUsedFreqThreshold;
         /*l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].delta_non_used_freq_thresh_included
           = FALSE;        */
          if((inter_f_evt_ext_ptr != NULL) && (inter_f_evt_ext_ptr->n >idx))
          {
            if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_f_evt_ext_ptr,threholdNonUsedFrequency_deltaList))
            {
              if(inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.n > idy)
              {
                if(inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].m.deltaRSCPPresent)
                {
                  /*l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].
                    delta_non_used_freq_thresh_included = TRUE;
                  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].delta_non_used_freq_thresh
                    = inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].deltaRSCP;*/
                  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh +=
                    (int16) inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].deltaRSCP;
                }
                idy++;
              }
            }
          }
          /* Non used f */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].w_non_used_f =
            (uint16) (local_non_used_param_ptr->elem[count].nonUsedFreqW);

          count++;

        } /* End of while */

        l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = count;
        count = 0;
      }
      break;
    case TDSL1_INTER_FREQ_EVENT_2C:
      /* The estimated quality of a non-used frequency is above a certain threshold. */

      /* Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2c->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2c->timeToTrigger);

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2c,
        tdsrrc_Event2c,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2c->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }
      /* Get Threshold Non-Used Freq */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2c,
        tdsrrc_Event2c,nonUsedFreqParameterList))
      {
        uint32 idy =0;
        local_non_used_param_ptr =
          &ifreq_event_list_ptr->elem[idx].u.event2c->nonUsedFreqParameterList;

        while ((local_non_used_param_ptr->n > count) && (count < TDSL1_MAX_NON_USED_FREQ))
        {
          /* Get non used threshold freq */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh =
            (int16) local_non_used_param_ptr->elem[count].nonUsedFreqThreshold;
         /*l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].delta_non_used_freq_thresh_included
           = FALSE;        */
          if((inter_f_evt_ext_ptr != NULL) && (inter_f_evt_ext_ptr->n >idx))
          {
            if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_f_evt_ext_ptr,threholdNonUsedFrequency_deltaList))
            {
              if(inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.n > idy)
              {
                if(inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].m.
                  deltaRSCPPresent)
                {
                  /*l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].
                    delta_non_used_freq_thresh_included = TRUE;
                  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].delta_non_used_freq_thresh
                    = inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].deltaRSCP;*/
                  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh +=
                    (int16) inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].deltaRSCP;
                }
                idy++;
              }
            }
          }
          /* Non used f */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].w_non_used_f =
            (uint16) (local_non_used_param_ptr->elem[count].nonUsedFreqW);

          count++;

        } /* End of while */

        l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = count;
        count = 0;
      }
      break;



    case TDSL1_INTER_FREQ_EVENT_2E:
      /* The estimated quality of a non-used frequency is below a certain threshold. */

      /* Vivek Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2e->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2e->timeToTrigger);

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2e,
        tdsrrc_Event2e,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2e->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }
      /* Get Threshold Non-Used Freq */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2e,
        tdsrrc_Event2e,nonUsedFreqParameterList))
      {
        uint32 idy=0;
        
        local_non_used_param_ptr =
          &ifreq_event_list_ptr->elem[idx].u.event2e->nonUsedFreqParameterList;

        while ((local_non_used_param_ptr->n > count) && (count < TDSL1_MAX_NON_USED_FREQ))
        {
          /* Get non used threshold freq */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh =
            (int16) local_non_used_param_ptr->elem[count].nonUsedFreqThreshold;
        /* l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].delta_non_used_freq_thresh_included
           = FALSE;        */
          if((inter_f_evt_ext_ptr != NULL) && (inter_f_evt_ext_ptr->n > idx))
          {
            if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_f_evt_ext_ptr,threholdNonUsedFrequency_deltaList))
            {
              if(inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.n > idy)
              {
                if(inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].m.
                  deltaRSCPPresent)
                {
                  /*l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].
                    delta_non_used_freq_thresh_included = TRUE;
                  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].delta_non_used_freq_thresh
                    = inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].deltaRSCP;*/
                  l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh +=
                    (int16) inter_f_evt_ext_ptr->elem[idx].threholdNonUsedFrequency_deltaList.elem[idy].deltaRSCP;
                }
                idy++;
              }
            }
          }
          /* Non used f */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].w_non_used_f =
            (uint16) (local_non_used_param_ptr->elem[count].nonUsedFreqW);

          count++;

        } /* End of while */

        l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = count;
        count = 0;
      }
      break;

    case TDSL1_INTER_FREQ_EVENT_2D:

      /* Get used freq threshold */
      l1_event_criteria_ptr->evt[cnt].freq_thresh = (int16)
        ifreq_event_list_ptr->elem[idx].u.event2d->usedFreqThreshold;
      /*l1_event_criteria_ptr->evt[cnt].delta_freq_thresh_included = FALSE; */
      if((inter_f_evt_ext_ptr != NULL) && (inter_f_evt_ext_ptr->n > idx))
      {
        /*l1_event_criteria_ptr->evt[cnt].delta_freq_thresh_included = TRUE;
        l1_event_criteria_ptr->evt[cnt].delta_freq_thresh = inter_f_evt_ext_ptr->elem[idx].
          threholdUsedFrequency_delta; */
        l1_event_criteria_ptr->evt[cnt].freq_thresh += (int16) inter_f_evt_ext_ptr->elem[idx].
          threholdUsedFrequency_delta;
      }
      /* Get W */
      l1_event_criteria_ptr->evt[cnt].w_used = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2d->usedFreqW;

      /* Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2d->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2d->timeToTrigger);

      l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2d,
        tdsrrc_Event2d,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2d->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }

      /* Initialize no of non used freq params to 0 */
      l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = 0;

      break;

    case TDSL1_INTER_FREQ_EVENT_2F:

      /* Get used freq threshold */
      l1_event_criteria_ptr->evt[cnt].freq_thresh = (int16)
        ifreq_event_list_ptr->elem[idx].u.event2f->usedFreqThreshold;
      /*l1_event_criteria_ptr->evt[cnt].delta_freq_thresh_included = FALSE; */
      if((inter_f_evt_ext_ptr != NULL) && (inter_f_evt_ext_ptr->n > idx))
      {
        /*l1_event_criteria_ptr->evt[cnt].delta_freq_thresh_included = TRUE;
        l1_event_criteria_ptr->evt[cnt].delta_freq_thresh = inter_f_evt_ext_ptr->elem[idx].
          threholdUsedFrequency_delta; */
        l1_event_criteria_ptr->evt[cnt].freq_thresh += (int16) inter_f_evt_ext_ptr->elem[idx].
          threholdUsedFrequency_delta;
      }
      /* Get W */
      l1_event_criteria_ptr->evt[cnt].w_used = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2f->usedFreqW;

      /* Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2f->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2f->timeToTrigger);

      l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2f,
        tdsrrc_Event2d,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2f->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }

      /* Initialize no of non used freq params to 0 */
      l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = 0;

      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"InterFreq Event %d not supported",l1_event_criteria_ptr->evt[cnt].evt_id);
      return FAILURE;

    } /* End of switch */

   if ((l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl == TRUE) &&
      ((l1_event_criteria_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2A) ||
      (l1_event_criteria_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2B) ||
      (l1_event_criteria_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2C) ||
      (l1_event_criteria_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2E)) &&
      ((l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Event 2A/B/C/D,rpt cell stat %d",l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat);

      return FAILURE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Reporting Cell Status valid");
    }

    cnt++;

    /* Increment Pointer count */
   idx++;
  } /* End of while */

  /* Store the count */
  l1_event_criteria_ptr->num_event = cnt;

  return SUCCESS;

}

/*===========================================================================

FUNCTION   RRCIFREQ_INITIALIZE_MEAS_ID_LIST

DESCRIPTION

  Handles initialization of MEASUREMNT_IDENTIY variable
  for Inter Freq Measurements.

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcifreq_initialize_meas_id_list
(
 void
)
{
  uint8 count = 0, cnt = 0;
  for ( count = 0; count < TDSMAX_INTER_FREQ_MEAS; count++ )
  {
    /* Set all positions to vacant and make CPID as invalid */
    tdsinter_freq_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
    tdsinter_freq_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
    tdsinter_freq_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
    tdsinter_freq_meas_id_list[count].mcm_owned = FALSE;

    /* Also initialize additional meas */
    for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
      tdsinter_freq_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
  }
}



/*===========================================================================

FUNCTION  RRCIFREQ_SEND_MEAS_REPORT

DESCRIPTION

 This function sends the measurement report. First it validates the event
 and measured results depending upon the report type and it ignores the L1
 report if reported values are incorrect. The function also appends
 additional measurements if any and also updates the Inter freq meas
 id list if last periodic report is being sent

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcifreq_send_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
)
{
  /* Stores meas report */
  tdsrrc_UL_DCCH_Message* msg_ptr = NULL;

  uint8 cnt = 0, count = 0, cell_number = 0, i =0;

  uint8 inter_cell_cnt = 0;

  boolean delta_rscp_present = FALSE;

  tdsrrc_InterFreqCellList_LCR_r4_ext* local_rrc_event_cell_list_ptr = NULL;

  tdsrrc_InterFreqMeasuredResultsList * local_rrc_meas_results_ptr = NULL;

  tdsrrc_InterFreqCellMeasuredResultsList * local_inter_freq_cell_list_ptr = NULL;

  tdsrrc_InterFrequencyMeasuredResultsList_v590ext *inter_freq_meas_res_ext_ptr = NULL;

  /* Stores status of send chain */
  tdsrrcsend_status_e_type status;

  /* RLC ack or unack mode */
  uecomdef_logch_mode_e_type logical_channel_mode;

  /* Logical Channel Id */
  rlc_lc_id_type ul_lc_id;

  /* Radio bearer mode */
  tdsrrc_RB_Identity rb_mode;

  /* L2 ack status */
  tdsrrcsend_l2ack_status_e_type l2_ack_status = TDSRRCSEND_L2ACK_REQUIRED;

  if (tdsrrcifreq_validate_meas_report(l1_meas_report_ptr) == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ: Ignore Meas Report");
  /* Deallocate the cell list if any allocated by L1 */
    tdsrrcifreq_free_memory_allocated_for_reporting(l1_meas_report_ptr);
    return;
  }

  if ( l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE )
  {
    logical_channel_mode = UE_MODE_ACKNOWLEDGED_DATA;
    rb_mode = DCCH_AM_RADIO_BEARER_ID;
  }
  else
  {
    logical_channel_mode = UE_MODE_UNACKNOWLEDGED;
    rb_mode = DCCH_UM_RADIO_BEARER_ID;
    l2_ack_status = TDSRRCSEND_L2ACK_NOT_REQUIRED;
  }

  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH, rb_mode,
    logical_channel_mode);


  if ( ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RLC Logical channel not found");
  /* Deallocate the cell list if any allocated by L1 */
    tdsrrcifreq_free_memory_allocated_for_reporting(l1_meas_report_ptr);
    return;
  }

  /* Allocate a buffer for an uplink DCCH Message. This buffer
  * should be released by SEND_CHAIN using OSS compiler functions */

  msg_ptr = (tdsrrc_UL_DCCH_Message *) tdsrrc_malloc (sizeof(struct tdsrrc_UL_DCCH_Message));

  if ( msg_ptr == NULL )
  {
    ERR_FATAL("Failed to allocate memory ",0,0,0);
  }

  /* Store meas report chosen */
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_measurementReport;


  /* First store the meas id */
  msg_ptr->message.u.measurementReport.measurementIdentity =
    (tdsrrc_MeasurementIdentity)l1_meas_report_ptr->meas_id;

  /* Initialize Meas Report bit mask */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport);

  /* Malloc memory for MeasResult_v4b0  */
  tdsrrcmeas_set_measurement_result_v4b0ext(msg_ptr);
  
  if (l1_meas_report_ptr->rpt_crit == TDSL1_EVENT_TRIGGER_RPT)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ: Event Triggered Report");
    /* set bit mask for interFreqEventResults_LCR */
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.measurementReport_v4b0ext.m.interFreqEventResults_LCRPresent = 1;
  
    tdsrrcifreq_set_inter_freq_event_id(&l1_meas_report_ptr->u.inter_f_rpt.event_results.evt_id,
      &msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
       v4b0NonCriticalExtensions.measurementReport_v4b0ext.interFreqEventResults_LCR.eventID);

    if ((l1_meas_report_ptr->u.inter_f_rpt.event_results.evt_id == TDSL1_INTER_FREQ_EVENT_2D) ||
      (l1_meas_report_ptr->u.inter_f_rpt.event_results.evt_id == TDSL1_INTER_FREQ_EVENT_2F))
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Event 2D/2F. No cell list reqd");
    }
    else
    {
      /* For event results, inter freq event would be present */
      msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
         v4b0NonCriticalExtensions.measurementReport_v4b0ext.m.interFreqEventResults_LCRPresent = 1;
      
      /* For event results, inter freq cell list would be present */
      msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
         v4b0NonCriticalExtensions.measurementReport_v4b0ext.interFreqEventResults_LCR.m.interFreqCellListPresent = 1;

      /* For event results, inter freq cell list would be present */
      msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
       v4b0NonCriticalExtensions.measurementReport_v4b0ext.interFreqEventResults_LCR.
             interFreqCellList.n = l1_meas_report_ptr->u.inter_f_rpt.event_results.num_freq;
      
      /* Alloc memory for eventlist*/
      ALLOC_ASN1ARRAY1(&tdsenc_ctxt,&msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.
                laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext.
                interFreqEventResults_LCR.interFreqCellList,
                         tdsrrc_InterFreqCell_LCR_r4);
      
      /* Assign ptr to head */
      local_rrc_event_cell_list_ptr = &msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
         v4b0NonCriticalExtensions.measurementReport_v4b0ext.interFreqEventResults_LCR.interFreqCellList;      

      if (local_rrc_event_cell_list_ptr->elem == NULL)
      {
        ERR_FATAL("Failed to allocate memory",0,0,0);
      }
      
      for (cnt = 0; cnt < TDSL1_MAX_NON_USED_FREQ && cnt < l1_meas_report_ptr->u.inter_f_rpt.event_results.num_freq; cnt++)
      {
        local_rrc_event_cell_list_ptr->elem[cnt].frequencyInfo.modeSpecificInfo.t =
          T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd;
        local_rrc_event_cell_list_ptr->elem[cnt].frequencyInfo.modeSpecificInfo.u.tdd =
             rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_FrequencyInfoTDD);
        if (local_rrc_event_cell_list_ptr->elem[cnt].frequencyInfo.modeSpecificInfo.u.tdd == NULL)
        {
          ERR_FATAL("Failed to allocate memory",0,0,0);
        }

        /* Get DL UARFCN */
        local_rrc_event_cell_list_ptr->elem[cnt].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt =
          l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].freq_info.uarfcn_dl;

        if(l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].num_cpi < TDSL1_MAX_CELL_MEAS)
        {
          local_rrc_event_cell_list_ptr->elem[cnt].nonFreqRelatedEventResults.n =
            l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].num_cpi;
        }
        else
        {
          local_rrc_event_cell_list_ptr->elem[cnt].nonFreqRelatedEventResults.n = TDSL1_MAX_CELL_MEAS;
        }  
        
        ALLOC_ASN1ARRAY1(&tdsenc_ctxt,&local_rrc_event_cell_list_ptr->elem[cnt].nonFreqRelatedEventResults,
                         tdsrrc_PrimaryCCPCH_Info_LCR_r4);
        
        if (local_rrc_event_cell_list_ptr->elem[cnt].nonFreqRelatedEventResults.elem == NULL)
        {
          ERR_FATAL("Failed to allocate memory",0,0,0);
        }
          
        for (count = 0; count < TDSL1_MAX_NON_USED_FREQ && count < l1_meas_report_ptr->u.inter_f_rpt.
                    event_results.freq[cnt].num_cpi; count++)
        {
          /* Set CellParaID */
          local_rrc_event_cell_list_ptr->elem[cnt].nonFreqRelatedEventResults.elem[count].m.cellParametersIDPresent = 1;
          /* Store CellParaID */
          local_rrc_event_cell_list_ptr->elem[cnt].nonFreqRelatedEventResults.elem[count].cellParametersID =
            (tdsrrc_CellParametersID)l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].cpi[count];
        } /* End of for of list of cpi */
      } /* End of for of num_cells */
    } /* End of else */
  } /* End of if of Event triggered report */


  /* Now add Measured Results */
  if ((l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.num_msmt_results != 0) &&
    (l1_meas_report_ptr->u.inter_f_rpt.meas_results_included == TRUE))
  {
    /* Set inter-freq periodic meas report */
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
       v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.n = 1;
    
    TDSRRC_SET_COMMON_MSG_TYPE(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.
              laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext.
              additionalMeasuredResults_LCR.elem[0],
              rrc_MeasuredResults_LCR_r4_interFreqMeasuredResultsList);

    local_rrc_meas_results_ptr = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_InterFreqMeasuredResultsList);

    if (local_rrc_meas_results_ptr == NULL)
    {
      ERR_FATAL("Unable to allocate memory",0,0,0);
    }

    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
      v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.elem[0].u.
      interFreqMeasuredResultsList = local_rrc_meas_results_ptr;

    if(TDSL1_MAX_NON_USED_FREQ > l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.num_msmt_results)
    {
      local_rrc_meas_results_ptr->n= l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.num_msmt_results;
    }
    else
    {
      local_rrc_meas_results_ptr->n=TDSL1_MAX_NON_USED_FREQ;
    }

    ALLOC_ASN1ARRAY1(&tdsenc_ctxt,local_rrc_meas_results_ptr,tdsrrc_InterFreqMeasuredResults);

    if (local_rrc_meas_results_ptr->elem == NULL)
    {
      ERR_FATAL("Unable to allocate memory",0,0,0);
    }
    
    for (cnt = 0; (cnt < TDSL1_MAX_NON_USED_FREQ) && (cnt < l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.num_msmt_results); cnt++)
    {
      TDSRRC_RESET_MSG_IE_PRESENT(local_rrc_meas_results_ptr->elem[cnt]);

      if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
        msmt_results[cnt].utra_carrier_rssi_incl == TRUE)
      {
        local_rrc_meas_results_ptr->elem[cnt].m.utra_CarrierRSSIPresent = 1;

        local_rrc_meas_results_ptr->elem[cnt].utra_CarrierRSSI =
          (tdsrrc_UTRA_CarrierRSSI)l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
          msmt_results[cnt].utra_carrier_rssi;
      }

      /* Set bit maks for Freq Info */
      local_rrc_meas_results_ptr->elem[cnt].m.frequencyInfoPresent = 1;

      /* Set t to tdd */
      local_rrc_meas_results_ptr->elem[cnt].frequencyInfo.modeSpecificInfo.t =
        T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd;
      local_rrc_meas_results_ptr->elem[cnt].frequencyInfo.modeSpecificInfo.u.tdd =
        rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_FrequencyInfoTDD);

      if (local_rrc_meas_results_ptr->elem[cnt].frequencyInfo.modeSpecificInfo.u.tdd == NULL)
      {
        ERR_FATAL("Unable to allocate memory",0,0,0);
      }
      
      /* Set DL ARFCN bit mask to 0 */
      local_rrc_meas_results_ptr->elem[cnt].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt=
        l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].freq_info.uarfcn_dl;


      /* Now get Inter Freq Cells */

      if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].num_cells == 0)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Num cells per freq is %d",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].num_cells);
      }
      else
      {
        /* Assign memory for these cells */
        local_inter_freq_cell_list_ptr = &local_rrc_meas_results_ptr->elem[cnt].interFreqCellMeasuredResultsList;
        /* Set bit mask for Cell Meas Results List present */
        local_rrc_meas_results_ptr->elem[cnt].m.interFreqCellMeasuredResultsListPresent =1;

        /* Assign head */
         if(TDSL1_MAX_CELL_MEAS > l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].num_cells)
         {
           local_inter_freq_cell_list_ptr->n = 
            l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].num_cells;
         }
         else
         {
           local_inter_freq_cell_list_ptr->n = TDSL1_MAX_CELL_MEAS;
         }

        ALLOC_ASN1ARRAY1(&tdsenc_ctxt,local_inter_freq_cell_list_ptr,tdsrrc_CellMeasuredResults); 

        if (local_inter_freq_cell_list_ptr->elem == NULL)
        {
          ERR_FATAL("Unable to allocate memory",0,0,0);
        }
     

        if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) && (cnt == 0))
        {
          /* Memory already malloced in v4b0NonCriticalExtensions before using */
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent =1;
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.v590NonCriticalExtensions);
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext.m.
            measuredResults_v590extPresent =1;
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext.
            measuredResults_v590ext.t = T_tdsrrc_MeasuredResults_v590ext_interFrequencyMeasuredResultsList;
          
          inter_freq_meas_res_ext_ptr = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_InterFrequencyMeasuredResultsList_v590ext);
          if ( inter_freq_meas_res_ext_ptr == NULL )
          {
            ERR_FATAL("Failed to allocate memory",0,0,0);
          }
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext.
            measuredResults_v590ext.u.interFrequencyMeasuredResultsList = inter_freq_meas_res_ext_ptr;
          
          /* Malloc max number, then use the actual cell number  */
          inter_freq_meas_res_ext_ptr->n = TDSL1_MAX_CELL_MEAS;
          ALLOC_ASN1ARRAY1(&tdsenc_ctxt,inter_freq_meas_res_ext_ptr,tdsrrc_DeltaRSCPPerCell);         
          inter_freq_meas_res_ext_ptr->n = 0;
          if ( inter_freq_meas_res_ext_ptr->elem == NULL )
          {
            ERR_FATAL("Failed to allocate memory",0,0,0);
          }
        }
        
        local_inter_freq_cell_list_ptr->n = 0;
        
        /* Now validate Measured Results */
        for ( cell_number = 0; ((cell_number < l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].num_cells)
          && ( cell_number < TDSL1_MAX_CELL_MEAS)); cell_number++ )
        {
          TDSRRC_RESET_MSG_IE_PRESENT(local_inter_freq_cell_list_ptr->elem[cell_number]);
          if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_rpt_ind == TDSL1_NO_REPORT)
          {
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SFN SFN observed time diff not set");
          }
          else
          {
            /* Set bit mask to sfn_sfn present */
            local_inter_freq_cell_list_ptr->elem[cell_number].m.dummyPresent =1;

            /* Store sfn-sfn type indicator */

            if ( l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_rpt_ind == TDSL1_REPORT_TYPE_1)
            {
              local_inter_freq_cell_list_ptr->elem[cell_number].dummy.t =  T_tdsrrc_SFN_SFN_ObsTimeDifference_type1;

              TDSRRC_MSG0(MSG_LEGACY_LOW,"Report type 1 selected");
              /* Type 1 report */
              local_inter_freq_cell_list_ptr->elem[cell_number].dummy.u.type1 =
                l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_sfn_tm_diff;

            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_LOW,"Report type 2 selected");
              local_inter_freq_cell_list_ptr->elem[cell_number].dummy.t =  T_tdsrrc_SFN_SFN_ObsTimeDifference_type2;
              /* Type 2 report */
              local_inter_freq_cell_list_ptr->elem[cell_number].dummy.u.type2 =
                (tdsrrc_SFN_SFN_ObsTimeDifference2)l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_sfn_tm_diff;

            }

          } /* End of else of TDSL1_NO_REPORT */

          /* Check if cell sync info present */

          if ( l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->cell_sync_info_incl == TRUE )
          {
            /* Set cell sync info present to rrc_cellSynchronisationInfo) */
            local_inter_freq_cell_list_ptr->elem[cell_number].m.cellSynchronisationInfoPresent =1;

            /* Store Choice Mode info to tdd*/
            local_inter_freq_cell_list_ptr->elem[cell_number].cellSynchronisationInfo.
              modeSpecificInfo.t =
              T_tdsrrc_CellSynchronisationInfo_modeSpecificInfo_tdd;

            local_inter_freq_cell_list_ptr->elem[cell_number].cellSynchronisationInfo.modeSpecificInfo.u.tdd
                = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_CellSynchronisationInfo_tdd);
            
            if (local_inter_freq_cell_list_ptr->elem[cell_number].cellSynchronisationInfo.modeSpecificInfo.u.tdd == NULL)
            {
              ERR_FATAL("Malloc failed",0,0,0);
            }

            local_inter_freq_cell_list_ptr->elem[cell_number].cellSynchronisationInfo.modeSpecificInfo.u.
                    tdd->m.countC_SFN_Frame_differencePresent = 1;

            /* Set tdsrrc_CountC_SFN_Frame_difference value structure */
            
            if(l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                   cell[cell_number]->cell_sync_info.count_c_sfn_diff_incl)
            {
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"CNT_C_SFN value from L1 : %d", 
                l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                   cell[cell_number]->cell_sync_info.count_c_sfn_high,0,0);
              local_inter_freq_cell_list_ptr->elem[cell_number].cellSynchronisationInfo.modeSpecificInfo.u.
                tdd->countC_SFN_Frame_difference.countC_SFN_High =
                (uint8)l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->
                         cell_sync_info.count_c_sfn_high;
            }
            else
            {
              local_inter_freq_cell_list_ptr->elem[cell_number].cellSynchronisationInfo.modeSpecificInfo.u.
                tdd->countC_SFN_Frame_difference.countC_SFN_High = 0;
            }
            /* Set off value */
            if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                    cell[cell_number]->cell_sync_info.off <= TDSOFF_MAX)
            {
              local_inter_freq_cell_list_ptr->elem[cell_number].cellSynchronisationInfo.
                modeSpecificInfo.u.tdd->countC_SFN_Frame_difference.off =
                (uint8)l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                     cell[cell_number]->cell_sync_info.off;
            }
            else
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"Off value %d not in range",
                l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                   cell[cell_number]->cell_sync_info.off,0,0);
              local_inter_freq_cell_list_ptr->elem[cell_number].cellSynchronisationInfo.
                modeSpecificInfo.u.tdd->countC_SFN_Frame_difference.off = TDSOFF_MIN;
            }
          } /* End of if of rrc_cellSynchronisationInfo) */

          /* Set in the TDD mode for tdsrrc_CellMeasuredResults_modeSpecificInfo */
          local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.t =
            T_tdsrrc_CellMeasuredResults_modeSpecificInfo_tdd;

          local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd =
            rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_CellMeasuredResults_tdd);

          if (local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd == NULL)
          {
            ERR_FATAL("Malloc failed",0,0,0);
          }

          TDSRRC_RESET_MSG_IE_PRESENT_PTR(local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd);

          /* Set primary scrambling code */
          local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->cellParametersID =
            (tdsrrc_CellParametersID)l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->cpi;


          /* Convert meas value to UTRAN format */

          /* Check if RSCP included */
          if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->rscp_included == TRUE)
          {
            local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->primaryCCPCH_RSCP =
              (tdsrrc_PrimaryCCPCH_RSCP)l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                        cell[cell_number]->rscp_val;

            /* Set bit mask */
            local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->m.primaryCCPCH_RSCPPresent =1;
            if(inter_freq_meas_res_ext_ptr != NULL)
            {              
              TDSRRC_RESET_MSG_IE_PRESENT(inter_freq_meas_res_ext_ptr->elem[inter_freq_meas_res_ext_ptr->n]);
              if(l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
              cell[cell_number]->delta_rscp_included == TRUE)
              {
                delta_rscp_present = TRUE;
                inter_freq_meas_res_ext_ptr->elem[inter_freq_meas_res_ext_ptr->n].m.deltaRSCPPresent =1;
                inter_freq_meas_res_ext_ptr->elem[inter_freq_meas_res_ext_ptr->n].deltaRSCP = 
                  (tdsrrc_DeltaRSCP)l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->delta_rscp;
              }
              inter_freq_meas_res_ext_ptr->n++;            
            }
          }
          /* Check if ISCP included */
          if ((l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                cell[cell_number]->ts_list_included == TRUE) &&
                (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                 cell[cell_number]->no_ts != 0))
          {
            local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->timeslotISCP_List.n =
              l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
                     msmt_results[cnt].cell[cell_number]->no_ts;
            
            ALLOC_ASN1ARRAY1(&tdsenc_ctxt, &local_inter_freq_cell_list_ptr->elem[cell_number].
                   modeSpecificInfo.u.tdd->timeslotISCP_List,
                   tdsrrc_TimeslotISCP);
            
            if (local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->
                timeslotISCP_List.elem == NULL)
            {
              ERR_FATAL("Failed to allocate memory",0,0,0);
            }
            for (i = 0; i < l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                 cell[cell_number]->no_ts; i++)
            {
              local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->
                 timeslotISCP_List.elem[i] = 
                  (tdsrrc_TimeslotISCP)l1_meas_report_ptr->u.inter_f_rpt.
                   inter_freq_measurements.msmt_results[cnt].cell[cell_number]->iscp_ts_list[i].iscp_value;
            }
            /* Set bit mask */
            local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->m.timeslotISCP_ListPresent =1;             
          }

          /* Check if Path Loss included */
          if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                cell[cell_number]->pathloss_included == TRUE)
          {

            local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->pathloss =
              (tdsrrc_Pathloss)l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->pathloss_val;

            /* Set bit mask */
            local_inter_freq_cell_list_ptr->elem[cell_number].modeSpecificInfo.u.tdd->m.pathlossPresent =1;

          }
          local_inter_freq_cell_list_ptr->n++;
          inter_cell_cnt++;
          if (inter_cell_cnt == TDSL1_MAX_CELL_MEAS)
          {
            break;
          }
          /* One cell completely filled here */
        } /* End of for loop */
      } /* No of cell != 0 */
    } /* End of for of meas results */
  }    /* num msmt results != 0 */


  tdsrrcmeas_process_additional_measured_results(
    &l1_meas_report_ptr->meas_id, TDSINTER_FREQ, &msg_ptr->message.u.measurementReport);

  /* If no delta RSCP, close v590 extension */
  if (delta_rscp_present != TRUE)
  {
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
	  v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext.
	  m.measuredResults_v590extPresent = 0;
    /* close v590NonCriticalExtensions if v5b0NonCriticalExtensionsPresent is not open, or tc 8.4.1.25, 8.4.1.25.a fail */
	if (!(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
	  v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v5b0NonCriticalExtensionsPresent))
	{
	  msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
	  v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent = 0;
	}
  }

  /* Now check if this is the last report. Meas Id needs to be deleted if true */
  if ((l1_meas_report_ptr->last_periodic_report == TRUE) &&
    (l1_meas_report_ptr->rpt_crit == TDSL1_PERIODIC_RPT))
  {
    /* Delete this Measurement */
    for ( cnt =0; cnt < TDSMAX_INTER_FREQ_MEAS; cnt++)
    {
      if (tdsinter_freq_meas_id_list[cnt].meas_id == l1_meas_report_ptr->meas_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Delete Meas %d",l1_meas_report_ptr->meas_id);
        tdsinter_freq_meas_id_list[cnt].meas_id = TDSINVALID_MEAS_ID;

        tdsinter_freq_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;

        tdsinter_freq_meas_id_list[count].mcm_owned = FALSE;

        tdsinter_freq_meas_id_list[count].meas_validity = TDSNOT_PRESENT;

        /* Also initialize additional meas */
        for ( count = 0; count < TDSMAX_ADDITIONAL_MEAS; count++ )
          tdsinter_freq_meas_id_list[cnt].additional_meas[count] = TDSINVALID_MEAS_ID;

        break;
      }
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Delete %d from addtl meas DB",l1_meas_report_ptr->meas_id);
    tdsrrcmeas_delete_addtl_meas(&l1_meas_report_ptr->meas_id);

  }

  /* Closed measurement_result_v4b0ext if malloc memory no use*/
  tdsrrcmeas_close_measurement_result_v4b0ext(msg_ptr);

  status =  tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_MCMR, (void *) msg_ptr,
    ul_lc_id, l2_ack_status);

  if ( status != TDSRRCSEND_SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Send chain failed to send Inter-RAT Meas Rpt");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter-Freq Rpt sent to RLC");
    if(l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE)
    {
      tdsrrcmeas_pending_rpt++;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"pending meas rpt = %d",tdsrrcmeas_pending_rpt);
    }
  }

  /* Deallocate the cell list if any allocated by L1 */
  tdsrrcifreq_free_memory_allocated_for_reporting(l1_meas_report_ptr);

  return;
}




/*===========================================================================

FUNCTION  RRCIFREQ_VALIDATE_MEAS_REPORT

DESCRIPTION

 This function validates the measurement report.

DEPENDENCIES

 None

RETURN VALUE

 True if validation successful
 False if validation fails

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcifreq_validate_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
)
{
  uint8 cnt = 0, count = 0, cell_number = 0, i = 0;

  if ((l1_meas_report_ptr->rpt_crit != TDSL1_PERIODIC_RPT) &&
    (l1_meas_report_ptr->rpt_crit != TDSL1_EVENT_TRIGGER_RPT))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq: Invalid Rpt Crit %d",l1_meas_report_ptr->rpt_crit);
    return FALSE;
  }


  if (l1_meas_report_ptr->rpt_crit == TDSL1_EVENT_TRIGGER_RPT)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Validate Event Report");

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:No of Freq is %d",l1_meas_report_ptr->u.inter_f_rpt.event_results.num_freq);

    if ((l1_meas_report_ptr->u.inter_f_rpt.event_results.evt_id == TDSL1_INTER_FREQ_EVENT_2D) ||
      (l1_meas_report_ptr->u.inter_f_rpt.event_results.evt_id == TDSL1_INTER_FREQ_EVENT_2F))
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Event 2D/2F reported");
    }
    else
    {
      if (l1_meas_report_ptr->u.inter_f_rpt.event_results.num_freq == 0)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq: Invalid no of freq %d",l1_meas_report_ptr->u.inter_f_rpt.event_results.num_freq);
        return FALSE;
      }

      for (cnt = 0; (cnt < TDSL1_MAX_NON_USED_FREQ) && (cnt < l1_meas_report_ptr->u.inter_f_rpt.event_results.num_freq); cnt++)
      {
        if (tdsrrcifreq_validate_freq(&l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].freq_info) == FALSE)
        {
          return FALSE;
        }

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:No of cpi is %d",l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].num_cpi);

        if (l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].num_cpi == 0)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid no of cpid codes %d",l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].num_cpi);
          return FALSE;
        }
        for (count = 0; (count < TDSL1_MAX_NON_USED_FREQ) && (count < l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].num_cpi); count++)
        {
          if (l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].cpi[count] > TDSMAX_CPI)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq: Invalid %d CPI",l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].cpi[count]);
            return FALSE;
          }
          else
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"IFreq:cellparaid cnt %d is %d",count,l1_meas_report_ptr->u.inter_f_rpt.event_results.freq[cnt].cpi[count]);
        }
      }
    } /* End of else */
  } /* End of if TDSL1_EVENT_TRIGGER_RPT */

  if (l1_meas_report_ptr->u.inter_f_rpt.meas_results_included == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Meas Results not included");
    return TRUE;
  }

  /* Check for Measured Results */
  if ((l1_meas_report_ptr->u.inter_f_rpt.meas_results_included == TRUE) || (l1_meas_report_ptr->rpt_crit == TDSL1_PERIODIC_RPT))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Validate Measured Results");

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:No of msmt res is %d",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.num_msmt_results);

    if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.num_msmt_results != 0)
    {

      for (cnt = 0; (cnt < TDSL1_MAX_NON_USED_FREQ) && (cnt < l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.num_msmt_results); cnt++)
      {
        if ((l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
          msmt_results[cnt].utra_carrier_rssi_incl == TRUE) &&
          (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
          msmt_results[cnt].utra_carrier_rssi > TDSINTER_FREQ_RSSI))
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq: UTRAN RSSI value %d invalid",
            l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
            msmt_results[cnt].utra_carrier_rssi,0,0);
          return FALSE;

        }

        if (tdsrrcifreq_validate_freq(&l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
          msmt_results[cnt].freq_info) == FALSE)
        {
          return FALSE;
        }

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFreq:RSSI is %d, DFreq is %d", l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
          msmt_results[cnt].utra_carrier_rssi, l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
          msmt_results[cnt].freq_info.uarfcn_dl,0);

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Num Cells per Freq is %d",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].num_cells);

        if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].num_cells != 0)
        {
          /* Now validate Measured Results */
          for ( cell_number = 0; ((cell_number < l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].num_cells)
            && ( cell_number < TDSL1_MAX_CELL_MEAS)); cell_number++ )
          {
            if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_rpt_ind == TDSL1_NO_REPORT)
            {
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SFN SFN observed time diff not set");
            }
            else
            {
              /* Store sfn-sfn type indicator */
              if ( l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_rpt_ind == TDSL1_REPORT_TYPE_1)
              {
                if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_sfn_tm_diff >
                  TDSSFN_SFN_TYPE1_MAX_VALUE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Incorrect SFN-SFN Type1 value %d",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_sfn_tm_diff);

                  return FALSE;
                }
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"sfn-sfn type 2 selected");

                /* Type 2 report */
                if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_sfn_tm_diff >
                  TDSSFN_SFN_TYPE2_MAX_VALUE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Incorrect SFN-SFN Type2 value %d",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->sfn_sfn_tm_diff);

                  return FALSE;
                }
              }
            } /* End of else of TDSL1_NO_REPORT */

            /* Check if cell sync info present */

            if ( l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->cell_sync_info_incl == TRUE )
            {
              /* Set off value */
              if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->cell_sync_info.off > TDSOFF_MAX)
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFReq:Off value %d not in range",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->cell_sync_info.off);
                return FALSE;

              }
            } /* End of if of rrc_cellSynchronisationInfo) */

            /* Check primary scrambling code */
            if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->cpi > TDSMAX_CPI)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid CPID = %d",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->cpi);
              return FALSE;
            }

            /* Check if rscp included */
            if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->rscp_included == TRUE)
            {
              if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->rscp_val > TDSRSCP_MAX_VALUE)
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Incorrect attached RSCP value %d received",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->rscp_val);

                return FALSE;

              }
              if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
                && (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
                msmt_results[cnt].cell[cell_number]->delta_rscp_included == TRUE))
              {
                if((l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
                msmt_results[cnt].cell[cell_number]->delta_rscp > TDSDELTA_RSCP_MAX)
                  || (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
                msmt_results[cnt].cell[cell_number]->delta_rscp < TDSDELTA_RSCP_MIN))
                {
                   TDSRRC_MSG3(MSG_LEGACY_ERROR,"Delta RSCP not in range -5..-1 %d received",
                     l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.
                     msmt_results[cnt].cell[cell_number]->delta_rscp,0,0);
                   TDSRRC_MSG0(MSG_LEGACY_ERROR,"Trash Meas Report");
                   return FALSE;
                }
              }
            }

            /* Now add check to determine if ts_list_included too */
            if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                 cell[cell_number]->ts_list_included == TRUE)
            {
              //This code is to satify KW error only.
              for (i = 0; (i < L1_MAX_TS) && (i < l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].
                     cell[cell_number]->no_ts); i++)
              {
                if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->
                         iscp_ts_list[i].iscp_value > TDSISCP_MAX_VALUE)
                {
                  TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Incorrect attached ISCP value %d received",
                    l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->
                                 iscp_ts_list[i].iscp_value,0,0);
                  return  FALSE;
                }
              }
            }

            /* Now add check to determine if path loss included too */
            if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->pathloss_included == TRUE)
            {
              if ((l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->pathloss_val < TDSPATH_LOSS_MIN_VALUE) ||
                  (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->pathloss_val > TDSPATH_LOSS_MAX_VALUE))
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Incorrect attached Path Loss value %d received",l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[cnt].cell[cell_number]->pathloss_val);

                return  FALSE;

              }
            }

          } /* End of for loop of num cells */

        } /* No of cell != 0 */

      } /* End of for loop of num_msmt_res */

    } /* End of if num_msmt_results != 0 */

  } /* End of if measured results included  */
  return TRUE;
}


/*===========================================================================

FUNCTION  RRCIFREQ_VALIDATE_FREQ

DESCRIPTION

 This function validates downlink carrier frequency.


DEPENDENCIES

 None

RETURN VALUE

 True if validation successful
 False if validation fails

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcifreq_validate_freq
(
  tdsl1_freq_info_struct_type* freq_info_ptr
)
{
  if (freq_info_ptr->uarfcn_dl > TDS_ARFCN)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:DL UARFCN is %d",freq_info_ptr->uarfcn_dl);
    return FALSE;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:DL Freq is %d",freq_info_ptr->uarfcn_dl);
  }

  return TRUE;
}


/*===========================================================================

FUNCTION  RRCIFREQ_SET_INTER_FREQ_EVENT_ID

DESCRIPTION

 This function sets the event id as per ASN1 format from what L1 specified
 in the measurement report

DEPENDENCIES

 None

RETURN VALUE

 True if validation successful
 False if validation fails

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcifreq_set_inter_freq_event_id
(
 tdsl1_inter_freq_event_enum_type* event_id_ptr,
 tdsrrc_EventIDInterFreq* inter_freq_event_id_ptr
)
{
  /* Only 1 event is sent to UTRAN. So if L1 sends more than 1 event, neglect it*/

  TDSRRC_MSG1(MSG_LEGACY_LOW,"Value of event Id is : %d",*event_id_ptr);

  /* Find type of event and set it */
  switch ( *event_id_ptr )
  {
  case TDSL1_INTER_FREQ_EVENT_2A:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ifreq:TDSEVENT 2A reported");
    *inter_freq_event_id_ptr = tdsrrc_EventIDInterFreq_e2a;
    break;

  case TDSL1_INTER_FREQ_EVENT_2B:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:TDSEVENT 2B reported");
    *inter_freq_event_id_ptr = tdsrrc_EventIDInterFreq_e2b;

    break;

  case TDSL1_INTER_FREQ_EVENT_2C:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:TDSEVENT 2C reported");
    *inter_freq_event_id_ptr = tdsrrc_EventIDInterFreq_e2c;

    break;

  case TDSL1_INTER_FREQ_EVENT_2D:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:TDSEVENT 2D reported");
    *inter_freq_event_id_ptr = tdsrrc_EventIDInterFreq_e2d;

    break;

  case TDSL1_INTER_FREQ_EVENT_2E:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:TDSEVENT 2E reported");
    *inter_freq_event_id_ptr = tdsrrc_EventIDInterFreq_e2e;

    break;
  case TDSL1_INTER_FREQ_EVENT_2F:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:TDSEVENT 2F reported");
    *inter_freq_event_id_ptr = tdsrrc_EventIDInterFreq_e2f;

    break;
  default:
    /* Not possible as validation has already been done */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"L1 sent a wrong event ID.",*event_id_ptr);
    break;

  }  /* End of switch */
}

/*===========================================================================

FUNCTION   copy_sib11_inter_freq_cell_list_to_sib12

DESCRIPTION

    This function copies the inter freq cell list from SIB 11 nto the SIB12
    inter freq cell list ptr

DEPENDENCIES

    None

RETURN VALUE
   None


SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12
(
tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib11_ptr,
tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib12_ptr
)
{
  uint16 count;
  l1_inter_freq_sib12_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
  l1_inter_freq_sib12_ptr->num_cell_add =  l1_inter_freq_sib11_ptr->num_cell_add;

  for(count = 0; count < l1_inter_freq_sib11_ptr->num_cell_add; count ++)
  {
    l1_inter_freq_sib12_ptr->add_cell[count].cpi
     =  l1_inter_freq_sib11_ptr->add_cell[count].cpi;
    l1_inter_freq_sib12_ptr->add_cell[count].cpi_incl
     =  l1_inter_freq_sib11_ptr->add_cell[count].cpi_incl;
    l1_inter_freq_sib12_ptr->add_cell[count].measure_in_idle_pch_fach
     =  l1_inter_freq_sib11_ptr->add_cell[count].measure_in_idle_pch_fach;
    l1_inter_freq_sib12_ptr->add_cell[count].freq_info
     =  l1_inter_freq_sib11_ptr->add_cell[count].freq_info;
    l1_inter_freq_sib12_ptr->add_cell[count].cell_info
     =  l1_inter_freq_sib11_ptr->add_cell[count].cell_info;
    l1_inter_freq_sib12_ptr->add_cell[count].sec_freq_ind_incl
     =  l1_inter_freq_sib11_ptr->add_cell[count].sec_freq_ind_incl;
    l1_inter_freq_sib12_ptr->add_cell[count].sec_freq_ind
     =  l1_inter_freq_sib11_ptr->add_cell[count].sec_freq_ind;
  }
}

/*===========================================================================

FUNCTION   tdsrrcifreq_process_remove_inter_freq_cells

DESCRIPTION

    This function processes the inter freq remove cell list of SIB12 and updates
    the inter freq cell list variable.

DEPENDENCIES

    This function assumes that SIB12 inter freq removel cell list IE is present

RETURN VALUE
   None


SIDE EFFECTS

    None

===========================================================================*/
static void tdsrrcifreq_process_remove_inter_freq_cells
(
tdsrrc_RemovedInterFreqCellList *removedInterFreqCellList,
uint32 *num_rmv_cells
)
{
  uint32 inter_freq_cell_id;
  uint32 idx=0;
  *num_rmv_cells = 0;
  switch(removedInterFreqCellList->t)
  {
    case T_tdsrrc_RemovedInterFreqCellList_removeAllInterFreqCells:
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"remove all cells from inter freq cells list");
         *num_rmv_cells = TDSL1_MAX_CELL_MEAS;
         tdsrrcifreq_initialize_cell_list();
      break;

    case T_tdsrrc_RemovedInterFreqCellList_removeSomeInterFreqCells:
         if(removedInterFreqCellList->u.removeSomeInterFreqCells->n == 0)
         {
           TDSRRC_MSG0(MSG_LEGACY_LOW,"Assign no of removed cells to be 0");
         }
         else
         {
          
           do
           {
             inter_freq_cell_id = removedInterFreqCellList->u.removeSomeInterFreqCells->elem[idx];

             /* Remove cell at position given by the cell id from the inter freq cell info list */
             if(inter_freq_cell_id < TDSL1_MAX_CELL_MEAS)
             {
               tdsinter_freq_cell_list[inter_freq_cell_id].cpi = TDSINVALID_CPI;
               tdsinter_freq_cell_list[inter_freq_cell_id].cell_position = TDSVACANT;
               tdsinter_freq_cell_list[inter_freq_cell_id].dl_freq = TDSINVALID_INTER_FREQ;               
               tdsinter_freq_cell_list[inter_freq_cell_id].sec_freq_ind_incl = FALSE;
               tdsinter_freq_cell_list[inter_freq_cell_id].sec_freq_ind = FALSE;
               (*num_rmv_cells)++; /* Increment count */
             }

             /*  Get next element in the list */
             TDSRRC_MSG1(MSG_LEGACY_HIGH,"Removing cell id %d from the inter freq cell list",inter_freq_cell_id);
             idx++;

           } /* Continue if cond satisfied */
           while ((removedInterFreqCellList->u.removeSomeInterFreqCells->n > idx) &&
            ((*num_rmv_cells) < TDSL1_MAX_CELL_MEAS));
         }
      break;

    case T_tdsrrc_RemovedInterFreqCellList_removeNoInterFreqCells:
         /* no cells need to be removed */
         TDSRRC_MSG0(MSG_LEGACY_LOW,"Assign no of removed cells to be 0");
      break;

    default:
      /* cannot come here */
      break;
  }
}

/*===========================================================================

FUNCTION   tdsrrcifreq_build_composite_cell_list

DESCRIPTION

    This function builds the composite inter freq cell info list from SIB 11
    and SIB12.

DEPENDENCIES

    None

RETURN VALUE
   None


SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcifreq_build_composite_cell_list
(
tdsrrc_RemovedInterFreqCellList *removedInterFreqCellList,
tdsl1_inter_freq_cell_list_struct_type* sib11_cell_list_ptr,
tdsl1_inter_freq_cell_list_struct_type* sib12_cell_list_ptr,
uint32 *num_rmv_cells
)
{
/* Initialize all variables */

  /* Stores no of intra-freq cells stored in SIB11 */
  uint32 sib11_added_cells = 0;
  uint32 idy=0;
  /* Stores no of intra-freq cells stored in SIB12 */
  uint32 sib12_added_cells = 0;

  /* Counts no of cells in SIB12 */
  uint32 sib12_count_cells = 0;

  /* Counts removed cells matched against new intra freq cells of SIB11 */
  uint32 sib12_r_cell = 0;

  /* Counts no of new intra freq cells in SIB11 and helps in comparing
  * with no fo removed cells in SIB12 */
  uint32 sib11_a_cell = 0;

  /* If removed cell is found */
  uint32 match_found = 0;

  uint16 new_cell_list = 0;
  uint16 new_cell_list_from_sib11 = 0;
  uint16 count = 0;
  uint32 cell_id;



  /* Stores ptr to SIB12 list */
  tdsl1_inter_freq_cell_list_struct_type * new_cell_list_ptr = NULL;

  sib11_added_cells = sib11_cell_list_ptr->num_cell_add;

  if(removedInterFreqCellList->t == T_tdsrrc_RemovedInterFreqCellList_removeAllInterFreqCells)
  {
    /* No cells need to be picked up from SIB 11 */
    sib12_cell_list_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
    return;
  }

  /* Criteria: Match removed cell of SIB12 against all cells of SIB11
  * one at a time */

  for ( sib11_a_cell=0 ; sib11_a_cell<sib11_added_cells; sib11_a_cell++ )
  {
    
    for ( sib12_r_cell=0; sib12_r_cell < (*num_rmv_cells); sib12_r_cell++ )
    {
  
      if(removedInterFreqCellList->t == T_tdsrrc_RemovedInterFreqCellList_removeSomeInterFreqCells)
      {
        idy=0;
        if(removedInterFreqCellList->u.removeSomeInterFreqCells->n> idy)
        {
          if(tdstmp_inter_freq_cell_list[removedInterFreqCellList->u.removeSomeInterFreqCells->elem[idy]].cpi !=
            sib11_cell_list_ptr->add_cell[sib11_a_cell].cpi)
          {
            idy++;
            continue; /* Read next value of removed cell*/
          }
        }
        match_found = 1;
      }
      if(match_found == 1)
      {
         /* Do not add this cell to the composite list */
         break;
      }
    }  /* End of for for sib12_r_cell */

    if ( match_found == 0 )
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Copying SIB 11 cell back in new list");

      tdsrrcifreq_sib12_new_cell_list.add_cell[new_cell_list].measure_in_idle_pch_fach =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].measure_in_idle_pch_fach;

      tdsrrcifreq_sib12_new_cell_list.add_cell[new_cell_list].cpi_incl =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].cpi_incl;

      tdsrrcifreq_sib12_new_cell_list.add_cell[new_cell_list].cpi =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].cpi;

      tdsrrcifreq_sib12_new_cell_list.add_cell[new_cell_list].cell_info =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].cell_info;

      tdsrrcifreq_sib12_new_cell_list.add_cell[new_cell_list].freq_info =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].freq_info;
      
      tdsrrcifreq_sib12_new_cell_list.add_cell[new_cell_list].sec_freq_ind_incl =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].sec_freq_ind_incl;

      tdsrrcifreq_sib12_new_cell_list.add_cell[new_cell_list].sec_freq_ind =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].sec_freq_ind;

      /* Increment counter */
      new_cell_list++;

    } /* End of if */

    /* Reinitialize match_found variable */
    match_found = 0;

    /* Read next SIB 11 cell */

  } /* End of for sib11_a_cell */

  new_cell_list_ptr = &tdsrrcifreq_sib12_new_cell_list;
  new_cell_list_from_sib11 = new_cell_list;

  /* Check if SIB12 new intra freq cells have id same as the one already present
   * from SIB 11 if so overwrite, if not append to the list */
  for ( sib12_added_cells =0; sib12_added_cells < sib12_cell_list_ptr->num_cell_add && new_cell_list < TDSL1_MAX_CELL_MEAS;
  sib12_added_cells++ )
  {
    for(cell_id = 0; cell_id < TDSL1_MAX_CELL_MEAS; cell_id++)
    {
      if(tdsinter_freq_cell_list[cell_id].cpi == sib12_cell_list_ptr->add_cell[sib12_added_cells].cpi)
      {
        break;
      }
    }
    if((cell_id < TDSL1_MAX_CELL_MEAS) && (tdstmp_inter_freq_cell_list[cell_id].cpi != TDSINVALID_CPI))
    {
      for(count = 0; count < new_cell_list_from_sib11; count++)
      {
        if(tdstmp_inter_freq_cell_list[cell_id].cpi
          == new_cell_list_ptr->add_cell[count].cpi)
        {
          new_cell_list_ptr->add_cell[count].cpi =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].cpi;

          new_cell_list_ptr->add_cell[count].cpi_incl =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].cpi_incl;

          new_cell_list_ptr->add_cell[count].measure_in_idle_pch_fach =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].measure_in_idle_pch_fach;
          new_cell_list_ptr->add_cell[count].cell_info =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].cell_info;

          new_cell_list_ptr->add_cell[count].freq_info =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].freq_info;

          new_cell_list_ptr->add_cell[count].sec_freq_ind_incl =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].sec_freq_ind_incl;

          new_cell_list_ptr->add_cell[count].sec_freq_ind =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].sec_freq_ind;

          break;
        }
      }
      if(count == new_cell_list_from_sib11)
      {
        new_cell_list_ptr->add_cell[new_cell_list].cpi =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].cpi;

        new_cell_list_ptr->add_cell[new_cell_list].cpi_incl =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].cpi_incl;

         new_cell_list_ptr->add_cell[new_cell_list].measure_in_idle_pch_fach =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].measure_in_idle_pch_fach;
        new_cell_list_ptr->add_cell[new_cell_list].cell_info =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].cell_info;

        new_cell_list_ptr->add_cell[new_cell_list].freq_info =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].freq_info;
        
        new_cell_list_ptr->add_cell[new_cell_list].sec_freq_ind_incl =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].sec_freq_ind_incl;

        new_cell_list_ptr->add_cell[new_cell_list].sec_freq_ind =
            sib12_cell_list_ptr->add_cell[sib12_added_cells].sec_freq_ind;

        new_cell_list++;
      }
    }
    else
    {
      new_cell_list_ptr->add_cell[new_cell_list].measure_in_idle_pch_fach =
        sib12_cell_list_ptr->add_cell[sib12_added_cells].measure_in_idle_pch_fach;
      new_cell_list_ptr->add_cell[new_cell_list].cpi =
        sib12_cell_list_ptr->add_cell[sib12_added_cells].cpi;

      new_cell_list_ptr->add_cell[new_cell_list].cpi_incl =
        sib12_cell_list_ptr->add_cell[sib12_added_cells].cpi_incl;

      new_cell_list_ptr->add_cell[new_cell_list].cell_info =
        sib12_cell_list_ptr->add_cell[sib12_added_cells].cell_info;

      new_cell_list_ptr->add_cell[new_cell_list].freq_info =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].freq_info;

      new_cell_list_ptr->add_cell[new_cell_list].sec_freq_ind_incl =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].sec_freq_ind_incl;

      new_cell_list_ptr->add_cell[new_cell_list].sec_freq_ind =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].sec_freq_ind;

      new_cell_list++;
    }
  } /* End of for */

  /* Store the no of cells */
  sib12_cell_list_ptr->num_cell_add = new_cell_list;

  TDSRRC_MSG1(MSG_LEGACY_LOW,"There are %d cells in SIB12 cell list",new_cell_list);

  /* Now copy the structure in SIB12 */
  for ( sib12_count_cells =0; sib12_count_cells<new_cell_list && sib12_count_cells < TDSL1_MAX_CELL_MEAS;
  sib12_count_cells++ )
  {

    sib12_cell_list_ptr->add_cell[sib12_count_cells].cpi =
      new_cell_list_ptr->add_cell[sib12_count_cells].cpi;

    sib12_cell_list_ptr->add_cell[sib12_count_cells].cpi_incl =
      new_cell_list_ptr->add_cell[sib12_count_cells].cpi_incl;
    
    sib12_cell_list_ptr->add_cell[sib12_count_cells].measure_in_idle_pch_fach =
      new_cell_list_ptr->add_cell[sib12_count_cells].measure_in_idle_pch_fach;

    sib12_cell_list_ptr->add_cell[sib12_count_cells].cell_info =
      new_cell_list_ptr->add_cell[sib12_count_cells].cell_info;

     sib12_cell_list_ptr->add_cell[sib12_count_cells].freq_info =
      new_cell_list_ptr->add_cell[sib12_count_cells].freq_info;

     sib12_cell_list_ptr->add_cell[sib12_count_cells].sec_freq_ind_incl =
      new_cell_list_ptr->add_cell[sib12_count_cells].sec_freq_ind_incl;

     sib12_cell_list_ptr->add_cell[sib12_count_cells].sec_freq_ind =
      new_cell_list_ptr->add_cell[sib12_count_cells].sec_freq_ind;

  } /* End of for */

    /* Initialize no of removed cells to 0 as SIB12 intra-freq cell info list has
  * already taken them into account */
  sib12_cell_list_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
}

/*===========================================================================

FUNCTION   IFREQ_READ_SIB12

DESCRIPTION

    This function reads Inter Freq Cell List from SIB 12 and
    stores it in tdsinter_freq_meas_id_list. This Inter Freq
    Cell List is used for cell reselection. When UTRAN sends a meas
    in connected DCH mode, this cell list is overwritten.

DEPENDENCIES

    This function assumes that SIB12 is present and that the sib12_ptr
    != NULL

RETURN VALUE
   None


SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_read_sib12
(
 tdsrrc_SysInfoType12 *sib12_ptr,
 tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib11_ptr,
 tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib12_ptr
)
{

  tdsrrc_SysInfoType18 *sib18_ptr = NULL; /* Declared to store the value of SIB 18*/
  tdsrrc_PLMNsOfInterFreqCellsList *tmp_rrc_PLMNsOfInterFreqCellsList_ptr = NULL;
  rrc_plmn_identity_type tmp_plmn_identity;
  rrc_plmn_identity_type selected_plmn_identity;
  tdsrrc_int_cell_id_type  tmp_cell_id;
  boolean sib18_present = FALSE;
  uint32 idx=0;
  /* Local variables for storing value from SIB11 */
  tdsrrc_InterFreqMeasurementSysInfo_RSCP_LCR_r4 * inter_freq_meas_rscp_ptr = NULL;

  tdsrrc_InterFreqMeasurementSysInfo_ECN0_LCR_r4 * inter_freq_meas_ecno_ptr = NULL;

  tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4 * hcs_inter_freq_meas_rscp_ptr = NULL;

  tdsrrc_InterFreqMeasurementSysInfo_HCS_ECN0_LCR_r4 * hcs_inter_freq_meas_ecno_ptr = NULL;

  /* Ptrs to go thr linked list */
  tdsrrc_NewInterFreqCellSI_List_RSCP_LCR_r4 * rscp_cell_list_ptr = NULL;

  tdsrrc_NewInterFreqCellSI_List_ECN0_LCR_r4 * ecno_cell_list_ptr = NULL;

  tdsrrc_NewInterFreqCellSI_List_HCS_RSCP_LCR_r4 * hcs_rscp_cell_list_ptr = NULL;

  tdsrrc_NewInterFreqCellSI_List_HCS_ECN0_LCR_r4 * hcs_ecno_cell_list_ptr = NULL;

  tdsrrc_MeasurementControlSysInfo_LCR_r4_ext  * measurementControlSysInfo_lcr_ptr = NULL;

  tdsrrc_NewInterFreqCellList_v7b0ext          *sib12_rel7b0_ext_ptr = NULL;

  boolean sec_freq_ind = FALSE;
  
  boolean sec_freq_ind_incl = FALSE;

  uint32 count = 0;

  uint8 cell_count = 0;

  uint32 cell_position = 0;

  uint32 num_rmv_cells = 0;
  tdsrrc_SysInfoType12_v590ext_IEs *sib12_rel5_ext_ptr = NULL;
  tdsrrc_SysInfoType12_v590ext_IEs_newInterFrequencyCellInfoList_v590ext * rel5_ext_ptr = NULL;

#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
boolean rach_rpt_exists=FALSE;
struct tdsrrc_InterFreqRACHReportingInfo *interFreqRACHReportingInfo =NULL;
  tdsrrc_SysInfoType12_v6b0ext_IEs *sib12_rel6b0_ext_ptr = NULL;
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

  /* Initialize L1 params */
  l1_inter_freq_sib12_ptr->num_cell_add = 0;
  l1_inter_freq_sib12_ptr->num_cell_meas = 0;
  l1_inter_freq_sib12_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
  l1_inter_freq_sib12_ptr->cells_for_meas_incl = FALSE;
  l1_inter_freq_sib12_ptr->hcs_used = FALSE;
  tmp_plmn_identity.mcc[0] =  tmp_plmn_identity.mcc[1] = tmp_plmn_identity.mcc[2] = 0;
  tmp_plmn_identity.num_mnc_digits = 0;
  tmp_plmn_identity.mnc[0] =  tmp_plmn_identity.mnc[1] = 0;

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip Processing interFreq SIB12 measurements");
    return FAILURE;
  }

  if((sib18_ptr = (tdsrrc_SysInfoType18*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB18)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      /*
       1>if System Information Block type 11bis is scheduled on BCH:
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities for SIB 11bis"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Idle mode PLMN identities for SIB11bis";
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Connected mode PLMN identities"; 
       2>otherwise, 
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
       
       1> else
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
      */
    if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
          connectedModePLMNIdentitiesSIB11bis)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis,
          plmnsOfInterFreqCellsList)) &&
        (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
          connectedModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Conn Inter Freq from bis extension ");
      sib18_present = TRUE;
    }
    else if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
            (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, v6b0NonCriticalExtensions)) &&
            (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
              idleModePLMNIdentitiesSIB11bis)) &&
            (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
              plmnsOfInterFreqCellsList)) &&
            (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
          idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 idle Inter Freq from bis extension ");
      sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, connectedModePLMNIdentities)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities, plmnsOfInterFreqCellsList)) &&
      (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Conn Inter Freq");
      sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterFreqCellsList)) &&
      (sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter Freq");
      sib18_present = TRUE;
    }
#else
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterFreqCellsList))
      &&
      (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
    {
       tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB12 processing SIB18 Conn Inter Freq");
       sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterFreqCellsList))
      &&
      (sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB12 processing SIB18 Idle Inter Freq");
      sib18_present = TRUE;
    }
#endif
  }

  if(tdsrrcccm_get_serv_plmn_cellid(&tmp_cell_id, &selected_plmn_identity) == TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    sib18_present = FALSE;
	TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB18 not present", 0,0,0);
  }
  else
  {
    memscpy(&tmp_plmn_identity, sizeof(rrc_plmn_identity_type), &selected_plmn_identity, sizeof(rrc_plmn_identity_type));
  }

  if(tdsrrccsp_is_ue_in_true_limited_service() == TRUE)
  {
    sib18_present = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE in limited service ignoring SIB18");
  }
  {
    tdsrrc_MasterInformationBlock *mib_ptr = NULL;/* Declared to store the value of MIB*/
    if((mib_ptr = (tdsrrc_MasterInformationBlock*)  tdsrrcsibdb_return_sib_for_srv_cell
      (tdsrrc_MIB))!= NULL)
      {
        if(tdsrrc_is_nw_shared(mib_ptr))
        {
          sib18_present = FALSE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Ignore SIB18");
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: N/W not shader so consider SIB18");
        }
      }
  }

  for(count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
  {
    tdstmp_inter_freq_cell_list[count] = tdsinter_freq_cell_list[count];
  }

  /* Read SIB 12 parameters */
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
      /* Check for REL 5 extension ptr */
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib12_ptr,tdsrrc_SysInfoType12,v4b0NonCriticalExtensions)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(sib12_ptr->v4b0NonCriticalExtensions,
        tdsrrc_SysInfoType12_v4b0NonCriticalExtensions,v590NonCriticalExtension)))
      {
        sib12_rel5_ext_ptr = &(sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
          sysInfoType12_v590ext);
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib12_rel5_ext_ptr,
            tdsrrc_SysInfoType12_v590ext_IEs,newInterFrequencyCellInfoList_v590ext))
        {
          rel5_ext_ptr = &sib12_rel5_ext_ptr->newInterFrequencyCellInfoList_v590ext;
        }
      }
  }

#ifdef  FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib12_ptr,tdsrrc_SysInfoType12,v4b0NonCriticalExtensions)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE(sib12_ptr->v4b0NonCriticalExtensions,
      tdsrrc_SysInfoType12_v4b0NonCriticalExtensions,v590NonCriticalExtension)) &&
      (sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v690NonCriticalExtensionsPresent)&&
      (sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.m.
      v6b0NonCriticalExtensionsPresent))
    {
      sib12_rel6b0_ext_ptr = &(sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.v6b0NonCriticalExtensions.sysInfoType12_v6b0ext);
      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib12_rel6b0_ext_ptr,
        interFreqRACHReportingInfo))
      {
        interFreqRACHReportingInfo=&(sib12_rel6b0_ext_ptr->interFreqRACHReportingInfo);
        rach_rpt_exists = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter Frequency Rach Reporting info present in SIB12");
      }
    }
  }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
      if ((sib12_ptr->m.v4b0NonCriticalExtensionsPresent)
           && (sib12_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent)
            && (sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v690NonCriticalExtensionsPresent)
            && (sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
               v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent)
            && (sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
               v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v7b0NonCriticalExtensionsPresent))
      {
        if(sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.
              v6b0NonCriticalExtensions.v7b0NonCriticalExtensions.sysInfoType12_v7b0ext.m.newInterFreqCellListPresent)
        {
          sib12_rel7b0_ext_ptr = &sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                    v690NonCriticalExtensions.v6b0NonCriticalExtensions.v7b0NonCriticalExtensions.
                    sysInfoType12_v7b0ext.newInterFreqCellList;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"intraSecondaryFreqIndicator present in SIB12");
        }
      }
    }
  
  /* get cell info from "measurementControlSysInfo-LCR". */
  if ((sib12_ptr->m.v4b0NonCriticalExtensionsPresent) && 
         (sib12_ptr->v4b0NonCriticalExtensions.m.sysInfoType12_v4b0extPresent))
  {
    measurementControlSysInfo_lcr_ptr = 
      &sib12_ptr->v4b0NonCriticalExtensions.sysInfoType12_v4b0ext.measurementControlSysInfo_LCR;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"measurementControlSysInfo_LCR absent in sib12");
    /* Inter freq meas sys info absent in sib 12, use the meas sys info in sib11 */
    if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
      tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
      l1_inter_freq_sib12_ptr->hcs_used = l1_inter_freq_sib11_ptr->hcs_used;
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
      if(rach_rpt_exists)
      {
        (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
      }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
      return SUCCESS;
    }
    else
    {
      return FAILURE;
    }
  }
  
  /* Check if HCS is in use. */
  if ( measurementControlSysInfo_lcr_ptr->use_of_HCS.t ==
       T_tdsrrc_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:HCS value is TRUE");
    /* start processing HCS */
    if ( measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
      cellSelectQualityMeasure.t ==
      T_tdsrrc_MeasurementControlSysInfo_LCR_r4_ext_cellSelectQualityMeasure_1_cpich_RSCP )
    {
      /* The Cell Select Quality Measure is RSCP */
      if (!(measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
        cellSelectQualityMeasure.u.cpich_RSCP->m.interFreqMeasurementSysInfoPresent))
      {
        /* Optional param Inter freq meas sys RSCP info is absent */
        /* No need to initialize inter_freq_meas_info_incl as it is already set to FALSE */
        if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
          tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
          l1_inter_freq_sib12_ptr->hcs_used = TRUE;
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if(rach_rpt_exists)
          {
            (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
          return SUCCESS;
        }
        else
        {
          return FAILURE;
        }
      }

      /* Stores RSCP sys info in the following var */
      hcs_inter_freq_meas_rscp_ptr = &measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
        cellSelectQualityMeasure.u.cpich_RSCP->interFreqMeasurementSysInfo;
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hcs_inter_freq_meas_rscp_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4,interFreqCellInfoSI_List)))
      {
        if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq cell info list absent in SIB 12 pick it from SIB11 ");
          tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
          l1_inter_freq_sib12_ptr->hcs_used = TRUE;
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if(rach_rpt_exists)
          {
            (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
          return SUCCESS;
        }
        else
        {
          return FAILURE;
        }
      }

      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hcs_inter_freq_meas_rscp_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_HCS_RSCP_LCR,removedInterFreqCellList)))
      {
        tdsrrcifreq_process_remove_inter_freq_cells
          (&hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.removedInterFreqCellList, &num_rmv_cells);
      }

      /* Check inter-freq cell info present */
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hcs_inter_freq_meas_rscp_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_HCS_RSCP_LCR,newInterFreqCellList)) &&
        (hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n != 0))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Cell List present");

        /* Check "intraSecondaryFreqIndicator" in v7b0 */
        if ((sib12_rel7b0_ext_ptr != NULL) && 
            (sib12_rel7b0_ext_ptr->n != 
            hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n ))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell number mismatch in v7b0&v4b0");
          return FAILURE;
        }
        
        /* Always ignore the number of removed cells in System Info 11
        as there is nothing to delete initially */

        /*  Process the new cell list */

        cell_count = 0;

        /* Store the ptr in a local variable */
        hcs_rscp_cell_list_ptr = &hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList;

        while ( (hcs_rscp_cell_list_ptr->n > idx) &&
          (cell_count < TDSL1_MAX_CELL_MEAS) )
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max as per specs */

          /* Get the Freq Info */
          if (TDSRRC_MSG_LIST_BITMASK_IE(hcs_rscp_cell_list_ptr,
            tdsrrc_NewInterFreqCellSI_HCS_RSCP_LCR_r4,frequencyInfo))
          {
            if (hcs_rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
              T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. FDD cell");
              idx++;
              continue;
            }
            if (tdsrrc_validate_frequency_info(&hcs_rscp_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignored cell with dl freq %d",hcs_rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
             idx++;
              continue;
            }

            if (hcs_rscp_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",hcs_rscp_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
             idx++;
              continue;
            }

            /* Store DL freq */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) hcs_rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

            TDSRRC_MSG3(MSG_LEGACY_MED,"IFreq:DL Freq is %d", l1_inter_freq_sib12_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);

          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
             idx++;
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib12_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib12_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }
          /*Initial IE "intraSecondaryFreqIndicator" */
          l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind_incl = FALSE;
          /*Store "intraSecondaryFreqIndicator"*/
          if (sib12_rel7b0_ext_ptr != NULL)
          {
            sec_freq_ind_incl = TRUE;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind_incl = TRUE;
            
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind = 
                   sib12_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
            
            sec_freq_ind = sib12_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
          }
          else
          {
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind = FALSE;
          }
          /* Set default values for the HCS neighboring cell info if it is first cell
          * else use the info from the previous cell */
          if(cell_count == 0)
          {
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 115;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
          }
          else
          {
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio =
              l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.hcs_prio;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs =
              l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.q_hcs;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time =
              l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time;

            if(l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time
              != 0)
            {
              l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.tmp_offset1 =
                l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.tmp_offset1;
            }
          }
          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_hcs_rscp_cell_info(&hcs_rscp_cell_list_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");
            idx++;
            continue;
          }

          /* Check if inter-freq cell ID is present */
          if (TDSRRC_MSG_LIST_BITMASK_IE(hcs_rscp_cell_list_ptr,
            tdsrrc_NewInterFreqCellSI_HCS_RSCP_LCR_r4,interFreqCellID))
          {
            cell_position = hcs_rscp_cell_list_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */


          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl, &sec_freq_ind_incl,&sec_freq_ind) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib12_ptr->add_cell[cell_count].
              cell_info.cpi;

            l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi_incl =
              l1_inter_freq_sib12_ptr->add_cell[cell_count].
              cell_info.cpi_incl;
            
            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;

			/* set the default value of high-cost indicator: normal cell */
			l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.high_cost_cell = FALSE;
			
			/* set the high-cost indicator */

			  tdsrrcsibdb_find_cell_index_for_freq_cpid(
								&l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info,
								l1_inter_freq_sib12_ptr->add_cell[cell_count].freq_info.uarfcn_dl);
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"high cost flag - %d",l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.high_cost_cell);

            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */

          /* Get the next cell */
          idx++;
        } /* End of while */
        /* Set number of added cells */
        l1_inter_freq_sib12_ptr->num_cell_add = cell_count;

        if(!(TDSRRC_MSG_COMMON_BITMASK_IE(hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_HCS_RSCP_LCR,removedInterFreqCellList)))
        {
          hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.removedInterFreqCellList.t =
            T_tdsrrc_RemovedInterFreqCellList_removeNoInterFreqCells;
        }
        tdsrrcifreq_build_composite_cell_list(&hcs_inter_freq_meas_rscp_ptr->
          interFreqCellInfoSI_List.removedInterFreqCellList,
          l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr, &num_rmv_cells);

      } /* Inter freq cell info list is present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List absent");

        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hcs_inter_freq_meas_rscp_ptr,
          tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4,interFreqCellInfoSI_List)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE(hcs_inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_HCS_RSCP_LCR,removedInterFreqCellList)))
        {
          l1_inter_freq_sib12_ptr->num_cell_add = 0;
          tdsrrcifreq_build_composite_cell_list(&hcs_inter_freq_meas_rscp_ptr->
            interFreqCellInfoSI_List.removedInterFreqCellList,
            l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr, &num_rmv_cells);
        }
        else
        {
          if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
            tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12
              (l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
          }
        }
      }
      for(cell_count = 0; cell_count < l1_inter_freq_sib12_ptr->num_cell_add; cell_count++)
      {
        if(sib18_present)
        {
          if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
          {
            if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
            {
              tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                &tmp_plmn_identity);
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                if((tdsrrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                  tmp_plmn_identity))) == FALSE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CPID %d Non eq PLMN measure only in DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
                  /* set the flag to FALSE */
                  l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                }
              }
            }
            else
            {
              /* use the previous value of the flag if PLMN id is not selected PLMN */
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                if(cell_count > 0)
                {
                  l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                    l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                }
              }
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
            /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
          }
        }
        else
        {
          /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
          TDSRRC_MSG1(MSG_LEGACY_MED,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
        }
      }
    } /* End of if of RSCP chosen */
    else /* rrc_hcs_used_cellSelectQualityMeasure_cpich_Ec_N0 */
    {
      /* Store first the cell selection and reselection quality parameter */
      //rrc_inter_freq_sib_ptr->cell_sel_resel_quan_type =  TDSL1_EC_NO;

      if (!(measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
        cellSelectQualityMeasure.u.cpich_Ec_N0->m.interFreqMeasurementSysInfoPresent))
      {
        /* Optional param Inter freq meas sys ECN0 info is absent */
        if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
          tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
          l1_inter_freq_sib12_ptr->hcs_used = TRUE;
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if(rach_rpt_exists)
          {
            (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
          return SUCCESS;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Optional param Inter freq meas sys ECN0 info is not absent");
          return FAILURE;
        }
      }

      /* Stores EcNo sys info in the following var */
      hcs_inter_freq_meas_ecno_ptr = &measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
        cellSelectQualityMeasure.u.cpich_Ec_N0->interFreqMeasurementSysInfo;
      if(!(hcs_inter_freq_meas_ecno_ptr->m.interFreqCellInfoSI_ListPresent))
      {
        if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq cell info list absent in SIB 12 pick it from SIB11 ");
          tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
          l1_inter_freq_sib12_ptr->hcs_used = TRUE;
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if(rach_rpt_exists)
          {
            (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
          return SUCCESS;
        }
        else
        {
         
		  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq cell info list not absent in SIB 12 pick it from SIB11 ");
          return FAILURE;
        }
      }
      if((hcs_inter_freq_meas_ecno_ptr->m.interFreqCellInfoSI_ListPresent) &&
        (hcs_inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.m.removedInterFreqCellListPresent))
      {
        tdsrrcifreq_process_remove_inter_freq_cells
          (&hcs_inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.removedInterFreqCellList, &num_rmv_cells);
      }

      /* Check inter-freq cell info present */
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hcs_inter_freq_meas_ecno_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_HCS_ECN0_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(hcs_inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_HCS_ECN0_LCR,newInterFreqCellList)) &&
        (hcs_inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n != 0))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List present");

        /* Check "intraSecondaryFreqIndicator" in v7b0 */
        if ((sib12_rel7b0_ext_ptr != NULL) && 
            (sib12_rel7b0_ext_ptr->n != 
            hcs_inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n ))
        {

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell number mismatch in v7b0&v4b0");
          return FAILURE;
        }

        /* Always ignore the number of removed cells in System Info 11
        as there is nothing to delete initially */

        /*  Process the new cell list */

        cell_count = 0;

        /* Store the ptr in a local variable */
        hcs_ecno_cell_list_ptr = &hcs_inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList;
        idx=0;
        while ( (hcs_ecno_cell_list_ptr->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS) )
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max that is allowed by specs */

          /* Get the Freq Info */
          if (TDSRRC_MSG_LIST_BITMASK_IE(hcs_ecno_cell_list_ptr,tdsrrc_NewInterFreqCellSI_HCS_ECN0_LCR_r4,frequencyInfo))
          {
            if (hcs_ecno_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
              T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent/FDD");
              idx++;
              continue;
            }
            if (tdsrrc_validate_frequency_info(&hcs_ecno_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignored cell with dl freq %d",hcs_ecno_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
              idx++;
              continue;
            }
            if (hcs_ecno_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",hcs_ecno_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
              idx++;
              continue;
            }

            /* Store DL freq */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) hcs_ecno_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

            TDSRRC_MSG3(MSG_LEGACY_MED,"IFreq:DL Freq is %d", l1_inter_freq_sib12_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);

          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
              idx++;
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib12_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib12_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }
          /* Initail IE 'intraSecondaryFreqIndicator' */
          l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind_incl = FALSE;
          /*Store "intraSecondaryFreqIndicator"*/
          if (sib12_rel7b0_ext_ptr != NULL)
          {
            sec_freq_ind_incl = TRUE;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind_incl = TRUE;
            
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind = 
                   sib12_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
            
            sec_freq_ind = sib12_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
          }
          else
          {
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind = FALSE;
          }

          /* Downlink Freq Present */
          /* Set default values for the HCS neighboring cell info if it is first cell
          * else use the info from the previous cell */
          if(cell_count == 0)
          {
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 24 * 2;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
          }
          else
          {
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio =
              l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.hcs_prio;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs =
              l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.q_hcs;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time =
              l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time;

            if(l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time
              != 0)
            {
              l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.hcs_info.tmp_offset1 =
                l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].cell_info.hcs_info.tmp_offset1;
            }
          }
          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_hcs_ecn0_cell_info(&hcs_ecno_cell_list_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");
            idx++;
            continue;
          }

          /* Check if inter-freq cell ID is present */
          if (TDSRRC_MSG_LIST_BITMASK_IE(hcs_ecno_cell_list_ptr,
            tdsrrc_NewInterFreqCellSI_HCS_ECN0_LCR_r4,interFreqCellID))
          {
            cell_position = hcs_ecno_cell_list_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */

          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl, &sec_freq_ind_incl,&sec_freq_ind) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib12_ptr->add_cell[cell_count].
              cell_info.cpi;
            
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi_incl =
              l1_inter_freq_sib12_ptr->add_cell[cell_count].
              cell_info.cpi_incl;
            
            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
			/* set the default value of high-cost indicator: normal cell */
			l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.high_cost_cell = FALSE;
			
			/* set the high-cost indicator */

			  tdsrrcsibdb_find_cell_index_for_freq_cpid(
								&l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info,
								l1_inter_freq_sib12_ptr->add_cell[cell_count].freq_info.uarfcn_dl);
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"high cost flag - %d",l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.high_cost_cell);

            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */

          /* Get the next cell */
          idx++;
        } /* End of while */
         /* Set number of added cells */
        l1_inter_freq_sib12_ptr->num_cell_add = cell_count;

        if(!(TDSRRC_MSG_COMMON_BITMASK_IE(hcs_inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_HCS_ECN0_LCR,removedInterFreqCellList)))
        {
          hcs_inter_freq_meas_ecno_ptr->
          interFreqCellInfoSI_List.removedInterFreqCellList.t = T_tdsrrc_RemovedInterFreqCellList_removeNoInterFreqCells;
        }
        tdsrrcifreq_build_composite_cell_list(&hcs_inter_freq_meas_ecno_ptr->
          interFreqCellInfoSI_List.removedInterFreqCellList,
          l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr, &num_rmv_cells);

      } /* Inter freq cell info list is present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List absent");
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hcs_inter_freq_meas_ecno_ptr,
          tdsrrc_InterFreqMeasurementSysInfo_HCS_ECN0_LCR_r4,interFreqCellInfoSI_List)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE(hcs_inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_HCS_ECN0_LCR,removedInterFreqCellList)))
        {
          l1_inter_freq_sib12_ptr->num_cell_add = 0;
          tdsrrcifreq_build_composite_cell_list(&hcs_inter_freq_meas_ecno_ptr->
            interFreqCellInfoSI_List.removedInterFreqCellList,
            l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr, &num_rmv_cells);
        }
        else
        {
          if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
            tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12
              (l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
          }
        }
      }
      for(cell_count = 0; cell_count < l1_inter_freq_sib12_ptr->num_cell_add; cell_count++)
      {
        if(sib18_present)
        {
          if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
          {
            if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
            {
              tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                &tmp_plmn_identity);
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                if((tdsrrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                  tmp_plmn_identity))) == FALSE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CPID %d Non eq PLMN measure only in DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
                  /* set the flag to FALSE */
                  l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                }
              }
            }
            else
            {
              /* use the previous value of the flag if PLMN id is not selected PLMN */
             if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
             {
                /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                if(cell_count > 0)
                {
                  l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                    l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                }
              }
            }

            
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
            /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
          }
        }
        else
        {
          /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
          TDSRRC_MSG1(MSG_LEGACY_MED,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
        }
      }
    } /* End of else of EcNo chosen */
    /* end processing HCS */
    l1_inter_freq_sib12_ptr->hcs_used = TRUE;
  }
  else
  { /* HCS is NOT in use */
    /* Set HCS flag to FALSE */

    if ( measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
      cellSelectQualityMeasure.t ==
      T_tdsrrc_MeasurementControlSysInfo_LCR_r4_ext_cellSelectQualityMeasure_cpich_RSCP )
    {
      /* The Cell Select Quality Measure is RSCP */
      if (!(measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
        cellSelectQualityMeasure.u.cpich_RSCP->m.
        interFreqMeasurementSysInfoPresent))
      {
        /* Optional param Inter freq meas sys RSCP info is absent */
        /* No need to initialize inter_freq_meas_info_incl as it is already set to FALSE */
        if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
          tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if(rach_rpt_exists)
          {
            (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
          return SUCCESS;
        }
        else
        {
          return FAILURE;
        }
      }

      /* Stores RSCP sys info in the following var */
      inter_freq_meas_rscp_ptr = &measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
        cellSelectQualityMeasure.u.cpich_RSCP->interFreqMeasurementSysInfo;
      if(!(inter_freq_meas_rscp_ptr->m.interFreqCellInfoSI_ListPresent))
      {
        if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq cell info list absent in SIB 12 pick it from SIB11 ");
          tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if(rach_rpt_exists)
          {
            (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
          return SUCCESS;
        }
        else
        {
          return FAILURE;
        }
      }

      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_rscp_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_RSCP_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_RSCP_LCR,removedInterFreqCellList)))
      {
        tdsrrcifreq_process_remove_inter_freq_cells
          (&inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.removedInterFreqCellList, &num_rmv_cells);
      }

      /* Check inter-freq cell info present */
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_rscp_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_RSCP_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_RSCP_LCR,newInterFreqCellList)) &&
        (inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n != 0))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Cell List present");

        /* Check "intraSecondaryFreqIndicator" in v7b0 */
        if ((sib12_rel7b0_ext_ptr != NULL) && 
            (sib12_rel7b0_ext_ptr->n != 
            inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n ))
        {

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell number mismatch in v7b0&v4b0");
          return FAILURE;
        }

        /* Always ignore the number of removed cells in System Info 11
        as there is nothing to delete initially */

        /*  Process the new cell list */

        cell_count = 0;

        /* Store the ptr in a local variable */
        rscp_cell_list_ptr = &inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList;
        idx=0;
        while ( (rscp_cell_list_ptr->n > idx) &&
          (cell_count < TDSL1_MAX_CELL_MEAS) )
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max as per specs */

          /* Get the Freq Info */
          if (TDSRRC_MSG_LIST_BITMASK_IE(rscp_cell_list_ptr,tdsrrc_NewInterFreqCellSI_RSCP,frequencyInfo))
          {
            if (rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
              T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. FDD cell");
              idx++;
              continue;
            }
            if (tdsrrc_validate_frequency_info(&rscp_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignored cell with dl freq %d",rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
              idx++;
              continue;
            }

            if (rscp_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",rscp_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
              idx++;
              continue;
            }

            /* Store DL freq */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

            TDSRRC_MSG3(MSG_LEGACY_MED,"IFreq:DL Freq is %d", l1_inter_freq_sib12_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);

          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
              idx++;
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib12_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib12_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }

          /* Initial IE 'intraSecondaryFreqIndicator' */
          l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind_incl = FALSE;
          /*Store "intraSecondaryFreqIndicator"*/
          if (sib12_rel7b0_ext_ptr != NULL)
          {
            sec_freq_ind_incl = TRUE;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind_incl = TRUE;
            
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind = 
                   sib12_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
            
            sec_freq_ind = sib12_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
          }
          else
          {
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind = FALSE;
          }

          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_rscp_cell_info(&rscp_cell_list_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");
            idx++;
            continue;
          }

          /* Check if inter-freq cell ID is present */
          if (TDSRRC_MSG_LIST_BITMASK_IE(rscp_cell_list_ptr,
            tdsrrc_NewInterFreqCellSI_RSCP_LCR_r4,interFreqCellID))
          {
            cell_position = rscp_cell_list_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */


          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl, &sec_freq_ind_incl,&sec_freq_ind) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib12_ptr->add_cell[cell_count].
              cell_info.cpi;
            
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi_incl =
                          l1_inter_freq_sib12_ptr->add_cell[cell_count].
                          cell_info.cpi_incl;

            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;

			/* set the default value of high-cost indicator: normal cell */
			l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.high_cost_cell = FALSE;
			
			/* set the high-cost indicator */

			  tdsrrcsibdb_find_cell_index_for_freq_cpid(
								&l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info,
								l1_inter_freq_sib12_ptr->add_cell[cell_count].freq_info.uarfcn_dl);
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"high cost flag - %d",l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.high_cost_cell);

            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */

          /* Get the next cell */
          idx++;
        } /* End of while */
        /* Set number of added cells */
        l1_inter_freq_sib12_ptr->num_cell_add = cell_count;

        if(!(TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_RSCP_LCR,removedInterFreqCellList)))
        {
          inter_freq_meas_rscp_ptr->
          interFreqCellInfoSI_List.removedInterFreqCellList.t = T_tdsrrc_RemovedInterFreqCellList_removeNoInterFreqCells;
        }
        tdsrrcifreq_build_composite_cell_list(&inter_freq_meas_rscp_ptr->
          interFreqCellInfoSI_List.removedInterFreqCellList,
          l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr, &num_rmv_cells);
      } /* Inter freq cell info list is present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List absent");

        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_rscp_ptr,
          tdsrrc_InterFreqMeasurementSysInfo_RSCP_LCR_r4,interFreqCellInfoSI_List)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_RSCP_LCR,removedInterFreqCellList)))
        {
          l1_inter_freq_sib12_ptr->num_cell_add = 0;
          tdsrrcifreq_build_composite_cell_list(&inter_freq_meas_rscp_ptr->
            interFreqCellInfoSI_List.removedInterFreqCellList,
            l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr, &num_rmv_cells);
        }
        else
        {
          if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
            tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12
              (l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
          }
        }
      }
      for(cell_count = 0; cell_count < l1_inter_freq_sib12_ptr->num_cell_add; cell_count++)
      {
        if(sib18_present)
        {
          if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
          {
            if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
            {
              tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                &tmp_plmn_identity);
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                if((tdsrrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                  tmp_plmn_identity))) == FALSE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d Non eq PLMN measure only in DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
                  /* set the flag to FALSE */
                  l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                }
              }
            }
            else
            {
              /* use the previous value of the flag if PLMN id is not selected PLMN */
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                if(cell_count > 0) /* to satisfy lint */
                {
                  l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                    l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                }
              }
            }

          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
            /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi%d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
          }
        }
        else
        {
          /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
          TDSRRC_MSG1(MSG_LEGACY_MED,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
        }
      }
    } /* End of if of RSCP chosen */
    else /* rrc_hcs_not_used_cellSelectQualityMeasure_cpich_Ec_N0 */
    {
      /* Store first the cell selection and reselection quality parameter */
      //rrc_inter_freq_sib_ptr->cell_sel_resel_quan_type =  TDSL1_EC_NO;

      if (!(measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
        cellSelectQualityMeasure.u.cpich_Ec_N0->m.interFreqMeasurementSysInfoPresent))
      {
        /* Optional param Inter freq meas sys ECN0 info is absent */
        if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
          tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if(rach_rpt_exists)
          {
            (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
          return SUCCESS;
        }
        else
        {
          return FAILURE;
        }
      }

      /* Stores EcNo sys info in the following var */
      inter_freq_meas_ecno_ptr = &measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
        cellSelectQualityMeasure.u.cpich_Ec_N0->interFreqMeasurementSysInfo;
      if(!(inter_freq_meas_ecno_ptr->m.interFreqCellInfoSI_ListPresent))
      {
        if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter freq cell info list absent in SIB 12 pick it from SIB11 ");
          tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12(l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if(rach_rpt_exists)
          {
            (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
          return SUCCESS;
        }
        else
        {
          return FAILURE;
        }
      }
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_ecno_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_ECN0_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_ECN0_LCR,removedInterFreqCellList)))
      {
        tdsrrcifreq_process_remove_inter_freq_cells
          (&inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.removedInterFreqCellList, &num_rmv_cells);
      }

      /* Check inter-freq cell info present */
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_ecno_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_ECN0_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_ECN0_LCR,newInterFreqCellList)) &&
        (inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n != 0))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List present");

        /* Check "intraSecondaryFreqIndicator" in v7b0 */
        if ((sib12_rel7b0_ext_ptr != NULL) && 
            (sib12_rel7b0_ext_ptr->n != 
            inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n ))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell number mismatch in v7b0&v4b0");
          return FAILURE;
        }

        /* Always ignore the number of removed cells in System Info 11
        as there is nothing to delete initially */

        /*  Process the new cell list */

        cell_count = 0;
        idx=0;
        /* Store the ptr in a local variable */
        ecno_cell_list_ptr = &inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList;

        while ( (ecno_cell_list_ptr->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS) )
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max that is allowed by specs */

          /* Get the Freq Info */
          if (TDSRRC_MSG_LIST_BITMASK_IE(ecno_cell_list_ptr,tdsrrc_NewInterFreqCellSI_ECN0,frequencyInfo))
          {
            if (ecno_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
              T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent/FDD");
             idx++;
              continue;
            }
            if (tdsrrc_validate_frequency_info(&ecno_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignored cell with dl freq %d",ecno_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
              idx++;
              continue;
            }
            if (ecno_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",ecno_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
              idx++;
              continue;
            }

            /* Store DL freq */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) ecno_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

            TDSRRC_MSG3(MSG_LEGACY_MED,"IFreq:DL Freq is %d", l1_inter_freq_sib12_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);

          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
              idx++;
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib12_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib12_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }

          /* Initial IE 'intraSecondaryFreqIndicator' */
          l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind_incl = FALSE;
          /*Store "intraSecondaryFreqIndicator"*/
          if (sib12_rel7b0_ext_ptr != NULL)
          {
            sec_freq_ind_incl = TRUE;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind_incl = TRUE;
            
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind = 
                   sib12_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;

            sec_freq_ind = sib12_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
          }
          else
          {
            l1_inter_freq_sib12_ptr->add_cell[cell_count].sec_freq_ind = FALSE;
          }

          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_ecn0_cell_info(&ecno_cell_list_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");
            idx++;
            continue;
          }

          /* Check if inter-freq cell ID is present */
          if (TDSRRC_MSG_LIST_BITMASK_IE(ecno_cell_list_ptr,
            tdsrrc_NewInterFreqCellSI_ECN0_LCR_r4,interFreqCellID))
          {
            cell_position = ecno_cell_list_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */

          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib12_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl, &sec_freq_ind_incl,&sec_freq_ind) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib12_ptr->add_cell[cell_count].
              cell_info.cpi;
            l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi_incl =
              l1_inter_freq_sib12_ptr->add_cell[cell_count].
              cell_info.cpi_incl;

            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;

			/* set the default value of high-cost indicator: normal cell */
			l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.high_cost_cell = FALSE;
			
			/* set the high-cost indicator */

			  tdsrrcsibdb_find_cell_index_for_freq_cpid(
								&l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info,
								l1_inter_freq_sib12_ptr->add_cell[cell_count].freq_info.uarfcn_dl);
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"high cost flag - %d",l1_inter_freq_sib12_ptr->add_cell[cell_count].cell_info.high_cost_cell);

            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */

          /* Get the next cell */
          idx++;
        } /* End of while */
         /* Set number of added cells */
        l1_inter_freq_sib12_ptr->num_cell_add = cell_count;

        if(!(TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_ECN0_LCR,removedInterFreqCellList)))
        {
          inter_freq_meas_ecno_ptr->
          interFreqCellInfoSI_List.removedInterFreqCellList.t = T_tdsrrc_RemovedInterFreqCellList_removeNoInterFreqCells;
        }
        tdsrrcifreq_build_composite_cell_list(&inter_freq_meas_ecno_ptr->
          interFreqCellInfoSI_List.removedInterFreqCellList,
          l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr, &num_rmv_cells);

      } /* Inter freq cell info list is present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List absent");
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_ecno_ptr,
          tdsrrc_InterFreqMeasurementSysInfo_ECN0_LCR_r4,interFreqCellInfoSI_List)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_ECN0_LCR,removedInterFreqCellList)))
        {
          l1_inter_freq_sib12_ptr->num_cell_add = 0;
          tdsrrcifreq_build_composite_cell_list(&inter_freq_meas_ecno_ptr->
            interFreqCellInfoSI_List.removedInterFreqCellList,
            l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr, &num_rmv_cells);
        }
        else
        {
          if(l1_inter_freq_sib11_ptr->num_cell_add != 0)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter freq meas sys info absent in SIB 12 pick it from SIB11 ");
            tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12
              (l1_inter_freq_sib11_ptr, l1_inter_freq_sib12_ptr);
          }
        }
      }
      for(cell_count = 0; cell_count < l1_inter_freq_sib12_ptr->num_cell_add; cell_count++)
      {
        if(sib18_present)
        {
          if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
          {
            if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
            {
              tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                &tmp_plmn_identity);
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                if((tdsrrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                  tmp_plmn_identity))) == FALSE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CPID %d Non eq PLMN measure only in DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
                  /* set the flag to FALSE */
                  l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                }
              }
            }
            else
            {
            /* use the previous value of the flag if PLMN id is not selected PLMN */
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                /* Add code to set the flag to the flag value of the previous cell */
                if(cell_count > 0) /* to satisfy lint */
                {
                  l1_inter_freq_sib12_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                    l1_inter_freq_sib12_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                }
              }
            }

            
          }
          else
          {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
            /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
          }
        }
        else
        {
          /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
          TDSRRC_MSG1(MSG_LEGACY_MED,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib12_ptr->add_cell[cell_count].cpi);
        }
      }
    } /* End of else of EcNo chosen */
  } /* Else of t is set to 'not hcs' */
  if(l1_inter_freq_sib12_ptr->num_cell_add != 0)
  {
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
      if(rach_rpt_exists)
      {
        (void)tdsrrcifreq_read_rach_rpt_info_sib12(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
      }
    }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
}

/*===========================================================================

FUNCTION   IFREQ_READ_SIB11

DESCRIPTION

    This function reads Inter Freq Cell List from SIB 11 and
    stores it in tdsinter_freq_meas_id_list. This Inter Freq
    Cell List is used for cell reselection. When UTRAN sends a meas
    in connected DCH mode, this cell list is overwritten.

DEPENDENCIES

    -

RETURN VALUE



SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_read_sib11
(
 tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib_ptr
)
{
  tdsrrc_SysInfoType11 *sib11_ptr = NULL; /* Declared to store the value of SIB 11*/
  tdsrrc_SysInfoType18 *sib18_ptr = NULL; /* Declared to store the value of SIB 18*/
  tdsrrc_PLMNsOfInterFreqCellsList *tmp_rrc_PLMNsOfInterFreqCellsList_ptr = NULL;

  boolean sib18_present = FALSE;
  boolean sec_freq_ind = FALSE;  
  boolean sec_freq_ind_incl = FALSE;
  rrc_plmn_identity_type tmp_plmn_identity;
  rrc_plmn_identity_type selected_plmn_identity;
  tdsrrc_int_cell_id_type  tmp_cell_id;

  /* Local variables for storing value from SIB11 */
  tdsrrc_InterFreqMeasurementSysInfo_RSCP_LCR_r4 * inter_freq_meas_rscp_ptr = NULL;

  tdsrrc_InterFreqMeasurementSysInfo_ECN0_LCR_r4 * inter_freq_meas_ecno_ptr = NULL;

  /* Ptrs to go thr linked list */
  tdsrrc_NewInterFreqCellSI_List_RSCP_LCR_r4 * rscp_cell_list_ptr = NULL;

  tdsrrc_NewInterFreqCellSI_List_ECN0_LCR_r4 * ecno_cell_list_ptr = NULL;

  tdsrrc_MeasurementControlSysInfo_LCR_r4_ext  * measurementControlSysInfo_lcr_ptr = NULL;

  tdsrrc_NewInterFreqCellList_v7b0ext          *sib11_rel7b0_ext_ptr = NULL;
  uint32 idx=0;
  uint8 cell_count = 0;

  uint32 cell_position = 0;

  tdsrrc_SysInfoType11_v590ext_IEs *sib11_rel5_ext_ptr = NULL;
  tdsrrc_SysInfoType11_v590ext_IEs_newInterFrequencyCellInfoList_v590ext * rel5_ext_ptr = NULL;
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
  boolean rach_rpt_exists=FALSE;
  struct tdsrrc_InterFreqRACHReportingInfo *interFreqRACHReportingInfo =NULL;
  tdsrrc_SysInfoType11_v6b0ext_IEs *sib11_rel6b0_ext_ptr = NULL;
  tdsinterf_rach_report_crit = TDSNO_RRC_MEAS_RACH_REPORT;
  tdsrrcifreq_inter_freq_rach_rpt.inter_freq_rach_info_incl = FALSE;
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  uint16 prev_cell_count =0;
#endif

  sib11_ptr = (tdsrrc_SysInfoType11*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB11);

  /* Initialize L1 params */
  l1_inter_freq_sib_ptr->num_cell_add = 0;
  l1_inter_freq_sib_ptr->num_cell_meas = 0;
  l1_inter_freq_sib_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
  l1_inter_freq_sib_ptr->cells_for_meas_incl = FALSE;
  l1_inter_freq_sib_ptr->hcs_used = FALSE;
  tmp_plmn_identity.mcc[0] =  tmp_plmn_identity.mcc[1] = tmp_plmn_identity.mcc[2] = 0;
  tmp_plmn_identity.num_mnc_digits = 0;
  tmp_plmn_identity.mnc[0] =  tmp_plmn_identity.mnc[1] = 0;

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip Processing interFreq SIB11 measurements");
    return FAILURE;
  }

  if((sib18_ptr = (tdsrrc_SysInfoType18*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB18)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)))
    {
      /*
       1>if System Information Block type 11bis is scheduled on BCH:
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities for SIB 11bis"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Idle mode PLMN identities for SIB11bis";
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Connected mode PLMN identities"; 
       2>otherwise, 
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
       
       1> else
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
      */
      if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, v6b0NonCriticalExtensions)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
            connectedModePLMNIdentitiesSIB11bis)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis,
            plmnsOfInterFreqCellsList)) &&
          (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList.n= 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            connectedModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Conn Inter Freq from bis extension ");
        sib18_present = TRUE;
      }
      else if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
              (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, v6b0NonCriticalExtensions)) &&
              (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
                idleModePLMNIdentitiesSIB11bis)) &&
              (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
                plmnsOfInterFreqCellsList)) &&
              (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList.n= 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 idle Inter Freq from bis extension ");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, connectedModePLMNIdentities)) &&
              (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities, plmnsOfInterFreqCellsList)) &&
              (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList.n= 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Conn Inter Freq");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, idleModePLMNIdentities)) &&
              (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities, plmnsOfInterFreqCellsList)) &&
              (sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList.n= 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter Freq");
        sib18_present = TRUE;
      }
    }
    else
    {
   
      /*
       If SIB11bis is present and SIB18 bis extenstions are present, use SIB18 bis extension
       If SIB11bis is present and SIb18 bis extensions are absent, use SIB18
       If SIB11bis is absent, use SIB18
      */
      if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, v6b0NonCriticalExtensions)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
            idleModePLMNIdentitiesSIB11bis)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
            plmnsOfInterFreqCellsList)) &&
          (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Idle Inter Freq from bis extension ");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, idleModePLMNIdentities)) &&
              (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterFreqCellsList)) &&
              (sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter Freq ");
        sib18_present = TRUE;
      }
    }
#else
    if(((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH) || 
        (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH) || 
        (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)
        ) && 
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities))  &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterFreqCellsList)) &&
        (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Conn Inter Freq");
      sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities)) &&
            (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterFreqCellsList))  &&
            (sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter Freq");
      sib18_present = TRUE;
    }
#endif

  }

  if(tdsrrcccm_get_serv_plmn_cellid(&tmp_cell_id, &selected_plmn_identity) == TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    sib18_present = FALSE;
	TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB18 not present", 0,0,0);
  }
  else
  {
    memscpy(&tmp_plmn_identity, sizeof(rrc_plmn_identity_type), &selected_plmn_identity, sizeof(rrc_plmn_identity_type));
  }


  if(tdsrrccsp_is_ue_in_true_limited_service() == TRUE)
  {
    sib18_present = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE in limited service ignoring SIB18");
  }

  /* Check if the value received is O.K. */
  {
    tdsrrc_MasterInformationBlock *mib_ptr = NULL;/* Declared to store the value of MIB*/
    if((mib_ptr = (tdsrrc_MasterInformationBlock*)  tdsrrcsibdb_return_sib_for_srv_cell
      (tdsrrc_MIB))!= NULL)
    {
      if(tdsrrc_is_nw_shared(mib_ptr))
      {
        sib18_present = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Ignore SIB18");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: N/W not shader so consider SIB18");
      }
    }
  }

  if ( sib11_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SIB 11 from SIB DB is NULL");

    return FAILURE;
  }
  else
  {
    /* Read SIB 11 parameters */
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
    {
      /* Check for REL 5 extension ptr */
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib11_ptr,tdsrrc_SysInfoType11,v4b0NonCriticalExtensions)) &&
        (sib11_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent))
      {
        sib11_rel5_ext_ptr = &(sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
          sysInfoType11_v590ext);
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib11_rel5_ext_ptr,
            tdsrrc_SysInfoType11_v590ext_IEs,newInterFrequencyCellInfoList_v590ext))
        {
          rel5_ext_ptr = &sib11_rel5_ext_ptr->newInterFrequencyCellInfoList_v590ext;
        }
      }
    }
#ifdef  FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
      if((sib11_ptr->m.v4b0NonCriticalExtensionsPresent) &&
        (sib11_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent) &&
        (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v690NonCriticalExtensionsPresent)&&
        (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.m.
        v6b0NonCriticalExtensionsPresent))
      {
        sib11_rel6b0_ext_ptr = &(sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.v6b0NonCriticalExtensions.sysInfoType11_v6b0ext);
        if(sib11_rel6b0_ext_ptr->m.interFreqRACHReportingInfoPresent)
        {
          interFreqRACHReportingInfo=&(sib11_rel6b0_ext_ptr->interFreqRACHReportingInfo);
          rach_rpt_exists = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter Frequency Rach Reporting info present in SIB11");
        }
      }
    }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
      if ((sib11_ptr->m.v4b0NonCriticalExtensionsPresent)
           && (sib11_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent)
            && (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v690NonCriticalExtensionsPresent)
            && (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
               v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent)
            && (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
               v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v770NonCriticalExtensionsPresent)
           && (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.
                v6b0NonCriticalExtensions.v770NonCriticalExtensions.m.v7b0NonCriticalExtensionsPresent))
      {
        if(sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.
                v6b0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
                     sysInfoType11_v7b0ext.m.newInterFreqCellListPresent)
        {
          sib11_rel7b0_ext_ptr = &sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                           v690NonCriticalExtensions.v6b0NonCriticalExtensions.v770NonCriticalExtensions.
                            v7b0NonCriticalExtensions.sysInfoType11_v7b0ext.newInterFreqCellList;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"intraSecondaryFreqIndicator present in SIB11");
        }
      }
    }


    /* get cell info from "measurementControlSysInfo-LCR". */
    if ((sib11_ptr->m.v4b0NonCriticalExtensionsPresent) && 
          (sib11_ptr->v4b0NonCriticalExtensions.m.sysInfoType11_v4b0extPresent))
    {
      measurementControlSysInfo_lcr_ptr = 
        &sib11_ptr->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"measurementControlSysInfo_LCR absent in sib11");
      return FAILURE;
    }
    /* Check if HCS is in use. */
    if ( measurementControlSysInfo_lcr_ptr->use_of_HCS.t ==
         T_tdsrrc_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:HCS value is TRUE");
      l1_inter_freq_sib_ptr->hcs_used = TRUE;
      if ((tdsrrcifreq_fill_hcs_info(l1_inter_freq_sib_ptr, 
        (tdsrrc_SysInfoType12_v590ext_IEs_newInterFrequencyCellInfoList_v590ext *) rel5_ext_ptr) == FAILURE))
      {
        return FAILURE;
      }
    }
    else
    { /* HCS is NOT in use */
      /* Set HCS flag to FALSE */

      if ( measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
        cellSelectQualityMeasure.t ==
        T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_1_cpich_RSCP )
      {
        /* The Cell Select Quality Measure is RSCP */
        if (!(measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
          cellSelectQualityMeasure.u.cpich_RSCP->m.interFreqMeasurementSysInfoPresent))
        {
          /* Optional param Inter freq meas sys RSCP info is absent */
          /* No need to initialize inter_freq_meas_info_incl as it is already set to FALSE */

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter freq meas sys info absent");

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
          /*
            SIB11bis has the cell list, SIb11 doesnt have the cell list
          */
          if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
            return FAILURE;
          }
          return SUCCESS;
#else
          return FAILURE;
#endif
        }

        /* Stores RSCP sys info in the following var */
        inter_freq_meas_rscp_ptr = &measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
          cellSelectQualityMeasure.u.cpich_RSCP->interFreqMeasurementSysInfo;


        /* Check inter-freq cell info present */
        if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_rscp_ptr,
          tdsrrc_InterFreqMeasurementSysInfo_RSCP_LCR_r4,interFreqCellInfoSI_List)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_RSCP_LCR,newInterFreqCellList)) &&
          (inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n != 0))

        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Cell List present");
          /* Check "intraSecondaryFreqIndicator" in v7b0 */
          if ((sib11_rel7b0_ext_ptr != NULL) && 
              (sib11_rel7b0_ext_ptr->n != 
              inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n ))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell number mismatch in v7b0&v4b0");
            return FAILURE;
          }

          /* Always ignore the number of removed cells in System Info 11
          as there is nothing to delete initially */

          /*  Process the new cell list */

          cell_count = 0;

          /* Store the ptr in a local variable */
          rscp_cell_list_ptr = &inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList;
          idx=0;
          while ( (rscp_cell_list_ptr->n > idx) &&
            (cell_count < TDSL1_MAX_CELL_MEAS) )
          {
          /* Pointer to next cell is Non-NULL and cell count
            hasn't exceeded the max as per specs */
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 115;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;

            /* Get the Freq Info */
            if (TDSRRC_MSG_LIST_BITMASK_IE(rscp_cell_list_ptr,
                tdsrrc_NewInterFreqCellSI_RSCP,frequencyInfo))
            {
              if (rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
                T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. FDD cell");
                idx++;
                continue;
              }

              if (tdsrrc_validate_frequency_info(&rscp_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignored cell with dl freq %d",rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
                idx++;
                continue;
              }

              if (rscp_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
              {
                TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",rscp_cell_list_ptr->elem[idx].
                  frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
                idx++;
                continue;
              }

              /* Store DL freq */
              l1_inter_freq_sib_ptr->add_cell[cell_count].
                freq_info.uarfcn_dl = (uint16) rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

              TDSRRC_MSG3(MSG_LEGACY_MED,"IFreq:DL Freq is %d", l1_inter_freq_sib_ptr->add_cell[cell_count].
                freq_info.uarfcn_dl,0,0);

            }
            else
            {
              if (cell_count == 0)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
                idx++;
                continue;
              }
              else
              {
                /* This implies that there is already one DL and UL freq present. Copy from there */
                memscpy(&l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info,
                  sizeof(tdsl1_freq_info_struct_type),
                  &l1_inter_freq_sib_ptr->add_cell[cell_count-1].freq_info,
                  sizeof(tdsl1_freq_info_struct_type));
              }
            }

            /* Initial IE 'intraSecondaryFreqIndicator' */
            l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind_incl = FALSE;
            /*Store "intraSecondaryFreqIndicator"*/
            if (sib11_rel7b0_ext_ptr != NULL)
            {
              sec_freq_ind_incl = TRUE;
              l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind_incl = TRUE;
              
              l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind = 
                     sib11_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;

              sec_freq_ind = sib11_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
            }
            else
            {
              l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind = FALSE;
            }

            /* First store cell info so that cpi of this cell can be extracted */
            if (tdsrrcmeas_fill_rscp_cell_info(&rscp_cell_list_ptr->elem[idx].
              cellInfo, &l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");
              idx++;
              continue;
            }
            /* Check if inter-freq cell ID is present */
            if (TDSRRC_MSG_LIST_BITMASK_IE(rscp_cell_list_ptr,
              tdsrrc_NewInterFreqCellSI_RSCP,interFreqCellID))
            {
              cell_position = rscp_cell_list_ptr->elem[idx].interFreqCellID;
            } /* End of if of intra-freq cell id present */
            else
            {
              cell_position = TDSAVAILABLE_CELL_POSITION;
            } /* End of else of inter-freq cell id not present */


            if ( tdsrrcifreq_update_inter_freq_cell_list(
              &cell_position,
              &l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi,
              &l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl, &sec_freq_ind_incl,&sec_freq_ind) == SUCCESS)
            {
              /* cpi sucessfully stored in the cell info list variable */
              /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
              l1_inter_freq_sib_ptr->add_cell[cell_count].cpi =
                l1_inter_freq_sib_ptr->add_cell[cell_count].
                cell_info.cpi;

              l1_inter_freq_sib_ptr->add_cell[cell_count].cpi_incl =
                l1_inter_freq_sib_ptr->add_cell[cell_count].
                cell_info.cpi_incl;

              /* set the measure in PCH IDLE and FACH flag to TRUE */
              l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
			  /* set the default value of high-cost indicator: normal cell */
			  l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.high_cost_cell = FALSE;
			  
			  /* set the high-cost indicator */

			    tdsrrcsibdb_find_cell_index_for_freq_cpid(
									&l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info,
									l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info.uarfcn_dl);
				TDSRRC_MSG1(MSG_LEGACY_HIGH,"high cost flag - %d",l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.high_cost_cell);

              if(sib18_present)
              {
                if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
                {
                  if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
                  {
                    tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                      &tmp_plmn_identity);
                    if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                    {
                      if((tdsrrccsp_check_for_eplmn(
                        mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                        tmp_plmn_identity))) == FALSE)
                      {
                        TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d Non eq PLMN measure only in DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                        /* set the flag to FALSE */
                        l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                      }
                    }
                  }
                  else
                  {
                    /* use the previous value of the flag if PLMN id is not selected PLMN */
                    if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                    {
                      /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                      if(cell_count > 0)
                      {
                        l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                          l1_inter_freq_sib_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                      }
                    }
                  }
                }
                else
                {
                  /* PLMN list is shorter than the cell info list set flag to TRUE for
                  * the remaining cells */
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                }
              }
              else
              {
                /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
                TDSRRC_MSG1(MSG_LEGACY_MED,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
              }
              /* Increment cell count as add cell successfully stored */
              cell_count ++;
            }  /* End of if of rrcmeas_update_inter_freq_cell_list */

            /* Get the next cell */
            idx++;
          } /* End of while */
          /* Set number of added cells */
          l1_inter_freq_sib_ptr->num_cell_add = cell_count;

          if (l1_inter_freq_sib_ptr->num_cell_add == 0)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:No new cell. SIB req not sent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
            if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
              return FAILURE;
            }
            return SUCCESS;
#else
            return FAILURE;
#endif
          }

        } /* Inter freq cell info list is present */
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List absent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
          if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
            return FAILURE;
          }
          return SUCCESS;
#else
          return FAILURE;
#endif
        }
      } /* End of if of RSCP chosen */
      else /* rrc_hcs_not_used_cellSelectQualityMeasure_cpich_Ec_N0 */
      {
        /* Store first the cell selection and reselection quality parameter */
        //rrc_inter_freq_sib_ptr->cell_sel_resel_quan_type =  TDSL1_EC_NO;
   
        if (!(measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
          cellSelectQualityMeasure.u.cpich_Ec_N0->m.interFreqMeasurementSysInfoPresent))
        {
          /* Optional param Inter freq meas sys ECN0 info is absent */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter freq meas sys info absent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
          if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
            return FAILURE;
          }
          return SUCCESS;
#else
          return FAILURE;
#endif
        }
   
        /* Stores EcNo sys info in the following var */
        inter_freq_meas_ecno_ptr = &measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_not_used->
          cellSelectQualityMeasure.u.cpich_Ec_N0->interFreqMeasurementSysInfo;
   
   
        /* Check inter-freq cell info present */
        if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_ecno_ptr,
          tdsrrc_InterFreqMeasurementSysInfo_ECN0,interFreqCellInfoSI_List)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
          tdsrrc_InterFreqCellInfoSI_List_ECN0,newInterFreqCellList)) &&
          (inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n != 0))
   
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List present");

          /* Check "intraSecondaryFreqIndicator" in v7b0 */
          if ((sib11_rel7b0_ext_ptr != NULL) && 
              (sib11_rel7b0_ext_ptr->n != 
              inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n ))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell number mismatch in v7b0&v4b0");
            return FAILURE;
          }
   
          /* Always ignore the number of removed cells in System Info 11
          as there is nothing to delete initially */
   
          /*  Process the new cell list */
   
          cell_count = 0;
   
          /* Store the ptr in a local variable */
          ecno_cell_list_ptr = &inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList;
          idx=0;
          while ( (ecno_cell_list_ptr->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS) )
          {
          /* Pointer to next cell is Non-NULL and cell count
            hasn't exceeded the max that is allowed by specs */
   
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 24 * 2;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
   
            /* Get the Freq Info */
            if (TDSRRC_MSG_LIST_BITMASK_IE(ecno_cell_list_ptr,tdsrrc_NewInterFreqCellSI_ECN0,frequencyInfo))
            {
              if (ecno_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
                T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent/FDD");
                idx++;
                continue;
              }
   
              if (tdsrrc_validate_frequency_info(&ecno_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignored cell with dl freq %d",ecno_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
                idx++;
                continue;
              }
              if (ecno_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
              {
                TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",ecno_cell_list_ptr->elem[idx].
                  frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
                idx++;
                continue;
              }
   
              /* Store DL freq */
              l1_inter_freq_sib_ptr->add_cell[cell_count].
                freq_info.uarfcn_dl = (uint16) ecno_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
   
              TDSRRC_MSG3(MSG_LEGACY_MED,"IFreq:DL Freq is %d", l1_inter_freq_sib_ptr->add_cell[cell_count].
                freq_info.uarfcn_dl,0,0);
            }
            else
            {
              if (cell_count == 0)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
                idx++;
                continue;
              }
              else
              {
                /* This implies that there is already one DL and UL freq present. Copy from there */
                memscpy(&l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info,
                  sizeof(tdsl1_freq_info_struct_type),
                  &l1_inter_freq_sib_ptr->add_cell[cell_count-1].freq_info,
                  sizeof(tdsl1_freq_info_struct_type));
              }
            }

            /* Initial IE 'intraSecondaryFreqIndicator' */
            l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind_incl = FALSE;
            /*Store "intraSecondaryFreqIndicator"*/
            if (sib11_rel7b0_ext_ptr != NULL)
            {
              sec_freq_ind_incl = TRUE;
              l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind_incl = TRUE;
              l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind = 
                     sib11_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
              
              sec_freq_ind = sib11_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
            }
            else
            {
              l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind = FALSE;
            }

            /* Downlink Freq Present */
   
            /* First store cell info so that cpi of this cell can be extracted */
            if (tdsrrcmeas_fill_ecn0_cell_info(&ecno_cell_list_ptr->elem[idx].
              cellInfo, &l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info,((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");
              idx++;
              continue;
            }

            /* Check if inter-freq cell ID is present */
            if (TDSRRC_MSG_LIST_BITMASK_IE(ecno_cell_list_ptr,
              tdsrrc_NewInterFreqCellSI_ECN0,interFreqCellID))
            {
              cell_position = ecno_cell_list_ptr->elem[idx].interFreqCellID;
            } /* End of if of intra-freq cell id present */
            else
            {
              cell_position = TDSAVAILABLE_CELL_POSITION;
            } /* End of else of inter-freq cell id not present */
   
   
   
   
            if ( tdsrrcifreq_update_inter_freq_cell_list(
              &cell_position,
              &l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi,
              &l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,&sec_freq_ind_incl, &sec_freq_ind) == SUCCESS)
            {
              /* cpi sucessfully stored in the cell info list variable */
              /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
              l1_inter_freq_sib_ptr->add_cell[cell_count].cpi =
                l1_inter_freq_sib_ptr->add_cell[cell_count].
                cell_info.cpi;

              l1_inter_freq_sib_ptr->add_cell[cell_count].cpi_incl =
                l1_inter_freq_sib_ptr->add_cell[cell_count].
                cell_info.cpi_incl;
   
              /* set the measure in PCH IDLE and FACH flag to TRUE */
              l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
			  /* set the default value of high-cost indicator: normal cell */
			  l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.high_cost_cell = FALSE;
			  
			  /* set the high-cost indicator */

			    tdsrrcsibdb_find_cell_index_for_freq_cpid(
									&l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info,
									l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info.uarfcn_dl);
				TDSRRC_MSG1(MSG_LEGACY_HIGH,"high cost flag - %d",l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.high_cost_cell);

              if(sib18_present)
              {
                if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
                {
                  if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
                  {
                    tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                      &tmp_plmn_identity);
                    if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                    {
                      if((tdsrrccsp_check_for_eplmn(
                        mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                        tmp_plmn_identity))) == FALSE)
                      {
                        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CPID %d Non eq PLMN measure only in DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                        /* set the flag to FALSE */
                        l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                      }
                    }
                  }
                  else
                  {
                  /* use the previous value of the flag if PLMN id is not selected PLMN */
                    if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                    {
                      /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                        l1_inter_freq_sib_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                    }
                  }
                }
                else
                {
                  /* PLMN list is shorter than the cell info list set flag to true for the remaining cells */
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
                  /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                }
              }
              else
              {
                /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
                TDSRRC_MSG1(MSG_LEGACY_MED,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
              }
              /* Increment cell count as add cell successfully stored */
              cell_count ++;
            }  /* End of if of rrcmeas_update_inter_freq_cell_list */
   
            /* Get the next cell */
            idx++;
          } /* End of while */
          /* Set number of added cells */
          l1_inter_freq_sib_ptr->num_cell_add = cell_count;
   
          if (l1_inter_freq_sib_ptr->num_cell_add == 0)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:No new cell. SIB req not sent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
            if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
              return FAILURE;
            }
            return SUCCESS;
#else
            return FAILURE;
#endif
          }
   
        } /* Inter freq cell info list is present */
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List absent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
          if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
            return FAILURE;
          }
          return SUCCESS;
#else
          return FAILURE;
#endif
        }
   
      } /* End of else of EcNo chosen */
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      prev_cell_count = l1_inter_freq_sib_ptr->num_cell_add;
      if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
        l1_inter_freq_sib_ptr->num_cell_add = prev_cell_count;
      }
#endif
    } /* Else of t is set to 'not hcs' */
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
    if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
      if(rach_rpt_exists)
      {
        (void)tdsrrcifreq_read_rach_rpt_info_sib11(interFreqRACHReportingInfo,&tdsrrcifreq_inter_freq_rach_rpt);
      }
    }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
  } /* End of sib11_ptr is valid */

  return SUCCESS;
}


/*===========================================================================

FUNCTION   RRCIFREQ_UPDATE_INTER_FREQ_CELL_LIST

DESCRIPTION

  Updates the inter freq cell list with new values of inter freq
  cell id. This is done on receving SIB 11 system info. The calling fn gives
  position_no_ptr indicating where to store the cpi. If it
  points to TDSAVAILABLE_CELL_POSITION, then store cpi in the first
  available position in tdsinter_freq_cell_list and and mark that
  position as occupied. If position_no_ptr points to some other
  value then store cpi at the position given by that value

DEPENDENCIES

  None

RETURN VALUE

  FAILURE if inter freq cell list cannot be updated due to incorrect
  value of the inter freq cell id
  SUCCESS if inter freq cell list can be successfully updated

SIDE EFFECTS

  None
===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_update_inter_freq_cell_list
(
 uint32* position_no_ptr,
 uint16* cpi_ptr,
 uint16* dl_freq_ptr,
 boolean* sec_freq_ind_incl,
 boolean* sec_freq_ind 
)
{
  uint16 count = 0;

  if ((position_no_ptr == NULL) || (cpi_ptr == NULL) || (dl_freq_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Input parameters pointer is NULL");
    return FAILURE;
  }

  /* If UTRAN tells UE to update cell in a position in which there is
  * already a cell, then this variable stores the cpi */

  /* First check if one has to add this in the first available position */
  if (*position_no_ptr == TDSAVAILABLE_CELL_POSITION)
  {
    for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
    {
      if (tdsinter_freq_cell_list[count].cell_position == TDSVACANT)
      {
        /* Store this cell in the first available vacant position */
        tdsinter_freq_cell_list[count].cell_position = TDSOCCUPIED;
        tdsinter_freq_cell_list[count].cpi = *cpi_ptr;

        tdsinter_freq_cell_list[count].dl_freq = *dl_freq_ptr;
        tdsinter_freq_cell_list[count].sec_freq_ind_incl = *sec_freq_ind_incl;
        tdsinter_freq_cell_list[count].sec_freq_ind = *sec_freq_ind;

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFreq:Adding cell %d,cpi=%d,dfreq=%d",count,*cpi_ptr,*dl_freq_ptr);

        return SUCCESS;
      }
    } /* End of for */
  } /* End of if of  position_no_ptr == TDSAVAILABLE_CELL_POSITION */


  /* Now check which position the cells needs to be updated */
  for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
  {
    if (count == *position_no_ptr)
    {
    /* We have reached the position in the array which is given by id
    * Check if there is already a cell in this position
    * If yes, then this cell has to be removed and L1
    * has to be informed about it. So store its cpi
      */

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFreq:Adding cell %d,cpi=%d,dfreq=%d",count,*cpi_ptr,*dl_freq_ptr);

      /* To add support for SIB12. Store removed freq and cpi */
      /* Store the new cpi at this position */
      tdsinter_freq_cell_list[count].cpi = *cpi_ptr;

      tdsinter_freq_cell_list[count].dl_freq = *dl_freq_ptr;
      
      tdsinter_freq_cell_list[count].sec_freq_ind_incl = *sec_freq_ind_incl;
      tdsinter_freq_cell_list[count].sec_freq_ind = *sec_freq_ind;

      /* Mark the position as occupied */
      tdsinter_freq_cell_list[count].cell_position = TDSOCCUPIED;
      return SUCCESS;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Inter-freq cell ID = %d not found",*position_no_ptr);
  return FAILURE;

}

/*===========================================================================

FUNCTION   RRCIFREQ_INITIALIZE_CELL_LIST

DESCRIPTION

    This function initializes the inter freq cell list


DEPENDENCIES

    None

RETURN VALUE

    None

SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcifreq_initialize_cell_list
(
 void
)
{
  uint8 cnt = 0;
  for (cnt = 0; cnt <TDSL1_MAX_CELL_MEAS; cnt++ )
  {
    tdsinter_freq_cell_list[cnt].cpi = TDSINVALID_CPI;
    tdsinter_freq_cell_list[cnt].cell_position = TDSVACANT;
    tdsinter_freq_cell_list[cnt].dl_freq = TDSINVALID_INTER_FREQ;
    tdsinter_freq_cell_list[cnt].sec_freq_ind_incl = FALSE;
    tdsinter_freq_cell_list[cnt].sec_freq_ind = FALSE;
    tdsinter_freq_cell_list_tmp[cnt].cpi = TDSINVALID_CPI;
    tdsinter_freq_cell_list_tmp[cnt].cell_position = TDSVACANT;
    tdsinter_freq_cell_list_tmp[cnt].dl_freq = TDSINVALID_INTER_FREQ;
    tdsinter_freq_cell_list_tmp[cnt].sec_freq_ind = FALSE;    
    tdsinter_freq_cell_list_tmp[cnt].sec_freq_ind_incl = FALSE;
  }
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
  tdsinterf_rach_report_crit = TDSNO_RRC_MEAS_RACH_REPORT;
  tdsrrcifreq_inter_freq_rach_rpt.inter_freq_rach_info_incl = FALSE;
#endif
}



/*===========================================================================

FUNCTION   RRCIFREQ_FIND_PSC_FREQ

DESCRIPTION

 Finds the primary scrambling code of the intra freq cell id. The primary
 scrambling code is returned back in cpi_ptr provided. If not found,
 then cpi_ptr contains TDSINVALID_CPI

DEPENDENCIES

 None

RETURN VALUE

 TDSCPI_ABSENT if cpi absent
 TDSCPI_PRESENT if cpi present

SIDE EFFECTS

 None
===========================================================================*/

tdscpi_e_type tdsrrcifreq_find_cpi_freq
(
 uint32 inter_freq_cell_id,
 uint16* cpi_ptr,
 uint16* dl_freq_ptr,
 boolean *sec_freq_ind_incl,
 boolean *sec_freq_ind 
)
{
  if((cpi_ptr == NULL) || (dl_freq_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Input parameters pointer is NULL");
    return TDSCPI_ABSENT;
  }
  /* Found the position for which cpi needs to be returned */
  if (tdsinter_freq_cell_list[inter_freq_cell_id].cell_position == TDSOCCUPIED)
  {
    /* Found the good cpi */
    *cpi_ptr = tdsinter_freq_cell_list[inter_freq_cell_id].cpi;

    *dl_freq_ptr = tdsinter_freq_cell_list[inter_freq_cell_id].dl_freq;
    
    *sec_freq_ind_incl = tdsinter_freq_cell_list[inter_freq_cell_id].sec_freq_ind_incl;

    *sec_freq_ind = tdsinter_freq_cell_list[inter_freq_cell_id].sec_freq_ind;

    return TDSCPI_PRESENT;
  }
  else
  {
    /* CPID at this position not valid */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Cell Id %d incorrect",inter_freq_cell_id);

    *cpi_ptr = TDSINVALID_CPI;
    *dl_freq_ptr = TDSINVALID_INTER_FREQ;
    *sec_freq_ind_incl = FALSE;
    *sec_freq_ind = FALSE;

    return TDSCPI_ABSENT;
  }
}


/*===========================================================================

FUNCTION   RRCIFREQ_REMOVE_CELLID

DESCRIPTION

 Removes the primary scrambling code at position given by intra freq cell id
 in tdsinter_freq_cell_list. The cpi at the removed position is given back so that
 L1 can be informed of removing the particular cell.

DEPENDENCIES

 None

RETURN VALUE

 TDSCPI_ABSENT if cpi absent
TDSCPI_PRESENTT if cpi present

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcifreq_remove_cellid
(
 uint32 inter_freq_cell_id
)
{
  tdsinter_freq_cell_list[inter_freq_cell_id].cell_position = TDSVACANT;

  tdsinter_freq_cell_list[inter_freq_cell_id].cpi =  TDSINVALID_CPI;

  tdsinter_freq_cell_list[inter_freq_cell_id].dl_freq = TDSINVALID_INTER_FREQ;

}


/*===========================================================================

FUNCTION   RRCIFREQ_UPDATE_INTER_FREQ_CELL_LIST_MCM

DESCRIPTION

 Invoked from the cell list processing fn specified by MCM
 The calling fn specifies the position, scrambling code
 and the downlink freq. The fn checks if there is
 already a valid scr code and dl freq at that position.
 If yes, then it stores them in cpi_to_remove_ptr and
 dl_freq_to_remove_ptr and updates the inter freq cell list
 with the new scr code and dl freq and does not change
 the position. If there was no cell at that position, then
 it stores the dl freq and scr code and markls that position
 in the inter freq cell list as occupied.

DEPENDENCIES

 None.

RETURN VALUE

 FAILURE: If unable to update the inter freq cell list.
 SUCCESS: If able to update the inter freq cell list.

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcifreq_update_inter_freq_cell_list_mcm
(
 uint32* position_no_ptr,
 uint16* cpi_ptr,
 uint16* dl_freq_ptr,
 uint16* cpi_to_remove_ptr,
 uint16* dl_freq_to_remove_ptr,
 boolean *sec_freq_ind_incl,
 boolean *sec_freq_ind
)
{
  uint16 count = 0;

  if((position_no_ptr == NULL) || (cpi_ptr == NULL) || (dl_freq_ptr == NULL) || 
       (cpi_to_remove_ptr == NULL) || (dl_freq_to_remove_ptr == NULL)
       || (sec_freq_ind_incl == NULL)|| (sec_freq_ind == NULL)
       )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Input parameters pointer is NULL");
    return TDSRRCMEAS_FAIL;
  }

  /* First check if one has to add this in the first available position */
  if (*position_no_ptr == TDSAVAILABLE_CELL_POSITION)
  {
    for (count = 0; count < TDSMAX_NO_OF_CELLS; count++)
    {
      if (tdsinter_freq_cell_list[count].cell_position == TDSVACANT)
      {
        /* Store this cell in this first available vacant position */
        tdsinter_freq_cell_list[count].cell_position = TDSOCCUPIED;
        tdsinter_freq_cell_list[count].cpi = *cpi_ptr;
        tdsinter_freq_cell_list[count].dl_freq = *dl_freq_ptr;
        tdsinter_freq_cell_list[count].sec_freq_ind_incl = *sec_freq_ind_incl;
        tdsinter_freq_cell_list[count].sec_freq_ind = *sec_freq_ind;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFreq:Add Cell at pos=%d,cpi=%d,freq=%d",count,*cpi_ptr,*dl_freq_ptr);


        /* Set psc_to_remove_ptr to TDSINVALID_CPI so that calling fn
        * does not need to update this */
        *cpi_to_remove_ptr = TDSINVALID_CPI;

        *dl_freq_to_remove_ptr = TDSINVALID_INTER_FREQ;
        return TDSRRCMEAS_SUCCESS;
      }
    } /* End of for */
  } /* End of if of  position_no_ptr == TDSAVAILABLE_CELL_POSITION */

  /* Now check which position the cells needs to be updated */
  for (count = 0; count < TDSMAX_NO_OF_CELLS; count++)
  {
  /*  Check if there is already a cell in this position
  *  If yes, then this cell has to be removed and L1
    *  has to be informed about it. So store its cpi  */

    if ( (count == *position_no_ptr) &&
      (tdsinter_freq_cell_list[count].cell_position == TDSOCCUPIED) )
    {

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFreq:Add:CPI %d at pos=%d removed[Cascading] by %d",
        tdsinter_freq_cell_list[count].cpi,count,*cpi_ptr);


      /* Store the removed cpi */
      *cpi_to_remove_ptr = tdsinter_freq_cell_list[count].cpi;

      *dl_freq_to_remove_ptr = tdsinter_freq_cell_list[count].dl_freq;

      /* Store the new cpi, dl freq at this position */
      tdsinter_freq_cell_list[count].cpi = *cpi_ptr;

      tdsinter_freq_cell_list[count].dl_freq = *dl_freq_ptr;
      tdsinter_freq_cell_list[count].sec_freq_ind_incl = *sec_freq_ind_incl;
      tdsinter_freq_cell_list[count].sec_freq_ind = *sec_freq_ind;

      return TDSRRCMEAS_SUCCESS;
    }
    else if ( (count == *position_no_ptr) &&
      (tdsinter_freq_cell_list[count].cell_position == TDSVACANT) )
    {

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"IFreq:Adding cell at pos %d with cpi %d",count,*cpi_ptr);


      /* Store the new cpi at this position */
      tdsinter_freq_cell_list[count].cpi = *cpi_ptr;

      tdsinter_freq_cell_list[count].dl_freq = *dl_freq_ptr;
      tdsinter_freq_cell_list[count].sec_freq_ind_incl = *sec_freq_ind_incl;
      tdsinter_freq_cell_list[count].sec_freq_ind = *sec_freq_ind;

      /* Mark the position as occupied. NOT REQD */
      tdsinter_freq_cell_list[count].cell_position = TDSOCCUPIED;

      /* No cpi to be removes as this place is vacant */
      *cpi_to_remove_ptr = TDSINVALID_CPI;

      *dl_freq_to_remove_ptr = TDSINVALID_INTER_FREQ;

      return TDSRRCMEAS_SUCCESS;

    } /* End of else if */
  } /* End of for */
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"Inter-freq cell ID = %d not valid",*position_no_ptr);
  return TDSRRCMEAS_FAIL;

}


/*===========================================================================

FUNCTION   RRCIFREQ_SEND_L1_CMD

DESCRIPTION

 This function sends the L1 command from SIB's only

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcifreq_send_l1_cmd
(
 tdsl1_meas_ctrl_enum_type*  meas_choice_ptr,
 tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib_ptr
)
{
  /* Ptr which carries info about the command */
  tdsrrc_cmd_type        *rrc_meas_out_cmd_ptr = NULL;;

  /* Now allocate memory */
  if ( (rrc_meas_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL )
  {
    ERR_FATAL("Memory Failure for RRC command",0,0,0);
    /* ERR_FATAL would take care of bringing down RRC */
  }
  else
  {
    tdsl1_req_cmd * rrc_meas_cmd_l1_ptr = NULL;
    tdsrrc_get_pointer_l1_req_value(&rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_meas_cmd_l1_ptr);
    
    /* Initialize the LLC command header first */
  
    rrc_meas_out_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_MCMR;
  
    rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
  
    /* Now fill the L1 cmd header */
    rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_MEASUREMENT_REQ;
  
    /* act_time_type */
    rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type =
      TDSL1_ACTIVATION_TIME_NONE;
    
    /* Now allocate memory to the pointer inside the command */
    if (( rrc_meas_cmd_l1_ptr->meas.meas_ptr =
         (tdsl1_meas_parm_type *)tdsl1_mem_alloc(sizeof(tdsl1_meas_parm_type)))
      == NULL )
    {
      /* Note: Before removing this Error Fatal, make sure that the memory
       * being pointed to by rrc_meas_out_cmd_ptr above is freed.
       */
      ERR_FATAL("Unable to get Cmd buffer",0,0,0);
    }
  
    /* Now fill the values inside the ptr */
    if ( *meas_choice_ptr == TDSL1_INTER_FREQ_SIB_PARMS)
    {
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
      if(tdsrrcmeas_validate_sib19_priority_info())
      {
        l1_inter_freq_sib_ptr->hcs_used = FALSE;
      }
#endif
      rrc_meas_cmd_l1_ptr->meas.meas_ptr->u.inter_freq_sib =
        *l1_inter_freq_sib_ptr;
    }
  
    /* Fill in param t */
    rrc_meas_cmd_l1_ptr->meas.meas_ptr->parm_type =
      *meas_choice_ptr;
  #ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
    rrc_meas_cmd_l1_ptr->meas.meas_ptr->inter_freq_rach_info_incl = FALSE;
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
      rrc_meas_cmd_l1_ptr->meas.meas_ptr->inter_freq_rach_info_incl =
                        tdsrrcifreq_inter_freq_rach_rpt.inter_freq_rach_info_incl;
      rrc_meas_cmd_l1_ptr->meas.meas_ptr->interf_rach_rpt_inf=
                        tdsrrcifreq_inter_freq_rach_rpt.interf_rach_rpt_inf;
    }
  #endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
    /* Now send command to L1 to start Inter Freq Measurements */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending SIB cmd to L1");
  }

  /* Send the Internal RRC Command - LLC should directly handle it */
  tdsrrc_put_int_cmd( rrc_meas_out_cmd_ptr );
}


/*===========================================================================

FUNCTION   RRCIFREQ_FILL_RPT_QUAN

DESCRIPTION

 This function fills the reporting quantity in L1 structure from the ASN1 format

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcifreq_fill_rpt_quan
(
 tdsrrc_InterFreqReportingQuantity* mcm_rpt_quan_ptr,
 tdsl1_inter_freq_rpt_quan_struct_type* l1_rpt_quan_ptr)
{
  l1_rpt_quan_ptr->rpt_rssi = FALSE;
  l1_rpt_quan_ptr->freq_qual_est = FALSE;
  l1_rpt_quan_ptr->other_rpt_quan.cell_id_rpt = FALSE;
  l1_rpt_quan_ptr->other_rpt_quan.cfn_sfn_delta_rpt = FALSE;
  l1_rpt_quan_ptr->other_rpt_quan.iscp_rpt = FALSE;
  l1_rpt_quan_ptr->other_rpt_quan.pathloss_rpt = FALSE;
  l1_rpt_quan_ptr->other_rpt_quan.rscp_rpt = FALSE;
  
  if (mcm_rpt_quan_ptr->utra_Carrier_RSSI == TRUE)
    l1_rpt_quan_ptr->rpt_rssi = TRUE;

  if (mcm_rpt_quan_ptr->frequencyQualityEstimate == TRUE)
    l1_rpt_quan_ptr->freq_qual_est = TRUE;

  if (mcm_rpt_quan_ptr->nonFreqRelatedQuantities.cellIdentity_reportingIndicator == TRUE)
    l1_rpt_quan_ptr->other_rpt_quan.cell_id_rpt = TRUE;

  l1_rpt_quan_ptr->other_rpt_quan.sfn_rpt =
    TDSRRCMEAS_RET_INTRA_FREQ_SFN_DELTA(mcm_rpt_quan_ptr->nonFreqRelatedQuantities.dummy);

  if (mcm_rpt_quan_ptr->nonFreqRelatedQuantities.cellSynchronisationInfoReportingIndicator == TRUE)
    l1_rpt_quan_ptr->other_rpt_quan.cfn_sfn_delta_rpt = TRUE;


  if (mcm_rpt_quan_ptr->nonFreqRelatedQuantities.modeSpecificInfo.t ==
    T_tdsrrc_CellReportingQuantities_modeSpecificInfo_tdd)
  {
    if (mcm_rpt_quan_ptr->nonFreqRelatedQuantities.modeSpecificInfo.u.tdd->timeslotISCP_reportingIndicator == TRUE)
      l1_rpt_quan_ptr->other_rpt_quan.iscp_rpt = TRUE;

    if (mcm_rpt_quan_ptr->nonFreqRelatedQuantities.modeSpecificInfo.u.tdd->primaryCCPCH_RSCP_reportingIndicator == TRUE)
      l1_rpt_quan_ptr->other_rpt_quan.rscp_rpt = TRUE;

    if (mcm_rpt_quan_ptr->nonFreqRelatedQuantities.modeSpecificInfo.u.tdd->pathloss_reportingIndicator == TRUE)
      l1_rpt_quan_ptr->other_rpt_quan.pathloss_rpt = TRUE;
  }
}


/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MEAS_QUANTITY

DESCRIPTION

 This function processes the measurement quantity and converst it into L1 format.

DEPENDENCIES

 None

RETURN VALUE

 Success if meas quantity is valid
 Failure if meas quantity is invalid

SIDE EFFECTS

 None
===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcifreq_process_meas_quantity
(
 tdsrrc_InterFreqMeasQuantity* meas_quan_ptr,
 tdsl1_inter_freq_meas_quan_struct_type* l1_meas_quan_ptr
)
{
  tdsrrcmeas_status_e_type meas_status = TDSRRCMEAS_SUCCESS;

  l1_meas_quan_ptr->interf_meas_quan.filter_coef = 0;


  /* Get Measurement Quantity elements */
  if (meas_quan_ptr->reportingCriteria.t == T_tdsrrc_InterFreqMeasQuantity_reportingCriteria_interFreqReportingCriteria)
  {
    /* Set criteria to event triggered */
    l1_meas_quan_ptr->rpt_choice = TDSL1_INTER_FREQ;


    /* TBD Check filter coefficient value if not present which is the current scenario */
    l1_meas_quan_ptr->interf_meas_quan.filter_coef = 0;

    if (meas_quan_ptr->reportingCriteria.u.interFreqReportingCriteria->modeSpecificInfo.t == 
         T_tdsrrc_InterFreqMeasQuantity_modeSpecificInfo_fdd)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupported FDD mode");
      meas_status = TDSRRCMEAS_FAIL;
      return meas_status;
    }
    if (meas_quan_ptr->reportingCriteria.u.interFreqReportingCriteria->modeSpecificInfo.u.tdd->
            freqQualityEstimateQuantity_TDD == tdsrrc_FreqQualityEstimateQuantity_TDD_primaryCCPCH_RSCP) 
    {
      /* Inter have one meas quantity */
      l1_meas_quan_ptr->interf_meas_quan.meas_quan_list_no = 1;
      l1_meas_quan_ptr->interf_meas_quan.meas_quan[0]= TDSL1_RSCP;
    }

    /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient) */
  
    /* Store filter coefficient */
    l1_meas_quan_ptr->interf_meas_quan.filter_coef = (uint16)
      TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(
      meas_quan_ptr->reportingCriteria.u.interFreqReportingCriteria->filterCoefficient);
   

  }
  else
  {
    /* Set criteria to event triggered */
    l1_meas_quan_ptr->rpt_choice = TDSL1_INTRA_FREQ;

    
      /* Store filter coefficient */
      l1_meas_quan_ptr->interf_meas_quan.filter_coef = (uint16)
        TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(
        meas_quan_ptr->reportingCriteria.u.intraFreqReportingCriteria->
        intraFreqMeasQuantity.filterCoefficient);
    

    if (meas_quan_ptr->reportingCriteria.u.intraFreqReportingCriteria->intraFreqMeasQuantity.modeSpecificInfo.t
      == T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd)
    {
      tdsrrcmeas_set_intra_meas_quan_info(&l1_meas_quan_ptr->interf_meas_quan,
                               &meas_quan_ptr->reportingCriteria.u.intraFreqReportingCriteria->
                               intraFreqMeasQuantity.modeSpecificInfo.u.tdd->intraFreqMeasQuantity_TDDList);
      /* NOTE:"We can't find any proof in 25.331 either. Systems team couldn't identify the cause as well. 
              The suggestion is to remove this checking in TDD and please take a note of this change as open issue. 
              We may come back later on this after we have the real network".*/
      /*
      for(i = 0; i < l1_meas_quan_ptr->interf_meas_quan.meas_quan_list_no; i ++)
      {
        if (l1_meas_quan_ptr->interf_meas_quan.meas_quan[i]== TDSL1_PATHLOSS)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:PathLoss not supported as MeasQuan");
          meas_status = TDSRRCMEAS_FAIL;
        }
      }
      */
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Mode specific t is FDD");
      meas_status = TDSRRCMEAS_FAIL;
    }

  }
  return meas_status;
}


/*===========================================================================

FUNCTION   RRCIFREQ_CONFIG_INTRA_FREQ_DB

DESCRIPTION

  Sets Intra Freq Reporting Criteria in RRC Measurement Database
  from Measurement Control Message after the validation of
  the message has been completed

DEPENDENCIES

  None

RETURN VALUE

  Boolean

SIDE EFFECTS

  None
===========================================================================*/

boolean tdsrrcifreq_config_inter_freq_db
(
 tdsl1_meas_ctrl_parm_struct_type* l1_mcm_ptr,
 tdsrpt_crit_enum_type* rpt_crit_ptr,
 tdsmeas_validity_enum_type m_validity
)
{
  uint16 count = 0;
  /* First check if the meas id already exists */

  /* SETUP existing & MODIFY scenario */
  for ( count = 0; count < TDSMAX_INTER_FREQ_MEAS; count++ )
  {
    if (tdsinter_freq_meas_id_list[count].meas_id == l1_mcm_ptr->meas_id)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Meas Id = %d found in RRC db",l1_mcm_ptr->meas_id);

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Before commiting");
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFREQ:Id %d, Val %d, Owner %d", tdsinter_freq_meas_id_list[count].meas_id,
        tdsinter_freq_meas_id_list[count].meas_validity,
        tdsinter_freq_meas_id_list[count].mcm_owned);

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Meas Id = %d found in RRC db",l1_mcm_ptr->meas_id);

      /* If a setup, always overwrite validity */
      if (l1_mcm_ptr->meas_cmd == TDSL1_MEAS_SETUP)
      {
        /* Set Validity */
        tdsinter_freq_meas_id_list[count].meas_validity = m_validity;
      }
      /* For MODIFY scenario, when validity not specified */
      else if ((l1_mcm_ptr->meas_cmd == TDSL1_MEAS_MODIFY) &&
        (l1_mcm_ptr->meas_object_info_included == TRUE) &&
        (m_validity == TDSNOT_PRESENT))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Retain Current Validity %d",tdsinter_freq_meas_id_list[count].meas_validity);
      }
      /* UTRAN specified Meas Validity with a Modify */
      else if ((l1_mcm_ptr->meas_cmd == TDSL1_MEAS_MODIFY) &&
        (l1_mcm_ptr->meas_object_info_included == TRUE) &&
        (m_validity != TDSNOT_PRESENT))
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"IFREQ:Overwrite Current Validity %d with %d",tdsinter_freq_meas_id_list[count].meas_validity,m_validity);

        tdsinter_freq_meas_id_list[count].meas_validity = m_validity;
      }
      else /* To satisfy LINT */
      {
        /* Normal processing */
      }

      /* MCM in the owner of this Meas */
      tdsinter_freq_meas_id_list[count].mcm_owned = TRUE;

      /* Set Report Criteria */
      tdsinter_freq_meas_id_list[count].rpt_crit = *rpt_crit_ptr;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Meas Crit %d saved in RRC",tdsinter_freq_meas_id_list[count].rpt_crit);

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:After commiting");
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFREQ:Id %d, Val %d, Owner %d", tdsinter_freq_meas_id_list[count].meas_id,
        tdsinter_freq_meas_id_list[count].meas_validity,
        tdsinter_freq_meas_id_list[count].mcm_owned);

      return TRUE;
    }

  } /* End of for loop */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:New Meas Id = %d configured",l1_mcm_ptr->meas_id);

  /* SETUP not existing scenario */
  for ( count = 0; count < TDSMAX_INTER_FREQ_MEAS; count++ )
  {
    /* If Meas Id not found, then it is a new Meas. Set it in first available position */
    if (tdsinter_freq_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Setting Meas Id = %d in RRC db",l1_mcm_ptr->meas_id);

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Before commiting");
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFREQ:Id %d, Val %d, Owner %d", tdsinter_freq_meas_id_list[count].meas_id,
        tdsinter_freq_meas_id_list[count].meas_validity,
        tdsinter_freq_meas_id_list[count].mcm_owned);

      tdsinter_freq_meas_id_list[count].meas_id = l1_mcm_ptr->meas_id;

      tdsinter_freq_meas_id_list[count].meas_validity = m_validity;

      tdsinter_freq_meas_id_list[count].mcm_owned = TRUE;

      /* Set Report Criteria */
      tdsinter_freq_meas_id_list[count].rpt_crit = *rpt_crit_ptr;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:After commiting");
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IFREQ:Id %d, Val %d, Owner %d", tdsinter_freq_meas_id_list[count].meas_id,
        tdsinter_freq_meas_id_list[count].meas_validity,
        tdsinter_freq_meas_id_list[count].mcm_owned);

      return TRUE;
    }

  } /* End of for loop */

  TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:RRC MeasDB for InterFreq FULL!!");
  return FALSE;

}


/*===========================================================================

FUNCTION  RRCIFREQ_FILL_HCS_INFO

DESCRIPTION

 This function fills the overall HCS info for an Inter Freq Cell.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill cell info correctly
 SUCCESS: If Cell Info is filled correctly

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_fill_hcs_info
(
  tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib_ptr
  ,tdsrrc_SysInfoType12_v590ext_IEs_newInterFrequencyCellInfoList_v590ext * rel5_ext_ptr
)
{
  tdsrrc_SysInfoType11 *sib11_ptr = NULL; /* Declared to store the value of SIB 11*/

  /* Local variables for storing value from SIB11 */
  tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4 * inter_freq_meas_rscp_ptr = NULL;

  tdsrrc_InterFreqMeasurementSysInfo_HCS_ECN0_LCR_r4 * inter_freq_meas_ecno_ptr = NULL;

  /* Ptrs to go thr linked list */
  tdsrrc_NewInterFreqCellSI_List_HCS_RSCP_LCR_r4* rscp_cell_list_ptr = NULL;

  tdsrrc_NewInterFreqCellSI_List_HCS_ECN0_LCR_r4* ecno_cell_list_ptr = NULL;

  tdsrrc_MeasurementControlSysInfo_LCR_r4_ext  * measurementControlSysInfo_lcr_ptr = NULL;
  
  tdsrrc_NewInterFreqCellList_v7b0ext          *sib11_rel7b0_ext_ptr = NULL;
  boolean  sec_freq_ind = FALSE;  
  boolean  sec_freq_ind_incl = FALSE;
  
  uint32 idx=0;
  uint8 cell_count = 0;

  uint32 cell_position = 0;

  tdsrrc_SysInfoType18 *sib18_ptr = NULL; /* Declared to store the value of SIB 18*/
  tdsrrc_PLMNsOfInterFreqCellsList *tmp_rrc_PLMNsOfInterFreqCellsList_ptr = NULL;
  rrc_plmn_identity_type tmp_plmn_identity;
  rrc_plmn_identity_type selected_plmn_identity;
  tdsrrc_int_cell_id_type  tmp_cell_id;
  boolean sib18_present = FALSE;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  uint16 prev_cell_count = 0;
#endif
  sib11_ptr = (tdsrrc_SysInfoType11*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB11);

  /* Initialize L1 params */
  l1_inter_freq_sib_ptr->num_cell_add = 0;
  l1_inter_freq_sib_ptr->num_cell_meas = 0;
  l1_inter_freq_sib_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
  l1_inter_freq_sib_ptr->cells_for_meas_incl = FALSE;

  tmp_plmn_identity.mcc[0] =  tmp_plmn_identity.mcc[1] = tmp_plmn_identity.mcc[2] = 0;
  tmp_plmn_identity.num_mnc_digits = 0;
  tmp_plmn_identity.mnc[0] =  tmp_plmn_identity.mnc[1] = 0;

  if((sib18_ptr = (tdsrrc_SysInfoType18*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB18)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)))
    {
      /*
       1>if System Information Block type 11bis is scheduled on BCH:
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities for SIB 11bis"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Idle mode PLMN identities for SIB11bis";
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Connected mode PLMN identities"; 
       2>otherwise, 
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
       
       1> else
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
      */
     
       if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
        connectedModePLMNIdentitiesSIB11bis)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis,
          plmnsOfInterFreqCellsList)) &&
          (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            connectedModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Conn Inter Freq from bis extension ");
        sib18_present = TRUE;
      }
      else if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, v6b0NonCriticalExtensions)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
        idleModePLMNIdentitiesSIB11bis)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
          plmnsOfInterFreqCellsList)) &&
          (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 idle Inter Freq from bis extension ");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, connectedModePLMNIdentities)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterFreqCellsList)) &&
        (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Conn Inter Freq");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, idleModePLMNIdentities)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterFreqCellsList)) &&
        (sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter Freq");
        sib18_present = TRUE;
      }
    }
    else
    {
   
      /*
       If SIB11bis is present and SIB18 bis extenstions are present, use SIB18 bis extension
       If SIB11bis is present and SIb18 bis extensions are absent, use SIB18
       If SIB11bis is absent, use SIB18
      */
      if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18, v6b0NonCriticalExtensions)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
        idleModePLMNIdentitiesSIB11bis)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
        plmnsOfInterFreqCellsList)) &&
        (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            idleModePLMNIdentitiesSIB11bis.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Idle Inter Freq from bis extension ");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, rrc_SysInfoType18_idleModePLMNIdentities))
        &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterFreqCellsList))
      &&
        (sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterFreqCellsList_ptr =& sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter Freq ");
        sib18_present = TRUE;
      }
    }
#else
    if(((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH))
      && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterFreqCellsList))
      &&
      (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Conn Inter Freq");
      sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterFreqCellsList))
      &&
      (sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterFreqCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterFreqCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter Freq");
      sib18_present = TRUE;
    }
#endif

  }
  if(tdsrrcccm_get_serv_plmn_cellid(&tmp_cell_id, &selected_plmn_identity) == TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    sib18_present = FALSE;
	TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB18 not present", 0,0,0);
  }
  else
  {
    memscpy(&tmp_plmn_identity, sizeof(rrc_plmn_identity_type), &selected_plmn_identity, sizeof(rrc_plmn_identity_type));
  }
  if(tdsrrccsp_is_ue_in_true_limited_service() == TRUE)
  {
    sib18_present = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE in limited service ignoring SIB18");
  }
  {
    tdsrrc_MasterInformationBlock *mib_ptr = NULL;/* Declared to store the value of MIB*/
    if((mib_ptr = (tdsrrc_MasterInformationBlock*)  tdsrrcsibdb_return_sib_for_srv_cell
      (tdsrrc_MIB))!= NULL)
    {
      if(tdsrrc_is_nw_shared(mib_ptr))
      {
        sib18_present = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Ignore SIB18");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: N/W not shader so consider SIB18");
      }
    }
  }


  /* Check if the value received is O.K. */

  if ( sib11_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SIB 11 from SIB DB is NULL");

    return FAILURE;
  }
  else
  {
    /* Read SIB 11 parameters */
    if ((sib11_ptr->m.v4b0NonCriticalExtensionsPresent) && 
         (sib11_ptr->v4b0NonCriticalExtensions.m.sysInfoType11_v4b0extPresent))
    {
      measurementControlSysInfo_lcr_ptr = 
        &sib11_ptr->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"cellinfo absent in SIB12");
      return FAILURE;
    }

    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
      if ((sib11_ptr->m.v4b0NonCriticalExtensionsPresent)
           && (sib11_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent)
            && (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v690NonCriticalExtensionsPresent)
            && (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
               v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent)
            && (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
               v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v770NonCriticalExtensionsPresent)
            && (sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.
               v6b0NonCriticalExtensions.v770NonCriticalExtensions.m.v7b0NonCriticalExtensionsPresent))
      {
        if(sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v690NonCriticalExtensions.
               v6b0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
               sysInfoType11_v7b0ext.m.newInterFreqCellListPresent)
        {
          sib11_rel7b0_ext_ptr = &sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                    v690NonCriticalExtensions.v6b0NonCriticalExtensions.v770NonCriticalExtensions.
                     v7b0NonCriticalExtensions.sysInfoType11_v7b0ext.newInterFreqCellList;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"intraSecondaryFreqIndicator present in SIB11");
        }
      }
    }

    if ( measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
      cellSelectQualityMeasure.t ==
      T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_1_cpich_RSCP )
    {

      /* The Cell Select Quality Measure is RSCP */
      if (!(measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
        cellSelectQualityMeasure.u.cpich_RSCP->m.interFreqMeasurementSysInfoPresent))
      {
        /* Optional param Inter freq meas sys RSCP info is absent */
        /* No need to initialize inter_freq_meas_info_incl as it is already set to FALSE */

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Meas Sys Info ABS");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
          return FAILURE;
        }
        return SUCCESS;
#else
        return FAILURE;
#endif
      }

      /* Stores RSCP sys info in the following var */
      inter_freq_meas_rscp_ptr = &measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
        cellSelectQualityMeasure.u.cpich_RSCP->interFreqMeasurementSysInfo;


      /* Check inter-freq cell info present */
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_rscp_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_HCS_RSCP_LCR,newInterFreqCellList)) &&
        (inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n != 0))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Cell List present");

        /* Check "intraSecondaryFreqIndicator" in v7b0 */
        if ((sib11_rel7b0_ext_ptr != NULL) && 
            (sib11_rel7b0_ext_ptr->n != 
            inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n ))
        {

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell number mismatch in v7b0&v4b0");
          return FAILURE;
        }

        /* Always ignore the number of removed cells in System Info 11
        as there is nothing to delete initially */

        /*  Process the new cell list */

        cell_count = 0;

        /* Store the ptr in a local variable */
        rscp_cell_list_ptr = &inter_freq_meas_rscp_ptr->interFreqCellInfoSI_List.newInterFreqCellList;

        while ((rscp_cell_list_ptr->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS))
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max as per specs */

          /* Get the Freq Info */
          if (TDSRRC_MSG_LIST_BITMASK_IE(rscp_cell_list_ptr,tdsrrc_NewInterFreqCellSI_HCS_RSCP_LCR_r4,frequencyInfo))
          {
            if(rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
              T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell fdd");
              idx++;
              continue;
            }

            if (tdsrrc_validate_frequency_info(&rscp_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignored cell with dl freq %d",rscp_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
              idx++;
              continue;
            }

            /* Downlink Freq Present */
            if (rscp_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",rscp_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
              idx++;
              continue;
            }

            /* Store DL freq */
            l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) rscp_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

              TDSRRC_MSG3(MSG_LEGACY_MED,"IFreq:DL Freq is %d", l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);

          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
              idx++;
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }
          /* Initial IE 'intraSecondaryFreqIndicator' */
          l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind_incl = FALSE;
          /*Store "intraSecondaryFreqIndicator"*/
          if (sib11_rel7b0_ext_ptr != NULL)
          {
            sec_freq_ind_incl = TRUE;
            l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind_incl = TRUE;
            
            l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind = 
                   sib11_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
            sec_freq_ind = sib11_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
          }
          else
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind = FALSE;
          }

          /* Set default values for the HCS neighboring cell info if it is first cell
          * else use the info from the previous cell */
          if(cell_count == 0)
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 115;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
          }
          else
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.hcs_prio;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.q_hcs;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time;

            if(l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time
              != 0)
            {
              l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.tmp_offset1 =
                l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.tmp_offset1;
            }
          }

          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_hcs_rscp_cell_info(&rscp_cell_list_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");
            idx++;
            continue;
          }

          /* Check if inter-freq cell ID is present */
          if (TDSRRC_MSG_LIST_BITMASK_IE(rscp_cell_list_ptr,
            tdsrrc_NewInterFreqCellSI_HCS_RSCP_LCR_r4,interFreqCellID))
          {
            cell_position = rscp_cell_list_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */



          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl,&sec_freq_ind_incl, &sec_freq_ind) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi_incl =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi_incl;
            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
            if(sib18_present)
            {
              if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
              {
                if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
                {
                  tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                    &tmp_plmn_identity);
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    if((tdsrrccsp_check_for_eplmn(
                      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tmp_plmn_identity))) == FALSE)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d Non eq PLMN measure only in DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                      /* set the flag to FALSE */
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                    }
                  }
                }
                else
                {
                  /* use the previous value of the flag if PLMN id is not selected PLMN */
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                    if(cell_count > 0)
                    {
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                        l1_inter_freq_sib_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                    }
                  }
                }


              }
              else
              {
              /* PLMN list is shorter than the cell info list set flag to TRUE for
                * the remaining cells */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
              }
            }
            else
            {
              /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
              TDSRRC_MSG1(MSG_LEGACY_MED,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
            }
            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */

          /* Get the next cell */
          idx++;
        }/* End of while */
        /* Set number of added cells */
        l1_inter_freq_sib_ptr->num_cell_add = cell_count;

        if (l1_inter_freq_sib_ptr->num_cell_add == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:No new cell. SIB req not sent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
          if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
            return FAILURE;
          }
          return SUCCESS;
#else
          return FAILURE;
#endif
        }

      } /* Inter freq cell info list is present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List absent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
          return FAILURE;
        }
        return SUCCESS;
#else
        return FAILURE;
#endif
      }
    } /* End of if of RSCP chosen */
    else /* rrc_hcs_used_cellSelectQualityMeasure_cpich_Ec_N0 */
    {
      if (!(measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
        cellSelectQualityMeasure.u.cpich_Ec_N0->m.interFreqMeasurementSysInfoPresent))
      {
        /* Optional param Inter freq meas sys ECN0 info is absent */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Meas Sys info absent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
          return FAILURE;
        }
        return SUCCESS;
#else
        return FAILURE;
#endif
      }

      /* Stores EcNo sys info in the following var */
      inter_freq_meas_ecno_ptr = &measurementControlSysInfo_lcr_ptr->use_of_HCS.u.hcs_used->
        cellSelectQualityMeasure.u.cpich_Ec_N0->interFreqMeasurementSysInfo;


      /* Check inter-freq cell info present */
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(inter_freq_meas_ecno_ptr,
        tdsrrc_InterFreqMeasurementSysInfo_HCS_ECN0_LCR_r4,interFreqCellInfoSI_List)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List,
        tdsrrc_InterFreqCellInfoSI_List_HCS_ECN0_LCR,newInterFreqCellList)) &&
        (inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n != 0))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List present");

        /* Check "intraSecondaryFreqIndicator" in v7b0 */
        if ((sib11_rel7b0_ext_ptr != NULL) && 
            (sib11_rel7b0_ext_ptr->n != 
            inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList.n ))
        {

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell number mismatch in v7b0&v4b0");
          return FAILURE;
        }

        /* Always ignore the number of removed cells in System Info 11
        as there is nothing to delete initially */

        /*  Process the new cell list */

        cell_count = 0;

        /* Store the ptr in a local variable */
        ecno_cell_list_ptr = &inter_freq_meas_ecno_ptr->interFreqCellInfoSI_List.newInterFreqCellList;
        idx=0;
        while ((ecno_cell_list_ptr->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS))
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max that is allowed by specs */

          /* Get the Freq Info */
          if (TDSRRC_MSG_LIST_BITMASK_IE(ecno_cell_list_ptr,
            tdsrrc_NewInterFreqCellSI_HCS_ECN0_LCR_r4,frequencyInfo))
          {
            if(ecno_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
            T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell.FDD");
              idx++;
              continue;
            }

            if (tdsrrc_validate_frequency_info(&ecno_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignored cell with dl freq %d",ecno_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
              idx++;
              continue;
            }

            /* Downlink Freq Present */
            if (ecno_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",ecno_cell_list_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
              idx++;
              continue;
            }

            /* Store DL freq */
            l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) ecno_cell_list_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

            TDSRRC_MSG3(MSG_LEGACY_MED,"IFreq:DL Freq is %d", l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);

          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
             idx++;
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }
          
          /* Initial IE 'intraSecondaryFreqIndicator' */
          l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind_incl = FALSE;
          /*Store "intraSecondaryFreqIndicator"*/
          if (sib11_rel7b0_ext_ptr != NULL)
          {
            sec_freq_ind_incl = TRUE;
            l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind_incl = TRUE;
            
            l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind = 
                   sib11_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
            sec_freq_ind = sib11_rel7b0_ext_ptr->elem[idx].intraSecondaryFreqIndicator;
          }
          else
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].sec_freq_ind = FALSE;
          }

          /* Set default values for the HCS neighboring cell info if it is first cell
          * else use the info from the previous cell */
          if(cell_count == 0)
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 24 * 2;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
          }
          else
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.hcs_prio;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.q_hcs;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time;

            if(l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time
              != 0)
            {
              l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.tmp_offset1 =
                l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.tmp_offset1;
            }
          }
          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_hcs_ecn0_cell_info(&ecno_cell_list_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");
            idx++;
            continue;
          }

          /* Check if inter-freq cell ID is present */
          if (TDSRRC_MSG_LIST_BITMASK_IE(ecno_cell_list_ptr,
            tdsrrc_NewInterFreqCellSI_HCS_ECN0_LCR_r4,interFreqCellID))
          {
            cell_position = ecno_cell_list_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */



          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl, &sec_freq_ind_incl,&sec_freq_ind) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi_incl =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi_incl;

            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
            /* Increment cell count as add cell successfully stored */
            if(sib18_present)
            {
              if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
              {
                if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
                {
                  tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                    &tmp_plmn_identity);
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    if((tdsrrccsp_check_for_eplmn(
                      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tmp_plmn_identity))) == FALSE)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d Non eq PLMN measure only in DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                      /* set the flag to FALSE */
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                    }
                  }
                }
                else
                {
                  /* use the previous value of the flag if PLMN id is not selected PLMN */
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                    if(cell_count > 0)
                    {
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                        l1_inter_freq_sib_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                    }
                  }
                }

                
              }
              else
              {
              /* PLMN list is shorter than the cell info list set flag to TRUE for
                * the remaining cells */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
              }
            }
            else
            {
              /* Add code to set flag meas_pch_fach_idle to TRUE for the cell */
              TDSRRC_MSG1(MSG_LEGACY_MED,"cpi %d measured in IDLE,FACH,PCH,DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
            }
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */

          /* Get the next cell */
          idx++;
        } /* End of while */
        /* Set number of added cells */
        l1_inter_freq_sib_ptr->num_cell_add = cell_count;

        if (l1_inter_freq_sib_ptr->num_cell_add == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:No new cell. SIB req not sent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
          if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
            return FAILURE;
          }
          return SUCCESS;
#else
          return FAILURE;
#endif
        }

      } /* Inter freq cell info list is present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Cell List absent");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
          return FAILURE;
        }
        return SUCCESS;
#else
        return FAILURE;
#endif
      }

    } /* End of else of EcNo chosen */

  } /* End of sib11_ptr is valid */
  
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  prev_cell_count = l1_inter_freq_sib_ptr->num_cell_add;
  if(tdsrrcifreq_read_sib11bis(l1_inter_freq_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterFreqCellsList_ptr) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading inter frequency failure");
    l1_inter_freq_sib_ptr->num_cell_add = prev_cell_count;
  }
#endif
  return SUCCESS;

}

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM_R5

DESCRIPTION

    This function processes the release 5 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm_r5
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_freq_meas_params;
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  tdsrrcmeas_status_e_type result;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_FREQ;

  /* Meas Validity */
  tdsmeas_validity_enum_type meas_validity = TDSNOT_PRESENT;

  tdsrrc_MeasurementControl_r4_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  tdsrrc_MeasurementControl_v590ext_IEs* mcm_590_ext_ptr = NULL;

  uint8 count = 0;

  uint8 cnt =  0;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Assign default values to l1_inter_freq_meas_params */
  memset(&l1_inter_freq_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_freq_meas_params.meas_object.meas_type = TDSL1_INTER_FREQ;

    /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.r4.measurementControl_r4;

  if ((dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.
    criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
    (dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.
    criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent))
    {
      mcm_590_ext_ptr = &dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.
      criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.measurementControl_v590ext;
    }


  /* Set Transaction Id and Meas Id */
  l1_inter_freq_meas_params.trans_id = tdstransaction_id;
  l1_inter_freq_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;



  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_setup)
  {

    /* Double check that it is INTER-FREQ Meas */
    if (rrc_mcm_ptr->measurementCommand.u.setup->t !=
        T_tdsrrc_MeasurementType_r4_interFrequencyMeasurement)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    /* Check Measurement Quantity present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r4,interFreqMeasQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:MeasQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }


    l1_inter_freq_meas_params.meas_object_info_included = TRUE;

    if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.setup->
      u.interFrequencyMeasurement->interFreqMeasQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
    {
      /* Set Meas Quantity to TRUE */
      l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
    }
    else
    {
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    /* Check Reporting Quantity */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r4,interFreqReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SETUP:RptQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }
    if(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r4_intraFreqReportingCriteria)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SETUP:inter freq rpt criteria set to Intra freq rpt criteria");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    /* Set Reporting quantity */
    tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->interFreqReportingQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


    l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;

    /* Store reporting mode to TRUE */
    l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

    l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Rpting Mode %d ACK:0",l1_inter_freq_meas_params.rpt_trans_mode);

    /* Process Reporting Criteria which is mandatory in case meas obj is present */

    if (tdsrrcifreq_process_reporting_criteria_r5(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria, &l1_inter_freq_meas_params.meas_object.u.inter_f, mcm_590_ext_ptr) == FAILURE)
    {

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */

      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    else
    {
      l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

      mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);
    }



    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Addtl Meas spec for Meas %d",rrc_mcm_ptr->measurementIdentity);
     /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
		TDSRRC_MSG0(MSG_LEGACY_ERROR," Called function has already sent Meas Ctrl Failure");
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */
    
    result = tdsrrcifreq_fill_inter_freq_info_r5(&rrc_mcm_ptr->measurementCommand.u.setup->u.
      interFrequencyMeasurement->interFreqCellInfoList,
      &l1_inter_freq_meas_params, NULL) ;
    /* Fill cell list, cell info */
    if (result == TDSRRCMEAS_FAIL)
    {
      /* Send Meas Ctrl Failure Message */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to fill Inter Freq");
      /* Send Meas Ctrl Failure Msg */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

      /* Set failure cause  which is IE value incorrect */
      rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    else
    {
      if(result == TDSSFN_FAIL)
      {
              /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SFN-ind set as True");
      /* Send Meas Ctrl Failure Msg */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
      if (result == TDSRRCMEAS_UNSUPPORTED)
      {
        /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to fill Inter Freq");
        /* Send Meas Ctrl Failure Msg */
  
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
        return FAILURE;
      }

      l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;

    }
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
      tdsrrc_InterFrequencyMeasurement_r4,measurementValidity))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Meas Validity present");
      /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
      then assume it to be CELL_DCH. If network specifies validity to be other than CELL_DCH,
      the UE behaviour is unspecified. So we assume it to be CELL_DCH */
      meas_validity = TDSSTATE_DCH;
    }

    /* Now commit new meas params to RRC db.*/
    if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
    {

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to set MeasId in DB");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    /* Now commit addtl meas db */
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
    }

    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    /* Now send command to L1 */
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_modify)
  {
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

      l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    /* Set type to MODIFY */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r4_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Modify:InterFreq:MType absent");

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
        }
      } /* End of if of Additional Meas Present */

    }
    else
    {
      /* Meas Type has already been verified */
      l1_inter_freq_meas_params.meas_object_info_included = TRUE;

      /* Check Measurement Quantity present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r4,interFreqMeasQuantity))
      {

        if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.
          u.interFrequencyMeasurement->interFreqMeasQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
        {
          /* Set Meas Quantity to TRUE */
          l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      } /* End of meas quantity present */

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r4,interFreqReportingQuantity))
      {

        /* Set Reporting quantity */
        tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->interFreqReportingQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;
      }
      if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.t  == T_tdsrrc_InterFreqReportCriteria_r4_intraFreqReportingCriteria) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.u.intraFreqReportingCriteria,
        & tdsrrc_IntraFreqReportingCriteria_r4,eventCriteriaList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Modify:rpt crit set to Intraf rpt crit and evnt list non null");

        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      /* Process Reporting Criteria */

      if (tdsrrcifreq_process_reporting_criteria_r5(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria, &l1_inter_freq_meas_params.meas_object.u.inter_f, mcm_590_ext_ptr) == FAILURE)
      {

        if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r4_interFreqReportingCriteria) &&
         !((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria,
         tdsrrc_InterFreqReportingCriteria,interFreqEventList)) &&
         (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria->interFreqEventList.n != 0)))
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }
      }
      else
      {
        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

        mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);


      }

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
		  TDSRRC_MSG0(MSG_LEGACY_ERROR,"Called function has already sent Meas Ctrl Failure ");
          return FAILURE;
        }
        else
        {
          addtl_meas_incl = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
               addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
               &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);


            return FAILURE;

          }

        }
      } /* End of if of Additional Meas Present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:additional meas list absent");
      }


      /* Fill cell list, cell info */
      result = tdsrrcifreq_fill_inter_freq_info_r5(
        &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        interFreqCellInfoList, &l1_inter_freq_meas_params, NULL);
      if (result == TDSRRCMEAS_FAIL)
      {
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      else
      {
        if(result == TDSSFN_FAIL)
        {
                /* Send Meas Ctrl Failure Message */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SFN-ind set as True");
        /* Send Meas Ctrl Failure Msg */

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE;
        }
        else
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;
        }
      }

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
        tdsrrc_InterFrequencyMeasurement_r4,measurementValidity))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Meas Validity present");
        /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
        then assume it to be CELL_DCH. */
        meas_validity = TDSSTATE_DCH;
      }

      /* Now commit new meas params to RRC db.*/
      if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to set MeasId in DB");
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        /* Make the old CELL_INFO_LIST valid */

        for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));
        }

        return FAILURE;
      }

      /* Now commit addtl meas db */
      if (addtl_meas_incl == TRUE)
      {
        tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
      }

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    that this meas exists. So let us delete it from tdsinter_freq_meas_id_list
    Meas Identity, Transaction Id already set */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_freq_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_freq_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_FREQ_MEAS; count++)
    {
      if (tdsinter_freq_meas_id_list[count].meas_id == l1_inter_freq_meas_params.meas_id)
      {
        tdsinter_freq_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsinter_freq_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        tdsinter_freq_meas_id_list[count].mcm_owned = FALSE;
        tdsinter_freq_meas_id_list[count].meas_validity = TDSNOT_PRESENT;

        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
          tdsinter_freq_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;

        break;
      }
    }
  } /* End of else of release */

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending interFreq measurements from MCM to L1");
    return SUCCESS;
  }

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_freq_meas_params.trans_id = alloc_idx;
  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_freq_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);

  return SUCCESS;
}

/*===========================================================================

FUNCTION  RRCIFREQ_FILL_INTER_FREQ_INFO_R5

DESCRIPTION

 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 Also updates the Inter Freq Cell Info List for rel 5

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcifreq_fill_inter_freq_info_r5
(
 tdsrrc_InterFreqCellInfoList_r4* interFreqCellInfoList_ptr,
 tdsl1_meas_ctrl_parm_struct_type* l1_meas_ptr,
 tdsrrc_NewInterFreqCellList_v7b0ext *newInterFreqCellList_ptr
)
{
  tdsinter_freq_removed_cell_struct_type cascaded_rem_cell[TDSL1_MAX_CELL_MEAS];

  uint32 inter_freq_cell_id = 0;
  uint32 idx=0;
  uint16 dl_freq = 0;

  uint16 cpi = 0;
  
  boolean sec_freq_ind = FALSE;
  boolean sec_freq_ind_incl = FALSE;

  uint32 cell_position = 0;

  uint16 cpi_to_measure = TDSINVALID_CPI;
  uint16 freq_to_measure = TDSINVALID_INTER_FREQ;
  uint8 meas_count = 0;
  uint32 cell_id = 0;

  uint8 cell_count = 0, removed_cell_count = 0, cpi_removed_count = 0, cnt = 0;

  tdsrrc_RemovedInterFreqCellList_removeSomeInterFreqCells* local_rm_inter_freq_cell_ptr = NULL;

  tdsrrc_CellsForInterFreqMeasList *cellsForInterFreqMeasList_ptr = NULL;

  tdsrrc_NewInterFreqCellList_r4* local_network_new_cell_list_ptr = NULL;


  /* Declare a local var to store the inter freq object */

  tdsl1_inter_freq_meas_ctrl_struct_type* l1_inter_freq_meas_ptr = NULL;

  tdsl1_inter_freq_report_crit_struct_type* inter_rpt_crit_ptr = NULL;

  l1_inter_freq_meas_ptr = &l1_meas_ptr->meas_object.u.inter_f;

  inter_rpt_crit_ptr = &l1_inter_freq_meas_ptr->u.inter_rpt_crit;

  /* Copy Inter Freq CELL_INFO_LIST to a local var. If failure in the processing of
  CELL_INFO_LIST, copy  tdsinter_freq_cell_list_tmp back to tdsinter_freq_cell_list*/
  for (cell_count = 0; cell_count < TDSL1_MAX_CELL_MEAS; cell_count++)
  {
    memscpy(&tdsinter_freq_cell_list_tmp[cell_count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list[cell_count], sizeof(tdsinter_freq_cell_list_struct_type));

    cascaded_rem_cell[cell_count].freq = TDSINVALID_INTER_FREQ;
    cascaded_rem_cell[cell_count].cpi = TDSINVALID_CPI;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,
    tdsrrc_InterFreqCellInfoList_r4,removedInterFreqCellList))
  {
    switch (interFreqCellInfoList_ptr->removedInterFreqCellList.t)
    {
    case T_tdsrrc_RemovedInterFreqCellList_removeAllInterFreqCells:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Remove:Delete all cells selected");
      l1_inter_freq_meas_ptr->cell_list.num_cell_rmv = 0xff;


      /* Update Cell_info_list variable */
      for (cell_count = 0; cell_count < TDSL1_MAX_CELL_MEAS; cell_count++)
      {
        tdsinter_freq_cell_list[cell_count].cell_position = TDSVACANT;
        tdsinter_freq_cell_list[cell_count].cpi = TDSINVALID_CPI;
        tdsinter_freq_cell_list[cell_count].dl_freq = TDSINVALID_INTER_FREQ;
        tdsinter_freq_cell_list[cell_count].sec_freq_ind_incl = FALSE;
        tdsinter_freq_cell_list[cell_count].sec_freq_ind = FALSE;
      }

      break;

    case T_tdsrrc_RemovedInterFreqCellList_removeSomeInterFreqCells:

      if (interFreqCellInfoList_ptr->removedInterFreqCellList.u.removeSomeInterFreqCells == NULL )
      {
        break;
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Delete some cells");
      /* Store head of Link list */
      local_rm_inter_freq_cell_ptr =  interFreqCellInfoList_ptr->removedInterFreqCellList.u.removeSomeInterFreqCells;
      do
      {
        inter_freq_cell_id = local_rm_inter_freq_cell_ptr->elem[idx];

        /* Find the cpi, dl freq as per given cell id from CELL_INFO_LIST */
        if ( tdsrrcifreq_find_cpi_freq(
          inter_freq_cell_id, &cpi, &dl_freq, &sec_freq_ind_incl,&sec_freq_ind) == TDSCPI_ABSENT)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Rem:Incorrect cell id %d",inter_freq_cell_id);
          idx++;
          continue;
        }
        else
        {
          /* First remove cell from inter freq cell list */
          tdsrrcifreq_remove_cellid(inter_freq_cell_id);

          /* Here one needs to put cpi, dl freq so that L1 can remove it */
          l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].cpi = cpi;

          l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].freq = dl_freq;

          removed_cell_count++; /* Increment count */
        }

        /*  Get next element in the list */

        idx++;

      } /* Continue if while cond satisfied */

      while ( (local_rm_inter_freq_cell_ptr->n  > idx) &&
        (removed_cell_count < TDSL1_MAX_CELL_MEAS) );


      /* Now store the number of actual cells sent by UTRAN */

      l1_inter_freq_meas_ptr->cell_list.num_cell_rmv =  removed_cell_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Removed cell cnt = %d cells",removed_cell_count);

      break;

    case T_tdsrrc_RemovedInterFreqCellList_removeNoInterFreqCells:
      /* Already set removed cells to 0 */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Delete no cells selected");
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid cell t");
      break;
    } /* End of switch */

  } /* End of if of removed inter freq cells present */

  /* Reinitialize these variables */
  cell_count = 0;
  cpi = 0;

  /* Check if new cells present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,
    tdsrrc_InterFreqCellInfoList_r4,newInterFreqCellList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:New cells to be added");

    /* Do not check for cell order. If UTRAN messes up, its their problem */
    idx=0;
    /* Save the Head pointer of linked List */
    local_network_new_cell_list_ptr = &interFreqCellInfoList_ptr->newInterFreqCellList;

    while ( ( local_network_new_cell_list_ptr->n > idx) &&
      (cell_count < TDSL1_MAX_CELL_MEAS) )
    {
    /* Pointer to next cell is Non-NULL and cell count
      * hasn't exceeded the max. L1 can handle */

      if (local_network_new_cell_list_ptr->elem[idx].
        cellInfo.modeSpecificInfo.t ==
        T_tdsrrc_CellInfo_r4_modeSpecificInfo_fdd)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore FDD cell");

        idx++;
        continue;

      }

      /* Fill in Cell Info */
      if (tdsrrcmeas_fill_mcm_cell_info_r5(
        &local_network_new_cell_list_ptr->elem[idx].cellInfo,
        &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
        cell_info) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignoring Cell");
        idx++;
        continue;
      }

      /* Check if cell id present */
      if (TDSRRC_MSG_LIST_BITMASK_IE(local_network_new_cell_list_ptr,
        tdsrrc_NewInterFreqCell_r4,interFreqCellID))
      {
        cell_position = local_network_new_cell_list_ptr->elem[idx].interFreqCellID;

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Add:Cell at pos %d to be added",cell_position);

      } /*  End of if of inter-freq cell id present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Add: Cell Id not present");
        cell_position = TDSAVAILABLE_CELL_POSITION;
      }

      /* Check if freq present */
      if (TDSRRC_MSG_LIST_BITMASK_IE(local_network_new_cell_list_ptr,
          tdsrrc_NewInterFreqCell_r4,frequencyInfo))
      {
        if (local_network_new_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
          T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. FDD cell");
          idx++;
          continue;
        }

        if (tdsrrc_validate_frequency_info(&local_network_new_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell with DL freq %d out of ",local_network_new_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
          return TDSRRCMEAS_UNSUPPORTED;
        }


        if (local_network_new_cell_list_ptr->elem[idx].
          frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid DL ARFCN %d",dl_freq);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Inter Freq Cell");
          idx++;
          continue;
        }

        /* Store DL freq */
        dl_freq = l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          freq_info.uarfcn_dl = (uint16) local_network_new_cell_list_ptr->elem[idx].
          frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

      } /*  End of if of inter-freq freq present */
      else
      {
        if (cell_count == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore Cell. DL Freq absent");
          idx++;
          continue;
        }
        else
        {
          /* This implies that there is already one DL and UL freq present. Copy from there */
          memscpy(&l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].freq_info,
            sizeof(tdsl1_freq_info_struct_type),
            &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count-1].freq_info,
            sizeof(tdsl1_freq_info_struct_type));
        }
      }

      /* Initial IE 'intraSecondaryFreqIndicator' */
      l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind_incl = FALSE;
      /* get intraSecondarIndicator if its valid */
      if (newInterFreqCellList_ptr != NULL)
      {
         sec_freq_ind_incl = TRUE;
         l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind_incl = TRUE;
         
         sec_freq_ind = newInterFreqCellList_ptr->elem[cell_count].intraSecondaryFreqIndicator;
         l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind = 
             newInterFreqCellList_ptr->elem[cell_count].intraSecondaryFreqIndicator;
      }
      
      if ( tdsrrcifreq_update_inter_freq_cell_list_mcm(
        &cell_position,
        &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
        cell_info.cpi, &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].freq_info.uarfcn_dl,
        &cpi, &dl_freq, &sec_freq_ind_incl,&sec_freq_ind) == TDSRRCMEAS_SUCCESS)
       {
        /* cpi sucessfully stored in the cell info list variable */
        /* Now store cpi in the mcm in add cell for layer1 to use cpi */
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].cpi =
          l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          cell_info.cpi;

        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].cpi_incl =
          l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          cell_info.cpi_incl;
        
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind_incl = sec_freq_ind_incl;
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind = sec_freq_ind;

        /* This flag is always true for MCM */
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].measure_in_idle_pch_fach = TRUE;

        if ((cpi != TDSINVALID_CPI) && (dl_freq != TDSINVALID_INTER_FREQ) && (cpi_removed_count < TDSL1_MAX_CELL_MEAS))
        {
          /* UTRAN wants us to delete this cpi/freq. Result of CASCADING effect */
          cascaded_rem_cell[cpi_removed_count].cpi = cpi;

          cascaded_rem_cell[cpi_removed_count++].freq = dl_freq;
        }

        /* Increment cell count as add cell successfully stored */
        cell_count ++;
      }  /* End of if of rrcmeas_update_inter_freq_cell_list */

      /* Read next element */
      idx++;

    } /* End of while newIntraFreqCellList != NULL */

    /* Initialize no of added cells */
    l1_inter_freq_meas_ptr->cell_list.num_cell_add = cell_count;

    //This code is to satify KW error only.
    for (cell_count = 0; ((cell_count < cpi_removed_count) && (cell_count < TDSL1_MAX_CELL_MEAS));
         cell_count++, removed_cell_count++)
    {
      if ((removed_cell_count >= TDSL1_MAX_CELL_MEAS) ||(cell_count >= TDSL1_MAX_CELL_MEAS))
      {
        /* Preventive check */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"KW: IFREQ:Unable to delete all cells");
        break;
      }
      /* Add this removed cell to the already built removed cell list for L1 */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Cascad:L1 removed cell list upd %d cpi",cascaded_rem_cell[cell_count].cpi);
      l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].cpi =
        cascaded_rem_cell[cell_count].cpi;

      l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].freq =
        cascaded_rem_cell[cell_count].freq;

    }
    /* Now update the number of removed cells */
    l1_inter_freq_meas_ptr->cell_list.num_cell_rmv += cpi_removed_count;

  } /* End of if for new inter freq cells present */

  /* Initialize no fo cells to 0 */
  l1_inter_freq_meas_ptr->cell_list.num_cell_meas = 0;
  cell_count = 0;
  idx=0;
  l1_inter_freq_meas_ptr->cell_list.cells_for_meas_incl = FALSE;
  /* Check if cells for meas present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,
    & tdsrrc_InterFreqCellInfoList_r4,cellsForInterFreqMeasList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cells for meas present");
    l1_inter_freq_meas_ptr->cell_list.cells_for_meas_incl = TRUE;

    cellsForInterFreqMeasList_ptr = &interFreqCellInfoList_ptr->cellsForInterFreqMeasList;
    meas_count = 0;
    while (cellsForInterFreqMeasList_ptr->n > idx && (meas_count < TDSL1_MAX_CELL_MEAS))
    {

      cell_id = cellsForInterFreqMeasList_ptr->elem[idx];
      /* Check if the cell id is present in tdsrrcmeas_intra_freq_cell_list VARIABLE */
      if ( tdsrrcifreq_find_cpi_freq(cell_id, &cpi_to_measure, &freq_to_measure, &sec_freq_ind_incl,&sec_freq_ind) == TDSCPI_ABSENT)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cell to measure with id = %d",cell_id);

		if (l1_inter_freq_meas_ptr->rpt_mode == TDSL1_INTER_FREQ_EVT_TRIG)
        {
		  while ((cnt < inter_rpt_crit_ptr->num_event) && (cnt < TDSL1_MAX_MEAS_EVENT))
		  {
            if ( (inter_rpt_crit_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2D) ||
			     (inter_rpt_crit_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2F) )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pass serving cell prim freq and cpi to L1 for 2D/2F MCM");
			  cpi_to_measure = (uint16)tdsrrcccm_get_curr_camped_cpid();
			  freq_to_measure = (uint16)tdsrrcccm_get_curr_camped_freq();
			  sec_freq_ind_incl = FALSE;
			  sec_freq_ind = FALSE;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].cpi = cpi_to_measure;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].dl_freq = freq_to_measure;      
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind_incl = sec_freq_ind_incl;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind = sec_freq_ind;
              meas_count++;
			  break;
            }
            cnt++;
          }
        }
      } /* End of if for cpi not found */
      else
      {

      l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].cpi = cpi_to_measure;
      l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].dl_freq = freq_to_measure;      
      l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind_incl = sec_freq_ind_incl;
      l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind = sec_freq_ind;
      /* Increment Ptr */
      meas_count++;
      }
      idx++;
    } /* End of while */

    l1_inter_freq_meas_ptr->cell_list.num_cell_meas =  meas_count;

  } /* End of if of rrc_cellsForInterFreqMeasList) */
  return TDSRRCMEAS_SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_REPORTING_CRITERIA_R5

DESCRIPTION

    This function process the reporting criteria specified
  in Measurement Control Message

DEPENDENCIES

    None

RETURN VALUE

    Success or Failure depending upon processing of reporting criteria

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_reporting_criteria_r5
(
 tdsrrc_InterFreqReportCriteria_r4* ifreq_rpt_criteria_ptr,
 tdsl1_inter_freq_meas_ctrl_struct_type* l1_rpt_params_ptr,
 tdsrrc_MeasurementControl_v590ext_IEs* mcm_590_ext_ptr
)
{
  switch(ifreq_rpt_criteria_ptr->t)
  {
  case T_tdsrrc_InterFreqReportCriteria_r4_interFreqReportingCriteria:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Event Rpt Crit");

    l1_rpt_params_ptr->u.inter_rpt_crit.num_event = 0;

    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria,
         tdsrrc_InterFreqReportingCriteria,interFreqEventList)) &&
      (ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria->interFreqEventList.n != 0))
    {
      if (tdsrrcifreq_process_event_criteria(ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria,
        &l1_rpt_params_ptr->u.inter_rpt_crit, mcm_590_ext_ptr) == FAILURE)
      {
        return FAILURE;
      }

    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"InterFreq Event List Empty");
      return FAILURE;
    }

    /* Inter-Freq Events successfully processed */
    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_EVT_TRIG;

    break;

  case T_tdsrrc_InterFreqReportCriteria_r4_intraFreqReportingCriteria:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Intra Freq Event Rpt Crit");

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting rpt crit to TDSL1_INTER_FREQ_NO_CHANGE");
    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
    break;

  case T_tdsrrc_InterFreqReportCriteria_r4_periodicalReportingCriteria:

    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_PERIODIC;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Perodic Rpt Crit");

    l1_rpt_params_ptr->rpt_cell_status_incl = FALSE;

    /* Set reporting cells */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria,
      tdsrrc_PeriodicalWithReportingCellStatus,reportingCellStatus))
    {
      if (tdsrrcmeas_fill_no_of_reporting_cells(&ifreq_rpt_criteria_ptr->u.
        periodicalReportingCriteria->reportingCellStatus,
        &l1_rpt_params_ptr->rpt_cell_status) == SUCCESS)
      {
        l1_rpt_params_ptr->rpt_cell_status_incl = TRUE;

        /* Check if reporting cell status t is valid */
        if ((l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid cell status %d",l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat);
          return FAILURE;
        }
      }
      else
      {
        return FAILURE;
      }
    }

 
      /* Store reporting amount */
      l1_rpt_params_ptr->u.periodic_crit.rpt_amount = (tdsl1_amount_of_reporting_enum_type)
        TDSRRCMEAS_RET_REPORTING_AMOUNT(
        ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->periodicalReportingCriteria.reportingAmount);
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Rpting Amt is %d,Infin:0",l1_rpt_params_ptr->u.periodic_crit.rpt_amount);

    /* The spec indicates that 0 means UE behaviour unspecified */
    if (ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->
      periodicalReportingCriteria.reportingInterval == tdsrrc_ReportingIntervalLong_ril0)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Periodic Rpting 0 specified. use 250");
      l1_rpt_params_ptr->u.periodic_crit.rpt_interval = TDSL1_RPT_INTERVAL_250MS;
    }
    else
    {
      /* Store reporting interval */
      l1_rpt_params_ptr->u.periodic_crit.rpt_interval = (tdsl1_reporting_interval_enum_type)
        TDSRRCMEAS_RET_PERIODIC_REPORTING_INTERVAL
        (ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->periodicalReportingCriteria.reportingInterval);
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Rpting Intvl is %d msec",l1_rpt_params_ptr->u.periodic_crit.rpt_interval);
    break; /* Come out of switch */

  case T_tdsrrc_InterFreqReportCriteria_r4_noReporting:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Addtl Meas selected");

    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_NO_REPORTING;

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.noReporting,
        tdsrrc_ReportingCellStatusOpt,reportingCellStatus))
    {
      if (tdsrrcmeas_fill_no_of_reporting_cells(&ifreq_rpt_criteria_ptr->u.noReporting->reportingCellStatus,
        &l1_rpt_params_ptr->rpt_cell_status) == SUCCESS)
      {
        l1_rpt_params_ptr->rpt_cell_status_incl = TRUE;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Rpting Crit is Addtl");

        /* Check if reporting cell status t is valid */
        if ((l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid cell status %d",l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat);
          return FAILURE;
        }
      }
    }
    break;



  default:
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"InterFreq Rpt Crit %d not supported",ifreq_rpt_criteria_ptr->t);
    return FAILURE;

  }
  return SUCCESS;

}


/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM_R6

DESCRIPTION

    This function processes the release 6 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm_r6
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_freq_meas_params;
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  tdsrrcmeas_status_e_type result;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_FREQ;

  /* Meas Validity */
  tdsmeas_validity_enum_type meas_validity = TDSNOT_PRESENT;

  tdsrrc_MeasurementControl_r6_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 count = 0;

  uint8 cnt =  0;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Assign default values to l1_inter_freq_meas_params */
  memset(&l1_inter_freq_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_freq_meas_params.meas_object.meas_type = TDSL1_INTER_FREQ;

    /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.r6.measurementControl_r6;

  /* Set Transaction Id and Meas Id */
  l1_inter_freq_meas_params.trans_id = tdstransaction_id;
  l1_inter_freq_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;



  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r6_setup)
  {

    /* Double check that it is INTER-FREQ Meas */
    if (rrc_mcm_ptr->measurementCommand.u.setup->t !=
        T_tdsrrc_MeasurementType_r6_interFrequencyMeasurement)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    /* Check Measurement Quantity present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r6,interFreqMeasQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:MeasQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    l1_inter_freq_meas_params.meas_object_info_included = TRUE;

    if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.setup->
      u.interFrequencyMeasurement->interFreqMeasQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
    {
      /* Set Meas Quantity to TRUE */
      l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
    }
    else
    {
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    /* Check Reporting Quantity */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r6,interFreqReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SETUP:RptQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }
    if(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r6_intraFreqReportingCriteria)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SETUP:inter freq rpt criteria set to Intra freq rpt criteria");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    /* Set Reporting quantity */
    tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->interFreqReportingQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


    l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;

    /* Store reporting mode to TRUE */
    l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

    l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Rpting Mode %d ACK:0",l1_inter_freq_meas_params.rpt_trans_mode);

    /* Process Reporting Criteria which is mandatory in case meas obj is present */

    if (tdsrrcifreq_process_reporting_criteria_r6(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria, &l1_inter_freq_meas_params.meas_object.u.inter_f) == FAILURE)
    {

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */

      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    else
    {
      l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

      mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);
    }



    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Addtl Meas spec for Meas %d",rrc_mcm_ptr->measurementIdentity);
     /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
		TDSRRC_MSG0(MSG_LEGACY_ERROR,"Called function has already sent Meas Ctrl Failure");
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */

    result = tdsrrcifreq_fill_inter_freq_info_r5(&rrc_mcm_ptr->measurementCommand.u.setup->u.
      interFrequencyMeasurement->interFreqCellInfoList,
      &l1_inter_freq_meas_params, NULL) ;
    /* Fill cell list, cell info */
    if (result == TDSRRCMEAS_FAIL)
    {
      /* Send Meas Ctrl Failure Message */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to fill Inter Freq");
      /* Send Meas Ctrl Failure Msg */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

      /* Set failure cause  which is IE value incorrect */
      rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
    else
    {
      if(result == TDSSFN_FAIL)
      {
              /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SFN-ind set as True");
      /* Send Meas Ctrl Failure Msg */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
      if (result == TDSRRCMEAS_UNSUPPORTED)
      {
        /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to fill Inter Freq");
        /* Send Meas Ctrl Failure Msg */
  
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
        return FAILURE;
      }

      l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;

    }
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
      tdsrrc_InterFrequencyMeasurement_r6,measurementValidity))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Meas Validity present");
      /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
      then assume it to be CELL_DCH. If network specifies validity to be other than CELL_DCH,
      the UE behaviour is unspecified. So we assume it to be CELL_DCH */
      meas_validity = TDSSTATE_DCH;
    }

    /* Now commit new meas params to RRC db.*/
    if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
    {

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to set MeasId in DB");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    /* Now commit addtl meas db */
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
    }

    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    /* Now send command to L1 */
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r6_modify)
  {
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

      l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    /* Set type to MODIFY */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r6_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Modify:InterFreq:MType absent");

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r6_IEs,
        additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
        }
      } /* End of if of Additional Meas Present */

    }
    else
    {
      /* Meas Type has already been verified */
      l1_inter_freq_meas_params.meas_object_info_included = TRUE;

      /* Check Measurement Quantity present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r6,interFreqMeasQuantity))
      {

        if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.
          u.interFrequencyMeasurement->interFreqMeasQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
        {
          /* Set Meas Quantity to TRUE */
          l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      } /* End of meas quantity present */

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r6,interFreqReportingQuantity))
      {

        /* Set Reporting quantity */
        tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->interFreqReportingQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;
      }
      if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.t  == T_tdsrrc_InterFreqReportCriteria_r6_intraFreqReportingCriteria) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.u.intraFreqReportingCriteria,
        & tdsrrc_IntraFreqReportingCriteria_r6,eventCriteriaList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Modify:rpt crit set to Intraf rpt crit and evnt list non null");

        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      /* Process Reporting Criteria */

      if (tdsrrcifreq_process_reporting_criteria_r6(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria, &l1_inter_freq_meas_params.meas_object.u.inter_f) == FAILURE)
      {

        if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r6_interFreqReportingCriteria) &&
         !((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria,
         tdsrrc_InterFreqReportingCriteria_r6,interFreqEventList)) &&
         (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria->interFreqEventList.n != 0)))
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }
      }
      else
      {
        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

        mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);


      }

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
		  TDSRRC_MSG0(MSG_LEGACY_ERROR,"Called function has already sent Meas Ctrl Failure");
          return FAILURE;
        }
        else
        {
          addtl_meas_incl = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
               addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
               &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);


            return FAILURE;

          }

        }
      } /* End of if of Additional Meas Present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:additional meas list absent");
      }

      /* Fill cell list, cell info */
      result = tdsrrcifreq_fill_inter_freq_info_r5(
        &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        interFreqCellInfoList, &l1_inter_freq_meas_params, NULL);
      if (result == TDSRRCMEAS_FAIL)
      {
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      else
      {
        if(result == TDSSFN_FAIL)
        {
                /* Send Meas Ctrl Failure Message */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SFN-ind set as True");
        /* Send Meas Ctrl Failure Msg */

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE;
        }
        else
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;
        }
      }

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
        tdsrrc_InterFrequencyMeasurement_r6,measurementValidity))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Meas Validity present");
        /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
        then assume it to be CELL_DCH. */
        meas_validity = TDSSTATE_DCH;
      }

      /* Now commit new meas params to RRC db.*/
      if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to set MeasId in DB");
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        /* Make the old CELL_INFO_LIST valid */

        for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));
        }

        return FAILURE;
      }

      /* Now commit addtl meas db */
      if (addtl_meas_incl == TRUE)
      {
        tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
      }

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    that this meas exists. So let us delete it from tdsinter_freq_meas_id_list
    Meas Identity, Transaction Id already set */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_freq_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_freq_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_FREQ_MEAS; count++)
    {
      if (tdsinter_freq_meas_id_list[count].meas_id == l1_inter_freq_meas_params.meas_id)
      {
        tdsinter_freq_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsinter_freq_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        tdsinter_freq_meas_id_list[count].mcm_owned = FALSE;
        tdsinter_freq_meas_id_list[count].meas_validity = TDSNOT_PRESENT;

        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
          tdsinter_freq_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;

        break;
      }
    }
  } /* End of else of release */

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending interFreq measurements from MCM to L1");
    return SUCCESS;
  }

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_freq_meas_params.trans_id = alloc_idx;
  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_freq_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);

  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_REPORTING_CRITERIA_R6

DESCRIPTION

    This function process the reporting criteria specified
  in Measurement Control Message

DEPENDENCIES

    None

RETURN VALUE

    Success or Failure depending upon processing of reporting criteria

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_reporting_criteria_r6
(
 tdsrrc_InterFreqReportCriteria_r6* ifreq_rpt_criteria_ptr,
 tdsl1_inter_freq_meas_ctrl_struct_type* l1_rpt_params_ptr
)
{
  switch(ifreq_rpt_criteria_ptr->t)
  {
  case T_tdsrrc_InterFreqReportCriteria_r6_interFreqReportingCriteria:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Event Rpt Crit");

    l1_rpt_params_ptr->u.inter_rpt_crit.num_event = 0;

    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria,
      & tdsrrc_InterFreqReportingCriteria_r6,interFreqEventList)) &&
      (ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria->interFreqEventList.n != 0))
    {
      if (tdsrrcifreq_process_event_criteria_r6(ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria,
        &l1_rpt_params_ptr->u.inter_rpt_crit) == FAILURE)
      {
        return FAILURE;
      }

    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"InterFreq Event List Empty");
      return FAILURE;
    }

    /* Inter-Freq Events successfully processed */
    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_EVT_TRIG;

    break;

  case T_tdsrrc_InterFreqReportCriteria_r6_intraFreqReportingCriteria:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Intra Freq Event Rpt Crit");

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting rpt crit to TDSL1_INTER_FREQ_NO_CHANGE");
    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
    break;

  case T_tdsrrc_InterFreqReportCriteria_r6_periodicalReportingCriteria:

    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_PERIODIC;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Perodic Rpt Crit");

    l1_rpt_params_ptr->rpt_cell_status_incl = FALSE;

    /* Set reporting cells */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria,
      tdsrrc_PeriodicalWithReportingCellStatus,reportingCellStatus))
    {
      if (tdsrrcmeas_fill_no_of_reporting_cells(&ifreq_rpt_criteria_ptr->u.
        periodicalReportingCriteria->reportingCellStatus,
        &l1_rpt_params_ptr->rpt_cell_status) == SUCCESS)
      {
        l1_rpt_params_ptr->rpt_cell_status_incl = TRUE;

        /* Check if reporting cell status t is valid */
        if ((l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid cell status %d",l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat);
          return FAILURE;
        }
      }
      else
      {
        return FAILURE;
      }
    }


      /* Store reporting amount */
      l1_rpt_params_ptr->u.periodic_crit.rpt_amount = (tdsl1_amount_of_reporting_enum_type)
        TDSRRCMEAS_RET_REPORTING_AMOUNT(
        ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->periodicalReportingCriteria.reportingAmount);
    

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Rpting Amt is %d,Infin:0",l1_rpt_params_ptr->u.periodic_crit.rpt_amount);

    /* The spec indicates that 0 means UE behaviour unspecified */
    if (ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->
      periodicalReportingCriteria.reportingInterval == tdsrrc_ReportingIntervalLong_ril0)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Periodic Rpting 0 specified. use 250");
      l1_rpt_params_ptr->u.periodic_crit.rpt_interval = TDSL1_RPT_INTERVAL_250MS;
    }
    else
    {
      /* Store reporting interval */
      l1_rpt_params_ptr->u.periodic_crit.rpt_interval = (tdsl1_reporting_interval_enum_type)
        TDSRRCMEAS_RET_PERIODIC_REPORTING_INTERVAL
        (ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->periodicalReportingCriteria.reportingInterval);
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Rpting Intvl is %d msec",l1_rpt_params_ptr->u.periodic_crit.rpt_interval);
    break; /* Come out of switch */

  case T_tdsrrc_InterFreqReportCriteria_r6_noReporting:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Addtl Meas selected");

    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_NO_REPORTING;

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.noReporting,
        tdsrrc_ReportingCellStatusOpt,reportingCellStatus))
    {
      if (tdsrrcmeas_fill_no_of_reporting_cells(&ifreq_rpt_criteria_ptr->u.noReporting->reportingCellStatus,
        &l1_rpt_params_ptr->rpt_cell_status) == SUCCESS)
      {
        l1_rpt_params_ptr->rpt_cell_status_incl = TRUE;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Rpting Crit is Addtl");

        /* Check if reporting cell status t is valid */
        if ((l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid cell status %d",l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat);
          return FAILURE;
        }
      }
    }
    break;



  default:
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"InterFreq Rpt Crit %d not supported",ifreq_rpt_criteria_ptr->t);
    return FAILURE;

  }
  return SUCCESS;

}

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_EVENT_CRITERIA_R6

DESCRIPTION

    This function processes the event criteria received in a Measurement Control Message
  and stores in the format that is sent to L1

DEPENDENCIES

    None

RETURN VALUE

    FAILURE: If event criteria is invalid
    SUCCESS: If event criteria is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_event_criteria_r6
(
  tdsrrc_InterFreqReportingCriteria_r6* rrc_event_criteria_ptr,
  tdsl1_inter_freq_report_crit_struct_type* l1_event_criteria_ptr
)
{
  /* Store the list in a local pointer */
  struct tdsrrc_InterFreqEventList_r6* ifreq_event_list_ptr = NULL;

  uint8 cnt = 0, count = 0;

  /* To satisfy LINT */
  uint32 tmp = 0,idx=0;

  struct tdsrrc_NonUsedFreqWList_r6 * local_2a_non_used_param_ptr = NULL;
  struct tdsrrc_NonUsedFreqParameterList_r6 * local_non_used_param_ptr = NULL;

  ifreq_event_list_ptr = &rrc_event_criteria_ptr->interFreqEventList;

  if (ifreq_event_list_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Event Criteria list empty");
    return FAILURE;
  }

  while ((ifreq_event_list_ptr->n > idx) && (cnt < TDSL1_MAX_MEAS_EVENT))
  {
    /* Get Event Id */
    l1_event_criteria_ptr->evt[cnt].evt_id =
      TDSRET_INTER_FREQ_EVENT_TYPE_R6(ifreq_event_list_ptr->elem[idx].t);

    l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;
    l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = 0;

    switch(l1_event_criteria_ptr->evt[cnt].evt_id)
    {

    case TDSL1_INTER_FREQ_EVENT_2A:
      /* Change of best frequency. */
      l1_event_criteria_ptr->evt[cnt].w_used = (uint16)
        (ifreq_event_list_ptr->elem[idx].u.event2a->usedFreqW);

      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2a->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2a->timeToTrigger);

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2a,
        tdsrrc_Event2a_r6,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2a->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }
      /* Get Threshold Non-Used Freq */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2a,
        tdsrrc_Event2a_r6,nonUsedFreqParameterList))
      {
        local_2a_non_used_param_ptr =
          &ifreq_event_list_ptr->elem[idx].u.event2a->nonUsedFreqParameterList;

        while ((local_2a_non_used_param_ptr->n > count) && (count < TDSL1_MAX_NON_USED_FREQ))
        {
          /* Get non used threshold freq */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh
            =  0;
          /* Non used f */
          tmp = local_2a_non_used_param_ptr->elem[count];
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].w_non_used_f =
            (uint16) tmp;

          count++;
        } /* End of while */

        l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = count;
        count = 0;

      }
      break;

    case TDSL1_INTER_FREQ_EVENT_2B:
    /* The estimated quality of the currently used frequency is below a certain
    threshold and the estimated quality of a non-used frequency is above a certain
      threshold. */

      l1_event_criteria_ptr->evt[cnt].freq_thresh = (int16)
        (ifreq_event_list_ptr->elem[idx].u.event2b->usedFreqThreshold);

      l1_event_criteria_ptr->evt[cnt].w_used = (uint16)
        (ifreq_event_list_ptr->elem[idx].u.event2b->usedFreqW);

      /* Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2b->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2b->timeToTrigger);

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2b,
        tdsrrc_Event2b_r6,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2b->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }
      /* Get Threshold Non-Used Freq */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2b,
        tdsrrc_Event2b_r6,nonUsedFreqParameterList))
      {
        local_non_used_param_ptr =
          &ifreq_event_list_ptr->elem[idx].u.event2b->nonUsedFreqParameterList;

        while ((local_non_used_param_ptr->n > count) && (count < TDSL1_MAX_NON_USED_FREQ))
        {
          /* Get non used threshold freq */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh =
            (int16) local_non_used_param_ptr->elem[count].nonUsedFreqThreshold;
          /* Non used f */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].w_non_used_f =
            (uint16) (local_non_used_param_ptr->elem[count].nonUsedFreqW);

          count++;

        } /* End of while */

        l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = count;
        count = 0;
      }
      break;

    case TDSL1_INTER_FREQ_EVENT_2C:
      /* The estimated quality of a non-used frequency is above a certain threshold. */

      /* Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2c->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2c->timeToTrigger);

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2c,
        tdsrrc_Event2c_r6,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2c->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }
      /* Get Threshold Non-Used Freq */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
        ifreq_event_list_ptr->elem[idx].u.event2c,
        tdsrrc_Event2c_r6,nonUsedFreqParameterList))
      {
        local_non_used_param_ptr =
          &ifreq_event_list_ptr->elem[idx].u.event2c->nonUsedFreqParameterList;

        while ((local_non_used_param_ptr->n > count) && (count < TDSL1_MAX_NON_USED_FREQ))
        {
          /* Get non used threshold freq */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh =
            (int16) local_non_used_param_ptr->elem[count].nonUsedFreqThreshold;

          /* Non used f */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].w_non_used_f =
            (uint16) (local_non_used_param_ptr->elem[count].nonUsedFreqW);

          count++;

        } /* End of while */

        l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = count;
        count = 0;
      }
      break;

    case TDSL1_INTER_FREQ_EVENT_2E:
      /* The estimated quality of a non-used frequency is below a certain threshold. */

      /* Vivek Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2e->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2e->timeToTrigger);

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2e,
        tdsrrc_Event2e_r6,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2e->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }
      /* Get Threshold Non-Used Freq */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_event_list_ptr->elem[idx].u.event2e,
          tdsrrc_Event2e_r6,nonUsedFreqParameterList))
      {
        local_non_used_param_ptr =
          &ifreq_event_list_ptr->elem[idx].u.event2e->nonUsedFreqParameterList;

        while ((local_non_used_param_ptr->n > count) && (count < TDSL1_MAX_NON_USED_FREQ))
        {
          /* Get non used threshold freq */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].non_used_f_thresh =
            (int16) local_non_used_param_ptr->elem[count].nonUsedFreqThreshold;

          /* Non used f */
          l1_event_criteria_ptr->evt[cnt].non_used_f_parm_array[count].w_non_used_f =
            (uint16) (local_non_used_param_ptr->elem[count].nonUsedFreqW);

          count++;

        } /* End of while */

        l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = count;
        count = 0;
      }
      break;

    case TDSL1_INTER_FREQ_EVENT_2D:

      /* Get used freq threshold */
      l1_event_criteria_ptr->evt[cnt].freq_thresh = (int16)
        ifreq_event_list_ptr->elem[idx].u.event2d->usedFreqThreshold;

      /* Get W */
      l1_event_criteria_ptr->evt[cnt].w_used = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2d->usedFreqW;

      /* Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2d->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2d->timeToTrigger);

      l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
        ifreq_event_list_ptr->elem[idx].u.event2d,
        tdsrrc_Event2d_r6,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2d->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }

      /* Initialize no of non used freq params to 0 */
      l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = 0;

      break;

    case TDSL1_INTER_FREQ_EVENT_2F:

      /* Get used freq threshold */
      l1_event_criteria_ptr->evt[cnt].freq_thresh = (int16)
        ifreq_event_list_ptr->elem[idx].u.event2f->usedFreqThreshold;
      /* Get W */
      l1_event_criteria_ptr->evt[cnt].w_used = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2f->usedFreqW;

      /* Actual Value is IE*0.5. Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        ifreq_event_list_ptr->elem[idx].u.event2f->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_FREQ_TIME_TO_TRIGGER(
        ifreq_event_list_ptr->elem[idx].u.event2f->timeToTrigger);

      l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
        ifreq_event_list_ptr->elem[idx].u.event2f,
        tdsrrc_Event2f_r6,reportingCellStatus))
      {
        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &ifreq_event_list_ptr->elem[idx].u.event2f->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == SUCCESS)
        {
          l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;
        }
      }

      /* Initialize no of non used freq params to 0 */
      l1_event_criteria_ptr->evt[cnt].num_non_used_f_parm = 0;

      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"InterFreq Event %d not supported",l1_event_criteria_ptr->evt[cnt].evt_id);
      return FAILURE;

    } /* End of switch */

    if ((l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl == TRUE) &&
      ((l1_event_criteria_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2A) ||
      (l1_event_criteria_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2B) ||
      (l1_event_criteria_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2C) ||
      (l1_event_criteria_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2E)) &&
      ((l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
      (l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Event 2A/B/C/D,rpt cell stat %d",l1_event_criteria_ptr->evt[cnt].rpt_cell_status.rpt_cell_stat);

      return FAILURE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Reporting Cell Status valid");
    }

    cnt++;

    /* Increment Pointer count */
    idx++;
  } /* End of while */

  /* Store the count */
  l1_event_criteria_ptr->num_event = cnt;

  return SUCCESS;

}



/*==========================================================================
 FUNCTION   tdsrrcifreq_free_memory_allocated_for_reporting

DESCRIPTION

  This function takes care of deallocating the cell list  which was allocated by L1 when it is 
  sending a inter frequency measurement indication to RRC
DEPENDENCIES

    None
RETURN VALUE

    None
SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcifreq_free_memory_allocated_for_reporting
(
  tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
)
{
  int freq_idx=0;
  int cell_count=0;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deallocating the Cell list allocated by L1");
  if(l1_meas_report_ptr == NULL) 
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Measurement Pointer is NULL");
    return ;
  }
  if(l1_meas_report_ptr->u.inter_f_rpt.meas_results_included == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Measurement Results not Included");
    return;
  }
  for (freq_idx = 0;(freq_idx < l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.num_msmt_results)&&(freq_idx<TDSL1_MAX_NON_USED_FREQ);freq_idx++)
  {
    for ( cell_count = 0; ((cell_count < l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[freq_idx].num_cells)
      && ( cell_count < TDSL1_MAX_CELL_MEAS)); cell_count++ )
    {
      if (l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[freq_idx].cell[cell_count] != NULL)
      {
        tdsrrc_free_for_external_cmd(l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[freq_idx].cell[cell_count]);
        l1_meas_report_ptr->u.inter_f_rpt.inter_freq_measurements.msmt_results[freq_idx].cell[cell_count] = NULL;
      }
    }
  }
}


#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
/*===========================================================================

FUNCTION   RRCIFREQ_READ_RACH_RPT_INFO_SIB11

DESCRIPTION

    This function reads inter frequency rach repoting info from
    sib11. This function is called when the reading of inter frequency
    cell list from sib11 is successful and the rach reporting info is present
    in the sib11

DEPENDENCIES

    -

RETURN VALUE
FAILUE if rach repoting info reading fails
SUCCESS if rach reporting info rading is successful


SIDE EFFECTS

    None

===========================================================================*/
uecomdef_status_e_type tdsrrcifreq_read_rach_rpt_info_sib11(tdsrrc_InterFreqRACHReportingInfo *interFreqRACHReportingInfo
              ,tdsl1_inter_freq_rach_rpt_type *l1_inter_freq_rach_rpt)
{
  unit8 i;
  uecomdef_status_e_type status = FAILURE;
  l1_inter_freq_rach_rpt->inter_freq_rach_info_incl =FALSE;
  if(interFreqRACHReportingInfo->modeSpecificInfo.t == rrc_InterFreqRACHReportingInfo_modeSpecificInfo_tdd)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reading Inter Frequency Rach Reporting Info for SIB11");
    l1_inter_freq_rach_rpt->inter_freq_rach_info_incl = TRUE ;
    l1_inter_freq_rach_rpt->interf_rach_rpt_inf.rach_meas_quan_list_no=
              interFreqRACHReportingInfo->modeSpecificInfo.u.tdd->interFreqRepQuantityRACH_TDDList.n;
    for (i = 0; i < l1_inter_freq_rach_rpt->interf_rach_rpt_inf.rach_meas_quan_list_no; i++ )
    {
      TDSRRCMEAS_RET_INTER_FREQ_RACH_REP_QUAN(interFreqRACHReportingInfo->modeSpecificInfo.u.tdd->
                interFreqRepQuantityRACH_TDDList.elem[i]);
    }
    l1_inter_freq_rach_rpt->interf_rach_rpt_inf.interf_rach_reporting_thresh = interFreqRACHReportingInfo->interFreqRACHReportingThreshold;
    l1_inter_freq_rach_rpt->interf_rach_rpt_inf.interf_rach_max_reporting_cells =interFreqRACHReportingInfo->maxReportedCellsOnRACHinterFreq;
    tdsinterf_rach_report_crit = TDSAPPEND_RRC_MEAS_RACH_REPORT;
    status = SUCCESS;
  }
  else
  {
    /*FDD Not Supported*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD Not Supported");
    status = FAILURE;
  }
  return status;
}

/*===========================================================================

FUNCTION   RRCIFREQ_READ_RACH_RPT_INFO_SIB12

DESCRIPTION

    This function reads inter frequency rach repoting info from
    sib12. This function is called when the reading of inter frequency
    cell list from sib12 is successful and the rach reporting info is present
    in the sib12

DEPENDENCIES

    -

RETURN VALUE
FAILUE if rach repoting info reading fails
SUCCESS if rach reporting info rading is successful


SIDE EFFECTS

    None

===========================================================================*/
uecomdef_status_e_type tdsrrcifreq_read_rach_rpt_info_sib12(
 tdsrrc_InterFreqRACHReportingInfo *interFreqRACHReportingInfo
              ,tdsl1_inter_freq_rach_rpt_type * l1_inter_freq_rach_rpt )
{
  unit8 i;
  uecomdef_status_e_type status = FAILURE;
  if(interFreqRACHReportingInfo->modeSpecificInfo.t == rrc_InterFreqRACHReportingInfo_modeSpecificInfo_tdd)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reading Inter Frequency Rach Reporting Info for SIB12");
    l1_inter_freq_rach_rpt->inter_freq_rach_info_incl = TRUE ;
    l1_inter_freq_rach_rpt->interf_rach_rpt_inf.rach_meas_quan_list_no = 
            interFreqRACHReportingInfo->modeSpecificInfo.u.tdd->interFreqRepQuantityRACH_TDDList.n;
    for (i = 0; i < l1_inter_freq_rach_rpt->interf_rach_rpt_inf.rach_meas_quan_list_no; i++ )
    {
      TDSRRCMEAS_RET_INTER_FREQ_RACH_REP_QUAN(interFreqRACHReportingInfo->modeSpecificInfo.u.tdd->
                interFreqRepQuantityRACH_TDDList.elem[i]);
    }
    l1_inter_freq_rach_rpt->interf_rach_rpt_inf.interf_rach_reporting_thresh = interFreqRACHReportingInfo->interFreqRACHReportingThreshold;
    l1_inter_freq_rach_rpt->interf_rach_rpt_inf.interf_rach_max_reporting_cells =interFreqRACHReportingInfo->maxReportedCellsOnRACHinterFreq;
    tdsinterf_rach_report_crit = TDSAPPEND_RRC_MEAS_RACH_REPORT;
    status = SUCCESS;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD Not Supported");
    status = FAILURE;
  }
  return status;
}
/*===========================================================================

FUNCTION   RRCIFREQ_POPULATE_RACH_RPT-INFO

DESCRIPTION

    This function populates the meas_ptr with the rach reporting info 
    which is stored in tdsrrcifreq_inter_freq_rach_rpt
    This is called from rrcmeas when it is sending interfrequency cell list as
    state transition parameters to L1

DEPENDENCIES


RETURN VALUE
None

SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcifreq_populate_rach_rpt_info(tdsl1_meas_parm_type * meas_ptr)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Populating Rach Rpt info along with state transition parameters");
  meas_ptr->inter_freq_rach_info_incl = tdsrrcifreq_inter_freq_rach_rpt.inter_freq_rach_info_incl;
  meas_ptr->interf_rach_rpt_inf = tdsrrcifreq_inter_freq_rach_rpt.interf_rach_rpt_inf;
}
/*===========================================================================

FUNCTION RRCIFREQ_APPEND_MEAS_RPT_ON_RACH

DESCRIPTION

 This function appends the measured results to RACH messages if
 requested by the UTRAN. First, it checks if the
 meas report can be appended to the measge. If true,
 then it invokes a tdsl1mcmd_process_immediate_interf_meas_req to get
 intra-freq meas reports from L1.

DEPENDENCIES

 RACH reporting must have been activated by UTRAN in sib11 or sib12

RETURN VALUE

 FAILURE if RACH report not appended
 SUCCESS if RACH report appended


SIDE EFFECTS

 None

===========================================================================*/
uecomdef_status_e_type  tdsrrcifreq_append_meas_rpt_on_rach (tdsrrc_MeasuredResultsOnRACHinterFreq* rrcifreq_rach_report_ptr,
    tdsrrc_proc_e_type proc_id)
{
  tdsl1_immediate_meas_type l1_rach_report;
  struct tdsrrc_InterFreqRACHRepCellsList* list=NULL;
  int reported_cell_count =0;
  int i,j;
  uint16 cell_id_list[TDSL1_RACH_INTERF_NSET_MAX];
  uint16 cell_id;

  l1_rach_report.num_interf_freqs = 0;
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RACH rpt requested by procedure id %d",(uint32)proc_id);
  
  if (!tdsl1m_state_is_fach())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L1 State is not FACH");
    return FAILURE;
  }
  if(tdsinterf_rach_report_crit == TDSNO_RRC_MEAS_RACH_REPORT)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RACH criteria indicates No Report");
    return FAILURE;
  }
#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  TDS_TASKLOCK();
  /* Call L1 fn to get ARCH meas report */
  tdsl1mcmd_process_immediate_interf_meas_req(&l1_rach_report);
  /**/
  TDS_TASKFREE();
#else
  rrcff_l1_process_immediate_interf_meas_req(&l1_rach_report);
#endif
  if(l1_rach_report.num_interf_freqs== 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L1 Reported no frequencies");
    return FAILURE;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"L1 Reported %d  frequencies",l1_rach_report.num_interf_freqs);
  for(i=0;(i<l1_rach_report.num_interf_freqs) &&(i<TDSL1_MAX_NON_USED_FREQ);i++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"The Frequency is %d and number of cells on this frequency is %d",l1_rach_report.n_interf_cells[i].freq,l1_rach_report.n_interf_cells[i].num_cells);
  }
  for(i=0;i<TDSL1_RACH_INTERF_NSET_MAX;i++)
  {
    cell_id_list[i]=0;
  }
  for(i=0;(i<TDSL1_MAX_NON_USED_FREQ) && (i < l1_rach_report.num_interf_freqs) && (reported_cell_count <TDSL1_RACH_INTERF_NSET_MAX);i++)
  {
    for(j=0;(j<TDSL1_RACH_INTERF_NSET_MAX)&&(j<l1_rach_report.n_interf_cells[i].num_cells )&&( reported_cell_count <TDSL1_RACH_INTERF_NSET_MAX);j++)
    {
      cell_id = tdsrrcifreq_get_cellid(l1_rach_report.n_interf_cells[i].freq,l1_rach_report.n_interf_cells[i].cell_psc[j]);
      if(cell_id != TDSL1_MAX_CELL_MEAS)
      {
        cell_id_list[reported_cell_count++] = cell_id;
      }
    }
  }
  if(reported_cell_count ==0)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"number of cells reported is zero,error");
    return FAILURE;
  }
  rrcifreq_rach_report_ptr->interFreqCellIndication_SIB11 = tdsrrcsibdb_get_vtag_of_a_sib_for_serving_cell(tdsrrc_SIB11)%2;/*Take Value Tag from SIB*/
  if(tdssib12_present)
  {
    rrcifreq_rach_report_ptr->interFreqCellIndication_SIB12 = tdsrrcsibdb_get_vtag_of_a_sib_for_serving_cell(tdsrrc_SIB12)%2; /*Take Value Tag from SIB*/
  }
  else
  {
    rrcifreq_rach_report_ptr->interFreqCellIndication_SIB12 = 0; /*Set to Any Value*/
  }

  list->n = reported_cell_count;
  
  ALLOC_ASN1ARRAY1(&enc_ctxt, list, rrc_InterFreqCellID);
  
  if(list->elem == NULL)
  {
    ERR_FATAL("Failed to allocate memory",0,0,0);
  }

  rrcifreq_rach_report_ptr->interFreqRACHRepCellsList.n = list->n;
  rrcifreq_rach_report_ptr->interFreqRACHRepCellsList.elem = list->elem;

  for(i=0;i<reported_cell_count;i++)
  {    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell id reported is %d",cell_id_list[i]);
    list->elem[i] = cell_id_list[i];    
  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION RRCIFREQ_GET_CELLID

DESCRIPTION

  This function returns the cell id for a given frequency and CPID

DEPENDENCIES



RETURN VALUE

Cellid if the cell is found
otherwise TDSL1_MAX_CELL_MEAS

SIDE EFFECTS

 None

===========================================================================*/
uint16 tdsrrcifreq_get_cellid(uint16 frequency,uint16 cpi)
{
  uint16 count = 0;
  for(count=0;count <TDSL1_MAX_CELL_MEAS;count++)
  {
    if((tdsinter_freq_cell_list[count].dl_freq == frequency) && (tdsinter_freq_cell_list[count].cpi == cpi))
    {
     break;
    }
  }
  return count;
}

/*===========================================================================

FUNCTION RRCIFREQ_ADJUST_CCCH_MESSAGE_LENGTH

DESCRIPTION

  This function removes some cells from the interfrequency measurements cell list 
  to adjust the message length

DEPENDENCIES



RETURN VALUE

SUCCESS if the adjustment if size is successful
FAILURE otherwise

SIDE EFFECTS

 None

===========================================================================*/
uecomdef_status_e_type tdsrrcifreq_adjust_ccch_message_length(void *msg_ptr,uint32 pdu_size)
{

  struct tdsrrc_InterFreqRACHRepCellsList* list=NULL;
  struct  tdsrrc_CellUpdate* cell_update_ptr = NULL;
  struct tdsrrc_RRCConnectionRequest * rrc_connreq_ptr = NULL;
  int cell_count_to_be_removed=0;
  int reported_cell_count =0;
  int idx=0;
  uint32 offset =0;
  uint32 tb_size=0;
  uint32 tb_size_bytes=0;
  uint16 cell_id_list[TDSL1_RACH_INTERF_NSET_MAX];
  struct tdsrrc_UL_CCCH_Message *ccch_msg_ptr =(tdsrrc_UL_CCCH_Message *)msg_ptr;

  /*Get the Transport Block size by giving the PDU size as Zero*/
  tb_size = tdsrrcllc_calculate_ul_tm_ccch_rlc_size();
  if(tb_size == TDSRRCLLC_INVALID_RLC_SIZE)
  {
    return FAILURE;
  }
  tb_size_bytes = tb_size / 8;
  tb_size_bytes += (tb_size % 8) ? 1: 0;
  offset = pdu_size-tb_size_bytes;
  if(ccch_msg_ptr->message.t == T_tdsrrc_UL_CCCH_MessageType_cellUpdate)
  {
    cell_update_ptr=&(ccch_msg_ptr->message.u.cellUpdate);
    if((cell_update_ptr->m.laterNonCriticalExtensionsPresent)&&
        (cell_update_ptr->laterNonCriticalExtensions.m.v590NonCriticalExtensionsPresent) && 
        (cell_update_ptr->laterNonCriticalExtensions.v590NonCriticalExtensions.m.v690NonCriticalExtensionsPresent)&&
        (cell_update_ptr->laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.cellUpdate_v690ext.m.measuredResultsOnRACHinterFreqPresent))
    {
      list= &cell_update_ptr->laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
        cellUpdate_v690ext.measuredResultsOnRACHinterFreq.interFreqRACHRepCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Adjust CCCH Size for Cell Update Message");
    }
  }
  else if(ccch_msg_ptr->message.t == T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest)
  {
    rrc_connreq_ptr=&(ccch_msg_ptr->message.u.rrcConnectionRequest);
    if((rrc_connreq_ptr->m.v3d0NonCriticalExtensionsPresent)&&
        (rrc_connreq_ptr->v3d0NonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent) &&
        (rrc_connreq_ptr->v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent) &&
        (rrc_connreq_ptr->v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v690NonCriticalExtensionsPresent) && 
        (rrc_connreq_ptr->v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
          rrcConnectionRequest_v690ext.m.measuredResultsOnRACHinterFreqPresent))
    {
      list= &rrc_connreq_ptr->v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
        rrcConnectionRequest_v690ext.measuredResultsOnRACHinterFreq.interFreqRACHRepCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Adjust CCCH Size for Connection Request  Message");
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unexpected CCCH Message:Do Not Act on it");
    return FAILURE;
  }
  if(list->n != 0)
  {
    while(list->n > idx)
    {
      cell_id_list[reported_cell_count++] = list->elem[idx];
      idx++;
    }
    cell_count_to_be_removed = offset+1; /* TBD about how many cells to be removed*/

    list->n = 0;
    rtxMemFreePtr(&tdsenc_ctxt,list->elem);
    
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Reported Cells %d to be removed cells %d",reported_cell_count,cell_count_to_be_removed);
    if(reported_cell_count > cell_count_to_be_removed)
    {
      list->n = (reported_cell_count - cell_count_to_be_removed);
  
      ALLOC_ASN1ARRAY1(&tdsenc_ctxt, list, tdsrrc_InterFreqCellID);
  
      if(list->elem == NULL)
      {
        ERR_FATAL("Failed to allocate memory",0,0,0);
      }    

      if(ccch_msg_ptr->message.t == T_tdsrrc_UL_CCCH_MessageType_cellUpdate)
      {
        cell_update_ptr->laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.cellUpdate_v690ext.
              measuredResultsOnRACHinterFreq.interFreqRACHRepCellsList.n = list->n;
        cell_update_ptr->laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.cellUpdate_v690ext.
              measuredResultsOnRACHinterFreq.interFreqRACHRepCellsList.elem = list->elem;
      }
      else if(ccch_msg_ptr->message.t == T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest)
      {
        rrc_connreq_ptr->v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
            rrcConnectionRequest_v690ext.measuredResultsOnRACHinterFreq.interFreqRACHRepCellsList.n = list->n;
        rrc_connreq_ptr->v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
            rrcConnectionRequest_v690ext.measuredResultsOnRACHinterFreq.interFreqRACHRepCellsList.elem = list->elem;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unexpected CCCH Message:Do Not Act on it");
        return FAILURE;
      }
      for(idx=0;idx<(reported_cell_count - cell_count_to_be_removed) ;idx++)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell id reported is %d",cell_id_list[idx]);
        list->elem[idx] = cell_id_list[idx];        
      }
    }
    else
    {
      if(ccch_msg_ptr->message.t == T_tdsrrc_UL_CCCH_MessageType_cellUpdate)
      {
        cell_update_ptr->laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.cellUpdate_v690ext.m.measuredResultsOnRACHinterFreqPresent = 0;
      }
      else if(ccch_msg_ptr->message.t == T_rrc_UL_CCCH_MessageType_rrcConnectionRequest)
      {
        rrc_connreq_ptr->v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
            rrcConnectionRequest_v690ext.m.measuredResultsOnRACHinterFreqPresent = 0;
      }
      return SUCCESS;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No Cell List in RACH Report");
  }
  return SUCCESS;
}
#endif
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
/*===========================================================================

FUNCTION tdsrrcifreq_read_sib11bis

DESCRIPTION

  Reads SIB11bis from the current active serving cell for all the measurements.
  If SIB11bis information is incorrect, meas setup command is not send to L1/MAC
  and UE does not do any measurements..

DEPENDENCIES

  SIB must have been stored by the SIB procedure

RETURN VALUE

  Success if SIB11bis has been correctly read
  Failure if SIB11bis has not been correctly read

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_read_sib11bis
(
  tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib_ptr,
  boolean sib18_present,
  tdsrrc_PLMNsOfInterFreqCellsList *tmp_rrc_PLMNsOfInterFreqCellsList_ptr
)
{
  tdsrrc_SysInfoType11bis *sib11bis_ptr = NULL;
  tdsrrc_MeasurementControlSysInfoExtensionAddon_r5 *sib11bis_rel5_ptr = NULL;
  tdsrrc_NewInterFreqCellSI_List_HCS_RSCP_LCR_r4* inter_freq_meas_hcs_rscp_ptr = NULL;
  tdsrrc_NewInterFreqCellSI_List_HCS_ECN0_LCR_r4* inter_freq_meas_hcs_ecn0_ptr = NULL;
  tdsrrc_NewInterFreqCellSI_List_RSCP_LCR_r4* inter_freq_meas_rscp_ptr = NULL;
  tdsrrc_NewInterFreqCellSI_List_ECN0_LCR_r4* inter_freq_meas_ecn0_ptr = NULL;
  tdsrrc_MeasurementControlSysInfoExtensionAddon_r5_newInterFrequencyCellInfoListAddon_r5 *rel5_ext_ptr = NULL;
  uint16 cell_count = 0;
  uint32 cell_position = 0, idx = 0;

  rrc_plmn_identity_type tmp_plmn_identity;
  rrc_plmn_identity_type selected_plmn_identity;
  tdsrrc_int_cell_id_type  tmp_cell_id;
  tmp_plmn_identity.mcc[0] =  tmp_plmn_identity.mcc[1] = tmp_plmn_identity.mcc[2] = 0;
  tmp_plmn_identity.num_mnc_digits = 0;
  tmp_plmn_identity.mnc[0] =  tmp_plmn_identity.mnc[1] = 0;
  
  selected_plmn_identity.mcc[0] =  selected_plmn_identity.mcc[1] = selected_plmn_identity.mcc[2] = 0;
  selected_plmn_identity.num_mnc_digits = 0;
  selected_plmn_identity.mnc[0] =  selected_plmn_identity.mnc[1] = 0;
  
  if(!tdsrrcsibdb_is_sib11bis_present_in_srv_cell())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: SIB11bis is not present in Serving Cell");
    return FAILURE;
  }
  sib11bis_ptr = (tdsrrc_SysInfoType11bis *)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB11bis);
  if(sib11bis_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: SIB11bis ptr is NULL");
    return FAILURE;
  }
  if(!(sib11bis_ptr->m.measurementControlSysInfo_LCRPresent)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Mesaurement Control SysInfo is not Present in SIB11bis");
    return FAILURE;
  }
  if(sib11bis_ptr->m.measurementControlSysInfoExtensionAddon_r5Present)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Rel5 Measurement Control info present in SIB11bis");
    sib11bis_rel5_ptr = &sib11bis_ptr->measurementControlSysInfoExtensionAddon_r5;
    if(sib11bis_rel5_ptr->m.newInterFrequencyCellInfoListAddon_r5Present)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Rel5 Cell Selection Reselection info present for Inter frequency");
      rel5_ext_ptr = &sib11bis_rel5_ptr->newInterFrequencyCellInfoListAddon_r5;
    }
  }

  if(sib18_present == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: SIB18 is Absent/Ignored");
  }
  else if(tdsrrcccm_get_serv_plmn_cellid(&tmp_cell_id, &selected_plmn_identity) != TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    memscpy(&tmp_plmn_identity, sizeof(rrc_plmn_identity_type), &selected_plmn_identity, sizeof(rrc_plmn_identity_type));
  }
  else
  {
    sib18_present = FALSE;
  }

  if(sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.t == 
      rrc_MeasurementControlSysInfoExtension_use_of_HCS_hcs_used)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: HCS Used in SIB11bis");
    l1_inter_freq_sib_ptr->hcs_used = TRUE;
    switch(sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t)
    {
      /*RSCP is choosen and HCS is used*/
      case T_rrc_MeasurementControlSysInfoExtension_use_of_HCS_hcs_used_cellSelectQualityMeasure_cpich_RSCP:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: cellSelectQualityMeasure is RSCP ");
        if(!(sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_RSCP->m.newInterFreqCellListPresent))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter Frequency Cell list is absent");
          return FAILURE;
        }
        inter_freq_meas_hcs_rscp_ptr = &sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->
            cellSelectQualityMeasure.u.cpich_RSCP->newInterFreqCellList;
        cell_count = l1_inter_freq_sib_ptr->num_cell_add;
        while ((inter_freq_meas_hcs_rscp_ptr->n > idx) && (cell_count < L1_MAX_CELL_MEAS))
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max as per specs */

          /* Get the Freq Info */
          if (inter_freq_meas_hcs_rscp_ptr->elem[idx].m.frequencyInfoPresent)
          {
            if(inter_freq_meas_hcs_rscp_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t ==
              T_rrc_FrequencyInfo_modeSpecificInfo_fdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Cell fdd");

              idx++;              
              continue;
            }

            if (validate_frequency_info(&inter_freq_meas_hcs_rscp_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB11bis: Ignored cell with dl freq %d",inter_freq_meas_hcs_rscp_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);

              idx++;              
              continue;
            }

            /* Downlink Freq Present */
            if (inter_freq_meas_hcs_rscp_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB11bis: IFreq:Invalid DL ARFCN %d",inter_freq_meas_hcs_rscp_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Inter Freq Cell");
              
              idx++;
              continue;
            }

            /* Store DL freq */
            l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) inter_freq_meas_hcs_rscp_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

            TDSRRC_MSG3(MSG_LEGACY_MED,"SIB11bis: IFreq:DL Freq is %d", l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);

          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Cell. DL Freq absent");

              idx++;              
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }

          /* Set default values for the HCS neighboring cell info if it is first cell
          * else use the info from the previous cell */
          if(cell_count == 0)
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 115;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
          }
          else
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.hcs_prio;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.q_hcs;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time;

            if(l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time
              != 0)
            {
              l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.tmp_offset1 =
                l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.tmp_offset1;
            }
          }

          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_hcs_rscp_cell_info(&inter_freq_meas_hcs_rscp_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");

            idx++;           
            continue;
          }
          /* Check if inter-freq cell ID is present */
          if ( inter_freq_meas_hcs_rscp_ptr->elem[idx].m.interFreqCellIDPresent)
          {
            cell_position = inter_freq_meas_hcs_rscp_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */



          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpid in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi_incl =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi_incl;
            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
            if(sib18_present)
            {
              if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
              {
                if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
                {
                  tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                    &tmp_plmn_identity);
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    if((tdsrrccsp_check_for_eplmn(
                      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tmp_plmn_identity))) == FALSE)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CPID %d Non eq PLMN measure only in DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpid);
                      /* set the flag to FALSE */
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                    }
                  }
                }
                else
                {
                  /* use the previous value of the flag if PLMN id is not selected PLMN */
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                    if(cell_count > 0)
                    {
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                        l1_inter_freq_sib_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                    }
                  }
                }
              }
              else
              {
                /* PLMN list is shorter than the cell info list set flag to TRUE for
                * the remaining cells */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
              }
            }

            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */

          /* Get the next cell */
          idx++;
  
          if((cell_count == TDSL1_MAX_CELL_MEAS) &&
            (inter_freq_meas_hcs_rscp_ptr->n > idx))
          {
            /* If there are more new cells than can be accomodated in L1
            Cmd, print a message and hope L1 will change their capability
            sometime. Just pass the number that L1 can handle.
              */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: All Inter freq cells not included");
            break;
          }
        }/* End of while */
        
        /* Set number of added cells */
        l1_inter_freq_sib_ptr->num_cell_add = cell_count;

        if (l1_inter_freq_sib_ptr->num_cell_add == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:No new cell. SIB req not sent");
          return FAILURE;
        }
        
        break;

      /*ECNO is choosen and HCS is used*/
      case rrc_MeasurementControlSysInfoExtension_use_of_HCS_hcs_used_cellSelectQualityMeasure_cpich_Ec_N0:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: cellSelectQualityMeasure is ECNO ");
        if(!(sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_Ec_N0->m.newInterFreqCellListPresent))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter Frequency Cell list is absent");
          return FAILURE;
        }
        inter_freq_meas_hcs_ecn0_ptr = &sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->
            cellSelectQualityMeasure.u.cpich_Ec_N0->newInterFreqCellList;
        cell_count = l1_inter_freq_sib_ptr->num_cell_add;
        idx = 0;
        while ((inter_freq_meas_hcs_ecn0_ptr->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS))
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max as per specs */

          /* Get the Freq Info */
          if (inter_freq_meas_hcs_ecn0_ptr->elem[idx].m.frequencyInfoPresent)
          {
            if(inter_freq_meas_hcs_ecn0_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t ==
              T_rrc_FrequencyInfo_modeSpecificInfo_fdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Cell fdd");

              idx++;              
              continue;
            }

            if (tdsrrc_validate_frequency_info(&inter_freq_meas_hcs_ecn0_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB11bis: Ignored cell with dl freq %d",inter_freq_meas_hcs_ecn0_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);

              idx++;              
              continue;
            }

            /* Downlink Freq Present */
            if (inter_freq_meas_hcs_ecn0_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB11bis: IFreq:Invalid DL ARFCN %d",inter_freq_meas_hcs_ecn0_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Inter Freq Cell");
              
              idx++;
              continue;
            }

            /* Store DL freq */
            l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) inter_freq_meas_hcs_ecn0_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

            TDSRRC_MSG3(MSG_LEGACY_MED,"SIB11bis: IFreq:DL Freq is %d", l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);

          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Cell. DL Freq absent");

              idx++;              
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }

          /* Set default values for the HCS neighboring cell info if it is first cell
          * else use the info from the previous cell */
          if(cell_count == 0)
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 115;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
          }
          else
          {
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.hcs_prio;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.q_hcs;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time =
              l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time;

            if(l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.penalty_time
              != 0)
            {
              l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.tmp_offset1 =
                l1_inter_freq_sib_ptr->add_cell[cell_count - 1].cell_info.hcs_info.tmp_offset1;
            }
          }

          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_hcs_ecn0_cell_info(&inter_freq_meas_hcs_ecn0_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Ignore InterFreq Cell");

            idx++;            
            continue;
          }
          /* Check if inter-freq cell ID is present */
          if ( inter_freq_meas_hcs_ecn0_ptr->elem[idx].m.interFreqCellIDPresent)
          {
            cell_position = inter_freq_meas_hcs_ecn0_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */



          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi;

            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi_incl =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi_incl;
            
            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
            if(sib18_present)
            {
              if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
              {
                if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
                {
                  tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                    &tmp_plmn_identity);
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    if((tdsrrccsp_check_for_eplmn(
                      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tmp_plmn_identity))) == FALSE)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d Non eq PLMN measure only in DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                      /* set the flag to FALSE */
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                    }
                  }
                }
                else
                {
                  /* use the previous value of the flag if PLMN id is not selected PLMN */
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                    if(cell_count > 0)
                    {
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                        l1_inter_freq_sib_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                    }
                  }
                }
              }
              else
              {
                /* PLMN list is shorter than the cell info list set flag to TRUE for
                * the remaining cells */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
              }
            }

            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */

          /* Get the next cell */
          idx++;

          if((cell_count == TDSL1_MAX_CELL_MEAS) &&
            (inter_freq_meas_hcs_ecn0_ptr->n > idx))
          {
            /* If there are more new cells than can be accomodated in L1
            Cmd, print a message and hope L1 will change their capability
            sometime. Just pass the number that L1 can handle.
              */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: All Inter freq cells not included");
            break;
          }
        }/* End of while */
        /* Set number of added cells */
        l1_inter_freq_sib_ptr->num_cell_add = cell_count;

        if (l1_inter_freq_sib_ptr->num_cell_add == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:No new cell. SIB req not sent");
          return FAILURE;
        }
        break;
        
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"SIB11bis: Invalid cellSelectQualityMeasure :%d",sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t);
        return FAILURE;
    }
  }
  else
  {
    l1_inter_freq_sib_ptr->hcs_used = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: HCS is not Used in SIB11bis");
    switch(sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t)
    {
      case T_tdsrrc_MeasurementControlSysInfoExtension_LCR_r4_cellSelectQualityMeasure_cpich_RSCP:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: cellSelectQualityMeasure is RSCP ");
        if(!(sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_RSCP->m.newInterFreqCellListPresent))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter Frequency Cell list is absent");
          return FAILURE;
        }
        inter_freq_meas_rscp_ptr = &sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->
            cellSelectQualityMeasure.u.cpich_RSCP->newInterFreqCellList;
        cell_count = l1_inter_freq_sib_ptr->num_cell_add;
        while ( (inter_freq_meas_rscp_ptr->n > idx) &&
          (cell_count < TDSL1_MAX_CELL_MEAS) )
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max as per specs */
          l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
          l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 115;
          l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
  
          /* Get the Freq Info */
          if (inter_freq_meas_rscp_ptr->elem[idx].m.frequencyInfoPresent)
          {
            if (inter_freq_meas_rscp_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t ==
              T_rrc_FrequencyInfo_modeSpecificInfo_fdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Cell. FDD cell");

              idx++;              
              continue;
            }
  
            if (tdsrrc_validate_frequency_info(&inter_freq_meas_rscp_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB11bis: Ignored cell with dl freq %d",inter_freq_meas_rscp_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);

              idx++;              
              continue;
            }
  
            if (inter_freq_meas_rscp_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB11bis: IFreq:Invalid DL ARFCN %d",inter_freq_meas_rscp_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Inter Freq Cell");

              idx++;              
              continue;
            }
            /* Store DL freq */
            l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) inter_freq_meas_rscp_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
  
            TDSRRC_MSG3(MSG_LEGACY_MED,"SIB11bis: IFreq:DL Freq is %d", l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);
  
          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Cell. DL Freq absent");

              idx++;              
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }
  
          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_rscp_cell_info(&inter_freq_meas_rscp_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore InterFreq Cell");

            idx++;            
            continue;
          }
          /* Check if inter-freq cell ID is present */
          if ( inter_freq_meas_rscp_ptr->elem[idx].m.interFreqCellIDPresent)
          {
            cell_position = inter_freq_meas_rscp_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */
  
  
          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi;
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi_incl =
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi_incl;
  
            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
            if(sib18_present)
            {
              if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
              {
                if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
                {
                  tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                    &tmp_plmn_identity);
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    if((tdsrrccsp_check_for_eplmn(
                      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tmp_plmn_identity))) == FALSE)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d Non eq PLMN measure only in DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                      /* set the flag to FALSE */
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                    }
                  }
                }
                else
                {
                  /* use the previous value of the flag if PLMN id is not selected PLMN */
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                    if(cell_count > 0)
                    {
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                        l1_inter_freq_sib_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                    }
                  }
                }
              }
              else
              {
                /* PLMN list is shorter than the cell info list set flag to TRUE for
                * the remaining cells */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
              }
            }

            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */
  
          /* Get the next cell */
          idx++;
        } /* End of while */
        /* Set number of added cells */
        l1_inter_freq_sib_ptr->num_cell_add = cell_count;
        if (l1_inter_freq_sib_ptr->num_cell_add == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:No new cell. SIB req not sent");
          return FAILURE;
        }
        break;

        
      case T_tdsrrc_MeasurementControlSysInfoExtension_LCR_r4_cellSelectQualityMeasure_cpich_Ec_N0:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: cellSelectQualityMeasure is ECNO ");
        if(!(sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->m.newInterFreqCellListPresent))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter Frequency Cell list is absent");
          return FAILURE;
        }
        inter_freq_meas_ecn0_ptr = &sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->
            cellSelectQualityMeasure.u.cpich_Ec_N0->newInterFreqCellList;
        cell_count = l1_inter_freq_sib_ptr->num_cell_add;
        while ( (inter_freq_meas_ecn0_ptr->n > idx) &&
          (cell_count < TDSL1_MAX_CELL_MEAS) )
        {
        /* Pointer to next cell is Non-NULL and cell count
          hasn't exceeded the max as per specs */
          l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.hcs_prio = 0;
          l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.q_hcs = 115;
          l1_inter_freq_sib_ptr->add_cell[cell_count].cell_info.hcs_info.penalty_time = 0;
  
          /* Get the Freq Info */
          if (inter_freq_meas_ecn0_ptr->elem[idx].m.frequencyInfoPresent)
          {
            if (inter_freq_meas_ecn0_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t ==
              T_rrc_FrequencyInfo_modeSpecificInfo_fdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Cell. FDD cell");

              idx++;              
              continue;
            }
  
            if (tdsrrc_validate_frequency_info(&inter_freq_meas_ecn0_ptr->elem[idx].frequencyInfo) == FAILURE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB11bis: Ignored cell with dl freq %d",inter_freq_meas_ecn0_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);

              idx++;             
              continue;
            }
  
            if (inter_freq_meas_ecn0_ptr->elem[idx].
              frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
            {
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB11bis: IFreq:Invalid DL ARFCN %d",inter_freq_meas_ecn0_ptr->elem[idx].
                frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Inter Freq Cell");

              idx++;              
              continue;
            }
            /* Store DL freq */
            l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl = (uint16) inter_freq_meas_ecn0_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
  
            TDSRRC_MSG3(MSG_LEGACY_MED,"SIB11bis: IFreq:DL Freq is %d", l1_inter_freq_sib_ptr->add_cell[cell_count].
              freq_info.uarfcn_dl,0,0);
  
          }
          else
          {
            if (cell_count == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore Cell. DL Freq absent");

              idx++;              
              continue;
            }
            else
            {
              /* This implies that there is already one DL and UL freq present. Copy from there */
              memscpy(&l1_inter_freq_sib_ptr->add_cell[cell_count].freq_info,
                sizeof(tdsl1_freq_info_struct_type),
                &l1_inter_freq_sib_ptr->add_cell[cell_count-1].freq_info,
                sizeof(tdsl1_freq_info_struct_type));
            }
          }
  
          /* First store cell info so that cpi of this cell can be extracted */
          if (tdsrrcmeas_fill_ecn0_cell_info(&inter_freq_meas_ecn0_ptr->elem[idx].
            cellInfo, &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info, ((rel5_ext_ptr != NULL) ? ((rel5_ext_ptr->n > idx) ? (&rel5_ext_ptr->elem[idx]) : NULL) : NULL)) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: IFreq:Ignore InterFreq Cell");

            idx++;            
            continue;
          }
          /* Check if inter-freq cell ID is present */
          if ( inter_freq_meas_ecn0_ptr->elem[idx].m.interFreqCellIDPresent)
          {
            cell_position = inter_freq_meas_ecn0_ptr->elem[idx].interFreqCellID;
          } /* End of if of intra-freq cell id present */
          else
          {
            cell_position = TDSAVAILABLE_CELL_POSITION;
          } /* End of else of inter-freq cell id not present */
  
  
          if ( tdsrrcifreq_update_inter_freq_cell_list(
            &cell_position,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            cell_info.cpi,
            &l1_inter_freq_sib_ptr->add_cell[cell_count].
            freq_info.uarfcn_dl) == SUCCESS)
          {
            /* cpi sucessfully stored in the cell info list variable */
            /* Now store cpi in the sib11 in add cell for layer1 to use cpi */
            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi=
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi;

            l1_inter_freq_sib_ptr->add_cell[cell_count].cpi_incl=
              l1_inter_freq_sib_ptr->add_cell[cell_count].
              cell_info.cpi_incl;
  
            /* set the measure in PCH IDLE and FACH flag to TRUE */
            l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
            if(sib18_present)
            {
              if((tmp_rrc_PLMNsOfInterFreqCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterFreqCellsList_ptr->n > cell_count))
              {
                if(tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
                {
                  tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterFreqCellsList_ptr->elem[cell_count].plmn_Identity,
                    &tmp_plmn_identity);
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    if((tdsrrccsp_check_for_eplmn(
                      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tmp_plmn_identity))) == FALSE)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"cpi %d Non eq PLMN measure only in DCH",l1_inter_freq_sib_ptr->add_cell[cell_count].cpi);
                      /* set the flag to FALSE */
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                    }
                  }
                }
                else
                {
                  /* use the previous value of the flag if PLMN id is not selected PLMN */
                  if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
                  {
                    /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                    if(cell_count > 0)
                    {
                      l1_inter_freq_sib_ptr->add_cell[cell_count].measure_in_idle_pch_fach =
                        l1_inter_freq_sib_ptr->add_cell[cell_count - 1].measure_in_idle_pch_fach;
                    }
                  }
                }
              }
              else
              {
                /* PLMN list is shorter than the cell info list set flag to TRUE for
                * the remaining cells */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
              }
            }
            /* Increment cell count as add cell successfully stored */
            cell_count ++;
          }  /* End of if of rrcmeas_update_inter_freq_cell_list */
  
          /* Get the next cell */
          idx++;          
        } /* End of while */
        /* Set number of added cells */
        l1_inter_freq_sib_ptr->num_cell_add = cell_count;
        if (l1_inter_freq_sib_ptr->num_cell_add == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:No new cell. SIB req not sent");
          return FAILURE;
        }
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"SIB11bis: Invalid cellSelectQualityMeasure :%d",sib11bis_ptr->measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t);
        return FAILURE;
    }
  }
  return SUCCESS;
}
#endif /*FEATURE_TDSCDMA_SIB11BIS_SUPPORT*/

/*==========================================================================
 FUNCTION   tdsrrcifreq_free_memory_allocated_for_cell_list_in_meas_ind

DESCRIPTION

  This function takes care of deallocating the cell list  which was allocated by L1 when it is 
  sending a inter frequency measurement indication to RRC
DEPENDENCIES

    None
RETURN VALUE

    None
SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcifreq_free_memory_allocated_for_cell_list_in_meas_ind
(
  tdsrrc_cmd_type* cmd_ptr
)
{
  if (cmd_ptr->cmd.meas_ind.meas_rpt->meas_type == TDSL1_INTER_FREQ)
  {
    tdsrrcifreq_free_memory_allocated_for_reporting(cmd_ptr->cmd.meas_ind.meas_rpt);
  }
}

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM_R7

DESCRIPTION

    This function processes the release 7 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm_r7
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_freq_meas_params;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  tdsrrcmeas_status_e_type result;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_FREQ;

  /* Meas Validity */
  tdsmeas_validity_enum_type meas_validity = TDSNOT_PRESENT;

  tdsrrc_MeasurementControl_r7_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;
  
  tdsrrc_NewInterFreqCellList_v7b0ext *newInterFreqCellList_ptr = NULL;

  uint8 count = 0;

  uint8 cnt =  0;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Assign default values to l1_inter_freq_meas_params */
  memset(&l1_inter_freq_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_freq_meas_params.meas_object.meas_type = TDSL1_INTER_FREQ;

    /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.r7.measurementControl_r7;


  /* Set Transaction Id and Meas Id */
  l1_inter_freq_meas_params.trans_id = tdstransaction_id;
  l1_inter_freq_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;

  
  /* 10.3.7.13, REL7 add new IE "Intra-SecondaryFrequencyIndicator".TRUE means the inter frequencyneighbour cell 
       has a secondary frequency which is the same as the current working frequency of UE. */
  if ((dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.u.later_than_r4.
       criticalExtensions.u.criticalExtensions.u.r7.m.v7b0NonCriticalExtensionsPresent) &&
       (dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.u.later_than_r4.
       criticalExtensions.u.criticalExtensions.u.r7.v7b0NonCriticalExtensions.measurementControl_v7b0ext.
       m.newInterFreqCellListPresent))
  {
    newInterFreqCellList_ptr = &dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.
                 criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.
                 u.r7.v7b0NonCriticalExtensions.measurementControl_v7b0ext.newInterFreqCellList;
  }

  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r7_setup)
  {

    /* Double check that it is INTER-FREQ Meas */
    if (rrc_mcm_ptr->measurementCommand.u.setup->t !=
        T_tdsrrc_MeasurementType_r7_interFrequencyMeasurement)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    /* Check Measurement Quantity present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r7,interFreqMeasQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:MeasQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    l1_inter_freq_meas_params.meas_object_info_included = TRUE;

    if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.setup->
      u.interFrequencyMeasurement->interFreqMeasQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
    {
      /* Set Meas Quantity to TRUE */
      l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
    }
    else
    {
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    /* Check Reporting Quantity */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r7,interFreqReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SETUP:RptQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }
    if(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r7_intraFreqReportingCriteria)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SETUP:inter freq rpt criteria set to Intra freq rpt criteria");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    /* Set Reporting quantity */
    tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->interFreqReportingQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


    l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;

    /* Store reporting mode to TRUE */
    l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

    l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Rpting Mode %d ACK:0",l1_inter_freq_meas_params.rpt_trans_mode);

    /* Process Reporting Criteria which is mandatory in case meas obj is present */

    if (tdsrrcifreq_process_reporting_criteria_r7(&rrc_mcm_ptr->measurementCommand.u.setup->u.
           interFrequencyMeasurement->reportCriteria, 
          &l1_inter_freq_meas_params.meas_object.u.inter_f) == FAILURE)
    {

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */

      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    else
    {
      l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

      mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);
    }



    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Addtl Meas spec for Meas %d",rrc_mcm_ptr->measurementIdentity);
     /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
		TDSRRC_MSG0(MSG_LEGACY_ERROR," Called function has already sent Meas Ctrl Failure ");
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */

    result = tdsrrcifreq_fill_inter_freq_info_r5(&rrc_mcm_ptr->measurementCommand.u.setup->u.
      interFrequencyMeasurement->interFreqCellInfoList,
      &l1_inter_freq_meas_params, newInterFreqCellList_ptr) ;
    /* Fill cell list, cell info */
    if (result == TDSRRCMEAS_FAIL)
    {
      /* Send Meas Ctrl Failure Message */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to fill Inter Freq");
      /* Send Meas Ctrl Failure Msg */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

      /* Set failure cause  which is IE value incorrect */
      rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
    else
    {
      if(result == TDSSFN_FAIL)
      {
              /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SFN-ind set as True");
      /* Send Meas Ctrl Failure Msg */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
      if (result == TDSRRCMEAS_UNSUPPORTED)
      {
        /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to fill Inter Freq");
        /* Send Meas Ctrl Failure Msg */
  
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
        return FAILURE;
      }

      l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;

    }
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
      tdsrrc_InterFrequencyMeasurement_r7,measurementValidity))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Meas Validity present");
      /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
      then assume it to be CELL_DCH. If network specifies validity to be other than CELL_DCH,
      the UE behaviour is unspecified. So we assume it to be CELL_DCH */
      meas_validity = TDSSTATE_DCH;
    }

    /* Now commit new meas params to RRC db.*/
    if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
    {

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to set MeasId in DB");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    /* Now commit addtl meas db */
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
    }

    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    /* Now send command to L1 */
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r7_modify)
  {
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

      l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    /* Set type to MODIFY */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r7_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Modify:InterFreq:MType absent");

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
        }
      } /* End of if of Additional Meas Present */

    }
    else
    {
      /* Meas Type has already been verified */
      l1_inter_freq_meas_params.meas_object_info_included = TRUE;

      /* Check Measurement Quantity present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r7,interFreqMeasQuantity))
      {

        if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.
          u.interFrequencyMeasurement->interFreqMeasQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
        {
          /* Set Meas Quantity to TRUE */
          l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      } /* End of meas quantity present */

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r7,interFreqReportingQuantity))
      {

        /* Set Reporting quantity */
        tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->interFreqReportingQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;
      }
      if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.t  == T_tdsrrc_InterFreqReportCriteria_r7_intraFreqReportingCriteria) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.u.intraFreqReportingCriteria,
        tdsrrc_IntraFreqReportingCriteria_r7,eventCriteriaList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Modify:rpt crit set to Intraf rpt crit and evnt list non null");

        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      /* Process Reporting Criteria */

      if (tdsrrcifreq_process_reporting_criteria_r7(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
             interFrequencyMeasurement->reportCriteria,
             &l1_inter_freq_meas_params.meas_object.u.inter_f) == FAILURE)
      {

        if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r7_interFreqReportingCriteria) &&
         !((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria,
         tdsrrc_InterFreqReportingCriteria_r6,interFreqEventList)) &&
         (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria->interFreqEventList.n != 0)))
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }
      }
      else
      {
        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

        mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);

      }

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFREQ:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          addtl_meas_incl = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
               addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
               &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);


            return FAILURE;

          }

        }
      } /* End of if of Additional Meas Present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:additional meas list absent");
      }

      /* Fill cell list, cell info */
      result = tdsrrcifreq_fill_inter_freq_info_r5(
        &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        interFreqCellInfoList, 
        &l1_inter_freq_meas_params, newInterFreqCellList_ptr);
      if (result == TDSRRCMEAS_FAIL)
      {
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      else
      {
        if(result == TDSSFN_FAIL)
        {
                /* Send Meas Ctrl Failure Message */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:SFN-ind set as True");
        /* Send Meas Ctrl Failure Msg */

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE;
        }
        else
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;
        }
      }

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
        tdsrrc_InterFrequencyMeasurement_r7,measurementValidity))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Meas Validity present");
        /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
        then assume it to be CELL_DCH. */
        meas_validity = TDSSTATE_DCH;
      }

      /* Now commit new meas params to RRC db.*/
      if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IFreq:Unable to set MeasId in DB");
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        /* Make the old CELL_INFO_LIST valid */

        for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));
        }

        return FAILURE;
      }

      /* Now commit addtl meas db */
      if (addtl_meas_incl == TRUE)
      {
        tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Commit Addtl Meas to RRC DB");
      }

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    that this meas exists. So let us delete it from tdsinter_freq_meas_id_list
    Meas Identity, Transaction Id already set */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_freq_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_freq_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_FREQ_MEAS; count++)
    {
      if (tdsinter_freq_meas_id_list[count].meas_id == l1_inter_freq_meas_params.meas_id)
      {
        tdsinter_freq_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsinter_freq_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        tdsinter_freq_meas_id_list[count].mcm_owned = FALSE;
        tdsinter_freq_meas_id_list[count].meas_validity = TDSNOT_PRESENT;

        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
          tdsinter_freq_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;

        break;
      }
    }
  } /* End of else of release */

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending interFreq measurements from MCM to L1");
    return SUCCESS;
  }

  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_freq_meas_params.trans_id = alloc_idx;

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */

  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_freq_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);

  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_REPORTING_CRITERIA_R7

DESCRIPTION

    This function process the reporting criteria specified
  in Measurement Control Message

DEPENDENCIES

    None

RETURN VALUE

    Success or Failure depending upon processing of reporting criteria

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_reporting_criteria_r7
(
 tdsrrc_InterFreqReportCriteria_r7* ifreq_rpt_criteria_ptr,
 tdsl1_inter_freq_meas_ctrl_struct_type* l1_rpt_params_ptr
)
{
  switch(ifreq_rpt_criteria_ptr->t)
  {
  case T_tdsrrc_InterFreqReportCriteria_r7_interFreqReportingCriteria:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Event Rpt Crit");

    l1_rpt_params_ptr->u.inter_rpt_crit.num_event = 0;

    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria,
      & tdsrrc_InterFreqReportingCriteria_r6,interFreqEventList)) &&
      (ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria->interFreqEventList.n != 0))
    {
      if (tdsrrcifreq_process_event_criteria_r6(ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria,
        &l1_rpt_params_ptr->u.inter_rpt_crit) == FAILURE)
      {
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"InterFreq Event List Empty");
      return FAILURE;
    }

    /* Inter-Freq Events successfully processed */
    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_EVT_TRIG;

    break;

  case T_tdsrrc_InterFreqReportCriteria_r7_intraFreqReportingCriteria:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Intra Freq Event Rpt Crit");

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting rpt crit to TDSL1_INTER_FREQ_NO_CHANGE");
    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
    break;

  case T_tdsrrc_InterFreqReportCriteria_r7_periodicalReportingCriteria:

    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_PERIODIC;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Perodic Rpt Crit");

    l1_rpt_params_ptr->rpt_cell_status_incl = FALSE;

    /* Set reporting cells */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria,
      tdsrrc_PeriodicalWithReportingCellStatus,reportingCellStatus))
    {
      if (tdsrrcmeas_fill_no_of_reporting_cells(&ifreq_rpt_criteria_ptr->u.
        periodicalReportingCriteria->reportingCellStatus,
        &l1_rpt_params_ptr->rpt_cell_status) == SUCCESS)
      {
        l1_rpt_params_ptr->rpt_cell_status_incl = TRUE;

        /* Check if reporting cell status t is valid */
        if ((l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid cell status %d",l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat);
          return FAILURE;
        }
      }
      else
      {
        return FAILURE;
      }
    }

    /* Check if reporting amount present */
 
      /* Store reporting amount */
      l1_rpt_params_ptr->u.periodic_crit.rpt_amount = (tdsl1_amount_of_reporting_enum_type)
        TDSRRCMEAS_RET_REPORTING_AMOUNT(
        ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->periodicalReportingCriteria.reportingAmount);
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Rpting Amt is %d,Infin:0",l1_rpt_params_ptr->u.periodic_crit.rpt_amount);

    /* The spec indicates that 0 means UE behaviour unspecified */
    if (ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->
      periodicalReportingCriteria.reportingInterval == tdsrrc_ReportingIntervalLong_ril0)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Periodic Rpting 0 specified. use 250");
      l1_rpt_params_ptr->u.periodic_crit.rpt_interval = TDSL1_RPT_INTERVAL_250MS;
    }
    else
    {
      /* Store reporting interval */
      l1_rpt_params_ptr->u.periodic_crit.rpt_interval = (tdsl1_reporting_interval_enum_type)
        TDSRRCMEAS_RET_PERIODIC_REPORTING_INTERVAL
        (ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->periodicalReportingCriteria.reportingInterval);
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Rpting Intvl is %d msec",l1_rpt_params_ptr->u.periodic_crit.rpt_interval);
    break; /* Come out of switch */

  case T_tdsrrc_InterFreqReportCriteria_r7_noReporting:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Addtl Meas selected");

    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_NO_REPORTING;

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.noReporting,
        tdsrrc_ReportingCellStatusOpt,reportingCellStatus))
    {
      if (tdsrrcmeas_fill_no_of_reporting_cells(&ifreq_rpt_criteria_ptr->u.noReporting->reportingCellStatus,
        &l1_rpt_params_ptr->rpt_cell_status) == SUCCESS)
      {
        l1_rpt_params_ptr->rpt_cell_status_incl = TRUE;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Rpting Crit is Addtl");

        /* Check if reporting cell status t is valid */
        if ((l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid cell status %d",l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat);
          return FAILURE;
        }
      }
    }
    break;



  default:
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"InterFreq Rpt Crit %d not supported",ifreq_rpt_criteria_ptr->t);
    return FAILURE;

  }
  return SUCCESS;

}


#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION   RRCIFREQ_PROCESS_MCM_R8

DESCRIPTION
    This function processes the release 7 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES
    None.

RETURN VALUE
    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS
    None
==========================================================================*/
uecomdef_status_e_type tdsrrcifreq_process_mcm_r8
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_freq_meas_params;
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  tdsrrcmeas_status_e_type result;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_FREQ;

  /* Meas Validity */
  tdsmeas_validity_enum_type meas_validity = TDSNOT_PRESENT;

  tdsrrc_MeasurementControl_r8_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 count = 0;

  uint8 cnt =  0;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Assign default values to l1_inter_freq_meas_params */
  memset(&l1_inter_freq_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_freq_meas_params.meas_object.meas_type = TDSL1_INTER_FREQ;

    /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.measurementControl_r8;

  /* Set Transaction Id and Meas Id */
  l1_inter_freq_meas_params.trans_id = tdstransaction_id;
  l1_inter_freq_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;


  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r8_setup)
  {

    /* Double check that it is INTER-FREQ Meas */
    if (rrc_mcm_ptr->measurementCommand.u.setup->t !=
        T_tdsrrc_MeasurementType_r8_interFrequencyMeasurement)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFREQ:UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
              tdsrrc_InterFrequencyMeasurement_r8,adjacentFrequencyIndex))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFREQ:adjacentFrequencyIndex for FDD UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    /* Check Measurement Quantity present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r8,interFreqMeasQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:SETUP:MeasQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    l1_inter_freq_meas_params.meas_object_info_included = TRUE;

    if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.setup->
      u.interFrequencyMeasurement->interFreqMeasQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
    {
      /* Set Meas Quantity to TRUE */
      l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
    }
    else
    {
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    /* Check Reporting Quantity */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r8,interFreqReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:SETUP:RptQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }
    if(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r7_intraFreqReportingCriteria)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:SETUP:inter freq rpt criteria set to Intra freq rpt criteria");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    /* Set Reporting quantity */
    tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->interFreqReportingQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


    l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;

    /* Store reporting mode to TRUE */
    l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

    l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFREQ:Rpting Mode %d ACK:0",l1_inter_freq_meas_params.rpt_trans_mode);

    /* Process Reporting Criteria which is mandatory in case meas obj is present */

    if (tdsrrcifreq_process_reporting_criteria_r7(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria, &l1_inter_freq_meas_params.meas_object.u.inter_f) == FAILURE)
    {

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */

      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    else
    {
      l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

      mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);
    }



    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFreq:Addtl Meas spec for Meas %d",rrc_mcm_ptr->measurementIdentity);
     /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */


    result = tdsrrcifreq_fill_inter_freq_info_r8(&rrc_mcm_ptr->measurementCommand.u.setup->u.
      interFrequencyMeasurement->interFreqCellInfoList,
      &l1_inter_freq_meas_params) ;
    /* Fill cell list, cell info */
    if (result == TDSRRCMEAS_FAIL)
    {
      /* Send Meas Ctrl Failure Message */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:Unable to fill Inter Freq");
      /* Send Meas Ctrl Failure Msg */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

      /* Set failure cause  which is IE value incorrect */
      rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
    else
    {
      if(result == TDSSFN_FAIL)
      {
              /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:SFN-ind set as True");
      /* Send Meas Ctrl Failure Msg */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
      if (result == TDSRRCMEAS_UNSUPPORTED)
      {
        /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:Unable to fill Inter Freq");
        /* Send Meas Ctrl Failure Msg */
  
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
        return FAILURE;
      }

      l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;

    }
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Sending Meas Ctrl Failure");

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
      tdsrrc_InterFrequencyMeasurement_r8,measurementValidity))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Meas Validity present");
      /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
      then assume it to be CELL_DCH. If network specifies validity to be other than CELL_DCH,
      the UE behaviour is unspecified. So we assume it to be CELL_DCH */
      meas_validity = TDSSTATE_DCH;
    }

    /* Now commit new meas params to RRC db.*/
    if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
    {

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:Unable to set MeasId in DB");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    /* Now commit addtl meas db */
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Commit Addtl Meas to RRC DB");
    }

    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    /* Now send command to L1 */
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r8_modify)
  {
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

      l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    /* Set type to MODIFY */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r8_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Modify:InterFreq:MType absent");

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFreq:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Commit Addtl Meas to RRC DB");
        }
      } /* End of if of Additional Meas Present */

    }
    else
    {
      /* Meas Type has already been verified */
      l1_inter_freq_meas_params.meas_object_info_included = TRUE;

      
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r8,adjacentFrequencyIndex))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFREQ:adjacentFrequencyIndex for FDD UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      /* Check Measurement Quantity present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r8,interFreqMeasQuantity))
      {

        if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.
          u.interFrequencyMeasurement->interFreqMeasQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
        {
          /* Set Meas Quantity to TRUE */
          l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      } /* End of meas quantity present */

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r8,interFreqReportingQuantity))
      {

        /* Set Reporting quantity */
        tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->interFreqReportingQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;
      }
      if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.t  == T_tdsrrc_InterFreqReportCriteria_r7_intraFreqReportingCriteria) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.u.intraFreqReportingCriteria,
         tdsrrc_IntraFreqReportingCriteria_r7,eventCriteriaList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:Modify:rpt crit set to Intraf rpt crit and evnt list non null");

        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      /* Process Reporting Criteria */

      if (tdsrrcifreq_process_reporting_criteria_r7(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria, &l1_inter_freq_meas_params.meas_object.u.inter_f) == FAILURE)
      {

        if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r7_interFreqReportingCriteria) &&
         !((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria,
         tdsrrc_InterFreqReportingCriteria_r6,interFreqEventList)) &&
         (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria->interFreqEventList.n != 0)))
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }
      }
      else
      {
        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

        mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);

      }

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFREQ:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          addtl_meas_incl = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
               addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
               &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);


            return FAILURE;

          }

        }
      } /* End of if of Additional Meas Present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:additional meas list absent");
      }

      /* Fill cell list, cell info */
      result = tdsrrcifreq_fill_inter_freq_info_r8(
        &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        interFreqCellInfoList, &l1_inter_freq_meas_params);
      if (result == TDSRRCMEAS_FAIL)
      {
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      else
      {
        if(result == TDSSFN_FAIL)
        {
                /* Send Meas Ctrl Failure Message */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:SFN-ind set as True");
        /* Send Meas Ctrl Failure Msg */

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE;
        }
        else
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;
        }
      }

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
        tdsrrc_InterFrequencyMeasurement_r8,measurementValidity))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Meas Validity present");
        /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
        then assume it to be CELL_DCH. */
        meas_validity = TDSSTATE_DCH;
      }

      /* Now commit new meas params to RRC db.*/
      if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:Unable to set MeasId in DB");
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        /* Make the old CELL_INFO_LIST valid */

        for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));
        }

        return FAILURE;
      }

      /* Now commit addtl meas db */
      if (addtl_meas_incl == TRUE)
      {
        tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Commit Addtl Meas to RRC DB");
      }

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    that this meas exists. So let us delete it from tdsinter_freq_meas_id_list
    Meas Identity, Transaction Id already set */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_freq_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_freq_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_FREQ_MEAS; count++)
    {
      if (tdsinter_freq_meas_id_list[count].meas_id == l1_inter_freq_meas_params.meas_id)
      {
        tdsinter_freq_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsinter_freq_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        tdsinter_freq_meas_id_list[count].mcm_owned = FALSE;
        tdsinter_freq_meas_id_list[count].meas_validity = TDSNOT_PRESENT;

        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
          tdsinter_freq_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;

        break;
      }
    }
  } /* End of else of release */

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Skip sending interFreq measurements from MCM to L1");
    return SUCCESS;
  }

  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_freq_meas_params.trans_id = alloc_idx;

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */

  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_freq_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);

  return SUCCESS;
}

/*===========================================================================
FUNCTION  tdsrrcifreq_fill_inter_freq_info_r8

DESCRIPTION
 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 Also updates the Inter Freq Cell Info List for rel 8

DEPENDENCIES
 None

RETURN VALUE
 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS
 None
===========================================================================*/
tdsrrcmeas_status_e_type tdsrrcifreq_fill_inter_freq_info_r8
(
 tdsrrc_InterFreqCellInfoList_r8* interFreqCellInfoList_ptr,
 tdsl1_meas_ctrl_parm_struct_type* l1_meas_ptr
)
{
  tdsinter_freq_removed_cell_struct_type cascaded_rem_cell[TDSL1_MAX_CELL_MEAS];

  uint32 inter_freq_cell_id = 0;

  uint16 dl_freq = 0;

  uint16 cpi = 0;

  uint32 cell_position = 0;

  uint16 cpi_to_measure = TDSINVALID_CPI;
  uint16 freq_to_measure = TDSINVALID_INTER_FREQ;
  uint8 meas_count = 0;
  uint32 cell_id = 0,idx=0;  
  boolean sec_freq_ind = FALSE;
  boolean sec_freq_ind_incl = FALSE;

  uint8 cell_count = 0, removed_cell_count = 0, cpi_removed_count = 0, cnt = 0;

  tdsrrc_RemovedInterFreqCellList_removeSomeInterFreqCells* local_rm_inter_freq_cell_ptr = NULL;

  struct tdsrrc_CellsForInterFreqMeasList *cellsForInterFreqMeasList_ptr = NULL;

  tdsrrc_NewInterFreqCellList_r8* local_network_new_cell_list_ptr = NULL;


  /* Declare a local var to store the inter freq object */

  tdsl1_inter_freq_meas_ctrl_struct_type* l1_inter_freq_meas_ptr = NULL;

  tdsl1_inter_freq_report_crit_struct_type* inter_rpt_crit_ptr = NULL;

  l1_inter_freq_meas_ptr = &l1_meas_ptr->meas_object.u.inter_f;

  inter_rpt_crit_ptr = &l1_inter_freq_meas_ptr->u.inter_rpt_crit;

  /* Copy Inter Freq CELL_INFO_LIST to a local var. If failure in the processing of
  CELL_INFO_LIST, copy  tdsinter_freq_cell_list_tmp back to tdsinter_freq_cell_list*/
  for (cell_count = 0; cell_count < TDSL1_MAX_CELL_MEAS; cell_count++)
  {
    memscpy(&tdsinter_freq_cell_list_tmp[cell_count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list[cell_count], sizeof(tdsinter_freq_cell_list_struct_type));

    cascaded_rem_cell[cell_count].freq = TDSINVALID_INTER_FREQ;
    cascaded_rem_cell[cell_count].cpi = TDSINVALID_CPI;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,tdsrrc_InterFreqCellInfoList_r8,
    removedInterFreqCellList))
  {
    switch (interFreqCellInfoList_ptr->removedInterFreqCellList.t)
    {
    case T_tdsrrc_RemovedInterFreqCellList_removeAllInterFreqCells:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Remove:Delete all cells selected");
      l1_inter_freq_meas_ptr->cell_list.num_cell_rmv = 0xff;


      /* Update Cell_info_list variable */
      for (cell_count = 0; cell_count < TDSL1_MAX_CELL_MEAS; cell_count++)
      {
        tdsinter_freq_cell_list[cell_count].cell_position = TDSVACANT;
        tdsinter_freq_cell_list[cell_count].cpi = TDSINVALID_CPI;
        tdsinter_freq_cell_list[cell_count].dl_freq= TDSINVALID_INTER_FREQ;
        tdsinter_freq_cell_list[cell_count].sec_freq_ind_incl = FALSE;
        tdsinter_freq_cell_list[cell_count].sec_freq_ind = FALSE;
      }

      break;

    case T_tdsrrc_RemovedInterFreqCellList_removeSomeInterFreqCells:

      if ((interFreqCellInfoList_ptr->removedInterFreqCellList.u.removeSomeInterFreqCells == NULL )||
        (interFreqCellInfoList_ptr->removedInterFreqCellList.u.removeSomeInterFreqCells->n == 0 ))
      {
        break;
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Delete some cells");
      /* Store head of Link list */
      local_rm_inter_freq_cell_ptr =  interFreqCellInfoList_ptr->
        removedInterFreqCellList.u.removeSomeInterFreqCells;
      do
      {
        inter_freq_cell_id = local_rm_inter_freq_cell_ptr->elem[idx];

        /* Find the cpi, dl freq as per given cell id from CELL_INFO_LIST */
        if ( tdsrrcifreq_find_cpi_freq(
          inter_freq_cell_id, &cpi, &dl_freq,&sec_freq_ind_incl,&sec_freq_ind) == TDSCPI_ABSENT)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Rem:Incorrect cell id %d",inter_freq_cell_id);
          idx++;
          continue;          
        }
        else
        {
          /* First remove cell from inter freq cell list */
          tdsrrcifreq_remove_cellid(inter_freq_cell_id);

          /* Here one needs to put cpi, dl freq so that L1 can remove it */
          l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].cpi = cpi;

          l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].freq = dl_freq;

          removed_cell_count++; /* Increment count */
        }

        /*  Get next element in the list */

        idx++;

      } /* Continue if while cond satisfied */

      while ( (local_rm_inter_freq_cell_ptr->n > idx) &&
        (removed_cell_count < TDSL1_MAX_CELL_MEAS) );


      /* Now store the number of actual cells sent by UTRAN */

      l1_inter_freq_meas_ptr->cell_list.num_cell_rmv =  removed_cell_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFreq:Removed cell cnt = %d cells",removed_cell_count);

      break;

    case T_tdsrrc_RemovedInterFreqCellList_removeNoInterFreqCells:
      /* Already set removed cells to 0 */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Delete no cells selected");
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Invalid cell t");
      break;
    } /* End of switch */

  } /* End of if of removed inter freq cells present */

  /* Reinitialize these variables */
  cell_count = 0;
  cpi = 0;

  /* Check if new cells present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,
    tdsrrc_InterFreqCellInfoList_r8,newInterFreqCellList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:New cells to be added");

    /* Do not check for cell order. If UTRAN messes up, its their problem */
    idx=0;
    /* Save the Head pointer of linked List */
    local_network_new_cell_list_ptr = &interFreqCellInfoList_ptr->newInterFreqCellList;

    while ( ( local_network_new_cell_list_ptr->n > idx) &&
      (cell_count < TDSL1_MAX_CELL_MEAS) )
    {
    /* Pointer to next cell is Non-NULL and cell count
      * hasn't exceeded the max. L1 can handle */

      if (local_network_new_cell_list_ptr->elem[idx].
        cellInfo.modeSpecificInfo.t ==
        T_tdsrrc_CellInfo_r4_modeSpecificInfo_fdd)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Ignore FDD cell");

        idx++;
        continue;

      }

      /* Fill in Cell Info */
      if (tdsrrcmeas_fill_mcm_cell_info_r5(
        &local_network_new_cell_list_ptr->elem[idx].cellInfo,
        &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
        cell_info) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Ignoring Cell");
        idx++;
        continue;
      }

      /* Check if cell id present */
      if (TDSRRC_MSG_LIST_BITMASK_IE(local_network_new_cell_list_ptr,
        tdsrrc_NewInterFreqCell_r8,interFreqCellID))
      {
        cell_position = local_network_new_cell_list_ptr->elem[idx].interFreqCellID;

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFreq:Add:Cell at pos %d to be added",cell_position);

      } /*  End of if of inter-freq cell id present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Add: Cell Id not present");
        cell_position = TDSAVAILABLE_CELL_POSITION;
      }

      /* Check if freq present */
      if (TDSRRC_MSG_LIST_BITMASK_IE(local_network_new_cell_list_ptr,
        tdsrrc_NewInterFreqCell_r8,frequencyInfo))
      {
        if (local_network_new_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
          T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Ignore Cell. TDD cell");
          idx++;
          continue;
        }

        if (tdsrrc_validate_frequency_info(&local_network_new_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:Cell with DL freq %d out of ",local_network_new_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
          return TDSRRCMEAS_UNSUPPORTED;
        }


        if (local_network_new_cell_list_ptr->elem[idx].
          frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8:IFreq:Invalid DL ARFCN %d",dl_freq);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Ignore Inter Freq Cell");
          idx++;
          continue;
        }

        /* Store DL freq */
        dl_freq = l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          freq_info.uarfcn_dl = (uint16) local_network_new_cell_list_ptr->elem[idx].
          frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

      } /*  End of if of inter-freq freq present */
      else
      {
        if (cell_count == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFreq:Ignore Cell. DL Freq absent");
          idx++;
          continue;
        }
        else
        {
          /* This implies that there is already one DL and UL freq present. Copy from there */
          memscpy(&l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].freq_info,
            sizeof(tdsl1_freq_info_struct_type),
            &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count-1].freq_info,
            sizeof(tdsl1_freq_info_struct_type));
        }
      }

      if (local_network_new_cell_list_ptr->elem[idx].modeSpecificInfo.t ==
         T_tdsrrc_NewInterFreqCell_r8_modeSpecificInfo_tdd128)
      {
        sec_freq_ind_incl = TRUE;
        sec_freq_ind = local_network_new_cell_list_ptr->elem[idx].modeSpecificInfo.u.tdd128->intraSecondaryFreqIndicator;
      }
        
      if ( tdsrrcifreq_update_inter_freq_cell_list_mcm(
          &cell_position,
          &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          cell_info.cpi, &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].freq_info.uarfcn_dl,
          &cpi, &dl_freq,&sec_freq_ind_incl, &sec_freq_ind) == TDSRRCMEAS_SUCCESS)
      {
        /* cpi sucessfully stored in the cell info list variable */
        /* Now store cpi in the mcm in add cell for layer1 to use cpi */
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].cpi =
          l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          cell_info.cpi;

        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].cpi_incl =
          l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          cell_info.cpi_incl;
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind_incl = sec_freq_ind_incl;  
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind = sec_freq_ind;      

        /* This flag is always true for MCM */
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].measure_in_idle_pch_fach = TRUE;

        if ((cpi_removed_count < TDSL1_MAX_CELL_MEAS) && (cpi != TDSINVALID_CPI) && (dl_freq != TDSINVALID_INTER_FREQ))
        {
          /* UTRAN wants us to delete this cpi/freq. Result of CASCADING effect */
          cascaded_rem_cell[cpi_removed_count].cpi = cpi;

          cascaded_rem_cell[cpi_removed_count++].freq = dl_freq;
        }

        /* Increment cell count as add cell successfully stored */
        cell_count ++;
      }  /* End of if of rrcmeas_update_inter_freq_cell_list */

      /* Read next element */
     idx++;

    } /* End of while newIntraFreqCellList != NULL */

    /* Initialize no of added cells */
    l1_inter_freq_meas_ptr->cell_list.num_cell_add = cell_count;

    //This code is to satify KW error only.
    for (cell_count = 0; cell_count < cpi_removed_count; cell_count++, removed_cell_count++)
    {
      // 2012/09/10 KW
	  if ((removed_cell_count >= TDSL1_MAX_CELL_MEAS) ||(cell_count >= TDSL1_MAX_CELL_MEAS))
      {
        /* Preventive check */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"KW: REL8:IFREQ:Unable to delete all cells");
        break;
      }
      /* Add this removed cell to the already built removed cell list for L1 */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IFREQ:Cascad:L1 removed cell list upd %d cpi",cascaded_rem_cell[cell_count].cpi);
      l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].cpi =
        cascaded_rem_cell[cell_count].cpi;

      l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].freq =
        cascaded_rem_cell[cell_count].freq;

    }
    /* Now update the number of removed cells */
    l1_inter_freq_meas_ptr->cell_list.num_cell_rmv += cpi_removed_count;

  } /* End of if for new inter freq cells present */
  
  /* Initialize no fo cells to 0 */
  l1_inter_freq_meas_ptr->cell_list.num_cell_meas = 0;
  cell_count = 0;
  idx=0;
  l1_inter_freq_meas_ptr->cell_list.cells_for_meas_incl = FALSE;
  /* Check if cells for meas present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,
    & tdsrrc_InterFreqCellInfoList_r8,cellsForInterFreqMeasList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Cells for meas present");
    l1_inter_freq_meas_ptr->cell_list.cells_for_meas_incl = TRUE;
   
    cellsForInterFreqMeasList_ptr = &interFreqCellInfoList_ptr->cellsForInterFreqMeasList;
    meas_count = 0;
    while (cellsForInterFreqMeasList_ptr->n > idx)
    {

      cell_id = cellsForInterFreqMeasList_ptr->elem[idx];
      /* Check if the cell id is present in tdsrrcmeas_intra_freq_cell_list VARIABLE */
      if ( tdsrrcifreq_find_cpi_freq(cell_id, &cpi_to_measure, &freq_to_measure,&sec_freq_ind_incl, &sec_freq_ind) == TDSCPI_ABSENT)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8:Invalid cell to measure with id = %d",cell_id);

		if (l1_inter_freq_meas_ptr->rpt_mode == TDSL1_INTER_FREQ_EVT_TRIG)
        {
		  while ((cnt < inter_rpt_crit_ptr->num_event) && (cnt < TDSL1_MAX_MEAS_EVENT))
		  {
            if ( (inter_rpt_crit_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2D) ||
			     (inter_rpt_crit_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2F) )
      {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Pass serving cell prim freq and cpi to L1 for 2D/2F MCM");
			  cpi_to_measure = (uint16)tdsrrcccm_get_curr_camped_cpid();
			  freq_to_measure = (uint16)tdsrrcccm_get_curr_camped_freq();
              sec_freq_ind_incl = FALSE;
              sec_freq_ind = FALSE;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].cpi = cpi_to_measure;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].dl_freq = freq_to_measure;      
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind_incl = sec_freq_ind_incl;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind = sec_freq_ind;
              meas_count++;
              break;
            }
			cnt++;
		  }
        }
      } /* End of if for cpi not found */
      else
      {
        l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].cpi = cpi_to_measure;
        l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].dl_freq = freq_to_measure;      
        l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind_incl = sec_freq_ind_incl;
        l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind = sec_freq_ind;
        meas_count++;
      }
      idx++;
    } /* End of while */

    l1_inter_freq_meas_ptr->cell_list.num_cell_meas =  meas_count;

  } /* End of if of rrc_cellsForInterFreqMeasList) */

  return TDSRRCMEAS_SUCCESS;
}

#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================
FUNCTION   RRCIFREQ_PROCESS_MCM_R9

DESCRIPTION
    This function processes the release 9 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES
    None.

RETURN VALUE
    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS
    None
==========================================================================*/
uecomdef_status_e_type tdsrrcifreq_process_mcm_r9
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_freq_meas_params;
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  tdsrrcmeas_status_e_type result;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_FREQ;

  /* Meas Validity */
  tdsmeas_validity_enum_type meas_validity = TDSNOT_PRESENT;

  tdsrrc_MeasurementControl_r9_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 count = 0;

  uint8 cnt =  0;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Assign default values to l1_inter_freq_meas_params */
  memset(&l1_inter_freq_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_freq_meas_params.meas_object.meas_type = TDSL1_INTER_FREQ;

    /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.criticalExtensions.u.r9.measurementControl_r9;

  /* Set Transaction Id and Meas Id */
  l1_inter_freq_meas_params.trans_id = tdstransaction_id;
  l1_inter_freq_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;


  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r9_setup)
  {
    /* Double check that it is INTER-FREQ Meas */
    if (rrc_mcm_ptr->measurementCommand.u.setup->t !=
        T_tdsrrc_MeasurementType_r9_interFrequencyMeasurement)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFREQ:UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r9,adjacentFrequencyIndex))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFREQ:adjacentFrequencyIndex for FDD UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r9,interBandFrequencyIndex))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFREQ:interBandFrequencyIndex for FDD UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    /* Check Measurement Quantity present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r9,interFreqMeasQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:SETUP:MeasQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    l1_inter_freq_meas_params.meas_object_info_included = TRUE;

    if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.setup->
      u.interFrequencyMeasurement->interFreqMeasQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
    {
      /* Set Meas Quantity to TRUE */
      l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
    }
    else
    {
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }

    /* Check Reporting Quantity */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r9,interFreqReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IFreq:SETUP:RptQuan absent");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;

    }
    if(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r9_intraFreqReportingCriteria)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IFreq:SETUP:inter freq rpt criteria set to Intra freq rpt criteria");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    /* Set Reporting quantity */
    tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->interFreqReportingQuantity,
      &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


    l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;

    /* Store reporting mode to TRUE */
    l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

    l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFREQ:Rpting Mode %d ACK:0",l1_inter_freq_meas_params.rpt_trans_mode);

    /* Process Reporting Criteria which is mandatory in case meas obj is present */

    if (tdsrrcifreq_process_reporting_criteria_r9(&rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
      reportCriteria, &l1_inter_freq_meas_params.meas_object.u.inter_f) == FAILURE)
    {

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */

      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    else
    {
      l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

      mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);
    }



    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFreq:Addtl Meas spec for Meas %d",rrc_mcm_ptr->measurementIdentity);
     /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */

    if ((rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
           interFreqCellInfoList.m.cSGInterFreqCellInfoListPresent) ||
        (rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement->
           interFreqCellInfoList.m.interFreqSIAcquisitionPresent) )
    {
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    result = tdsrrcifreq_fill_inter_freq_info_r9(&rrc_mcm_ptr->measurementCommand.u.setup->u.
      interFrequencyMeasurement->interFreqCellInfoList,
      &l1_inter_freq_meas_params) ;
    /* Fill cell list, cell info */
    if (result == TDSRRCMEAS_FAIL)
    {
      /* Send Meas Ctrl Failure Message */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IFreq:Unable to fill Inter Freq");
      /* Send Meas Ctrl Failure Msg */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

      /* Set failure cause  which is IE value incorrect */
      rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
    else
    {
      if(result == TDSSFN_FAIL)
      {
              /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IFreq:SFN-ind set as True");
      /* Send Meas Ctrl Failure Msg */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
      if (result == TDSRRCMEAS_UNSUPPORTED)
      {
        /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IFreq:Unable to fill Inter Freq");
        /* Send Meas Ctrl Failure Msg */
  
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
        return FAILURE;
      }

      l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;

    }
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending Meas Ctrl Failure");

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interFrequencyMeasurement,
      tdsrrc_InterFrequencyMeasurement_r9,measurementValidity))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Meas Validity present");
      /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
      then assume it to be CELL_DCH. If network specifies validity to be other than CELL_DCH,
      the UE behaviour is unspecified. So we assume it to be CELL_DCH */
      meas_validity = TDSSTATE_DCH;
    }

    /* Now commit new meas params to RRC db.*/
    if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
    {

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IFreq:Unable to set MeasId in DB");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      /* Make the old CELL_INFO_LIST valid */

      for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));

      }

      return FAILURE;

    }

    /* Now commit addtl meas db */
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Commit Addtl Meas to RRC DB");
    }

    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    /* Now send command to L1 */
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r8_modify)
  {
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_inter_freq_meas_params.rpt_mode_incl = TRUE;

      l1_inter_freq_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    /* Set type to MODIFY */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r9_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Modify:InterFreq:MType absent");

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFreq:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:IFREQ:Commit Addtl Meas to RRC DB");
        }
      } /* End of if of Additional Meas Present */

    }
    else
    {
      /* Meas Type has already been verified */
      l1_inter_freq_meas_params.meas_object_info_included = TRUE;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r9,adjacentFrequencyIndex))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFREQ:adjacentFrequencyIndex for FDD UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r9,interBandFrequencyIndex))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFREQ:interBandFrequencyIndex for FDD UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      /* Check Measurement Quantity present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r9,interFreqMeasQuantity))
      {

        if (tdsrrcifreq_process_meas_quantity(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.
          u.interFrequencyMeasurement->interFreqMeasQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan) == TDSRRCMEAS_SUCCESS)
        {
          /* Set Meas Quantity to TRUE */
          l1_inter_freq_meas_params.meas_object.u.inter_f.meas_quan_incl = TRUE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      } /* End of meas quantity present */

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
          tdsrrc_InterFrequencyMeasurement_r9,interFreqReportingQuantity))
      {

        /* Set Reporting quantity */
        tdsrrcifreq_fill_rpt_quan(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->interFreqReportingQuantity,
          &l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan);


        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_quan_incl = TRUE;
      }
      if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.t  == T_tdsrrc_InterFreqReportCriteria_r9_intraFreqReportingCriteria) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria.u.intraFreqReportingCriteria,
         tdsrrc_IntraFreqReportingCriteria_r9,eventCriteriaList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IFreq:Modify:rpt crit set to Intraf rpt crit and evnt list non null");

        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      /* Process Reporting Criteria */

      if (tdsrrcifreq_process_reporting_criteria_r9(&rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        reportCriteria, &l1_inter_freq_meas_params.meas_object.u.inter_f) == FAILURE)
      {

        if((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.t == T_tdsrrc_InterFreqReportCriteria_r9_interFreqReportingCriteria) &&
         !((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria,
         tdsrrc_InterFreqReportingCriteria_r6,interFreqEventList)) &&
         (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
         reportCriteria.u.interFreqReportingCriteria->interFreqEventList.n != 0)))
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;
          l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
        }
        else
        {
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }
      }
      else
      {
        l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode_incl = TRUE;

        mcm_rpt_crit = TDSRET_INTER_FREQ_RPT_CRIT(l1_inter_freq_meas_params.meas_object.u.inter_f.rpt_mode);

      }

      /* Now check Additional Measurements */
      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFREQ:Addtl Meas spec. for Meas %d",rrc_mcm_ptr->measurementIdentity);

        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          addtl_meas_incl = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
               addtl_meas_incl, FALSE, TDSINTER_FREQ, rrc_mcm_ptr->measurementIdentity,
               &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);


            return FAILURE;

          }

        }
      } /* End of if of Additional Meas Present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:additional meas list absent");
      }
      
      if ((rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
               interFreqCellInfoList.m.cSGInterFreqCellInfoListPresent) ||
          (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
             interFreqCellInfoList.m.interFreqSIAcquisitionPresent) )
      {
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
        return FAILURE;
      }

      /* Fill cell list, cell info */
      result = tdsrrcifreq_fill_inter_freq_info_r9(
        &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement->
        interFreqCellInfoList, &l1_inter_freq_meas_params);
      if (result == TDSRRCMEAS_FAIL)
      {
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      else
      {
        if(result == TDSSFN_FAIL)
        {
                /* Send Meas Ctrl Failure Message */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IFreq:SFN-ind set as True");
        /* Send Meas Ctrl Failure Msg */

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

        /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE;
        }
        else
        {
          l1_inter_freq_meas_params.meas_object.u.inter_f.cell_list_incl = TRUE;
        }
      }

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interFrequencyMeasurement,
        tdsrrc_InterFrequencyMeasurement_r9,measurementValidity))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Meas Validity present");
        /* No need to check value as by default validity is CELL_DCH and if it is not CELL_DCH,
        then assume it to be CELL_DCH. */
        meas_validity = TDSSTATE_DCH;
      }

      /* Now commit new meas params to RRC db.*/
      if (tdsrrcifreq_config_inter_freq_db(&l1_inter_freq_meas_params, &mcm_rpt_crit, meas_validity) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IFreq:Unable to set MeasId in DB");
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        /* Make the old CELL_INFO_LIST valid */

        for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_freq_cell_list[count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list_tmp[count], sizeof(tdsinter_freq_cell_list_struct_type));
        }

        return FAILURE;
      }

      /* Now commit addtl meas db */
      if (addtl_meas_incl == TRUE)
      {
        tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Commit Addtl Meas to RRC DB");
      }

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    that this meas exists. So let us delete it from tdsinter_freq_meas_id_list
    Meas Identity, Transaction Id already set */
    l1_inter_freq_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_freq_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_freq_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_FREQ_MEAS; count++)
    {
      if (tdsinter_freq_meas_id_list[count].meas_id == l1_inter_freq_meas_params.meas_id)
      {
        tdsinter_freq_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsinter_freq_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        tdsinter_freq_meas_id_list[count].mcm_owned = FALSE;
        tdsinter_freq_meas_id_list[count].meas_validity = TDSNOT_PRESENT;

        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
          tdsinter_freq_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;

        break;
      }
    }
  } /* End of else of release */

#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
  if(TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO)
  {
    if(TRUE == rrc_mcm_ptr->m.cellDCHMeasOccasionInfo_TDD128Present)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: cellDCHMeasOccasionInfo_TDD128 IE present and supported!");

      switch(tdsrrcllc_get_dmo_r9(&rrc_mcm_ptr->cellDCHMeasOccasionInfo_TDD128,
                            &l1_inter_freq_meas_params.meas_occasion_info))
      {
        case TDSDMO_mlength_k_chk_Unsupported_Config:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: tdsrrcllc_get_dmo_r9 return unsupport failure");

          /* Send Meas Ctrl Failure Msg */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

        /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE;

        case TDSDMO_mlength_k_chk_Invalid_Config:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: tdsrrcllc_get_dmo_r9 return invalid failure");

          /* Send Meas Ctrl Failure Msg */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE;

        case TDSDMO_mlength_k_chk_SUCCESS:
          break;
        default:
          break;
      }
      l1_inter_freq_meas_params.meas_occasion_info_incl = TRUE;
      /* save in CC, in order to revert old config, send to L1 old config */
      tdscurrent_config_ptr->meas_occasion_info_incl = TRUE;

      memscpy(&tdscurrent_config_ptr->meas_occasion_info,
	  	     sizeof(tdsl1_meas_occasion_info_struct_type),
             &l1_inter_freq_meas_params.meas_occasion_info,
             sizeof(tdsl1_meas_occasion_info_struct_type));
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: cellDCHMeasOccasionInfo_TDD128 IE is not present!");
      l1_inter_freq_meas_params.meas_occasion_info_incl = FALSE;
      tdscurrent_config_ptr->meas_occasion_info_incl = FALSE;
    }
  }
  else
  {
#endif
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rrc_mcm_ptr,tdsrrc_MeasurementControl_r9_IEs,cellDCHMeasOccasionInfo_TDD128))
    {
#ifdef FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS
      /* Return failure when conditional ignore DMO NV is set and some statusFlag are active.
         Ignore DMO config in other cases. */
      if((TDSCHECK_OPTIONAL_FEATURE_CONDITIONAL_IGNORE_DMO) &&
       (tdsrrcmeas_check_all_statusFlag_deact_in_DMO_config(&rrc_mcm_ptr->cellDCHMeasOccasionInfo_TDD128) == FALSE))
    {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: FEATURE_CONDITIONAL_IGNORE_DMO enabled and some statusFlag in DMO config are activate");
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: cellDCHMeasOccasionInfo_TDD128 IE not supported yet!");
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

    /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

    /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    else
    {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: CONDITIONAL_IGNORE_DMO disabled or all statusFlag deactivate,ignore R9 DMO config");
      }
#else
      /* FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS not defined, return failure directly as before */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: cellDCHMeasOccasionInfo_TDD128 IE not supported yet!");
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
#endif
    }
#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
  }
#endif
  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Skip sending interFreq measurements from MCM to L1");
    return SUCCESS;
  }

  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_freq_meas_params.trans_id = alloc_idx;

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */

  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_freq_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);

  return SUCCESS;
}
/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_REPORTING_CRITERIA_R9

DESCRIPTION

    This function process the reporting criteria specified
  in Measurement Control Message

DEPENDENCIES

    None

RETURN VALUE

    Success or Failure depending upon processing of reporting criteria

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_reporting_criteria_r9
(
 tdsrrc_InterFreqReportCriteria_r9* ifreq_rpt_criteria_ptr,
 tdsl1_inter_freq_meas_ctrl_struct_type* l1_rpt_params_ptr
)
{
  switch(ifreq_rpt_criteria_ptr->t)
  {
  case T_tdsrrc_InterFreqReportCriteria_r9_interFreqReportingCriteria:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Inter Freq Event Rpt Crit");

    l1_rpt_params_ptr->u.inter_rpt_crit.num_event = 0;

    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria,
      & tdsrrc_InterFreqReportingCriteria_r6,interFreqEventList)) &&
      (ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria->interFreqEventList.n != 0))
    {
      if (tdsrrcifreq_process_event_criteria_r6(ifreq_rpt_criteria_ptr->u.interFreqReportingCriteria,
        &l1_rpt_params_ptr->u.inter_rpt_crit) == FAILURE)
      {
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"InterFreq Event List Empty");
      return FAILURE;
    }

    /* Inter-Freq Events successfully processed */
    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_EVT_TRIG;

    break;

  case T_tdsrrc_InterFreqReportCriteria_r9_intraFreqReportingCriteria:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Intra Freq Event Rpt Crit");

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting rpt crit to TDSL1_INTER_FREQ_NO_CHANGE");
    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_NO_CHANGE;
    break;

  case T_tdsrrc_InterFreqReportCriteria_r9_periodicalReportingCriteria:

    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_PERIODIC;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Perodic Rpt Crit");

    l1_rpt_params_ptr->rpt_cell_status_incl = FALSE;

    /* Set reporting cells */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria,
      tdsrrc_PeriodicalWithReportingCellStatus,reportingCellStatus))
    {
      if (tdsrrcmeas_fill_no_of_reporting_cells(&ifreq_rpt_criteria_ptr->u.
        periodicalReportingCriteria->reportingCellStatus,
        &l1_rpt_params_ptr->rpt_cell_status) == SUCCESS)
      {
        l1_rpt_params_ptr->rpt_cell_status_incl = TRUE;

        /* Check if reporting cell status t is valid */
        if ((l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid cell status %d",l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat);
          return FAILURE;
        }
      }
      else
      {
        return FAILURE;
      }
    }

    /* Check if reporting amount present */
 
      /* Store reporting amount */
      l1_rpt_params_ptr->u.periodic_crit.rpt_amount = (tdsl1_amount_of_reporting_enum_type)
        TDSRRCMEAS_RET_REPORTING_AMOUNT(
        ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->periodicalReportingCriteria.reportingAmount);
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Rpting Amt is %d,Infin:0",l1_rpt_params_ptr->u.periodic_crit.rpt_amount);

    /* The spec indicates that 0 means UE behaviour unspecified */
    if (ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->
      periodicalReportingCriteria.reportingInterval == tdsrrc_ReportingIntervalLong_ril0)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Periodic Rpting 0 specified. use 250");
      l1_rpt_params_ptr->u.periodic_crit.rpt_interval = TDSL1_RPT_INTERVAL_250MS;
    }
    else
    {
      /* Store reporting interval */
      l1_rpt_params_ptr->u.periodic_crit.rpt_interval = (tdsl1_reporting_interval_enum_type)
        TDSRRCMEAS_RET_PERIODIC_REPORTING_INTERVAL
        (ifreq_rpt_criteria_ptr->u.periodicalReportingCriteria->periodicalReportingCriteria.reportingInterval);
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IFreq:Rpting Intvl is %d msec",l1_rpt_params_ptr->u.periodic_crit.rpt_interval);
    break; /* Come out of switch */

  case T_tdsrrc_InterFreqReportCriteria_r9_noReporting:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFreq:Addtl Meas selected");

    l1_rpt_params_ptr->rpt_mode = TDSL1_INTER_FREQ_NO_REPORTING;

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ifreq_rpt_criteria_ptr->u.noReporting,
        tdsrrc_ReportingCellStatusOpt,reportingCellStatus))
    {
      if (tdsrrcmeas_fill_no_of_reporting_cells(&ifreq_rpt_criteria_ptr->u.noReporting->reportingCellStatus,
        &l1_rpt_params_ptr->rpt_cell_status) == SUCCESS)
      {
        l1_rpt_params_ptr->rpt_cell_status_incl = TRUE;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"IFREQ:Rpting Crit is Addtl");

        /* Check if reporting cell status t is valid */
        if ((l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ALL_ASET_MSET_DSET_USED_FREQ) ||
          (l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat == TDSL1_RPT_ASET))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Invalid cell status %d",l1_rpt_params_ptr->rpt_cell_status.rpt_cell_stat);
          return FAILURE;
        }
      }
    }
    break;



  default:
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"InterFreq Rpt Crit %d not supported",ifreq_rpt_criteria_ptr->t);
    return FAILURE;

  }
  return SUCCESS;

}
/*===========================================================================
FUNCTION  tdsrrcifreq_fill_inter_freq_info_r9

DESCRIPTION
 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 Also updates the Inter Freq Cell Info List for rel 8

DEPENDENCIES
 None

RETURN VALUE
 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS
 None
===========================================================================*/
tdsrrcmeas_status_e_type tdsrrcifreq_fill_inter_freq_info_r9
(
 tdsrrc_InterFreqCellInfoList_r9* interFreqCellInfoList_ptr,
 tdsl1_meas_ctrl_parm_struct_type* l1_meas_ptr
)
{
  tdsinter_freq_removed_cell_struct_type cascaded_rem_cell[TDSL1_MAX_CELL_MEAS];

  uint32 inter_freq_cell_id = 0;

  uint16 dl_freq = 0;

  uint16 cpi = 0;

  uint32 cell_position = 0;

  uint16 cpi_to_measure = TDSINVALID_CPI;
  uint16 freq_to_measure = TDSINVALID_INTER_FREQ;
  uint8 meas_count = 0;
  uint32 cell_id = 0,idx=0;  
  boolean sec_freq_ind = FALSE;
  
  boolean sec_freq_ind_incl = FALSE;
  
  uint8 cell_count = 0, removed_cell_count = 0, cpi_removed_count = 0, cnt = 0;

  tdsrrc_RemovedInterFreqCellList_removeSomeInterFreqCells* local_rm_inter_freq_cell_ptr = NULL;

  struct tdsrrc_CellsForInterFreqMeasList *cellsForInterFreqMeasList_ptr = NULL;

  tdsrrc_NewInterFreqCellList_r9* local_network_new_cell_list_ptr = NULL;


  /* Declare a local var to store the inter freq object */

  tdsl1_inter_freq_meas_ctrl_struct_type* l1_inter_freq_meas_ptr = NULL;

  tdsl1_inter_freq_report_crit_struct_type* inter_rpt_crit_ptr = NULL;

  l1_inter_freq_meas_ptr = &l1_meas_ptr->meas_object.u.inter_f;

  inter_rpt_crit_ptr = &l1_inter_freq_meas_ptr->u.inter_rpt_crit;

  /* Copy Inter Freq CELL_INFO_LIST to a local var. If failure in the processing of
  CELL_INFO_LIST, copy  tdsinter_freq_cell_list_tmp back to tdsinter_freq_cell_list*/
  for (cell_count = 0; cell_count < TDSL1_MAX_CELL_MEAS; cell_count++)
  {
    memscpy(&tdsinter_freq_cell_list_tmp[cell_count], sizeof(tdsinter_freq_cell_list_struct_type), &tdsinter_freq_cell_list[cell_count], sizeof(tdsinter_freq_cell_list_struct_type));

    cascaded_rem_cell[cell_count].freq = TDSINVALID_INTER_FREQ;
    cascaded_rem_cell[cell_count].cpi = TDSINVALID_CPI;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,tdsrrc_InterFreqCellInfoList_r9,
    removedInterFreqCellList))
  {
    switch (interFreqCellInfoList_ptr->removedInterFreqCellList.t)
    {
    case T_tdsrrc_RemovedInterFreqCellList_removeAllInterFreqCells:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Remove:Delete all cells selected");
      l1_inter_freq_meas_ptr->cell_list.num_cell_rmv = 0xff;


      /* Update Cell_info_list variable */
      for (cell_count = 0; cell_count < TDSL1_MAX_CELL_MEAS; cell_count++)
      {
        tdsinter_freq_cell_list[cell_count].cell_position = TDSVACANT;
        tdsinter_freq_cell_list[cell_count].cpi = TDSINVALID_CPI;
        tdsinter_freq_cell_list[cell_count].dl_freq= TDSINVALID_INTER_FREQ;
        tdsinter_freq_cell_list[cell_count].sec_freq_ind_incl = FALSE;
        tdsinter_freq_cell_list[cell_count].sec_freq_ind = FALSE;
      }

      break;

    case T_tdsrrc_RemovedInterFreqCellList_removeSomeInterFreqCells:

      if ((interFreqCellInfoList_ptr->removedInterFreqCellList.u.removeSomeInterFreqCells == NULL )||
        (interFreqCellInfoList_ptr->removedInterFreqCellList.u.removeSomeInterFreqCells->n == 0 ))
      {
        break;
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Delete some cells");
      /* Store head of Link list */
      local_rm_inter_freq_cell_ptr =  interFreqCellInfoList_ptr->
        removedInterFreqCellList.u.removeSomeInterFreqCells;
      do
      {
        inter_freq_cell_id = local_rm_inter_freq_cell_ptr->elem[idx];

        /* Find the cpi, dl freq as per given cell id from CELL_INFO_LIST */
        if ( tdsrrcifreq_find_cpi_freq(
          inter_freq_cell_id, &cpi, &dl_freq, &sec_freq_ind_incl, &sec_freq_ind) == TDSCPI_ABSENT)
        {
          
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"IFreq:Rem:Incorrect cell id %d",inter_freq_cell_id);
          idx++;
          continue;          
        }
        else
        {
          /* First remove cell from inter freq cell list */
          tdsrrcifreq_remove_cellid(inter_freq_cell_id);

          /* Here one needs to put cpi, dl freq so that L1 can remove it */
          l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].cpi = cpi;

          l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].freq = dl_freq;

          removed_cell_count++; /* Increment count */
        }

        /*  Get next element in the list */

        idx++;

      } /* Continue if while cond satisfied */

      while ( (local_rm_inter_freq_cell_ptr->n > idx) &&
        (removed_cell_count < TDSL1_MAX_CELL_MEAS) );


      /* Now store the number of actual cells sent by UTRAN */

      l1_inter_freq_meas_ptr->cell_list.num_cell_rmv =  removed_cell_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFreq:Removed cell cnt = %d cells",removed_cell_count);

      break;

    case T_tdsrrc_RemovedInterFreqCellList_removeNoInterFreqCells:
      /* Already set removed cells to 0 */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Delete no cells selected");
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Invalid cell t");
      break;
    } /* End of switch */

  } /* End of if of removed inter freq cells present */

  /* Reinitialize these variables */
  cell_count = 0;
  cpi = 0;

  /* Check if new cells present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,
    tdsrrc_InterFreqCellInfoList_r9,newInterFreqCellList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:New cells to be added");

    /* Do not check for cell order. If UTRAN messes up, its their problem */
    idx=0;
    /* Save the Head pointer of linked List */
    local_network_new_cell_list_ptr = &interFreqCellInfoList_ptr->newInterFreqCellList;

    while ( ( local_network_new_cell_list_ptr->n > idx) &&
      (cell_count < TDSL1_MAX_CELL_MEAS) )
    {
    /* Pointer to next cell is Non-NULL and cell count
      * hasn't exceeded the max. L1 can handle */

      if (local_network_new_cell_list_ptr->elem[idx].
        cellInfo.modeSpecificInfo.t ==
        T_tdsrrc_CellInfo_r9_modeSpecificInfo_fdd)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Ignore FDD cell");

        idx++;
        continue;

      }

      /* Fill in Cell Info */
      if (tdsrrcmeas_fill_mcm_cell_info_r9(
        &local_network_new_cell_list_ptr->elem[idx].cellInfo,
        &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
        cell_info) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Ignoring Cell");
        idx++;
        continue;
      }

      /* Check if cell id present */
      if (TDSRRC_MSG_LIST_BITMASK_IE(local_network_new_cell_list_ptr,
        tdsrrc_NewInterFreqCell_r9,interFreqCellID))
      {
        cell_position = local_network_new_cell_list_ptr->elem[idx].interFreqCellID;

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFreq:Add:Cell at pos %d to be added",cell_position);

      } /*  End of if of inter-freq cell id present */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFREQ:Add: Cell Id not present");
        cell_position = TDSAVAILABLE_CELL_POSITION;
      }

      /* Check if freq present */
      if (TDSRRC_MSG_LIST_BITMASK_IE(local_network_new_cell_list_ptr,
        tdsrrc_NewInterFreqCell_r9,frequencyInfo))
      {
        if (local_network_new_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.t !=
          T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Ignore Cell. TDD cell");
          idx++;
          continue;
        }

        if (tdsrrc_validate_frequency_info(&local_network_new_cell_list_ptr->elem[idx].frequencyInfo) == FAILURE)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:Cell with DL freq %d out of ",local_network_new_cell_list_ptr->elem[idx].frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt);
          return TDSRRCMEAS_UNSUPPORTED;
        }


        if (local_network_new_cell_list_ptr->elem[idx].
          frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt > TDS_ARFCN)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL9:IFreq:Invalid DL ARFCN %d",dl_freq);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Ignore Inter Freq Cell");
          idx++;
          continue;
        }

        /* Store DL freq */
        dl_freq = l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          freq_info.uarfcn_dl = (uint16) local_network_new_cell_list_ptr->elem[idx].
          frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;

      } /*  End of if of inter-freq freq present */
      else
      {
        if (cell_count == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:IFreq:Ignore Cell. DL Freq absent");
          idx++;
          continue;
        }
        else
        {
          /* This implies that there is already one DL and UL freq present. Copy from there */
          memscpy(&l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].freq_info,
            sizeof(tdsl1_freq_info_struct_type),
            &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count-1].freq_info,
            sizeof(tdsl1_freq_info_struct_type));
        }
      }

      if (local_network_new_cell_list_ptr->elem[idx].modeSpecificInfo.t ==
         T_tdsrrc_NewInterFreqCell_r9_modeSpecificInfo_tdd128)
      {
        sec_freq_ind_incl = TRUE;
        sec_freq_ind = local_network_new_cell_list_ptr->elem[idx].modeSpecificInfo.u.tdd128->intraSecondaryFreqIndicator;
      }
        
      if ( tdsrrcifreq_update_inter_freq_cell_list_mcm(
          &cell_position,
          &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          cell_info.cpi, &l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].freq_info.uarfcn_dl,
          &cpi, &dl_freq, &sec_freq_ind_incl,&sec_freq_ind) == TDSRRCMEAS_SUCCESS)
      {
        /* cpi sucessfully stored in the cell info list variable */
        /* Now store cpi in the mcm in add cell for layer1 to use cpi */
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].cpi =
          l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          cell_info.cpi;

        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].cpi_incl =
          l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].
          cell_info.cpi_incl;
        
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind_incl = sec_freq_ind_incl; 
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].sec_freq_ind = sec_freq_ind;      

        /* This flag is always true for MCM */
        l1_inter_freq_meas_ptr->cell_list.add_cell[cell_count].measure_in_idle_pch_fach = TRUE;

        if ((cpi_removed_count < TDSL1_MAX_CELL_MEAS) && (cpi != TDSINVALID_CPI) && (dl_freq != TDSINVALID_INTER_FREQ))
        {
          /* UTRAN wants us to delete this cpi/freq. Result of CASCADING effect */
          cascaded_rem_cell[cpi_removed_count].cpi = cpi;

          cascaded_rem_cell[cpi_removed_count++].freq = dl_freq;
        }

        /* Increment cell count as add cell successfully stored */
        cell_count ++;
      }  /* End of if of rrcmeas_update_inter_freq_cell_list */

      /* Read next element */
     idx++;

    } /* End of while newIntraFreqCellList != NULL */

    /* Initialize no of added cells */
    l1_inter_freq_meas_ptr->cell_list.num_cell_add = cell_count;

    //This code is to satify KW error only.
    for (cell_count = 0; cell_count < cpi_removed_count; cell_count++, removed_cell_count++)
    {
      // 2012/09/10 KW
      if ((removed_cell_count >= TDSL1_MAX_CELL_MEAS) ||(cell_count >= TDSL1_MAX_CELL_MEAS))
      {
        /* Preventive check */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"KW: REL9 IFREQ:Unable to delete all cells");
        break;
      }
      /* Add this removed cell to the already built removed cell list for L1 */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IFREQ:Cascad:L1 removed cell list upd %d cpi",cascaded_rem_cell[cell_count].cpi);
      l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].cpi =
        cascaded_rem_cell[cell_count].cpi;

      l1_inter_freq_meas_ptr->cell_list.rmv_cell[removed_cell_count].freq =
        cascaded_rem_cell[cell_count].freq;

    }
    /* Now update the number of removed cells */
    l1_inter_freq_meas_ptr->cell_list.num_cell_rmv += cpi_removed_count;

  } /* End of if for new inter freq cells present */
  
  /* Initialize no fo cells to 0 */
  l1_inter_freq_meas_ptr->cell_list.num_cell_meas = 0;
  cell_count = 0;
  idx=0;
  l1_inter_freq_meas_ptr->cell_list.cells_for_meas_incl = FALSE;
  /* Check if cells for meas present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(interFreqCellInfoList_ptr,
    & tdsrrc_InterFreqCellInfoList_r9,cellsForInterFreqMeasList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Cells for meas present");
    l1_inter_freq_meas_ptr->cell_list.cells_for_meas_incl = TRUE;
   
    cellsForInterFreqMeasList_ptr = &interFreqCellInfoList_ptr->cellsForInterFreqMeasList;
    meas_count = 0;
    while (cellsForInterFreqMeasList_ptr->n > idx)
    {

      cell_id = cellsForInterFreqMeasList_ptr->elem[idx];
      /* Check if the cell id is present in tdsrrcmeas_intra_freq_cell_list VARIABLE */
      if ( tdsrrcifreq_find_cpi_freq(cell_id, &cpi_to_measure, &freq_to_measure, &sec_freq_ind_incl, &sec_freq_ind) == TDSCPI_ABSENT)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL9:Invalid cell to measure with id = %d",cell_id);

		if (l1_inter_freq_meas_ptr->rpt_mode == TDSL1_INTER_FREQ_EVT_TRIG)
      {
		  while ((cnt < inter_rpt_crit_ptr->num_event) && (cnt < TDSL1_MAX_MEAS_EVENT))
		  {
            if ( (inter_rpt_crit_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2D) ||
			     (inter_rpt_crit_ptr->evt[cnt].evt_id == TDSL1_INTER_FREQ_EVENT_2F) )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Pass serving cell prim freq and cpi to L1 for 2D/2F MCM");
			  cpi_to_measure = (uint16)tdsrrcccm_get_curr_camped_cpid();
			  freq_to_measure = (uint16)tdsrrcccm_get_curr_camped_freq();
			  sec_freq_ind_incl = FALSE;
			  sec_freq_ind = FALSE;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].cpi = cpi_to_measure;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].dl_freq = freq_to_measure;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind_incl = sec_freq_ind_incl;
              l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind = sec_freq_ind;
              meas_count++;
			  break;
            }
			cnt++;
		  }
        }

      } /* End of if for cpi not found */
      else
      {
        l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].cpi = cpi_to_measure;
        l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].dl_freq = freq_to_measure;
        l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind_incl = sec_freq_ind_incl;
        l1_inter_freq_meas_ptr->cell_list.meas_cells_list[meas_count].sec_freq_ind = sec_freq_ind;
        meas_count++;
      }
      idx++;
    } /* End of while */

    l1_inter_freq_meas_ptr->cell_list.num_cell_meas =  meas_count;

  } /* End of if of rrc_cellsForInterFreqMeasList) */

  return TDSRRCMEAS_SUCCESS;
}

#endif /*FEATURE_TDSCDMA_REL9*/
