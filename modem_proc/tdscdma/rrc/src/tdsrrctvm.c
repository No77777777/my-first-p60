/*===========================================================================
                   RRC TRAFFIC VOLUME MEASUREMENTS  

DESCRIPTION

      This module supports traffic Volume Measurements from the UE to UTRAN.
      
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrctvm.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
03/10/11   zwj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/17/11   zwj    3GPP-10.3.7.70. Added TVM on E-DCH info for HSUPA.  
11/23/10   zwj    Modify meas report for TDD.

===========================================================================*/



/*==========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "customer.h"
#include "tdsrrcsend.h"
#include "tdsrrclcm.h"
#include "tdsuecomdef.h"
#include "tdsrrcscmgr.h"
#include "msg.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcmeasi.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcmeas.h"
#include "tdsrrcasn1util.h"
#include "tdsmactraffic.h"
#include "tdsrrcllc.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif

/* -----------------------------------------------------------------------
** Extern Declarations
** ----------------------------------------------------------------------- */

extern tdssib12_present_e_type tdssib12_present;
extern uint8 tdsrrcmeas_pending_rpt;


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Default Traffic Vol Meas Identity  */

#define TDSINVALID_PAYLOAD 0xffffffff



/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Global Object Definition
** ----------------------------------------------------------------------- */
// Stores SIB params
tdsmac_ul_traffic_meas_config_type  tdsmac_sib_params;


/*-------------------------------------------------------------------
STRUCTURE: tdstvm_meas_id_list_struct_type
Stores the traffic volume Meas Id's
--------------------------------------------------------------------*/
tdstvm_meas_id_list_struct_type     tdstvm_meas_id_list[TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS]; 


/*-------------------------------------------------------------------
ENUM: tdsrrcmeas_current_substate
Stores substate of RRC Measurement procedure
--------------------------------------------------------------------*/
extern tdsrrcmeas_substate_e_type tdsrrcmeas_current_substate; 


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */

/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION     RRCTVM_PROCESS_SIB_INFO

DESCRIPTION  

    Process traffic volume information from SIB11/12 
                    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_sib_info
(
tdsrrc_MeasurementControlSysInfo* sib_meas_ptr
)
{
  /* SIB params stored in MAC structure  tdsmac_sib_params */  
  tdsrrc_TrafficVolumeMeasurementObjectList * mac_obj_ptr = NULL;
  
  /* No of transport channels */
  uint8 transport_channel_count = 0;
  
  uint8 count = 0;
  
  uint8 cnt = 0;
  boolean block_set = FALSE;
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_meas_ptr,trafficVolumeMeasSysInfo))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sys Info present");
    

      TDSRRC_MSG0(MSG_LEGACY_LOW,"Meas Identity present");
      
      /* Set Meas Identity in traffic volume meas identity variable */
      tdsmac_sib_params.meas_id = (uint16) 
        sib_meas_ptr->trafficVolumeMeasSysInfo.trafficVolumeMeasurementID;
    
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas Id is %d",tdsmac_sib_params.meas_id);
    /* Set Meas Id and Validity */
    tdsrrctvm_set_meas_identity(&tdsmac_sib_params.meas_id, &sib_meas_ptr->trafficVolumeMeasSysInfo); 
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE(sib_meas_ptr->trafficVolumeMeasSysInfo,
        tdsrrc_TrafficVolumeMeasSysInfo,measurementValidity))
    {
      
      if (((sib_meas_ptr->trafficVolumeMeasSysInfo.measurementValidity.ue_State == tdsrrc_MeasurementValidity_ue_State_cell_DCH) 
           && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
        ((sib_meas_ptr->trafficVolumeMeasSysInfo.measurementValidity.ue_State == tdsrrc_MeasurementValidity_ue_State_all_But_Cell_DCH) 
         && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: RRC val,MCM val differ");
        tdsmac_sib_params.meas_start_incl = FALSE;
      }
    }
    
    
  } /* End of if of sib12 for traffic vol info present */
  
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:No Sys Info present");
    return TDSTVM_FAIL;
  }   
  
  if (TDSRRC_MSG_COMMON_BITMASK_IE(sib_meas_ptr->trafficVolumeMeasSysInfo,tdsrrc_TrafficVolumeMeasSysInfo,trafficVolumeMeasurementObjectList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Meas Object present");
    
    mac_obj_ptr = 
      &sib_meas_ptr->trafficVolumeMeasSysInfo.trafficVolumeMeasurementObjectList;
    
    tdsmac_sib_params.meas_obj_incl = TRUE;
    
    while ((mac_obj_ptr->n > cnt) && (transport_channel_count < TDSUE_MAX_TRCH))
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(mac_obj_ptr->elem[cnt],rrc_UL_TrCH_Identity_usch))
      {
#ifdef FEATURE_TDSCDMA_HSUPA
        if (mac_obj_ptr->elem[cnt].u.usch == 32) 
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:E-DCH object %d specified",mac_obj_ptr->elem[cnt].u.usch);
          tdsmac_sib_params.meas_obj_list.trch_info[transport_channel_count].trch_type = TDSEDCH;

          /* Get Channel ID too */
          tdsmac_sib_params.meas_obj_list.trch_info[transport_channel_count].trch_id = (uint16)
             mac_obj_ptr->elem[cnt].u.usch; 
          /* Increment count */
          transport_channel_count++;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Support E-DCH but USCH object given in SIB. Ignoring"); 
        }
#else
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupport HSUPA but USCH object given. Ignoring"); 
#endif
      }
      else if (mac_obj_ptr->elem[cnt].t ==  T_tdsrrc_UL_TrCH_Identity_rachorcpch)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:RACH object specified");
        /* Set channel type */
        
        tdsmac_sib_params.meas_obj_list.trch_info[transport_channel_count].trch_type = TDSRACH;
        
        /* Increment count */
        transport_channel_count++;
        
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:DCH object %d specified",mac_obj_ptr->elem[cnt].u.dch);
        
        /* Set channel type */
        tdsmac_sib_params.meas_obj_list.trch_info[transport_channel_count].
          trch_type = TDSDCH;
        
        /* Get Channel ID too */
        tdsmac_sib_params.meas_obj_list.trch_info[transport_channel_count].trch_id = (uint16)
          mac_obj_ptr->elem[cnt].u.dch; 
        
        transport_channel_count++;
      }
      cnt++;
      if ((mac_obj_ptr != NULL) && (transport_channel_count == TDSUE_MAX_TRCH))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:All transport Ch not included");
        break;
      }
      continue;
      
    }   /* End of while */
    
    tdsmac_sib_params.meas_obj_list.num_trch = transport_channel_count; 

    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas Obj No of TransCh %d",transport_channel_count);
    
  } /* End of meas object present */

  
  /* TDD comment:
         MAC only consider meas_obj_incl = FALSE represent measurement all exiting transport channel. 
         If RRC config meas_obj_incl = TRUE and meas_obj_list.num_trch = 0 ,
         MAC will not measurement traffic volume, so add handle to set meas_obj_incl = FALSE.
  */
  if (tdsmac_sib_params.meas_obj_list.num_trch == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:No Of TrCh 0,RepCrit ALL");
    tdsmac_sib_params.meas_obj_incl = FALSE;
  }
  
  /* Check if Meas Quantity present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(sib_meas_ptr->trafficVolumeMeasSysInfo,
    tdsrrc_TrafficVolumeMeasSysInfo,trafficVolumeMeasQuantity))
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TVM:Meas Quan %d RLC/A/V:0/1/2", sib_meas_ptr->trafficVolumeMeasSysInfo.
      trafficVolumeMeasQuantity.t,0,0);
    
    tdsmac_sib_params.meas_qty_incl = TRUE;
    
    if (TDSRRC_CHECK_COMMON_MSG_TYPE2(sib_meas_ptr->trafficVolumeMeasSysInfo.trafficVolumeMeasQuantity,
        rrc_TrafficVolumeMeasQuantity,rlc_BufferPayload))
    {
      tdsmac_sib_params.meas_qty.meas_qty_enum = TDSRLC_BUF_PAYLOAD;
      tdsmac_sib_params.meas_qty.meas_interval = TDSQUANTITY_NOT_PRESENT;
    }
    else if (TDSRRC_CHECK_COMMON_MSG_TYPE2(sib_meas_ptr->trafficVolumeMeasSysInfo.
      trafficVolumeMeasQuantity,rrc_TrafficVolumeMeasQuantity,averageRLC_BufferPayload))
    {
      tdsmac_sib_params.meas_qty.meas_qty_enum = TDSAVERAGE_RLC_BUF_PAYLOAD;
      /* Put time interval too */
      tdsmac_sib_params.meas_qty.meas_interval = 
        sib_meas_ptr->trafficVolumeMeasSysInfo.trafficVolumeMeasQuantity.
        u.averageRLC_BufferPayload;
      tdsmac_sib_params.meas_qty.meas_interval = tdsmac_sib_params.meas_qty.meas_interval * TDSMEAS_QUANTITY_INTERVAL_OFFSET;
    }
    else
    {
      tdsmac_sib_params.meas_qty.meas_qty_enum = TDSVARIANCE_RLC_BUF_PAYLOAD;
      /* Put time interval too */
      tdsmac_sib_params.meas_qty.meas_interval = sib_meas_ptr->trafficVolumeMeasSysInfo.
        trafficVolumeMeasQuantity.u.varianceOfRLC_BufferPayload;
      tdsmac_sib_params.meas_qty.meas_interval = tdsmac_sib_params.meas_qty.meas_interval * TDSMEAS_QUANTITY_INTERVAL_OFFSET;
    } 
    
    
  } /* End of if of Meas Quantity present */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Meas Quantity absent");
    return TDSTVM_FAIL;
    /* Do not send command to MAC */
  }
  
  /* Check if Reporting Quantity present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(sib_meas_ptr->trafficVolumeMeasSysInfo,
    tdsrrc_TrafficVolumeMeasSysInfo,trafficVolumeReportingQuantity))
  {
    tdsmac_sib_params.meas_report_incl = TRUE;
    
    if (sib_meas_ptr->trafficVolumeMeasSysInfo.
      trafficVolumeReportingQuantity.rlc_RB_BufferPayload == TRUE)
    {
      tdsmac_sib_params.meas_report_qty.rlc_payload = TRUE;
    }
    if (sib_meas_ptr->trafficVolumeMeasSysInfo.
      trafficVolumeReportingQuantity.rlc_RB_BufferPayloadAverage == TRUE)
    {
      tdsmac_sib_params.meas_report_qty.avg_payload = TRUE;
    }
    if (sib_meas_ptr->trafficVolumeMeasSysInfo.
      trafficVolumeReportingQuantity.rlc_RB_BufferPayloadVariance == TRUE)
    {
      tdsmac_sib_params.meas_report_qty.var_payload = TRUE;
    }
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TVM:Rpting Quan RLC%d A%d V%d", tdsmac_sib_params.meas_report_qty.rlc_payload,
      tdsmac_sib_params.meas_report_qty.avg_payload, 
      tdsmac_sib_params.meas_report_qty.var_payload);
  }   /* End of reporting quantity present */
  
  /* To read traffic volume reporting mode */
  tdsmac_sib_params.meas_report_mode.transfer_mode = 
    TDSTVM_RET_REPORTING_MODE(sib_meas_ptr->trafficVolumeMeasSysInfo.
    measurementReportingMode.measurementReportTransferMode);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Rpting Mode %d ACK:0",tdsmac_sib_params.meas_report_mode.transfer_mode);
  
  
  /* ReportingCriteria */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(sib_meas_ptr->trafficVolumeMeasSysInfo.
    reportCriteriaSysInf,
    rrc_TrafficVolumeReportCriteriaSysInfo_trafficVolumeReportingCriteria))
  {
    if ( (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_meas_ptr->trafficVolumeMeasSysInfo.reportCriteriaSysInf.u.trafficVolumeReportingCriteria,
      transChCriteriaList)) &&
      (sib_meas_ptr->trafficVolumeMeasSysInfo.
      reportCriteriaSysInf.u.trafficVolumeReportingCriteria->transChCriteriaList.n == 0))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Empty event crit structure");
      return TDSTVM_FAIL;
    }
    
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Event triggered Mode selected");
    
    
    if ((tdsrrctvm_process_event_criteria(&sib_meas_ptr->trafficVolumeMeasSysInfo.reportCriteriaSysInf.u.
        trafficVolumeReportingCriteria->transChCriteriaList,
        &tdsmac_sib_params.meas_report_criteria.event_cfg)) == TDSTVM_FAIL) 
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Meas Req to MAC not sent");
      return TDSTVM_FAIL;
    }
    
    /* Now set to Event Triggered Mode */
    tdsmac_sib_params.meas_report_mode.report_mode = TDSEVENT_TRIGGERED_MODE;
    
    /* Set meas_criteria to TRUE */
    tdsmac_sib_params.meas_criteria_incl = TRUE;
    
    /* Check if Block intption on RACH present */
    for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
    {
      if (tdstvm_meas_id_list[count].meas_id == 
        tdsmac_sib_params.meas_id)
      {
        if (tdsmac_sib_params.meas_report_mode.report_mode == 
          TDSEVENT_TRIGGERED_MODE) 
          
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"trch_info_incl = %d",tdsmac_sib_params.meas_report_criteria.event_cfg.trch_info_incl);
          if (tdsmac_sib_params.meas_report_criteria.event_cfg.trch_info_incl == TRUE)
          {
            for (transport_channel_count = 0; transport_channel_count < tdsmac_sib_params.meas_report_criteria.event_cfg.num_trch; transport_channel_count++)
            {
              for (cnt = 0; cnt < tdsmac_sib_params.meas_report_criteria.event_cfg.trch_event_cfg[transport_channel_count].num_events; cnt++)
              {
                if ((tdsmac_sib_params.meas_report_criteria.event_cfg.trch_event_cfg[transport_channel_count].event_param[cnt].event_id == TDSEVENT_4A) &&
                  (tdsmac_sib_params.meas_report_criteria.event_cfg.trch_event_cfg[transport_channel_count].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
                {
                  tdstvm_meas_id_list[count].block_rach_incl = TRUE;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
                  block_set = TRUE;
                  break; 
                } /* End of if of event_id and tx_interrupt_tmr check */
              } /* End of for */
              if (block_set == TRUE)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
                break;
              }
            } /* end of 2nd for */
          }
          else
          {
            /* Keep up with MAC, if trch_info_incl = FALSE, only get event info from first element */
            for (cnt = 0; cnt < tdsmac_sib_params.meas_report_criteria.event_cfg.trch_event_cfg[0].num_events; cnt++)
            {
              if ((tdsmac_sib_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].event_id == TDSEVENT_4A) &&
                (tdsmac_sib_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
              {
                tdstvm_meas_id_list[count].block_rach_incl = TRUE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
                
                block_set = TRUE;
                break; 
              } /* End of if of event_id and tx_interrupt_tmr check */
            } /* End of for */
            if (block_set == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
              break;
            }
          }
          break;
        } /* End of if for Event Triggered Mode */
        break;
      } /* End of if */
      
    } /* End of for loop */ 
    
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Periodic Mode selected");
    
    /* Set meas_criteria to TRUE */
    tdsmac_sib_params.meas_criteria_incl = TRUE;
    
    tdsmac_sib_params.meas_report_mode.report_mode = TDSPERIODIC_MODE;
    
    /* Set default no of reports. TRUE indicates infinity */
    tdsmac_sib_params.meas_report_criteria.periodic_cfg.report_infinity = TRUE;
    
    /* Get periodic information */
   
    tdsmac_sib_params.meas_report_criteria.periodic_cfg.num_reports = 
      TDSTVM_RET_REPORTING_AMOUNT(sib_meas_ptr->
      trafficVolumeMeasSysInfo.reportCriteriaSysInf.
      u.periodicalReportingCriteria->reportingAmount);
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Rpting Amt is %d",tdsmac_sib_params.meas_report_criteria.periodic_cfg.num_reports);
    
    if (sib_meas_ptr->trafficVolumeMeasSysInfo.reportCriteriaSysInf.
      u.periodicalReportingCriteria->reportingAmount != tdsrrc_ReportingAmount_ra_Infinity)
    {
      tdsmac_sib_params.meas_report_criteria.periodic_cfg.report_infinity = FALSE;
    }
    
    
  
    
    /* Get Reporting Interval */
    tdsmac_sib_params.meas_report_criteria.periodic_cfg.report_interval = 
      TDSTVM_RET_PERIODIC_REPORTING_INTERVAL(sib_meas_ptr->
      trafficVolumeMeasSysInfo.reportCriteriaSysInf.u.
      periodicalReportingCriteria->reportingInterval);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Rpting Intvl is %d msec",tdsmac_sib_params.meas_report_criteria.periodic_cfg.report_interval);
    
      /* Check for periodic report, reporting quantity must be present
    * If not present, then do not send meas command to MAC */
    if (tdsmac_sib_params.meas_report_incl != TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Periodic Report: No Rep Quantity");
      return TDSTVM_FAIL;
    }
  } /* End of else of periodic reporting  */
  
  /* Store Reporting Criteria */
  tdsrrctvm_set_reporting_criteria(&tdsmac_sib_params.meas_id, &sib_meas_ptr->
    trafficVolumeMeasSysInfo.reportCriteriaSysInf);
  
  return TDSTVM_SUCCESS; 
}




/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R5

