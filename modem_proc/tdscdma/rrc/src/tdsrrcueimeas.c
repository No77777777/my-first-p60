/*===========================================================================
            R R C   M E A S U R E M E N T S

DESCRIPTION

   This module supports Measurement Control and Measurement Reporting
   for the RRC layer for UE Internal Measurements



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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcueimeas.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/17/12   zy      fix CR430249, and add the check of m_length and k in DMO
09/29/12   zy      add code of IE DMO and IE Idele Interval Info
03/10/11   zwj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/23/10   zwj   Mosidy meas report for TDD.
11/08/10   zwj   Modify internal event 14.6.2 and report quantity 14.6.1 for TDD.
11/06/10   zwj   Delete compress mode CCSA 8.4.1.3 for TDD.

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
#include "tdsrrcasn1util.h"

#ifdef FEATURE_TDSCDMA_REL9
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#endif

#include "tdsrrcnv.h"
#include "tdsrrccommon.h"


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSUE_MIN_POWER 21
#define TDSUE_MAX_POWER 104


#define TDSMIN_RX_TX_TIME_DIFF_THRESHOLD 768
#define TDSMAX_RX_TX_TIME_DIFF_THRESHOLD 1280
#define TDSMAX_T_ADV_THERSHOLD   504
#define TDSMAX_TADV_THRESHOLD 8191
#define TDSMAX_SFN_THRESHOLD 4095
#define TDSRRC_TADV_THERSHOLD_2047 2047




/* Internal Meas Event Type */
#define  TDSRET_UEIM_EVENT_TYPE(event_type)  \
(  \
 ((event_type == T_tdsrrc_UE_InternalEventParam_event6a) ? TDSL1_INTERNAL_MEAS_EVENT_6A : \
  ((event_type == T_tdsrrc_UE_InternalEventParam_event6b) ? TDSL1_INTERNAL_MEAS_EVENT_6B : \
  ((event_type == T_tdsrrc_UE_InternalEventParam_event6c) ? TDSL1_INTERNAL_MEAS_EVENT_6C : \
  ((event_type == T_tdsrrc_UE_InternalEventParam_event6d) ? TDSL1_INTERNAL_MEAS_EVENT_6D : \
  ((event_type == T_tdsrrc_UE_InternalEventParam_event6e) ? TDSL1_INTERNAL_MEAS_EVENT_6E : \
  ((event_type == T_tdsrrc_UE_InternalEventParam_event6f) ? TDSL1_INTERNAL_MEAS_EVENT_6F : \
  TDSL1_INTERNAL_MEAS_EVENT_6G )))))) \
)

/* UE Internal Measurement Quantity */
#define  TDSRRCUEIM_RET_MEAS_QUAN(quantity)  \
(  \
 ((quantity == tdsrrc_UE_MeasurementQuantity_ue_TransmittedPower) ? TDSL1_UE_TX_PWR : \
  ((quantity == tdsrrc_UE_MeasurementQuantity_utra_Carrier_RSSI) ? TDSL1_UTRA_RSSI : \
  ((quantity == tdsrrc_UE_MeasurementQuantity_ue_RX_TX_TimeDifference) ? TDSL1_TADV : \
  TDSL1_MEAS_NO_MEAS ))) \
)

/* Converts report deactivation threshold for FDD from ASN1 format to 
 * L1 format 
 */  
#define  TDSRRCUEIM_RET_REPORTING_AMOUNT(reporting_amount)  \
(  \
 ((reporting_amount == tdsrrc_ReportingAmount_ra_Infinity) ? 0 : \
 ((reporting_amount == tdsrrc_ReportingAmount_ra1) ? 1 : \
  ((reporting_amount == tdsrrc_ReportingAmount_ra2) ? 2 : \
  ((reporting_amount == tdsrrc_ReportingAmount_ra4) ? 4 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra8) ? 8 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra16) ? 16 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra32) ? 32 : 64 \
  ))))))) \
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
extern uint8 tdsrrcmeas_pending_rpt;

/*Boolean added to track if interfrequency and interrat measurements are to be processed or not*/
/*Used the same variable to block sending UE internal measurements to L1*/
extern boolean tdsprocess_interfreq_interrat_meas_rel7;

/*Boolean to track if inter frequency measurements are allowed or not
Inter freq measurements are allowed if process_interfreq is TRUE 
(Even in case tdsprocess_interfreq_interrat_meas_rel7 is FALSE)

Use the same variable to unblock UE internal measurements also
*/
extern boolean tdsprocess_interfreq_uei_meas_rel7;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */

/* Maximum No of UE Internal measurements based on additional meas */
tdsueim_meas_id_list_struct_type tdsueim_meas_id_list[TDSMAX_INTERNAL_MEAS];


/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_EVENT_CRITERIA

DESCRIPTION 

 This function processes the event criteria specified in Measurement
 Control Message

DEPENDENCIES

 None

