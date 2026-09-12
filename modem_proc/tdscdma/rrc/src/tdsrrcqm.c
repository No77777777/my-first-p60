/*===========================================================================
            R R C   M E A S U R E M E N T S

DESCRIPTION

   This module supports Quality Measurement Control and Measurement Reporting
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcqm.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/17/12   zy      fix CR430249, and add the check of m_length and k in DMO
09/29/12   zy      add code of IE DMO and IE Idele Interval Info
03/10/11   zwj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/23/10   zwj     Modify meas report for TDD.
11/08/10   zwj     Modify Quality report quantity 14.5.1 for TDD. 

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
#define TDSMAX_BLER 63
#define TDSMAX_CRC 512
#define TDSMIN_CRC 1
#define TDSMAX_PENDING_AFTER_TRIGGER 512
#define TDSMIN_PENDING_AFTER_TRIGGER 1

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

tdsquality_meas_id_list_struct_type tdsquality_meas_id_list[TDSMAX_QUALITY_MEAS];


/* -----------------------------------------------------------------------
** Local Object Definition
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
extern uint8 tdsrrcmeas_pending_rpt;
/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */

/* =======================================================================
**                            Function Definitions
** ======================================================================= */


/*==========================================================================
 FUNCTION   RRCQM_FILL_MCM_INFO

DESCRIPTION

  This function fills the L1 structure for Quality Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_fill_mcm_info
(
  tdsrrc_QualityMeasurement *qualityMeasurement_ptr,
  tdsl1_qual_meas_ctrl_struct_type  *qual_ptr,
  uint8 tdstransaction_id,
  tdsrpt_crit_enum_type *mcm_rpt_crit_ptr
)
{
  uint8 count = 0, cnt = 0, count1 = 0;
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  /* Stores mode  parameters in report quantity*/
  tdsrrc_QualityReportingQuantity_modeSpecificInfo *mode_ptr;
  /* local ptr to the trch list for BLER measurement */
  tdsrrc_BLER_TransChIdList *bler_dl_TransChIdList_ptr = NULL;
                       
  /* local ptr to the event criteria list */
  tdsrrc_QualityReportingCriteria *qualityReportingCriteria_ptr = NULL;

  qual_ptr->bler_rpt_requested = FALSE;
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(qualityMeasurement_ptr,qualityReportingQuantity))
  {
    if(qualityMeasurement_ptr->qualityReportingQuantity.dl_TransChBLER == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: BLER rpt true");        
      qual_ptr->bler_rpt_requested = TRUE; 
      qual_ptr->rpt_all_transport_ch = TRUE;
      if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(qualityMeasurement_ptr->qualityReportingQuantity,
         bler_dl_TransChIdList)) && 
        (qualityMeasurement_ptr->qualityReportingQuantity.
        bler_dl_TransChIdList.n != 0))
      {
        qual_ptr->rpt_all_transport_ch = FALSE;
        bler_dl_TransChIdList_ptr = &qualityMeasurement_ptr->
          qualityReportingQuantity.bler_dl_TransChIdList;
        
        while((bler_dl_TransChIdList_ptr->n > cnt) && (cnt < TDSL1_MAX_TRANSPORT_CHANNELS))
        {
          if((bler_dl_TransChIdList_ptr->elem[cnt] > TDSL1_MAX_TRANSPORT_CHANNELS)
            || (bler_dl_TransChIdList_ptr->elem[cnt] < 1))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Invalid Tr ch id");
            /* Set faliure choice */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }         
          qual_ptr->reported_transport_channels[cnt]
            =  (uint8) bler_dl_TransChIdList_ptr->elem[cnt];
          cnt++;
        }     
        qual_ptr->no_of_tr_ch = cnt;   
      }      
    }
    
    /* Zero means no need to measure SIR  */
    qual_ptr->no_of_cctrch = 0;      

    mode_ptr = &qualityMeasurement_ptr->qualityReportingQuantity.modeSpecificInfo;
    
    /* 10.3.5.79, processing SIR */
    if((mode_ptr->t == T_tdsrrc_QualityReportingQuantity_modeSpecificInfo_tdd) && 
          (mode_ptr->u.tdd->m.sir_TFCS_ListPresent == 1))
    {
      qual_ptr->no_of_cctrch = mode_ptr->u.tdd->sir_TFCS_List.n;
      for (cnt = 0; cnt < qual_ptr->no_of_cctrch; cnt++)
      {
        qual_ptr->tfcs_id[cnt] = mode_ptr->u.tdd->sir_TFCS_List.elem[cnt];
      }
    }
  }


  /* processing the report criteria IE */ 
  switch(qualityMeasurement_ptr->reportCriteria.t)
  {
  case T_tdsrrc_QualityReportCriteria_qualityReportingCriteria:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: processing event criteria");   
    cnt = 0;   
    *mcm_rpt_crit_ptr =  TDSEVENT; 
    qual_ptr->rpt_mode = TDSL1_EVENT_TRIGGER_RPT;
    qualityReportingCriteria_ptr = qualityMeasurement_ptr->reportCriteria.u.qualityReportingCriteria;
    
    while((qualityReportingCriteria_ptr->n > cnt ) && (cnt < TDSL1_MAX_TRANSPORT_CHANNELS))
    {
      if((qualityReportingCriteria_ptr->elem[cnt].transportChannelIdentity > TDSL1_MAX_TRANSPORT_CHANNELS)
        ||(qualityReportingCriteria_ptr->elem[cnt].transportChannelIdentity < 1))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Invalid Trch id");
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE; 
      }                         
      qual_ptr->u.evt_trig_crit.tr_ch[cnt].tr_ch_id =
        (uint8) qualityReportingCriteria_ptr->elem[cnt].transportChannelIdentity;
      
      if((qualityReportingCriteria_ptr->elem[cnt].totalCRC > TDSMAX_CRC) ||
        (qualityReportingCriteria_ptr->elem[cnt].totalCRC < TDSMIN_CRC) ||
        (qualityReportingCriteria_ptr->elem[cnt].badCRC > TDSMAX_CRC) ||
        (qualityReportingCriteria_ptr->elem[cnt].badCRC < TDSMIN_CRC))       
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Invalid CRC value");
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE; 
      }
      
      if((qualityReportingCriteria_ptr->elem[cnt].pendingAfterTrigger > TDSMAX_PENDING_AFTER_TRIGGER) ||
        (qualityReportingCriteria_ptr->elem[cnt].pendingAfterTrigger < TDSMIN_PENDING_AFTER_TRIGGER))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Invalid Pending After Trigger value");
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE; 
      }
      
      if(qualityReportingCriteria_ptr->elem[cnt].totalCRC < qualityReportingCriteria_ptr->elem[cnt].badCRC)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"MODIFY:total CRC < BAD CRC");
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE; 
      }
      qual_ptr->u.evt_trig_crit.tr_ch[cnt].total_crc =
        (uint16) qualityReportingCriteria_ptr->elem[cnt].totalCRC;
      qual_ptr->u.evt_trig_crit.tr_ch[cnt].bad_crc =
        (uint16) qualityReportingCriteria_ptr->elem[cnt].badCRC;
      qual_ptr->u.evt_trig_crit.tr_ch[cnt].pending_time_after_trigger =
        (uint16) qualityReportingCriteria_ptr->elem[cnt].pendingAfterTrigger;
      cnt++;
    }
    
    qual_ptr->u.evt_trig_crit.no_of_transport_channels = cnt;
    for(count = 0; count < cnt-1; count++)
    {
      for(count1 = count+1; count1 < cnt; count1++)
      {
        if(qual_ptr->u.evt_trig_crit.tr_ch[count].tr_ch_id
          == qual_ptr->u.evt_trig_crit.tr_ch[count1].tr_ch_id)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:trch id repeated");
          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE; 
        }
      }   
    }  
    break;
    
  case T_tdsrrc_QualityReportCriteria_periodicalReportingCriteria:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: processing periodic criteria");               
    *mcm_rpt_crit_ptr =  TDSPERIODIC; 
    qual_ptr->rpt_mode = TDSL1_PERIODIC_RPT;
    
    qual_ptr->u.periodic_crit.rpt_amount = TDSL1_RPT_AMOUNT_INFINITY;
    
 
    qual_ptr->u.periodic_crit.rpt_amount = (tdsl1_amount_of_reporting_enum_type) 
      TDSRRCMEAS_RET_REPORTING_AMOUNT(qualityMeasurement_ptr->reportCriteria.u.
      periodicalReportingCriteria->reportingAmount);
    
    
    if(qualityMeasurement_ptr->
      reportCriteria.u.periodicalReportingCriteria->reportingInterval == tdsrrc_ReportingIntervalLong_ril0)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"rpt interval 0 using 250");
      qual_ptr->u.periodic_crit.rpt_interval = TDSL1_RPT_INTERVAL_250MS; 
    }
    else
    {
      qual_ptr->u.periodic_crit.rpt_interval = (tdsl1_reporting_interval_enum_type) 
        TDSRRCMEAS_RET_PERIODIC_REPORTING_INTERVAL(qualityMeasurement_ptr->reportCriteria.
        u.periodicalReportingCriteria->reportingInterval);
    }    
    
    break;
    
  case T_tdsrrc_QualityReportCriteria_noReporting:
    qual_ptr->rpt_mode = TDSL1_NO_RPT;
    break; 
    
  default: /* Invalid t value */
    break;
  }
  return SUCCESS;
}

