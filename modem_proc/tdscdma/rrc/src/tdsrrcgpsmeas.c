/*===========================================================================
            R R C   M E A S U R E M E N T S

DESCRIPTION

   This module supports Inter Freq Measurement Control and Measurement Reporting
   for the RRC layer. At present, it supports the following features
   1. Sends one shot A-GPS Meas Report to UTRAN in connected mode of UE
   2. Supports Measurement Control Message received from UTRAN



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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcgpsmeas.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl    Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/



/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "customer.h"
#include "tdsuecomdef.h"
#include "tdsrrcsend.h"
#include "tdsrrcscmgr.h"
#include "comdef.h"
#include "tdsrrcsibdb.h"
#include "msg.h"
#include "tdsrrccmd_v.h"
#include "tdsrrclcm.h"
#include "tdsrrc_dependancy.h"

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
#include "tdsrrcmeas.h"
#include "tdsrrcmeasi.h"
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
#include "tdsrrclsmif.h"
#include "tdsrrcmcm.h"
#include "tdsl1rrcif.h"
#include "tdsrrcmeasi.h"
#include "tdsrrccspi.h"
#include "tdsrrcsibproc.h"
#include "tdsrrcllc.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcscmgr.h"
#include "tdsrrccu.h"
#include "tdsrrccui.h"
#endif   /* FEATURE_TDSCDMA_GPS_CELL_ID_READING*/
#include "tdsrrcasn1util.h"

/* UMTS_CP_TDSCDMA call back function pointers */
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
/*lint -e551 */
static tdsrrcgps_cgps_event_cb_type         tdsrrcgps_cgps_event_cb = NULL;
/*lint +e551 */
tdsrrcgps_cgps_ue_pos_capability_cb_type    tdsrrcgps_cgps_ue_pos_capability_cb = NULL;
#endif
 

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSMAX_SFN 4095

#define TDSGPS_MSB_TIMING 16383

#define TDSCPID_MAX 127

#define TDSMAX_SAT_ID 63

#define TDSMAX_DOPPLER 32768

#define TDSMIN_DOPPLER -32768

#define TDSMULTIPATH_VALUE 3

#define TDSMAX_GPS_CHIPS 1022

#define TDSMAX_FRAC_GPS_CHIPS 1023

#define TDSMAX_RMS_ERROR 63

#define TDSMAX_C_NO 63

#define TDSMAX_LATITUDE 8388607

#define TDSMIN_LONGITUDE -8388608

#define TDSMAX_LONGITUDE 8388608

#define TDSMAX_LATITUDE_SIGN 1

#define TDSMAX_ALTITUDE 32767

#define TDSMAX_ALTITUDE_DIRECTION 1

#define TDSMAX_CONFIDENCE 100

#define TDSUNCERTAINITY_SEMI_MINOR 127

#define TDSUNCERTAINITY_SEMI_MAJOR 127

#define TDSORIENTATION_MAJOR_AXIS 89

#define TDSMAX_GPS_WEEK 1023

#define TDSGPS_AGE_OF_TOLERANCE 10

#define TDSMAX_IODE 255

#define TDSMAX_GPS_TOW 167

#define TDSMAX_UNCERTAINITY_ALTITUDE 127


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/*-------------------------------------------------------------------
ENUM: tdsrrcmeas_current_substate
Stores substate of RRC Measurement procedure
--------------------------------------------------------------------*/
extern tdsrrcmeas_substate_e_type tdsrrcmeas_current_substate; 


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
/* -----------------------------------------------------------------------
** Local Object Definition
** ----------------------------------------------------------------------- */
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA

/* Note: with FEATURE_MMGPS which is mutually exclusive from FEATURE_CGPS_xxx,
   an equivelant function (lsm_convert_assistance_data), and the function it 
   calls (oss_bitstring_to_int), are defined in LSM and were once shared by 
   RRC and LSM. For FEATURE_CGPS, there will be no LSM, so these functions 
   will be defined only here. */
boolean tdsrrcgps_convert_assistance_data( 
  tdsrrc_UE_Positioning_GPS_AssistanceData* asn1_ptr,
  tdsrrc_assistance_data_struct_type* rrc_ptr );
#endif

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING

/* RRC GPS Module substate*/
tdsrrcgps_substates_e_type tdsrrcgps_substate;


uint32 tdsrrc_prev_cell_id_reported;


#define TDSRRC_GPS_INVALID_CPID 0xFF
#define TDSRRC_GPS_INVALID_FREQ  0xFFFF

uint32 tdsrrc_gps_cpid_stored=TDSRRC_GPS_INVALID_CPID;

/*--------------------------------------------------------------------------
       Macro to convert scrambling code to primary scrambling code
--------------------------------------------------------------------------*/
#define TDSSCR_CODE_TO_PRIMARY_SCR_CODE(cpid) ((cpid) >> 4)

#endif/* FEATURE_TDSCDMA_GPS_CELL_ID_READING*/


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

tdspos_meas_id_list_struct_type tdspos_meas_id_list[TDSMAX_POS_MEAS];

extern uint8 tdsrrcmeas_pending_rpt;


#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
/* pointer to callback func for cell id change notification */
TDSRRC_GPS_CELL_ID_CNF_CB_FUNC_TYPE *tdsrrc_cell_id_cnf_cb_func;
#endif/* FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */


#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA

/*===========================================================================

FUNCTION  tdsrrcgps_register_cgps_event_cb

DESCRIPTION
  This function is called by CGPS to install a callback which will be used
  by RRC to provide events and/or data back to CGPS

DEPENDENCIES None

RETURN VALUE : TRUE/FALSE

SIDE EFFECTS None

===========================================================================*/

boolean tdsrrcgps_register_cgps_event_cb( tdsrrcgps_cgps_event_cb_type event_cb_fp )
{
  if ( event_cb_fp == NULL )
    return FALSE;

  tdsrrcgps_cgps_event_cb = event_cb_fp;
  return TRUE;
}


/*===========================================================================

FUNCTION  rrcgps_register_cgps_ue_capability_cb

DESCRIPTION
  This function is called by CGPS to install a callback which will be used
  by RRC to get UE capability information from TM.

DEPENDENCIES None

RETURN VALUE : TRUE/FALSE

SIDE EFFECTS None

===========================================================================*/


boolean tdsrrcgps_register_cgps_ue_pos_capability_cb ( tdsrrcgps_cgps_ue_pos_capability_cb_type ue_cap_cb_fp)
{
  if ( ue_cap_cb_fp == NULL )
    return FALSE;

  tdsrrcgps_cgps_ue_pos_capability_cb = ue_cap_cb_fp;
  return TRUE;
}  

#endif /* FEATURE_CGPS_UMTS_CP_TDSCDMA */



/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM

DESCRIPTION

    This function processes the Measurement Control Message for
    A-GPS Measurements. 

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr
  ,uint8 meas_ctrl_tr_id
)
{
  /* Process the A-GPS Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  uint16 deleted_meas_id = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsrrc_meas_params_struct_type  gps_meas_params;
  
  /* Declare ptr to store message received from UTRAN */
  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;
  
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  tdsrpt_crit_enum_type rpt_mode;
  
  boolean modify_meas_params_incl = FALSE;
  
  tdsmeas_validity_enum_type pos_meas_validity = TDSNOT_PRESENT;
  
  tdsrrc_UE_Positioning_ReportingQuantity* rpt_quan_ptr = NULL;
  
  uint8 tdstransaction_id = TDSINVALID_TRANSACTION_ID;

  
  gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = FALSE;
  
  gps_meas_params.u.meas_ctrl_params.rpt_quan_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.environ_ch_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.hor_accuracy_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.sfn_gps_timing_required = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.additional_assistance_data_request = FALSE;
  gps_meas_params.u.meas_ctrl_params.gps_assistance_data_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.meas_start_incl = TRUE;
  
  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)pos_mcm_ptr;
  tdstransaction_id = meas_ctrl_tr_id;
  
  /* By default, it is set to 0 => 1 */
  gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = 0;
  
  /* Set Transaction Id and Meas Id */
  p_info_ptr->meas_id = gps_meas_params.u.meas_ctrl_params.meas_id =
    (uint8) dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementIdentity;
  
  gps_meas_params.u.meas_ctrl_params.config_conf_ind = TRUE;
  
  gps_meas_params.u.meas_ctrl_params.config_ack_num = (uint8) tdstransaction_id;
  
  gps_meas_params.u.meas_ctrl_params.rpt_quan.method = (uint8)TDSRRC_GPS;
  gps_meas_params.params_type = (uint8)TDSRRC_MEAS_CTRL_PARMS;
  
  /* Check if Meas Validity present */
  if (dcch_msg_ptr->message.u.measurementControl.u.r3.m.v390nonCriticalExtensionsPresent)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:Non Critical Extensions present");
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(dcch_msg_ptr->message.u.measurementControl.u.r3.
      v390nonCriticalExtensions.measurementControl_v390ext,
       ue_Positioning_Measurement_v390ext))
    {
      /* Check if Meas Validity present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.measurementControl.u.r3.
        v390nonCriticalExtensions.measurementControl_v390ext.ue_Positioning_Measurement_v390ext,
        tdsrrc_UE_Positioning_Measurement_v390ext,measurementValidity))
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"POS:Meas validity %d (0/d:1/f:2/a)", dcch_msg_ptr->message.u.measurementControl.u.r3.
          v390nonCriticalExtensions.measurementControl_v390ext.ue_Positioning_Measurement_v390ext.measurementValidity.ue_State,0,0);
        
        /* Store Measurement Validity */
        pos_meas_validity = TDSTVM_RET_MEAS_VALIDITY(dcch_msg_ptr->message.u.measurementControl.u.r3.
          v390nonCriticalExtensions.measurementControl_v390ext.ue_Positioning_Measurement_v390ext.measurementValidity.ue_State);
        /* Check if Meas needs to be restarted */
        if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand,
            rrc_MeasurementCommand_release))
           && (((pos_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
           ((pos_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH))))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: RRC val,MCM val differ");
          gps_meas_params.u.meas_ctrl_params.meas_start_incl = FALSE;
        }
      }
    }
  }
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand,
       rrc_MeasurementCommand_setup))
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_SETUP;
    
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3,
        tdsrrc_MeasurementControl_r3_IEs,measurementReportingMode)))
    {
      /* Send Meas Ctrl Failure */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas Reporting Mode missing for SETUP");
      
      /* Set transaction ID */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    else
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode =
       (uint8) TDSRET_POS_RPT_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementReportingMode.measurementReportTransferMode);
    }
    
    
    rpt_quan_ptr = &dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.
      measurementCommand.u.setup->u.ue_positioning_Measurement->ue_positioning_ReportingQuantity;
    
    
    gps_meas_params.u.meas_ctrl_params.rpt_quan_incl = TRUE;
    
    /* Get Pos Method type */
    gps_meas_params.u.meas_ctrl_params.rpt_quan.method_type = (uint8)TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
    
    if (tdsrrcgps_verify_positioning_capability(rpt_quan_ptr)
      == FAILURE)
    {
      /* Send Meas Ctrl Failure */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Pos Capability error");
      
      /* Set transaction ID */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      if(tdsrrcgps_cgps_ue_pos_capability_cb == NULL)
      {
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      }
      else
#endif
      {
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      }
      
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }

    
    /* Get Reporting Quantity */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rpt_quan_ptr,horizontal_Accuracy))
    {
      gps_meas_params.u.meas_ctrl_params.rpt_quan.hor_accuracy_incl = TRUE;
      
      /* Set the 7 bits */
      gps_meas_params.u.meas_ctrl_params.rpt_quan.horizontal_accuracy = (uint8)
        (*(rpt_quan_ptr->horizontal_Accuracy.data) >> 1);
    }
    
    
    /* AdditionalAssistanceDataRequest */
    if (rpt_quan_ptr->additionalAssistanceDataRequest == TRUE)
    {
      gps_meas_params.u.meas_ctrl_params.rpt_quan.additional_assistance_data_request = TRUE;
    }
    
    if (rpt_quan_ptr->gps_TimingOfCellWanted == TRUE)
    {
      gps_meas_params.u.meas_ctrl_params.rpt_quan.sfn_gps_timing_required = TRUE;
    }
    
    /* Get Reporting Criteria */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.
        measurementCommand.u.setup->u.ue_positioning_Measurement->reportCriteria,
      rrc_UE_Positioning_ReportCriteria_ue_positioning_ReportingCriteria))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Event trigg crit unsupported");
      /* Set transaction ID */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    else if (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.setup->u.
      ue_positioning_Measurement->reportCriteria,
      rrc_UE_Positioning_ReportCriteria_noReporting))
    {
      gps_meas_params.u.meas_ctrl_params.rpt_crit.rpt_mode = (uint8)TDSRRC_NO_RPT;
    }
    else
    {
      
      gps_meas_params.u.meas_ctrl_params.rpt_crit.rpt_mode = (uint8) TDSRRC_PERIODIC_RPT;
      
      /* Get reporting criteria and interval */
      gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = (uint8)tdsrrc_ReportingAmount_ra_Infinity;

   
      gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = (uint8) 
        dcch_msg_ptr->message.u.measurementControl.u.r3.
           measurementControl_r3.measurementCommand.u.setup->u.ue_positioning_Measurement->
           reportCriteria.u.periodicalReportingCriteria->reportingAmount;
     
      
      gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_interval =(uint8) 
        dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.
        u.setup->u.ue_positioning_Measurement->reportCriteria.u.periodicalReportingCriteria->reportingInterval;
    }
    if(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.setup->u.
        ue_positioning_Measurement->m.ue_positioning_GPS_AssistanceDataPresent
      )
    {
    /* Now call LSM fn to store Meas Object if FEATURE_MMGPS; call a local function to do the same if FEATURE_CGPS_xxx */
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      if (tdsrrcgps_convert_assistance_data(&dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.setup->u.
        ue_positioning_Measurement->ue_positioning_GPS_AssistanceData,
        &gps_meas_params.u.meas_ctrl_params.gps_assistance_data) == FALSE)
#else  /* this will only be FEATURE_MMGPS */
      if (lsm_convert_assistance_data(&dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.setup->u.
        ue_positioning_Measurement->ue_positioning_GPS_AssistanceData,
        &gps_meas_params.u.meas_ctrl_params.gps_assistance_data) == FALSE)
#endif
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Assistance Data conversion failed");
        
        /* Set transaction ID */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
        
        rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        
        return FAILURE;
      }
      gps_meas_params.u.meas_ctrl_params.gps_assistance_data_incl = TRUE;
    }
     
    switch(gps_meas_params.u.meas_ctrl_params.rpt_crit.rpt_mode)
    {
      case 0: rpt_mode = TDSPERIODIC;
          break;
      case 1: rpt_mode = TDSEVENT;
          break;
      case 2: rpt_mode = TDSADDTL;
          break;
      case 3: rpt_mode = TDSNOT_AVAILABLE;
          break;
      default:break;
    } 
    
    /* Additional Meas. LSM does not support it. Ignore */
    if(tdsrrcmeas_config_pos_db(p_info_ptr, &pos_meas_validity, &rpt_mode, modify_meas_params_incl) == FALSE)
    {
      return FAILURE;   
    }
    
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand,
                                        rrc_MeasurementCommand_modify))
  {
    /* Addtl Meas processing later. Reporting crit stored in db later */
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_MODIFY;
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3,
                                     tdsrrc_MeasurementControl_r3_IEs,measurementReportingMode))
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode = (uint8)
        TDSRET_POS_RPT_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_msg_ptr->message.u.measurementControl.u.r3.
          measurementControl_r3.measurementCommand.u.modify,tdsrrc_MeasurementCommand_modify,
          measurementType))
    {
      modify_meas_params_incl = TRUE;
      
      rpt_quan_ptr = &dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.modify->measurementType.u.
        ue_positioning_Measurement->ue_positioning_ReportingQuantity;
     
      gps_meas_params.u.meas_ctrl_params.rpt_quan_incl = TRUE;
      
      /* Get Pos Method */
      gps_meas_params.u.meas_ctrl_params.rpt_quan.method_type = (uint8) TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
      
      if (tdsrrcgps_verify_positioning_capability(rpt_quan_ptr)
        == FAILURE)
      {
        /* Send Meas Ctrl Failure */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Pos Capability error");
        
        /* Set transaction ID */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
        if(tdsrrcgps_cgps_ue_pos_capability_cb == NULL)
        {
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        }
        else
#endif
        {
          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
        }
        
        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return FAILURE;
      }

      
      /* Get Reporting Quantity */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rpt_quan_ptr,horizontal_Accuracy))
      {
        gps_meas_params.u.meas_ctrl_params.rpt_quan.hor_accuracy_incl = TRUE;
        
        /* Set the 7 bits */
        gps_meas_params.u.meas_ctrl_params.rpt_quan.horizontal_accuracy = (uint8)
          (*(rpt_quan_ptr->horizontal_Accuracy.data) >> 1);
      }
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r4,verticalAccuracy))||
          (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r4,gANSSPositioningMethods))||
          (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r4,gANSSTimingOfCellWanted))||
          (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r4,gANSSCarrierPhaseMeasurementRequested)))
      {
        /*TBD*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel 4 IE's Support TBD");
      }
      
      /* AdditionalAssistanceDataRequest */
      if (rpt_quan_ptr->additionalAssistanceDataRequest == TRUE)
      {
        gps_meas_params.u.meas_ctrl_params.rpt_quan.additional_assistance_data_request = TRUE;
      }
      
      if (rpt_quan_ptr->gps_TimingOfCellWanted == TRUE)
      {
        gps_meas_params.u.meas_ctrl_params.rpt_quan.sfn_gps_timing_required = TRUE;
      }
      
      
      /* Get Reporting Criteria */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.
                                       modify->measurementType.u.ue_positioning_Measurement->reportCriteria,
                                       rrc_UE_Positioning_ReportCriteria_ue_positioning_ReportingCriteria))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Event trigg crit unsupported");
        /* Set transaction ID */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        
        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        
        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        
        return FAILURE;
      }
      else if (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.modify->measurementType.u.
                                            ue_positioning_Measurement->reportCriteria,
                                            rrc_UE_Positioning_ReportCriteria_noReporting))
      {
        gps_meas_params.u.meas_ctrl_params.rpt_crit.rpt_mode = (uint8)TDSRRC_NO_RPT;
      }
      else
      {
        gps_meas_params.u.meas_ctrl_params.rpt_crit.rpt_mode = (uint8)TDSRRC_PERIODIC_RPT;
        
        /* Get reporting criteria and interval */
        gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = (uint8)tdsrrc_ReportingAmount_ra_Infinity;


          gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = 
            (uint8) dcch_msg_ptr->message.u.measurementControl.u.r3.
              measurementControl_r3.measurementCommand.u.modify->measurementType.u.ue_positioning_Measurement->
              reportCriteria.u.periodicalReportingCriteria->reportingAmount;     
        

        gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_interval = (uint8)
          dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.
            u.modify->measurementType.u.ue_positioning_Measurement->reportCriteria.u.periodicalReportingCriteria->reportingInterval;

      }
      if(dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.modify->measurementType.u.ue_positioning_Measurement->m.ue_positioning_GPS_AssistanceDataPresent  )
      {
      /* Now call LSM fn to store Meas Object if FEATURE_MMGPS; call a local function to do the same if FEATURE_CGPS_xxx */
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      if (tdsrrcgps_convert_assistance_data(&dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.modify->measurementType.u.
                                            ue_positioning_Measurement->ue_positioning_GPS_AssistanceData,
                                            &gps_meas_params.u.meas_ctrl_params.gps_assistance_data) == FALSE)
#else /* this will only be FEATURE_MMGPS */
      if (lsm_convert_assistance_data(&dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementCommand.u.modify->measurementType.u.
                                      ue_positioning_Measurement->ue_positioning_GPS_AssistanceData,
                                      &gps_meas_params.u.meas_ctrl_params.gps_assistance_data) == FALSE)
#endif
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Assistance Data conversion failed");
        
        /* Set transaction ID */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        
        /* Set faliure t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
        
        rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        
        return FAILURE;
      }
        gps_meas_params.u.meas_ctrl_params.gps_assistance_data_incl = TRUE;
      }
    } /* End of if of Meas Type present */
    
    /* Store Pos Parameters in RRC Database */
    if(tdsrrcmeas_config_pos_db(p_info_ptr, &pos_meas_validity, &rpt_mode, modify_meas_params_incl) == FALSE)
    {
      return FAILURE;
    }
    
  } /* End of modify */
  else
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8) TDSRRC_MEAS_DELETE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
    /* At present just store meas id in meas db */
    for ( count = 0; count < TDSMAX_POS_MEAS; count++ )
    {
      if (tdspos_meas_id_list[count].meas_id == gps_meas_params.u.meas_ctrl_params.meas_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS: Meas Id %d deleted from DB",gps_meas_params.u.meas_ctrl_params.meas_id);
        tdspos_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdspos_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        tdspos_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
        {
          tdspos_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break;
      }
    } /* End of for */
    
    deleted_meas_id = (uint16) dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.measurementIdentity;
    
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&deleted_meas_id);
    tdsrrcgps_send_lsm_cmd(&gps_meas_params);
    return SUCCESS;
  }
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
  tdsrrcgps_send_lsm_cmd(&gps_meas_params);
  return SUCCESS;

}