DESCRIPTION  

    Process traffic volume information in a Meas
    Ctrl Message R5. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r5
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
)
{
  uint8 count = 0, cnt = 0, trch_cnt = 0;

  tdsrrc_MeasurementControl_r4_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 transaction_identifier = 0;

  /* Declare MCM config structure that goes to MAC */
  tdsmac_ul_traffic_meas_config_type  mac_mcm_params = {0};
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Variable defined to check if addtl present in MCM is valid
  This varibale is set to TRUE if addtl meas is valid. True means
  in setting addtl meas in RRC db */
  boolean addtl_meas_validation = FALSE;
  
  tdsmeas_validity_enum_type prim_meas_validity = TDSNOT_PRESENT;
  
  tdsrrcmeas_layer_e_type meas_type = TDSMAC_MEAS;
  
  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;
  
  boolean block_set = FALSE;
  /* Assign some default values */
  mac_mcm_params.meas_obj_incl = FALSE;
  mac_mcm_params.meas_qty_incl = FALSE;
  mac_mcm_params.meas_report_incl = FALSE;
  mac_mcm_params.meas_criteria_incl = FALSE;  
  mac_mcm_params.meas_start_incl = TRUE;
  mac_mcm_params.meas_obj_list.num_trch = 0;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.r4.measurementControl_r4;

  /* Store the Transaction Identifier */
  transaction_identifier = tid;
  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_setup)
  {
    /* Check if meas quantity and reporting quantity received. If either of them
    * not received, then configuration incomplete */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement, 
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeReportingQuantity)))
    {
    /* If Reporting quantity indicates var or average but measurement quantity
      * does not indicate time interval, then configuration incomplete */
      if ((rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadAverage == TRUE) ||
        (rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadVariance == TRUE))
      {
        /* Check if meas quantity is equal to RAW. If yes, then ERROR */
        if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
          trafficVolumeMeasQuantity,rrc_TrafficVolumeMeasQuantity,rlc_BufferPayload))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:R.Quan = Var/Avge,M.Quan= Raw");
          
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          /* Send Meas Ctrl Failure Message */
          
          return TDSTVM_FAIL;
        }
        
      }
      
      /* Now check if reporting mode present */
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode)))
      {
        /* Meas reporting Mode absent. Send Meas Ctrl Failure */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of if Meas Quan, Reporting Quan present */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MeasQuantity or ReportingQuantity absent");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
    }
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    
    /* Time to process the actual parameters */
    if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
      u.setup->u.trafficVolumeMeasurement,&mac_mcm_params)
      == TDSTVM_FAIL)
    {
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
      
      /* Set Error Cause */
      rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
    }
    
    /* Check if Meas Validity PRESENT */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,measurementValidity))
    {
      prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
        rrc_mcm_ptr->measurementCommand.u.setup->u.
        trafficVolumeMeasurement->measurementValidity.ue_State);
    }
    
    if (((prim_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
      ((prim_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: RRC val,MCM val differ");
      mac_mcm_params.meas_start_incl = FALSE;
    }

    if((prim_meas_validity == TDSALL_STATES) && (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.
      trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasurementObjectList))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas validity == all states,but no tvm object");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf); 
      return TDSTVM_FAIL;
    }
 
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
      /* Call fn which makes check for additional measurements */
      if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
        transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return TDSTVM_FAIL;
      }
      
      /* This means additional meas has been successfully validated */
      addtl_meas_validation = TRUE;
      
    } /* End of if of Additional Meas Present */
    
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    /* To read traffic volume reporting mode */
    mac_mcm_params.meas_report_mode.transfer_mode = 
      TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
      measurementReportingMode.measurementReportTransferMode);

    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_validation, TRUE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
      
    }
    
    /* Set Measurement Identity in tdstvm_meas_id_list for SETUP */
    tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
    
    /* Set parameters in RRC DB */
    if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
    {
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
      
    }
    /* Store additional meas */
    /* Check if Additional Meas has been validated successfully. */
    /* If yes, then store all additional meas in RRC DB */
    if (addtl_meas_validation == TRUE) 
    {
      tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
    }
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_modify)
  {
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_MODIFY;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))
    {
      /* To read traffic volume reporting mode */
      mac_mcm_params.meas_report_mode.transfer_mode = 
        TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
        measurementReportingMode.measurementReportTransferMode);  
      
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify
        ,tdsrrc_MeasurementCommand_r4_modify,measurementType))
    {
      if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType,
        rrc_MeasurementType_r4_trafficVolumeMeasurement)))
      {
      /* If one comes here, then it means that meas id earlier determined
      * by RRC as traffic volume is actually not Traffic Volume. Send
        * Meas Ctrl failure Msg to UTRAN */
        
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Meas Id is not Traffic Volume");
        /* Send Meas Ctrl Failure */
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
      
      /* Time to process the actual parameters */
      if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
        u.modify->measurementType.u.trafficVolumeMeasurement,
        &mac_mcm_params) == TDSTVM_SUCCESS)
      {
        
        /* Check if Meas Validity PRESENT */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          trafficVolumeMeasurement,
          tdsrrc_TrafficVolumeMeasurement,measurementValidity))
        {
          prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
           rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
            trafficVolumeMeasurement->measurementValidity.ue_State);
        }

        /* Process Additional Meas */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

          if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
            transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
          {
            /* Called function has already sent Meas Ctrl Failure */
            return TDSTVM_FAIL;
          }
          
          /* This means additional meas has been successfully validated */
          addtl_meas_validation = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
          
            rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
          
            /* Send MCF msg to UTRAN */  
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
            return TDSTVM_FAIL;
          
          }
          
        } /* End of if of Additional Meas Present */
        
        
        tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
        
        if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
        {
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          /* Send MCF msg to UTRAN */  
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          
          return TDSTVM_FAIL;
          
        }

        /* Store additional meas */
        /* Check if Additional Meas has been validated successfully. */
        /* If yes, then store all additional meas in RRC DB */
        if (addtl_meas_validation == TRUE) 
        {
          tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
        }
        
        
      } /* End if if of Traffic Volume params successfully read */
      else
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
        
        /* Set Error Cause */
        rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of If of meas_type present */
    else
    {
      /* Set Measurement Identity in tdstvm_meas_id_list for MODIFY */
      tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);

      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

        if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
          transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return TDSTVM_FAIL;
        }
        
        /* This means additional meas has been successfully validated */
        addtl_meas_validation = TRUE;
        
      } /* End of if of Additional Meas Present */
      
      if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
        addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
      {
        /* As UTRAN tried to configure more than 4 Addtl Meas */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
        
      }
      
      
      if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
      {
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        
        return TDSTVM_FAIL;
        
      }

      /* Store additional meas */
      /* Check if Additional Meas has been validated successfully. */
      /* If yes, then store all additional meas in RRC DB */
      if (addtl_meas_validation == TRUE) 
      {
        tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
      }
    }
    
    
  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdstvm_meas_id_list */
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_DELETE;
    
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&mac_mcm_params.meas_id);
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
    {
      if (tdstvm_meas_id_list[count].meas_id == 
        rrc_mcm_ptr->measurementIdentity)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Delete:Remove Meas Id");
        
        tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        
        tdstvm_meas_id_list[count].block_rach_incl = FALSE;
        
        tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
        
        tdstvm_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        
        tdstvm_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++) 
        {
          tdstvm_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        
        break;  /* Come out of for */
      } /* End of if */
      
    }
    alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
    mac_mcm_params.config_ack_num = alloc_idx;
    tdsrrctvm_send_mac_cmd(&mac_mcm_params);
    
    return TDSTVM_SUCCESS;
    
  }
  
  /* Time to store meas validity for SETUP or Modify 
  It has already been verified that the Measurement exists */ 
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == 
      rrc_mcm_ptr->measurementIdentity)
    {
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      
      /* This means that meas would be saved on trans from DCH to FACH */
      /* Change ownership to Meas Ctrl Msg */      
      
      if (mac_mcm_params.meas_report_mode.report_mode == TDSEVENT_TRIGGERED_MODE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"trch_info_incl = %d",mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl);
        if (mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl == TRUE)
        {
          for (trch_cnt = 0; trch_cnt < mac_mcm_params.meas_report_criteria.event_cfg.num_trch; trch_cnt++)
          {
            for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].num_events; cnt++)
            {
              if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].event_id == TDSEVENT_4A) &&
                (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
              {
                tdstvm_meas_id_list[count].block_rach_incl = TRUE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
                block_set = TRUE;
                break; 
              } /* End of if of event_id and tx_interrupt_tmr check */
            } /* End of for */
            if (block_set == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
              break;
            }
          } /* end of 2nd for */
        }
        else
        {
          /* Keep up with MAC, if trch_info_incl = FALSE, only get event info from first element */
          for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].num_events; cnt++)
          {
            if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].event_id == TDSEVENT_4A) &&
              (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
            {
              tdstvm_meas_id_list[count].block_rach_incl = TRUE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
              block_set = TRUE;
              break; 
            } /* End of if of event_id and tx_interrupt_tmr check */
          } /* End of for */
          if (block_set == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
            break;
          }
        }
        break;
      } /* End of if for Event Triggered Mode */
      break;
    } /* End of if */
    
  } /* End of for loop */
  
  
  /* Now send config command to MAC */
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
  mac_mcm_params.config_ack_num = alloc_idx;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending rrctvm_send_mac_command");
  tdsrrctvm_send_mac_cmd(&mac_mcm_params);
  return TDSTVM_SUCCESS;
}




/*===========================================================================

FUNCTION     RRCTVM_READ_TRAFFIC_VOL_PARAMS

DESCRIPTION  

    Read traffic volume information in a Meas
    Ctrl Message. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    Sucess/Failure.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_read_traffic_vol_params
(
tdsrrc_TrafficVolumeMeasurement* rrc_mcm_ptr,
tdsmac_ul_traffic_meas_config_type*  mac_config_ptr
)
{
  tdsrrc_TrafficVolumeMeasurementObjectList * mac_obj_ptr = NULL;
  uint32 idx=0;
  /* No of transport channels */
  uint8 transport_channel_count = 0;

  /* Initialize Measured Interval */
  mac_config_ptr->meas_qty.meas_interval = TDSQUANTITY_NOT_PRESENT;

  /* Initialize individual reporting quantities */
  mac_config_ptr->meas_report_qty.rlc_payload = FALSE;
  mac_config_ptr->meas_report_qty.avg_payload = FALSE;
  mac_config_ptr->meas_report_qty.var_payload = FALSE;
  
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, 
    tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasurementObjectList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Meas Object present");
    
    mac_obj_ptr = 
      &rrc_mcm_ptr->trafficVolumeMeasurementObjectList;
    
    mac_config_ptr->meas_obj_incl = TRUE;
    
    while ((mac_obj_ptr->n > idx) && (transport_channel_count < TDSUE_MAX_TRCH))
    {
      if (mac_obj_ptr->elem[idx].t == T_tdsrrc_UL_TrCH_Identity_usch)
      {
#ifdef FEATURE_TDSCDMA_HSUPA
        /* RRC no need to judgement weather UPA is started, beacuse CELL_FACH may recevie TVM OTA */
        if (mac_obj_ptr->elem[idx].u.usch == 32) 
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:E-DCH object %d specified",mac_obj_ptr->elem[idx].u.usch);
          mac_config_ptr->meas_obj_list.trch_info[transport_channel_count].trch_type = TDSEDCH;
          /* Get Channel ID too */
          mac_config_ptr->meas_obj_list.trch_info[transport_channel_count].trch_id = 
            (uint16) mac_obj_ptr->elem[idx].u.usch; 
          /* Increment count */
          transport_channel_count++;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Support TVM on E-DCH but USCH object given. Ignoring"); 
        }
#else
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:USCH object given. Ignoring"); 
#endif
      }
      else if (mac_obj_ptr->elem[idx].t ==  T_tdsrrc_UL_TrCH_Identity_rachorcpch)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:RACH object specified");
        /* Set channel type */
        mac_config_ptr->meas_obj_list.trch_info[transport_channel_count].
          trch_type = TDSRACH;
        
        /* Increment count */
        transport_channel_count++;
        
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:DCH object %d specified",mac_obj_ptr->elem[idx].u.dch);
        
        /* Set channel type */
        mac_config_ptr->meas_obj_list.trch_info[transport_channel_count].
          trch_type = TDSDCH;
        
        /* Get Channel ID too */
        mac_config_ptr->meas_obj_list.trch_info[transport_channel_count].trch_id = 
          (uint16) mac_obj_ptr->elem[idx].u.dch; 
        
        transport_channel_count++;
      }
      idx++;
      if ((mac_obj_ptr != NULL) && (transport_channel_count == TDSUE_MAX_TRCH))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:All transport Ch not included");
        break;
      }
      continue;
      
    }   /* End of while */
    
    mac_config_ptr->meas_obj_list.num_trch = transport_channel_count;  
    
  } /* End of meas object present */

  /* TDD comment:
         MAC only consider meas_obj_incl = FALSE represent measurement all exiting transport channel. 
         If RRC config meas_obj_incl = TRUE and meas_obj_list.num_trch = 0 ,
         MAC will not measurement traffic volume, so add handle to set meas_obj_incl = FALSE.
  */
  if (mac_config_ptr->meas_obj_list.num_trch == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:No Of TrCh 0,RepCrit ALL");
    mac_config_ptr->meas_obj_incl = FALSE;
  }
  
  
  /* Check if Meas Quantity present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, 
    tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasQuantity))
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TVM:Meas Quan %d RlC:A:V/1:2:3", rrc_mcm_ptr->
      trafficVolumeMeasQuantity.t,0,0);

    mac_config_ptr->meas_qty_incl = TRUE;
    
    if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->
      trafficVolumeMeasQuantity,rrc_TrafficVolumeMeasQuantity,rlc_BufferPayload))
    {
      mac_config_ptr->meas_qty.meas_qty_enum = TDSRLC_BUF_PAYLOAD;
    }
    else if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->
      trafficVolumeMeasQuantity,rrc_TrafficVolumeMeasQuantity,averageRLC_BufferPayload))
    {
      mac_config_ptr->meas_qty.meas_qty_enum = TDSAVERAGE_RLC_BUF_PAYLOAD;
      /* Put time interval too */
      mac_config_ptr->meas_qty.meas_interval = 
        rrc_mcm_ptr->trafficVolumeMeasQuantity.
        u.averageRLC_BufferPayload;

      mac_config_ptr->meas_qty.meas_interval = mac_config_ptr->meas_qty.meas_interval * TDSMEAS_QUANTITY_INTERVAL_OFFSET;
    }
    else
    {
      mac_config_ptr->meas_qty.meas_qty_enum = TDSVARIANCE_RLC_BUF_PAYLOAD;
      /* Put time interval too */
      mac_config_ptr->meas_qty.meas_interval = rrc_mcm_ptr->
        trafficVolumeMeasQuantity.u.varianceOfRLC_BufferPayload;

      mac_config_ptr->meas_qty.meas_interval = mac_config_ptr->meas_qty.meas_interval * TDSMEAS_QUANTITY_INTERVAL_OFFSET;
    }
    
    
  } /* End of if of Meas Quantity present */

  /* Meas Quantity absent has already been taken care off for setup by
   * sending a Meas Ctrl Msg */
  
  /* Check if Reporting Quantity present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, 
    tdsrrc_TrafficVolumeMeasurement,trafficVolumeReportingQuantity))
  {
    mac_config_ptr->meas_report_incl = TRUE;
    
    if (rrc_mcm_ptr->
      trafficVolumeReportingQuantity.rlc_RB_BufferPayload == TRUE)
    {
      mac_config_ptr->meas_report_qty.rlc_payload = TRUE;
    }
    if (rrc_mcm_ptr->
      trafficVolumeReportingQuantity.rlc_RB_BufferPayloadAverage == TRUE)
    {
      mac_config_ptr->meas_report_qty.avg_payload = TRUE;
    }
    if (rrc_mcm_ptr->
      trafficVolumeReportingQuantity.rlc_RB_BufferPayloadVariance == TRUE)
    {
      mac_config_ptr->meas_report_qty.var_payload = TRUE;
    }
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TVM:Rpting Quan RLC%d A%d V%d", mac_config_ptr->meas_report_qty.rlc_payload,
      mac_config_ptr->meas_report_qty.avg_payload, 
      mac_config_ptr->meas_report_qty.var_payload);

  }   /* End of reporting quantity present */
  
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->reportCriteria, 
    rrc_TrafficVolumeReportCriteria_trafficVolumeReportingCriteria))
  {
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->reportCriteria.u.trafficVolumeReportingCriteria,
      transChCriteriaList)) &&
      (rrc_mcm_ptr->reportCriteria.u.trafficVolumeReportingCriteria->
      transChCriteriaList.n != 0))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Event triggered Mode selected");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Empty event crit structure");
      return TDSTVM_FAIL;
    }
    mac_config_ptr->meas_report_mode.report_mode = TDSEVENT_TRIGGERED_MODE;
    if (tdsrrctvm_process_event_criteria(&rrc_mcm_ptr->reportCriteria.
      u.trafficVolumeReportingCriteria->transChCriteriaList, 
      &mac_config_ptr->meas_report_criteria.event_cfg) == TDSTVM_FAIL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Meas Req to MAC not to be sent");
      return TDSTVM_FAIL;
    }
    /* Set meas_criteria_incl to TRUE */
    mac_config_ptr->meas_criteria_incl = TRUE;
    
  } /* End of if of Event Triggered Mode */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->reportCriteria,
    rrc_TrafficVolumeReportCriteria_periodicalReportingCriteria))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Periodic Mode selected");
    mac_config_ptr->meas_report_mode.report_mode = TDSPERIODIC_MODE;
    
    /* Set default no of reports */
    mac_config_ptr->meas_report_criteria.periodic_cfg.report_infinity = TRUE;
    
    /* Set meas_criteria_incl to TRUE */
    mac_config_ptr->meas_criteria_incl = TRUE;
    
    
    /* Get periodic information */
   
    mac_config_ptr->meas_report_criteria.periodic_cfg.num_reports = 
      TDSTVM_RET_REPORTING_AMOUNT(rrc_mcm_ptr->reportCriteria.
      u.periodicalReportingCriteria->reportingAmount);
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Reporting Amt is %d",mac_config_ptr->meas_report_criteria.periodic_cfg.num_reports);
    
    if (rrc_mcm_ptr->reportCriteria.u.periodicalReportingCriteria->
      reportingAmount != tdsrrc_ReportingAmount_ra_Infinity)
    {
      mac_config_ptr->meas_report_criteria.periodic_cfg.report_infinity = FALSE;
      
    }
      
    
    
    /* Get Reporting Interval */
    mac_config_ptr->meas_report_criteria.periodic_cfg.report_interval = 
      TDSTVM_RET_PERIODIC_REPORTING_INTERVAL(
      rrc_mcm_ptr->reportCriteria.u.periodicalReportingCriteria->reportingInterval);
    
    TDSRRC_MSG3(MSG_LEGACY_LOW,"Reporting Interval is %d",
      mac_config_ptr->meas_report_criteria.
      periodic_cfg.report_interval,0,0);
    
  } /* End of else if of periodic reporting  */
  else
  {
   TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Addtl Meas Mode selected");

   /* Set meas_criteria_incl to TRUE */
   mac_config_ptr->meas_criteria_incl = TRUE;
   mac_config_ptr->meas_report_mode.report_mode = TDSNO_REPORTING;
  }

  return TDSTVM_SUCCESS; 
}


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_EVENT_CRITERIA

DESCRIPTION 

    Sets the traffic volume event criteria for MAC. Event criteria here can be 
    specified for a transport channel and one transport channel can have at the
    maximum of 2 events (4A/4B).

DEPENDENCIES

    None

RETURN VALUE

    Sucess/Failure  

SIDE EFFECTS

    None