/*==========================================================================
 FUNCTION   RRCQM_INITIALIZE_MEAS_ID_LIST

DESCRIPTION

  This function initializes the measurement identity list for
  Quality Measurements

DEPENDENCIES

    None.

RETURN VALUE

    None
    
SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcqm_initialize_meas_id_list()
{
  uint8 count, cnt;
   
  for(count = 0; count < TDSMAX_QUALITY_MEAS; count ++)
  {
    tdsquality_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
    tdsquality_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
    tdsquality_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;

    /* Also initialize additional meas */
    for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
      tdsquality_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
   }
}

/*===========================================================================

FUNCTION   RRCQM_SEND_MEAS_REPORT

DESCRIPTION 

 This function sends the Quality Measurement Report after validation

DEPENDENCIES

 None

RETURN VALUE

 None 

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcqm_send_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
)
{
  /* Stores meas report */
  tdsrrc_UL_DCCH_Message* msg_ptr = NULL;
  
  /* Stores status of send chain */
  tdsrrcsend_status_e_type status;
  
  uint8 meas_id_cnt = 0, cnt = 0;
  uint8 i, j;
  uint8  no_ts_offset = 0;
  /* RLC ack or unack mode */
  uecomdef_logch_mode_e_type logical_channel_mode;
  
  /* Logical Channel Id */
  rlc_lc_id_type ul_lc_id;
  
  /* Radio bearer mode */
  tdsrrc_RB_Identity rb_mode;
  
  /* L2 ack status */
  tdsrrcsend_l2ack_status_e_type l2_ack_status;  
  
  tdsrrc_BLER_MeasurementResultsList *blerMeasurementResultsList_ptr = NULL;

  tdsrrc_QualityEventResults *qualityEventResults_ptr = NULL;

  tdsrrc_QualityMeasuredResults *qualityMeasuredResults_ptr = NULL;
  
  /* Sanity Tests before building Measurement Report */
  /* Check if reporting criteria is O.K. */
  
  if (l1_meas_report_ptr->rpt_crit == TDSL1_NO_RPT ||
    l1_meas_report_ptr->rpt_crit == TDSL1_NO_CHANGE || l1_meas_report_ptr->rpt_crit == TDSL1_INVALID_RPT_MODE)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rejecting QualityMeasRpt R.Crit=%d",l1_meas_report_ptr->rpt_crit);
    return;
  }
  
  if (l1_meas_report_ptr->u.qual_meas_rpt.no_of_tr_ch == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Reject QualityMeasRpt 0 tr ch");
    return;
  }
  
  if (l1_meas_report_ptr->u.qual_meas_rpt.no_of_tr_ch > TDSL1_MAX_TRANSPORT_CHANNELS)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Reject QualityMeasRpt num tr ch %d > max",l1_meas_report_ptr->u.qual_meas_rpt.no_of_tr_ch);
    return;
  }
  
  for(cnt = 0; cnt < l1_meas_report_ptr->u.qual_meas_rpt.no_of_tr_ch; cnt++)
  {
    if((l1_meas_report_ptr->u.qual_meas_rpt.l1_trch_bler[cnt].transport_ch_identity > TDSL1_MAX_TRANSPORT_CHANNELS)
      || (l1_meas_report_ptr->u.qual_meas_rpt.l1_trch_bler[cnt].transport_ch_identity < 1))       
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Reject QualityMeasRpt invalid tr ch id %d", l1_meas_report_ptr->u.qual_meas_rpt.
        l1_trch_bler[cnt].transport_ch_identity,0,0);
      return;
    }
    if((l1_meas_report_ptr->rpt_crit == TDSL1_PERIODIC_RPT) &&
      (l1_meas_report_ptr->u.qual_meas_rpt.l1_trch_bler[cnt].bler_present == TRUE) &&
      (l1_meas_report_ptr->u.qual_meas_rpt.l1_trch_bler[cnt].bler_value > TDSMAX_BLER))
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Reject QualityMeasRpt invalid BLER value %d", l1_meas_report_ptr->u.qual_meas_rpt.
        l1_trch_bler[cnt].bler_value,0,0);
      return;
    }
  }
  
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
    (uint8)l1_meas_report_ptr->meas_id;
  
  /* Initialize Meas Report bit mask */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport);

  /* Malloc memory for MeasResult_v4b0  */
  tdsrrcmeas_set_measurement_result_v4b0ext(msg_ptr);
  
  switch(l1_meas_report_ptr->rpt_crit)
  {
  case TDSL1_EVENT_TRIGGER_RPT:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: event 5A");       
    /* Set event results present */
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.measurementReport,
    tdsrrc_MeasurementReport,eventResults);
    
    /* Set choice to Inter RAT Events Results chosen */
    TDSRRC_SET_COMMON_MSG_TYPE2(msg_ptr->message.u.measurementReport.eventResults,
    rrc_EventResults,qualityEventResults);
    msg_ptr->message.u.measurementReport.eventResults.u.qualityEventResults = 
     rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_QualityEventResults );

    if( msg_ptr->message.u.measurementReport.eventResults.u.qualityEventResults == NULL)
    {
      ERR_FATAL("Quality rpt Unable allocate memory",0,0,0); 
    }
     msg_ptr->message.u.measurementReport.eventResults.u.qualityEventResults->n = 
    l1_meas_report_ptr->u.qual_meas_rpt.no_of_tr_ch;

    ALLOC_ASN1ARRAY1(&tdsenc_ctxt,
        msg_ptr->message.u.measurementReport.eventResults.u.qualityEventResults,
        tdsrrc_TransportChannelIdentity);
    if( msg_ptr->message.u.measurementReport.eventResults.u.qualityEventResults->elem == NULL)
    {
      ERR_FATAL("Quality rpt Unable allocate memory for list",0,0,0); 
    }
    qualityEventResults_ptr = msg_ptr->message.u.measurementReport.eventResults.u.qualityEventResults;
    for(cnt = 0; cnt < l1_meas_report_ptr->u.qual_meas_rpt.no_of_tr_ch; cnt++)
    {
      
      qualityEventResults_ptr->elem[cnt] = 
         l1_meas_report_ptr->u.qual_meas_rpt.
        l1_trch_bler[cnt].transport_ch_identity;
    
    } 
    
  case TDSL1_PERIODIC_RPT:
    /* set periodic measured results to TRUE */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: periodic BLER rpt");
    
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
         v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.n = 1;
    
    /* set measured results LCR_r4_qualityMeasuredResults present */
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
         v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.elem[0].t = 
          T_tdsrrc_MeasuredResults_LCR_r4_qualityMeasuredResults;

    qualityMeasuredResults_ptr = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_QualityMeasuredResults);

    if (qualityMeasuredResults_ptr == NULL)
    {
      ERR_FATAL("Quality rpt Unable allocate memory",0,0,0); 
    }

    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
          v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.
          elem[0].u.qualityMeasuredResults = qualityMeasuredResults_ptr;


    TDSRRC_RESET_MSG_IE_PRESENT_PTR(qualityMeasuredResults_ptr);
    
    TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2_PTR(qualityMeasuredResults_ptr,blerMeasurementResultsList);
    
    qualityMeasuredResults_ptr->blerMeasurementResultsList.n =
      l1_meas_report_ptr->u.qual_meas_rpt.no_of_tr_ch;
    
    blerMeasurementResultsList_ptr= &qualityMeasuredResults_ptr->blerMeasurementResultsList;
    
    ALLOC_ASN1ARRAY1(&tdsenc_ctxt,
        blerMeasurementResultsList_ptr,
        tdsrrc_BLER_MeasurementResults);

    if(blerMeasurementResultsList_ptr->elem == NULL)
    {
        ERR_FATAL("Quality rpt Unable allocate memory",0,0,0); 
    }
    for(cnt = 0; cnt < l1_meas_report_ptr->u.qual_meas_rpt.no_of_tr_ch; cnt++)
    {
      blerMeasurementResultsList_ptr->elem[cnt].transportChannelIdentity
        =  l1_meas_report_ptr->u.qual_meas_rpt.
        l1_trch_bler[cnt].transport_ch_identity;
      
      TDSRRC_RESET_MSG_IE_PRESENT(blerMeasurementResultsList_ptr->elem[cnt]);
      
      if(l1_meas_report_ptr->u.qual_meas_rpt.
        l1_trch_bler[cnt].bler_present == TRUE)
      {   

        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(blerMeasurementResultsList_ptr->elem[cnt],
          dl_TransportChannelBLER);
        
        blerMeasurementResultsList_ptr->elem[cnt].dl_TransportChannelBLER
          =  l1_meas_report_ptr->u.qual_meas_rpt.
          l1_trch_bler[cnt].bler_value;       
      }
    }
    /* 10.3.7.55 - Quality Measured Results List */
    TDSRRC_SET_COMMON_MSG_TYPE(qualityMeasuredResults_ptr->modeSpecificInfo,
      rrc_QualityMeasuredResults_modeSpecificInfo_tdd);
    
    qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd = 
      rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_QualityMeasuredResults_tdd);
    
    if(qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd == NULL)
    {
      ERR_FATAL("Quality rpt Unable allocate memory",0,0,0); 
    }
    
    TDSRRC_RESET_MSG_IE_PRESENT_PTR(qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd);
    
    if (l1_meas_report_ptr->u.qual_meas_rpt.no_of_cctrch != 0)
    {
      qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->m.sir_MeasurementResultsPresent = 1;

      qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.n =
         l1_meas_report_ptr->u.qual_meas_rpt.no_of_cctrch;
      
      ALLOC_ASN1ARRAY1(&tdsenc_ctxt,
        &qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults,
        tdsrrc_SIR_MeasurementResults);

      if(qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.elem == NULL)
      {
        ERR_FATAL("Quality rpt Unable allocate memory",0,0,0); 
      }  

      for (i = 0; i < l1_meas_report_ptr->u.qual_meas_rpt.no_of_cctrch; i++)
      {
        if(l1_meas_report_ptr->u.qual_meas_rpt.l1_sir_result[i].no_ts == 0)
        {
          /* timeslot number is MP in sir result per CCTRCH, if L1 send timeslot number = 0, RRC will ignore the sir result of the CCTRCH 
                    The design is for future use, becasue TDD only support one CCTRCH present. */
          no_ts_offset ++;
          continue;
        }
        
        qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.elem[i - no_ts_offset].tfcs_ID =
          l1_meas_report_ptr->u.qual_meas_rpt.l1_sir_result[i].tfcs_id;
        
        qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.elem[i - no_ts_offset].sir_TimeslotList.n =
            l1_meas_report_ptr->u.qual_meas_rpt.l1_sir_result[i].no_ts;
        
        ALLOC_ASN1ARRAY1(&tdsenc_ctxt,
          &qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.elem[i - no_ts_offset].sir_TimeslotList,
          tdsrrc_SIR);
        
        if (qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.elem[i - no_ts_offset].sir_TimeslotList.elem == NULL)
        {
          ERR_FATAL("Quality rpt Unable allocate memory",0,0,0); 
        }
        
        for (j = 0; j < l1_meas_report_ptr->u.qual_meas_rpt.l1_sir_result[i].no_ts; j++)
        {
          qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.elem[i - no_ts_offset].sir_TimeslotList.elem[j] =
            l1_meas_report_ptr->u.qual_meas_rpt.l1_sir_result[i].sir[j];
        }
      }
      
      if (no_ts_offset != 0)
      {
        qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.n = 
              qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.n - no_ts_offset;
        
        if (qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->sir_MeasurementResults.n == 0)
        {
          qualityMeasuredResults_ptr->modeSpecificInfo.u.tdd->m.sir_MeasurementResultsPresent = 0;
        }
      }
    }
  
    break;
    
  default: TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid meas rpt criteria");
    break;
  } 
  
  
  /* Append Additional Measured Results if available */
  tdsrrcmeas_process_additional_measured_results(
    &l1_meas_report_ptr->meas_id, TDSQUALITY_MEAS, &msg_ptr->message.u.measurementReport);
  
  if (l1_meas_report_ptr->rpt_crit == TDSL1_PERIODIC_RPT) 
  {
    /* Now check if this is the last report. Meas Id needs to be deleted if true */
    if (l1_meas_report_ptr->last_periodic_report == TRUE)
    {
      /* Delete this Measurement */
      for (meas_id_cnt =0; meas_id_cnt < TDSMAX_QUALITY_MEAS; meas_id_cnt++ )
      {
        if (tdsquality_meas_id_list[meas_id_cnt].meas_id
          == l1_meas_report_ptr->meas_id)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete Meas %d",l1_meas_report_ptr->meas_id);
          tdsquality_meas_id_list[meas_id_cnt].meas_id = TDSINVALID_MEAS_ID;
          
          tdsquality_meas_id_list[meas_id_cnt].rpt_crit = TDSNOT_AVAILABLE;
          
          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsquality_meas_id_list[meas_id_cnt].additional_meas[cnt]
            = TDSINVALID_MEAS_ID;
          
          break;
        }
      }
    }
  }

  /* Closed measurement_result_v4b0ext if malloc memory no use*/
  tdsrrcmeas_close_measurement_result_v4b0ext(msg_ptr);
  
  /* This places SDU into RLC watermark queue */
  status =  tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_MCMR, (void *) msg_ptr,
    ul_lc_id, l2_ack_status);
  
  if ( status != TDSRRCSEND_SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Send chain failed to send QualityMeas Rpt");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"QualityMeas Rpt sent from RRC=>RLC");
    if(l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE)
    {
      tdsrrcmeas_pending_rpt++; 
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"pending meas rpt = %d",tdsrrcmeas_pending_rpt);
    } 
  }
  
  return;
}