/*===========================================================================

FUNCTION    RRCGPS_SEND_MEAS_REPORT

DESCRIPTION

 Processes measurement report received from LSM
 in CELL_DCH state.

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_send_meas_report
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Stores meas report */
  tdsrrc_UL_DCCH_Message* msg_ptr = NULL;
  
  uint8 cnt = 0;
  
  /* Stores status of send chain */
  tdsrrcsend_status_e_type status;
  
  /* RLC ack or unack mode */
  uecomdef_logch_mode_e_type logical_channel_mode = UE_MODE_TRANSPARENT;
  
  tdsrrc_meas_rpt_struct_type* lsm_meas_ptr = NULL;
  
  /* Logical Channel Id */
  rlc_lc_id_type ul_lc_id = 0;
  
  /* Radio bearer mode */
  tdsrrc_RB_Identity rb_mode = 0;
  
  /* L2 ack status */
  tdsrrcsend_l2ack_status_e_type l2_ack_status;
  
  lsm_meas_ptr = cmd_ptr->cmd.meas_rpt_ind.meas_rpt;
  
  if((lsm_meas_ptr->meas_id < TDSMEASUREMENT_IDENTITY_MIN) || (lsm_meas_ptr->meas_id > TDSMEASUREMENT_IDENTITY_MAX))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Measurement identity %d set by LSM",lsm_meas_ptr->meas_id);
    return;
  } 
  if ( lsm_meas_ptr->tranfer_mode == (uint8)TDSMODE_ACKNOWLEDGED)
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
  
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH, rb_mode, logical_channel_mode);
  
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

  
  /* Store meas report chosen */
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_measurementReport;
  
  /* First store the meas id */
  msg_ptr->message.u.measurementReport.measurementIdentity = lsm_meas_ptr->meas_id;
  
  /* Initialize Meas Report bit mask */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport);
  
  TDSRRC_SET_COMMON_MSG_TYPE(msg_ptr->message.u.measurementReport.measuredResults,
                             rrc_MeasuredResults_ue_positioning_MeasuredResults);
  msg_ptr->message.u.measurementReport.measuredResults.u.ue_positioning_MeasuredResults =
    rtxMemAllocTypeZ(&enc_ctxt,rrc_UE_Positioning_MeasuredResults);
  
  
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(msg_ptr->message.u.measurementReport.measuredResults.u.
                                  ue_positioning_MeasuredResults);
  
  TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.measurementReport,
                                   tdsrrc_MeasurementReport,measuredResults);
  
  /* TBD Add a function to validate the report */
  
  
  /* Check what kind of report it is */
  if (lsm_meas_ptr->gps_measured_results_incl == TRUE)
  {
    TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(msg_ptr->message.u.measurementReport.measuredResults.u.
                                         ue_positioning_MeasuredResults,
                                         tdsrrc_UE_Positioning_MeasuredResults,ue_positioning_GPS_Measurement);
    
    if (tdsrrcgps_validate_meas_results(&lsm_meas_ptr->gps_measured_results) == FALSE )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Ignore Meas Report");
      return;
    }
    
    tdsrrcgps_set_meas_results(&lsm_meas_ptr->gps_measured_results,
                               &msg_ptr->message.u.measurementReport.measuredResults.u.
                               ue_positioning_MeasuredResults->ue_positioning_GPS_Measurement);
    if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
      if(lsm_meas_ptr->gps_measured_results.gps_reference_time_uncertainity_incl)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL7: GPS reference time uncertainity included : %d ",lsm_meas_ptr->gps_measured_results.gps_reference_time_uncertainity);
        if(!TDSRRC_CHECK_MSG_TYPE_IE(msg_ptr->message.u.measurementReport,v390nonCriticalExtensionsPresent))
        {
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport,v390nonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.measurementReport_v390ext);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions);
        }
        if(!TDSRRC_CHECK_MSG_TYPE_IE(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions, laterNonCriticalExtensionsPresent))
        {
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions, laterNonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions);
        }
        if(!TDSRRC_CHECK_MSG_TYPE_IE(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions, v4b0NonCriticalExtensionsPresent))
        {
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions, v4b0NonCriticalExtensions);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.measurementReport_v4b0ext);
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);
        }
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions, v590NonCriticalExtensions);
        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                                    v590NonCriticalExtensions.measurementReport_v590ext);

        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                                               v590NonCriticalExtensions, v5b0NonCriticalExtensions);
        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                                    v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
    
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                                               v590NonCriticalExtensions.v5b0NonCriticalExtensions, v690NonCriticalExtensions);
        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                                    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.measurementReport_v690ext);

        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                                               v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions, v770NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                                    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions);

        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                                               v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
                                               measurementReport_v770ext, measuredResults);

        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
          v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
            measurementReport_v770ext.measuredResults.t = T_tdsrrc_MeasuredResults_v770ext_ue_positioning_MeasuredResults;

        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
          v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
            measurementReport_v770ext.measuredResults.u.ue_positioning_MeasuredResults =
              rtxMemAllocTypeZ(&enc_ctxt,rrc_UE_Positioning_MeasuredResults_v770ext);

        TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
          v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
          measurementReport_v770ext.measuredResults.u.ue_positioning_MeasuredResults, rrc_UE_Positioning_MeasuredResults_v770ext, ue_positioning_GPS_Measurement);

        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
          v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
          measurementReport_v770ext.measuredResults.u.ue_positioning_MeasuredResults->ue_positioning_GPS_Measurement, ue_Positioning_GPS_ReferenceTimeUncertainty);

        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
          v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.measurementReport_v770ext.
            measuredResults.u.ue_positioning_MeasuredResults->ue_positioning_GPS_Measurement.ue_Positioning_GPS_ReferenceTimeUncertainty = 
            lsm_meas_ptr->gps_measured_results.gps_reference_time_uncertainity;

      }
    }
  }
  else if (lsm_meas_ptr->positioning_error_incl == TRUE)
  {
    TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(msg_ptr->message.u.measurementReport.measuredResults.u.
                                         ue_positioning_MeasuredResults,
                                         tdsrrc_UE_Positioning_MeasuredResults,ue_positioning_Error);
    
    tdsrrcgps_set_pos_error(&lsm_meas_ptr->pos_error,
                            &msg_ptr->message.u.measurementReport.measuredResults.u.
                            ue_positioning_MeasuredResults->ue_positioning_Error);
    
    
  }
  else if (lsm_meas_ptr->positioning_estimate_incl == TRUE)
  {
    
    if (tdsrrcgps_validate_pos_estimate(&lsm_meas_ptr->positioning_estimate) == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Ignore Meas Report");
      return;
    }
    TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(msg_ptr->message.u.measurementReport.measuredResults.u.
                                         ue_positioning_MeasuredResults,
                                         tdsrrc_UE_Positioning_MeasuredResults,ue_positioning_PositionEstimateInfo);
    
    tdsrrcgps_set_pos_estimate(&lsm_meas_ptr->positioning_estimate,
                               &msg_ptr->message.u.measurementReport.measuredResults.u.
                               ue_positioning_MeasuredResults->ue_positioning_PositionEstimateInfo);
    
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Invalid t");
    tdsrrc_free(msg_ptr);
    return;
  }
  
  /* Additional Meas to be added later to this report */
  
  /* Check if this is the last report. Addtl Meas to be taken care of later */
  if (lsm_meas_ptr->num_reports_ind == TRUE)
  {
    for (cnt = 0; cnt < TDSMAX_POS_MEAS; cnt++)
    {
      if (tdspos_meas_id_list[cnt].meas_id == lsm_meas_ptr->meas_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS: Meas %d deleted from DB",lsm_meas_ptr->meas_id);
        tdspos_meas_id_list[cnt].meas_id = TDSINVALID_MEAS_ID;
        break;
      }
    }
  }
  

  /* This places SDU into RLC watermark queue */
  status =  tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_MCMR, (void *) msg_ptr,
                                   ul_lc_id, l2_ack_status);

  
  
  if ( status != TDSRRCSEND_SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS:Send chain failed Meas Rpt");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:Rpt sent to RLC");
    if(lsm_meas_ptr->tranfer_mode == (uint8) TDSMODE_ACKNOWLEDGED)
    {
      tdsrrcmeas_pending_rpt++; 
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"pending meas rpt = %d",tdsrrcmeas_pending_rpt);
    }
  }
  
  return;
}


/*===========================================================================

FUNCTION    RRCGPS_SEND_LSM_CMD

DESCRIPTION

 Sends command to LSM to start Measurements

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_send_lsm_cmd ( tdsrrc_meas_params_struct_type* gps_meas_params_ptr )
{
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
  if(rrcgps_cgps_event_cb != NULL)
  {
    /*lint -e522 */
    tdsrrc_cgps_event_data_s_type    event_data;

    event_data.e_event_type = TDSRRC_CGPS_RRC_MSG_EVENT;
    event_data.u.z_rrc_cgps_msg_data.length = sizeof(tdsrrc_meas_params_struct_type);
    event_data.u.z_rrc_cgps_msg_data.p_data = (uint8 *)gps_meas_params_ptr;
    (void)rrcgps_cgps_event_cb(&event_data);
    /*lint +e522 */
  }
  else
  {
    ERR_FATAL("Callback func pointer is NULL",0,0,0);
  }

#endif
}


/*===========================================================================

FUNCTION    RRCGPS_SEND_MEAS_REPORT

DESCRIPTION

 Sets Measured Results received from LSM in ASN1 format

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_set_meas_results
(
  tdsrrc_gps_measured_results_struct_type* lsm_gps_meas_results_ptr,
  tdsrrc_UE_Positioning_GPS_MeasurementResults* rrc_gps_meas_results_ptr
)
{
  tdsrrc_GPS_MeasurementParamList * gps_meas_list_ptr = NULL;
  uint8 cnt = 0;
  
  if (lsm_gps_meas_results_ptr->positioning_estimate == (uint8)TDSGPS_REFERENCE_TIME_ONLY )
  {
    TDSRRC_SET_COMMON_MSG_TYPE(rrc_gps_meas_results_ptr->referenceTime,
      rrc_UE_Positioning_GPS_MeasurementResults_referenceTime_gps_ReferenceTimeOnly);
    
    rrc_gps_meas_results_ptr->referenceTime.u.gps_ReferenceTimeOnly =
      lsm_gps_meas_results_ptr->u.gps_reference_time;
    
  }
  else
  {
    TDSRRC_SET_COMMON_MSG_TYPE(rrc_gps_meas_results_ptr->referenceTime,
      rrc_UE_Positioning_GPS_MeasurementResults_referenceTime_utran_GPSReferenceTimeResult);
    
    if (lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.gps_ref_cpid_incl == TRUE)
    {
      TDSRRC_SET_COMMON_MSG_TYPE(rrc_gps_meas_results_ptr->referenceTime.u.utran_GPSReferenceTimeResult->modeSpecificInfo,
        rrc_UTRAN_GPSReferenceTime_modeSpecificInfo_fdd);
      
      rrc_gps_meas_results_ptr->referenceTime.u.utran_GPSReferenceTimeResult->modeSpecificInfo.
        u.fdd->referenceIdentity.primaryScramblingCode =
        lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.cpid;
    }
    
    /* SFN */
    rrc_gps_meas_results_ptr->referenceTime.u.utran_GPSReferenceTimeResult->sfn =
      lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.sfn;
    
    rrc_gps_meas_results_ptr->referenceTime.u.utran_GPSReferenceTimeResult->ue_GPSTimingOfCell.ls_part =
      lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.lsb_timing;
    
    rrc_gps_meas_results_ptr->referenceTime.u.utran_GPSReferenceTimeResult->ue_GPSTimingOfCell.ms_part =
      lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.msb_timing;
    
  } /* End of else of utran_GPSReferenceTimeResults */
  
  /* Now set measured params */
  if (lsm_gps_meas_results_ptr->no_of_satellites == 0)
  {
    return;
  }

    /* Assign head of list */
  gps_meas_list_ptr = &rrc_gps_meas_results_ptr->gps_MeasurementParamList;
  
  gps_meas_list_ptr->n = lsm_gps_meas_results_ptr->no_of_satellites;
  ALLOC_ASN1ARRAY1(&tdsenc_ctxt,gps_meas_list_ptr,tdsrrc_GPS_MeasurementParam);

  if(gps_meas_list_ptr->elem == NULL)
  {
    ERR_FATAL("Failed to allocate memory",0,0,0);
  }
  
  for (cnt =0; cnt < lsm_gps_meas_results_ptr->no_of_satellites; cnt++)
  {
    gps_meas_list_ptr->elem[cnt].c_N0 = lsm_gps_meas_results_ptr->gps_measured_params[cnt].c_no;
    gps_meas_list_ptr->elem[cnt].doppler = lsm_gps_meas_results_ptr->gps_measured_params[cnt].doppler;
    gps_meas_list_ptr->elem[cnt].fractionalGPS_Chips =
      lsm_gps_meas_results_ptr->gps_measured_params[cnt].fractional_gps_chips;
    gps_meas_list_ptr->elem[cnt].multipathIndicator =
      (tdsrrc_MultipathIndicator)lsm_gps_meas_results_ptr->gps_measured_params[cnt].multipath_ind;
    gps_meas_list_ptr->elem[cnt].pseudorangeRMS_Error =
      lsm_gps_meas_results_ptr->gps_measured_params[cnt].rms_error;
    gps_meas_list_ptr->elem[cnt].satelliteID =
      lsm_gps_meas_results_ptr->gps_measured_params[cnt].sat_id;
    gps_meas_list_ptr->elem[cnt].wholeGPS_Chips =
      lsm_gps_meas_results_ptr->gps_measured_params[cnt].whole_gps_chips;
  }
  
  return;
}

/*===========================================================================

FUNCTION    RRCGPS_SET_POS_ERROR

DESCRIPTION

 Sets Positioning Error received from LSM in ASN1 format

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_set_pos_error
(
  tdsrrc_positioning_error_struct_type* lsm_gps_meas_ptr,
  tdsrrc_UE_Positioning_Error* rrc_gps_pos_error_ptr
)
{
  uint8 cnt = 0;
  
  tdsrrc_SatDataList * sat_list_ptr = NULL;
  
  TDSRRC_RESET_MSG_IE_PRESENT(rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest);
  
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(rrc_gps_pos_error_ptr);
  
  /* Set error reason */
  
  switch(lsm_gps_meas_ptr->error_cause)
  {
    case 0: rrc_gps_pos_error_ptr->errorReason = tdsrrc_UE_Positioning_ErrorCause_notEnoughOTDOA_Cells;
        break;
    case 1: rrc_gps_pos_error_ptr->errorReason = tdsrrc_UE_Positioning_ErrorCause_notEnoughGPS_Satellites;
        break;
    case 2: rrc_gps_pos_error_ptr->errorReason = tdsrrc_UE_Positioning_ErrorCause_assistanceDataMissing;
        break;
    case 3: rrc_gps_pos_error_ptr->errorReason =tdsrrc_UE_Positioning_ErrorCause_notAccomplishedGPS_TimingOfCellFrames;
        break;
    case 4: rrc_gps_pos_error_ptr->errorReason = tdsrrc_UE_Positioning_ErrorCause_undefinedError;
        break;
    case 5: rrc_gps_pos_error_ptr->errorReason = tdsrrc_UE_Positioning_ErrorCause_requestDeniedByUser;
        break;
    case 6: rrc_gps_pos_error_ptr->errorReason = tdsrrc_UE_Positioning_ErrorCause_notProcessedAndTimeout;
        break;
    case 7: rrc_gps_pos_error_ptr->errorReason = tdsrrc_UE_Positioning_ErrorCause_referenceCellNotServingCell;
        break;
    default: break;/* no action */
  }
    
  if (lsm_gps_meas_ptr->addtl_assistance_data_incl)
  {
    TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(rrc_gps_pos_error_ptr,
        tdsrrc_UE_Positioning_Error,ue_positioning_GPS_additionalAssistanceDataRequest);
    
    if (lsm_gps_meas_ptr->addtl_assistance_data.almanac_request_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.almanacRequest = TRUE;
    }
    
    if (lsm_gps_meas_ptr->addtl_assistance_data.utc_model_request_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.utcModelRequest = TRUE;
    }
    
    if (lsm_gps_meas_ptr->addtl_assistance_data.ionospheric_model_request_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.ionosphericModelRequest = TRUE;
    }
    
    if (lsm_gps_meas_ptr->addtl_assistance_data.nac_model_data_req_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.navigationModelRequest = TRUE;
    }
    
    if (lsm_gps_meas_ptr->addtl_assistance_data.dgps_corrections_request_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.dgpsCorrectionsRequest = TRUE;
    }
    if (lsm_gps_meas_ptr->addtl_assistance_data.reference_location_request_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.referenceLocationRequest = TRUE;
    }
    if (lsm_gps_meas_ptr->addtl_assistance_data.reference_time_request_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.referenceTimeRequest = TRUE;
    }
    if (lsm_gps_meas_ptr->addtl_assistance_data.aquisition_assistance_request_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.aquisitionAssistanceRequest = TRUE;
    }
    if (lsm_gps_meas_ptr->addtl_assistance_data.real_time_integrity_request_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.realTimeIntegrityRequest = TRUE;
    }
    if (lsm_gps_meas_ptr->addtl_assistance_data.nac_model_data_req_incl == TRUE)
    {
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.m.navModelAddDataRequestPresent =1;
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.navigationModelRequest = TRUE;
      
      /* GPS Week */
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.navModelAddDataRequest.gps_Week =
        lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.gps_week;
      
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.navModelAddDataRequest.gps_Toe =
        lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.gps_tow;
      
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.navModelAddDataRequest.tToeLimit =
        lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.age_tolerance;
      
      rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.
        navModelAddDataRequest.satDataList.n = 0;
      
      
      if (lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.no_of_satellites == 0)
      {
        return;
      }
      
      /* Assign head of list */
      sat_list_ptr = &rrc_gps_pos_error_ptr->ue_positioning_GPS_additionalAssistanceDataRequest.
          navModelAddDataRequest.satDataList  ;
      sat_list_ptr->n =lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.no_of_satellites;

      ALLOC_ASN1ARRAY1(&tdsenc_ctxt,sat_list_ptr,tdsrrc_SatData);

      if(sat_list_ptr->elem == NULL)
      {
        ERR_FATAL("Failed to allocate memory",0,0,0);
      }    
     
      for (cnt =0; cnt < lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.no_of_satellites; cnt++)
      {
        sat_list_ptr->elem[cnt].satID = lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.sat_data[cnt].sat_id;
        sat_list_ptr->elem[cnt].iode = lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.sat_data[cnt].iode;
      }
    }
  }
}


/*===========================================================================

FUNCTION    RRCGPS_SET_POS_ESTIMATE

DESCRIPTION

 Sets Positioning Estimate received from LSM in ASN1 format

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_set_pos_estimate
(
  tdsrrc_positioning_estimate_struct_type* lsm_pos_est_ptr,
  tdsrrc_UE_Positioning_PositionEstimateInfo* rrc_gps_pos_est_ptr
)
{
  switch (lsm_pos_est_ptr->positioning_estimate)
  {
    case TDSCELL_TIMING_ONLY:
      TDSRRC_SET_COMMON_MSG_TYPE2(rrc_gps_pos_est_ptr->referenceTime,
          rrc_UE_Positioning_PositionEstimateInfo_referenceTime,cell_Timing);

      rrc_gps_pos_est_ptr->referenceTime.u.cell_Timing
          = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_UE_Positioning_PositionEstimateInfo_cell_Timing);
      rrc_gps_pos_est_ptr->referenceTime.u.cell_Timing->modeSpecificInfo.t =
        T_tdsrrc_UE_Positioning_PositionEstimateInfo_modeSpecificInfo_fdd;

      rrc_gps_pos_est_ptr->referenceTime.u.cell_Timing->modeSpecificInfo.u.fdd=
          rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_UE_Positioning_PositionEstimateInfo_fdd);
      rrc_gps_pos_est_ptr->referenceTime.u.cell_Timing->modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode =
        lsm_pos_est_ptr->u.cell_timing.cpid;
      
      rrc_gps_pos_est_ptr->referenceTime.u.cell_Timing->sfn = lsm_pos_est_ptr->u.cell_timing.sfn;
      
      break;
      
    case TDSGPS_REFERENCE_TIME_ONLY:
      TDSRRC_SET_COMMON_MSG_TYPE(rrc_gps_pos_est_ptr->referenceTime,
        rrc_UE_Positioning_PositionEstimateInfo_referenceTime_gps_ReferenceTimeOnly);
      
      rrc_gps_pos_est_ptr->referenceTime.u.gps_ReferenceTimeOnly = lsm_pos_est_ptr->u.gps_reference_time;
      
      break;
      
    case TDSUTRAN_GPS_REFERENCE_TIME:
      TDSRRC_SET_COMMON_MSG_TYPE(rrc_gps_pos_est_ptr->referenceTime,
        rrc_UE_Positioning_PositionEstimateInfo_referenceTime_utran_GPSReferenceTimeResult);

      rrc_gps_pos_est_ptr->referenceTime.u.utran_GPSReferenceTimeResult
          = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_UTRAN_GPSReferenceTimeResult);
      // TBD. Vivek gps_ref_psc_incl should be removed from interface
      if (lsm_pos_est_ptr->u.utran_gps_ref_time.gps_ref_cpid_incl == TRUE)
      {
        
        TDSRRC_SET_COMMON_MSG_TYPE(rrc_gps_pos_est_ptr->referenceTime.u.utran_GPSReferenceTimeResult->modeSpecificInfo,
          rrc_UTRAN_GPSReferenceTime_modeSpecificInfo_fdd);

        rrc_gps_pos_est_ptr->referenceTime.u.utran_GPSReferenceTimeResult->modeSpecificInfo.u.fdd
          = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_UTRAN_GPSReferenceTimeResult_fdd);
        rrc_gps_pos_est_ptr->referenceTime.u.utran_GPSReferenceTimeResult->modeSpecificInfo.u.fdd->
          referenceIdentity.primaryScramblingCode = lsm_pos_est_ptr->u.utran_gps_ref_time.cpid;
      }
      
      rrc_gps_pos_est_ptr->referenceTime.u.utran_GPSReferenceTimeResult->sfn = lsm_pos_est_ptr->u.utran_gps_ref_time.sfn;
      
      rrc_gps_pos_est_ptr->referenceTime.u.utran_GPSReferenceTimeResult->ue_GPSTimingOfCell.ls_part =
        lsm_pos_est_ptr->u.utran_gps_ref_time.lsb_timing;
      
      rrc_gps_pos_est_ptr->referenceTime.u.utran_GPSReferenceTimeResult->ue_GPSTimingOfCell.ms_part =
        lsm_pos_est_ptr->u.utran_gps_ref_time.msb_timing;
      
      break;
    default: 
      break;
  } /* End of switch */
  
  /* POS Estimate structure. Vivek Added type tdsrrc_gps_reference_loc_struct_type */
  
  
  switch (lsm_pos_est_ptr->pos_estimate_choice)
  {
    case TDSELLIPSOID_POINT:
      TDSRRC_SET_COMMON_MSG_TYPE(rrc_gps_pos_est_ptr->positionEstimate,rrc_PositionEstimate_ellipsoidPoint);
       rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPoint =
         rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_EllipsoidPoint);
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPoint->latitude =
        lsm_pos_est_ptr->gps_ref_location.latitude;
     
      if(lsm_pos_est_ptr->gps_ref_location.latitude_sign == 0)
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPoint->latitudeSign = tdsrrc_EllipsoidPoint_latitudeSign_north;
      }
      else
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPoint->latitudeSign = tdsrrc_EllipsoidPoint_latitudeSign_south;
      }
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPoint->longitude =
        lsm_pos_est_ptr->gps_ref_location.longitude;
      break;
      
    case TDSELLIPSOID_POINT_UNCERTAIN_CIRCLE:
      TDSRRC_SET_COMMON_MSG_TYPE2(rrc_gps_pos_est_ptr->positionEstimate,rrc_PositionEstimate,ellipsoidPointUncertCircle);
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertCircle
          =rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_EllipsoidPointUncertCircle);
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertCircle->latitude =
        lsm_pos_est_ptr->gps_ref_location.latitude;
      if(lsm_pos_est_ptr->gps_ref_location.latitude_sign == 0)
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertCircle->latitudeSign = tdsrrc_EllipsoidPointUncertCircle_latitudeSign_north;
      }
      else
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertCircle->latitudeSign = tdsrrc_EllipsoidPointUncertCircle_latitudeSign_south;
      }
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertCircle->longitude =
        lsm_pos_est_ptr->gps_ref_location.longitude;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertCircle->uncertaintyCode =
        lsm_pos_est_ptr->gps_ref_location.uncertainity_altitude;
      break;
      
    case TDSELLIPSOID_POINT_UNCERTAIN_ELLIPSE:
      TDSRRC_SET_COMMON_MSG_TYPE2(rrc_gps_pos_est_ptr->positionEstimate,rrc_PositionEstimate,ellipsoidPointUncertEllipse);

      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertEllipse
          =rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_EllipsoidPointUncertEllipse);
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertEllipse->confidence =
        lsm_pos_est_ptr->gps_ref_location.confidence;
      
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertEllipse->latitude =
        lsm_pos_est_ptr->gps_ref_location.latitude;
      if(lsm_pos_est_ptr->gps_ref_location.latitude_sign == 0)
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertEllipse->latitudeSign = tdsrrc_EllipsoidPointUncertEllipse_latitudeSign_north;
      }
      else
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPoint->latitudeSign = tdsrrc_EllipsoidPointUncertEllipse_latitudeSign_south;
      }
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertEllipse->longitude =
        lsm_pos_est_ptr->gps_ref_location.longitude;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertEllipse->orientationMajorAxis =
        lsm_pos_est_ptr->gps_ref_location.orientation_major_axis;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertEllipse->uncertaintySemiMajor =
        lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_major;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointUncertEllipse->uncertaintySemiMinor =
        lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_minor;
      break;
      
    case TDSELLIPSOID_POINT_ALTITUDE:
      TDSRRC_SET_COMMON_MSG_TYPE2(rrc_gps_pos_est_ptr->positionEstimate,rrc_PositionEstimate,ellipsoidPointAltitude);
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitude
          =rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_EllipsoidPointAltitude);
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitude->altitude =
        lsm_pos_est_ptr->gps_ref_location.altitude;
      if(lsm_pos_est_ptr->gps_ref_location.altitude_direction == 0)
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitude->altitudeDirection = tdsrrc_EllipsoidPointAltitude_altitudeDirection_height;
      }
      else
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitude->altitudeDirection = tdsrrc_EllipsoidPointAltitude_altitudeDirection_depth;
      }
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitude->latitude =
        lsm_pos_est_ptr->gps_ref_location.latitude;
      if(lsm_pos_est_ptr->gps_ref_location.latitude_sign == 0)
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitude->latitudeSign = tdsrrc_EllipsoidPointAltitude_latitudeSign_north;
      }
      else
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitude->latitudeSign = tdsrrc_EllipsoidPointAltitude_latitudeSign_south;
      }
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitude->longitude =
        lsm_pos_est_ptr->gps_ref_location.longitude;
      break;
      
    case TDSELLIPSOID_POINT_ALTITUDE_ELLIPSE:
      TDSRRC_SET_COMMON_MSG_TYPE2(rrc_gps_pos_est_ptr->positionEstimate,rrc_PositionEstimate,ellipsoidPointAltitudeEllipse);
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse
          =rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_EllipsoidPointAltitudeEllipsoide);
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->confidence =
        lsm_pos_est_ptr->gps_ref_location.confidence;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->latitude =
        lsm_pos_est_ptr->gps_ref_location.latitude;
      if(lsm_pos_est_ptr->gps_ref_location.latitude_sign == 0)
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->latitudeSign = tdsrrc_EllipsoidPointAltitudeEllipsoide_latitudeSign_north;
      }
      else
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->latitudeSign = tdsrrc_EllipsoidPointAltitudeEllipsoide_latitudeSign_south;
      }
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->longitude =
        lsm_pos_est_ptr->gps_ref_location.longitude;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->orientationMajorAxis =
        lsm_pos_est_ptr->gps_ref_location.orientation_major_axis;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->uncertaintySemiMajor =
        lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_major;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->uncertaintySemiMinor =
        lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_minor;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->altitude =
        lsm_pos_est_ptr->gps_ref_location.altitude;
      rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->uncertaintyAltitude =
        lsm_pos_est_ptr->gps_ref_location.uncertainity_altitude;
      if(lsm_pos_est_ptr->gps_ref_location.altitude_direction == 0)
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->altitudeDirection = tdsrrc_EllipsoidPointAltitudeEllipsoide_altitudeDirection_height;
      }
      else
      {
        rrc_gps_pos_est_ptr->positionEstimate.u.ellipsoidPointAltitudeEllipse->altitudeDirection = tdsrrc_EllipsoidPointAltitudeEllipsoide_altitudeDirection_depth;
      }
      break;

    default: 
      break; 
  }

}