RETURN VALUE

 Success if event parameters are valid
 Failure Otherwise 

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_event_criteria
(
 tdsrrc_UE_InternalReportingCriteria* rrc_event_criteria_ptr,
 tdsl1_internal_meas_rpt_crit_struct_type* l1_event_criteria_ptr
)
{
  /* Store the list in a local pointer */
  tdsrrc_UE_InternalEventParamList * rrc_local_list_ptr = NULL;
  
  uint8 cnt = 0;
  
  rrc_local_list_ptr = &rrc_event_criteria_ptr->ue_InternalEventParamList;
  
  while ((rrc_local_list_ptr->n > cnt) && (cnt < TDSL1_MAX_MEAS_EVENT))
  {
    /* Get Event Id */
    l1_event_criteria_ptr->evt[cnt].event_id =
      TDSRET_UEIM_EVENT_TYPE(rrc_local_list_ptr->elem[cnt].t);
    
    
    switch(l1_event_criteria_ptr->evt[cnt].event_id)
    {
    case TDSL1_INTERNAL_MEAS_EVENT_6A:
      
      /* Txion Power threshold */
      l1_event_criteria_ptr->evt[cnt].ue_tx_pwr_thresh = (int16)
        rrc_local_list_ptr->elem[cnt].u.event6a->transmittedPowerThreshold;
      
      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].ttt =
        TDSRRCMEAS_RET_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[cnt].u.event6a->timeToTrigger);
      
      break;
      
      
    case TDSL1_INTERNAL_MEAS_EVENT_6B:
      /* Txion Power threshold */
      l1_event_criteria_ptr->evt[cnt].ue_tx_pwr_thresh = (int16)
        rrc_local_list_ptr->elem[cnt].u.event6b->transmittedPowerThreshold;
      
      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].ttt =
        TDSRRCMEAS_RET_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[cnt].u.event6b->timeToTrigger);
      
      break;
      
    case TDSL1_INTERNAL_MEAS_EVENT_6C:
      
      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].ttt =
        TDSRRCMEAS_RET_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[cnt].u.event6c);
      
      
      break;
      
    case TDSL1_INTERNAL_MEAS_EVENT_6D:
      
      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].ttt =
        TDSRRCMEAS_RET_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[cnt].u.event6d);
      
      
      break;
    
    case  TDSL1_INTERNAL_MEAS_EVENT_6E:
        
      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].ttt =
        TDSRRCMEAS_RET_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[cnt].u.event6e);
      
      break;

    case TDSL1_INTERNAL_MEAS_EVENT_6F:
      
      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].ttt =
        TDSRRCMEAS_RET_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[cnt].u.event6f->timeToTrigger);
      
      if ((rrc_local_list_ptr->elem[cnt].u.event6f->ue_RX_TX_TimeDifferenceThreshold 
        < TDSMIN_RX_TX_TIME_DIFF_THRESHOLD) ||
        (rrc_local_list_ptr->elem[cnt].u.event6f->ue_RX_TX_TimeDifferenceThreshold
        > TDSMAX_RX_TX_TIME_DIFF_THRESHOLD))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"6F RX-TX-diff-thresold %d out of range",rrc_local_list_ptr->elem[cnt].u.event6f->ue_RX_TX_TimeDifferenceThreshold);
        return FAILURE;
      }
      
      /* 10.3.7.80, real (0..63), step 0.125, RRC send L1 is (0, 504), L1 will translate to real value */
      l1_event_criteria_ptr->evt[cnt].t_adv_thresh = rrc_local_list_ptr->elem[cnt].u.event6f->
             ue_RX_TX_TimeDifferenceThreshold - TDSMIN_RX_TX_TIME_DIFF_THRESHOLD;
      if (l1_event_criteria_ptr->evt[cnt].t_adv_thresh > TDSMAX_T_ADV_THERSHOLD)
      {
        l1_event_criteria_ptr->evt[cnt].t_adv_thresh = TDSMAX_T_ADV_THERSHOLD;
      }
      break;
      
    case TDSL1_INTERNAL_MEAS_EVENT_6G:
      
      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].ttt =
        TDSRRCMEAS_RET_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[cnt].u.event6g->timeToTrigger);
      
      if ((rrc_local_list_ptr->elem[cnt].u.event6g->ue_RX_TX_TimeDifferenceThreshold 
        < TDSMIN_RX_TX_TIME_DIFF_THRESHOLD) ||
        (rrc_local_list_ptr->elem[cnt].u.event6g->ue_RX_TX_TimeDifferenceThreshold
        > TDSMAX_RX_TX_TIME_DIFF_THRESHOLD))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"6G RX-TX-diff-thresold %d out of range",rrc_local_list_ptr->elem[cnt].u.event6g->ue_RX_TX_TimeDifferenceThreshold);
        return FAILURE;
      }
      
      /* 10.3.7.80, real (0..63), step 0.125, RRC send L1 is (0, 504), L1 will translate to real value */
      l1_event_criteria_ptr->evt[cnt].t_adv_thresh = rrc_local_list_ptr->elem[cnt].u.event6g->
                   ue_RX_TX_TimeDifferenceThreshold - TDSMIN_RX_TX_TIME_DIFF_THRESHOLD;

      if (l1_event_criteria_ptr->evt[cnt].t_adv_thresh > TDSMAX_T_ADV_THERSHOLD)
      {
        l1_event_criteria_ptr->evt[cnt].t_adv_thresh = TDSMAX_T_ADV_THERSHOLD;
      }

      break;
      
      
    default:
      break;  
    } /* End of switch */
    
    cnt++;
  } /* End of while */
  
  /* Store the count */
  l1_event_criteria_ptr->num_event = cnt;
  
  return SUCCESS;

}



/*===========================================================================

FUNCTION   RRCUEIM_INITIALIZE_MEAS_ID_LIST

DESCRIPTION 

 This function initializes the measurement identity list of
 UE Internal Measurements

DEPENDENCIES

 None

RETURN VALUE

 None 

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcueim_initialize_meas_id_list
(
 void
)
{
  uint8 count = 0, cnt = 0;
  /* Initialize Measurement Identity variable */
  TDSRRC_MSG0(MSG_LEGACY_LOW,"Initializing Internal Meas Id List");
  for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
  {
    
    tdsueim_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
    tdsueim_meas_id_list[count].meas_validity = TDSSTATE_DCH;
    tdsueim_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
    
    /* Also initialize additional meas */
    for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
      tdsueim_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
  }

}