===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_event_criteria
(
tdsrrc_TransChCriteriaList * event_crit_ptr,
tdsmac_ul_traffic_event_trigger_cfg_type* mac_event_crit_ptr
)
{
  /* Store event criteria parameters */
  tdsrrc_TransChCriteriaList * rrc_event_crit_ptr = NULL;
  
  /* To count the no of transport channels */
  uint8 transport_channel_cnt = 0;
  
  boolean dch_object_specified = FALSE;
  
  boolean rach_object_specified = FALSE;
  
  uint16 no_of_events = 0;
  uint32 idx=0;
  tdsrrc_TransChCriteria_eventSpecificParameters*  event_param_ptr;
  
  tdsrrctvm_status_e_type status = TDSTVM_SUCCESS;
  
  uint8 event_tr_ch_flag = 0; /* If 0, then event criteria is for all uplink channels */
  
  rrc_event_crit_ptr = event_crit_ptr;
 
  if(rrc_event_crit_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Event criteria pointer is null");
    return TDSTVM_FAIL;
  } 
  
  /* Store the head ptr of event parameters */
  event_param_ptr = &rrc_event_crit_ptr->elem[0].eventSpecificParameters;
  
  /* Get the events */
  while ((rrc_event_crit_ptr->n > idx) && (transport_channel_cnt < TDSUE_MAX_TRCH))
  { 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Event Criteria List present");
    
    /* If transport channel id is not present, then that means that this
    * criteria is valid for all the transport channels. So ignore rest of the channels */
    if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(rrc_event_crit_ptr,ul_transportChannelID))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Transport Channel Id present");
      /* Set Id */
      if ( rrc_event_crit_ptr->elem[idx].ul_transportChannelID.t == 
        T_tdsrrc_UL_TrCH_Identity_usch)
      {
#ifdef FEATURE_TDSCDMA_HSUPA
         if (rrc_event_crit_ptr->elem[idx].ul_transportChannelID.u.usch == 32) 
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:E-DCH object given");
            mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
              trch_info.trch_type = TDSEDCH; 
            
            dch_object_specified = TRUE;
            /* Tr Id */
            mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].trch_info.trch_id = 
              (uint16) rrc_event_crit_ptr->elem[idx].ul_transportChannelID.u.usch;
         }
         else
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Ignoring USCH TrCh object,support HSUPA");
           idx++;
           continue;
         }
#else
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Ignoring USCH TrCh object,unsupport HSUPA");
         idx++;
         continue;
#endif

      }
      else if (rrc_event_crit_ptr->elem[idx].ul_transportChannelID.t ==
         T_tdsrrc_UL_TrCH_Identity_rachorcpch)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:RACH object given in R.Crit"); 
        /* Tr Ch type */
        mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          trch_info.trch_type = TDSRACH;
        rach_object_specified = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:DCH object given");
        mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          trch_info.trch_type = TDSDCH; 
        
        dch_object_specified = TRUE;
        /* Tr Id */
        mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].trch_info.trch_id = 
          (uint16) rrc_event_crit_ptr->elem[idx].ul_transportChannelID.u.dch;
      }
      /* Increment flag */
      event_tr_ch_flag++;
      
    }   /* End of if of rrc_ul_transportChannelID_present present */
    
        /* We are following the policy that if UTRAN does not specify the meas
        * object here for the firts scenario, then the event criteria is common 
        * for all uplink channels. So read the tr ch id the first time. If not 
        * present, then read the event criteria and exit. UTRAN can mess up by 
        * not giving the event criteria too. In that scenario, don't send command to MAC */
    
    /* Now for each TrChId, there can be 2 events specified */
    if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(rrc_event_crit_ptr,eventSpecificParameters))
    {
      uint32 idy =0;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Event Params present");
      
      while (event_param_ptr->n > idy)
      {
        /* Set Event ID */
        mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          event_param[no_of_events].event_id = 
          TDSTVM_RET_EVENT_ID(event_param_ptr->elem[idy].eventID);
        
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"TVM:Event Id %d 4A:1/4B:2", mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          event_param[no_of_events].event_id,0,0);
        
        /* Set threshold bytes */
        mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          event_param[no_of_events].report_threshold = 
          TDSTVM_RET_THRESHOLD_BYTES(event_param_ptr->elem[idy].reportingThreshold);
        
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"TVM:Threshold %d bytes", mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          event_param[no_of_events].report_threshold,0,0);
        
        
        /* Set these params to Default values */
        mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          event_param[no_of_events].time_to_trigger = TDSQUANTITY_NOT_PRESENT;
        
        mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          event_param[no_of_events].trigger_prohibit_tmr = TDSQUANTITY_NOT_PRESENT;
        
        mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
          event_param[no_of_events].tx_interrupt_tmr = TDSQUANTITY_NOT_PRESENT;
        
        
        /* Now browse the event parameters */
        if ( event_param_ptr->elem[idy].m.timeToTriggerPresent)
        {
          mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
            event_param[no_of_events].time_to_trigger = 
            TDSTVM_RET_TIME_TO_TRIGGER(event_param_ptr->elem[idy].timeToTrigger);
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"TVM:Time to trigger %d msec",mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
            event_param[no_of_events].time_to_trigger,0,0);
        }
        
        /* Pending time after trigger */
        if (event_param_ptr->elem[idy].m.pendingTimeAfterTriggerPresent)
        {
          mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
            event_param[no_of_events].trigger_prohibit_tmr = 
            TDSTVM_RET_PENDING_TIME_AFTER_TRIGGER(
            event_param_ptr->elem[idy].pendingTimeAfterTrigger);
          
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"TVM:Pending time after trigger %d msec",mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].
            event_param[no_of_events].trigger_prohibit_tmr,0,0);
        }
        
        
        /* Interruption time after trigger */
        if (event_param_ptr->elem[idy].m.tx_InterruptionAfterTriggerPresent)
        {
          if (((dch_object_specified == TRUE) && (rach_object_specified == TRUE)) ||
            (rach_object_specified == TRUE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:I.Time after trigger present");
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"DCH object %d,RACH object %d",dch_object_specified,rach_object_specified);
            
            mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].event_param[no_of_events].tx_interrupt_tmr = 
              TDSTVM_RET_INTPT_TIME_AFTER_TRIGGER(
              event_param_ptr->elem[idy].tx_InterruptionAfterTrigger);
          }
          else
          {
            if (event_tr_ch_flag == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"No of TrCh = 0,I.Time value specified");
              mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].event_param[no_of_events].tx_interrupt_tmr = 
                TDSTVM_RET_INTPT_TIME_AFTER_TRIGGER(
                event_param_ptr->elem[idy].tx_InterruptionAfterTrigger);
            }
          }
        }
        
        /* Increase no of events */
        no_of_events++;
        
        /* Get next value */
        idy++;
        
        
      } /* End of while of no of events */
      
      /* Set no of events */
      mac_event_crit_ptr->trch_event_cfg[transport_channel_cnt].num_events
        = no_of_events;
      
      /* Make sure event struct was not NULL */
      if (no_of_events == 0)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Event Parameters NULL");
        return TDSTVM_FAIL;
      }
      
      if (event_tr_ch_flag == 0)
      {
      /* This means that meas object was not specified but event params were present
        * So just skip reading the rest of the event channels */
        
        /* Set it to FALSE==>Apply to all uplink channels */
        mac_event_crit_ptr->trch_info_incl = FALSE;
        
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:EventCrit to ULTransCh");
        
        return TDSTVM_SUCCESS;
      }
      
      
      
    } /* End of if of event params present */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Event params not specified:TrChId");
      return TDSTVM_FAIL;
    } /* End of else of event params present */
    
    /* Increment Tr Ch count */
    transport_channel_cnt++;
    
    /* Get next one */
    idx++;
    
    if (rrc_event_crit_ptr->n  == idx)
    {
      /* End of event params */
      break;
    }
    
    event_param_ptr = &rrc_event_crit_ptr->elem[idx].eventSpecificParameters;
    /* Reinitialize no of events */
    no_of_events = 0;    
    
  } /* End of while of rrc_event_crit_ptr not NULL */
  
  mac_event_crit_ptr->num_trch = transport_channel_cnt;
  
  /* Means individual tr Ch criteria present */
  mac_event_crit_ptr->trch_info_incl = TRUE;
  
  return status;
}


/*===========================================================================

FUNCTION   RRCTVM_SEND_TRAFFIC_VOL_MEAS_REPORT

DESCRIPTION 

    Sends the traffic volume Measurement Report

DEPENDENCIES

    None

RETURN VALUE

    None  

SIDE EFFECTS

    None
===========================================================================*/

void tdsrrctvm_send_traffic_vol_meas_report
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Stores meas report */
  tdsrrc_UL_DCCH_Message* msg_ptr = NULL; 
  
  /* Stores status of send chain */
  tdsrrcsend_status_e_type status; 
  
  /* RLC ack or unack mode */
  uecomdef_logch_mode_e_type logical_channel_mode;
  
  /* Logical Channel Id */
  rlc_lc_id_type ul_lc_id;  
  
  /* Radio bearer mode */
  tdsrrc_RB_Identity rb_mode;
  
  /* L2 ack status */
  tdsrrcsend_l2ack_status_e_type l2_ack_status;
  
  uint8 count = 0, cnt = 0;

  uint16 tvm_to_be_deleted;
  
  tdsrrc_TrafficVolumeMeasuredResultsList * measured_results_ptr = NULL;
  
  tdsmac_ul_traffic_meas_ind_type* meas_report_ptr = NULL;
  
  /* Ptr which carries info about the command */
  tdsrrc_cmd_type  *rrc_meas_out_cmd_ptr = NULL;

  meas_report_ptr = &(cmd_ptr->cmd.cmac_ul_traffic_ind);
  
  /* First find if it is an event triggered or periodic report */
  if (meas_report_ptr->report_mode == TDSNO_REPORTING)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:MAC sent incorrect report type");
    return;
  }
  
  if (meas_report_ptr->num_reports_ind == TRUE)
  {    
    for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
    {
      if (tdstvm_meas_id_list[count].meas_id == meas_report_ptr->meas_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Removing Meas %d from ID List",meas_report_ptr->meas_id);
        tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;

        tdstvm_meas_id_list[count].block_rach_incl = FALSE;
        tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
        tdstvm_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        tdstvm_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;

        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
          tdstvm_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;

        break;
      }      
    } /* End of for loop */     
  }

  /* Now delete it from Addtl Meas DB */
  if (meas_report_ptr->num_reports_ind == TRUE)
  {
    tvm_to_be_deleted = meas_report_ptr->meas_id;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Delete %d from addtl meas DB",tvm_to_be_deleted);
    tdsrrcmeas_delete_addtl_meas(&tvm_to_be_deleted);
  }

  
  if ( meas_report_ptr->report_transfer_mode == TDSACK_MODE )
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
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:RLC Logical channel not found");
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
    (uint8)meas_report_ptr->meas_id;
  
  /* Initialize Meas Report bit mask */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport);

  /* Malloc memory for MeasResult_v4b0  */
  tdsrrcmeas_set_measurement_result_v4b0ext(msg_ptr);
  
  /* It is event triggered report */
  if (meas_report_ptr->report_mode == TDSEVENT_TRIGGERED_MODE)
  {
    /* Set event results present */
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.measurementReport,
      tdsrrc_MeasurementReport,eventResults); 
    
    /* Set traffic vol event results */
    TDSRRC_SET_COMMON_MSG_TYPE2(msg_ptr->message.u.measurementReport.eventResults,
      rrc_EventResults,trafficVolumeEventResults);

    msg_ptr->message.u.measurementReport.eventResults.u.
        trafficVolumeEventResults =
        rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_TrafficVolumeEventResults);
    /* Set event name */
    if (meas_report_ptr->event_result.event_id == TDSEVENT_4A)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:TDSEVENT 4A Reported");
      msg_ptr->message.u.measurementReport.eventResults.u.
        trafficVolumeEventResults->trafficVolumeEventIdentity = tdsrrc_TrafficVolumeEventType_e4a;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:TDSEVENT 4B Reported");
      msg_ptr->message.u.measurementReport.eventResults.u.
        trafficVolumeEventResults->trafficVolumeEventIdentity = tdsrrc_TrafficVolumeEventType_e4b;
    }
    
    /* Set transport channel on which event happens */
    msg_ptr->message.u.measurementReport.eventResults.u.
      trafficVolumeEventResults->ul_transportChannelCausingEvent.t
      = TDSTVM_RET_TRCH_TYPE(meas_report_ptr->event_result.trch_info.trch_type);
    
    /* Now set the channel id corresponding to channel type
     * This is doen only for DCH */
    if (msg_ptr->message.u.measurementReport.eventResults.u.
      trafficVolumeEventResults->ul_transportChannelCausingEvent.t == 
      T_tdsrrc_UL_TrCH_Identity_dch)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Event occurred on TDSDCH TrChId = %d",meas_report_ptr->event_result.trch_info.trch_id);
      
      msg_ptr->message.u.measurementReport.eventResults.u.
        trafficVolumeEventResults->ul_transportChannelCausingEvent.u.dch
        = (tdsrrc_TransportChannelIdentity)meas_report_ptr->event_result.trch_info.trch_id;
    }
#ifdef FEATURE_TDSCDMA_HSUPA
    else if(msg_ptr->message.u.measurementReport.eventResults.u.trafficVolumeEventResults->
          ul_transportChannelCausingEvent.t == T_tdsrrc_UL_TrCH_Identity_usch)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Event occurred on TDSEDCH TrChId = %d",meas_report_ptr->event_result.trch_info.trch_id);
      
      msg_ptr->message.u.measurementReport.eventResults.u.
        trafficVolumeEventResults->ul_transportChannelCausingEvent.u.usch
        = (tdsrrc_TransportChannelIdentity)meas_report_ptr->event_result.trch_info.trch_id;
    }
#endif
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Event occurred on RACH");
    }
  } /* End of event triggered report */
  
  if ((meas_report_ptr->report_mode == TDSPERIODIC_MODE) &&
    (meas_report_ptr->num_rbs == 0))
  {
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Blank Meas Rpt");
  }
  else
  {
  if ((meas_report_ptr->report_mode == TDSPERIODIC_MODE)  ||
    (meas_report_ptr->num_rbs != 0))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Measured Results received");
    
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
         v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.n = 1;
    
    /* Set Choice to Traffic Volume Results */
    TDSRRC_SET_COMMON_MSG_TYPE(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.
       laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext.
       additionalMeasuredResults_LCR.elem[0],
       rrc_MeasuredResults_LCR_r4_trafficVolumeMeasuredResultsList);
    
    measured_results_ptr = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_TrafficVolumeMeasuredResultsList);

    if (measured_results_ptr == NULL)
    {
      ERR_FATAL("Failed to allocate memory",0,0,0);
    }

    /* Update the linked list head ptr */
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.
       laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext.
       additionalMeasuredResults_LCR.elem[0].u.trafficVolumeMeasuredResultsList = measured_results_ptr;

    measured_results_ptr->n = meas_report_ptr->num_rbs;

    ALLOC_ASN1ARRAY1(&tdsenc_ctxt,measured_results_ptr,tdsrrc_TrafficVolumeMeasuredResults);

    if (measured_results_ptr->elem == NULL)
    {
      ERR_FATAL("Failed to allocate memory",0,0,0);
    }
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Measured Results is for %d RB's",meas_report_ptr->num_rbs);
    
    for (count=0; count < meas_report_ptr->num_rbs; count++)
    {
     /* Initialize bit mask */
      TDSRRC_RESET_MSG_IE_PRESENT(measured_results_ptr->elem[count]);
      /* Get radio bearer Id */
      measured_results_ptr->elem[count].rb_Identity = 
       (tdsrrc_RB_Identity) meas_report_ptr->rb_meas[count].rb_id;
      
     
      /* Now check the payload in each radio bearer */
      if (meas_report_ptr->rb_meas[count].raw_rlc_payload == TDSINVALID_PAYLOAD)
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Raw RLC payload not present");
      }
      else
      {
        measured_results_ptr->elem[count].m.rlc_BuffersPayloadPresent =1; 
        
        /* Convert the size */        
        measured_results_ptr->elem[count].rlc_BuffersPayload = 
          TDSTVM_RET_RLC_BUFFER_PAYLOAD(meas_report_ptr->rb_meas[count].raw_rlc_payload);
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"TVM:RB = %d,Raw RLC payload sent = %d",meas_report_ptr->rb_meas[count].rb_id,meas_report_ptr->rb_meas[count].raw_rlc_payload);
        
      }
      if (meas_report_ptr->rb_meas[count].average_rlc_payload == TDSINVALID_PAYLOAD)
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"TVM:Avge RLC payload not present");
      }
      else
      {
        measured_results_ptr->elem[count].m.averageRLC_BufferPayloadPresent =1; 
        
        /* Convert the size in bytes */
        measured_results_ptr->elem[count].averageRLC_BufferPayload = 
          TDSTVM_RET_AVG_BUFFER_PAYLOAD(meas_report_ptr->rb_meas[count].average_rlc_payload);
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"TVM:RB = %d,Avge RLC payload sent = %d",meas_report_ptr->rb_meas[count].rb_id,meas_report_ptr->rb_meas[count].average_rlc_payload);
        
      }
      if (meas_report_ptr->rb_meas[count].variance_rlc_payload == TDSINVALID_PAYLOAD)
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Var RLC payload not present");
      }
      else
      {
        measured_results_ptr->elem[count].m.varianceOfRLC_BufferPayloadPresent=1;
        
        /* Convert the size in bytes */
        measured_results_ptr->elem[count].varianceOfRLC_BufferPayload = 
          TDSTVM_RET_VAR_BUFFER_PAYLOAD(meas_report_ptr->rb_meas[count].variance_rlc_payload);
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"TVM:RB = %d,Var Payload = %ul",meas_report_ptr->rb_meas[count].rb_id,meas_report_ptr->rb_meas[count].variance_rlc_payload);
        
      }
      
         /* End of else */
    } /* End of for loop */
  } /* End of if of measured results */
  } /* End of else */

  /* Append Additional Measured Results if available */

  tdsrrcmeas_process_additional_measured_results(
   &meas_report_ptr->meas_id, TDSMAC_MEAS, &msg_ptr->message.u.measurementReport);
  
  if (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)
  {
    /* Append RACH report if RACH criteria satisfied and report available */
    if(tdsrrcmeas_append_meas_rpt_on_rach(
     &msg_ptr->message.u.measurementReport.measuredResultsOnRACH, TDSRRC_PROCEDURE_MCMR, FALSE) == TDSRRCMEAS_RACH_SUCCESSFUL)
    {
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.measurementReport,
        tdsrrc_MeasurementReport,measuredResultsOnRACH);
    }
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
      if(SUCCESS == tdsrrcifreq_append_meas_rpt_on_rach( &(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.
        measurementReport_v690ext.measuredResultsOnRACHinterFreq),TDSRRC_PROCEDURE_MCMR))
      {
        if(!(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.measurementReport,
             tdsrrc_MeasurementReport,v390nonCriticalExtensions)))
        {
          TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.measurementReport,
            tdsrrc_MeasurementReport,v390nonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.measurementReport_v390ext);
        }
        if(!(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.m.laterNonCriticalExtensionsPresent))
        {
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.m.laterNonCriticalExtensionsPresent =1;
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions);
        }
        if(!(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.m.
            v4b0NonCriticalExtensionsPresent))
        {
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent =1;
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext);
        }
        if(!(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent))
        {
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.
             v590NonCriticalExtensionsPresent=1 ;
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
             measurementReport_v590ext);
        }
        if(!(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.
            laterNonCriticalExtensions.v4b0NonCriticalExtensions.
            v590NonCriticalExtensions.m.v5b0NonCriticalExtensionsPresent))
        {
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.
             v5b0NonCriticalExtensionsPresent=1;
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.
             measurementReport_v5b0ext);
        }
        if(!(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.m.
             v690NonCriticalExtensionsPresent))
        {
          msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.m.
             v690NonCriticalExtensionsPresent=1;
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.
             v690NonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.
             v690NonCriticalExtensions.measurementReport_v690ext);
        }
        TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.
            v690NonCriticalExtensions.measurementReport_v690ext,
            tdsrrc_MeasurementReport_v690ext_IEs,measuredResultsOnRACHinterFreq);
      }
    }