/*===========================================================================

FUNCTION    RRCGPS_PROCESS_ASD_MSG

DESCRIPTION

 Processes Assistance Data Delievery Message received from UTRAN

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_process_asd_msg
(
  tdsrrc_cmd_type* cmd_ptr
)
{
  uint8 trans_id = 0;
  
  uint8 error_choice = 0;
  
  tdsrrc_meas_params_struct_type lsm_assistance_data;
  
  /* Declare ptr to store message received from UTRAN */
  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;
  
  /* Declare ptr to point to actual IE's in Meas Ctrl Msg */
  tdsrrc_AssistanceDataDelivery *assistance_data_delievery_ptr = NULL;
  
  lsm_assistance_data.params_type = (uint8)TDSRRC_ASSISTANCE_DATA_PARMS;
  
  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;
  
  if ( dcch_msg_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS:ASN1 decoding failed");
    
    /* Transaction ID already set to 0 */
    
    error_choice = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    
    tdsrrcmeas_send_rrc_status_msg(&trans_id, &error_choice);
    
    return;
  }
  
  if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.assistanceDataDelivery,
    rrc_AssistanceDataDelivery_r3 )))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS:Critical extensions not supported");
    
    error_choice = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
    
    /* Transaction ID already set to 0. Send Failure Msg */
    tdsrrcmeas_send_rrc_status_msg(&trans_id, &error_choice);
    return ;
  }
  
  /* Get the pointer to the actual MC message */
  assistance_data_delievery_ptr = &(dcch_msg_ptr->message.u.assistanceDataDelivery);
  
  trans_id = assistance_data_delievery_ptr->u.r3.assistanceDataDelivery_r3.rrc_TransactionIdentifier;
  if(assistance_data_delievery_ptr->u.r3.assistanceDataDelivery_r3.m.ue_positioning_GPS_AssistanceDataPresent  )
  {
  /* No need to store it in transaction table */
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
    if (tdsrrcgps_convert_assistance_data(&assistance_data_delievery_ptr->u.r3.assistanceDataDelivery_r3.
      ue_positioning_GPS_AssistanceData,
      &lsm_assistance_data.u.assistance_data_params) == FALSE)
#else /* this will only be FEATURE_MMGPS */ 
    if (lsm_convert_assistance_data(&assistance_data_delievery_ptr->u.r3.assistanceDataDelivery_r3.
      ue_positioning_GPS_AssistanceData,
      &lsm_assistance_data.u.assistance_data_params) == FALSE)
#endif
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Assistance Data conversion failed");
      
      error_choice = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      
      /* Send Failure Message */
      tdsrrcmeas_send_rrc_status_msg(&trans_id, &error_choice);
      
      return;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Assistance Data not present. Ignoring ADD msg.");
    return;
  }
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending Assistance Meas Request");
  tdsrrcgps_send_lsm_cmd(&lsm_assistance_data);
  
  return;
}


/*===========================================================================

FUNCTION    RRCGPS_VALIDATE_MEAS_RESULTS

DESCRIPTION

 Validates Measured Results received from LSM

DEPENDENCIES

 None.

RETURN VALUE

 True/False

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcgps_validate_meas_results
(
  tdsrrc_gps_measured_results_struct_type* lsm_gps_meas_results_ptr
)
{
  uint8 cnt = 0;
  
  if (lsm_gps_meas_results_ptr->positioning_estimate == (uint8)TDSGPS_REFERENCE_TIME_ONLY )
  {
    /* TBD Check to be added */
    // lsm_gps_meas_results_ptr->u.gps_reference_time;
  }
  else
  {
    if (lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.gps_ref_cpid_incl == TRUE)
    {
      if (lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.cpid > TDSCPID_MAX)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: CPID %d invalid 0..127",lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.cpid);
        return FALSE;
      }
    }
    
    /* SFN */
    if (lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.sfn > TDSMAX_SFN)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: SFN %d invalid 0..4095",lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.sfn);
      return FALSE;
    }
    
    if (lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.msb_timing > TDSGPS_MSB_TIMING)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:GPS MSB timing %d invalid 0..16383",lsm_gps_meas_results_ptr->u.gps_umts_meas_results.gps_umts_reference_time.msb_timing);
      return FALSE;
    }
    
  } /* End of else of utran_GPSReferenceTimeResults */
  
  /* Now set measured params */
  if (lsm_gps_meas_results_ptr->no_of_satellites == 0)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: No of satellites %d invalid 1..16",lsm_gps_meas_results_ptr->no_of_satellites);
    return FALSE;
  }
  for (cnt =0; cnt < lsm_gps_meas_results_ptr->no_of_satellites; cnt++)
  {
    if (lsm_gps_meas_results_ptr->gps_measured_params[cnt].c_no > TDSMAX_C_NO)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:C_NO %d invalid 0..63",lsm_gps_meas_results_ptr->gps_measured_params[cnt].c_no);
      return FALSE;
      
    }
    if ((lsm_gps_meas_results_ptr->gps_measured_params[cnt].doppler > TDSMAX_DOPPLER) ||
      (lsm_gps_meas_results_ptr->gps_measured_params[cnt].doppler < TDSMIN_DOPPLER))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: Doppler %d invalid -32768...32768",lsm_gps_meas_results_ptr->gps_measured_params[cnt].doppler);
      return FALSE;
    }
    if (lsm_gps_meas_results_ptr->gps_measured_params[cnt].fractional_gps_chips > TDSMAX_FRAC_GPS_CHIPS)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: Fractional GPS Chips %d invalid ",lsm_gps_meas_results_ptr->gps_measured_params[cnt].fractional_gps_chips);
      return FALSE;
    }
    if (lsm_gps_meas_results_ptr->gps_measured_params[cnt].multipath_ind > TDSMULTIPATH_VALUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Multipath %d invalid 0..3",lsm_gps_meas_results_ptr->gps_measured_params[cnt].multipath_ind);
      return FALSE;
    }
    if (lsm_gps_meas_results_ptr->gps_measured_params[cnt].rms_error > TDSMAX_RMS_ERROR)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:RMS Error Value %d invalid 0..63",lsm_gps_meas_results_ptr->gps_measured_params[cnt].rms_error);
      return FALSE;
    }
    if (lsm_gps_meas_results_ptr->gps_measured_params[cnt].sat_id > TDSMAX_SAT_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Satellite Id %d invalid 0..63",lsm_gps_meas_results_ptr->gps_measured_params[cnt].sat_id);
      return FALSE;
    }
    if (lsm_gps_meas_results_ptr->gps_measured_params[cnt].whole_gps_chips > TDSMAX_GPS_CHIPS)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Max GPS Chips %d invalid 0..1022",lsm_gps_meas_results_ptr->gps_measured_params[cnt].whole_gps_chips);
      return FALSE;
    }
  }
  return TRUE;
}


/*===========================================================================

FUNCTION    RRCGPS_VALIDATE_POS_ESTIMATE

DESCRIPTION

 Validates Positioning Estimate received from LSM

DEPENDENCIES

 None.

RETURN VALUE

 True/False

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcgps_validate_pos_estimate
(
  tdsrrc_positioning_estimate_struct_type* lsm_pos_est_ptr
)
{
  switch (lsm_pos_est_ptr->positioning_estimate)
  {
    case TDSCELL_TIMING_ONLY:
      if (lsm_pos_est_ptr->u.cell_timing.cpid > TDSCPID_MAX)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: cpid %d invalid 0..127",lsm_pos_est_ptr->u.cell_timing.cpid);
        return FALSE;
      }
      if (lsm_pos_est_ptr->u.cell_timing.sfn > TDSMAX_SFN)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: SFN %d invalid 0..4095",lsm_pos_est_ptr->u.cell_timing.sfn);
        return FALSE;
      }
      break;
      
    case TDSGPS_REFERENCE_TIME_ONLY:    
      break;
      
    case TDSUTRAN_GPS_REFERENCE_TIME:
      // TBD. Vivek gps_ref_psc_incl should be removed from interface
      if (lsm_pos_est_ptr->u.utran_gps_ref_time.gps_ref_cpid_incl == TRUE)
      {
        
        if (lsm_pos_est_ptr->u.utran_gps_ref_time.cpid > TDSCPID_MAX)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:cpid %d invalid 0..127",lsm_pos_est_ptr->u.utran_gps_ref_time.cpid);
          return FALSE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: CPID missing");
        return FALSE;
      }
      if (lsm_pos_est_ptr->u.utran_gps_ref_time.sfn > TDSMAX_SFN)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:SFN %d invalid 0..4095",lsm_pos_est_ptr->u.utran_gps_ref_time.sfn);
        return FALSE;
      }
      if (lsm_pos_est_ptr->u.utran_gps_ref_time.msb_timing > TDSGPS_MSB_TIMING)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:MSB Timing %d invalid 0..16383",lsm_pos_est_ptr->u.utran_gps_ref_time.msb_timing);
        return FALSE;
      }
      break;
      
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: Invalid Pos Estimate Choice",lsm_pos_est_ptr->positioning_estimate);
      return FALSE;
  }
  
  switch (lsm_pos_est_ptr->pos_estimate_choice)
  {
    case TDSELLIPSOID_POINT:
      if (lsm_pos_est_ptr->gps_ref_location.latitude > TDSMAX_LATITUDE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Latitude %d invalid 0..8388607",lsm_pos_est_ptr->gps_ref_location.latitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.latitude_sign > TDSMAX_LATITUDE_SIGN)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Latitude Sign %d invalid 0..1",lsm_pos_est_ptr->gps_ref_location.latitude_sign);
        return FALSE;
      }
      if ((lsm_pos_est_ptr->gps_ref_location.longitude < TDSMIN_LONGITUDE) ||
        (lsm_pos_est_ptr->gps_ref_location.longitude > TDSMAX_LONGITUDE))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Longitude %d invalid -8388608 .. 83886080",lsm_pos_est_ptr->gps_ref_location.longitude);
        return FALSE;
      }
      break;
      
      
    case TDSELLIPSOID_POINT_UNCERTAIN_CIRCLE:
    case TDSELLIPSOID_POINT_ALTITUDE:
      if (lsm_pos_est_ptr->gps_ref_location.altitude > TDSMAX_ALTITUDE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Altitude %d invalid 0..32767",lsm_pos_est_ptr->gps_ref_location.altitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.altitude_direction > TDSMAX_ALTITUDE_DIRECTION)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Altitude direction %d invalid 0..1",lsm_pos_est_ptr->gps_ref_location.altitude_direction);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.latitude > TDSMAX_LATITUDE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Latitude %d invalid 0..8388607",lsm_pos_est_ptr->gps_ref_location.latitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.latitude_sign > TDSMAX_LATITUDE_SIGN)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Latitude Sign %d invalid 0..1",lsm_pos_est_ptr->gps_ref_location.latitude_sign);
        return FALSE;
      }
      if ((lsm_pos_est_ptr->gps_ref_location.longitude < TDSMIN_LONGITUDE) ||
        (lsm_pos_est_ptr->gps_ref_location.longitude > TDSMAX_LONGITUDE))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Longitude %d invalid -8388608 .. 83886080",lsm_pos_est_ptr->gps_ref_location.longitude);
        return FALSE;
      }
      break;
      
    case TDSELLIPSOID_POINT_ALTITUDE_ELLIPSE:
      if (lsm_pos_est_ptr->gps_ref_location.confidence > TDSMAX_CONFIDENCE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Confidence %d invalid 0..100",lsm_pos_est_ptr->gps_ref_location.confidence);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.latitude > TDSMAX_LATITUDE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Latitude %d invalid 0..8388607",lsm_pos_est_ptr->gps_ref_location.latitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.latitude_sign > TDSMAX_LATITUDE_SIGN)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Latitude %d invalid 0..8388607",lsm_pos_est_ptr->gps_ref_location.latitude_sign);
        return FALSE;
      }
      if ((lsm_pos_est_ptr->gps_ref_location.longitude < TDSMIN_LONGITUDE) ||
        (lsm_pos_est_ptr->gps_ref_location.longitude > TDSMAX_LONGITUDE))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Longitude %d invalid -8388608 .. 83886080",lsm_pos_est_ptr->gps_ref_location.longitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_major > TDSUNCERTAINITY_SEMI_MAJOR)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:UncertSemiMajor %d invalid 0..127",lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_major);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_minor > TDSUNCERTAINITY_SEMI_MINOR)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:UncertSemiMinor %d invalid 0..127",lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_minor);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.orientation_major_axis > TDSORIENTATION_MAJOR_AXIS)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:OrientationMajorAxis %d invalid 0..89",lsm_pos_est_ptr->gps_ref_location.orientation_major_axis);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.altitude > TDSMAX_ALTITUDE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Altitude %d invalid 0..32767",lsm_pos_est_ptr->gps_ref_location.altitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.uncertainity_altitude > TDSMAX_UNCERTAINITY_ALTITUDE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Uncert.Altitude %d invalid 0..127",lsm_pos_est_ptr->gps_ref_location.uncertainity_altitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.altitude_direction > TDSMAX_ALTITUDE_DIRECTION)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Max Altitude dir %d invalid 0..1",lsm_pos_est_ptr->gps_ref_location.altitude_direction);
        return FALSE;
      }
      break;
      
    case TDSELLIPSOID_POINT_UNCERTAIN_ELLIPSE:
      
      if (lsm_pos_est_ptr->gps_ref_location.confidence > TDSMAX_CONFIDENCE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Confidence %d invalid 0..100",lsm_pos_est_ptr->gps_ref_location.confidence);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.latitude > TDSMAX_LATITUDE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Latitude %d invalid 0..8388607",lsm_pos_est_ptr->gps_ref_location.latitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.latitude_sign > TDSMAX_LATITUDE_SIGN)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Latitude %d invalid 0..8388607",lsm_pos_est_ptr->gps_ref_location.latitude_sign);
        return FALSE;
      }
      if ((lsm_pos_est_ptr->gps_ref_location.longitude < TDSMIN_LONGITUDE) ||
        (lsm_pos_est_ptr->gps_ref_location.longitude > TDSMAX_LONGITUDE))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Longitude %d invalid -8388608 .. 83886080",lsm_pos_est_ptr->gps_ref_location.longitude);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_major > TDSUNCERTAINITY_SEMI_MAJOR)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:UncertSemiMajor %d invalid 0..127",lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_major);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_minor > TDSUNCERTAINITY_SEMI_MINOR)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:UncertSemiMinor %d invalid 0..127",lsm_pos_est_ptr->gps_ref_location.uncertainity_semi_minor);
        return FALSE;
      }
      if (lsm_pos_est_ptr->gps_ref_location.orientation_major_axis > TDSORIENTATION_MAJOR_AXIS)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:OrientationMajorAxis %d invalid 0..89",lsm_pos_est_ptr->gps_ref_location.orientation_major_axis);
        return FALSE;
      }
      break;
      
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Estimate Choice %d invalid",lsm_pos_est_ptr->pos_estimate_choice);
      return FALSE;
  }
  
  return TRUE;
}


/*===========================================================================

FUNCTION    RRCGPS_VALIDATE_POS_ERROR

DESCRIPTION

 Validates Positioning Error received from LSM

DEPENDENCIES

 None.

RETURN VALUE

 True/False

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcgps_validate_pos_error
(
  tdsrrc_positioning_error_struct_type* lsm_gps_meas_ptr
)
{
  uint8 cnt = 0;
  
  if (lsm_gps_meas_ptr->addtl_assistance_data_incl == TRUE)
  {
    if (lsm_gps_meas_ptr->addtl_assistance_data.nac_model_data_req_incl == TRUE)
    {
      
      if (lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.gps_week > TDSMAX_GPS_WEEK)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: GPS Week %d invalid 0..1023",lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.gps_week);
        return FALSE;
      }
      
      if (lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.gps_tow> TDSMAX_GPS_TOW)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: GPS TOW %d invalid 0..167",lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.gps_tow);
        return FALSE;
      }
      
      if (lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.age_tolerance> TDSGPS_AGE_OF_TOLERANCE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: GPS Age of Tolerance %d invalid 0..10",lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.age_tolerance);
        return FALSE;
      }
      
      if (lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.no_of_satellites == 0)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS: No of sat %d invalid 0..10",lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.no_of_satellites);
        return FALSE;
      }
      
      for (cnt =0; cnt < lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.no_of_satellites; cnt++)
      {
        if (lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.sat_data[cnt].sat_id > TDSMAX_SAT_ID)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"POS:Sat Id %d invalid 0..63",lsm_gps_meas_ptr->addtl_assistance_data.navModelAddDataRequest.sat_data[cnt].sat_id);
          return FALSE;
        }
        
      }
    }
  }
  return TRUE;
}


/*===========================================================================

FUNCTION    RRCGPS_VERIFY_POSITIONING_CAPABILITY

DESCRIPTION

 Checks positioning capability to determine if UE could process
 A-GPS Measurement Control Message

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_verify_positioning_capability
(
  const tdsrrc_UE_Positioning_ReportingQuantity *rpt_quan_ptr
)
{

  tdsrrc_positioning_method_type_enum_type pos_method_type;
  tm_umts_cp_tds_ue_pos_capability_struct_type pos_capability;  
  
  uecomdef_status_e_type verify_status = SUCCESS;

  pos_method_type = TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
  
  /* First get positioning capability from LSM */
  if(tdsrrcgps_cgps_ue_pos_capability_cb == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"GPS Call back function is NULL");
    return FAILURE;
  }
  else
  {
    tdsrrcgps_cgps_ue_pos_capability_cb(&pos_capability);
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:LSM returned capability %d",pos_capability.network_assisted_gps_support);
#ifdef FEATURE_TDSCDMA_CGPS_TIMING_OF_CELL_RULE_ENFORCED
  if ( ( rpt_quan_ptr->gps_TimingOfCellWanted == TRUE  ) &&
       ( pos_capability.gps_timing_of_cell_frames_supported == FALSE )
     )
  {
    verify_status = FAILURE;
  }
#endif
  if ((rpt_quan_ptr->positioningMethod == tdsrrc_PositioningMethod_otdoa) &&
      (pos_capability.ue_based_otdoa_supported == FALSE))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Pos Method OTDOA unsupported");
    verify_status = FAILURE;
  }

  if ( rpt_quan_ptr->positioningMethod == tdsrrc_PositioningMethod_cellID )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Pos Method CELL ID unsupported");
    verify_status = FAILURE;
  }

  if ( verify_status == SUCCESS ) /* continue examining if still good */
  {

    switch ( pos_capability.network_assisted_gps_support )
    {
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_UE_BASED:
#endif
      {
        /* Reject any UE Assisted Requests */
        if (pos_method_type == TDSRRC_UE_ASSISTED )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=UE BASED,Req=UE_ASSISTED");
        }

        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_NETWORK_BASED:
#endif
      {
        /* Reject UE Based if we don't support standalone */
        if ( ( pos_method_type == TDSRRC_UE_BASED ) && 
             ( pos_capability.standalone_location_method_supported == FALSE ) )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=NW BASED, Req=UE_BASED,No Standalone");
        }
        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_BOTH:
#endif
      {
        /* Never Reject */
        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_NONE:
#endif
      {
        /* Reject everything unless UE Based is requested and standalone is supported */
        if ( ( pos_method_type == TDSRRC_UE_ASSISTED ) || 
             ( pos_capability.standalone_location_method_supported == FALSE ) )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=NONE,Req=UE_ASSISTED or No Standalone");
        }
        break;
      }

      default:
        break;
    }

  } /* if verify_status is still SUCCESS */

  if ( verify_status == SUCCESS )
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:UE cap check passed");
  else
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:UE cap check failed");
  
  return verify_status;
}