/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM_R5

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements for rel 5

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm_r5
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the Quality Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSQUALITY_MEAS;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_qual_meas_params;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  /* local ptr to the additional measurements list */
  tdsrrc_AdditionalMeasurementID_List *additionalMeasurementList_ptr = NULL;
          
  tdsrrc_MeasurementControl_r4_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;  

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.r4.measurementControl_r4;
  
  /* Initialize the l1 structure */
  memset(&l1_qual_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_qual_meas_params.meas_object.meas_type = TDSL1_QUALITY;
  l1_qual_meas_params.trans_id = tdstransaction_id;
  l1_qual_meas_params.meas_id = (uint16)rrc_mcm_ptr->measurementIdentity;
  l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;
  
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))
  {
    l1_qual_meas_params.rpt_mode_incl = TRUE;   
    l1_qual_meas_params.reporting_mode.rpt_trans_mode
      = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode); 
  }

                  
  /* set the Transaction Id and the Meas Id */
  switch(rrc_mcm_ptr->measurementCommand.t)
  {
  case T_tdsrrc_MeasurementCommand_r4_setup:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: setup processing");    
    /* check if the quality reporting quantity is present if not return failure */
    if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      qualityReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Quality Rpt Quan not present");
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }    
    
    if(l1_qual_meas_params.rpt_mode_incl == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:meas Rpt mode mandatory");
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;    
    }
    
    l1_qual_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    l1_qual_meas_params.meas_object_info_included = TRUE;
    
    /*process the reporting quantity IE */
    
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
      rrc_QualityReportCriteria,qualityReportingCriteria))
      &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.dl_TransChBLER == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:BLER rpt set to true for evnt trigg meas");
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }  
    
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.modeSpecificInfo,
        rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:qual rept quantity for fdd chosen");
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    } 
    
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
      rrc_QualityReportCriteria_periodicalReportingCriteria))
    {
      l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE; 
    } 
        
    if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      &l1_qual_meas_params.meas_object.u.qual, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    /* validate the additional measurements*/
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
      && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
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
          addtl_meas_incl, TRUE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
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
      }
    }
    
    /* fill up the measurement identity, validity and rpt criteria in the db */
    /* Store meas in MEASUREMENT_IDENTITY List */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: updating meas identity db");
    for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
      {
        tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsquality_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsquality_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      }
    } /* End of if */
    break;
        
    case T_tdsrrc_MeasurementCommand_r4_modify:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: modify processing"); 
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        & tdsrrc_MeasurementCommand_r4_modify,measurementType))
      {
        l1_qual_meas_params.meas_object_info_included = TRUE;
        
        l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;
        
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement,
          qualityReportingQuantity))
        {
          l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE;
          
          if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->reportCriteria,
            rrc_QualityReportCriteria,qualityReportingCriteria))
            &&
            (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->qualityReportingQuantity.
            dl_TransChBLER == TRUE))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:BLER rpt set to true for evnt trigg meas");
            /* Set faliure t */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }  
          
          if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->
            qualityReportingQuantity.modeSpecificInfo,rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:qual rept quantity for fdd chosen");
            /* Set faliure t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }
        }
        if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.modify->
        measurementType.u.qualityMeasurement,&l1_qual_meas_params.meas_object.u.qual,
        tdstransaction_id, &mcm_rpt_crit))
        {
          return FAILURE;
        }        
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality:Modify:MType absent");
      }
      /* validate the additional measurements*/
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
        && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
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
            addtl_meas_incl, FALSE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
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
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: update meas id db");
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* this meas already exists */
        if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
        {
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      } 
      break;
      
    case T_tdsrrc_MeasurementCommand_r4_release:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: release processing");
      
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
      /* Delete this Measurement */
      for (count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        if (tdsquality_meas_id_list[count].meas_id
          == rrc_mcm_ptr->measurementIdentity)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete Meas %d",rrc_mcm_ptr->measurementIdentity);
          tdsquality_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
          
          tdsquality_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
          
          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsquality_meas_id_list[count].additional_meas[cnt]
            = TDSINVALID_MEAS_ID;
          break;
        }
      }                
      alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
      l1_qual_meas_params.trans_id = alloc_idx;
      tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
      
      return SUCCESS;    
      
    default:
      break;
  }
  
  cnt = 0; 
 
  additionalMeasurementList_ptr = &rrc_mcm_ptr->additionalMeasurementList;
  while((additionalMeasurementList_ptr->n > cnt) && (cnt < TDSMAX_ADDITIONAL_MEAS))
  {
    l1_qual_meas_params.add_meas[cnt] =  (uint16) additionalMeasurementList_ptr->elem[cnt];
    cnt++;
  }
  l1_qual_meas_params.num_add_meas = cnt;
  if (addtl_meas_incl == TRUE)
  {
    tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
      rrc_mcm_ptr->measurementIdentity);

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
  }

  

  /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_qual_meas_params.trans_id = alloc_idx;
  tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}