/*===========================================================================

FUNCTION   RRCUEIM_SEND_MEAS_REPORT

DESCRIPTION 

 This function sends the Measurement Report after validation

DEPENDENCIES

 None

RETURN VALUE

 None 

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcueim_send_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
)
{
  /* Stores meas report */
  tdsrrc_UL_DCCH_Message* msg_ptr = NULL;
  
  /* Stores status of send chain */
  tdsrrcsend_status_e_type status;
  
  uint8 meas_id_cnt = 0, cnt = 0;
  
  /* Indicates if measured results would be appended to event results */
  boolean measured_res_incl = FALSE;
  
  /* RLC ack or unack mode */
  uecomdef_logch_mode_e_type logical_channel_mode;
  
  /* Logical Channel Id */
  rlc_lc_id_type ul_lc_id;
  
  /* Radio bearer mode */
  tdsrrc_RB_Identity rb_mode;
  
  /* L2 ack status */
  tdsrrcsend_l2ack_status_e_type l2_ack_status;    
  tdsrrc_UE_InternalMeasuredResults_LCR_r4  *ue_InternalMeasuredResults_ptr = NULL;
  tdsrrc_MeasuredResultsList_v770xet        *tadv_770ext_ptr = NULL;
  /* Sanity Tests before building Measurement Report */
  /* Check if reporting criteria is O.K. */
  
  if (l1_meas_report_ptr->rpt_crit == TDSL1_NO_RPT ||
  l1_meas_report_ptr->rpt_crit == TDSL1_NO_CHANGE )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rejecting InternalMeasRpt R.Crit=%d",l1_meas_report_ptr->rpt_crit);
    return;
  }
  
  
  if ((l1_meas_report_ptr->rpt_crit == TDSL1_PERIODIC_RPT) &&
    (l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_results_included == TRUE))
  {
    /* Validate Measured Results */
    if (tdsrrcueim_validate_measured_results(
      &l1_meas_report_ptr->u.ue_internal_meas_rpt) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Measured Results");
      return;
    }
    else
    {
      if((l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.ue_tx_pwr_included == TRUE)
        || (l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.t_adv_included == TRUE))
      {  
        measured_res_incl = TRUE;
      }
    }
  }
  else
  {
    if (l1_meas_report_ptr->rpt_crit == TDSL1_EVENT_TRIGGER_RPT)
    {
      if(l1_meas_report_ptr->u.ue_internal_meas_rpt.evt_id > TDSL1_INTERNAL_MEAS_EVENT_6G)                
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Ignore %d Event Report invalid evt id",l1_meas_report_ptr->u.ue_internal_meas_rpt.evt_id);
        return;
      }      
      
      if (l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_results_included == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Meas Results not included");
      }
      else
      {
        /* Now check if measured results are present */
        if (tdsrrcueim_validate_measured_results(
          &l1_meas_report_ptr->u.ue_internal_meas_rpt) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Measured Results");
          /* Nevertheless send event report only */
        }
        else
        {
          if((l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.ue_tx_pwr_included == TRUE)
            || (l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.t_adv_included == TRUE))
          {  
            measured_res_incl = TRUE;
          }
        }
      }
    }
  } /* End of else of event triggered report */
  
  if ( l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE )
  {
    logical_channel_mode = UE_MODE_ACKNOWLEDGED_DATA;
    rb_mode = DCCH_AM_RADIO_BEARER_ID;
    l2_ack_status = TDSRRCSEND_L2ACK_REQUIRED;
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
    return;
  }
  
  /* Allocate a buffer for an uplink DCCH Message. This buffer
  * should be released by SEND_CHAIN using OSS compiler functions */
  
  msg_ptr = (tdsrrc_UL_DCCH_Message *) tdsrrc_malloc (sizeof(struct tdsrrc_UL_DCCH_Message));
  
  if ( msg_ptr == NULL )
  {
    ERR_FATAL("Failed to allocate memory ",0,0,0);
  }
  
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport);
  
  /* Store meas report chosen */
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_measurementReport;
  
  
  /* First store the meas id */
  msg_ptr->message.u.measurementReport.measurementIdentity =
   (uint8) l1_meas_report_ptr->meas_id;
  
  /* Initialize Meas Report bit mask */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport);

  /* Malloc memory for MeasResult_v4b0  */
  tdsrrcmeas_set_measurement_result_v4b0ext(msg_ptr);

  /* Append Event Results if present */
  if (l1_meas_report_ptr->rpt_crit == TDSL1_EVENT_TRIGGER_RPT)
  {
    /* Set event results present */
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.measurementReport,
        tdsrrc_MeasurementReport,eventResults);
    
    /* Set t to Inter RAT Events Results chosen */
    msg_ptr->message.u.measurementReport.eventResults.t = T_tdsrrc_EventResults_ue_InternalEventResults;

     msg_ptr->message.u.measurementReport.eventResults.u.ue_InternalEventResults =
    rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_UE_InternalEventResults);
    
    /* Set Event ID */
    tdsrrcueim_set_event_id(&l1_meas_report_ptr->u.ue_internal_meas_rpt.evt_id,
      msg_ptr->message.u.measurementReport.eventResults.u.ue_InternalEventResults);

    if ((l1_meas_report_ptr->u.ue_internal_meas_rpt.evt_id == TDSL1_INTERNAL_MEAS_EVENT_6F) ||
    (l1_meas_report_ptr->u.ue_internal_meas_rpt.evt_id == TDSL1_INTERNAL_MEAS_EVENT_6G))
    {
      if (l1_meas_report_ptr->u.ue_internal_meas_rpt.cpi > TDSMAX_CPI)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"CPI %d out of range",l1_meas_report_ptr->u.ue_internal_meas_rpt.cpi);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring Meas Report");
      //  tdsrrc_free(msg_ptr);
       tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU,msg_ptr);
        return;
      }

       msg_ptr->message.u.measurementReport.eventResults.u.ue_InternalEventResults->u.event6f =
      rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_PrimaryCPICH_Info);
      /* Set CellParaID */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Event 6F on CPI %d",l1_meas_report_ptr->u.ue_internal_meas_rpt.cpi);
      msg_ptr->message.u.measurementReport.eventResults.u.
        ue_InternalEventResults->u.event6f->primaryScramblingCode = l1_meas_report_ptr->u.ue_internal_meas_rpt.cpi;
      
    }
    
    
  }
  /* Now add Measured Results */
  if (measured_res_incl == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UEIM: Meas Results absent");
  }
  else
  {
    
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
         v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.n = 1;
    
    /* Set Measured Results */   
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
       v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.elem[0].t =
         T_tdsrrc_MeasuredResults_LCR_r4_ue_InternalMeasuredResults;

    ue_InternalMeasuredResults_ptr = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_UE_InternalMeasuredResults_LCR_r4);
    
    if (ue_InternalMeasuredResults_ptr == NULL)
    {
      ERR_FATAL("Unable to allocate memory",0,0,0);
    }
    
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.
            laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext.
            additionalMeasuredResults_LCR.elem[0].u.ue_InternalMeasuredResults = ue_InternalMeasuredResults_ptr;

    if ((l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.ue_tx_pwr_included == TRUE) &&
        (l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.no_pwr != 0))
    {
      ue_InternalMeasuredResults_ptr->ue_TransmittedPowerTDD_List.n = 
              l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.no_pwr;
      
      ALLOC_ASN1ARRAY1(&tdsenc_ctxt, &ue_InternalMeasuredResults_ptr->ue_TransmittedPowerTDD_List,
                   tdsrrc_UE_TransmittedPower);

      if (ue_InternalMeasuredResults_ptr->ue_TransmittedPowerTDD_List.elem == NULL)
      {
        ERR_FATAL("Unable to allocate memory",0,0,0);
      }
      
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2_PTR(ue_InternalMeasuredResults_ptr,
        ue_TransmittedPowerTDD_List);
            
      for (cnt = 0; cnt < l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.no_pwr; cnt++)
      {
        ue_InternalMeasuredResults_ptr->ue_TransmittedPowerTDD_List.elem[cnt]=
          (tdsrrc_UE_TransmittedPower)l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.ue_tx_pwr_list[cnt];
      }
      
    }
    /* Check if Tadv present */
    if (l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.t_adv_included == TRUE) 
    {
      if (l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.t_adv > TDSRRC_TADV_THERSHOLD_2047)
      {
       /*
       UE-InternalMeasuredResults-LCR-r4 ::= SEQUENCE {
          ue-TransmittedPowerTDD-List UE-TransmittedPowerTDD-List OPTIONAL,
              -- If TA value is not greater than 2047, then use this IE to report.
              t-ADVinfo T-ADVinfo OPTIONAL
          }
          
        UE-InternalMeasuredResults-v770ext ::= SEQUENCE {
          modeSpecificInfo CHOICE {
            tdd384-768 SEQUENCE {
              appliedTA EXT-UL-TimingAdvance OPTIONAL
              },
            tdd128 SEQUENCE {
              -- If TA value is greater than 2047, then use this IE to report
              t-ADVinfo T-ADVinfo-ext OPTIONAL
              }
            }
          }
           */
        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent = 1;
        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v5b0NonCriticalExtensionsPresent = 1;
        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.m.v690NonCriticalExtensionsPresent = 1;
        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.m.v770NonCriticalExtensionsPresent = 1;
        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.measurementReport_v770ext.m.additionalMeasuredResultsPresent = 1;
        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.measurementReport_v770ext.additionalMeasuredResults.n = 1;
        ALLOC_ASN1ARRAY1(&tdsenc_ctxt, &msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.measurementReport_v770ext.additionalMeasuredResults, tdsrrc_MeasuredResultsList_v770xet_element);
        tadv_770ext_ptr = &msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.measurementReport_v770ext.additionalMeasuredResults;
        if (tadv_770ext_ptr->elem == NULL)
        {
          ERR_FATAL("Unable to allocate memory",0,0,0);
        }
        tadv_770ext_ptr->elem[0].m.measuredResultsPresent = 1;
        tadv_770ext_ptr->elem[0].measuredResults.t = T_tdsrrc_MeasuredResults_v770ext_ueInternalMeasuredResults;
        tadv_770ext_ptr->elem[0].measuredResults.u.ueInternalMeasuredResults = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_UE_InternalMeasuredResults_v770ext);
        if (tadv_770ext_ptr->elem[0].measuredResults.u.ueInternalMeasuredResults == NULL)
        {
          ERR_FATAL("Unable to allocate memory",0,0,0);
        }
        tadv_770ext_ptr->elem[0].measuredResults.u.ueInternalMeasuredResults->modeSpecificInfo.t = T_tdsrrc_UE_InternalMeasuredResults_v770ext_modeSpecificInfo_tdd128;
        tadv_770ext_ptr->elem[0].measuredResults.u.ueInternalMeasuredResults->modeSpecificInfo.u.tdd128 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_UE_InternalMeasuredResults_v770ext_tdd128);;
        if (tadv_770ext_ptr->elem[0].measuredResults.u.ueInternalMeasuredResults->modeSpecificInfo.u.tdd128 == NULL)
        {
          ERR_FATAL("Unable to allocate memory",0,0,0);
        }
        tadv_770ext_ptr->elem[0].measuredResults.u.ueInternalMeasuredResults->modeSpecificInfo.u.tdd128->m.t_ADVinfoPresent = 1;
        tadv_770ext_ptr->elem[0].measuredResults.u.ueInternalMeasuredResults->modeSpecificInfo.u.tdd128->t_ADVinfo.t_ADV = l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.t_adv;
        tadv_770ext_ptr->elem[0].measuredResults.u.ueInternalMeasuredResults->modeSpecificInfo.u.tdd128->t_ADVinfo.sfn = l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.sfn;
      }
      else
      {
        ue_InternalMeasuredResults_ptr->m.t_ADVinfoPresent = 1;
        ue_InternalMeasuredResults_ptr->t_ADVinfo.t_ADV = 
          l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.t_adv;
        ue_InternalMeasuredResults_ptr->t_ADVinfo.sfn = 
          l1_meas_report_ptr->u.ue_internal_meas_rpt.cell_measured_result_info.sfn;
      }
    }/* Tadv included */
  } /* End of else of measured_res_incl */
  
  
  /* Append Additional Measured Results if available */
  tdsrrcmeas_process_additional_measured_results(
    &l1_meas_report_ptr->meas_id, TDSINTERNAL_MEAS, &msg_ptr->message.u.measurementReport);
  
  if (l1_meas_report_ptr->rpt_crit == TDSL1_PERIODIC_RPT) 
  {
    
    /* Now check if this is the last report. Meas Id needs to be deleted if true */
    if (l1_meas_report_ptr->last_periodic_report == TRUE)
    {
      /* Delete this Measurement */
      for (meas_id_cnt =0; meas_id_cnt < TDSMAX_INTERNAL_MEAS; meas_id_cnt++ )
      {
        if (tdsueim_meas_id_list[meas_id_cnt].meas_id == l1_meas_report_ptr->meas_id)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete Meas %d",l1_meas_report_ptr->meas_id);
          tdsueim_meas_id_list[meas_id_cnt].meas_id = TDSINVALID_MEAS_ID;
          
          tdsueim_meas_id_list[meas_id_cnt].rpt_crit = TDSNOT_AVAILABLE;
          
          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsueim_meas_id_list[meas_id_cnt].additional_meas[cnt] = TDSINVALID_MEAS_ID;
          
          break;
        }
      }
    }
    
    /* Now delete this meas from addtl meas DB */
    if (l1_meas_report_ptr->last_periodic_report == TRUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete %d from addtl meas DB",l1_meas_report_ptr->meas_id);
      tdsrrcmeas_delete_addtl_meas(&l1_meas_report_ptr->meas_id);
    }
  }

  /* Closed measurement_result_v4b0ext if malloc memory no use*/
  tdsrrcmeas_close_measurement_result_v4b0ext(msg_ptr);
  
  /* This places SDU into RLC watermark queue */
  status =  tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_MCMR, (void *) msg_ptr,
    ul_lc_id, l2_ack_status);
  
  if ( status != TDSRRCSEND_SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Send chain failed to send InternalMeas Rpt");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"InternalMeas Rpt sent from RRC=>RLC");
    if(l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE)
    {
      tdsrrcmeas_pending_rpt++; 
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"pending meas rpt = %d",tdsrrcmeas_pending_rpt);
    }
  }
  
  return;

}