/*===========================================================================

FUNCTION    RRCGPS_VERIFY_POSITIONING_CAPABILITY

DESCRIPTION

 Checks positioning capability to determine if UE could process
 A-GPS Measurement Control Message

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_verify_positioning_capability_r5
(
  const tdsrrc_UE_Positioning_ReportingQuantity_r4 *rpt_quan_ptr
)
{

  tdsrrc_positioning_method_type_enum_type pos_method_type;

  tm_umts_cp_tds_ue_pos_capability_struct_type pos_capability;  
  
  uecomdef_status_e_type verify_status = SUCCESS;

  
  pos_method_type = TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
  
  /* First get positioning capability from LSM */
  if(tdsrrcgps_cgps_ue_pos_capability_cb == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"GPS Call back function is NULL");
    return FAILURE;
  }
  else
  {
    tdsrrcgps_cgps_ue_pos_capability_cb(&pos_capability);
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:LSM/TM returned capability %d",pos_capability.network_assisted_gps_support);

#ifdef FEATURE_TDSCDMA_CGPS_TIMING_OF_CELL_RULE_ENFORCED
  if ( ( rpt_quan_ptr->gps_TimingOfCellWanted == TRUE  ) &&
       ( pos_capability.gps_timing_of_cell_frames_supported == FALSE )
     )
  {
    verify_status = FAILURE;
  }
#endif  
  if ((rpt_quan_ptr->positioningMethod == tdsrrc_PositioningMethod_otdoa) &&
      (pos_capability.ue_based_otdoa_supported == FALSE))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Pos Method OTDOA unsupported");
    verify_status = FAILURE;
  }

  if ( rpt_quan_ptr->positioningMethod == tdsrrc_PositioningMethod_cellID )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Pos Method CELL ID unsupported");
    verify_status = FAILURE;
  }

  if ( verify_status == SUCCESS ) /* continue examining if still good */
  {

    switch ( pos_capability.network_assisted_gps_support )
    {
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_UE_BASED:
#endif
      {
        /* Reject any UE Assisted Requests */
        if (pos_method_type == TDSRRC_UE_ASSISTED )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=UE BASED,Req=UE_ASSISTED");
        }

        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_NETWORK_BASED:
#endif
      {
        /* Reject UE Based if we don't support standalone */
        if ( ( pos_method_type == TDSRRC_UE_BASED ) && 
             ( pos_capability.standalone_location_method_supported == FALSE ) )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=NW BASED, Req=UE_BASED,No Standalone");
        }
        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_BOTH:
#endif
      {
        /* Never Reject */
        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_NONE:
#endif
      {
        /* Reject everything unless UE Based is requested and standalone is supported */
        if ( ( pos_method_type == TDSRRC_UE_ASSISTED ) || 
             ( pos_capability.standalone_location_method_supported == FALSE ) )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=NONE,Req=UE_ASSISTED or No Standalone");
        }
        break;
      }

      default:
        break;
    }

  } /* if verify_status is still SUCCESS */

  if ( verify_status == SUCCESS )
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:UE cap check passed");
  else
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:UE cap check failed");
  
  return verify_status;
}

/*===========================================================================

FUNCTION   RRCGPS_PROCESS_DCH_TO_FACH_PCH

DESCRIPTION 

  DCH->FACH transition and DCH->PCH
  

DEPENDENCIES

  None

RETURN VALUE

  None. 

SIDE EFFECTS

===========================================================================*/

void tdsrrcgps_process_dch_to_fach_pch
(
 void
)
{
  uint8 count = 0;
  
  tdsrrc_meas_params_struct_type pos_cmd;
  
  pos_cmd.params_type = (uint8)TDSRRC_STATE_CHANGE_PARMS;
  
  /* Check the Validity in Measurement Identity variable for  position Meas */  
  for (count = 0; count < TDSMAX_POS_MEAS; count++)
  {
    if (tdspos_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID)
    {
      switch(tdspos_meas_id_list[count].meas_validity)
      {
      case TDSNOT_PRESENT:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity absent. Delete Meas = %d",tdspos_meas_id_list[count].meas_id);
        
        /* Cmd type */
        pos_cmd.u.trans_params.trans_cmd_type = (uint8)TDSRRC_MEAS_DELETE;
        
        /* Meas Id */
        pos_cmd.u.trans_params.meas_id = tdspos_meas_id_list[count].meas_id;
        
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete POS meas = %d,Indicate LSM ",pos_cmd.u.trans_params.meas_id);
        
        /* Set Meas Id in Meas Identity var to Invalid */
        tdspos_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        
        tdspos_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        
        tdspos_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        tdsrrcgps_send_lsm_cmd(&pos_cmd);
        
        break;  /* Come out of switch and find if any other meas needs to be deleted */
        
      case TDSALL_STATES:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity ALL. Continue meas = %d",tdspos_meas_id_list[count].meas_id);
#ifdef FEATURE_LSM_PCH_REPORTING
        #error code not present
#endif        
        /* No Need to send cmd to LSM */
        break;
        
      case TDSSTATE_DCH:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity DCH only. Stop,Save meas= %d",tdspos_meas_id_list[count].meas_id);
        
        /* Set Meas Id */
        pos_cmd.u.trans_params.meas_id = tdspos_meas_id_list[count].meas_id;
        
        /* Stop the Measurement */
        pos_cmd.u.trans_params.trans_cmd_type = (uint8)TDSRRC_MEAS_SAVE;
        
        tdsrrcgps_send_lsm_cmd(&pos_cmd);
        
        break;
        
      case TDSALL_STATES_BUT_DCH:
#ifdef FEATURE_LSM_PCH_REPORTING
        #error code not present
#endif
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity All except DCH. Resume meas= %d",tdspos_meas_id_list[count].meas_id);
        
        /* Set Meas Id */
        pos_cmd.u.trans_params.meas_id = tdspos_meas_id_list[count].meas_id;
        
        /* Resume the Measurement */
        pos_cmd.u.trans_params.trans_cmd_type = (uint8)TDSRRC_MEAS_RESUME;
        
        tdsrrcgps_send_lsm_cmd(&pos_cmd);
        
        break; 
        
      default:
        /* Not possible */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid meas validity in tdspos_meas_id_list");
        return;
        
      } /* End of switch */
    } /* end of if */
    
  } /* End of for */
}


/*===========================================================================

FUNCTION   RRCGPS_PROCESS_FACH_TO_DCH

DESCRIPTION 

  DCH->FACH transition
  

DEPENDENCIES

  None

RETURN VALUE

  None. 

SIDE EFFECTS

  

===========================================================================*/

void tdsrrcgps_process_fach_to_dch
(
 void
)
{
  uint8 count = 0;
  
  tdsrrc_meas_params_struct_type pos_cmd;
  
  pos_cmd.params_type = (uint8)TDSRRC_STATE_CHANGE_PARMS;
  
  /* Check the Validity in Measurement Identity variable for Pos Meas */  
  for (count = 0; count < TDSMAX_POS_MEAS; count++)
  {
    if (tdspos_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID)
    {
      switch(tdspos_meas_id_list[count].meas_validity)
      {
      case TDSNOT_PRESENT:
        
        break;  /* Come out of switch and find if any other meas needs to be deleted */
        
      case TDSALL_STATES:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity ALL. Continue meas = %d",tdspos_meas_id_list[count].meas_id);
        
        /* No Need to send cmd to LSM */
        break;
        
      case TDSSTATE_DCH:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity DCH only. Resume meas= %d",tdspos_meas_id_list[count].meas_id);
        
        /* Set Meas Id */
        pos_cmd.u.trans_params.meas_id = tdspos_meas_id_list[count].meas_id;
        
        /* Stop the Measurement */
        pos_cmd.u.trans_params.trans_cmd_type = (uint8)TDSRRC_MEAS_RESUME;
        
        tdsrrcgps_send_lsm_cmd(&pos_cmd);
        
        break;
        
      case TDSALL_STATES_BUT_DCH:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity All except DCH. Stop,Save meas= %d",tdspos_meas_id_list[count].meas_id);
        
        /* Set Meas Id */
        pos_cmd.u.trans_params.meas_id = tdspos_meas_id_list[count].meas_id;
        
        /* Resume the Measurement */
        pos_cmd.u.trans_params.trans_cmd_type = (uint8)TDSRRC_MEAS_SAVE;
        
        tdsrrcgps_send_lsm_cmd(&pos_cmd);
        
        break; 
        
      default:
        /* Not possible */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid meas validity in tdspos_meas_id_list");
        return;
        
      } /* End of switch */
    } /* end of if */
    
  } /* End of for */
}

/*===========================================================================

FUNCTION   RRCGPS_PROCESS_FACH_TO_PCH

DESCRIPTION 

  FACH->PCH transition
  

DEPENDENCIES

  None

RETURN VALUE

  None. 

SIDE EFFECTS

  

===========================================================================*/

void tdsrrcgps_process_fach_to_pch
(
 void
)
{
  uint8 count = 0;
  
  tdsrrc_meas_params_struct_type pos_cmd;
  
  pos_cmd.params_type = (uint8)TDSRRC_STATE_CHANGE_PARMS;
  
  /* Check the Validity in Measurement Identity variable for Traffic Vol Meas */  
  for (count = 0; count < TDSMAX_POS_MEAS; count++)
  {
    if (tdspos_meas_id_list[count].meas_id != TDSINVALID_MEAS_ID)
    {
      switch(tdspos_meas_id_list[count].meas_validity)
      {
      case TDSNOT_PRESENT:
        
        break;  /* Come out of switch and find if any other meas needs to be deleted */
        
      case TDSALL_STATES:
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity ALL. Continue meas = %d",tdspos_meas_id_list[count].meas_id);
        
        if (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
        {
          /* send cmd to LSM to change the reporting interval to 64s */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: change rpt interval to 64s for PCH");
          pos_cmd.u.trans_params.trans_cmd_type = (uint8)TDSRRC_CHANGE_PERIODIC_INTERVAL_TO_PCH;
          
          /* Meas Id */
          pos_cmd.u.trans_params.meas_id = tdspos_meas_id_list[count].meas_id;
          tdsrrcgps_send_lsm_cmd(&pos_cmd);
        }
        
        break;
        
      case TDSSTATE_DCH:
        /* no cmd is required as the measurement has already been saved */
        break;
        
      case TDSALL_STATES_BUT_DCH:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Validity All except DCH. Continue meas= %d",tdspos_meas_id_list[count].meas_id);
        
        if (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
        {
          /* send cmd to LSM to change the reporting interval to 64s */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: change rpt interval to 64s for PCH");
          pos_cmd.u.trans_params.trans_cmd_type = (uint8)TDSRRC_CHANGE_PERIODIC_INTERVAL_TO_PCH;
          
          /* Meas Id */
          pos_cmd.u.trans_params.meas_id = tdspos_meas_id_list[count].meas_id;
          tdsrrcgps_send_lsm_cmd(&pos_cmd);
        }
        
        break; 
        
      default:
        /* Not possible */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid meas validity in tdspos_meas_id_list");
        return;
        
      } /* End of switch */
    } /* end of if */
    
  } /* End of for */
}



/*===========================================================================

FUNCTION   RRCGPS_FILL_MCM_R5_INFO

DESCRIPTION

    This function fill the GPS structure for 
    A-GPS Measurements. 

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_fill_mcm_info_r5
(
  tdspos_info_struct_type* pos_info_ptr,                       
  tdsrrc_UE_Positioning_Measurement_r4 *up_Measurement_ptr,
  tdsrrc_meas_ctrl_struct_type *meas_ctrl_params_ptr,
  uint8 tdstransaction_id,
  boolean modify_meas_params_incl
)
{
  tdsmeas_validity_enum_type pos_meas_validity = TDSNOT_PRESENT;
  
  tdsrrc_UE_Positioning_ReportingQuantity_r4* rpt_quan_ptr = NULL;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  tdsrpt_crit_enum_type rpt_mode;

  rpt_quan_ptr = &up_Measurement_ptr->ue_positioning_ReportingQuantity;

  meas_ctrl_params_ptr->rpt_quan_incl = TRUE;
  
  
  /* Get Pos Method */
  meas_ctrl_params_ptr->rpt_quan.method_type =
    (uint8) TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
  
  if (tdsrrcgps_verify_positioning_capability_r5(rpt_quan_ptr)
    == FAILURE)
  {
    /* Send Meas Ctrl Failure */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Pos Capability error");
    
    /* Set transaction ID */
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
    if(tdsrrcgps_cgps_ue_pos_capability_cb == NULL)
    {
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
    }
    else
#endif
    {
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    }
    
    /* Send MCF msg to UTRAN */
    tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
    
    return FAILURE;
    
  }
  
  /* Get Reporting Quantity */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r4,
    horizontalAccuracy))
  {
    meas_ctrl_params_ptr->rpt_quan.hor_accuracy_incl = TRUE;
    
    /* Set the 7 bits */
    meas_ctrl_params_ptr->rpt_quan.horizontal_accuracy = (uint8)
      (*(rpt_quan_ptr->horizontalAccuracy.data) >> 1);
  }
  if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r7,verticalAccuracy))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r7,gANSSPositioningMethods))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r7,gANSSTimingOfCellWanted))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r7,gANSSCarrierPhaseMeasurementRequested)))
      {
        /*TBD*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel 5 IE's Support TBD");
      }
  
  /* AdditionalAssistanceDataRequest */
  if (rpt_quan_ptr->additionalAssistanceDataReq == TRUE)
  {
    meas_ctrl_params_ptr->rpt_quan.additional_assistance_data_request = TRUE;
  }
  
  if (rpt_quan_ptr->gps_TimingOfCellWanted == TRUE)
  {
    meas_ctrl_params_ptr->rpt_quan.sfn_gps_timing_required = TRUE;
  }
  
  
  /* Get Reporting Criteria */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(up_Measurement_ptr->reportCriteria,
    rrc_UE_Positioning_ReportCriteria_ue_positioning_ReportingCriteria))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Event trigg crit unsupported");
    /* Set transaction ID */
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
    /* Set error t */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
    
    /* Send MCF msg to UTRAN */
    tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
    
    return FAILURE;
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(up_Measurement_ptr->reportCriteria,
    rrc_UE_Positioning_ReportCriteria_noReporting))
  {
    meas_ctrl_params_ptr->rpt_crit.rpt_mode = (uint8)TDSRRC_NO_RPT;
  }
  else
  {
    meas_ctrl_params_ptr->rpt_crit.rpt_mode = (uint8)TDSRRC_PERIODIC_RPT;
    
    /* Get reporting criteria and interval */
  meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_amount = (uint8)tdsrrc_ReportingAmount_ra_Infinity;
  

      meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_amount = (uint8)
        up_Measurement_ptr->
        reportCriteria.u.periodicalReportingCriteria->reportingAmount;
    
    meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_interval = (uint8)
      up_Measurement_ptr->reportCriteria.u.
      periodicalReportingCriteria->reportingInterval;
  }
  if(up_Measurement_ptr->m.ue_positioning_GPS_AssistanceDataPresent )
  {
  /* Now call LSM fn to store Meas Object if FEATURE_MMGPS; call a local function to do the same if FEATURE_CGPS_xxx */
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
  if (tdsrrcgps_convert_assistance_data(&up_Measurement_ptr->ue_positioning_GPS_AssistanceData,
    &meas_ctrl_params_ptr->gps_assistance_data) == FALSE)
#else /* this will only be FEATURE_MMGPS */
  if (lsm_convert_assistance_data(&up_Measurement_ptr->ue_positioning_GPS_AssistanceData,
    &meas_ctrl_params_ptr->gps_assistance_data) == FALSE)
#endif
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Assistance Data conversion failed");
    
    /* Set transaction ID */
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
    /* Set faliure t */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
    
    rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
    
    return FAILURE;
  }
  meas_ctrl_params_ptr->gps_assistance_data_incl = TRUE;
  }
  
  /* Check if Meas Validity present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(up_Measurement_ptr,
    & tdsrrc_UE_Positioning_Measurement_r4,measurementValidity))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Meas validity %d (0/d:1/f:2/a)",up_Measurement_ptr->measurementValidity.ue_State);
    
    /* Store Measurement Validity */
    pos_meas_validity = TDSTVM_RET_MEAS_VALIDITY(up_Measurement_ptr->measurementValidity.ue_State);
    
    /* Check if Meas needs to be restarted */
    if (((pos_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
       ((pos_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: RRC val,MCM val differ");
      meas_ctrl_params_ptr->meas_start_incl = FALSE;
    }
  }    
  /* Store Pos Parameters in RRC Database */
  if(tdsrrcmeas_config_pos_db(pos_info_ptr, &pos_meas_validity, &rpt_mode, modify_meas_params_incl) == FALSE)
  {
    return FAILURE;
  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM_R5

DESCRIPTION

    This function processes the rel 5 Measurement Control Message for
    A-GPS Measurements. 

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm_r5
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr
  ,uint8 meas_ctrl_tr_id
)
{
  /* Process the A-GPS Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  uint16 deleted_meas_id = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsrrc_meas_params_struct_type  gps_meas_params;
  
  /* Declare ptr to store message received from UTRAN */
  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  tdsrrc_MeasurementControl_r4_IEs* rrc_mcm_ptr = NULL; 

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  uint8 tdstransaction_id = TDSINVALID_TRANSACTION_ID;
  
  gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = FALSE;
  
  gps_meas_params.u.meas_ctrl_params.rpt_quan_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.environ_ch_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.hor_accuracy_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.sfn_gps_timing_required = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.additional_assistance_data_request = FALSE;
  gps_meas_params.u.meas_ctrl_params.gps_assistance_data_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.meas_start_incl = TRUE;
  
  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)pos_mcm_ptr;
  tdstransaction_id = meas_ctrl_tr_id;

  /* By default, it is set to 0 => 1 */
  gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = 0;

  if(!(dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.t
    == T_tdsrrc_MeasurementControl_criticalExtensions_5_r4))
  {
    return FAILURE;
  }

  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.r4.measurementControl_r4;

  /* Set Transaction Id and Meas Id */
  p_info_ptr->meas_id = gps_meas_params.u.meas_ctrl_params.meas_id =
    (uint8) rrc_mcm_ptr->measurementIdentity;
  
  gps_meas_params.u.meas_ctrl_params.config_conf_ind = TRUE;
  
  gps_meas_params.u.meas_ctrl_params.config_ack_num = (uint8) tdstransaction_id;
  
  gps_meas_params.u.meas_ctrl_params.rpt_quan.method = (uint8)TDSRRC_GPS;
  gps_meas_params.params_type = (uint8)TDSRRC_MEAS_CTRL_PARMS;
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r4_setup))
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_SETUP;
    
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode)))
    {
      /* Send Meas Ctrl Failure */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas Reporting Mode missing for SETUP");
      
      /* Set transaction ID */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    else
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode =
       (uint8) TDSRET_POS_RPT_TYPE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    if(FAILURE == tdsrrcgps_fill_mcm_info_r5(p_info_ptr, rrc_mcm_ptr->measurementCommand.u.setup->u.
      up_Measurement, &gps_meas_params.u.meas_ctrl_params, tdstransaction_id, FALSE))
    {
      return FAILURE;
    }
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r4_modify))
  {
    /* Addtl Meas processing later. Reporting crit stored in db later */
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_MODIFY;
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode = (uint8)
        TDSRET_POS_RPT_TYPE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,tdsrrc_MeasurementCommand_r4_modify,measurementType))
    {
      if(FAILURE == tdsrrcgps_fill_mcm_info_r5(p_info_ptr, rrc_mcm_ptr->measurementCommand.u.modify->
         measurementType.u.up_Measurement, &gps_meas_params.u.meas_ctrl_params, tdstransaction_id,
         TRUE))
      {
        return FAILURE;
      }
    } /* End of if of Meas Type present */
  } /* End of modify */
  else
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8) TDSRRC_MEAS_DELETE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
    /* At present just store meas id in meas db */
    for ( count = 0; count < TDSMAX_POS_MEAS; count++ )
    {
      if (tdspos_meas_id_list[count].meas_id == gps_meas_params.u.meas_ctrl_params.meas_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS: Meas Id %d deleted from DB",gps_meas_params.u.meas_ctrl_params.meas_id);
        tdspos_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdspos_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        tdspos_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
        {
          tdspos_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break;
      }
    } /* End of for */
    deleted_meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&deleted_meas_id);
    tdsrrcgps_send_lsm_cmd(&gps_meas_params);
    return SUCCESS;
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
  tdsrrcgps_send_lsm_cmd(&gps_meas_params);
  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM_R6