/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM_R6

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements for rel 6

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm_r6
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the Quality Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSQUALITY_MEAS;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_qual_meas_params;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  /* local ptr to the additional measurements list */
  tdsrrc_AdditionalMeasurementID_List *additionalMeasurementList_ptr = NULL;
          
  tdsrrc_MeasurementControl_r6_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;  

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.r6.measurementControl_r6;
  
  /* Initialize the l1 structure */
  memset(&l1_qual_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));
 
  l1_qual_meas_params.meas_object.meas_type = TDSL1_QUALITY;
  l1_qual_meas_params.trans_id = tdstransaction_id;
  l1_qual_meas_params.meas_id = (uint16)rrc_mcm_ptr->measurementIdentity;
  l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode))
  {
    l1_qual_meas_params.rpt_mode_incl = TRUE;   
    l1_qual_meas_params.reporting_mode.rpt_trans_mode
      = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode); 
  }

                  
  /* set the Transaction Id and the Meas Id */
  switch(rrc_mcm_ptr->measurementCommand.t)
  {
  case T_tdsrrc_MeasurementCommand_r6_setup:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: setup processing");    
    /* check if the quality reporting quantity is present if not return failure */
    if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
        qualityReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Quality Rpt Quan not present");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }    
    
    if(l1_qual_meas_params.rpt_mode_incl == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:meas Rpt mode mandatory");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;    
    }
    
    l1_qual_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    l1_qual_meas_params.meas_object_info_included = TRUE;
    
    /*process the reporting quantity IE */
    
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
       rrc_QualityReportCriteria,qualityReportingCriteria))
      &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.dl_TransChBLER == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:BLER rpt set to true for evnt trigg meas");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }  
    
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.modeSpecificInfo,
      rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:qual rept quantity for fdd chosen");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    } 

    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
       rrc_QualityReportCriteria_periodicalReportingCriteria))
    {
      l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE; 
    } 
        
    if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      &l1_qual_meas_params.meas_object.u.qual, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    /* validate the additional measurements*/
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
      && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
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
          addtl_meas_incl, TRUE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
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
      }
    }
    
    /* fill up the measurement identity, validity and rpt criteria in the db */
    /* Store meas in MEASUREMENT_IDENTITY List */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: updating meas identity db");
    for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
      {
        tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsquality_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsquality_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      }
    } /* End of if */
    break;
        
    case T_tdsrrc_MeasurementCommand_r6_modify:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: modify processing"); 
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        & tdsrrc_MeasurementCommand_r6_modify,measurementType))
      {
        l1_qual_meas_params.meas_object_info_included = TRUE;
        
        l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;
        
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement,
          qualityReportingQuantity))
        {
          l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE;
          
          if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->reportCriteria,
             rrc_QualityReportCriteria,qualityReportingCriteria))
            &&
            (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->qualityReportingQuantity.
            dl_TransChBLER == TRUE))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:BLER rpt set to true for evnt trigg meas");
            /* Set faliure t */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }  
          
          if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->
            qualityReportingQuantity.modeSpecificInfo,rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:qual rept quantity for fdd chosen");
            /* Set faliure t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }
        }
        if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.modify->
        measurementType.u.qualityMeasurement,&l1_qual_meas_params.meas_object.u.qual,
        tdstransaction_id, &mcm_rpt_crit))
        {
          return FAILURE;
        }        
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality:Modify:MType absent");
      }
      /* validate the additional measurements*/
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
        && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
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
            addtl_meas_incl, FALSE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
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
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: update meas id db");
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* this meas already exists */
        if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
        {
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      } 
      break;
      
    case T_tdsrrc_MeasurementCommand_r6_release:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: release processing");
      
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
      /* Delete this Measurement */
      for (count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        if (tdsquality_meas_id_list[count].meas_id
          == rrc_mcm_ptr->measurementIdentity)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete Meas %d",rrc_mcm_ptr->measurementIdentity);
          tdsquality_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
          
          tdsquality_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
          
          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsquality_meas_id_list[count].additional_meas[cnt]
            = TDSINVALID_MEAS_ID;
          break;
        }
      }                
      alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
      l1_qual_meas_params.trans_id = alloc_idx;
      tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
      
      return SUCCESS;    
      
    default:
      break;
  }
  
  cnt = 0; 
 
  additionalMeasurementList_ptr = &rrc_mcm_ptr->additionalMeasurementList;
  while((additionalMeasurementList_ptr->n > cnt) && (cnt < TDSMAX_ADDITIONAL_MEAS))
  {
    l1_qual_meas_params.add_meas[cnt] =  (uint16) additionalMeasurementList_ptr->elem[cnt];
    cnt++;
  }
  l1_qual_meas_params.num_add_meas = cnt;
  if (addtl_meas_incl == TRUE)
  {
    tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
      rrc_mcm_ptr->measurementIdentity);

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
  }

  

  /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_qual_meas_params.trans_id = alloc_idx;
  tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}