#endif
  }

  /* Closed measurement_result_v4b0ext if malloc memory no use*/
  tdsrrcmeas_close_measurement_result_v4b0ext(msg_ptr);

  /* This places SDU into RLC watermark queue */
  status =  tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_MCMR, (void *) msg_ptr,
    ul_lc_id, l2_ack_status);
  
  if ( status != TDSRRCSEND_SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Send chain failed to send MeasReport");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Meas Report sent from RRC to RLC");       
    
    if(meas_report_ptr->report_transfer_mode == TDSACK_MODE)
    {
      tdsrrcmeas_pending_rpt++; 
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"pending meas rpt = %d",tdsrrcmeas_pending_rpt);
    } 
    
    /* Now check if we need to send block command to MAC 
     * RRC sends to MAC this command only if meas_id sent by
     * MAC has block_rach_incl set to TRUE in tdstvm_meas_id_list
     * and RRC is in CELL_FACH state. If either of this condition is
     * not true, RRC does not send this command to MAC */    
    
    if ((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH) &&
    (meas_report_ptr->report_mode == TDSEVENT_TRIGGERED_MODE) &&
    (meas_report_ptr->event_result.event_id == TDSEVENT_4A))
    { 
      
      for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
      {
        if ((tdstvm_meas_id_list[count].meas_id == meas_report_ptr->meas_id) &&
          (tdstvm_meas_id_list[count].block_rach_incl == TRUE))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH for Meas Id = %d",meas_report_ptr->meas_id);

          /* Send command to MAC. Allocate memory */
          if ( (rrc_meas_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL )
          {
            ERR_FATAL("Memory Failure for Internal command",0,0,0);
          }
          else     
          {
            tdsl2_ul_req_cmd * rrc_cmd_l2_req = NULL;
            tdsrrc_get_pointer_l2_req_value(&rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.l2_ul_req, &rrc_cmd_l2_req);
          
            /* Initialize the LLC command header first */
          
            rrc_meas_out_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
            rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_MCMR;
          
            rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER2_MAC_UL;
          
            /* Now fill the L2 cmd header */
            rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id = 
              TDSCMAC_UL_BLOCK_RACH_TRAFFIC_REQ;
          
            /* Set Block RACH indication */
              rrc_cmd_l2_req->block_rach = TRUE;
          }
          
          /* Send the Internal RRC Command - LLC should directly handle it */
          tdsrrc_put_int_cmd( rrc_meas_out_cmd_ptr );
          
          break;
        }  /* End of if for meas id found and block_rach is TRUE */
      } /* End of for */
      
    } /* End of if tdsrrcmeas_current_substate == CELL_FACH */
    
  } /* End of else */
  
  return; 

}


/*===========================================================================

FUNCTION          RRCTVM_SEND_MAC_CMD

DESCRIPTION 

    This function sends measurement requests to L1. 
    Since all MAC cmds go via LLC, the LLC command 
    TDSRRC_LLC_CMD_REQ actaully encapsules the real L1 
    command. This is the LLC command funneling mechanism.
    It frees allocated command and embedded buffers when failure 
    is detected before sending the command.

DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None.

===========================================================================*/

void tdsrrctvm_send_mac_cmd
(
tdsmac_ul_traffic_meas_config_type* traffic_vol_params_ptr
)
{
  /* Ptr which carries info about the command */
  tdsrrc_cmd_type        *rrc_meas_out_cmd_ptr = NULL;
  
  /* Now allocate memory */
  if ( (rrc_meas_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL )
  {
    ERR_FATAL("Memory Failure for MAC command",0,0,0);
    /* ERR_FATAL would take care of bringing down RRC */  
  }
  else
  {
    tdsl2_ul_req_cmd * rrc_cmd_l2_req = NULL;
    tdsrrc_get_pointer_l2_req_value(&rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.l2_ul_req, &rrc_cmd_l2_req);
  
    /* Initialize the LLC command header first */
  
    rrc_meas_out_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_MCMR;
  
    rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER2_MAC_UL;
  
    /* Now fill the L1 cmd header */
    rrc_meas_out_cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id = 
      TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ;
  
    /* Now allocate memory to the pointer inside the command */
    if ( (rrc_cmd_l2_req->mac_ul_traffic_vol_config = 
          (tdsmac_ul_traffic_meas_config_type *)tdsmac_ul_allocate_mem_ext_cmd
          (sizeof(tdsmac_ul_traffic_meas_config_type)))
    == NULL )
    {
      /* Note: Before removing this Error Fatal, make sure that the memory
       * being pointed to by rrc_meas_out_cmd_ptr above is freed.
       */
      ERR_FATAL("Unable to get MAC UL Cmd buffer",0,0,0);
    }
  
    /* Set Traffic Vol params */
    *(rrc_cmd_l2_req->mac_ul_traffic_vol_config) 
     = *traffic_vol_params_ptr;
  }
  
  
  /* Send the Internal RRC Command - LLC should directly handle it */
  tdsrrc_put_int_cmd( rrc_meas_out_cmd_ptr );

  /* Now send command to MAC to start measurements */
  TDSRRC_MSG0(MSG_LEGACY_LOW,"Sent Meas command to MAC");


}

#ifdef FEATURE_TDSCDMA_TVM_BLOCK
/*===========================================================================

FUNCTION          tdsrrctvm_send_block_tvm_cmd

DESCRIPTION 

    This function sends TDSCMAC_UL_TRAFFIC_VOLUME_BLOCK_REQ to MAC.

DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None.

===========================================================================*/

void tdsrrctvm_send_block_tvm_cmd
(
  uint16 block_time
)
{
  /* Ptr which carries info about the command */
  tdsrrc_cmd_type        *rrc_block_tvm_cmd_ptr = NULL;
  
  /* Now allocate memory */
  if ( (rrc_block_tvm_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL )
  {
    ERR_FATAL("Memory Failure for MAC command",0,0,0);
    /* ERR_FATAL would take care of bringing down RRC */  
  }
  else
  {
    tdsl2_ul_req_cmd * rrc_cmd_l2_req = NULL;
    tdsrrc_get_pointer_l2_req_value(&rrc_block_tvm_cmd_ptr->cmd.rrc_llc_req.l2_ul_req, &rrc_cmd_l2_req);
    
    /* Initialize the LLC command header first */
    rrc_block_tvm_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    rrc_block_tvm_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_MCMR;
  
    rrc_block_tvm_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER2_MAC_UL;
  
    /* Now fill the L1 cmd header */
    rrc_block_tvm_cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id = 
      TDSCMAC_UL_TRAFFIC_VOLUME_BLOCK_REQ;
    rrc_cmd_l2_req->block_time_interval = block_time;
  
  }
  
  /* Send the Internal RRC Command - LLC should directly handle it */
  tdsrrc_put_int_cmd( rrc_block_tvm_cmd_ptr );

  /* Now send command to MAC to start measurements */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent Meas block command to MAC with time %d",block_time);
}

/*===========================================================================

FUNCTION   tdsrrctvm_tvm_identity_exist

DESCRIPTION 

    Check if TVM measurement ID exist or not

DEPENDENCIES

    None

RETURN VALUE

    TRUE   -  Exist
    FALSE - Non-exist

SIDE EFFECTS

    None
===========================================================================*/

boolean tdsrrctvm_tvm_identity_exist
(
  void
)
{
  uint8 count = 0;
  boolean status = FALSE;

  for (count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
  {
    if (tdstvm_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM %d exist",tdstvm_meas_id_list[count].meas_id); 
      status = TRUE;
      return status;
    }
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"No TVM exist"); 
  return status; 
}
#endif /*FEATURE_TDSCDMA_TVM_BLOCK*/


/*===========================================================================

FUNCTION   RRCTVM_SET_MEAS_IDENTITY

DESCRIPTION 

    Set traffic volume meas identity variable from SIB information.

DEPENDENCIES

    None

RETURN VALUE

    None  

SIDE EFFECTS

    None
===========================================================================*/

void tdsrrctvm_set_meas_identity
(
uint16* meas_id_ptr,
tdsrrc_TrafficVolumeMeasSysInfo* sib_meas_ptr
)
{
  uint16 count = 0;
  /* First check if the meas id already exists */
  
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == *meas_id_ptr)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas Id = %d already existing",*meas_id_ptr);
      
      tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
      /* Check if validity is present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib_meas_ptr,
        tdsrrc_TrafficVolumeMeasSysInfo,measurementValidity))
      {
        tdstvm_meas_id_list[count].meas_validity = 
          TDSTVM_RET_MEAS_VALIDITY(sib_meas_ptr->measurementValidity.ue_State);
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:SIB12 Meas Validity is %d",tdstvm_meas_id_list[count].meas_validity);
        
      }

      return;
    }
    
  } /* End of for loop */ 
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Appending TVM meas identity");
  for (count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
  {
    if (tdstvm_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Setting Meas Id = %d in tdstvm_meas_id_list",*meas_id_ptr);        
      tdstvm_meas_id_list[count].meas_id =  (uint8) *meas_id_ptr;
      
      tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
      
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib_meas_ptr,
        tdsrrc_TrafficVolumeMeasSysInfo,measurementValidity))
      {
        tdstvm_meas_id_list[count].meas_validity = TDSTVM_RET_MEAS_VALIDITY(sib_meas_ptr->measurementValidity.ue_State);
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:SIB12 Meas Validity is %d",tdstvm_meas_id_list[count].meas_validity);
      }
      
      return;
    }
  }
  /* The following should never happen */
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Unable to set TVM Measurement Identity");
  return;

}


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_DCH_TO_FACH

DESCRIPTION 

  The main handler function that handles the processing of
  traffic volume measurements when RRC transitions from 
  DCH-->FACH. Check the MEASUREMENT_IDENTITY variable and
  determine for each measurement what has to be done as per
  its validity. The function accordingly updates the
  MEASUREMENT_IDENTITY variable and sends a command to
  MAC to reconfigure the traffic volume measurements.
  It also processes the scenario when no traffic volume 
  measurement has been assigned to the UE with a MEASUREMENT 
  CONTROL message that is valid in CELL_FACH state (stored 
  in the variable tdstvm_meas_id_list), that has the same identity 
  as the one indicated in the "Traffic volume measurement 
  system information
  

DEPENDENCIES

  None

RETURN VALUE

  None. 

SIDE EFFECTS

  If SIB11 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

void tdsrrctvm_process_dch_to_fach
(
 void
)
{
  uint8 count = 0, cnt = 0, addtl_deleted_cnt = 0;
   
  
  tdsmac_ul_traffic_meas_config_type tvm_cmd;
  
  uint16 tvm_meas_id;
  
  uint8 prim_meas_to_be_deleted_from_addtl_db[TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS];
  
  /* Initialize it */
  for (count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
    prim_meas_to_be_deleted_from_addtl_db[count] = TDSINVALID_MEAS_ID;
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Processing Traffic Vol");
  
  /* Check the Validity in Measurement Identity variable for Traffic Vol Meas */  
  for (count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
  {
    if (tdstvm_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID)
    {
      switch(tdstvm_meas_id_list[count].meas_validity)
      {
      case TDSNOT_PRESENT:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity absent. Delete Meas = %d",tdstvm_meas_id_list[count].meas_id);
        
          /* Build Delete Command and also got to read the 
        new meas from SIB11/12 */
        
        /* Cmd type */
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_DELETE;
        
        /* Meas Id */
        tvm_cmd.meas_id = tdstvm_meas_id_list[count].meas_id;
        
        /* Cnf not reqd */
        tvm_cmd.config_conf_ind = FALSE;
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Tell MAC to delete meas = %d ",tvm_cmd.meas_id);
        
        /* Set Meas Id in Meas Identity var to Invalid */
        tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        
        tdstvm_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        
        tdstvm_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
        
        tdstvm_meas_id_list[count].block_rach_incl = FALSE;
        
        /* Just delete all the addtl meas associated with this identity */
        for (cnt = 0; cnt <TDSMAX_ADDITIONAL_MEAS; cnt++ ) 
          tdstvm_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        
          /* Store the deleted prim meas in prim_meas_to_be_deleted_from_addtl_db
        so that it could be deleted from addtl meas db */
        for (addtl_deleted_cnt = 0; addtl_deleted_cnt <TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; 
        addtl_deleted_cnt++) 
        {
          if (prim_meas_to_be_deleted_from_addtl_db[addtl_deleted_cnt] == TDSINVALID_MEAS_ID)
          {
          /* Found the first position. Store it so that addtl meas fn could be called
            to delete it */
            prim_meas_to_be_deleted_from_addtl_db[addtl_deleted_cnt] = (uint8) tvm_cmd.meas_id;
            break; /* Come out of for */
          }
        }
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);
        
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Invoke tdsrrctvm_send_mac_cmd(..)");
        
        break;  /* Come out of switch and find if any other meas needs to be deleted */
        
      case TDSALL_STATES:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity ALL. Continue meas = %d",tdstvm_meas_id_list[count].meas_id);

     /* Since there are scenarios when MAC does not know about DCH to FACH
       trans and a meas could have both DCH and FACH object, so send MAC
       a explicit resume. In case MAC has stopped the meas, it resumes the meas
       with the new object. In case it is already doing, it ignores the cmd */
    
        tvm_cmd.meas_id = tdstvm_meas_id_list[count].meas_id;
        
        /* Ack not reqd */
        tvm_cmd.config_conf_ind = FALSE;
        
        /* Resume the Measurement */
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_RESUME;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);
        
        /* No Need to send cmd to MAC */
        break;
        
      case TDSSTATE_DCH:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity DCH only. Stop,Save meas= %d",tdstvm_meas_id_list[count].meas_id);
        
        /* Set Meas Id */
        tvm_cmd.meas_id = tdstvm_meas_id_list[count].meas_id;
        
        /* Ack not reqd */
        tvm_cmd.config_conf_ind = FALSE;
        
        /* Stop the Measurement */
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_SAVE;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);
        
        break;
        
      case TDSALL_STATES_BUT_DCH:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity all except DCH.Resume meas=%d",tdstvm_meas_id_list[count].meas_id);
        
        /* Set Meas Id */
        tvm_cmd.meas_id = tdstvm_meas_id_list[count].meas_id;
        
        /* Ack not reqd */
        tvm_cmd.config_conf_ind = FALSE;
        
        /* Resume the Measurement */
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_RESUME;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);
        
        break; 
        
      default:
        /* Not possible */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid meas validity in tdstvm_meas_id_list");
        return;
        
      } /* End of switch */
    } /* end of if */
    
  } /* End of for */
  
  /* Now check if any primary meas which was deleted earlier needs to be deleted 
  from addtl meas db */
  
  for (addtl_deleted_cnt = 0; addtl_deleted_cnt <TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; 
  addtl_deleted_cnt++) 
  {
    if (prim_meas_to_be_deleted_from_addtl_db[addtl_deleted_cnt] != TDSINVALID_MEAS_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_LOW,"Found prim meas %d to be deleted from addtl db",prim_meas_to_be_deleted_from_addtl_db[addtl_deleted_cnt]);
      
      /* Store in tvm_meas_id as fn expects uint16 */
      tvm_meas_id = prim_meas_to_be_deleted_from_addtl_db[addtl_deleted_cnt];
      
      /* Remove this meas from any additional meas DB*/
      tdsrrcmeas_delete_addtl_meas(&tvm_meas_id);
    }
  }
  
  if ((tvm_meas_id = tdsrrctvm_get_meas_id_from_sibs()) == TDSINVALID_MEAS_ID)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: SIB 11/12 not used");
  }
  else 
  {
  /* Find if any MCM is present in DB and has same id as one in tvm_meas_id  
    If yes, then cannot overwrite that with the one from SIB's. */
    for (count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
    {
      if ((tdstvm_meas_id_list[count].meas_id == tvm_meas_id) &&
        (tdstvm_meas_id_list[count].meas_ctrl_msg == FALSE))
      {
        /* Meas can be overwritten */
        
        /* Read new SIB's in tdsmac_sib_params */
        if (tdsrrctvm_get_sibs_in_connected_state() == TDSTVM_SUCCESS)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Send SETUP (SIB cmd) to MAC"); 
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unbale to read SIB for TVM");
          return;
        }
        
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas %d overwritten by new SIB",tdstvm_meas_id_list[count].meas_id);
        
        
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Send SETUP (SIB cmd) to MAC"); 
        
        tvm_cmd = tdsmac_sib_params;
        tvm_cmd.config_conf_ind = FALSE;
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);  
        
        /* Since there could be only one TVM MCM owned meas which has same id as the one
        specified by the new SIB11 or SIB12, so return */
        return;
        
      } /* End of if of meas_id as valid and MCM does not own the meas */
      else if ((tdstvm_meas_id_list[count].meas_id == tvm_meas_id) &&
        (tdstvm_meas_id_list[count].meas_ctrl_msg == TRUE))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:New SIB Meas Id %d == MCM Meas Id",tvm_meas_id);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:No new cmd to be sent to MAC");
        return;
      }
      else
      {
        /* Do Nothing */
      }
    } /* End of for of count of TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS */
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:No Meas in RRC DB matches SIB11/12 %d Meas",tvm_meas_id);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Configure MAC with new SIB11/12 Meas");
    if (tdsrrctvm_get_sibs_in_connected_state() == TDSTVM_SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Send SETUP (SIB cmd) to MAC"); 
      
      tvm_cmd = tdsmac_sib_params;
      tvm_cmd.config_conf_ind = FALSE;
      tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
      
      tdsrrctvm_send_mac_cmd(&tvm_cmd);  
    }    
  } /* End of else of SIB11/12 Meas Id being valid */
}


/*===========================================================================

FUNCTION   RRCTVM_GET_SIBS_IN_CONNECTED_STATE

DESCRIPTION 

  Processes the scenario when the transition from DCH-->FACH 
  transition determines that traffic volume measurement for the
  default measurement has to be overwritten by new SIB's from
  the new cell. So read the new SIB's for traffic volume measurements
  

DEPENDENCIES

  None

RETURN VALUE

  TDSTVM_SUCCESS if SIB 11/12 is successfully read for Traffic Volume
  TDSTVM_FAIL if SIB 11/12 is successfully read for Traffic Volume 

SIDE EFFECTS

  If SIB11 is not valid, then  traffic volume measurements may not be done.

===========================================================================*/