DESCRIPTION

    This function processes the rel 6 Measurement Control Message for
    A-GPS Measurements. 

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/
uecomdef_status_e_type tdsrrcgps_process_mcm_r6
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr
  ,uint8 meas_ctrl_tr_id
)
{
  /* Process the A-GPS Measurement Control Message */
  uint8 count = 0, cnt = 0;
  uint16 deleted_meas_id = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsrrc_meas_params_struct_type  gps_meas_params;
  
  /* Declare ptr to store message received from UTRAN */
  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  tdsrrc_MeasurementControl_r6_IEs* rrc_mcm_ptr = NULL; 

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  uint8 tdstransaction_id = TDSINVALID_TRANSACTION_ID;
  
  gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = FALSE;
  
  gps_meas_params.u.meas_ctrl_params.rpt_quan_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.environ_ch_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.hor_accuracy_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.sfn_gps_timing_required = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.additional_assistance_data_request = FALSE;
  gps_meas_params.u.meas_ctrl_params.gps_assistance_data_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.meas_start_incl = TRUE;
  
  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)pos_mcm_ptr;
  tdstransaction_id = meas_ctrl_tr_id;

  /* By default, it is set to 0 => 1 */
  gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = 0;

  if(!(dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.t
    == T_tdsrrc_MeasurementControl_criticalExtensions_5_later_than_r4))
  {
    return FAILURE;
  }

  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.r6.measurementControl_r6;

  /* Set Transaction Id and Meas Id */
  p_info_ptr->meas_id = gps_meas_params.u.meas_ctrl_params.meas_id =
    (uint8) rrc_mcm_ptr->measurementIdentity;
  
  gps_meas_params.u.meas_ctrl_params.config_conf_ind = TRUE;
  gps_meas_params.u.meas_ctrl_params.config_ack_num = (uint8) tdstransaction_id;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.method = (uint8)TDSRRC_GPS;
  gps_meas_params.params_type = (uint8)TDSRRC_MEAS_CTRL_PARMS;
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r6_setup))
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_SETUP;
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode)))
    {
      /* Send Meas Ctrl Failure */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas Reporting Mode missing for SETUP");
      
      /* Set transaction ID */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    else
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode =
       (uint8) TDSRET_POS_RPT_TYPE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    if(FAILURE == tdsrrcgps_fill_mcm_info_r5(p_info_ptr, rrc_mcm_ptr->measurementCommand.u.setup->u.
      up_Measurement, &gps_meas_params.u.meas_ctrl_params, tdstransaction_id, FALSE))
    {
      return FAILURE;
    }
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r6_modify))
  {
    /* Addtl Meas processing later. Reporting crit stored in db later */
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_MODIFY;
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,
        measurementReportingMode))
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode = (uint8)
        TDSRET_POS_RPT_TYPE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      & tdsrrc_MeasurementCommand_r6_modify,measurementType))
    {
      if(FAILURE == tdsrrcgps_fill_mcm_info_r5(p_info_ptr, rrc_mcm_ptr->measurementCommand.u.modify->
         measurementType.u.up_Measurement, &gps_meas_params.u.meas_ctrl_params, tdstransaction_id,
         TRUE))
      {
        return FAILURE;
      }
    } /* End of if of Meas Type present */
  } /* End of modify */
  else
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8) TDSRRC_MEAS_DELETE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
    /* At present just store meas id in meas db */
    for ( count = 0; count < TDSMAX_POS_MEAS; count++ )
    {
      if (tdspos_meas_id_list[count].meas_id == gps_meas_params.u.meas_ctrl_params.meas_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS: Meas Id %d deleted from DB",gps_meas_params.u.meas_ctrl_params.meas_id);
        tdspos_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdspos_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        tdspos_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
          tdspos_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        break;
      }
    } /* End of for */
    
    deleted_meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&deleted_meas_id);
    
    tdsrrcgps_send_lsm_cmd(&gps_meas_params);
    return SUCCESS;
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
  tdsrrcgps_send_lsm_cmd(&gps_meas_params);
  return SUCCESS;
}

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
/*===========================================================================

FUNCTION
  OSS_BITSTRING_TO_INT 

DESCRIPTION
  This utility function converts the ossBitString type to an integer. 

  The ossBitString is a structure containing a length field in bits and a pointer to an 
  array of one or more bytes. The bytes are ordered from most significant to least significant.
  It is possible that the number of bits is not divisible by 8. If this is the case, the remaining
  bits ( in the last byte ) will reside in the upper bits of the byte.

PARAMETERS
  max_bytes - Maximum number of bytes that are available to hold result
  bitstring_failed - Pointer to a flag that indicates whether the bitstring conversion failed
  bitstring_ptr - Pointer to the OSS bitstring to be converted

RETURN VALUE
  Integer containing contents of OSS bitstring. 

===========================================================================*/
static uint32 tdsrrcgps_oss_bitstring_to_int( uint8 max_bytes, boolean *bitstring_failed, ASN1BitStr32 *bitstring_ptr )
{
  uint32 result = 0;
  uint32 num_bits = (uint32) bitstring_ptr->numbits;
  uint8  *tdsdata_ptr = (uint8 *) bitstring_ptr->data;

  if ( num_bits > ((uint32)max_bytes * 8) )
  {
    /* More bits than can be held in the result, return an error */
    #ifndef WIN32
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERROR: OSS Bitstring exceeds maximum INT size!");
    #endif
    *bitstring_failed = TRUE;
    return (0);
  }
  
  /* While there are bits remaining, build the int */
  while( num_bits )
  {
    if ( num_bits >= 8 )
    {
      /* More than 8 bits remaining so shift the next byte in */
      result <<= 8;
      result |= *tdsdata_ptr++;
      num_bits -= 8;
    }
    else
    {
      /* Less than 8 bits remaining. Shift the remaining bits in */ 
      result <<= num_bits;
      result |= ( ( *tdsdata_ptr++ ) >> ( 8 - num_bits ) );
      num_bits = 0;
    }
  }
  return ( result );
}

/*===========================================================================

FUNCTION
  RRCGPS_CONVERT_ASSISTANCE_DATA

DESCRIPTION
  This is a function takes a tdsrrc_UE_Positioning_GPS_AssistanceData structure 
  filled in by the oss_decode() function and returns a rrc_assistance_data
  structure. 

  The ASN.1 structure makes frequent use of linked lists. When these are encountered
  the data is copied into the appropriate array in the RRC structure.

PARAMETERS
  asn1_ptr - Pointer to ASN.1 data structure output from the OSS decoder
  rrc_ptr - Pointer to the RRC structure to be filled in

RETURN VALUE
  TRUE if conversion succeeded, FALSE if an error occurred.

===========================================================================*/
boolean tdsrrcgps_convert_assistance_data( 
  tdsrrc_UE_Positioning_GPS_AssistanceData* asn1_ptr,
  tdsrrc_assistance_data_struct_type* rrc_ptr )
{
  uint32 q_Cnt;
  uint8  sat_info_cnt = 0;
  boolean bs_failed = FALSE;

  /* Zero out RRC structure */
  memset( (void *) rrc_ptr, 0, sizeof( tdsrrc_assistance_data_struct_type ) );

  /* Acq Assistance */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData,ue_positioning_GPS_AcquisitionAssistance))
  {
    tdsrrc_UE_Positioning_GPS_AcquisitionAssistance *asn1_acq_assist = 
      &asn1_ptr->ue_positioning_GPS_AcquisitionAssistance;
    tdsrrc_gps_acq_assistance_struct_type *rrc_acq_assist = 
      &rrc_ptr->gps_acq_assistance;

    struct tdsrrc_AcquisitionSatInfoList *asn1_info_ptr;
    tdsrrc_gps_sat_info_struct_type *rrc_info_ptr;

    rrc_ptr->gps_acq_assistance_incl = TRUE;
    rrc_acq_assist->gps_reference_time = asn1_acq_assist->gps_ReferenceTime;

    /* GPS - UTRAN Reference Time */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR( asn1_acq_assist,tdsrrc_UE_Positioning_GPS_AcquisitionAssistance,utran_GPSReferenceTime))
    {
      rrc_acq_assist->gps_umts_reference_time_incl = TRUE;
      rrc_acq_assist->gps_umts_reference_time.msb_timing =
        asn1_acq_assist->utran_GPSReferenceTime.utran_GPSTimingOfCell.ms_part;
      rrc_acq_assist->gps_umts_reference_time.lsb_timing =
        asn1_acq_assist->utran_GPSReferenceTime.utran_GPSTimingOfCell.ls_part;
      rrc_acq_assist->gps_umts_reference_time.sfn =
        asn1_acq_assist->utran_GPSReferenceTime.sfn;

      /* Primary Scrambling Code */
      if ( TDSRRC_MSG_COMMON_BITMASK_IE(asn1_acq_assist->utran_GPSReferenceTime,tdsrrc_UTRAN_GPSReferenceTime,modeSpecificInfo) )
      {
        rrc_acq_assist->gps_umts_reference_time.gps_ref_cpid_incl = TRUE;

        if(asn1_acq_assist->utran_GPSReferenceTime.modeSpecificInfo.t == T_tdsrrc_UTRAN_GPSReferenceTime_modeSpecificInfo_fdd)
        {
        /* Only FDD mode supported in UE, grab primary scrambling code */
        rrc_acq_assist->gps_umts_reference_time.cpid =
          asn1_acq_assist->utran_GPSReferenceTime.modeSpecificInfo.u.fdd->referenceIdentity.primaryScramblingCode;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDD info given in MCM. Send failure.");
          return FALSE;
        }
      }
    }

    /* Traverse Satellite Info List */
    sat_info_cnt = 0;
    asn1_info_ptr = &asn1_ptr->ue_positioning_GPS_AcquisitionAssistance.satelliteInformationList;
    rrc_info_ptr = &rrc_ptr->gps_acq_assistance.gps_sat_info[ 0 ];

    for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& 
        (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
    {
      /* Check to see if we have reached the end of the linked list */
      if( asn1_info_ptr == NULL )
      {
        break;
      }

      rrc_info_ptr->satellite_id = (uint8) asn1_info_ptr->elem[q_Cnt].satID;
      rrc_info_ptr->doppler = asn1_info_ptr->elem[q_Cnt].doppler0thOrder;

          /* Extra Doppler Info */
      if ( asn1_info_ptr->elem[q_Cnt].m.extraDopplerInfoPresent )
      {
        rrc_info_ptr->extra_doppler_info_incl = TRUE;
        rrc_info_ptr->extra_doppler_info.doppler_first_order =
          asn1_info_ptr->elem[q_Cnt].extraDopplerInfo.doppler1stOrder;
        rrc_info_ptr->extra_doppler_info.Doppler_uncertainity =
          asn1_info_ptr->elem[q_Cnt].extraDopplerInfo.dopplerUncertainty;
      }

      rrc_info_ptr->code_phase = asn1_info_ptr->elem[q_Cnt].codePhase;
      rrc_info_ptr->integer_code_phase = asn1_info_ptr->elem[q_Cnt].integerCodePhase;
      rrc_info_ptr->gps_bit_number = asn1_info_ptr->elem[q_Cnt].gps_BitNumber;
      rrc_info_ptr->search_window = asn1_info_ptr->elem[q_Cnt].codePhaseSearchWindow;

          /* Azimuth and Elevation */
      if ( asn1_info_ptr->elem[q_Cnt].m.azimuthAndElevationPresent )
      {
        rrc_info_ptr->azimuth_elevation_incl = TRUE;
        rrc_info_ptr->azimuth_elevation_info.azimuth =
          asn1_info_ptr->elem[q_Cnt].azimuthAndElevation.azimuth;
        rrc_info_ptr->azimuth_elevation_info.elevation =
          asn1_info_ptr->elem[q_Cnt].azimuthAndElevation.elevation;
      }
      /* Update sat info ptr with next element in the list */
      rrc_info_ptr++;
      sat_info_cnt++;
    }
    rrc_acq_assist->no_of_gps_sat = sat_info_cnt;
  }
  
  /* Almanac */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData,ue_positioning_GPS_Almanac))
  {
    tdsrrc_UE_Positioning_GPS_Almanac *asn1_almanac =
      &asn1_ptr->ue_positioning_GPS_Almanac;
    tdsrrc_gps_almanac_struct_type *rrc_almanac =
      &rrc_ptr->gps_almanc;

    tdsrrc_AlmanacSatInfoList *asn1_info_ptr;
    tdsrrc_almanac_sat_info_struct_type *rrc_info_ptr; 

    rrc_ptr->gps_almanac_incl = TRUE;
    rrc_almanac->wn_a = 
      ( uint8 )tdsrrcgps_oss_bitstring_to_int( sizeof( uint8 ), &bs_failed, &asn1_almanac->wn_a );

    /* Global Health */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(asn1_almanac,sv_GlobalHealth))
    {
      uint16 num_bits = asn1_almanac->sv_GlobalHealth.numbits;
      uint8 num_bytes; 
      uint8 *dest_ptr = (uint8 *)&rrc_almanac->global_health[0];
      uint8 *src_ptr = (uint8 *)asn1_almanac->sv_GlobalHealth.data;

      /* This is a special case where we can not use tdsrrcgps_oss_bitstring_to_int(). The Global
         Health could be hundreds of bytes long, therefore we need to handle this data 
         in this function */
      num_bytes = num_bits / 8;

      /* Determine if we need an extra byte for any straggler bits */
      if ( num_bits % 8 )
        num_bytes++;

      /* Copy global health data */
      if ( num_bytes <= TDSGLOBAL_HEALTH_ARRAY_SIZE )
      {
        rrc_almanac->global_health_incl = TRUE;
        rrc_almanac->global_health_num_bits = num_bits;
      
        /* Copy global health bits */
        for ( q_Cnt = num_bytes; q_Cnt; q_Cnt-- )
        {
          *dest_ptr++ = *src_ptr++;
        }
      }
    }

    /* Traverse Almanac List */
     
    sat_info_cnt = 0;
    asn1_info_ptr = &asn1_almanac->almanacSatInfoList;
    rrc_info_ptr = &rrc_almanac->almanac_sat_info_list[0];

    for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& 
        (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
    {
      /* Check to see if we have reached the end of the linked list */
      if ( asn1_info_ptr == NULL )
      {
        break;
      }

      rrc_info_ptr->satellite_id = (uint8) asn1_info_ptr->elem[q_Cnt].satID;
      rrc_info_ptr->data_id = (uint8) asn1_info_ptr->elem[q_Cnt].dataID;

      rrc_info_ptr->a_Sqrt = 
        (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].a_Sqrt );
      rrc_info_ptr->deltaI = 
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].deltaI );
      rrc_info_ptr->e = 
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].e );
      rrc_info_ptr->m0 = 
        (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].m0 );
      rrc_info_ptr->omega0 = 
        (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].omega0 );
      rrc_info_ptr->omega_dot = 
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].omegaDot );
      rrc_info_ptr->omega =
        (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].omega );
      rrc_info_ptr->sat_health = 
        (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].satHealth );
      rrc_info_ptr->t_oa =
        (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].t_oa );
      rrc_info_ptr->af0 =
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].af0 );
      rrc_info_ptr->af1 =
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].af1 );
      rrc_info_ptr++;
      sat_info_cnt++;
    }
    rrc_ptr->gps_almanc.no_of_almanac_sat = sat_info_cnt;
  }

  /* Nav Model */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData,ue_positioning_GPS_NavigationModel))
  {
    tdsrrc_navigation_model_sat_info_list_struct_type *rrc_info_ptr;
    tdsrrc_NavigationModelSatInfoList *asn1_info_ptr;
    rrc_ptr->gps_navigation_model_incl = TRUE;

    /* Traverse NavModel List */
    asn1_info_ptr = &asn1_ptr->ue_positioning_GPS_NavigationModel.navigationModelSatInfoList;
    rrc_info_ptr = &rrc_ptr->gps_navigation_model.satellite_info[0];
    sat_info_cnt = 0;

    for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& 
        (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
    {
      /* Check to see if we have reached the end of the linked list */
      if ( asn1_info_ptr == NULL )
      {
        break;
      }

      rrc_info_ptr->satellite_id = (uint8) asn1_info_ptr->elem[q_Cnt].satID;
      rrc_info_ptr->satellite_status = asn1_info_ptr->elem[q_Cnt].satelliteStatus;
     
      /* Ephemeris Parameters */
      if ( asn1_info_ptr->elem[q_Cnt].m.ephemerisParameterPresent )
      {
        rrc_info_ptr->ephermeral_clock_params_incl = TRUE;
        rrc_info_ptr->ephermeral_clock_params.a_Sqrt =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.a_Sqrt );
        rrc_info_ptr->ephermeral_clock_params.af0 =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.af0 );
        rrc_info_ptr->ephermeral_clock_params.af1 = 
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.af1 );
        rrc_info_ptr->ephermeral_clock_params.af2 =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.af2 );
        rrc_info_ptr->ephermeral_clock_params.aodo =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.aodo );
        rrc_info_ptr->ephermeral_clock_params.c_ic =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_ic );
        rrc_info_ptr->ephermeral_clock_params.c_is =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_is );
        rrc_info_ptr->ephermeral_clock_params.c_rc =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_rc );
        rrc_info_ptr->ephermeral_clock_params.c_rs =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_rs );
        rrc_info_ptr->ephermeral_clock_params.c_uc =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_uc );
        rrc_info_ptr->ephermeral_clock_params.c_us =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_us );
        rrc_info_ptr->ephermeral_clock_params.code_on_l2 =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.codeOnL2 );
        rrc_info_ptr->ephermeral_clock_params.delta_n =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.delta_n );
        rrc_info_ptr->ephermeral_clock_params.e =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.e );
        rrc_info_ptr->ephermeral_clock_params.fit_interval =
            (boolean) tdsrrcgps_oss_bitstring_to_int( sizeof(boolean), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.fitInterval );
        rrc_info_ptr->ephermeral_clock_params.i0 =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.i0 );
        rrc_info_ptr->ephermeral_clock_params.iDot =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.iDot );
        rrc_info_ptr->ephermeral_clock_params.iodc =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.iodc );
        rrc_info_ptr->ephermeral_clock_params.l2_pflag =
            (boolean) tdsrrcgps_oss_bitstring_to_int( sizeof(boolean), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.l2Pflag );
        rrc_info_ptr->ephermeral_clock_params.m0 =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.m0 );
        rrc_info_ptr->ephermeral_clock_params.omega =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.omega );
        rrc_info_ptr->ephermeral_clock_params.omega0 =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.omega0 );
        rrc_info_ptr->ephermeral_clock_params.omegaDot =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.omegaDot );
        rrc_info_ptr->ephermeral_clock_params.sat_health =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.satHealth );
        rrc_info_ptr->ephermeral_clock_params.t_gd =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.t_GD );
        rrc_info_ptr->ephermeral_clock_params.t_oc =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.t_oc );
        rrc_info_ptr->ephermeral_clock_params.t_oe =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.t_oe );
        rrc_info_ptr->ephermeral_clock_params.ura_index =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.uraIndex );
      }

      rrc_info_ptr++;
      sat_info_cnt++;
    }
    rrc_ptr->gps_navigation_model.no_of_satellites = sat_info_cnt;
  }

  /* Real Time Integrity */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR( asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData,ue_positioning_GPS_Real_timeIntegrity) )
  {
    tdsrrc_BadSatList *asn1_info_ptr;
    uint8 *bad_sat_ptr;

    rrc_ptr->gps_real_time_integrity_incl = TRUE;

    /* Fill in RTI values */
    asn1_info_ptr = &asn1_ptr->ue_positioning_GPS_Real_timeIntegrity;
    bad_sat_ptr = (uint8 *)&rrc_ptr->gps_rti.satellite_id[0];
    sat_info_cnt = 0;
    for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& 
        (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
    {
      /* Check to see if we have reached the end of the linked list */
      if ( asn1_info_ptr == NULL )
      {
        break;
      }

      *bad_sat_ptr = (uint8) asn1_info_ptr->elem[q_Cnt];
      bad_sat_ptr++;
      sat_info_cnt++;
    }
    rrc_ptr->gps_rti.no_of_satellites = sat_info_cnt;
  }
    
  /* Reference Location */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData,ue_positioning_GPS_ReferenceLocation) )
  {
    tdsrrc_gps_reference_loc_struct_type *rrc_ref_loc =
    &rrc_ptr->gps_ref_loc;
    tdsrrc_EllipsoidPointAltitudeEllipsoide *asn1_ref_loc =
    &asn1_ptr->ue_positioning_GPS_ReferenceLocation.ellipsoidPointAltitudeEllipsoide;

    rrc_ptr->gps_ref_loc_incl = TRUE;
    rrc_ref_loc->altitude = asn1_ref_loc->altitude;
    rrc_ref_loc->altitude_direction = asn1_ref_loc->altitudeDirection;
    rrc_ref_loc->confidence = asn1_ref_loc->confidence;
    rrc_ref_loc->latitude = asn1_ref_loc->latitude;
    rrc_ref_loc->latitude_sign = asn1_ref_loc->latitudeSign;
    rrc_ref_loc->longitude = asn1_ref_loc->longitude;
    rrc_ref_loc->orientation_major_axis = (uint8) asn1_ref_loc->orientationMajorAxis;
    rrc_ref_loc->uncertainity_altitude = asn1_ref_loc->uncertaintyAltitude;
    rrc_ref_loc->uncertainity_semi_major = (uint8) asn1_ref_loc->uncertaintySemiMajor;
    rrc_ref_loc->uncertainity_semi_minor = (uint8) asn1_ref_loc->uncertaintySemiMinor;
  }

  /* Reference Time */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR( asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData,ue_positioning_GPS_ReferenceTime))
  {
    tdsrrc_gps_reference_time_struct_type *rrc_ref_time =
    &rrc_ptr->gps_ref_time;
    tdsrrc_UE_Positioning_GPS_ReferenceTime *asn1_ref_time =
    &asn1_ptr->ue_positioning_GPS_ReferenceTime;

    rrc_ptr->gps_ref_time_incl = TRUE;
    rrc_ref_time->time = asn1_ref_time->gps_tow_1msec;
    rrc_ref_time->week = asn1_ref_time->gps_Week;
  
    /* GPS Drift Rate */
    if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, 
        tdsrrc_UE_Positioning_GPS_ReferenceTime,utran_GPS_DriftRate) )
    {
      rrc_ref_time->gps_drift_rate_incl = TRUE;
      rrc_ref_time->gps_drift_rate =
      asn1_ref_time->utran_GPS_DriftRate;
    }

    /* GPS - UTRAN reference time */
    if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, 
        tdsrrc_UE_Positioning_GPS_ReferenceTime,utran_GPSReferenceTime))
    {
      rrc_ref_time->gps_utran_reference_time_incl = TRUE;
      rrc_ref_time->gps_utran_ref_time.lsb_timing =
        asn1_ref_time->utran_GPSReferenceTime.utran_GPSTimingOfCell.ls_part;
      rrc_ref_time->gps_utran_ref_time.msb_timing =
        asn1_ref_time->utran_GPSReferenceTime.utran_GPSTimingOfCell.ms_part;
      rrc_ref_time->gps_utran_ref_time.sfn =
        asn1_ref_time->utran_GPSReferenceTime.sfn;

      /* Primary Scrambling Code */ 
      if (TDSRRC_MSG_COMMON_BITMASK_IE( asn1_ref_time->utran_GPSReferenceTime,
        tdsrrc_UTRAN_GPSReferenceTime,modeSpecificInfo))
      {
        rrc_ref_time->gps_utran_ref_time.gps_ref_cpid_incl = TRUE;

        if(asn1_ref_time->utran_GPSReferenceTime.modeSpecificInfo.t == T_tdsrrc_UTRAN_GPSReferenceTime_modeSpecificInfo_fdd)
        {
        /* UE Only supports FDD, grab primary scrambling code */
        rrc_ref_time->gps_utran_ref_time.cpid =
        asn1_ref_time->utran_GPSReferenceTime.modeSpecificInfo.u.fdd->referenceIdentity.primaryScramblingCode;
      }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDD info given in MCM. Send failure.");
          return FALSE;
        }
      }
    }

    /* SFN TOW Uncertainty */
    if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, tdsrrc_UE_Positioning_GPS_ReferenceTime,
        sfn_tow_Uncertainty) )
    {
      rrc_ref_time->sfn_tow_uncertainity_incl = TRUE;
      rrc_ref_time->sfn_tow_uncertainity = asn1_ref_time->sfn_tow_Uncertainty;
    }

    /* GPS TOW Assistance List */
    if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, tdsrrc_UE_Positioning_GPS_ReferenceTime,gps_TOW_AssistList))
    {
      tdsrrc_GPS_TOW_AssistList *asn1_info_ptr;
      tdsrrc_gps_tow_assist_struct_type *rrc_info_ptr;

      asn1_info_ptr = &asn1_ref_time->gps_TOW_AssistList;
      rrc_info_ptr = &rrc_ref_time->gps_tow_assist[0];

      /* Traverse TOW Assist List */
      sat_info_cnt = 0;
      for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
      {
        /* Check to see if we have reached the end of the linked list */
        if ( asn1_info_ptr == NULL )
        break;

        rrc_info_ptr->satellite_identity = (uint8) asn1_info_ptr->elem[q_Cnt].satID;
        rrc_info_ptr->tlm_msg = 
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].tlm_Message );
        rrc_info_ptr->tlm_rsvd = 
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].tlm_Reserved );
        rrc_info_ptr->alert = asn1_info_ptr->elem[q_Cnt].alert;
        rrc_info_ptr->anti_spoof = asn1_info_ptr->elem[q_Cnt].antiSpoof;

        rrc_info_ptr++;
        sat_info_cnt++;                                                       
      }

      rrc_ref_time->no_of_gps_tow_assist = sat_info_cnt;
    }
  }

  /* Reference Cell Info */

  /* DGPS Corrections - Not currently supported by RRC layer */

  /* UTC Model - Not currently supported by RRC layer */

  /* IONO Model */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR( asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData,ue_positioning_GPS_IonosphericModel))
  {
    tdsrrc_gps_iono_struct_type *rrc_iono =
      &rrc_ptr->gps_iono;

    tdsrrc_UE_Positioning_GPS_IonosphericModel *asn1_iono = 
      &asn1_ptr->ue_positioning_GPS_IonosphericModel;

    rrc_ptr->gps_iono_incl = TRUE;

    rrc_iono->alfa0 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->alpha0 );
    rrc_iono->alfa1 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->alpha1 );
    rrc_iono->alfa2 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->alpha2 );
    rrc_iono->alfa3 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->alpha3 );
    rrc_iono->beta0 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->beta0 );
    rrc_iono->beta1 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->beta1 );
    rrc_iono->beta2 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->beta2 );
    rrc_iono->beta3 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->beta3 );
  }

  /** UTC model ***/
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR( asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData,ue_positioning_GPS_UTC_Model))
  {
    tdsrrc_gps_utc_struct_type *rrc_utc = &rrc_ptr->gps_utc_model;

    tdsrrc_UE_Positioning_GPS_UTC_Model *asn1_utc = &asn1_ptr->ue_positioning_GPS_UTC_Model;

    rrc_ptr->gps_utc_model_incl = TRUE;

    rrc_utc->q_A0 = (uint32)tdsrrcgps_oss_bitstring_to_int(sizeof(uint32), &bs_failed, &asn1_utc->a0);
    rrc_utc->q_A1 =   (uint32)tdsrrcgps_oss_bitstring_to_int(sizeof(uint32), &bs_failed, &asn1_utc->a1);
    rrc_utc->u_DeltaTls = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->delta_t_LS );
    rrc_utc->u_DeltaTlsf = (uint8)tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->delta_t_LSF);
    rrc_utc->u_DN = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->dn );
    rrc_utc->u_Tot = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->t_ot );
    rrc_utc->u_WNlsf = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->wn_lsf );
    rrc_utc->u_WNt = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->wn_t );

  } 

  /* If any of the bitstring conversions failed, indicate this to caller by returning false. */
  if ( bs_failed )
  {
    #ifndef WIN32
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ASN.1 to RRC structure conversion encountered errors");
    #endif
    return( FALSE );
  }
  else
  {
    return( TRUE );
  }
}