/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM_R7

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements for rel 7

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm_r7
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the Quality Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSQUALITY_MEAS;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_qual_meas_params;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  /* local ptr to the additional measurements list */
  tdsrrc_AdditionalMeasurementID_List *additionalMeasurementList_ptr = NULL;
          
  tdsrrc_MeasurementControl_r7_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;  

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.r7.measurementControl_r7;
  
  /* Initialize the l1 structure */
  memset(&l1_qual_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_qual_meas_params.meas_object.meas_type = TDSL1_QUALITY;
  l1_qual_meas_params.trans_id = tdstransaction_id;
  l1_qual_meas_params.meas_id = (uint16)rrc_mcm_ptr->measurementIdentity;
  l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode))
  {
    l1_qual_meas_params.rpt_mode_incl = TRUE;   
    l1_qual_meas_params.reporting_mode.rpt_trans_mode
      = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode); 
  }

                  
  /* set the Transaction Id and the Meas Id */
  switch(rrc_mcm_ptr->measurementCommand.t)
  {
  case T_tdsrrc_MeasurementCommand_r7_setup:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: setup processing");    
    /* check if the quality reporting quantity is present if not return failure */
    if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      qualityReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Quality Rpt Quan not present");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }    
    
    if(l1_qual_meas_params.rpt_mode_incl == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:meas Rpt mode mandatory");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;    
    }
    
    l1_qual_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    l1_qual_meas_params.meas_object_info_included = TRUE;
    
    /*process the reporting quantity IE */
    
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
       rrc_QualityReportCriteria,qualityReportingCriteria))
      &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.dl_TransChBLER == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:BLER rpt set to true for evnt trigg meas");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }  
    
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.
        qualityMeasurement->qualityReportingQuantity.modeSpecificInfo,
        rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:qual rept quantity for fdd chosen");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    } 

    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
       rrc_QualityReportCriteria_periodicalReportingCriteria))
    {
      l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE; 
    } 
        
    if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      &l1_qual_meas_params.meas_object.u.qual, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    /* validate the additional measurements*/
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
      && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
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
          addtl_meas_incl, TRUE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
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
      }
    }
    
    /* fill up the measurement identity, validity and rpt criteria in the db */
    /* Store meas in MEASUREMENT_IDENTITY List */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: updating meas identity db");
    for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
      {
        tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsquality_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsquality_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      }
    } /* End of if */
    break;
        
    case T_tdsrrc_MeasurementCommand_r7_modify:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: modify processing"); 
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        & tdsrrc_MeasurementCommand_r7_modify,measurementType))
      {
        l1_qual_meas_params.meas_object_info_included = TRUE;
        
        l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;
        
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement,
          qualityReportingQuantity))
        {
          l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE;
          
          if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->reportCriteria,
             rrc_QualityReportCriteria,qualityReportingCriteria))
            &&
            (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->qualityReportingQuantity.
            dl_TransChBLER == TRUE))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:BLER rpt set to true for evnt trigg meas");
            /* Set faliure t */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }  
          
          if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->
            qualityReportingQuantity.modeSpecificInfo,
            rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:qual rept quantity for fdd chosen");
            /* Set faliure t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }
        }
        if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.modify->
        measurementType.u.qualityMeasurement,&l1_qual_meas_params.meas_object.u.qual,
        tdstransaction_id, &mcm_rpt_crit))
        {
          return FAILURE;
        }        
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality:Modify:MType absent");
      }
      /* validate the additional measurements*/
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
        && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
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
            addtl_meas_incl, FALSE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
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
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: update meas id db");
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* this meas already exists */
        if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
        {
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      } 
      break;
      
    case T_tdsrrc_MeasurementCommand_r7_release:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quality meas: release processing");
      
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
      /* Delete this Measurement */
      for (count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        if (tdsquality_meas_id_list[count].meas_id
          == rrc_mcm_ptr->measurementIdentity)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete Meas %d",rrc_mcm_ptr->measurementIdentity);
          tdsquality_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
          
          tdsquality_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
          
          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsquality_meas_id_list[count].additional_meas[cnt]
            = TDSINVALID_MEAS_ID;
          break;
        }
      }                
      
      alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
      l1_qual_meas_params.trans_id = alloc_idx;
      tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
      
      return SUCCESS;    
      
    default:
      break;
  }
  
  cnt = 0; 
 
  additionalMeasurementList_ptr = &rrc_mcm_ptr->additionalMeasurementList;
  while((additionalMeasurementList_ptr->n > cnt) && (cnt < TDSMAX_ADDITIONAL_MEAS))
  {
    l1_qual_meas_params.add_meas[cnt] =  (uint16) additionalMeasurementList_ptr->elem[cnt];
    cnt++;
  }
  l1_qual_meas_params.num_add_meas = cnt;
  if (addtl_meas_incl == TRUE)
  {
    tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
      rrc_mcm_ptr->measurementIdentity);

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
  }

  

  /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_qual_meas_params.trans_id = alloc_idx;

  tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}