/*===========================================================================

FUNCTION   RRCUEIM_SET_EVENT_ID

DESCRIPTION 

 This function sets the event ID in ASN1 format

DEPENDENCIES

 None

RETURN VALUE

 None 

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcueim_set_event_id
(
 tdsl1_internal_meas_event_enum_type* l1_event_type_ptr,
 tdsrrc_UE_InternalEventResults* rrc_event_id_ptr
)
{
  /* Find type of event and set it */
  switch ( *l1_event_type_ptr )
  {
  case TDSL1_INTERNAL_MEAS_EVENT_6A:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UEIM:TDSEVENT 6A reported");
    rrc_event_id_ptr->t = T_tdsrrc_UE_InternalEventResults_event6a;
    break;
    
  case TDSL1_INTERNAL_MEAS_EVENT_6B:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UEIM:TDSEVENT 6B reported");
    rrc_event_id_ptr->t = T_tdsrrc_UE_InternalEventResults_event6b;
    
    break;
    
  case TDSL1_INTERNAL_MEAS_EVENT_6C:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UEIM:TDSEVENT 6C reported");
    rrc_event_id_ptr->t = T_tdsrrc_UE_InternalEventResults_event6c;
    
    break;
    
  case TDSL1_INTERNAL_MEAS_EVENT_6D:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UEIM:TDSEVENT 6D reported");
    rrc_event_id_ptr->t = T_tdsrrc_UE_InternalEventResults_event6d;
    
    break;
    
  case TDSL1_INTERNAL_MEAS_EVENT_6E:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UEIM:TDSEVENT 6E reported");
    rrc_event_id_ptr->t = T_tdsrrc_UE_InternalEventResults_event6e;
    break;

  case TDSL1_INTERNAL_MEAS_EVENT_6F:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UEIM:Event 6F reported");
    rrc_event_id_ptr->t = T_tdsrrc_UE_InternalEventResults_event6f;

    break;
    
  case TDSL1_INTERNAL_MEAS_EVENT_6G:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UEIM:Event 6G reported");
    rrc_event_id_ptr->t = T_tdsrrc_UE_InternalEventResults_event6g;
    break;
   

  default:
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid t");
    break;
  }  /* End of switch */
  
  return;
}