#endif /* FEATURE_CGPS_UMTS_CP_TDSCDMA */


#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
/*===========================================================================

FUNCTION  tdsrrcgps_init_data

DESCRIPTION

  This function clears all the procedure variables and puts the initial
  values wherever needed.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcgps_init_data( void )
{                   
  /* Reset variables for each transaction */
  tdsrrcgps_substate = TDSRRCGPS_INITIAL;
  tdsrrc_cell_id_cnf_cb_func = NULL;
}

/*===========================================================================

FUNCTION  tdsrrcgps_set_psc_for_cell_id_req

DESCRIPTION

  This function updates the cpid for which cell id is requested
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcgps_set_cpid_for_cell_id_req(uint8 cpid)
{
  tdsrrc_gps_cpid_stored = TDSSCR_CODE_TO_PRIMARY_SCR_CODE(cpid);
}


/*===========================================================================

FUNCTION  tdsrrcgps_get_psc_for_cell_id_req

DESCRIPTION

  This function returns the cpid for which cell id is requested
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
uint8 tdsrrcgps_get_cpid_for_cell_id_req()
{
  return tdsrrc_gps_cpid_stored;
}


/*===========================================================================

FUNCTION  tdsrrc_register_cell_id_cnf_cb

DESCRIPTION

  This function registers the callback function to be called for the returning the cell id values to GPS.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE if Registration is success
  else
  FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_register_cell_id_cnf_cb
(
  TDSRRC_GPS_CELL_ID_CNF_CB_FUNC_TYPE  *cb_ptr
)
{
  if (cb_ptr)
  {
    /* First go thru the list to make sure func does 
     * not get registered twice.
     */
    if (tdsrrc_cell_id_cnf_cb_func == NULL)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS: Registering tdsrrc_cell_id_cnf_cb_func ind cb: %d",cb_ptr);
      tdsrrc_cell_id_cnf_cb_func = cb_ptr;
      return TRUE;
    }
    
    if (tdsrrc_cell_id_cnf_cb_func == cb_ptr)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS:Function already registered cb:%d",cb_ptr);
      return TRUE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCGPS: Cannot Register NULL CC Callback");
  }
  return FALSE;
}




/*===========================================================================

FUNCTION  tdsrrcgps_send_sib_cmd

DESCRIPTION

  This function fills the GET_SPECIFIC_SIB_CMD to be sent to SIB
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type  
tdsrrcgps_send_sib_cmd(void)
{
  uint32 i;
  tdsrrc_csp_sib_mask_type  gen_mask = TDSMIB_BIT;
  tdsrrc_cmd_type   *rrcgps_out_cmd_ptr;
  tdsrrc_csp_sib_mask_type    sib_mask = 0;
  /* uint32 pnpos; */
  

  /* Send SIB procedure an event to get SIBs */
  if((rrcgps_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Populate the cmd data */
    rrcgps_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_GET_SPECIFIC_SIBS_REQ;
    sib_mask = TDSMIB_BIT|TDSSIB_3_BIT|TDSSIB_1_BIT;

    /* Get Frequency and Scrambling Code from CSP Internal Data */
    rrcgps_out_cmd_ptr->cmd.get_specific_sibs_req.freq = tdsrrcccm_get_curr_camped_freq();

    /* This function is removed from TDS L1 build */
    /* (void)tdsl1_get_best_psc_in_aset((uint16 *)&rrcgps_out_cmd_ptr->cmd.get_specific_sibs_req.cpid, &pnpos); */
    rrcgps_out_cmd_ptr->cmd.get_specific_sibs_req.cpid = tdscurrent_config_ptr->cell_param_id;
    rrcgps_out_cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell = TRUE;
    rrcgps_out_cmd_ptr->cmd.get_specific_sibs_req.procedure = TDSRRC_PROCEDURE_GPS;
    rrcgps_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type =
        TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN; 
     
    /* Indicate which SIBs are needed */
    for(i = 0;  i < TDSRRC_TOT_SIBS;  i++, gen_mask <<= 1)
    {
      if(sib_mask & gen_mask)
      {
        rrcgps_out_cmd_ptr->cmd.get_specific_sibs_req.sib_req[i] = TRUE;
      }
      else
      {
        rrcgps_out_cmd_ptr->cmd.get_specific_sibs_req.sib_req[i] = FALSE;
      }
    }
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS:Send TDSRRC_GET_SPECIFIC_SIBS_REQ to SIB ");
    /* Send the Internal cmd to RRC */
    tdsrrc_put_int_cmd( rrcgps_out_cmd_ptr );
    return(SUCCESS);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to get cmd buf for cmd TDSRRC_GET_SPECIFIC_SIBS_REQ");
    return(FAILURE);
  }
}


/*===========================================================================

FUNCTION  tdsrrcgps_process_cell_id_req

DESCRIPTION

  This function handles the TDSRRC_GPS_CELL_ID_REQUEST from GPS.
   It Validates the RRC state, 
     If RRC is in a State Transition or  OOS or Not Camped , RRC will send a Failure to GPS.

    Else,
    In FACH, PCH, URA PCH and DISCONNECTED state It will send the GPS the Cell ID info  with
     the data present in RRC database.

     In DCH State,  it will request the SIB module to read SIBs to get the Cell ID for present Cell.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcgps_process_cell_id_req( void )
{
  tdsrrc_mode_e_type   tdsrrc_mode;
  boolean handle_cell_id_req= TRUE;
  tdsrrcllc_oc_set_status_e_type oc_status;
  tdsrrc_int_cell_id_type cell_id;
  rrc_plmn_identity_type plmn_id;
  tdsrrc_state_e_type rrc_curr_state;
  tdsrrcmeas_plmn_cell_info_struct_type  cell_id_info;
  tdsrrc_CellUpdateCause       cu_cause;
      
  tdsrrc_mode = tdsrrcmcm_get_rrc_mode();
  oc_status = tdsrrcllc_get_ordered_config_status();
  rrc_curr_state = tdsrrc_get_state();

  memset(&cell_id_info,0x00, sizeof(tdsrrcmeas_plmn_cell_info_struct_type));
  
  cell_id_info.cell_id = TDSINVALID_CELL_ID_OTA;
  cell_id_info.freq = TDSRRC_GPS_INVALID_FREQ;
  cell_id_info.cpid = TDSRRC_GPS_INVALID_CPID;

  /* Check if any State Transition or Reconfig or W->G Handover 
   * is ongoing or UE is not camped on
   */
  if( tdsrrc_mode != TDSRRC_MODE_ACTIVE )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR," RRCGPS: RRC is in %d mode so cannot handle  TDSRRC_GPS_CELL_ID_REQUEST ",tdsrrc_mode);

    tdsrrc_cell_id_cnf_cb_func(cell_id_info,TDSRRC_NO_CELL_ID_AVAIL);
    handle_cell_id_req = FALSE;
  }
  else if ( TDSRRC_NOT_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_GPS))
  {
    /*IF MS is not Camped then return Failure to GPS */
    TDSRRC_MSG0(MSG_LEGACY_ERROR," RRCGPS: UE not camped so cannot handle  TDSRRC_GPS_CELL_ID_REQUEST ");
    tdsrrc_cell_id_cnf_cb_func(cell_id_info,TDSRRC_NO_CELL_ID_AVAIL);
    handle_cell_id_req = FALSE;
  }

  if(handle_cell_id_req == TRUE )
  {
    switch(rrc_curr_state)
    {
      case TDSRRC_STATE_CELL_DCH:
        /* send a Failure to GPS if : 
          *  1)If the ordered config is in use that means some state transitions are ongoing,
          *   2) Or if UE is in OOS 
          */
        if( (oc_status != TDSOC_NOT_SET) ||(tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS) || 
            (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause)== TDSRRCCU_STARTED)) 
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR," RRCGPS: RRC cannot handle TDSRRC_GPS_CELL_ID_REQUEST oc_status %d  TOC  OOS status  %d cu state %d ",
                      oc_status,tdsrrcllc_get_toc_usage(),tdsrrccu_return_cu_substate() );
          tdsrrc_cell_id_cnf_cb_func(cell_id_info,TDSRRC_NO_CELL_ID_AVAIL);
        }
        else if(tdsrrcrcr_is_proc_active())
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR," RRCGPS: RRC cannot handle  RRC_GPS_CELL_ID_REQUEST due to RCR active");
          rrc_send_cell_id_cnf_to_cm(NULL);
        }
        else
        {
          if(tdsrrcgps_send_sib_cmd() == FAILURE)
          {
            ERR_FATAL("RRCGPS: Failure to send TDSRRC_GET_SPECIFIC_SIBS_REQ into SIB ",0,0,0);
          }
          tdsrrcgps_substate = TDSRRCGPS_WAIT_FOR_SIB_CNF;
        }
        break;
  
      /* get from SIB database */
      case TDSRRC_STATE_CELL_FACH:
      case TDSRRC_STATE_URA_PCH:
      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_CONNECTING:
      case TDSRRC_STATE_DISCONNECTED:
        (void)tdsrrcccm_get_serv_plmn_cellid(&cell_id,&plmn_id);
         /* send a Failure to GPS if : 
          *  1) Or if UE is in OOS 
          */
        if( tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS) 
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR," RRCGPS: RRC cannot handle TDSRRC_GPS_CELL_ID_REQUEST TOC OOS status %d",tdsrrcllc_get_toc_usage());
          tdsrrc_cell_id_cnf_cb_func(cell_id_info,TDSRRC_NO_CELL_ID_AVAIL);
        }
        else
        {
          /* Provide the Freq info, CPID, PLMN ID and the Cell ID to GPS */
          cell_id_info.cell_id = cell_id;
          cell_id_info.freq = tdsrrcccm_get_curr_camped_freq();
          cell_id_info.cpid = tdsrrcccm_get_curr_camped_cpid();
          cell_id_info.plmn_id = plmn_id;
          tdsrrc_cell_id_cnf_cb_func(cell_id_info,TDSRRC_NEW_CELL_ID_AVAIL);
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCGPS: Cell id sent to GPS in %d state is %d ",rrc_curr_state,cell_id);
        }
        break;
  
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCGPS: Received TDSRRC_GPS_CELL_ID_REQUEST in bad RRC state %d",rrc_curr_state);
        break;
    }
  }
}

/*===========================================================================

FUNCTION   tdsrrcgps_initial_substate_event_handler

DESCRIPTION
    
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void  tdsrrcgps_initial_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr /* Received command                   */
)
{

  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_GPS_CELL_ID_REQUEST:
      
      tdsrrcgps_process_cell_id_req();
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS: Received TDSRRC_GPS_CELL_ID_REQUEST in rrc state %d",tdsrrc_get_state());
      break;

    default:
      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCGPS: Ignoring RRC GPS event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcgps_substate);
      break;
  }
}



/*===========================================================================

FUNCTION   tdsrrcgps_wait_sib_cnf_substate_event_handler

DESCRIPTION

 This function handles the events received by this module when the module is in 
  TDSRRCGPS_WAIT_FOR_SIB_CNF sub-state. 
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcgps_wait_sib_cnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr /* Received command                   */
)
{
  tdsrrcmeas_plmn_cell_info_struct_type  cell_id_info;
  
  cell_id_info.cell_id = TDSINVALID_CELL_ID_OTA;
  cell_id_info.freq = TDSRRC_GPS_INVALID_FREQ;
  cell_id_info.cpid = TDSRRC_GPS_INVALID_CPID;
  
  memset(&cell_id_info,0x00, sizeof(rrc_plmn_identity_type));
   
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_GPS_CELL_ID_REQUEST:
      TDSRRC_MSG0(MSG_LEGACY_ERROR," RRCGPS: Did not expect a TDSRRC_GPS_CELL_ID_REQUEST while already processing one");
     
      break;

    case TDSRRC_GET_SPECIFIC_SIBS_CNF:
      if(cmd_ptr->cmd.get_sibs_cnf.status == TDSSIB_EVENT_SUCCEEDED)
      {
       /*  On successful read of Cell ID, send the Freq info, CPID, PLMN ID and the Cell ID to GPS */
        cell_id_info.cell_id = cmd_ptr->cmd.get_sibs_cnf.cell_id;
        cell_id_info.freq = tdsrrcccm_get_curr_camped_freq();
        cell_id_info.cpid = (uint8) tdsrrc_gps_cpid_stored;
         
        cell_id_info.plmn_id = cmd_ptr->cmd.get_sibs_cnf.plmn_id;
          
        tdsrrc_cell_id_cnf_cb_func(cell_id_info,TDSRRC_NEW_CELL_ID_AVAIL);
        TDSRRC_MSG1(MSG_LEGACY_HIGH," RRCGPS: Success read of cell id,Send cell id %d to GPS",cmd_ptr->cmd.get_sibs_cnf.cell_id);
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCGPS: Camped Freq %d cpid %d",tdsrrcccm_get_curr_camped_freq(),tdsrrc_gps_cpid_stored);
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"PLMN id MCC %d MNC %d",TDSRRC_CSP_GET_INT_MCC(cmd_ptr->cmd.get_sibs_cnf.plmn_id),TDSRRC_CSP_GET_INT_MNC(cmd_ptr->cmd.get_sibs_cnf.plmn_id));
      }
      else
      {
        /* If Cell was not read by Lower layer, send failure to GPS */
        tdsrrc_cell_id_cnf_cb_func(cell_id_info,TDSRRC_NO_CELL_ID_AVAIL);
        TDSRRC_MSG0(MSG_LEGACY_HIGH," RRCGPS: Failed to  read of cell id,Failure GPS");
      }
      tdsrrcgps_substate = TDSRRCGPS_INITIAL;
      break;

    case TDSRRC_CPHY_DL_WEAK_IND:
      /* On receiving TDSRRC_CPHY_DL_WEAK_IND from L1, call the SIB function 
        * to abort the event */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Recieved  TDSRRC_CPHY_DL_WEAK_IND,abort sib event");
      tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
      break;

    case TDSRRC_STATE_CHANGE_IND :
      tdsrrc_cell_id_cnf_cb_func(cell_id_info,TDSRRC_NO_CELL_ID_AVAIL);
      TDSRRC_MSG0(MSG_LEGACY_HIGH," RRCGPS: Abort Due to State Change Ind,Send failure GPS");
      tdsrrcgps_substate = TDSRRCGPS_INITIAL;
      break;

    default:
      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCGPS: Ignoring RRC GPS event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcgps_substate);
      break;
  }
}


/*===========================================================================

FUNCTION RRCLBT_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  RRC GPS  procedure. This function is called by the
  dispatcher whenever an event is received for this module.  
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrcgps_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{

  /* Check the procedure substate and procedure the event accordingly */  
  switch( tdsrrcgps_substate )
  {
    case TDSRRCGPS_INITIAL:
      /* Call the event handler for Initial substate */
      tdsrrcgps_initial_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCGPS_WAIT_FOR_SIB_CNF:
      /* Call the event handler for waiting for chan cfg cnf substate */
      tdsrrcgps_wait_sib_cnf_substate_event_handler( cmd_ptr );
      break;
  
    default:
      /* Invalid substate - should never happen. Clear
         all procedure vars */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCGPS: Invalid RRC GPS Substate %d",tdsrrcgps_substate);
      tdsrrcgps_init_data();
      break;
  }/* end switch */
}

/*===========================================================================

FUNCTION  

DESCRIPTION

  This function initializes the RRC GPS procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcgps_init_procedure( void )
{
  tdsrrcgps_init_data(); /* Initialize all the procedure variables */

  /* Register for state change  - When state changes from DCH
  to any other state we want to be notified since this procedure
  is active only in the DCH state. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_GPS,  /* Procedure name */
                                TDSRRC_STATE_WILDCARD, /* From State     */
                                TDSRRC_STATE_DISCONNECTED  /* To State       */
                                );
}


/*===========================================================================

FUNCTION  tdsrrcgps_is_gps_event_active

DESCRIPTION

  This function will return status of rrcgps event if active.
    
DEPENDENCIES

  None.
 
RETURN VALUE

TRUE : if GPS event is active,
FALSE : Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcgps_is_gps_event_active()
{
  boolean status = FALSE;

  if(tdsrrcgps_substate != TDSRRCGPS_INITIAL)
  {
    status = TRUE;
  }
  return status;
}
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/


/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM_R7

DESCRIPTION

    This function processes the rel 7 Measurement Control Message for
    A-GPS Measurements.

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm_r7
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr,
  uint8 meas_ctrl_tr_id
)
{
  /* Process the A-GPS Measurement Control Message */
  uint8 count = 0, cnt = 0;
  uint16 deleted_meas_id = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsrrc_meas_params_struct_type  gps_meas_params;
  
  /* Declare ptr to store message received from UTRAN */
  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  tdsrrc_MeasurementControl_r7_IEs* rrc_mcm_ptr = NULL; 

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  uint8 tdstransaction_id = TDSINVALID_TRANSACTION_ID;
  
  gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = FALSE;
  
  gps_meas_params.u.meas_ctrl_params.rpt_quan_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.environ_ch_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.hor_accuracy_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.sfn_gps_timing_required = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.additional_assistance_data_request = FALSE;
  gps_meas_params.u.meas_ctrl_params.gps_assistance_data_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.meas_start_incl = TRUE;
  
  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)pos_mcm_ptr;
  tdstransaction_id = meas_ctrl_tr_id;

  /* By default, it is set to 0 => 1 */
  gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = 0;

  if(!(dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.t
    == T_tdsrrc_MeasurementControl_criticalExtensions_5_later_than_r4))
  {
    return FAILURE;
  }

  rrc_mcm_ptr =  &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.r7.measurementControl_r7;

  /* Set Transaction Id and Meas Id */
  p_info_ptr->meas_id = gps_meas_params.u.meas_ctrl_params.meas_id =
    (uint8) rrc_mcm_ptr->measurementIdentity;
  
  gps_meas_params.u.meas_ctrl_params.config_conf_ind = TRUE;
  gps_meas_params.u.meas_ctrl_params.config_ack_num = (uint8) tdstransaction_id;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.method = (uint8)TDSRRC_GPS;
  gps_meas_params.params_type = (uint8)TDSRRC_MEAS_CTRL_PARMS;
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r7_setup))
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_SETUP;
    
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode)))
    {
      /* Send Meas Ctrl Failure */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas Reporting Mode missing for SETUP");
      
      /* Set transaction ID */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    else
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode =
       (uint8) TDSRET_POS_RPT_TYPE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    if(FAILURE == tdsrrcgps_fill_mcm_info_r7(p_info_ptr, rrc_mcm_ptr->measurementCommand.u.setup->u.
       up_Measurement, &gps_meas_params.u.meas_ctrl_params, tdstransaction_id, FALSE))
    {
      return FAILURE;
    }
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r7_modify))
  {
    /* Addtl Meas processing later. Reporting crit stored in db later */
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_MODIFY;
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode))
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode = (uint8)
        TDSRET_POS_RPT_TYPE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
                                         & tdsrrc_MeasurementCommand_r7_modify,measurementType))
    {
      if(FAILURE == tdsrrcgps_fill_mcm_info_r7(p_info_ptr, rrc_mcm_ptr->measurementCommand.u.modify->
         measurementType.u.up_Measurement, &gps_meas_params.u.meas_ctrl_params, tdstransaction_id,
         TRUE))
      {
        return FAILURE;
      }
    } /* End of if of Meas Type present */
  } /* End of modify */
  else
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8) TDSRRC_MEAS_DELETE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
    /* At present just store meas id in meas db */
    for ( count = 0; count < TDSMAX_POS_MEAS; count++ )
    {
      if (tdspos_meas_id_list[count].meas_id == gps_meas_params.u.meas_ctrl_params.meas_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS: Meas Id %d deleted from DB",gps_meas_params.u.meas_ctrl_params.meas_id);
        tdspos_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdspos_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        tdspos_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
        {
          tdspos_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break;
      }
    } /* End of for */
    deleted_meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&deleted_meas_id);
    tdsrrcgps_send_lsm_cmd(&gps_meas_params);
    return SUCCESS;
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
  tdsrrcgps_send_lsm_cmd(&gps_meas_params);
  return SUCCESS;
}