tdsrrctvm_status_e_type tdsrrctvm_get_sibs_in_connected_state
(
  void
)
{
  /* Declared pointer to SIB & SIB 12 */
  tdsrrc_SysInfoType11 *sib11_ptr = NULL; 
  
  tdsrrc_SysInfoType12 *sib12_ptr = NULL;
  
  /* Make SIB12 as absent */
    
  /* Reinitialize some key mac params */
  tdsrrctvm_initialize();
  
  /* Get traffic volume params from SIB11 */
  sib11_ptr = (tdsrrc_SysInfoType11*) 
    tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB11);
  
  /* Call this function which reads SIB11 */ 
  if ( sib11_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 11 empty."); 
    return TDSTVM_FAIL;
  }

  sib12_ptr = (tdsrrc_SysInfoType12*) 
    tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB12);

  /* SIB 12 present */
  if ((sib11_ptr->sib12indicator == TRUE) && (sib12_ptr != NULL))
  {
    tdssib12_present = TDSSIB12_PRESENT;
    
    /* Read traffic volume info from SIB12 and send meas req to MAC  */
    
    if (tdsrrctvm_process_sib_info(&sib12_ptr->measurementControlSysInfo)
      == TDSTVM_SUCCESS )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:SIB12 processed success"); 
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: Meas not done as per SIB 12");
      tdsrrctvm_initialize();
      return TDSTVM_FAIL;
    }
    
  } /* End of if of sib12 PRESENT */
  else
  {
    /* Get traffic volume params from SIB11 */
    
    /* Read traffic volume info from SIB12 and send meas req to MAC */
    if (tdsrrctvm_process_sib_info(&sib11_ptr->measurementControlSysInfo) 
      == TDSTVM_SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Successfully read from SIB11");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Not done as per SIB 11");
      tdsrrctvm_initialize();
      return TDSTVM_FAIL;
    }
  }
  return TDSTVM_SUCCESS; 

}


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_TVM_SIBS_IN_CONNECTED_STATE

DESCRIPTION 

  Invoked when cell reselection is done in CELL_FACH state
  and traffic volume measurements need to be updated as
  per the new SIB's. It reads the new traffic volume
  measurememt parameters from SIB11/12 and stores it in an
  internal data structure
 
DEPENDENCIES

  None

RETURN VALUE

  Returns TDSTVM_SUCCESS if SIB11/12 successfully read. 
  Returns TDSTVM_FAIL if SIB11/12 successfully read.

SIDE EFFECTS

  If SIB11/12 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

tdsrrctvm_status_e_type tdsrrctvm_process_tvm_sibs_in_connected_state
(
  void
)
{
  /* Declared pointer to SIB & SIB 12 */
  tdsrrc_SysInfoType11 *sib11_ptr = NULL; 
  
  tdsrrc_SysInfoType12 *sib12_ptr = NULL;
  
  /* Make SIB12 as absent */
   
  tdssib12_present = TDSSIB12_ABSENT;
  
  /* Get traffic volume params from SIB11 */
  sib11_ptr = (tdsrrc_SysInfoType11*) 
    tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB11);
  
  /* Call this function which reads SIB11 */ 
  if ( sib11_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 11 empty."); 
    return TDSTVM_FAIL;
  }

  sib12_ptr = (tdsrrc_SysInfoType12*) 
    tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB12);

  /* SIB 12 present */
  if ((sib11_ptr->sib12indicator == TRUE) && (sib12_ptr != NULL))
  {
    tdssib12_present = TDSSIB12_PRESENT;
    
    /* Read traffic volume info from SIB12 and send meas req to MAC  */
    
    /* Reinitialize some key mac params */
    tdsrrctvm_initialize();
    
    
    if (tdsrrctvm_process_sib_info(&sib12_ptr->measurementControlSysInfo)
      == TDSTVM_SUCCESS )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Successfully read from SIB12");
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Send cmd to MAC to Setup SIB Meas");
      tdsrrctvm_send_mac_cmd(&tdsmac_sib_params);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Meas not done as per SIB 12");
      tdsrrctvm_initialize(); 
      return TDSTVM_FAIL;
    }
    
  } /* End of if of sib12 PRESENT */
  else
  {
    /* Get traffic volume params from SIB11 */
    
    tdsrrctvm_initialize();
    
    /* Read traffic volume info from SIB12 and send meas req to MAC */
    if (tdsrrctvm_process_sib_info(&sib11_ptr->measurementControlSysInfo) 
      == TDSTVM_SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Successfully read from SIB11");
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"tvm:Send cmd to MAC to Setup SIB Meas");
      tdsrrctvm_send_mac_cmd(&tdsmac_sib_params);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Not done as per SIB 11");
      tdsrrctvm_initialize();
      return TDSTVM_FAIL;
    }
  }
  return TDSTVM_SUCCESS; 

}


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_TVM_ON_CELL_RESELECTION

DESCRIPTION 

  Main handler that processes traffic volume measurements on cell
  reselection in connected mode. Checks the MEASUREMENT_IDENTITY
  variable for any measurement that is owned by UTRAN.
  It saves that measurement and tells MAC to store it. If it does
  not find any measurement in the database that corresponds to the
  default measurement, it reads SIB11/12 again to get the default
  measurement.
 
DEPENDENCIES

  None

RETURN VALUE

  None. 

SIDE EFFECTS

  If SIB11/12 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

void tdsrrctvm_process_tvm_on_cell_reselection(void)
{
  uint8 count = 0;
  
  boolean meas_found = FALSE;
  
  /* For sending command to MAC */
  tdsmac_ul_traffic_meas_config_type traffic_vol_params;
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Processing on cell reselection");
  
  /* Check MEASUREMENT_IDENTITY variable for TVM */
  
  for (count=0; count <TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
  {
    if ((tdstvm_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID) &&
      (tdstvm_meas_id_list[count].meas_ctrl_msg == TRUE))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Found TVM = %d owned by MCM",tdstvm_meas_id_list[count].meas_id); 
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM: Meas %d to remain with UE",tdstvm_meas_id_list[count].meas_id);
      
      /* Set meas_found to TRUE as it helps in determining 
      if we need to clear tdstvm_meas_id_list */
      meas_found = TRUE;  
      break;
      
    }
    else
    {
      continue;
    } 
  } /* End of for */ 
  
  if (meas_found == FALSE)
  {
    /* Got to clean up tdstvm_meas_id_list and then read SIB11/12 again */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:No MCM owned TVM in M_Identity");
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Clean tdstvm_meas_id_list");
    
    for (count=0; count <TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
    {
      if ((tdstvm_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID) &&
        (tdstvm_meas_id_list[count].meas_ctrl_msg == FALSE))
      {
        /* Construct a cmd to tell MAC to delete all Measurements */
        traffic_vol_params.config_conf_ind = FALSE;
        
        traffic_vol_params.meas_id = tdstvm_meas_id_list[count].meas_id;
        traffic_vol_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_DELETE;
        
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending cmd to MAC to delete SIB Meas");
        
        tdsrrctvm_send_mac_cmd(&traffic_vol_params); 
        
        break;
      }
    }
    
    /* Now initialize tdsmac_sib_params and tdstvm_meas_id_list */
    tdsrrctvm_initialize(); 
    
    tdsrrctvm_initialize_measurement_identity();
    
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Read SIB11/12 for TVM");
    
    /* Read SIB's again. The fn sends cmd to MAC */
    if (tdsrrctvm_process_tvm_sibs_in_connected_state() == TDSTVM_SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Meas for new cell O.K");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Unable to configure TVM for new cell");
      /* Now initialize tdsmac_sib_params and tdstvm_meas_id_list */
      tdsrrctvm_initialize(); 
      
      /* Specifically make SIB meas in tdstvm_meas_id_list as TDSINVALID
      in case measurement identity has been put in tdstvm_meas_id_list */
      
      for (count=0; count <TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
      {
        if ((tdstvm_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID) &&
          (tdstvm_meas_id_list[count].meas_ctrl_msg == FALSE))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Make SIB11 Meas invalid %d",tdstvm_meas_id_list[count].meas_id);
          
          tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
          
          break;
        }
      }
    }    
  }
}


/*===========================================================================

FUNCTION   RRCTVM_INITIALIZE_MEASUREMENT_IDENTITY

DESCRIPTION 

  Handles initialization of MEASUREMNT_IDENTIY variable
  for Traffic Volume Measurements.
 
DEPENDENCIES

  None

RETURN VALUE

  None. 

SIDE EFFECTS

  If SIB11/12 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

void tdsrrctvm_initialize_measurement_identity
(
  void
)
{
  uint8 count = 0, cnt = 0;
  /* Initialize traffic volume Meas Id list */
  for (count=0; count <TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS ; count++)
  {
    tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
    tdstvm_meas_id_list[count].block_rach_incl = FALSE;
    tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
    tdstvm_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
    tdstvm_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
    
    /* Also initialize additional meas */
    for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
      tdstvm_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
    
  }
}


/*===========================================================================

FUNCTION   RRCTVM_GET_MEAS_ID_FROM_SIBS

DESCRIPTION 

  Gets the default traffic volume measurement identity by peeking
  into SIB11/12 of SIB database.
 
DEPENDENCIES

  None

RETURN VALUE

  Returns TDSDEFAULT_TRAFFIC_VOL_MEAS_IDENITY if SIB11/12 does not specify
  a measurement identity or return the measurememt identity specified
  in SIB11/12

  Returns TDSINVALID_MEAS_ID if traffic volume system information is not present 

SIDE EFFECTS

  If SIB11/12 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

uint16 tdsrrctvm_get_meas_id_from_sibs
(
  void
)
{
  /* Declared pointer to SIB & SIB 12 */
  tdsrrc_SysInfoType11 *sib11_ptr = NULL; 
  
  tdsrrc_SysInfoType12 *sib12_ptr = NULL;
  
  tdsrrc_MeasurementControlSysInfo* meas_ctrl_info_ptr;
  
  uint16 tdsmeas_id = TDSINVALID_MEAS_ID;
  
  /* Make SIB12 as absent */
    
  /* Reinitialize some key mac params */
  tdsrrctvm_initialize();
  
  /* Get traffic volume params from SIB11 */
  sib11_ptr = (tdsrrc_SysInfoType11*) 
    tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB11);
  
  /* Call this function which reads SIB11 */ 
  if ( sib11_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 11 empty."); 
    return tdsmeas_id;
  }
  else
  {
    meas_ctrl_info_ptr = & sib11_ptr->measurementControlSysInfo;
  }

  sib12_ptr = (tdsrrc_SysInfoType12*) 
    tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB12);

  /* SIB 12 present */
  if ((sib11_ptr->sib12indicator == TRUE) && (sib12_ptr != NULL))
  {
    meas_ctrl_info_ptr = &sib12_ptr->measurementControlSysInfo;
  }
  
  /* Check if Traffic volume meas identity present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(meas_ctrl_info_ptr,trafficVolumeMeasSysInfo))
  {
 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Meas Identity present");
    
      tdsmeas_id = (uint16)meas_ctrl_info_ptr->trafficVolumeMeasSysInfo.
      trafficVolumeMeasurementID;
      
    
  }
  return tdsmeas_id;
}



/*===========================================================================

FUNCTION   RRCTVM_PROCESS_FACH_TO_DCH

DESCRIPTION 

  The main handler function that handles the processing of
  traffic volume measurements when RRC transitions from 
  FACH-->DCH. Check the MEASUREMENT_IDENTITY variable and
  determine for each measurement what has to be done as per
  its validity. The function accordingly updates the
  MEASUREMENT_IDENTITY variable and sends a command to
  MAC to reconfigure the traffic volume measurements.
    

DEPENDENCIES

  None

RETURN VALUE

  None. 

SIDE EFFECTS

  If SIB11 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

void tdsrrctvm_process_fach_to_dch
(
  void
)
{ 
  uint8 count = 0;
  
  tdsmac_ul_traffic_meas_config_type tvm_cmd;
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Processing Traffic Vol for FACH-->DCH");
  
  /* Check the Validity in Measurement Identity variable for Traffic Vol Meas */
  
  for (count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
  {
    if (tdstvm_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID)
    {      
      switch(tdstvm_meas_id_list[count].meas_validity)
      {
      case TDSNOT_PRESENT:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity is absent. Delete Meas = %d",tdstvm_meas_id_list[count].meas_id);
        
        /* Build Delete Command and send to MAC */
        
        /* Cmd type */
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_DELETE;
        
        /* Meas Id */
        tvm_cmd.meas_id = tdstvm_meas_id_list[count].meas_id;
        
        /* Cnf not reqd */
        tvm_cmd.config_conf_ind = FALSE;
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Tell MAC to delete meas = %d ",tvm_cmd.meas_id);
        
        /* Set Meas Id in Meas Identity var to Invalid */
        tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        
        tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
        
        tdstvm_meas_id_list[count].block_rach_incl = FALSE;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);
        
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Invoke tdsrrctvm_send_mac_cmd(..)");
        
        break;
        
        
      case TDSALL_STATES:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity is ALL. Continue meas = %d",tdstvm_meas_id_list[count].meas_id);

    /* Since there are scenarios when MAC does not know about DCH to FACH
       trans and a meas could have both DCH and FACH object, so send MAC
       a explicit resume. In case MAC has stopped the meas, it resumes the meas
       with the new object. In case it is already doing, it ignores the cmd */

    /* Set Meas Id */
        tvm_cmd.meas_id = tdstvm_meas_id_list[count].meas_id;
        
        /* Ack not reqd */
        tvm_cmd.config_conf_ind = FALSE;
        
        /* Stop the Measurement */
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_RESUME;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);
        
        /* No Need to send cmd to MAC */
        break;
        
      case TDSSTATE_DCH:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity is DCH only. Resume meas= %d",tdstvm_meas_id_list[count].meas_id);
        
        /* Set Meas Id */
        tvm_cmd.meas_id = tdstvm_meas_id_list[count].meas_id;
        
        /* Ack not reqd */
        tvm_cmd.config_conf_ind = FALSE;
        
        /* Stop the Measurement */
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_RESUME;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);
        
        break;
        
      case TDSALL_STATES_BUT_DCH:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity is all except DCH. Stop,Save meas=%d",tdstvm_meas_id_list[count].meas_id);
        
        /* Set Meas Id */
        tvm_cmd.meas_id = tdstvm_meas_id_list[count].meas_id;
        
        /* Ack not reqd */
        tvm_cmd.config_conf_ind = FALSE;
        
        /* Resume the Measurement */
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_SAVE;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);
        
        break; 
        
      default:
        /* Not possible */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid meas validity in tdstvm_meas_id_list");
        return;
        
      } /* End of switch */
      
    } /* end of if */
    
  } /* End of for */
}
  

/*===========================================================================

FUNCTION   RRCTVM_INITIALIZE

DESCRIPTION 

    Initializes traffic volume configuration parameters.

DEPENDENCIES

    None

RETURN VALUE

    None  

SIDE EFFECTS

    None
===========================================================================*/

void tdsrrctvm_initialize
(
void
)
{ 
  /* Set default values to key params in tdsmac_sib_params */
  tdsmac_sib_params.meas_id = TDSINVALID_MEAS_ID;
  tdsmac_sib_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
  tdsmac_sib_params.meas_obj_incl = FALSE;
  tdsmac_sib_params.meas_obj_list.num_trch = 0;
  tdsmac_sib_params.meas_qty_incl = FALSE;
  tdsmac_sib_params.meas_report_incl = FALSE;
  tdsmac_sib_params.meas_report_qty.rlc_payload = FALSE;
  tdsmac_sib_params.meas_report_qty.avg_payload = FALSE;
  tdsmac_sib_params.meas_report_qty.var_payload = FALSE;
  tdsmac_sib_params.config_conf_ind = FALSE;
  tdsmac_sib_params.meas_criteria_incl = FALSE;
  tdsmac_sib_params.meas_report_criteria.periodic_cfg.report_infinity = TRUE;
  tdsmac_sib_params.meas_start_incl = TRUE;

}


void  tdsrrctvm_append_traffic_vol_results_ptr
(
   tdsrrc_MeasuredResults_LCR_r4 * measured_results_ptr,
   tdsmac_ul_traffic_meas_ind_type * additional_tvm
)
{         
    
  
  uint8 count = 0;
    
  /* Declare ptr for ease */
  
    tdsrrc_TrafficVolumeMeasuredResultsList * tvm_results_ptr = NULL;
  
    /* Set Choice to Traffic Volume Meas */
    TDSRRC_SET_COMMON_MSG_TYPE_PTR(measured_results_ptr, rrc_MeasuredResults_LCR_r4_trafficVolumeMeasuredResultsList);

       
     /* Assign this pointer to TV measured results */
    

     tvm_results_ptr = 
     rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_TrafficVolumeMeasuredResultsList);

    
     measured_results_ptr->u.trafficVolumeMeasuredResultsList=
        tvm_results_ptr;
    
    tvm_results_ptr->n = additional_tvm->num_rbs;
     ALLOC_ASN1ARRAY1(&tdsenc_ctxt,tvm_results_ptr,tdsrrc_TrafficVolumeMeasuredResults);
    
    if (tvm_results_ptr->elem == NULL)
    {
      ERR_FATAL("Failed to allocate memory",0,0,0);
    }
    
    for (count=0; count < additional_tvm->num_rbs; count++)
    {
      /* Get radio bearer Id */
      tvm_results_ptr->elem[count].rb_Identity = 
        (tdsrrc_RB_Identity)additional_tvm->rb_meas[count].rb_id;
      
      TDSRRC_RESET_MSG_IE_PRESENT(tvm_results_ptr->elem[count]);
      /* Now check the payload in each radio bearer */
      if (additional_tvm->rb_meas[count].raw_rlc_payload == TDSINVALID_PAYLOAD)
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Raw RLC payload not present");
      }
      else
      {
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(tvm_results_ptr->elem[count],
                                            rlc_BuffersPayload); 
        
        /* Convert the size */        
        tvm_results_ptr->elem[count].rlc_BuffersPayload = 
          TDSTVM_RET_RLC_BUFFER_PAYLOAD(additional_tvm->rb_meas[count].raw_rlc_payload);
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"TVM:RB = %d,Raw RLC payload sent = %d",additional_tvm->rb_meas[count].rb_id,additional_tvm->rb_meas[count].raw_rlc_payload);
        
      }
      if (additional_tvm->rb_meas[count].average_rlc_payload == TDSINVALID_PAYLOAD)
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Avge RLC payload not present");
      }
      else
      {
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
            tvm_results_ptr->elem[count],
            averageRLC_BufferPayload); 
        
        /* Convert the size in bytes */
        tvm_results_ptr->elem[count].averageRLC_BufferPayload = 
          TDSTVM_RET_AVG_BUFFER_PAYLOAD(additional_tvm->rb_meas[count].average_rlc_payload);
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"TVM:RB = %d,Avge RLC payload sent = %d",additional_tvm->rb_meas[count].rb_id,additional_tvm->rb_meas[count].average_rlc_payload);
        
      }
      if (additional_tvm->rb_meas[count].variance_rlc_payload == TDSINVALID_PAYLOAD)
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Var RLC payload not present");
      }
      else
      {
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
            tvm_results_ptr->elem[count],
            varianceOfRLC_BufferPayload);
        
        /* Convert the size in bytes */
        tvm_results_ptr->elem[count].varianceOfRLC_BufferPayload = 
          TDSTVM_RET_VAR_BUFFER_PAYLOAD(additional_tvm->rb_meas[count].variance_rlc_payload);
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"TVM:RB = %d,Var Payload = %ul",additional_tvm->rb_meas[count].rb_id,additional_tvm->rb_meas[count].variance_rlc_payload);
        
      }
      
      
      /* TVM additional results successfully appended */
      
    } /* End of for loop */
    
    

}
/*===========================================================================

FUNCTION   RRCTVM_APPEND_TRAFFIC_VOL_RESULTS

DESCRIPTION 
      
  This function appends Traffic Volume Measurements as additional measurements
  to any other Measurement Report 

DEPENDENCIES

  The called function appends the element at the end of 
  the list by allocating memory. The calling function must set
  the first element of the list to NULL if the list is empty

RETURN VALUE

  True
  False

SIDE EFFECTS

  None

===========================================================================*/