/*===========================================================================

FUNCTION   RRCUEIM_VALIDATE_MEASURED_RESULTS

DESCRIPTION 

 This function validates the measured results

DEPENDENCIES

 None

RETURN VALUE

 Success if validation successful
 Failure Otherwise 

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_validate_measured_results
(
  tdsl1_ue_internal_meas_rpt_struct_type* meas_results_ptr
)
{
  uint8 cnt = 0;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validate Internal Measured Results");

  if (meas_results_ptr->cell_measured_result_info.ue_tx_pwr_included == TRUE)
  {
    for (cnt = 0; cnt < meas_results_ptr->cell_measured_result_info.no_pwr; cnt++)
    {
      if ((meas_results_ptr->cell_measured_result_info.ue_tx_pwr_list[cnt] >= TDSUE_MIN_POWER) &&
        (meas_results_ptr->cell_measured_result_info.ue_tx_pwr_list[cnt] <= TDSUE_MAX_POWER))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"UEIM:UE TX Power is %d",meas_results_ptr->cell_measured_result_info.ue_tx_pwr_list[cnt]);
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"UEIM:UE TX Power invalid %d",meas_results_ptr->cell_measured_result_info.ue_tx_pwr_list[cnt]);
        return FAILURE;
      }
    }
  }
  /* Check if Tadv included. TADV indicates the difference between the Rx timing and Tx timing of a UE */
  if (meas_results_ptr->cell_measured_result_info.t_adv_included == TRUE)
  {
    if (meas_results_ptr->cell_measured_result_info.t_adv > TDSMAX_TADV_THRESHOLD)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Tadv info %d",meas_results_ptr->cell_measured_result_info.t_adv); 
      return FAILURE;
    }    
    if (meas_results_ptr->cell_measured_result_info.sfn > TDSMAX_SFN_THRESHOLD)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"SFN info %d",meas_results_ptr->cell_measured_result_info.sfn); 
      return FAILURE;
    }
  }

  return SUCCESS;
}