/*===========================================================================

FUNCTION   RRCGPS_FILL_MCM_R7_INFO

DESCRIPTION

    This function fill the GPS structure for 
    A-GPS Measurements. 

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_fill_mcm_info_r7
(
  tdspos_info_struct_type* pos_info_ptr,                       
  tdsrrc_UE_Positioning_Measurement_r7 *up_Measurement_ptr,
  tdsrrc_meas_ctrl_struct_type *meas_ctrl_params_ptr,
  uint8 tdstransaction_id,
  boolean modify_meas_params_incl
)
{
  tdsmeas_validity_enum_type pos_meas_validity = TDSNOT_PRESENT;
  tdsrrc_UE_Positioning_ReportingQuantity_r7* rpt_quan_ptr = NULL;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  tdsrpt_crit_enum_type rpt_mode;

  rpt_quan_ptr = &up_Measurement_ptr->ue_positioning_ReportingQuantity;
  meas_ctrl_params_ptr->rpt_quan_incl = TRUE;
  /* Get Pos Method */
  meas_ctrl_params_ptr->rpt_quan.method_type =
    (uint8) TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
  if (tdsrrcgps_verify_positioning_capability_r7(rpt_quan_ptr) == FAILURE)
  {
    /* Send Meas Ctrl Failure */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Pos Capability error");
    
    /* Set transaction ID */
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
    /* Set error t */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    
    /* Send MCF msg to UTRAN */
    tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
    return FAILURE;
  }
  
  /* Get Reporting Quantity */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,
    tdsrrc_UE_Positioning_ReportingQuantity_r7,horizontalAccuracy))
  {
    meas_ctrl_params_ptr->rpt_quan.hor_accuracy_incl = TRUE;
    /* Set the 7 bits */
    meas_ctrl_params_ptr->rpt_quan.horizontal_accuracy = (uint8)
      (*(rpt_quan_ptr->horizontalAccuracy.data) | 0xfe00);
  }

  if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r7,velocityRequested))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r7,gANSSPositioningMethods))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r7,gANSSTimingOfCellWanted))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,rc_UE_Positioning_ReportingQuantity_r7,gANSSCarrierPhaseMeasurementRequested)))
  {
    /*TBD*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel 7 IE's Support TBD");
  }
  
  /* AdditionalAssistanceDataRequest */
  if (rpt_quan_ptr->additionalAssistanceDataReq == TRUE)
  {
    meas_ctrl_params_ptr->rpt_quan.additional_assistance_data_request = TRUE;
  }
  if (rpt_quan_ptr->gps_TimingOfCellWanted == TRUE)
  {
    meas_ctrl_params_ptr->rpt_quan.sfn_gps_timing_required = TRUE;
  }
  
  /* Get Reporting Criteria */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(up_Measurement_ptr->reportCriteria,
    rrc_UE_Positioning_ReportCriteria_r7_ue_positioning_ReportingCriteria))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Event trigg crit unsupported");
    /* Set transaction ID */
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
    /* Set error t */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
    
    /* Send MCF msg to UTRAN */
    tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
    
    return FAILURE;
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(up_Measurement_ptr->reportCriteria,
    rrc_UE_Positioning_ReportCriteria_r7_noReporting))
  {
    meas_ctrl_params_ptr->rpt_crit.rpt_mode = (uint8)TDSRRC_NO_RPT;
  }
  else
  {
    meas_ctrl_params_ptr->rpt_crit.rpt_mode = (uint8)TDSRRC_PERIODIC_RPT;
    /* Get reporting criteria and interval */
    meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_amount = (uint8)tdsrrc_ReportingAmount_ra_Infinity;
    meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_amount = (uint8)
        up_Measurement_ptr->reportCriteria.u.periodicalReportingCriteria->reportingAmount;
    meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_interval = (uint8)
      up_Measurement_ptr->reportCriteria.u.periodicalReportingCriteria->reportingInterval;
  }
  if(up_Measurement_ptr->m.ue_positioning_GPS_AssistanceDataPresent )
  {
  /* Now call LSM fn to store Meas Object if FEATURE_MMGPS; call a local function to do the same if FEATURE_CGPS_xxx */
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
  if (tdsrrcgps_convert_assistance_data_r7(&up_Measurement_ptr->ue_positioning_GPS_AssistanceData,
    &meas_ctrl_params_ptr->gps_assistance_data) == FALSE)
#else /* this will only be FEATURE_MMGPS */
  /*To be Required from GPS guys*/
/*  if (lsm_convert_assistance_data_r7(&up_Measurement_ptr->ue_positioning_GPS_AssistanceData,
    &meas_ctrl_params_ptr->gps_assistance_data) == FALSE)*/
#endif
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Assistance Data conversion failed");
    
    /* Set transaction ID */
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
    /* Set faliure t */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
    
    rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
    
    return FAILURE;
  }
  meas_ctrl_params_ptr->gps_assistance_data_incl = TRUE;
  }
  
  /* Check if Meas Validity present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(up_Measurement_ptr,
    & tdsrrc_UE_Positioning_Measurement_r7,measurementValidity))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Meas validity %d (0/d:1/f:2/a)",up_Measurement_ptr->measurementValidity.ue_State);
    
    /* Store Measurement Validity */
    pos_meas_validity = TDSTVM_RET_MEAS_VALIDITY(up_Measurement_ptr->measurementValidity.ue_State);
    
    /* Check if Meas needs to be restarted */
    if (((pos_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
       ((pos_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: RRC val,MCM val differ");
      meas_ctrl_params_ptr->meas_start_incl = FALSE;
    }
  }    
  /* Store Pos Parameters in RRC Database */
  if(tdsrrcmeas_config_pos_db(pos_info_ptr, &pos_meas_validity, &rpt_mode, modify_meas_params_incl) == FALSE)
  {
     return FAILURE;
  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION    RRCGPS_VERIFY_POSITIONING_CAPABILITY

DESCRIPTION

 Checks positioning capability to determine if UE could process
 A-GPS Measurement Control Message

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_verify_positioning_capability_r7
(
  const tdsrrc_UE_Positioning_ReportingQuantity_r7 *rpt_quan_ptr
)
{

  tdsrrc_positioning_method_type_enum_type pos_method_type;

  tm_umts_cp_tds_ue_pos_capability_struct_type pos_capability;  
  
  uecomdef_status_e_type verify_status = SUCCESS;

  
  pos_method_type = TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
  
  /* First get positioning capability from LSM */
  if(tdsrrcgps_cgps_ue_pos_capability_cb == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"GPS Call back function is NULL");
    return FAILURE;
  }
  else
  {
    tdsrrcgps_cgps_ue_pos_capability_cb(&pos_capability);
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:LSM/TM returned capability %d",pos_capability.network_assisted_gps_support);

  if ( ( rpt_quan_ptr->gps_TimingOfCellWanted == TRUE  ) &&
       ( pos_capability.gps_timing_of_cell_frames_supported == FALSE )
     )
  {
    verify_status = FAILURE;
  }

  if ((rpt_quan_ptr->positioningMethod == tdsrrc_PositioningMethod_otdoa) &&
      (pos_capability.ue_based_otdoa_supported == FALSE))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Pos Method OTDOA unsupported");
    verify_status = FAILURE;
  }

  if ( rpt_quan_ptr->positioningMethod == tdsrrc_PositioningMethod_cellID )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Pos Method CELL ID unsupported");
    verify_status = FAILURE;
  }

  if ( verify_status == SUCCESS ) /* continue examining if still good */
  {

    switch ( pos_capability.network_assisted_gps_support )
    {
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_UE_BASED:
#endif
      {
        /* Reject any UE Assisted Requests */
        if (pos_method_type == TDSRRC_UE_ASSISTED )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=UE BASED,Req=UE_ASSISTED");
        }

        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_NETWORK_BASED:
#endif
      {
        /* Reject UE Based if we don't support standalone */
        if ( ( pos_method_type == TDSRRC_UE_BASED ) && 
             ( pos_capability.standalone_location_method_supported == FALSE ) )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=NW BASED, Req=UE_BASED,No Standalone");
        }
        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_BOTH:
#endif
      {
        /* Never Reject */
        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_NONE:
#endif
      {
        /* Reject everything unless UE Based is requested and standalone is supported */
        if ( ( pos_method_type == TDSRRC_UE_ASSISTED ) || 
             ( pos_capability.standalone_location_method_supported == FALSE ) )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=NONE,Req=UE_ASSISTED or No Standalone");
        }
        break;
      }
      default:
        break;
    }

  } /* if verify_status is still SUCCESS */

  if ( verify_status == SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:UE cap check passed");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:UE cap check failed");
  }
  
  return verify_status;
}

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA

/*===========================================================================

FUNCTION
  RRCGPS_CONVERT_ASSISTANCE_DATA

DESCRIPTION
  This is a function takes a tdsrrc_UE_Positioning_GPS_AssistanceData structure 
  filled in by the oss_decode() function and returns a rrc_assistance_data
  structure. 

  The ASN.1 structure makes frequent use of linked lists. When these are encountered
  the data is copied into the appropriate array in the RRC structure.

PARAMETERS
  asn1_ptr - Pointer to ASN.1 data structure output from the OSS decoder
  rrc_ptr - Pointer to the RRC structure to be filled in

RETURN VALUE
  TRUE if conversion succeeded, FALSE if an error occurred.

===========================================================================*/
boolean tdsrrcgps_convert_assistance_data_r7( 
  tdsrrc_UE_Positioning_GPS_AssistanceData_r7* asn1_ptr,
  tdsrrc_assistance_data_struct_type* rrc_ptr )
{
  uint32 q_Cnt;
  uint8  sat_info_cnt = 0;
  boolean bs_failed = FALSE;

  /* Zero out RRC structure */
  memset( (void *) rrc_ptr, 0, sizeof( tdsrrc_assistance_data_struct_type ) );

  /* Acq Assistance */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR( asn1_ptr,
    tdsrrc_UE_Positioning_GPS_AssistanceData_r7,ue_positioning_GPS_AcquisitionAssistance))
  {
    tdsrrc_UE_Positioning_GPS_AcquisitionAssistance_r7 *asn1_acq_assist = 
      &asn1_ptr->ue_positioning_GPS_AcquisitionAssistance;
    tdsrrc_gps_acq_assistance_struct_type *rrc_acq_assist = 
      &rrc_ptr->gps_acq_assistance;

    tdsrrc_AcquisitionSatInfoList *asn1_info_ptr;
    tdsrrc_gps_sat_info_struct_type *rrc_info_ptr;

    rrc_ptr->gps_acq_assistance_incl = TRUE;
    rrc_acq_assist->gps_reference_time = asn1_acq_assist->gps_ReferenceTime;

    /* GPS - UTRAN Reference Time */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR( asn1_acq_assist,
        tdsrrc_UE_Positioning_GPS_AcquisitionAssistance_r7,utran_GPSReferenceTime))
    {
      rrc_acq_assist->gps_umts_reference_time_incl = TRUE;
      rrc_acq_assist->gps_umts_reference_time.msb_timing =
        asn1_acq_assist->utran_GPSReferenceTime.utran_GPSTimingOfCell.ms_part;
      rrc_acq_assist->gps_umts_reference_time.lsb_timing =
        asn1_acq_assist->utran_GPSReferenceTime.utran_GPSTimingOfCell.ls_part;
      rrc_acq_assist->gps_umts_reference_time.sfn =
        asn1_acq_assist->utran_GPSReferenceTime.sfn;

      /* Primary Scrambling Code */
      if (TDSRRC_MSG_COMMON_BITMASK_IE( asn1_acq_assist->utran_GPSReferenceTime,
        tdsrrc_UTRAN_GPSReferenceTime,modeSpecificInfo))
      {
        rrc_acq_assist->gps_umts_reference_time.gps_ref_cpid_incl = TRUE;

        if(asn1_acq_assist->utran_GPSReferenceTime.modeSpecificInfo.t == T_tdsrrc_UTRAN_GPSReferenceTime_modeSpecificInfo_fdd)
        {
          /* Only FDD mode supported in UE, grab primary scrambling code */
          rrc_acq_assist->gps_umts_reference_time.cpid =
          asn1_acq_assist->utran_GPSReferenceTime.modeSpecificInfo.u.fdd->referenceIdentity.primaryScramblingCode;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDD info given in MCM. Send failure.");
          return FALSE;
        }
      }
    }

    /* Traverse Satellite Info List */
    sat_info_cnt = 0;
    asn1_info_ptr = &asn1_ptr->ue_positioning_GPS_AcquisitionAssistance.satelliteInformationList;
    rrc_info_ptr = &rrc_ptr->gps_acq_assistance.gps_sat_info[ 0 ];
    
    for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
    {
      /* Check to see if we have reached the end of the linked list */
      if( asn1_info_ptr == NULL )
      {
        break;
      }
    
      rrc_info_ptr->satellite_id = (uint8) asn1_info_ptr->elem[q_Cnt].satID;
      rrc_info_ptr->doppler = asn1_info_ptr->elem[q_Cnt].doppler0thOrder;
    
      /* Extra Doppler Info */
      if ( asn1_info_ptr->elem[q_Cnt].m.extraDopplerInfoPresent )
      {
        rrc_info_ptr->extra_doppler_info_incl = TRUE;
        rrc_info_ptr->extra_doppler_info.doppler_first_order =
          asn1_info_ptr->elem[q_Cnt].extraDopplerInfo.doppler1stOrder;
        rrc_info_ptr->extra_doppler_info.Doppler_uncertainity =
          asn1_info_ptr->elem[q_Cnt].extraDopplerInfo.dopplerUncertainty;
      }
    
      rrc_info_ptr->code_phase = asn1_info_ptr->elem[q_Cnt].codePhase;
      rrc_info_ptr->integer_code_phase = asn1_info_ptr->elem[q_Cnt].integerCodePhase;
      rrc_info_ptr->gps_bit_number = asn1_info_ptr->elem[q_Cnt].gps_BitNumber;
      rrc_info_ptr->search_window = asn1_info_ptr->elem[q_Cnt].codePhaseSearchWindow;
    
      /* Azimuth and Elevation */
      if ( asn1_info_ptr->elem[q_Cnt].m.azimuthAndElevationPresent )
      {
        rrc_info_ptr->azimuth_elevation_incl = TRUE;
        rrc_info_ptr->azimuth_elevation_info.azimuth =
          asn1_info_ptr->elem[q_Cnt].azimuthAndElevation.azimuth;
        rrc_info_ptr->azimuth_elevation_info.elevation =
          asn1_info_ptr->elem[q_Cnt].azimuthAndElevation.elevation;
      }
    
      /* Update sat info ptr with next element in the list */
      rrc_info_ptr++;
      sat_info_cnt++;
    }

    rrc_acq_assist->no_of_gps_sat = sat_info_cnt;
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR( asn1_acq_assist,
        tdsrrc_UE_Positioning_GPS_AcquisitionAssistance_r7,ue_Positioning_GPS_ReferenceTimeUncertainty))
    {
      /*TBD*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7 IE's TBD");
    }
  }
  
  /* Almanac */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr, tdsrrc_UE_Positioning_GPS_AssistanceData_r7,
    ue_positioning_GPS_Almanac))
  {
    tdsrrc_UE_Positioning_GPS_Almanac *asn1_almanac =
    &asn1_ptr->ue_positioning_GPS_Almanac;
    tdsrrc_gps_almanac_struct_type *rrc_almanac =
    &rrc_ptr->gps_almanc;

    tdsrrc_AlmanacSatInfoList *asn1_info_ptr;
    tdsrrc_almanac_sat_info_struct_type *rrc_info_ptr; 

    rrc_ptr->gps_almanac_incl = TRUE;
    rrc_almanac->wn_a = 
    ( uint8 )tdsrrcgps_oss_bitstring_to_int( sizeof( uint8 ), &bs_failed, &asn1_almanac->wn_a );

    /* Global Health */
    if ( asn1_almanac->m.sv_GlobalHealthPresent )
    {
      uint16 num_bits = asn1_almanac->sv_GlobalHealth.numbits;
      uint8 num_bytes; 
      uint8 *dest_ptr = (uint8 *)&rrc_almanac->global_health[0];
      uint8 *src_ptr = (uint8 *)asn1_almanac->sv_GlobalHealth.data;
    
      /* This is a special case where we can not use tdsrrcgps_oss_bitstring_to_int(). The Global
         Health could be hundreds of bytes long, therefore we need to handle this data 
       in this function */
      num_bytes = num_bits / 8;

      /* Determine if we need an extra byte for any straggler bits */
      if ( num_bits % 8 )
        num_bytes++;
    
      /* Copy global health data */
      if ( num_bytes <= TDSGLOBAL_HEALTH_ARRAY_SIZE )
      {
        rrc_almanac->global_health_incl = TRUE;
        rrc_almanac->global_health_num_bits = num_bits;
      
        /* Copy global health bits */
        for ( q_Cnt = num_bytes; q_Cnt; q_Cnt-- )
        {
          *dest_ptr++ = *src_ptr++;
        }
      }
    }

    /* Traverse Almanac List */
    sat_info_cnt = 0;
    asn1_info_ptr = &asn1_almanac->almanacSatInfoList;
    rrc_info_ptr = &rrc_almanac->almanac_sat_info_list[0];

    for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
    {
      /* Check to see if we have reached the end of the linked list */
      if ( asn1_info_ptr == NULL )
      {
        break;
      }
    
      rrc_info_ptr->satellite_id = (uint8) asn1_info_ptr->elem[q_Cnt].satID;
      rrc_info_ptr->data_id = (uint8) asn1_info_ptr->elem[q_Cnt].dataID;
      rrc_info_ptr->a_Sqrt = 
        (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].a_Sqrt );
      rrc_info_ptr->deltaI = 
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].deltaI );
      rrc_info_ptr->e = 
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].e );
      rrc_info_ptr->m0 = 
        (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].m0 );
      rrc_info_ptr->omega0 = 
        (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].omega0 );
      rrc_info_ptr->omega_dot = 
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].omegaDot );
      rrc_info_ptr->omega =
        (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].omega );
      rrc_info_ptr->sat_health = 
        (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].satHealth );
      rrc_info_ptr->t_oa =
        (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].t_oa );
      rrc_info_ptr->af0 =
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].af0 );
      rrc_info_ptr->af1 =
        (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].af1 );
      rrc_info_ptr++;
      sat_info_cnt++;
    }
    rrc_ptr->gps_almanc.no_of_almanac_sat = sat_info_cnt;
  }

  /* Nav Model */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr, tdsrrc_UE_Positioning_GPS_AssistanceData_r7,
    ue_positioning_GPS_NavigationModel))
  {
    tdsrrc_navigation_model_sat_info_list_struct_type *rrc_info_ptr;
    tdsrrc_NavigationModelSatInfoList *asn1_info_ptr;

    rrc_ptr->gps_navigation_model_incl = TRUE;

    /* Traverse NavModel List */
    asn1_info_ptr = &asn1_ptr->ue_positioning_GPS_NavigationModel.navigationModelSatInfoList;
    rrc_info_ptr = &rrc_ptr->gps_navigation_model.satellite_info[0];
    sat_info_cnt = 0;

    for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
    {
      /* Check to see if we have reached the end of the linked list */
      if ( asn1_info_ptr == NULL )
      {
        break;
      }
      rrc_info_ptr->satellite_id = (uint8) asn1_info_ptr->elem[q_Cnt].satID;
      rrc_info_ptr->satellite_status = asn1_info_ptr->elem[q_Cnt].satelliteStatus;
      /* Ephemeris Parameters */
      if ( asn1_info_ptr->elem[q_Cnt].m.ephemerisParameterPresent )
      {
        rrc_info_ptr->ephermeral_clock_params_incl = TRUE;
        rrc_info_ptr->ephermeral_clock_params.a_Sqrt =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.a_Sqrt );
        rrc_info_ptr->ephermeral_clock_params.af0 =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.af0 );
        rrc_info_ptr->ephermeral_clock_params.af1 = 
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.af1 );
        rrc_info_ptr->ephermeral_clock_params.af2 =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.af2 );
        rrc_info_ptr->ephermeral_clock_params.aodo =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.aodo );
        rrc_info_ptr->ephermeral_clock_params.c_ic =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_ic );
        rrc_info_ptr->ephermeral_clock_params.c_is =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_is );
        rrc_info_ptr->ephermeral_clock_params.c_rc =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_rc );
        rrc_info_ptr->ephermeral_clock_params.c_rs =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_rs );
        rrc_info_ptr->ephermeral_clock_params.c_uc =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_uc );
        rrc_info_ptr->ephermeral_clock_params.c_us =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.c_us );
        rrc_info_ptr->ephermeral_clock_params.code_on_l2 =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.codeOnL2 );
        rrc_info_ptr->ephermeral_clock_params.delta_n =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.delta_n );
        rrc_info_ptr->ephermeral_clock_params.e =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.e );
        rrc_info_ptr->ephermeral_clock_params.fit_interval =
            (boolean) tdsrrcgps_oss_bitstring_to_int( sizeof(boolean), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.fitInterval );
        rrc_info_ptr->ephermeral_clock_params.i0 =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.i0 );
        rrc_info_ptr->ephermeral_clock_params.iDot =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.iDot );
        rrc_info_ptr->ephermeral_clock_params.iodc =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.iodc );
        rrc_info_ptr->ephermeral_clock_params.l2_pflag =
            (boolean) tdsrrcgps_oss_bitstring_to_int( sizeof(boolean), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.l2Pflag );
        rrc_info_ptr->ephermeral_clock_params.m0 =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.m0 );
        rrc_info_ptr->ephermeral_clock_params.omega =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.omega );
        rrc_info_ptr->ephermeral_clock_params.omega0 =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.omega0 );
        rrc_info_ptr->ephermeral_clock_params.omegaDot =
            (uint32) tdsrrcgps_oss_bitstring_to_int( sizeof(uint32), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.omegaDot );
        rrc_info_ptr->ephermeral_clock_params.sat_health =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.satHealth );
        rrc_info_ptr->ephermeral_clock_params.t_gd =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.t_GD );
        rrc_info_ptr->ephermeral_clock_params.t_oc =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.t_oc );
        rrc_info_ptr->ephermeral_clock_params.t_oe =
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.t_oe );
        rrc_info_ptr->ephermeral_clock_params.ura_index =
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].ephemerisParameter.uraIndex );
      }
      rrc_info_ptr++;
      sat_info_cnt++;
    }
    rrc_ptr->gps_navigation_model.no_of_satellites = sat_info_cnt;
  }

  /* Real Time Integrity */
  if( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr, tdsrrc_UE_Positioning_GPS_AssistanceData_r7,
    ue_positioning_GPS_Real_timeIntegrity))
  {
    tdsrrc_BadSatList *asn1_info_ptr;
    uint8 *bad_sat_ptr;

    rrc_ptr->gps_real_time_integrity_incl = TRUE;

    /* Fill in RTI values */
    asn1_info_ptr = &asn1_ptr->ue_positioning_GPS_Real_timeIntegrity;
    bad_sat_ptr = (uint8 *)&rrc_ptr->gps_rti.satellite_id[0];
    sat_info_cnt = 0;

    for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
    {
      /* Check to see if we have reached the end of the linked list */
      if ( asn1_info_ptr == NULL )
      {
          break;
      }
      *bad_sat_ptr = (uint8) asn1_info_ptr->elem[q_Cnt];
      bad_sat_ptr++;
      sat_info_cnt++;
    }

    rrc_ptr->gps_rti.no_of_satellites = sat_info_cnt;
  }
    
  /* Reference Location */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr, tdsrrc_UE_Positioning_GPS_AssistanceData_r7,
    ue_positioning_GPS_ReferenceLocation))
  {
    tdsrrc_gps_reference_loc_struct_type *rrc_ref_loc =
    &rrc_ptr->gps_ref_loc;
    tdsrrc_EllipsoidPointAltitudeEllipsoide *asn1_ref_loc =
    &asn1_ptr->ue_positioning_GPS_ReferenceLocation.ellipsoidPointAltitudeEllipsoide;

    rrc_ptr->gps_ref_loc_incl = TRUE;
    rrc_ref_loc->altitude = asn1_ref_loc->altitude;
    rrc_ref_loc->altitude_direction = asn1_ref_loc->altitudeDirection;
    rrc_ref_loc->confidence = asn1_ref_loc->confidence;
    rrc_ref_loc->latitude = asn1_ref_loc->latitude;
    rrc_ref_loc->latitude_sign = asn1_ref_loc->latitudeSign;
    rrc_ref_loc->longitude = asn1_ref_loc->longitude;
    rrc_ref_loc->orientation_major_axis = (uint8) asn1_ref_loc->orientationMajorAxis;
    rrc_ref_loc->uncertainity_altitude = asn1_ref_loc->uncertaintyAltitude;
    rrc_ref_loc->uncertainity_semi_major = (uint8) asn1_ref_loc->uncertaintySemiMajor;
    rrc_ref_loc->uncertainity_semi_minor = (uint8) asn1_ref_loc->uncertaintySemiMinor;
  }

  /* Reference Time */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr, tdsrrc_UE_Positioning_GPS_AssistanceData_r7,
    ue_positioning_GPS_ReferenceTime))
  {
    tdsrrc_gps_reference_time_struct_type *rrc_ref_time = &rrc_ptr->gps_ref_time;
    tdsrrc_UE_Positioning_GPS_ReferenceTime_r7 *asn1_ref_time = &asn1_ptr->ue_positioning_GPS_ReferenceTime;

    rrc_ptr->gps_ref_time_incl = TRUE;
    rrc_ref_time->time = asn1_ref_time->gps_tow_1msec;
    rrc_ref_time->week = asn1_ref_time->gps_Week;
  
    /* GPS Drift Rate */
    if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, tdsrrc_UE_Positioning_GPS_ReferenceTime_r7,
        utran_GPS_DriftRate))
    {
      rrc_ref_time->gps_drift_rate_incl = TRUE;
      rrc_ref_time->gps_drift_rate = asn1_ref_time->utran_GPS_DriftRate;
    }

    /* GPS - UTRAN reference time */
    if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, tdsrrc_UE_Positioning_GPS_ReferenceTime_r7,
        utran_GPSReferenceTime))
    {
      rrc_ref_time->gps_utran_reference_time_incl = TRUE;
      rrc_ref_time->gps_utran_ref_time.lsb_timing =
        asn1_ref_time->utran_GPSReferenceTime.utran_GPSTimingOfCell.ls_part;
      rrc_ref_time->gps_utran_ref_time.msb_timing =
        asn1_ref_time->utran_GPSReferenceTime.utran_GPSTimingOfCell.ms_part;
      rrc_ref_time->gps_utran_ref_time.sfn =
        asn1_ref_time->utran_GPSReferenceTime.sfn;

      /* Primary Scrambling Code */ 
      if (TDSRRC_MSG_COMMON_BITMASK_IE( asn1_ref_time->utran_GPSReferenceTime,
        tdsrrc_UTRAN_GPSReferenceTime,modeSpecificInfo))
      {
        rrc_ref_time->gps_utran_ref_time.gps_ref_cpid_incl = TRUE;

        /* UE Only supports FDD, grab primary scrambling code */
        rrc_ref_time->gps_utran_ref_time.cpid =
        asn1_ref_time->utran_GPSReferenceTime.modeSpecificInfo.u.fdd->referenceIdentity.primaryScramblingCode;
      }
    }

    /* SFN TOW Uncertainty */
    if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, tdsrrc_UE_Positioning_GPS_ReferenceTime_r7,
        sfn_tow_Uncertainty))
    {
      rrc_ref_time->sfn_tow_uncertainity_incl = TRUE;
      rrc_ref_time->sfn_tow_uncertainity = asn1_ref_time->sfn_tow_Uncertainty;
    }

    /* GPS TOW Assistance List */
    if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, tdsrrc_UE_Positioning_GPS_ReferenceTime_r7,
        gps_TOW_AssistList))
    {
      tdsrrc_GPS_TOW_AssistList *asn1_info_ptr;
      tdsrrc_gps_tow_assist_struct_type *rrc_info_ptr;

      asn1_info_ptr = &asn1_ref_time->gps_TOW_AssistList;
      rrc_info_ptr = &rrc_ref_time->gps_tow_assist[0];

      /* Traverse TOW Assist List */
      sat_info_cnt = 0;
      for ( q_Cnt = 0; ((q_Cnt < TDSMAX_NO_OF_SATELLITES)&& (asn1_info_ptr->n > q_Cnt)); q_Cnt++ )
      {
        /* Check to see if we have reached the end of the linked list */
        if ( asn1_info_ptr == NULL )
        {
          break;
        }

        rrc_info_ptr->satellite_identity = (uint8) asn1_info_ptr->elem[q_Cnt].satID;
        rrc_info_ptr->tlm_msg = 
            (uint16) tdsrrcgps_oss_bitstring_to_int( sizeof(uint16), &bs_failed, &asn1_info_ptr->elem[q_Cnt].tlm_Message );
        rrc_info_ptr->tlm_rsvd = 
            (uint8) tdsrrcgps_oss_bitstring_to_int( sizeof(uint8), &bs_failed, &asn1_info_ptr->elem[q_Cnt].tlm_Reserved );
        rrc_info_ptr->alert = asn1_info_ptr->elem[q_Cnt].alert;
        rrc_info_ptr->anti_spoof = asn1_info_ptr->elem[q_Cnt].antiSpoof;

        rrc_info_ptr++;
        sat_info_cnt++;                                                       
      }

      rrc_ref_time->no_of_gps_tow_assist = sat_info_cnt;
    }
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ref_time, tdsrrc_UE_Positioning_GPS_ReferenceTime_r7,
        ue_Positioning_GPS_ReferenceTimeUncertainty))
    {
      /*TBD*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel 7 GPS IE's TBD");
    }
  }

  /* Reference Cell Info */

  /* DGPS Corrections - Not currently supported by RRC layer */

  /* UTC Model - Not currently supported by RRC layer */

  /* IONO Model */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr, tdsrrc_UE_Positioning_GPS_AssistanceData_r7,
    ue_positioning_GPS_IonosphericModel))
  {
    tdsrrc_gps_iono_struct_type *rrc_iono = &rrc_ptr->gps_iono;

    tdsrrc_UE_Positioning_GPS_IonosphericModel *asn1_iono = 
      &asn1_ptr->ue_positioning_GPS_IonosphericModel;

    rrc_ptr->gps_iono_incl = TRUE;
    rrc_iono->alfa0 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->alpha0 );
    rrc_iono->alfa1 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->alpha1 );
    rrc_iono->alfa2 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->alpha2 );
    rrc_iono->alfa3 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->alpha3 );
    rrc_iono->beta0 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->beta0 );
    rrc_iono->beta1 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->beta1 );
    rrc_iono->beta2 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->beta2 );
    rrc_iono->beta3 = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_iono->beta3 );
  }

  /** UTC model ***/
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(asn1_ptr, tdsrrc_UE_Positioning_GPS_AssistanceData_r7,
                                        ue_positioning_GPS_UTC_Model))
  {
    tdsrrc_gps_utc_struct_type *rrc_utc = &rrc_ptr->gps_utc_model;

    tdsrrc_UE_Positioning_GPS_UTC_Model *asn1_utc = &asn1_ptr->ue_positioning_GPS_UTC_Model;

    rrc_ptr->gps_utc_model_incl = TRUE;
    rrc_utc->q_A0   = (uint32)tdsrrcgps_oss_bitstring_to_int(sizeof(uint32), &bs_failed, &asn1_utc->a0);
    rrc_utc->q_A1 =   (uint32)tdsrrcgps_oss_bitstring_to_int(sizeof(uint32), &bs_failed, &asn1_utc->a1);
    rrc_utc->u_DeltaTls = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->delta_t_LS );
    rrc_utc->u_DeltaTlsf = (uint8)tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->delta_t_LSF);
    rrc_utc->u_DN = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->dn );
    rrc_utc->u_Tot = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->t_ot );
    rrc_utc->u_WNlsf = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->wn_lsf );
    rrc_utc->u_WNt = (uint8) tdsrrcgps_oss_bitstring_to_int(sizeof(uint8), &bs_failed, &asn1_utc->wn_t );

  } 

  /* If any of the bitstring conversions failed, indicate this to caller by returning
     false. */
  if ( bs_failed )
  {
    #ifndef WIN32
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ASN.1 to RRC structure conversion encountered errors");
    #endif
    return( FALSE );
  }
  else
  {
    return( TRUE );
  }
}