#ifdef FEATURE_TDSCDMA_REL8
/*==========================================================================
 FUNCTION   tdsrrcqm_process_mcm_r8

DESCRIPTION
  This function processes the Measurement Control Message for
  Quality Measurements for rel 8

DEPENDENCIES
   None.

RETURN VALUE
    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS
    None
===========================================================================*/
uecomdef_status_e_type tdsrrcqm_process_mcm_r8
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the Quality Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSQUALITY_MEAS;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_qual_meas_params;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  /* local ptr to the additional measurements list */
  tdsrrc_AdditionalMeasurementID_List *additionalMeasurementList_ptr = NULL;
          
  tdsrrc_MeasurementControl_r8_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;  

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.measurementControl_r8;
  
  /* Initialize the l1 structure */
  memset(&l1_qual_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_qual_meas_params.meas_object.meas_type = TDSL1_QUALITY;
  l1_qual_meas_params.trans_id = tdstransaction_id;
  l1_qual_meas_params.meas_id = (uint16)rrc_mcm_ptr->measurementIdentity;
  l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode))
  {
    l1_qual_meas_params.rpt_mode_incl = TRUE;   
    l1_qual_meas_params.reporting_mode.rpt_trans_mode
      = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode); 
  }

  /* set the Transaction Id and the Meas Id */
  switch(rrc_mcm_ptr->measurementCommand.t)
  {
  case T_tdsrrc_MeasurementCommand_r8_setup:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: setup processing");    
    /* check if the quality reporting quantity is present if not return failure */
    if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      qualityReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:SETUP:Quality Rpt Quan not present");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }    
    
    if(l1_qual_meas_params.rpt_mode_incl == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:SETUP:meas Rpt mode mandatory");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:QUALITY:Sending meas ctrl failure message");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;    
    }
    
    l1_qual_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    l1_qual_meas_params.meas_object_info_included = TRUE;
    
    /*process the reporting quantity IE */
    
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
      rrc_QualityReportCriteria,qualityReportingCriteria))
      &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.dl_TransChBLER == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:SETUP:BLER rpt set to true for evnt trigg meas");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }  
    
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.modeSpecificInfo,
       rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:SETUP:qual rept quantity for fdd chosen");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }

    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
        rrc_QualityReportCriteria_periodicalReportingCriteria))
    {
      l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE; 
    } 
        
    if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      &l1_qual_meas_params.meas_object.u.qual, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    /* validate the additional measurements*/
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
      && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);    
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
          addtl_meas_incl, TRUE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
          &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Sending Meas Ctrl Failure");
          
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
          
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
          
          /* Send MCF msg to UTRAN */  
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          return FAILURE;
        }
      }
    }
    
    /* fill up the measurement identity, validity and rpt criteria in the db */
    /* Store meas in MEASUREMENT_IDENTITY List */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: updating meas identity db");
    for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
      {
        tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsquality_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsquality_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      }
    } /* End of if */
    break;
        
    case T_tdsrrc_MeasurementCommand_r8_modify:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: modify processing"); 
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        & tdsrrc_MeasurementCommand_r8_modify,measurementType))
      {
        l1_qual_meas_params.meas_object_info_included = TRUE;
        
        l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;
        
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement,
          qualityReportingQuantity))
        {
          l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE;
          
          if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->reportCriteria,
            rrc_QualityReportCriteria,qualityReportingCriteria))
            &&
            (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->qualityReportingQuantity.
            dl_TransChBLER == TRUE))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Modify:BLER rpt set to true for evnt trigg meas");
            /* Set faliure t */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }  
          
          if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->
            qualityReportingQuantity.modeSpecificInfo,
             rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Modify:qual rept quantity for fdd chosen");
            /* Set faliure t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }
        }
        if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.modify->
        measurementType.u.qualityMeasurement,&l1_qual_meas_params.meas_object.u.qual,
        tdstransaction_id, &mcm_rpt_crit))
        {
          return FAILURE;
        }        
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality:Modify:MType absent");
      }
      /* validate the additional measurements*/
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
        && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);   
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
            addtl_meas_incl, FALSE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Sending Meas Ctrl Failure");
            
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
            
            /* Send MCF msg to UTRAN */  
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            
            return FAILURE;
          }
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: update meas id db");
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* this meas already exists */
        if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
        {
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      } 
      break;
      
    case T_tdsrrc_MeasurementCommand_r8_release:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: release processing");
      
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
      /* Delete this Measurement */
      for (count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        if (tdsquality_meas_id_list[count].meas_id
          == rrc_mcm_ptr->measurementIdentity)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:Delete Meas %d",rrc_mcm_ptr->measurementIdentity);
          tdsquality_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
          
          tdsquality_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
          
          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsquality_meas_id_list[count].additional_meas[cnt]
            = TDSINVALID_MEAS_ID;
          break;
        }
      }                
      
      alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
      l1_qual_meas_params.trans_id = alloc_idx;
      tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
      
      return SUCCESS;    
      
    default:
      break;
  }
  
  cnt = 0; 
 
  additionalMeasurementList_ptr = &rrc_mcm_ptr->additionalMeasurementList;
  while((additionalMeasurementList_ptr->n > cnt) && (cnt < TDSMAX_ADDITIONAL_MEAS))
  {
    l1_qual_meas_params.add_meas[cnt] =  (uint16) additionalMeasurementList_ptr->elem[cnt];
    cnt++;
  }
  l1_qual_meas_params.num_add_meas = cnt;
  if (addtl_meas_incl == TRUE)
  {
    tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
      rrc_mcm_ptr->measurementIdentity);

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Additional Meas set in RRC DB");
  }

   /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_qual_meas_params.trans_id = alloc_idx;

  tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