/*===========================================================================

FUNCTION   RRCUEIM_FILL_MCM_INFO_R5

DESCRIPTION 

 This function fills the Measurement Control information into the L1 structure

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise 

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_fill_mcm_info_r5
(
  tdsrrc_UE_InternalMeasurement_r4 *ue_InternalMeasurement_ptr,
  tdsl1_ue_internal_meas_ctrl_struct_type *ue_internal_ptr,
  uint8 tdstransaction_id,
  tdsrpt_crit_enum_type *mcm_rpt_crit_ptr
)
{
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ue_InternalMeasurement_ptr, tdsrrc_UE_InternalMeasurement_r4,ue_InternalMeasQuantity))
  {
    /* Set Meas Quantity to present */
    ue_internal_ptr->meas_quan_included = TRUE;

    /* Check if Filter Coefficient is present */

      ue_internal_ptr->filter_coeff_incl = TRUE;

      TDSRRC_MSG0(MSG_LEGACY_LOW,"Filter Coefficient present");

      ue_internal_ptr->filt_idx =
        TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(ue_InternalMeasurement_ptr->ue_InternalMeasQuantity.filterCoefficient);
    

    ue_internal_ptr->meas_quan =
      TDSRRCUEIM_RET_MEAS_QUAN(ue_InternalMeasurement_ptr->ue_InternalMeasQuantity.measurementQuantity);

  } /* End of Meas Quantity Present */

  /* Set Reporting Quantity */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ue_InternalMeasurement_ptr,
     tdsrrc_UE_InternalMeasurement_r4,ue_InternalReportingQuantity))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reporting Quan present");
    ue_internal_ptr->rpt_quan_included = TRUE;

    ue_internal_ptr->rpt_quan.ue_tx_pwr_rpt = 
      ue_InternalMeasurement_ptr->
      ue_InternalReportingQuantity.ue_TransmittedPower;
    /* Get the actual reporting quantity */
    if( TDSRRC_CHECK_COMMON_MSG_TYPE(ue_InternalMeasurement_ptr->
      ue_InternalReportingQuantity.modeSpecificInfo,
      rrc_UE_InternalReportingQuantity_r4_modeSpecificInfo_tdd))
    {
      /* 14.6.1, T-adv required */
      if (ue_InternalMeasurement_ptr->ue_InternalReportingQuantity.modeSpecificInfo.u.tdd->tddOption.t == 
             T_tdsrrc_UE_InternalReportingQuantity_r4_tddOption_tdd128)
      {
        ue_internal_ptr->rpt_quan.ue_t_adv = ue_InternalMeasurement_ptr->ue_InternalReportingQuantity.
                    modeSpecificInfo.u.tdd->tddOption.u.tdd128->t_ADVinfo;
      }
    }

  }

  /* Get Reporting Criteria */
  if (ue_InternalMeasurement_ptr->reportCriteria.t == T_tdsrrc_UE_InternalReportCriteria_ue_InternalReportingCriteria)
  {
    /* Set criteria to event triggered */
    ue_internal_ptr->rpt_mode = TDSL1_EVENT_TRIGGER_RPT;

    *mcm_rpt_crit_ptr = TDSEVENT;

    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ue_InternalMeasurement_ptr->reportCriteria.u.ue_InternalReportingCriteria,
        ue_InternalEventParamList))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Event List present");
    }

    /* Event Triggered Reporting Chosen */
    if (tdsrrcueim_process_event_criteria(ue_InternalMeasurement_ptr->reportCriteria.u.ue_InternalReportingCriteria,
      &ue_internal_ptr->u.evt_trig_crit) == FAILURE)
    {

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Event Criteria specified");

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF mhsg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(
    ue_InternalMeasurement_ptr->reportCriteria,rrc_UE_InternalReportCriteria_periodicalReportingCriteria))
  {
    /* Set periodic reporting */
    ue_internal_ptr->rpt_mode = TDSL1_PERIODIC_RPT;

    *mcm_rpt_crit_ptr = TDSPERIODIC;
    /* Periodic Reporting Chosen */
    /* Check if reporting amount present */

    /* Assign Default Value i.e. Infinity */
    ue_internal_ptr->u.periodic_crit.rpt_amount
      = TDSL1_RPT_AMOUNT_INFINITY;

 
    /* Store reporting amount */
    ue_internal_ptr->u.periodic_crit.rpt_amount =
      (tdsl1_amount_of_reporting_enum_type)
      TDSRRCUEIM_RET_REPORTING_AMOUNT(ue_InternalMeasurement_ptr->reportCriteria.u.periodicalReportingCriteria->reportingAmount);
  

    /* Store reporting interval */
    ue_internal_ptr->u.periodic_crit.rpt_interval =
      (tdsl1_reporting_interval_enum_type)
      TDSRRCMEAS_RET_PERIODIC_REPORTING_INTERVAL(
      ue_InternalMeasurement_ptr->reportCriteria.u.periodicalReportingCriteria->reportingInterval);


  } /* End of else of periodic reporting chosen */
  else
  {
    /* Set periodic reporting */
    ue_internal_ptr->rpt_mode = TDSL1_NO_RPT;

    *mcm_rpt_crit_ptr = TDSADDTL;
  }

  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM_R5