#endif


#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM_R8

DESCRIPTION

    This function processes the rel 8 Measurement Control Message for
    A-GPS Measurements.

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm_r8
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr
  ,uint8 meas_ctrl_tr_id
)
{
  /* Process the A-GPS Measurement Control Message */
  uint8 count = 0, cnt = 0;
  
  uint16 deleted_meas_id = 0;
  
  /* Declare MCM config structure that goes to L1 */
  tdsrrc_meas_params_struct_type  gps_meas_params;
  
  /* Declare ptr to store message received from UTRAN */
  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  tdsrrc_MeasurementControl_r8_IEs* rrc_mcm_ptr = NULL; 

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  uint8 tdstransaction_id = TDSINVALID_TRANSACTION_ID;
  gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.environ_ch_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.hor_accuracy_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.sfn_gps_timing_required = FALSE;
  gps_meas_params.u.meas_ctrl_params.rpt_quan.additional_assistance_data_request = FALSE;
  gps_meas_params.u.meas_ctrl_params.gps_assistance_data_incl = FALSE;
  gps_meas_params.u.meas_ctrl_params.meas_start_incl = TRUE;
  
  /* Store the message */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)pos_mcm_ptr;
  tdstransaction_id = meas_ctrl_tr_id;

  /* By default, it is set to 0 => 1 */
  gps_meas_params.u.meas_ctrl_params.rpt_crit.u.periodic_crit.rpt_amount = 0;

  if(!(dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.t
    == T_tdsrrc_MeasurementControl_criticalExtensions_5_later_than_r4))
  {
    return FAILURE;
  }

  rrc_mcm_ptr =  &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.measurementControl_r8;

  /* Set Transaction Id and Meas Id */
  p_info_ptr->meas_id = gps_meas_params.u.meas_ctrl_params.meas_id =
    (uint8) rrc_mcm_ptr->measurementIdentity;
  
  gps_meas_params.u.meas_ctrl_params.config_conf_ind = TRUE;
  
  gps_meas_params.u.meas_ctrl_params.config_ack_num = (uint8) tdstransaction_id;
  
  gps_meas_params.u.meas_ctrl_params.rpt_quan.method = (uint8)TDSRRC_GPS;
  gps_meas_params.params_type = (uint8)TDSRRC_MEAS_CTRL_PARMS;
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS: GPS Meas choice %d",rrc_mcm_ptr->measurementCommand.t);

  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r8_setup))
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_SETUP;
    
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode)))
    {
      /* Send Meas Ctrl Failure */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Meas Reporting Mode missing for SETUP");
      
      /* Set transaction ID */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      
      return FAILURE;
    }
    else
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode =
       (uint8) TDSRET_POS_RPT_TYPE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    if(FAILURE == tdsrrcgps_fill_mcm_info_r8(p_info_ptr, &rrc_mcm_ptr->measurementCommand.u.setup->u.
      up_Measurement, &gps_meas_params.u.meas_ctrl_params, tdstransaction_id, FALSE))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"filling gps info failed");
      return FAILURE;
    }
  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r8_modify))
  {
    /* Addtl Meas processing later. Reporting crit stored in db later */
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8)TDSRRC_MEAS_MODIFY;
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode))
    {
      gps_meas_params.u.meas_ctrl_params.transfer_mode_incl = TRUE;
      gps_meas_params.u.meas_ctrl_params.tranfer_mode = (uint8)
        TDSRET_POS_RPT_TYPE(rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }
    
    /* Check if meas type present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
         rrc_MeasurementCommand_r8_modify,measurementType))
    {
      if(FAILURE == tdsrrcgps_fill_mcm_info_r8(p_info_ptr, &rrc_mcm_ptr->measurementCommand.u.modify->
         measurementType.u.up_Measurement, &gps_meas_params.u.meas_ctrl_params, tdstransaction_id,
         TRUE))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"filling gps info failed");
        return FAILURE;
      }
    } /* End of if of Meas Type present */
  } /* End of modify */
  else
  {
    gps_meas_params.u.meas_ctrl_params.meas_cmd = (uint8) TDSRRC_MEAS_DELETE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
    /* At present just store meas id in meas db */
    for ( count = 0; count < TDSMAX_POS_MEAS; count++ )
    {
      if (tdspos_meas_id_list[count].meas_id == gps_meas_params.u.meas_ctrl_params.meas_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS: Meas Id %d deleted from DB",gps_meas_params.u.meas_ctrl_params.meas_id);
        tdspos_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdspos_meas_id_list[count].meas_validity = TDSNOT_PRESENT;
        tdspos_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
        
        /* Also initialize additional meas */
        for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
        {
          tdspos_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break;
      }
    } /* End of for */
    deleted_meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
    /* Delete Addtl Meas if any from the DB */
    tdsrrcmeas_delete_addtl_meas(&deleted_meas_id);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas delete Request");
    tdsrrcgps_send_lsm_cmd(&gps_meas_params);
    return SUCCESS;
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: Sending GPS Meas Request");
  tdsrrcgps_send_lsm_cmd(&gps_meas_params);
  return SUCCESS;

}


/*===========================================================================

FUNCTION   RRCGPS_FILL_MCM_R8_INFO

DESCRIPTION

    This function fill the GPS structure for 
    A-GPS Measurements. 

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_fill_mcm_info_r8
(
  tdspos_info_struct_type* pos_info_ptr,                       
  tdsrrc_UE_Positioning_Measurement_r8 *up_Measurement_ptr,
  tdsrrc_meas_ctrl_struct_type *meas_ctrl_params_ptr,
  uint8 tdstransaction_id,
  boolean modify_meas_params_incl
)
{
  tdsmeas_validity_enum_type pos_meas_validity = TDSNOT_PRESENT;
  
  tdsrrc_UE_Positioning_ReportingQuantity_r8* rpt_quan_ptr = NULL;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  
  tdsrpt_crit_enum_type rpt_mode;

  rpt_quan_ptr = &up_Measurement_ptr->ue_positioning_ReportingQuantity;

  meas_ctrl_params_ptr->rpt_quan_incl = TRUE;
  /* Get Pos Method */
  meas_ctrl_params_ptr->rpt_quan.method_type =
    (uint8) TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
  
  if (tdsrrcgps_verify_positioning_capability_r8(rpt_quan_ptr)
    == FAILURE)
  {
    /* Send Meas Ctrl Failure */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Pos Capability error");
    
    /* Set transaction ID */
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
    /* Set error t */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    
    /* Send MCF msg to UTRAN */
    tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
    
    return FAILURE;
    
  }
  
  /* Get Reporting Quantity */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,
    tdsrrc_UE_Positioning_ReportingQuantity_r8,horizontalAccuracy))
  {
    meas_ctrl_params_ptr->rpt_quan.hor_accuracy_incl = TRUE;
    
    /* Set the 7 bits */
    meas_ctrl_params_ptr->rpt_quan.horizontal_accuracy = (uint8)
      (*(rpt_quan_ptr->horizontalAccuracy.data) | 0xfe00);
  }

  if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r8,verticalAccuracy))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r8,gANSSPositioningMethods))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r8,gANSSTimingOfCellWanted))||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rpt_quan_ptr,tdsrrc_UE_Positioning_ReportingQuantity_r8,gANSSCarrierPhaseMeasurementRequested)))
  {
    /*TBD*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel 8 IE's Support TBD");
  }
  
  /* AdditionalAssistanceDataRequest */
  if (rpt_quan_ptr->additionalAssistanceDataReq == TRUE)
  {
    meas_ctrl_params_ptr->rpt_quan.additional_assistance_data_request = TRUE;
  }
  
  if (rpt_quan_ptr->gps_TimingOfCellWanted == TRUE)
  {
    meas_ctrl_params_ptr->rpt_quan.sfn_gps_timing_required = TRUE;
  }
  
  
  /* Get Reporting Criteria */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(up_Measurement_ptr->reportCriteria,
    rrc_UE_Positioning_ReportCriteria_r7_ue_positioning_ReportingCriteria))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Event trigg crit unsupported");
    /* Set transaction ID */
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
    /* Set error t */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
    
    /* Send MCF msg to UTRAN */
    tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
    
    return FAILURE;
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(up_Measurement_ptr->reportCriteria,
    rrc_UE_Positioning_ReportCriteria_r7_noReporting))
  {
    meas_ctrl_params_ptr->rpt_crit.rpt_mode = (uint8)TDSRRC_NO_RPT;
  }
  else
  {
    meas_ctrl_params_ptr->rpt_crit.rpt_mode = (uint8)TDSRRC_PERIODIC_RPT;
    
    /* Get reporting criteria and interval */
    meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_amount = (uint8)tdsrrc_ReportingAmount_ra_Infinity;
  
    meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_amount = (uint8)
        up_Measurement_ptr->reportCriteria.u.periodicalReportingCriteria->reportingAmount;
  
    meas_ctrl_params_ptr->rpt_crit.u.periodic_crit.rpt_interval = (uint8)
      up_Measurement_ptr->reportCriteria.u.periodicalReportingCriteria->reportingInterval;
  
  }
  if(up_Measurement_ptr->m.ue_positioning_GPS_AssistanceDataPresent )
  {
  /* Now call LSM fn to store Meas Object if FEATURE_MMGPS; call a local function to do the same if FEATURE_CGPS_xxx */
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
    if (tdsrrcgps_convert_assistance_data_r7(&up_Measurement_ptr->ue_positioning_GPS_AssistanceData,
                                             &meas_ctrl_params_ptr->gps_assistance_data) == FALSE)
#else /* this will only be FEATURE_MMGPS */
    /*To be Required from GPS guys*/
    /*  if (lsm_convert_assistance_data_r7(&up_Measurement_ptr->ue_positioning_GPS_AssistanceData,
    &meas_ctrl_params_ptr->gps_assistance_data) == FALSE)*/
#endif
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"POS: Assistance Data conversion failed");
      
      /* Set transaction ID */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      
      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
      
      rrcmeas_mcf.error_cause = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      
      return FAILURE;
    }
    meas_ctrl_params_ptr->gps_assistance_data_incl = TRUE;
  }
  
  /* Check if Meas Validity present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(up_Measurement_ptr,
                                       & tdsrrc_UE_Positioning_Measurement_r8,measurementValidity))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:Meas validity %d (0/d:1/f:2/a)",up_Measurement_ptr->measurementValidity.ue_State);
    
    /* Store Measurement Validity */
    pos_meas_validity = TDSTVM_RET_MEAS_VALIDITY(up_Measurement_ptr->measurementValidity.ue_State);
    
    /* Check if Meas needs to be restarted */
    if (((pos_meas_validity == TDSSTATE_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)) ||
       ((pos_meas_validity == TDSALL_STATES_BUT_DCH) && (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: RRC val,MCM val differ");
      meas_ctrl_params_ptr->meas_start_incl = FALSE;
    }
  }    
  /* Store Pos Parameters in RRC Database */
  if(tdsrrcmeas_config_pos_db(pos_info_ptr, &pos_meas_validity, &rpt_mode, modify_meas_params_incl) == FALSE)
  {
     return FAILURE;
  }
  return SUCCESS;
}


/*===========================================================================

FUNCTION    RRCGPS_VERIFY_POSITIONING_CAPABILITY

DESCRIPTION

 Checks positioning capability to determine if UE could process
 A-GPS Measurement Control Message

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_verify_positioning_capability_r8
(
  const tdsrrc_UE_Positioning_ReportingQuantity_r8 *rpt_quan_ptr
)
{

  tdsrrc_positioning_method_type_enum_type pos_method_type;

  tm_umts_cp_tds_ue_pos_capability_struct_type pos_capability;  
  
  uecomdef_status_e_type verify_status = SUCCESS;

  pos_method_type = TDSRET_POS_METHOD_TYPE(rpt_quan_ptr->methodType);
  
  /* First get positioning capability from LSM */
  if(tdsrrcgps_cgps_ue_pos_capability_cb == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"GPS Call back function is NULL");
    return FAILURE;
  }
  else
  {
    tdsrrcgps_cgps_ue_pos_capability_cb(&pos_capability);
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"POS:LSM/TM returned capability %d",pos_capability.network_assisted_gps_support);

  if ( ( rpt_quan_ptr->gps_TimingOfCellWanted == TRUE  ) &&
       ( pos_capability.gps_timing_of_cell_frames_supported == FALSE )
     )
  {
    verify_status = FAILURE;
  }

  if ((rpt_quan_ptr->positioningMethod == tdsrrc_PositioningMethod_otdoa) &&
      (pos_capability.ue_based_otdoa_supported == FALSE))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Pos Method OTDOA unsupported");
    verify_status = FAILURE;
  }

  if ( rpt_quan_ptr->positioningMethod == tdsrrc_PositioningMethod_cellID )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Pos Method CELL ID unsupported");
    verify_status = FAILURE;
  }

  if ( verify_status == SUCCESS ) /* continue examining if still good */
  {

    switch ( pos_capability.network_assisted_gps_support )
    {
#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_UE_BASED:
#endif
      {
        /* Reject any UE Assisted Requests */
        if (pos_method_type == TDSRRC_UE_ASSISTED )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=UE BASED,Req=UE_ASSISTED");
        }

        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_NETWORK_BASED:
#endif
      {
        /* Reject UE Based if we don't support standalone */
        if ( ( pos_method_type == TDSRRC_UE_BASED ) && 
             ( pos_capability.standalone_location_method_supported == FALSE ) )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=NW BASED, Req=UE_BASED,No Standalone");
        }
        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_BOTH:
#endif
      {
        /* Never Reject */
        break;
      }

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
      case TDS_CLASSMARK_UE_CAPABILITY_NONE:
#endif
      {
        /* Reject everything unless UE Based is requested and standalone is supported */
        if ( ( pos_method_type == TDSRRC_UE_ASSISTED ) || 
             ( pos_capability.standalone_location_method_supported == FALSE ) )
        {
          verify_status = FAILURE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS: REJECT Cap=NONE,Req=UE_ASSISTED or No Standalone");
        }
        break;
      }

      default:
        break;
    }

  } /* if verify_status is still SUCCESS */

  if ( verify_status == SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:UE cap check passed");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"POS:UE cap check failed");
  }
  
  return verify_status;
}


#endif /*FEATURE_TDSCDMA_REL8*/

#endif  /* FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP_TDSCDMA */