/*==========================================================================
 FUNCTION   tdsrrcqm_process_mcm_r9

DESCRIPTION
  This function processes the Measurement Control Message for
  Quality Measurements for rel 9

DEPENDENCIES
   None.

RETURN VALUE
    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS
    None
===========================================================================*/
uecomdef_status_e_type tdsrrcqm_process_mcm_r9
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the Quality Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  /* Declare MCM config structure that goes to L1 */
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;
  
  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;
  
  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSQUALITY_MEAS;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_qual_meas_params;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  /* local ptr to the additional measurements list */
  tdsrrc_AdditionalMeasurementID_List *additionalMeasurementList_ptr = NULL;
          
  tdsrrc_MeasurementControl_r9_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;  

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.measurementControl_r9;
  
  /* Initialize the l1 structure */
  memset(&l1_qual_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_qual_meas_params.meas_object.meas_type = TDSL1_QUALITY;
  l1_qual_meas_params.trans_id = tdstransaction_id;
  l1_qual_meas_params.meas_id = (uint16)rrc_mcm_ptr->measurementIdentity;
  l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,measurementReportingMode))
  {
    l1_qual_meas_params.rpt_mode_incl = TRUE;   
    l1_qual_meas_params.reporting_mode.rpt_trans_mode
      = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode); 
  }

  /* set the Transaction Id and the Meas Id */
  switch(rrc_mcm_ptr->measurementCommand.t)
  {
  case T_tdsrrc_MeasurementCommand_r9_setup:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: setup processing");    
    /* check if the quality reporting quantity is present if not return failure */
    if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      qualityReportingQuantity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:SETUP:Quality Rpt Quan not present");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }    
    
    if(l1_qual_meas_params.rpt_mode_incl == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:SETUP:meas Rpt mode mandatory");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:QUALITY:Sending meas ctrl failure message");
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;    
    }
    
    l1_qual_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
    l1_qual_meas_params.meas_object_info_included = TRUE;
    
    /*process the reporting quantity IE */
    
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
      rrc_QualityReportCriteria,qualityReportingCriteria))
      &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.dl_TransChBLER == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:SETUP:BLER rpt set to true for evnt trigg meas");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }  
    
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->qualityReportingQuantity.modeSpecificInfo,
       rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:SETUP:qual rept quantity for fdd chosen");
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:QUALITY:Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE; 
    }

    if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement->reportCriteria,
        rrc_QualityReportCriteria_periodicalReportingCriteria))
    {
      l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE; 
    } 
        
    if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.setup->u.qualityMeasurement,
      &l1_qual_meas_params.meas_object.u.qual, tdstransaction_id, &mcm_rpt_crit))
    {
      return FAILURE;
    }

    /* validate the additional measurements*/
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
      && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);    
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
          addtl_meas_incl, TRUE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
          &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Sending Meas Ctrl Failure");
          
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
          
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
          
          /* Send MCF msg to UTRAN */  
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          return FAILURE;
        }
      }
    }
    
    /* fill up the measurement identity, validity and rpt criteria in the db */
    /* Store meas in MEASUREMENT_IDENTITY List */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Quality meas: updating meas identity db");
    for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
      {
        tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
        meas_exists_for_setup = TRUE;
        break;
      }
    }
    
    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* Find first empty space */
        if (tdsquality_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsquality_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      }
    } /* End of if */
    break;
        
    case T_tdsrrc_MeasurementCommand_r9_modify:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: modify processing"); 
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        & tdsrrc_MeasurementCommand_r9_modify,measurementType))
      {
        l1_qual_meas_params.meas_object_info_included = TRUE;
        
        l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = FALSE;
        
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement,
          qualityReportingQuantity))
        {
          l1_qual_meas_params.meas_object.u.qual.rpt_quan_included = TRUE;
          
          if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->reportCriteria,
            rrc_QualityReportCriteria,qualityReportingCriteria))
            &&
            (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->qualityReportingQuantity.
            dl_TransChBLER == TRUE))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Modify:BLER rpt set to true for evnt trigg meas");
            /* Set faliure t */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }  
          
          if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.qualityMeasurement->
            qualityReportingQuantity.modeSpecificInfo,
             rrc_QualityReportingQuantity_modeSpecificInfo_fdd))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Modify:qual rept quantity for fdd chosen");
            /* Set faliure t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:QUALITY:Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            return FAILURE; 
          }
        }
        if(FAILURE == tdsrrcqm_fill_mcm_info(rrc_mcm_ptr->measurementCommand.u.modify->
        measurementType.u.qualityMeasurement,&l1_qual_meas_params.meas_object.u.qual,
        tdstransaction_id, &mcm_rpt_crit))
        {
          return FAILURE;
        }        
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Quality:Modify:MType absent");
      }
      /* validate the additional measurements*/
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
        && (rrc_mcm_ptr->additionalMeasurementList.n != 0)) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);   
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
            addtl_meas_incl, FALSE, TDSQUALITY_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Sending Meas Ctrl Failure");
            
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
            
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
            
            /* Send MCF msg to UTRAN */  
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
            
            return FAILURE;
          }
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: update meas id db");
      for ( count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        /* this meas already exists */
        if (tdsquality_meas_id_list[count].meas_id == l1_qual_meas_params.meas_id)
        {
          tdsquality_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsquality_meas_id_list[count].meas_validity = TDSSTATE_DCH;
          break;
        }
      } 
      break;
      
    case T_tdsrrc_MeasurementCommand_r9_release:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Quality meas: release processing");
      
      l1_qual_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;
      /* Delete this Measurement */
      for (count = 0; count < TDSMAX_QUALITY_MEAS; count++ )
      {
        if (tdsquality_meas_id_list[count].meas_id
          == rrc_mcm_ptr->measurementIdentity)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:Delete Meas %d",rrc_mcm_ptr->measurementIdentity);
          tdsquality_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
          
          tdsquality_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
          
          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsquality_meas_id_list[count].additional_meas[cnt]
            = TDSINVALID_MEAS_ID;
          break;
        }
      }                
      
      alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
      l1_qual_meas_params.trans_id = alloc_idx;
      tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
      
      return SUCCESS;    
      
    default:
      break;
  }
  
  cnt = 0; 
 
  additionalMeasurementList_ptr = &rrc_mcm_ptr->additionalMeasurementList;
  while((additionalMeasurementList_ptr->n > cnt) && (cnt < TDSMAX_ADDITIONAL_MEAS))
  {
    l1_qual_meas_params.add_meas[cnt] =  (uint16) additionalMeasurementList_ptr->elem[cnt];
    cnt++;
  }
  l1_qual_meas_params.num_add_meas = cnt;
  if (addtl_meas_incl == TRUE)
  {
    tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
      rrc_mcm_ptr->measurementIdentity);

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Additional Meas set in RRC DB");
  }

   /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */
  