boolean tdsrrctvm_append_traffic_vol_results
(
 uint8* additional_meas_id_ptr,
 tdsmac_ul_traffic_meas_ind_type * additional_tvm
)
{
  /* Would store Additional Results from MAC */

  
  uint8 count = 0;
  
  boolean addtl_meas_present = FALSE;
  
  /* Declare ptr for ease */
  
  
  TDS_TASKLOCK();
  
  addtl_meas_present = tdsmac_get_additional_traffic_vol_msmt_results(*additional_meas_id_ptr, additional_tvm);
  
  TDS_TASKFREE();
  /* Call MAC function to append Traffic Volume Meas Results */
  if (addtl_meas_present == FALSE)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addl Traffic Vol Meas %d not appended",*additional_meas_id_ptr);
    return FALSE;
  }
  
  /* Check if RB's are not set to 0 */
  if ((additional_tvm->num_rbs == 0) || (additional_tvm->num_rbs >= (TDS_MAX_RB-1)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TVM:No of RB's for Addl TVM is %d",additional_tvm->num_rbs);
    return FALSE;
  }
  
  /* Validate the Report */
  for (count=0; count < additional_tvm->num_rbs; count++)
  {
    /* Get radio bearer Id */
    if (additional_tvm->rb_meas[count].rb_id > (TDS_MAX_RB-1))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TVM:RB Id %d invalid",additional_tvm->rb_meas[count].rb_id);
      return FALSE;
    }
  } /* End of for */
  
  return TRUE;

}

/*===========================================================================

FUNCTION   RRCTVM_PROCESS_MEAS_IDENTITY

DESCRIPTION 

    Set traffic volume meas identity variable from SIB information.

DEPENDENCIES

    None

RETURN VALUE

    None  

SIDE EFFECTS

    None
===========================================================================*/

void tdsrrctvm_process_mcm_identity
(
uint16* meas_id_ptr
)
{
  uint8 count = 0;
  /* First check if the meas id already exists */
  
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == *meas_id_ptr)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas Id = %d already existing",*meas_id_ptr);
      /* Set owner to MCM */
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      return;
    }
    
  } /* End of for loop */ 
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Appending TVM meas identity");
  for (count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
  {
    if (tdstvm_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Setting Meas Id = %d in tdstvm_meas_id_list",*meas_id_ptr);        
      tdstvm_meas_id_list[count].meas_id =  (uint8) *meas_id_ptr;
      
      /* Set owner to MCM */
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      
      return;
    }
  }
  /* The following should never happen */
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Unable to set TVM Measurement Identity");
  return;
  
}


/*===========================================================================

FUNCTION   RRCTVM_VALIDATE_ADDITIONAL_MEAS

DESCRIPTION 
      
  This function checks if additional measurements exists in RRC database and have the
  same validity as the primary measurement. If it
  exists, it returns its type else returns unknown Measurement which essentially
  implies that the additional measurement does not exist. The function also returns
  the reporting criteria as per the measurement type. The reporting criteria is used by
  the calling function to determine if UTRAN is trying to append an event triggered meas
    

DEPENDENCIES

  None. 

RETURN VALUE

  False if invalid. Meas Ctrl Failure already sent.

SIDE EFFECTS

  None

===========================================================================*/

boolean tdsrrctvm_validate_additional_meas
(
 tdsmeas_validity_enum_type* p_meas_validity,
 tdsrrc_AdditionalMeasurementID_List * additional_list_ptr,
 uint8 mcm_tr_id,
 uint32 mid
)
{
  uint8 count = 0;
  uint32 idx=0;
  tdsrrcmeas_layer_e_type additional_meas_type;
  
  /* Reporting Criteria */
  tdsrpt_crit_enum_type a_rpt_crit;
  
  uint16 additional_meas_id;
  
  /* Set validity to default value */
  tdsmeas_validity_enum_type addtl_meas_validity = TDSNOT_PRESENT;
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* Now compare if validity is the same */
  while (additional_list_ptr->n > idx)
  {
    additional_meas_id = (uint16) additional_list_ptr->elem[idx];
    
    /* First check if additional measurement exists in RRC DB or not */
    
    additional_meas_type = tdsrrcmeas_find_additional_meas_type(&additional_meas_id, &a_rpt_crit, &addtl_meas_validity);
      if (additional_meas_type == TDSQUALITY_MEAS)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Addtl QualityMeas %d,for PMeas %d not allowed",additional_meas_id,mid);
        /* Set transaction ID */ 
        rrcmeas_mcf.rrc_transaction_id = mcm_tr_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return FALSE;
      }
    if (additional_meas_type == TDSUNKNOWN_MEAS)
      
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Measurement Identity absent,ignore");
      idx++;
      continue;
    } /* End of if of tdsrrcmeas_find_additional_meas_type */
    
    /* Check if more than one meas of same type would be include with Prim Meas */
    if (additional_meas_type == TDSMAC_MEAS)
      
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"TVM:Addtl Meas %d PMeas of same type %d",additional_meas_id,mid);
      /* Set transaction ID */ 
      rrcmeas_mcf.rrc_transaction_id = mcm_tr_id;
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FALSE;
    } /* End of if of tdsrrcmeas_find_additional_meas_type */
    
    /* UTRAN might messup by making addtl meas as Event Triggered for
       Intra Freq, Inter Freq, Inter RAT */
    if (((additional_meas_type == TDSINTRA_FREQ) && (a_rpt_crit == TDSEVENT)) ||
        ((additional_meas_type == TDSINTER_FREQ) && (a_rpt_crit == TDSEVENT)) ||
        ((additional_meas_type == TDSINTER_SYS)  && (a_rpt_crit == TDSEVENT)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Addtl Meas Intra/InterF/InterS Event Trig,ignore");
      idx++;
      continue;
    }

    /* Get VALIDITY of primary measurement */
    
    /* Check validity of the additional and primary meas */
    /* For now, since additional is non traffic volume, 
    it is valid in CELL_DCH only */
    if (addtl_meas_validity == *p_meas_validity)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Validity Addtl & Primary Meas Match");
    }
    /* Possible as Traffic Vol is TDSALL_STATES and addtl is CELL_DCH */
    else if ((*p_meas_validity == TDSALL_STATES) || ((*p_meas_validity == TDSNOT_PRESENT) &&
      (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:P.Meas Validity is all states/Not Present");
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"TVM:Validity Addtl Meas %d,P Meas %d Differ",additional_meas_id,tdstvm_meas_id_list[count].meas_id);
      return FALSE;
    }
    
    /* Go to the next addtl Meas to be appended */
    idx++;
    
  } /* End of while */
  /* If we reach this stage, then additional meas are valid */
  return TRUE;  
}



/*===========================================================================

FUNCTION   RRCTVM_CONFIG_TVM_DB

DESCRIPTION 

 Sets Traffic Volume Meas Information in RRC Measurement Database.
 
DEPENDENCIES

 None

RETURN VALUE

 None  

SIDE EFFECTS

 None
===========================================================================*/

boolean tdsrrctvm_config_tvm_db
(
 tdsmac_ul_traffic_meas_config_type* mac_mcm_ptr,
 tdsmeas_validity_enum_type* p_meas_validity
)
{
  uint8 count = 0;
  /* First check if the meas id already exists */

  tdsmeas_validity_enum_type validity = TDSNOT_PRESENT;
  
  /* SETUP existing & MODIFY scenario */
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == mac_mcm_ptr->meas_id)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas Id = %d found in RRC db",mac_mcm_ptr->meas_id);
      
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      
      /* Check if it is a MODIFY cmd */
      if (mac_mcm_ptr->meas_cmd == TDSMAC_TRAFFIC_MEAS_MODIFY)
      {
        /* Reporting Criteria for MODIFY */
        /* meas_criteria_incl is set to TRUE only if reporting crit is specified */
        if (mac_mcm_ptr->meas_criteria_incl)
        {
          tdstvm_meas_id_list[count].rpt_crit = TDSTVM_RPT_CRIT(mac_mcm_ptr->meas_report_mode.report_mode);
          
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas Crit %d saved in RRC",tdstvm_meas_id_list[count].rpt_crit);
          
          /* If for MODIFY, Validity is not present, then copy it from Meas DB */
          if (*p_meas_validity != validity)
          {
            tdstvm_meas_id_list[count].meas_validity = *p_meas_validity;
          }
        } /* End of if of meas_criteria_incl */
      } /* End of MODIFY */
      else /* SETUP case */
      {
        tdstvm_meas_id_list[count].rpt_crit = 
          TDSTVM_RPT_CRIT(mac_mcm_ptr->meas_report_mode.report_mode);
        
          tdstvm_meas_id_list[count].meas_validity = *p_meas_validity;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Validity %d saved in RRC DB",*p_meas_validity);
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas Crit %d saved in RRC",tdstvm_meas_id_list[count].rpt_crit);
      }
      
      return TRUE;
    } /* End of if of Meas Id found */
    
  } /* End of for loop */ 
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:New Meas Id = %d configured. Set params",mac_mcm_ptr->meas_id);
  
  /* SETUP not existing scenario */
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    /* If Meas Id not found, then it is a new Meas. Set it in first available position */
    if (tdstvm_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Setting Meas Id = %d in RRC db",mac_mcm_ptr->meas_id);
      
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      
      tdstvm_meas_id_list[count].meas_id = (uint8) mac_mcm_ptr->meas_id;
      
      /* Set Validity to CELL_DCH */
      tdstvm_meas_id_list[count].meas_validity = *p_meas_validity;
      
      tdstvm_meas_id_list[count].rpt_crit = TDSTVM_RPT_CRIT(mac_mcm_ptr->meas_report_mode.report_mode);
      
      return TRUE;
    }
    
  } /* End of for loop */ 
  
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:RRC MeasDB for TVM FULL!!");
  return FALSE;

}


/*===========================================================================

FUNCTION   RRCTVM_SET_REPORTING_CRITERIA

DESCRIPTION 
            
    This function sets reporting criteria in tdstvm_meas_id_list from SIB's
    

DEPENDENCIES

    None 

RETURN VALUE

    None

SIDE EFFECTS

    None

===========================================================================*/

void tdsrrctvm_set_reporting_criteria
(
  uint16* meas_id_ptr,
  tdsrrc_TrafficVolumeReportCriteriaSysInfo* sib_rpt_crit_ptr
)
{
  uint8 count = 0;
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == *meas_id_ptr)
    {
      /* In SIB's, addtl meas cannot be configured */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE2_PTR(sib_rpt_crit_ptr,
        rrc_TrafficVolumeReportCriteriaSysInfo,trafficVolumeReportingCriteria))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:SIB:Event Triggered Crit");
        tdstvm_meas_id_list[count].rpt_crit = TDSEVENT;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:SIB:Periodic Crit");
        tdstvm_meas_id_list[count].rpt_crit = TDSPERIODIC;
      }
      break;
    }
  } /* End of for */

}


/*===========================================================================

FUNCTION  RRCTVM_PROCESS_SIB_CHANGE_NOTIFICATION

DESCRIPTION  

 This function processes SIB change notification to modify TVM 

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrctvm_process_sib_change_notification
(
 tdsrrc_sib_change_mask_type sib_change_mask
)
{
  uint16 tvm_meas_id = TDSINVALID_MEAS_ID;
  
  uint32 count = 0;
  
  boolean tvm_sib_valid = FALSE;
  
  tdsmac_ul_traffic_meas_config_type tvm_cmd;
  
  /* Find if it is SIB11 or SIB12 */
  if (sib_change_mask & TDSSIB_11_BIT)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:SIB11 changed");
  }
  if(sib_change_mask & TDSSIB_12_BIT)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:SIB12 changed");
  }
  /* First do certain checks before processing it.
  Find if default meas identity or other meas identity
  exists in SIB12 or SIB11. If not, then the fn returns
  TDSINVALID_MEAS_ID. */ 
  
  if ((tvm_meas_id = tdsrrctvm_get_meas_id_from_sibs()) == TDSINVALID_MEAS_ID)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: SIB 11/12 not used");
  }
  else 
  {
    /* Read new SIB's in tdsmac_sib_params */
    if (tdsrrctvm_get_sibs_in_connected_state() == TDSTVM_SUCCESS)
    {
      tvm_sib_valid = TRUE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Send SETUP (SIB cmd) to MAC"); 
      
      /* Find if any MCM is present in DB and has same id as one in tvm_meas_id (New SIB Mid) 
      If yes, then cannot overwrite that with the one from SIB's. */
      for (count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++)
      {
        if ((tdstvm_meas_id_list[count].meas_id == tvm_meas_id) &&
          (tdstvm_meas_id_list[count].meas_ctrl_msg == FALSE))
        {
          /* Meas can be overwritten */
          
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Meas %d overwritten by new SIB",tdstvm_meas_id_list[count].meas_id);
          
          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Send SETUP (SIB cmd) to MAC"); 
          
          tvm_cmd = tdsmac_sib_params;
          tvm_cmd.config_conf_ind = FALSE;
          tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
          
          tdsrrctvm_send_mac_cmd(&tvm_cmd);  
          
          /* Since there could be only one TVM MCM owned meas which has same id as the one
          specified by the new SIB11 or SIB12, so return */
          return;
          
        } /* End of if of meas_id as valid and MCM does not own the meas */
        else if ((tdstvm_meas_id_list[count].meas_id == tvm_meas_id) &&
          (tdstvm_meas_id_list[count].meas_ctrl_msg == TRUE))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:New SIB Meas Id %d == MCM Meas Id",tvm_meas_id);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:No new cmd to be sent to MAC");
          return;
        }
        else
        {
          /* Do Nothing */
        }
      } /* End of for of count of TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS */
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:No Meas in RRC DB matches SIB11/12 %d Meas",tvm_meas_id);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Configure MAC with new SIB11/12 Meas");
      if (tvm_sib_valid == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Send SETUP (SIB cmd) to MAC"); 
        
        tvm_cmd = tdsmac_sib_params;
        tvm_cmd.config_conf_ind = FALSE;
        tvm_cmd.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
        
        tdsrrctvm_send_mac_cmd(&tvm_cmd);  
      }
      
    } /* End of if of SIB's valid in connected mode */
    
  } /* End of else of SIB11/12 Meas Id being valid */

}

/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R6