DESCRIPTION 

 This function processes the Measurement Control Message rel 5

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise 

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm_r5
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the UE Internal Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_internal_meas_params;
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTERNAL_MEAS;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r4_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;
  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.r4.measurementControl_r4; 

  /* Assign default values to inter_rat_meas_params */
  memset(&l1_internal_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));
  
  l1_internal_meas_params.meas_object.meas_type = TDSL1_UE_INTERNAL;
  
  /* Set Transaction Id and Meas Id */
  l1_internal_meas_params.trans_id = (uint16) tdstransaction_id;
  l1_internal_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;
  
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r4_setup))
  {
  /* Check if meas quantity,reporting quantity & Reporting criteria is received.
    If either of them is not received, then configuration incomplete ERROR */
    
    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalReportingQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/Rpt Mode");
      
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set failure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    
    /* Check if Reporting mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))
    {
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode =
        TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
    }
    
    if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }
   
    /* Store reporting mode to TRUE */
    l1_internal_meas_params.rpt_mode_incl = TRUE;
    
    l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    
    
    /* Now set other values */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    
    l1_internal_meas_params.meas_object_info_included = TRUE;
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
    
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");
      
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
      
    }
    
    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsueim_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsueim_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          break;
        }
      }
      
    } /* End of if */
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
    
    /* Now one needs to send command to L1 */
    
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r4_modify))
  {
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
    
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if Meas Type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r4_modify,measurementType))
    {
      if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
      {
        return FAILURE;
      }
      
      
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
      l1_internal_meas_params.meas_object_info_included = TRUE;
      
    }
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
          addtl_meas_incl, FALSE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
                &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
         rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */

    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* This meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        break;
      }
    }
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdsueim_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
    
    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_internal_meas_params.meas_id);
    
    /* Clear Meas Id from tdsueim_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsueim_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsueim_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending internal measurements from MCM to L1");
    return SUCCESS;
  }

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_internal_meas_params.trans_id = alloc_idx;
  TDSRRC_MSG0(MSG_LEGACY_LOW,"MCM:Sending Internal Meas cmd->L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_internal_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM_R6

DESCRIPTION 

 This function processes the Measurement Control Message rel 6

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise 

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm_r6
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the UE Internal Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_internal_meas_params;
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTERNAL_MEAS;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r6_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;
  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.r6.measurementControl_r6; 

  /* Assign default values to inter_rat_meas_params */
  memset(&l1_internal_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_internal_meas_params.meas_object.meas_type = TDSL1_UE_INTERNAL;
  
  /* Set Transaction Id and Meas Id */
  l1_internal_meas_params.trans_id = (uint16) tdstransaction_id;
  l1_internal_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;
  
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r6_setup))
  {
  /* Check if meas quantity,reporting quantity & Reporting criteria is received.
    If either of them is not received, then configuration incomplete ERROR */
    
    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalReportingQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/Rpt Mode");
      
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set failure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    
    /* Check if Reporting mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode))
    {
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode =
        TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
    }
    
    if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    
    /* Store reporting mode to TRUE */
    l1_internal_meas_params.rpt_mode_incl = TRUE;
    
    l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    
    
    /* Now set other values */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    
    l1_internal_meas_params.meas_object_info_included = TRUE;
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
    
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");
      
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
      
    }
    
    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsueim_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsueim_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          break;
        }
      }
      
    } /* End of if */
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
    
    /* Now one needs to send command to L1 */
    
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r6_modify))
  {
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
    
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if Meas Type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r6_modify,measurementType))
    {
      
      if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
      {
        return FAILURE;
      }

      
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
      l1_internal_meas_params.meas_object_info_included = TRUE;
      
    }
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
          addtl_meas_incl, FALSE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
                &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */


    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* This meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        break;
      }
    }
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdsueim_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
    
    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_internal_meas_params.meas_id);
    
    /* Clear Meas Id from tdsueim_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsueim_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsueim_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending internal measurements from MCM to L1");
    return SUCCESS;
  }

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_internal_meas_params.trans_id = alloc_idx;
  TDSRRC_MSG0(MSG_LEGACY_LOW,"MCM:Sending Internal Meas cmd->L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_internal_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}


/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM_R7

DESCRIPTION 

 This function processes the Measurement Control Message rel 7

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise 

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm_r7
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the UE Internal Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_internal_meas_params;
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTERNAL_MEAS;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r7_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.r7.measurementControl_r7;

  /* Assign default values to inter_rat_meas_params */
  memset(&l1_internal_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_internal_meas_params.meas_object.meas_type = TDSL1_UE_INTERNAL;
  
  /* Set Transaction Id and Meas Id */
  l1_internal_meas_params.trans_id = (uint16) tdstransaction_id;
  l1_internal_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;
  
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r7_setup))
  {
  /* Check if meas quantity,reporting quantity & Reporting criteria is received.
    If either of them is not received, then configuration incomplete ERROR */
    
    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalReportingQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/Rpt Mode");
      
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set failure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    
    /* Check if Reporting mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode))
    {
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode =
        TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
    }
    
    if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    
    /* Store reporting mode to TRUE */
    l1_internal_meas_params.rpt_mode_incl = TRUE;
    
    l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    
    
    /* Now set other values */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    
    l1_internal_meas_params.meas_object_info_included = TRUE;
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
    
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");
      
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
      
    }
    
    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsueim_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsueim_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          break;
        }
      }
      
    } /* End of if */
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
    
    /* Now one needs to send command to L1 */
    
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r7_modify))
  {
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
    
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if Meas Type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r7_modify,measurementType))
    {
      
      if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
      {
        return FAILURE;
      }

      
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
      l1_internal_meas_params.meas_object_info_included = TRUE;
      
    }
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
          addtl_meas_incl, FALSE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
                &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */


    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* This meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        break;
      }
    }
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdsueim_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
    
    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_internal_meas_params.meas_id);
    
    /* Clear Meas Id from tdsueim_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsueim_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsueim_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */
  
    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending internal measurements from MCM to L1");
    return SUCCESS;
  }
  
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_internal_meas_params.trans_id = alloc_idx;

  TDSRRC_MSG0(MSG_LEGACY_LOW,"MCM:Sending Internal Meas cmd->L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_internal_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}


#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION   RRCUEIM_PROCESS_MCM_R8

DESCRIPTION 
 This function processes the Measurement Control Message rel 8

DEPENDENCIES
 None

RETURN VALUE
 Success if message successfully processed
 Failure otherwise 

SIDE EFFECTS
 None
===========================================================================*/
uecomdef_status_e_type tdsrrcueim_process_mcm_r8
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the UE Internal Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_internal_meas_params;
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTERNAL_MEAS;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r8_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.measurementControl_r8;

  /* Assign default values to inter_rat_meas_params */
  memset(&l1_internal_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_internal_meas_params.meas_object.meas_type = TDSL1_UE_INTERNAL;
  
  /* Set Transaction Id and Meas Id */
  l1_internal_meas_params.trans_id = (uint16) tdstransaction_id;
  l1_internal_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;
  
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r8_setup))
  {
  /* Check if meas quantity,reporting quantity & Reporting criteria is received.
    If either of them is not received, then configuration incomplete ERROR */
    
    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalReportingQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/Rpt Mode");
      
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set failure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    
    /* Check if Reporting mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode))
    {
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode =
        TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
    }
    
    if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    
    /* Store reporting mode to TRUE */
    l1_internal_meas_params.rpt_mode_incl = TRUE;
    
    l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    
    
    /* Now set other values */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    
    l1_internal_meas_params.meas_object_info_included = TRUE;
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
    
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
      
    }
    
    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsueim_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsueim_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          break;
        }
      }
    } /* End of if */
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
    
    /* Now one needs to send command to L1 */
    
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r8_modify))
  {
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
    
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if Meas Type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r8_modify,measurementType))
    {
      
      if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
      {
        return FAILURE;
      }

      
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
      l1_internal_meas_params.meas_object_info_included = TRUE;
      
    }
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
          addtl_meas_incl, FALSE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
                &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */


    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* This meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        break;
      }
    }
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdsueim_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
    
    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_internal_meas_params.meas_id);
    
    /* Clear Meas Id from tdsueim_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsueim_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsueim_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */
  
    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */

  if((!tdsprocess_interfreq_interrat_meas_rel7) && (!tdsprocess_interfreq_uei_meas_rel7))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending internal measurements from MCM to L1");
    return SUCCESS;
  }
  
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_internal_meas_params.trans_id = alloc_idx;

  TDSRRC_MSG0(MSG_LEGACY_LOW,"MCM:Sending Internal Meas cmd->L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_internal_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}
#endif /*FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================
FUNCTION   RRCUEIM_PROCESS_MCM_R9

DESCRIPTION 
 This function processes the Measurement Control Message rel 9

DEPENDENCIES
 None

RETURN VALUE
 Success if message successfully processed
 Failure otherwise 

SIDE EFFECTS
 None
===========================================================================*/
uecomdef_status_e_type tdsrrcueim_process_mcm_r9
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the UE Internal Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_internal_meas_params;
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTERNAL_MEAS;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r9_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.
    criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.measurementControl_r9;

  /* Assign default values to inter_rat_meas_params */
  memset(&l1_internal_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_internal_meas_params.meas_object.meas_type = TDSL1_UE_INTERNAL;
  
  /* Set Transaction Id and Meas Id */
  l1_internal_meas_params.trans_id = (uint16) tdstransaction_id;
  l1_internal_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;
  
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r8_setup))
  {
  /* Check if meas quantity,reporting quantity & Reporting criteria is received.
    If either of them is not received, then configuration incomplete ERROR */
    
    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      tdsrrc_UE_InternalMeasurement_r4,ue_InternalReportingQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r9_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/Rpt Mode");
      
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set failure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    
    /* Check if Reporting mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r9_IEs,measurementReportingMode))
    {
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode =
        TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
    }
    
    if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.setup->u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    
    /* Store reporting mode to TRUE */
    l1_internal_meas_params.rpt_mode_incl = TRUE;
    
    l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    
    
    /* Now set other values */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    
    l1_internal_meas_params.meas_object_info_included = TRUE;
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
    
    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_incl, TRUE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
      
    }
    
    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsueim_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsueim_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          break;
        }
      }
    } /* End of if */
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
    
    /* Now one needs to send command to L1 */
    
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r9_modify))
  {
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
    
    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r9_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if Meas Type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r9_modify,measurementType))
    {
      
      if(FAILURE == tdsrrcueim_fill_mcm_info_r5(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.ue_InternalMeasurement,
      &l1_internal_meas_params.meas_object.u.ue_internal, tdstransaction_id, &mcm_rpt_crit))
      {
        return FAILURE;
      }

      
      /* Store reporting mode to TRUE */
      l1_internal_meas_params.rpt_mode_incl = TRUE;
      
      l1_internal_meas_params.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
      
      l1_internal_meas_params.meas_object_info_included = TRUE;
      
    }
    
    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
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
          addtl_meas_incl, FALSE, TDSINTERNAL_MEAS, rrc_mcm_ptr->measurementIdentity,
                &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
        /* Can't set additional meas in DB now as meas id has not yet been set */
      }
    } /* End of if of Additional Meas Present */


    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTERNAL_MEAS; count++ )
    {
      /* This meas already exists */
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        break;
      }
    }
    
    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }
  } /* End of else if of modify */
  else
  {
    /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdsueim_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_internal_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
    
    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_internal_meas_params.meas_id);
    
    /* Clear Meas Id from tdsueim_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsueim_meas_id_list[count].meas_id == l1_internal_meas_params.meas_id)
      {
        tdsueim_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsueim_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsueim_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */
  
    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */

#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
  if(TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO)
  {
    if(TRUE == rrc_mcm_ptr->m.cellDCHMeasOccasionInfo_TDD128Present)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: cellDCHMeasOccasionInfo_TDD128 IE present and supported!");

      switch(tdsrrcllc_get_dmo_r9(&rrc_mcm_ptr->cellDCHMeasOccasionInfo_TDD128,
                            &l1_internal_meas_params.meas_occasion_info))
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

      l1_internal_meas_params.meas_occasion_info_incl = TRUE;
      /* save in CC, in order to revert old config, send to L1 old config */
      tdscurrent_config_ptr->meas_occasion_info_incl = TRUE;

      memscpy(&tdscurrent_config_ptr->meas_occasion_info,
	      sizeof(tdsl1_meas_occasion_info_struct_type),
             &l1_internal_meas_params.meas_occasion_info,
             sizeof(tdsl1_meas_occasion_info_struct_type));
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: cellDCHMeasOccasionInfo_TDD128 IE is not present!");
      l1_internal_meas_params.meas_occasion_info_incl = FALSE;
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
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending internal measurements from MCM to L1");
    return SUCCESS;
  }
  
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_internal_meas_params.trans_id = alloc_idx;

  TDSRRC_MSG0(MSG_LEGACY_LOW,"MCM:Sending Internal Meas cmd->L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_internal_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}
#endif /*FEATURE_TDSCDMA_REL9*/