#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
  if(TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO)
  {
    if(TRUE == rrc_mcm_ptr->m.cellDCHMeasOccasionInfo_TDD128Present)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: cellDCHMeasOccasionInfo_TDD128 IE present and supported!");

      switch(tdsrrcllc_get_dmo_r9(&rrc_mcm_ptr->cellDCHMeasOccasionInfo_TDD128,
                            &l1_qual_meas_params.meas_occasion_info))
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

      l1_qual_meas_params.meas_occasion_info_incl = TRUE;
      /* save in CC, in order to revert old config, send to L1 old config */
      tdscurrent_config_ptr->meas_occasion_info_incl = TRUE;

      memscpy(&tdscurrent_config_ptr->meas_occasion_info,
	      sizeof(tdsl1_meas_occasion_info_struct_type),
             &l1_qual_meas_params.meas_occasion_info,
             sizeof(tdsl1_meas_occasion_info_struct_type));
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: cellDCHMeasOccasionInfo_TDD128 IE is not present!");
      l1_qual_meas_params.meas_occasion_info_incl = FALSE;
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
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_qual_meas_params.trans_id = alloc_idx;

  tdsrrcmeas_send_l1_cmd(NULL, &l1_qual_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  
  return SUCCESS;
}
#endif /*FEATURE_TDSCDMA_REL9 */