DESCRIPTION  

    Process traffic volume information in a Meas
    Ctrl Message R6. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r6
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
)
{
  uint8 count = 0, cnt = 0, trch_cnt = 0;

  tdsrrc_MeasurementControl_r6_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 transaction_identifier = 0;

  /* Declare MCM config structure that goes to MAC */
  tdsmac_ul_traffic_meas_config_type  mac_mcm_params = {0};
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Variable defined to check if addtl present in MCM is valid
  This varibale is set to TRUE if addtl meas is valid. True means
  in setting addtl meas in RRC db */
  boolean addtl_meas_validation = FALSE;
  
  tdsmeas_validity_enum_type prim_meas_validity = TDSNOT_PRESENT;
  
  tdsrrcmeas_layer_e_type meas_type = TDSMAC_MEAS;
  
  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;
  
  boolean block_set = FALSE;
  /* Assign some default values */
  mac_mcm_params.meas_obj_incl = FALSE;
  mac_mcm_params.meas_qty_incl = FALSE;
  mac_mcm_params.meas_report_incl = FALSE;
  mac_mcm_params.meas_criteria_incl = FALSE;  
  mac_mcm_params.meas_start_incl = TRUE;
  mac_mcm_params.meas_obj_list.num_trch = 0;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.r6.measurementControl_r6;

  /* Store the Transaction Identifier */
  transaction_identifier = tid;
  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r6_setup)
  {
    /* Check if meas quantity and reporting quantity received. If either of them
    * not received, then configuration incomplete */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement, 
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement, 
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeReportingQuantity)))
    {
    /* If Reporting quantity indicates var or average but measurement quantity
      * does not indicate time interval, then configuration incomplete */
      if ((rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadAverage == TRUE) ||
        (rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadVariance == TRUE))
      {
        /* Check if meas quantity is equal to RAW. If yes, then ERROR */
        if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
          trafficVolumeMeasQuantity,rrc_TrafficVolumeMeasQuantity,rlc_BufferPayload))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:R.Quan = Var/Avge,M.Quan= Raw");
          
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          /* Send Meas Ctrl Failure Message */
          
          return TDSTVM_FAIL;
        }
        
      }
      
      /* Now check if reporting mode present */
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode)))
      {
        /* Meas reporting Mode absent. Send Meas Ctrl Failure */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of if Meas Quan, Reporting Quan present */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MeasQuantity or ReportingQuantity absent");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
    }
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    
    /* Time to process the actual parameters */
    if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
      u.setup->u.trafficVolumeMeasurement,&mac_mcm_params)
      == TDSTVM_FAIL)
    {
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
      
      /* Set Error Cause */
      rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
    }
    
    /* Check if Meas Validity PRESENT */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,measurementValidity))
    {
      prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
        rrc_mcm_ptr->measurementCommand.u.setup->u.
        trafficVolumeMeasurement->measurementValidity.ue_State);
    }
    
    if (((prim_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
      ((prim_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: RRC val,MCM val differ");
      mac_mcm_params.meas_start_incl = FALSE;
    }

    if((prim_meas_validity == TDSALL_STATES) && (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.
      trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasurementObjectList))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas validity == all states,but no tvm object");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf); 
      return TDSTVM_FAIL;
    }
 
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
      /* Call fn which makes check for additional measurements */
      if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
        transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return TDSTVM_FAIL;
      }
      
      /* This means additional meas has been successfully validated */
      addtl_meas_validation = TRUE;
      
    } /* End of if of Additional Meas Present */
    
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    /* To read traffic volume reporting mode */
    mac_mcm_params.meas_report_mode.transfer_mode = 
      TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
      measurementReportingMode.measurementReportTransferMode);

    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_validation, TRUE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
      
    }
    
    /* Set Measurement Identity in tdstvm_meas_id_list for SETUP */
    tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
    
    /* Set parameters in RRC DB */
    if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
    {
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
      
    }
    /* Store additional meas */
    /* Check if Additional Meas has been validated successfully. */
    /* If yes, then store all additional meas in RRC DB */
    if (addtl_meas_validation == TRUE) 
    {
      tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
    }
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r6_modify)
  {
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_MODIFY;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode))
    {
      /* To read traffic volume reporting mode */
      mac_mcm_params.meas_report_mode.transfer_mode = 
        TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
        measurementReportingMode.measurementReportTransferMode);  
      
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r6_modify,measurementType))
    {
      if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType,
        rrc_MeasurementType_r6_trafficVolumeMeasurement)))
      {
      /* If one comes here, then it means that meas id earlier determined
      * by RRC as traffic volume is actually not Traffic Volume. Send
        * Meas Ctrl failure Msg to UTRAN */
        
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Meas Id is not Traffic Volume");
        /* Send Meas Ctrl Failure */
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
      
      /* Time to process the actual parameters */
      if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
        u.modify->measurementType.u.trafficVolumeMeasurement,
        &mac_mcm_params) == TDSTVM_SUCCESS)
      {
        
        /* Check if Meas Validity PRESENT */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          trafficVolumeMeasurement,
          tdsrrc_TrafficVolumeMeasurement,measurementValidity))
        {
          prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
           rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
            trafficVolumeMeasurement->measurementValidity.ue_State);
        }

        /* Process Additional Meas */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,
            additionalMeasurementList))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

          if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
            transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
          {
            /* Called function has already sent Meas Ctrl Failure */
            return TDSTVM_FAIL;
          }
          
          /* This means additional meas has been successfully validated */
          addtl_meas_validation = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
          
            rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
          
            /* Send MCF msg to UTRAN */  
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
            return TDSTVM_FAIL;
          
          }
        
        } /* End of if of Additional Meas Present */
        

        tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
        
        if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
        {
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          /* Send MCF msg to UTRAN */  
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          
          return TDSTVM_FAIL;
          
        }

        /* Store additional meas */
        /* Check if Additional Meas has been validated successfully. */
        /* If yes, then store all additional meas in RRC DB */
        if (addtl_meas_validation == TRUE) 
        {
          tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
        }
        
        
      } /* End if if of Traffic Volume params successfully read */
      else
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
        
        /* Set Error Cause */
        rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of If of meas_type present */
    else
    {
      /* Set Measurement Identity in tdstvm_meas_id_list for MODIFY */
      tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);

      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

        if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
          transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return TDSTVM_FAIL;
        }
        
        /* This means additional meas has been successfully validated */
        addtl_meas_validation = TRUE;
        
      } /* End of if of Additional Meas Present */
      
      if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
        addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
      {
        /* As UTRAN tried to configure more than 4 Addtl Meas */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
        
      }
      
      
      if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
      {
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        
        return TDSTVM_FAIL;
        
      }

      /* Store additional meas */
      /* Check if Additional Meas has been validated successfully. */
      /* If yes, then store all additional meas in RRC DB */
      if (addtl_meas_validation == TRUE) 
      {
        tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
      }
    }
    
    
  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdstvm_meas_id_list */
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_DELETE;
    
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&mac_mcm_params.meas_id);
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
    {
      if (tdstvm_meas_id_list[count].meas_id == 
        rrc_mcm_ptr->measurementIdentity)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Delete:Remove Meas Id");
        
        tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        
        tdstvm_meas_id_list[count].block_rach_incl = FALSE;
        
        tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
        
        tdstvm_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        
        tdstvm_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++) 
        {
          tdstvm_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        
        break;  /* Come out of for */
      } /* End of if */
      
    }
    alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
    mac_mcm_params.config_ack_num = alloc_idx;
    tdsrrctvm_send_mac_cmd(&mac_mcm_params);
    
    return TDSTVM_SUCCESS;
    
  }
  
  /* Time to store meas validity for SETUP or Modify 
  It has already been verified that the Measurement exists */ 
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == 
      rrc_mcm_ptr->measurementIdentity)
    {
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      
      /* This means that meas would be saved on trans from DCH to FACH */
      /* Change ownership to Meas Ctrl Msg */      
      
      if (mac_mcm_params.meas_report_mode.report_mode == 
        TDSEVENT_TRIGGERED_MODE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"trch_info_incl = %d",mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl);
        if (mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl == TRUE)
        {
          for (trch_cnt = 0; trch_cnt < mac_mcm_params.meas_report_criteria.event_cfg.num_trch; trch_cnt++)
          {
            for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].num_events; cnt++)
            {
              if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].event_id == TDSEVENT_4A) &&
                (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
              {
                tdstvm_meas_id_list[count].block_rach_incl = TRUE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
                block_set = TRUE;
                break; 
              } /* End of if of event_id and tx_interrupt_tmr check */
            } /* End of for */
            if (block_set == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
              break;
            }
          } /* end of 2nd for */
        }
        else
        {
          /* Keep up with MAC, if trch_info_incl = FALSE, only get event info from first element */
          for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].num_events; cnt++)
          {
            if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].event_id == TDSEVENT_4A) &&
              (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
            {
              tdstvm_meas_id_list[count].block_rach_incl = TRUE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
              
              block_set = TRUE;
              break; 
            } /* End of if of event_id and tx_interrupt_tmr check */
          } /* End of for */
          if (block_set == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
            break;
          }
        }
        break;
      } /* End of if for Event Triggered Mode */
      break;
    } /* End of if */
    
  } /* End of for loop */
  
  
  /* Now send config command to MAC */
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
  mac_mcm_params.config_ack_num = alloc_idx;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending rrctvm_send_mac_command");
  tdsrrctvm_send_mac_cmd(&mac_mcm_params);
  return TDSTVM_SUCCESS;
}

/*===========================================================================

FUNCTION   tdsrrctvm_is_traffic_volume_indicator_to_be_set_in_cu

DESCRIPTION 
  This function checks if there is any TVM with measurement ID 4, if that measurement is event trigerred
  and valid in all states or all states except DCH, if any present calls the mac function to check if the
  traffic volume is above the configured threshold for event 4A for that measurement

DEPENDENCIES
  None

RETURN VALUE
  TRUE  if there is any TVM with measurement ID 4, if that measurement is event trigerred and valid in 
        all states or all states except DCH, if any present calls the mac function and if traffic volume
        is above the configured threshold for event 4A for that measurement
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrctvm_is_traffic_volume_indicator_to_be_set_in_cu
(
  void
)
{
  int i=0;
  boolean status = FALSE;

  
  for(i=0;i<TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS;i++)
  {
    /* 25.331 
    if the TCTV is larger than the threshold in the IE "Reporting threshold" for a traffic volume measurement stored in the 
    MEASUREMENT_IDENTITY variable and that traffic volume measurement has "measurement identity" equal to 4, 
    "Traffic volume event identity" equal to "4a", "Measurement validity" equal to "all states" or "all states except CELL_DCH";
    */
    if(tdstvm_meas_id_list[i].meas_id == 4) 
    {
      if((tdstvm_meas_id_list[i].rpt_crit == TDSEVENT) && 
        (tdstvm_meas_id_list[i].meas_validity == TDSALL_STATES || tdstvm_meas_id_list[i].meas_validity == TDSALL_STATES_BUT_DCH))
      {
        /*Call the MAC function to check the threshold and the event identity*/
        TDS_TASKLOCK();
        status = tdsmac_compute_traffic_volume_above_threshold_cu(4,tdsrrcllc_get_rach_cctrch_ptr());
        TDS_TASKFREE();
        if(status)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: Set traffic volume indicator in CU");
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: Threshold is not exceeded");
        }
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"TVM: Meas id is 4 report criteria is %d  and validity is %d",tdstvm_meas_id_list[i].rpt_crit,tdstvm_meas_id_list[i].meas_validity);
        status = FALSE;
      }
      break;
    }
  }
  return status;
}

/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R7

DESCRIPTION  

    Process traffic volume information in a Meas
    Ctrl Message R7. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r7
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
)
{
  uint8 count = 0, cnt = 0, trch_cnt = 0;

  tdsrrc_MeasurementControl_r7_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 transaction_identifier = 0;

  /* Declare MCM config structure that goes to MAC */
  tdsmac_ul_traffic_meas_config_type  mac_mcm_params = {0};
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Variable defined to check if addtl present in MCM is valid
  This varibale is set to TRUE if addtl meas is valid. True means
  in setting addtl meas in RRC db */
  boolean addtl_meas_validation = FALSE;
  
  tdsmeas_validity_enum_type prim_meas_validity = TDSNOT_PRESENT;
  
  tdsrrcmeas_layer_e_type meas_type = TDSMAC_MEAS;
  
  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  boolean block_set = FALSE;
  /* Assign some default values */
  mac_mcm_params.meas_obj_incl = FALSE;
  mac_mcm_params.meas_qty_incl = FALSE;
  mac_mcm_params.meas_report_incl = FALSE;
  mac_mcm_params.meas_criteria_incl = FALSE;  
  mac_mcm_params.meas_start_incl = TRUE;
  mac_mcm_params.meas_obj_list.num_trch = 0;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.r7.measurementControl_r7;

  /* Store the Transaction Identifier */
  transaction_identifier = tid;
  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r7_setup)
  {
    /* Check if meas quantity and reporting quantity received. If either of them
    * not received, then configuration incomplete */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeReportingQuantity)))
    {
    /* If Reporting quantity indicates var or average but measurement quantity
      * does not indicate time interval, then configuration incomplete */
      if ((rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadAverage == TRUE) ||
        (rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadVariance == TRUE))
      {
        /* Check if meas quantity is equal to RAW. If yes, then ERROR */
        if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
          trafficVolumeMeasQuantity,rrc_TrafficVolumeMeasQuantity,rlc_BufferPayload))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:R.Quan = Var/Avge,M.Quan= Raw");
          
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          /* Send Meas Ctrl Failure Message */
          
          return TDSTVM_FAIL;
        }
        
      }
      
      /* Now check if reporting mode present */
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode)))
      {
        /* Meas reporting Mode absent. Send Meas Ctrl Failure */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of if Meas Quan, Reporting Quan present */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MeasQuantity or ReportingQuantity absent");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
    }
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    
    /* Time to process the actual parameters */
    if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
      u.setup->u.trafficVolumeMeasurement,&mac_mcm_params)
      == TDSTVM_FAIL)
    {
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
      
      /* Set Error Cause */
      rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
    }
    
    /* Check if Meas Validity PRESENT */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,measurementValidity))
    {
      prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
        rrc_mcm_ptr->measurementCommand.u.setup->u.
        trafficVolumeMeasurement->measurementValidity.ue_State);
    }
    
    if (((prim_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
      ((prim_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: RRC val,MCM val differ");
      mac_mcm_params.meas_start_incl = FALSE;
    }

    if((prim_meas_validity == TDSALL_STATES) && (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.
      trafficVolumeMeasurement, 
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasurementObjectList))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas validity == all states,but no tvm object");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf); 
      return TDSTVM_FAIL;
    }
 
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
      /* Call fn which makes check for additional measurements */
      if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
        transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return TDSTVM_FAIL;
      }
      
      /* This means additional meas has been successfully validated */
      addtl_meas_validation = TRUE;
      
    } /* End of if of Additional Meas Present */
    
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    /* To read traffic volume reporting mode */
    mac_mcm_params.meas_report_mode.transfer_mode = 
      TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
      measurementReportingMode.measurementReportTransferMode);

    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_validation, TRUE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
      
    }
    
    /* Set Measurement Identity in tdstvm_meas_id_list for SETUP */
    tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
    
    /* Set parameters in RRC DB */
    if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
    {
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
      
    }
    /* Store additional meas */
    /* Check if Additional Meas has been validated successfully. */
    /* If yes, then store all additional meas in RRC DB */
    if (addtl_meas_validation == TRUE) 
    {
      tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
    }
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r7_modify)
  {
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_MODIFY;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode))
    {
      /* To read traffic volume reporting mode */
      mac_mcm_params.meas_report_mode.transfer_mode = 
        TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
        measurementReportingMode.measurementReportTransferMode);  
      
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r7_modify,measurementType))
    {
      if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType,
        rrc_MeasurementType_r7_trafficVolumeMeasurement)))
      {
      /* If one comes here, then it means that meas id earlier determined
      * by RRC as traffic volume is actually not Traffic Volume. Send
        * Meas Ctrl failure Msg to UTRAN */
        
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Meas Id is not Traffic Volume");
        /* Send Meas Ctrl Failure */
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
      
      /* Time to process the actual parameters */
      if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
        u.modify->measurementType.u.trafficVolumeMeasurement,
        &mac_mcm_params) == TDSTVM_SUCCESS)
      {
        
        /* Check if Meas Validity PRESENT */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          trafficVolumeMeasurement,
          tdsrrc_TrafficVolumeMeasurement,measurementValidity))
        {
          prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
           rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
            trafficVolumeMeasurement->measurementValidity.ue_State);
        }

        /* Process Additional Meas */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

          if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
            transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
          {
            /* Called function has already sent Meas Ctrl Failure */
            return TDSTVM_FAIL;
          }
          
          /* This means additional meas has been successfully validated */
          addtl_meas_validation = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
          
            rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
          
            /* Send MCF msg to UTRAN */  
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
            return TDSTVM_FAIL;
          
          }
        
        } /* End of if of Additional Meas Present */
        

        tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
        
        if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
        {
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          /* Send MCF msg to UTRAN */  
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          
          return TDSTVM_FAIL;
          
        }

        /* Store additional meas */
        /* Check if Additional Meas has been validated successfully. */
        /* If yes, then store all additional meas in RRC DB */
        if (addtl_meas_validation == TRUE) 
        {
          tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
        }
        
        
      } /* End if if of Traffic Volume params successfully read */
      else
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
        
        /* Set Error Cause */
        rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of If of meas_type present */
    else
    {
      /* Set Measurement Identity in tdstvm_meas_id_list for MODIFY */
      tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);

      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

        if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
          transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return TDSTVM_FAIL;
        }
        
        /* This means additional meas has been successfully validated */
        addtl_meas_validation = TRUE;
        
      } /* End of if of Additional Meas Present */
      
      if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
        addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
      {
        /* As UTRAN tried to configure more than 4 Addtl Meas */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
        
      }
      
      
      if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
      {
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        
        return TDSTVM_FAIL;
        
      }

      /* Store additional meas */
      /* Check if Additional Meas has been validated successfully. */
      /* If yes, then store all additional meas in RRC DB */
      if (addtl_meas_validation == TRUE) 
      {
        tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
      }
    }
    
    
  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdstvm_meas_id_list */
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_DELETE;
    
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&mac_mcm_params.meas_id);
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
    {
      if (tdstvm_meas_id_list[count].meas_id == 
        rrc_mcm_ptr->measurementIdentity)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Delete:Remove Meas Id");
        
        tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        
        tdstvm_meas_id_list[count].block_rach_incl = FALSE;
        
        tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
        
        tdstvm_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        
        tdstvm_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++) 
        {
          tdstvm_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        
        break;  /* Come out of for */
      } /* End of if */
      
    }
    
    alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
    mac_mcm_params.config_ack_num = alloc_idx;

    tdsrrctvm_send_mac_cmd(&mac_mcm_params);
    
    return TDSTVM_SUCCESS;
    
  }
  
  /* Time to store meas validity for SETUP or Modify 
  It has already been verified that the Measurement exists */ 
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == 
      rrc_mcm_ptr->measurementIdentity)
    {
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      
      /* This means that meas would be saved on trans from DCH to FACH */
      /* Change ownership to Meas Ctrl Msg */      
      
      if (mac_mcm_params.meas_report_mode.report_mode == 
        TDSEVENT_TRIGGERED_MODE) 
        
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"trch_info_incl = %d",mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl);
        if (mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl == TRUE)
        {
          for (trch_cnt = 0; trch_cnt < mac_mcm_params.meas_report_criteria.event_cfg.num_trch; trch_cnt++)
          {
            for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].num_events; cnt++)
            {
              if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].event_id == TDSEVENT_4A) &&
                (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
              {
                tdstvm_meas_id_list[count].block_rach_incl = TRUE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
                
                block_set = TRUE;
                break; 
              } /* End of if of event_id and tx_interrupt_tmr check */
            } /* End of for */
            if (block_set == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
              break;
            }
          } /* end of 2nd for */
        }
        else
        {
          /* Keep up with MAC, if trch_info_incl = FALSE, only get event info from first element */
          for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].num_events; cnt++)
          {
            if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].event_id == TDSEVENT_4A) &&
              (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
            {
              tdstvm_meas_id_list[count].block_rach_incl = TRUE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
              
              block_set = TRUE;
              break; 
            } /* End of if of event_id and tx_interrupt_tmr check */
          } /* End of for */
          if (block_set == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
            break;
          }
        }
        break;
      } /* End of if for Event Triggered Mode */
      break;
    } /* End of if */
    
  } /* End of for loop */
  
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
  mac_mcm_params.config_ack_num = alloc_idx;

  /* Now send config command to MAC */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending rrctvm_send_mac_command");
  tdsrrctvm_send_mac_cmd(&mac_mcm_params);
  return TDSTVM_SUCCESS;
}

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R8

DESCRIPTION  

    Process traffic volume information in a Meas
    Ctrl Message R8. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r8
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
)
{
  uint8 count = 0, cnt = 0, trch_cnt = 0;

  tdsrrc_MeasurementControl_r8_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 transaction_identifier = 0;

  /* Declare MCM config structure that goes to MAC */
  tdsmac_ul_traffic_meas_config_type  mac_mcm_params = {0};
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Variable defined to check if addtl present in MCM is valid
  This varibale is set to TRUE if addtl meas is valid. True means
  in setting addtl meas in RRC db */
  boolean addtl_meas_validation = FALSE;
  
  tdsmeas_validity_enum_type prim_meas_validity = TDSNOT_PRESENT;
  
  tdsrrcmeas_layer_e_type meas_type = TDSMAC_MEAS;
  
  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  boolean block_set = FALSE;
  /* Assign some default values */
  mac_mcm_params.meas_obj_incl = FALSE;
  mac_mcm_params.meas_qty_incl = FALSE;
  mac_mcm_params.meas_report_incl = FALSE;
  mac_mcm_params.meas_criteria_incl = FALSE;  
  mac_mcm_params.meas_start_incl = TRUE;
  mac_mcm_params.meas_obj_list.num_trch = 0;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.measurementControl_r8;

  /* Store the Transaction Identifier */
  transaction_identifier = tid;
  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r8_setup)
  {
    /* Check if meas quantity and reporting quantity received. If either of them
    * not received, then configuration incomplete */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeReportingQuantity)))
    {
    /* If Reporting quantity indicates var or average but measurement quantity
      * does not indicate time interval, then configuration incomplete */
      if ((rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadAverage == TRUE) ||
        (rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadVariance == TRUE))
      {
        /* Check if meas quantity is equal to RAW. If yes, then ERROR */
        if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
          trafficVolumeMeasQuantity,rrc_TrafficVolumeMeasQuantity,rlc_BufferPayload))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:R.Quan = Var/Avge,M.Quan= Raw");
          
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          /* Send Meas Ctrl Failure Message */
          
          return TDSTVM_FAIL;
        }
        
      }
      
      /* Now check if reporting mode present */
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode)))
      {
        /* Meas reporting Mode absent. Send Meas Ctrl Failure */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of if Meas Quan, Reporting Quan present */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MeasQuantity or ReportingQuantity absent");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
    }
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    
    /* Time to process the actual parameters */
    if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
      u.setup->u.trafficVolumeMeasurement,&mac_mcm_params)
      == TDSTVM_FAIL)
    {
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
      
      /* Set Error Cause */
      rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
    }
    
    /* Check if Meas Validity PRESENT */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,measurementValidity))
    {
      prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
        rrc_mcm_ptr->measurementCommand.u.setup->u.
        trafficVolumeMeasurement->measurementValidity.ue_State);
    }
    
    if (((prim_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
      ((prim_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: RRC val,MCM val differ");
      mac_mcm_params.meas_start_incl = FALSE;
    }

    if((prim_meas_validity == TDSALL_STATES) && (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.
      trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasurementObjectList))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas validity == all states,but no tvm object");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf); 
      return TDSTVM_FAIL;
    }
 
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
      /* Call fn which makes check for additional measurements */
      if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
        transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return TDSTVM_FAIL;
      }
      
      /* This means additional meas has been successfully validated */
      addtl_meas_validation = TRUE;
      
    } /* End of if of Additional Meas Present */
    
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    /* To read traffic volume reporting mode */
    mac_mcm_params.meas_report_mode.transfer_mode = 
      TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
      measurementReportingMode.measurementReportTransferMode);

    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_validation, TRUE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
      
    }
    
    /* Set Measurement Identity in tdstvm_meas_id_list for SETUP */
    tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
    
    /* Set parameters in RRC DB */
    if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
    {
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
      
    }
    /* Store additional meas */
    /* Check if Additional Meas has been validated successfully. */
    /* If yes, then store all additional meas in RRC DB */
    if (addtl_meas_validation == TRUE) 
    {
      tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
    }
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r8_modify)
  {
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_MODIFY;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,
        tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode))
    {
      /* To read traffic volume reporting mode */
      mac_mcm_params.meas_report_mode.transfer_mode = 
        TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
        measurementReportingMode.measurementReportTransferMode);  
      
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r8_modify,measurementType))
    {
      if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType,
        rrc_MeasurementType_r8_trafficVolumeMeasurement)))
      {
      /* If one comes here, then it means that meas id earlier determined
      * by RRC as traffic volume is actually not Traffic Volume. Send
        * Meas Ctrl failure Msg to UTRAN */
        
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Meas Id is not Traffic Volume");
        /* Send Meas Ctrl Failure */
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
      
      /* Time to process the actual parameters */
      if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
        u.modify->measurementType.u.trafficVolumeMeasurement,
        &mac_mcm_params) == TDSTVM_SUCCESS)
      {
        
        /* Check if Meas Validity PRESENT */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          trafficVolumeMeasurement,
          tdsrrc_TrafficVolumeMeasurement,measurementValidity))
        {
          prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
           rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
            trafficVolumeMeasurement->measurementValidity.ue_State);
        }

        /* Process Additional Meas */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

          if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
            transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
          {
            /* Called function has already sent Meas Ctrl Failure */
            return TDSTVM_FAIL;
          }
          
          /* This means additional meas has been successfully validated */
          addtl_meas_validation = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
          
            rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
          
            /* Send MCF msg to UTRAN */  
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
            return TDSTVM_FAIL;
          
          }
        
        } /* End of if of Additional Meas Present */
        

        tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
        
        if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
        {
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          /* Send MCF msg to UTRAN */  
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          
          return TDSTVM_FAIL;
          
        }

        /* Store additional meas */
        /* Check if Additional Meas has been validated successfully. */
        /* If yes, then store all additional meas in RRC DB */
        if (addtl_meas_validation == TRUE) 
        {
          tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
        }
        
        
      } /* End if if of Traffic Volume params successfully read */
      else
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
        
        /* Set Error Cause */
        rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of If of meas_type present */
    else
    {
      /* Set Measurement Identity in tdstvm_meas_id_list for MODIFY */
      tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);

      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

        if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
          transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return TDSTVM_FAIL;
        }
        
        /* This means additional meas has been successfully validated */
        addtl_meas_validation = TRUE;
        
      } /* End of if of Additional Meas Present */
      
      if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
        addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
      {
        /* As UTRAN tried to configure more than 4 Addtl Meas */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
        
      }
      
      
      if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
      {
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        
        return TDSTVM_FAIL;
        
      }

      /* Store additional meas */
      /* Check if Additional Meas has been validated successfully. */
      /* If yes, then store all additional meas in RRC DB */
      if (addtl_meas_validation == TRUE) 
      {
        tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
      }
    }
    
    
  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdstvm_meas_id_list */
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_DELETE;
    
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&mac_mcm_params.meas_id);
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
    {
      if (tdstvm_meas_id_list[count].meas_id == 
        rrc_mcm_ptr->measurementIdentity)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Delete:Remove Meas Id");
        
        tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        
        tdstvm_meas_id_list[count].block_rach_incl = FALSE;
        
        tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
        
        tdstvm_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        
        tdstvm_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++) 
        {
          tdstvm_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        
        break;  /* Come out of for */
      } /* End of if */
      
    }
    
    alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
    mac_mcm_params.config_ack_num = alloc_idx;

    tdsrrctvm_send_mac_cmd(&mac_mcm_params);
    
    return TDSTVM_SUCCESS;
    
  }
  
  /* Time to store meas validity for SETUP or Modify 
  It has already been verified that the Measurement exists */ 
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == 
      rrc_mcm_ptr->measurementIdentity)
    {
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      
      /* This means that meas would be saved on trans from DCH to FACH */
      /* Change ownership to Meas Ctrl Msg */      
      
      if (mac_mcm_params.meas_report_mode.report_mode == 
        TDSEVENT_TRIGGERED_MODE)         
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"trch_info_incl = %d",mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl);
        if (mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl == TRUE)
        {
          for (trch_cnt = 0; trch_cnt < mac_mcm_params.meas_report_criteria.event_cfg.num_trch; trch_cnt++)
          {
            for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].num_events; cnt++)
            {
              if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].event_id == TDSEVENT_4A) &&
                (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
              {
                tdstvm_meas_id_list[count].block_rach_incl = TRUE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
                block_set = TRUE;
                break; 
              } /* End of if of event_id and tx_interrupt_tmr check */
            } /* End of for */
            if (block_set == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
              break;
            }
          } /* end of 2nd for */
        }
        else
        {
          /* Keep up with MAC, if trch_info_incl = FALSE, only get event info from first element */
          for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].num_events; cnt++)
          {
            if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].event_id == TDSEVENT_4A) &&
              (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
            {
              tdstvm_meas_id_list[count].block_rach_incl = TRUE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
              block_set = TRUE;
              break; 
            } /* End of if of event_id and tx_interrupt_tmr check */
          } /* End of for */
          if (block_set == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
            break;
          }
        }
        break;
      } /* End of if for Event Triggered Mode */
      break;
    } /* End of if */
    
  } /* End of for loop */
  
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
  mac_mcm_params.config_ack_num = alloc_idx;

  /* Now send config command to MAC */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending rrctvm_send_mac_command");
  tdsrrctvm_send_mac_cmd(&mac_mcm_params);
  return TDSTVM_SUCCESS;
}

#endif /*FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R9

DESCRIPTION  

    Process traffic volume information in a Meas
    Ctrl Message R9. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r9
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
)
{
  uint8 count = 0, cnt = 0, trch_cnt = 0;

  tdsrrc_MeasurementControl_r9_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 transaction_identifier = 0;

  /* Declare MCM config structure that goes to MAC */
  tdsmac_ul_traffic_meas_config_type  mac_mcm_params = {0};
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  /* Variable defined to check if addtl present in MCM is valid
  This varibale is set to TRUE if addtl meas is valid. True means
  in setting addtl meas in RRC db */
  boolean addtl_meas_validation = FALSE;
  
  tdsmeas_validity_enum_type prim_meas_validity = TDSNOT_PRESENT;
  
  tdsrrcmeas_layer_e_type meas_type = TDSMAC_MEAS;
  
  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  boolean block_set = FALSE;
  /* Assign some default values */
  mac_mcm_params.meas_obj_incl = FALSE;
  mac_mcm_params.meas_qty_incl = FALSE;
  mac_mcm_params.meas_report_incl = FALSE;
  mac_mcm_params.meas_criteria_incl = FALSE;  
  mac_mcm_params.meas_start_incl = TRUE;
  mac_mcm_params.meas_obj_list.num_trch = 0;

  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.
    criticalExtensions.u.criticalExtensions.u.r9.measurementControl_r9;

  /* Store the Transaction Identifier */
  transaction_identifier = tid;
  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r9_setup)
  {
    /* Check if meas quantity and reporting quantity received. If either of them
    * not received, then configuration incomplete */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeReportingQuantity)))
    {
    /* If Reporting quantity indicates var or average but measurement quantity
      * does not indicate time interval, then configuration incomplete */
      if ((rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadAverage == TRUE) ||
        (rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
        trafficVolumeReportingQuantity.rlc_RB_BufferPayloadVariance == TRUE))
      {
        /* Check if meas quantity is equal to RAW. If yes, then ERROR */
        if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement->
          trafficVolumeMeasQuantity,rrc_TrafficVolumeMeasQuantity,rlc_BufferPayload))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:R.Quan = Var/Avge,M.Quan= Raw");
          
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set faliure t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          /* Send Meas Ctrl Failure Message */
          
          return TDSTVM_FAIL;
        }
        
      }
      
      /* Now check if reporting mode present */
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,measurementReportingMode)))
      {
        /* Meas reporting Mode absent. Send Meas Ctrl Failure */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of if Meas Quan, Reporting Quan present */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MeasQuantity or ReportingQuantity absent");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
    }
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    
    /* Time to process the actual parameters */
    if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
      u.setup->u.trafficVolumeMeasurement,&mac_mcm_params)
      == TDSTVM_FAIL)
    {
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
      
      /* Set Error Cause */
      rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
    }
    
    /* Check if Meas Validity PRESENT */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,measurementValidity))
    {
      prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
        rrc_mcm_ptr->measurementCommand.u.setup->u.
        trafficVolumeMeasurement->measurementValidity.ue_State);
    }
    
    if (((prim_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
      ((prim_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM: RRC val,MCM val differ");
      mac_mcm_params.meas_start_incl = FALSE;
    }

    if((prim_meas_validity == TDSALL_STATES) && (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.
      trafficVolumeMeasurement,
      tdsrrc_TrafficVolumeMeasurement,trafficVolumeMeasurementObjectList))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas validity == all states,but no tvm object");
      /* Send Meas Ctrl Failure Configuration Incomplete */
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf); 
      return TDSTVM_FAIL;
    }
 
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);
      
      /* Call fn which makes check for additional measurements */
      if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
        transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return TDSTVM_FAIL;
      }
      
      /* This means additional meas has been successfully validated */
      addtl_meas_validation = TRUE;
      
    } /* End of if of Additional Meas Present */
    
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_START;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    /* To read traffic volume reporting mode */
    mac_mcm_params.meas_report_mode.transfer_mode = 
      TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
      measurementReportingMode.measurementReportTransferMode);

    if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
      addtl_meas_validation, TRUE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
    {
      /* As UTRAN tried to configure more than 4 Addtl Meas */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
      
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return TDSTVM_FAIL;
      
    }
    
    /* Set Measurement Identity in tdstvm_meas_id_list for SETUP */
    tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
    
    /* Set parameters in RRC DB */
    if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
    {
      rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      
      /* Send MCF msg to UTRAN */  
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      
      return TDSTVM_FAIL;
      
    }
    /* Store additional meas */
    /* Check if Additional Meas has been validated successfully. */
    /* If yes, then store all additional meas in RRC DB */
    if (addtl_meas_validation == TRUE) 
    {
      tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
    }
  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r9_modify)
  {
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    /* Now set other values */
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_MODIFY;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr,
        tdsrrc_MeasurementControl_r9_IEs,measurementReportingMode))
    {
      /* To read traffic volume reporting mode */
      mac_mcm_params.meas_report_mode.transfer_mode = 
        TDSTVM_RET_REPORTING_MODE(rrc_mcm_ptr->
        measurementReportingMode.measurementReportTransferMode);  
      
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r9_modify,measurementType))
    {
      if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType,
        rrc_MeasurementType_r9_trafficVolumeMeasurement)))
      {
      /* If one comes here, then it means that meas id earlier determined
      * by RRC as traffic volume is actually not Traffic Volume. Send
        * Meas Ctrl failure Msg to UTRAN */
        
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"TVM:Meas Id is not Traffic Volume");
        /* Send Meas Ctrl Failure */
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
      
      /* Time to process the actual parameters */
      if (tdsrrctvm_read_traffic_vol_params(rrc_mcm_ptr->measurementCommand.
        u.modify->measurementType.u.trafficVolumeMeasurement,
        &mac_mcm_params) == TDSTVM_SUCCESS)
      {
        
        /* Check if Meas Validity PRESENT */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          trafficVolumeMeasurement,
          tdsrrc_TrafficVolumeMeasurement,measurementValidity))
        {
          prim_meas_validity = TDSTVM_RET_MEAS_VALIDITY(
           rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
            trafficVolumeMeasurement->measurementValidity.ue_State);
        }

        /* Process Additional Meas */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

          if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
            transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
          {
            /* Called function has already sent Meas Ctrl Failure */
            return TDSTVM_FAIL;
          }
          
          /* This means additional meas has been successfully validated */
          addtl_meas_validation = TRUE;
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
          
            rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
          
            /* Send MCF msg to UTRAN */  
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
            return TDSTVM_FAIL;
          
          }
        
        } /* End of if of Additional Meas Present */
        

        tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);
        
        if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
        {
          rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
          
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
          
          /* Send MCF msg to UTRAN */  
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          
          
          return TDSTVM_FAIL;
          
        }

        /* Store additional meas */
        /* Check if Additional Meas has been validated successfully. */
        /* If yes, then store all additional meas in RRC DB */
        if (addtl_meas_validation == TRUE) 
        {
          tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
        }
        
        
      } /* End if if of Traffic Volume params successfully read */
      else
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
        
        /* Set Error Cause */
        rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
      }
      
    } /* End of If of meas_type present */
    else
    {
      /* Set Measurement Identity in tdstvm_meas_id_list for MODIFY */
      tdsrrctvm_process_mcm_identity(&mac_mcm_params.meas_id);

      /* Process Additional Meas */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TVM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

        if (tdsrrctvm_validate_additional_meas(&prim_meas_validity, &rrc_mcm_ptr->additionalMeasurementList,
          transaction_identifier, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return TDSTVM_FAIL;
        }
        
        /* This means additional meas has been successfully validated */
        addtl_meas_validation = TRUE;
        
      } /* End of if of Additional Meas Present */
      
      if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
        addtl_meas_validation, FALSE, TDSMAC_MEAS, rrc_mcm_ptr->measurementIdentity,
        &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
      {
        /* As UTRAN tried to configure more than 4 Addtl Meas */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending Meas Ctrl Failure");
        
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return TDSTVM_FAIL;
        
      }
      
      
      if (tdsrrctvm_config_tvm_db(&mac_mcm_params,&prim_meas_validity) == FALSE)
      {
        rrcmeas_mcf.rrc_transaction_id = transaction_identifier;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        
        /* Send MCF msg to UTRAN */  
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        
        return TDSTVM_FAIL;
        
      }

      /* Store additional meas */
      /* Check if Additional Meas has been validated successfully. */
      /* If yes, then store all additional meas in RRC DB */
      if (addtl_meas_validation == TRUE) 
      {
        tdsrrcmeas_set_additional_meas_in_db(&meas_type, &rrc_mcm_ptr->additionalMeasurementList,
          rrc_mcm_ptr->measurementIdentity);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Additional Meas set in RRC DB");
      }
    }
    
    
  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
    * that this meas exists. So let us delete it from tdstvm_meas_id_list */
    
    /* Set Meas Identity */
    mac_mcm_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    mac_mcm_params.meas_cmd = TDSMAC_TRAFFIC_MEAS_DELETE;
    
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&mac_mcm_params.meas_id);
    
    mac_mcm_params.config_ack_num = (uint16) transaction_identifier;
    
    mac_mcm_params.config_conf_ind = TRUE;
    
    for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
    {
      if (tdstvm_meas_id_list[count].meas_id == 
        rrc_mcm_ptr->measurementIdentity)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Delete:Remove Meas Id");
        
        tdstvm_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        
        tdstvm_meas_id_list[count].block_rach_incl = FALSE;
        
        tdstvm_meas_id_list[count].meas_ctrl_msg = FALSE;
        
        tdstvm_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        
        tdstvm_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++) 
        {
          tdstvm_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        
        break;  /* Come out of for */
      } /* End of if */
      
    }
    
    alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
    mac_mcm_params.config_ack_num = alloc_idx;

    tdsrrctvm_send_mac_cmd(&mac_mcm_params);
    
    return TDSTVM_SUCCESS;
    
  }
  
  /* Time to store meas validity for SETUP or Modify 
  It has already been verified that the Measurement exists */ 
  for ( count = 0; count < TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS; count++ )
  {
    if (tdstvm_meas_id_list[count].meas_id == 
      rrc_mcm_ptr->measurementIdentity)
    {
      tdstvm_meas_id_list[count].meas_ctrl_msg = TRUE;
      
      /* This means that meas would be saved on trans from DCH to FACH */
      /* Change ownership to Meas Ctrl Msg */      
      
      if (mac_mcm_params.meas_report_mode.report_mode == 
        TDSEVENT_TRIGGERED_MODE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"trch_info_incl = %d",mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl);
        if (mac_mcm_params.meas_report_criteria.event_cfg.trch_info_incl == TRUE)
        {
          for (trch_cnt = 0; trch_cnt < mac_mcm_params.meas_report_criteria.event_cfg.num_trch; trch_cnt++)
          {
            for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].num_events; cnt++)
            {
              if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].event_id == TDSEVENT_4A) &&
                (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[trch_cnt].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
              {
                tdstvm_meas_id_list[count].block_rach_incl = TRUE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
                block_set = TRUE;
                break; 
              } /* End of if of event_id and tx_interrupt_tmr check */
            } /* End of for */
            if (block_set == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
              break;
            }
          } /* end of 2nd for */
        }
        else
        {
          /* Keep up with MAC, if trch_info_incl = FALSE, only get event info from first element */
          for (cnt = 0; cnt < mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].num_events; cnt++)
            {
            if ((mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].event_id == TDSEVENT_4A) &&
              (mac_mcm_params.meas_report_criteria.event_cfg.trch_event_cfg[0].event_param[cnt].tx_interrupt_tmr != TDSQUANTITY_NOT_PRESENT))
            {
              tdstvm_meas_id_list[count].block_rach_incl = TRUE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Block RACH set Meas Id %d",tdstvm_meas_id_list[count].meas_id);
              block_set = TRUE;
              break; 
            } /* End of if of event_id and tx_interrupt_tmr check */
          } /* End of for */
          if (block_set == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Block RACH config finished");
            break;
          }
        }
        break;
      } /* End of if for Event Triggered Mode */
      break;
    } /* End of if */
    
  } /* End of for loop */
  
  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(transaction_identifier);
  mac_mcm_params.config_ack_num = alloc_idx;

  /* Now send config command to MAC */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TVM:Sending rrctvm_send_mac_command");
  tdsrrctvm_send_mac_cmd(&mac_mcm_params);
  return TDSTVM_SUCCESS;
}
#endif /*FEATURE_TDSCDMA_REL9*/



