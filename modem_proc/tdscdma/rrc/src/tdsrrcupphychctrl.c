/*===========================================================================
              R R C UPLINK PHYSICAL CHANNEL CONTROL  M O D U L E 

DESCRIPTION

  This module performs the Uplink Physical Channel Control Procedure. This
  module processes the Uplink Physical Channel Control Message and send 
  the message to L1.
  
  
EXTERNALIZED FUNCTIONS

  tdsrrcupcc_procedure_event_handler   Event handler for this procedure

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Procedure must be initialized at power-up by calling tdsrrcupcc_init_procedure. 
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcupphychctrl.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/08/10   ysh     Created file.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "err.h"
#include "msg.h"
#include "memheap.h"
#include "rex.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcdata.h"
#include "tdsrrcintif.h"
#include "tdsrrcllc.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsend.h"
#include "tdsrrctmr.h"
#include "tdsrrcsmc.h"
#include "tdsrrclcm.h"
#include "tdsrrclogging.h"

#include "tdsrrcmisc.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsrrcdormancy.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */



/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION tdsrrcupcc_get_upphych_control_info_r4


DESCRIPTION

  This is get UplinkPhysicalChannelControl IE.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
uecomdef_status_e_type tdsrrcupcc_get_upphych_control_info_r4(
  tdsl1_req_cmd  *llc_cmd_l1_req,
  tdsrrc_UplinkPhysicalChannelControl_r4  *upcc_r4
  )
{
  tdsrrc_UplinkPhysicalChannelControl_r4_IEs  *upcc_ptr_r4;
  
  upcc_ptr_r4 = &upcc_r4->uplinkPhysicalChannelControl_r4;

  if (upcc_ptr_r4->m.ccTrCH_PowerControlInfoPresent)
  {
    llc_cmd_l1_req->upphych.cctrch_pwr_ctrl_info_incl = TRUE;
    if (upcc_ptr_r4->ccTrCH_PowerControlInfo.m.tfcs_IdentityPresent)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UplinkPhysicalChannelControl's TFCS_ID present %d",upcc_ptr_r4->ccTrCH_PowerControlInfo.tfcs_Identity.tfcs_ID);
    }
    if( T_tdsrrc_UL_DPCH_PowerControlInfo_r4_tdd ==
            upcc_ptr_r4->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.t)
    {
      /* 0 – no such parameter */
      llc_cmd_l1_req->upphych.prx_dpch_des = 0;
      if (upcc_ptr_r4->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->m.ul_TargetSIRPresent)
      {
        /* -- The IE ul-TargetSIR corresponds to PRX-DPCHdes for 1.28Mcps TDD
         * -- Actual value PRX-DPCHdes = (value of IE "ul-TargetSIR" - 120)
         */
        llc_cmd_l1_req->upphych.prx_dpch_des = (int8)(upcc_ptr_r4->ccTrCH_PowerControlInfo.
             ul_DPCH_PowerControlInfo.u.tdd->ul_TargetSIR - 120);
      }
      /* Enumerated */
      /* Check with System team in this case: should Beacon PL Est be default set to FALSE for r4  */
      /* This means the enumerate only has one item: true. need check if 
       * option for UplinkPhysicalChannelControl-v690ext-IEs */
      /* check option bit for such extension. ture depends on if the option is set or not */
      if ((upcc_r4->m.v4d0NonCriticalExtensionsPresent)&&
           (upcc_r4->v4d0NonCriticalExtensions.m.v690NonCriticalExtensionsPresent)&&
           (upcc_r4->v4d0NonCriticalExtensions.v690NonCriticalExtensions.
                uplinkPhysicalChannelControl_v690ext.m.beaconPLEstPresent))
      {
        llc_cmd_l1_req->upphych.beacon_pl_est = TRUE;
      }
      else
      {
        llc_cmd_l1_req->upphych.beacon_pl_est = FALSE;
      }
      /* if the CHOICE UL OL PC info is set to 'Broadcast UL OL PC info':
       * set the variable INVALID_CONFIGURATION to true. 8.6.6.11
       */
      if (T_tdsrrc_UL_DPCH_PowerControlInfo_r4_ul_OL_PC_Signalling_broadcast_UL_OL_PC_info == 
                upcc_ptr_r4->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->ul_OL_PC_Signalling.t)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Broadcast UL OL PC info Configuration not supported. ");
        return FAILURE;
      }
      llc_cmd_l1_req->upphych.pccpch_tx_pwr = upcc_ptr_r4->ccTrCH_PowerControlInfo.
                 ul_DPCH_PowerControlInfo.u.tdd->ul_OL_PC_Signalling.u.individuallySignalled->primaryCCPCH_TX_Power;
      if (T_tdsrrc_UL_DPCH_PowerControlInfo_r4_tddOption_tdd128 ==
            upcc_ptr_r4->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.
            u.tdd->ul_OL_PC_Signalling.u.individuallySignalled->tddOption.t)
      {
        /* TPC-StepSizeTDD ::= INTEGER (1..3) */
        switch (upcc_ptr_r4->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->
                  ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128->tpc_StepSize)
        {
          case 1:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_1DB;
            break;
          case 2:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_2DB;
            break;
          case 3:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_3DB;
            break;
          default:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"UplinkPhysicalChannelControl invalid tpc_step_size. ");
            return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"3.84/7.68TDD Configuration not supported. ");
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD UL_DPCH_PowerControlInfo Configuration not supported. ");
      return FAILURE;
    }
  }
  else
  { 
    llc_cmd_l1_req->upphych.cctrch_pwr_ctrl_info_incl = FALSE;
  }
  /* Special burst scheduling included. */
  if (upcc_ptr_r4->m.specialBurstSchedulingPresent)
  {
    llc_cmd_l1_req->upphych.sb_scheduling_incl = TRUE;
    llc_cmd_l1_req->upphych.sb_gen_prd = upcc_ptr_r4->specialBurstScheduling;
  }
  else
  {
    llc_cmd_l1_req->upphych.sb_scheduling_incl = FALSE;
  }
  if (T_tdsrrc_UplinkPhysicalChannelControl_r4_IEs_tddOption_tdd128 == upcc_ptr_r4->tddOption.t)
  {
    /* Default value is 1 */
    llc_cmd_l1_req->upphych.ul_sync_step_size = 1;
    llc_cmd_l1_req->upphych.ul_sync_frequency = 1;
    if (upcc_ptr_r4->tddOption.u.tdd128.m.ul_SynchronisationParametersPresent)
    {
      llc_cmd_l1_req->upphych.ul_sync_step_size = upcc_ptr_r4->tddOption.u.tdd128.ul_SynchronisationParameters.stepSize;
      llc_cmd_l1_req->upphych.ul_sync_frequency = upcc_ptr_r4->tddOption.u.tdd128.ul_SynchronisationParameters.frequency;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"3.84TDD Configuration not supported. ");
    return FAILURE;
  }

  return SUCCESS;
} /* end function tdsrrcupcc_get_upphych_control_info_r4 */

/*===========================================================================

FUNCTION tdsrrcupcc_get_upphych_control_info_r5


DESCRIPTION

  This is get UplinkPhysicalChannelControl IE.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
uecomdef_status_e_type tdsrrcupcc_get_upphych_control_info_r5(
  tdsl1_req_cmd  *llc_cmd_l1_req,
  tdsrrc_UplinkPhysicalChannelControl_r5  *upcc_r5
  )
{
  tdsrrc_UplinkPhysicalChannelControl_r5_IEs  *upcc_ptr_r5;
  
  upcc_ptr_r5 = &upcc_r5->uplinkPhysicalChannelControl_r5;

  if (upcc_ptr_r5->m.ccTrCH_PowerControlInfoPresent)
  {
    llc_cmd_l1_req->upphych.cctrch_pwr_ctrl_info_incl = TRUE;
    if (upcc_ptr_r5->ccTrCH_PowerControlInfo.m.tfcs_IdentityPresent)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UplinkPhysicalChannelControl's TFCS_ID present %d",upcc_ptr_r5->ccTrCH_PowerControlInfo.tfcs_Identity.tfcs_ID);
    }
    if( T_tdsrrc_UL_DPCH_PowerControlInfo_r4_tdd ==
            upcc_ptr_r5->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.t)
    {
      /* 0 – no such parameter */
      llc_cmd_l1_req->upphych.prx_dpch_des = 0;
      if (upcc_ptr_r5->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->m.ul_TargetSIRPresent)
      {
        /* -- The IE ul-TargetSIR corresponds to PRX-DPCHdes for 1.28Mcps TDD
         * -- Actual value PRX-DPCHdes = (value of IE "ul-TargetSIR" - 120)
         */
        llc_cmd_l1_req->upphych.prx_dpch_des = (int8)(upcc_ptr_r5->ccTrCH_PowerControlInfo.
             ul_DPCH_PowerControlInfo.u.tdd->ul_TargetSIR - 120);
      }
      /* Enumerated */
      /* Check with System team in this case: should Beacon PL Est be default set to FALSE for r4  */
      /* This means the enumerate only has one item: true. need check if 
       * option for UplinkPhysicalChannelControl-v690ext-IEs */
      /* check option bit for such extension. ture depends on if the option is set or not */
      if ((upcc_r5->m.v690NonCriticalExtensionsPresent)&&
           (upcc_r5->v690NonCriticalExtensions.
                uplinkPhysicalChannelControl_v690ext.m.beaconPLEstPresent))
      {
        llc_cmd_l1_req->upphych.beacon_pl_est = TRUE;
      }
      else
      {
        llc_cmd_l1_req->upphych.beacon_pl_est = FALSE;
      }
      /* if the CHOICE UL OL PC info is set to 'Broadcast UL OL PC info':
       * set the variable INVALID_CONFIGURATION to true. 8.6.6.11
       */
      if (T_tdsrrc_UL_DPCH_PowerControlInfo_r4_ul_OL_PC_Signalling_broadcast_UL_OL_PC_info == 
                upcc_ptr_r5->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->ul_OL_PC_Signalling.t)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Broadcast UL OL PC info Configuration not supported. ");
        return FAILURE;
      }
      llc_cmd_l1_req->upphych.pccpch_tx_pwr = upcc_ptr_r5->ccTrCH_PowerControlInfo.
                 ul_DPCH_PowerControlInfo.u.tdd->ul_OL_PC_Signalling.u.individuallySignalled->primaryCCPCH_TX_Power;
      if (T_tdsrrc_UL_DPCH_PowerControlInfo_r4_tddOption_tdd128 ==
            upcc_ptr_r5->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.
            u.tdd->ul_OL_PC_Signalling.u.individuallySignalled->tddOption.t)
      {
        /* TPC-StepSizeTDD ::= INTEGER (1..3) */
        switch (upcc_ptr_r5->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->
                  ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128->tpc_StepSize)
        {
          case 1:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_1DB;
            break;
          case 2:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_2DB;
            break;
          case 3:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_3DB;
            break;
          default:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"UplinkPhysicalChannelControl invalid tpc_step_size. ");
            return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"3.84/7.68TDD Configuration not supported. ");
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD UL_DPCH_PowerControlInfo Configuration not supported. ");
      return FAILURE;
    }
  }
  else
  { 
    llc_cmd_l1_req->upphych.cctrch_pwr_ctrl_info_incl = FALSE;
  }
  /* Special burst scheduling included. */
  if (upcc_ptr_r5->m.specialBurstSchedulingPresent)
  {
    llc_cmd_l1_req->upphych.sb_scheduling_incl = TRUE;
    llc_cmd_l1_req->upphych.sb_gen_prd = upcc_ptr_r5->specialBurstScheduling;
  }
  else
  {
    llc_cmd_l1_req->upphych.sb_scheduling_incl = FALSE;
  }
  if (T_tdsrrc_UplinkPhysicalChannelControl_r4_IEs_tddOption_tdd128 == upcc_ptr_r5->tddOption.t)
  {
    /* Default value is 1 */
    llc_cmd_l1_req->upphych.ul_sync_step_size = 1;
    llc_cmd_l1_req->upphych.ul_sync_frequency = 1;
    if (upcc_ptr_r5->tddOption.u.tdd128.m.ul_SynchronisationParametersPresent)
    {
      llc_cmd_l1_req->upphych.ul_sync_step_size = upcc_ptr_r5->tddOption.u.tdd128.ul_SynchronisationParameters.stepSize;
      llc_cmd_l1_req->upphych.ul_sync_frequency = upcc_ptr_r5->tddOption.u.tdd128.ul_SynchronisationParameters.frequency;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"3.84TDD Configuration not supported. ");
    return FAILURE;
  }

  return SUCCESS;
} /* end function tdsrrcupcc_get_upphych_control_info_r5 */

/*===========================================================================

FUNCTION tdsrrcupcc_get_upphych_control_info_r7


DESCRIPTION

  This is get UplinkPhysicalChannelControl IE.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
uecomdef_status_e_type tdsrrcupcc_get_upphych_control_info_r7(
  tdsl1_req_cmd  *llc_cmd_l1_req,
  tdsrrc_UplinkPhysicalChannelControl_r7_IEs  *upcc_ptr_r7
  )
{

  if (upcc_ptr_r7->m.ccTrCH_PowerControlInfoPresent)
  {
    llc_cmd_l1_req->upphych.cctrch_pwr_ctrl_info_incl = TRUE;
    if (upcc_ptr_r7->ccTrCH_PowerControlInfo.m.tfcs_IdentityPresent)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UplinkPhysicalChannelControl's TFCS_ID present %d",upcc_ptr_r7->ccTrCH_PowerControlInfo.tfcs_Identity.tfcs_ID);
    }
    if( T_tdsrrc_UL_DPCH_PowerControlInfo_r4_tdd ==
            upcc_ptr_r7->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.t)
    {
      /* 0 – no such parameter */
      llc_cmd_l1_req->upphych.prx_dpch_des = 0;
      if (upcc_ptr_r7->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->m.ul_TargetSIRPresent)
      {
        /* -- The IE ul-TargetSIR corresponds to PRX-DPCHdes for 1.28Mcps TDD
         * -- Actual value PRX-DPCHdes = (value of IE "ul-TargetSIR" - 120)
         */
        llc_cmd_l1_req->upphych.prx_dpch_des = (int8)(upcc_ptr_r7->ccTrCH_PowerControlInfo.
             ul_DPCH_PowerControlInfo.u.tdd->ul_TargetSIR - 120);
      }
      /* if the CHOICE UL OL PC info is set to 'Broadcast UL OL PC info':
       * set the variable INVALID_CONFIGURATION to true. 8.6.6.11
       */
      if (T_tdsrrc_UL_DPCH_PowerControlInfo_r7_ul_OL_PC_Signalling_broadcast_UL_OL_PC_info == 
           upcc_ptr_r7->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->ul_OL_PC_Signalling.t)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Broadcast UL OL PC info Configuration not supported. ");
        return FAILURE;
      }
      llc_cmd_l1_req->upphych.pccpch_tx_pwr = upcc_ptr_r7->ccTrCH_PowerControlInfo.
                ul_DPCH_PowerControlInfo.u.tdd->ul_OL_PC_Signalling.u.individuallySignalled->primaryCCPCH_TX_Power;
      if (T_tdsrrc_UL_DPCH_PowerControlInfo_r7_tddOption_tdd128 ==
            upcc_ptr_r7->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.
            u.tdd->ul_OL_PC_Signalling.u.individuallySignalled->tddOption.t)
      {
        /* TPC-StepSizeTDD ::= INTEGER (1..3) */
        switch (upcc_ptr_r7->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->
                  ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128->tpc_StepSize)
        {
          case 1:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_1DB;
            break;
          case 2:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_2DB;
            break;
          case 3:
            llc_cmd_l1_req->upphych.tpc_step_size = TDSL1_TPC_STEP_SIZE_3DB;
            break;
          default:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"UplinkPhysicalChannelControl invalid tpc_step_size. ");
            return FAILURE;
        }
        /* Enumerated(true) */
        if (upcc_ptr_r7->ccTrCH_PowerControlInfo.ul_DPCH_PowerControlInfo.u.tdd->
                     ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128->m.beaconPLEstPresent)
        {
          llc_cmd_l1_req->upphych.beacon_pl_est = TRUE;
        }
        else
        {
          llc_cmd_l1_req->upphych.beacon_pl_est = FALSE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"3.84/7.68TDD Configuration not supported. ");
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD UL_DPCH_PowerControlInfo Configuration not supported. ");
      return FAILURE;
    }
  }
  else
  { 
    llc_cmd_l1_req->upphych.cctrch_pwr_ctrl_info_incl = FALSE;
  }
  /* Special burst scheduling included. */
  if (upcc_ptr_r7->m.specialBurstSchedulingPresent)
  {
    llc_cmd_l1_req->upphych.sb_scheduling_incl = TRUE;
    llc_cmd_l1_req->upphych.sb_gen_prd = upcc_ptr_r7->specialBurstScheduling;
  }
  else
  {
    llc_cmd_l1_req->upphych.sb_scheduling_incl = FALSE;
  }
  if (T_tdsrrc_UplinkPhysicalChannelControl_r7_IEs_tddOption_tdd128 == upcc_ptr_r7->tddOption.t)
  {
    /* Default value is 1 */
    llc_cmd_l1_req->upphych.ul_sync_step_size = 1;
    llc_cmd_l1_req->upphych.ul_sync_frequency = 1;
    if (upcc_ptr_r7->tddOption.u.tdd128.m.ul_SynchronisationParametersPresent)
    {
      llc_cmd_l1_req->upphych.ul_sync_step_size = upcc_ptr_r7->tddOption.u.tdd128.ul_SynchronisationParameters.stepSize;
      llc_cmd_l1_req->upphych.ul_sync_frequency = upcc_ptr_r7->tddOption.u.tdd128.ul_SynchronisationParameters.frequency;
    }
    /* 0 – no such parameter */
    llc_cmd_l1_req->upphych.prx_hssich = 0;
    if (upcc_ptr_r7->tddOption.u.tdd128.m.desired_HS_SICH_PowerLevelPresent)
    {
      /* (-120..-58 bystep of 1)In dBm. Desiredower level for HS-SICH. r7 */
      llc_cmd_l1_req->upphych.prx_hssich = upcc_ptr_r7->tddOption.u.tdd128.desired_HS_SICH_PowerLevel;
    }
    llc_cmd_l1_req->upphych.hssich_tpc_step_size_incl = FALSE;
    if (upcc_ptr_r7->tddOption.u.tdd128.m.tpc_Step_SizePresent)
    {
      llc_cmd_l1_req->upphych.hssich_tpc_step_size_incl = TRUE;
      /* tdsl1_tpc_step_size_enum_type(1, 2, 3)In dB. For HS- SICH. r7 */
      switch (upcc_ptr_r7->tddOption.u.tdd128.tpc_Step_Size)
      {
        case tdsrrc_UplinkPhysicalChannelControl_r7_IEs_tpc_Step_Size_s1:
          llc_cmd_l1_req->upphych.hssich_tpc_step_size = TDSL1_TPC_STEP_SIZE_1DB;
          break;
        case tdsrrc_UplinkPhysicalChannelControl_r7_IEs_tpc_Step_Size_s2:
          llc_cmd_l1_req->upphych.hssich_tpc_step_size = TDSL1_TPC_STEP_SIZE_2DB;
          break;
        case tdsrrc_UplinkPhysicalChannelControl_r7_IEs_tpc_Step_Size_s3:
          llc_cmd_l1_req->upphych.hssich_tpc_step_size = TDSL1_TPC_STEP_SIZE_3DB;
          break;
        case tdsrrc_UplinkPhysicalChannelControl_r7_IEs_tpc_Step_Size_spare1:
          llc_cmd_l1_req->upphych.hssich_tpc_step_size_incl = FALSE;
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"UplinkPhysicalChannelControl invalid tpc_step_size. ");
          return FAILURE;
      }
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"7.68TDD Configuration not supported. ");
    return FAILURE;
  }

  return SUCCESS;
} /* end function tdsrrcupcc_get_upphych_control_info_r7 */

/*===========================================================================

FUNCTION tdsrrcupcc_get_upphych_control_info


DESCRIPTION

  This is get UplinkPhysicalChannelControl IE.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
uecomdef_status_e_type tdsrrcupcc_get_upphych_control_info(
  tdsl1_req_cmd  *llc_cmd_l1_req,
  tdsrrc_DL_DCCH_Message  *dcch_msg_ptr
  )
{
  tdsrrc_UplinkPhysicalChannelControl  *msg_ptr;  /* Pointer to the message        */

  tdsrrc_UplinkPhysicalChannelControl_r4     *upcc_ptr_r4;
  tdsrrc_UplinkPhysicalChannelControl_r5     *upcc_ptr_r5;
  tdsrrc_UplinkPhysicalChannelControl_r7_IEs     *upcc_ptr_r7;

  /* Get the pointer to the actual PC Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.uplinkPhysicalChannelControl);

  if (msg_ptr->t != T_tdsrrc_UplinkPhysicalChannelControl_r3)
  {
    if (msg_ptr->u.later_than_r3.criticalExtensions.t == 
           T_tdsrrc_UplinkPhysicalChannelControl_criticalExtensions_3_r4)
    {
      upcc_ptr_r4 = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4;
      if (FAILURE == tdsrrcupcc_get_upphych_control_info_r4(llc_cmd_l1_req, upcc_ptr_r4))
      {
        return FAILURE;
      }
    }
    else
    if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_UplinkPhysicalChannelControl_criticalExtensions_2_r5))
    {
      upcc_ptr_r5 = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5;
      if (FAILURE == tdsrrcupcc_get_upphych_control_info_r5(llc_cmd_l1_req, upcc_ptr_r5))
      {
        return FAILURE;
      }
    }
    else
    if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) && 
              (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t ==
               T_tdsrrc_UplinkPhysicalChannelControl_criticalExtensions_1_r7))
    {
      upcc_ptr_r7 = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.uplinkPhysicalChannelControl_r7;
      if (FAILURE == tdsrrcupcc_get_upphych_control_info_r7(llc_cmd_l1_req, upcc_ptr_r7))
      {
        return FAILURE;
      }
    }
    else
    {
      tdsrrc_print_supported_asn1_rel_version();

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Configuration not supported.");
      return FAILURE;
    }
  }
  else
  {
      tdsrrc_print_supported_asn1_rel_version();

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Configuration not supported.");
      return FAILURE;
  }
  return SUCCESS;
} /* end function tdsrrcupcc_get_upphych_control_info */


/*===========================================================================

FUNCTION RRCUPCC_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  UPCC procedure. This function is called by the
  dispatcher whenever an event is received for this module. This function
  checks the current RRC state and calls the event handler for
  that state. 
  
  Note that the UPCC procedure can be initiated
  only in the ELL_DCH states.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrcupcc_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  tdsrrc_DL_DCCH_Message         *dcch_msg_ptr = NULL;  /* Pointer to the PC Reconfig message */
 
  /* Ptr which carries info about the command */
  tdsrrc_cmd_type        *rrc_upphy_out_cmd_ptr = NULL;
  tdsl1_req_cmd          *llc_cmd_l1_req = NULL;
  /* because tdsrrc_free_cmd_buf() can't free buf rrc_upphy_out_cmd_ptr. define a struct varible */
  tdsl1_req_cmd          llc_cmd_l1_req_temp;
  uint8 message_choice;                    /* Local varibale for message choice*/ 
  rlc_lc_id_type         lc_id;                      /* Local variable to store */

  /* Get the current RRC State */
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

 /* Assert received message is Downlink DCCH message */
  if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU) 
  {
	message_choice = 
	    tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
									cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

	/*check if decoding was successful otherwise set error flag to true*/
	tdsrrc_protocol_error_reject = FALSE;

	if (cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
	{
		dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
		cmd_ptr->cmd.downlink_sdu_ind.decoded_msg; 
	}
	else
	{ /* ASN1 decoding failed */
	  tdsrrc_protocol_error_reject = TRUE;
	} 

	 /* Check the received message is Uplink PhysicalChannel Control message */
   if( message_choice ==
		T_tdsrrc_DL_DCCH_MessageType_uplinkPhysicalChannelControl)
	{
	  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received Uplink PhysicalChannel Control Message");

	  if(tdsrrc_protocol_error_reject == TRUE)
	  {

#if(!defined(T_WINNT))
		/*log the protocol error*/
		tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
							   (uint8)TDSRRC_PROCEDURE_UPCC,
							   TDSRRCLOG_PROTOCOL_ERROR,
							   TDSRRCLOG_ASN1_ENC_ERR
							  );
#endif
		lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
				UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
				DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
											the above Downlink Logical 
											Channel type. */
				UE_MODE_ACKNOWLEDGED_DATA);
										  /* Mode of transmission for the RLC 
											 service entity. */
		if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
		{
		  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
		} 
		else /* Valid RLC ID found */
		{
		  if(SUCCESS != tdsrrcsend_send_rrc_status_message(
			 T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError,
										   // The protocol error cause to be 
										   // placed in the message
			 0,                            // Set transaction id to 0 since
										   // ASN1 decoding failed
			 tdsrrc_ReceivedMessageType_uplinkPhysicalChannelControl,     // Id  of the received message
			 TDSRRC_PROCEDURE_UPCC,            // RRC procedure
			 lc_id,                        /* Logical Channel Id */
			 TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
									 /* Indicates whether L2 Ack required or not */
		  {
			TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
		  }  
		  /* Reset the state machine and procedure ends */
		} /* Valid RLC ID found */
		return;
	  }

	  /* Process State Change to Idle Disconnected in all substates */
	  if (rrc_state == TDSRRC_STATE_CELL_DCH) 
	  {
		if (SUCCESS == tdsrrcupcc_get_upphych_control_info(&llc_cmd_l1_req_temp, dcch_msg_ptr))
		{
		  if ( (rrc_upphy_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL )
		  {
			ERR_FATAL("Memory Failure for RRC command",0,0,0);
			/* ERR_FATAL would take care of bringing down RRC */
		  }
		  else
		  {
			tdsrrc_get_pointer_l1_req_value(&rrc_upphy_out_cmd_ptr->cmd.rrc_llc_req.l1_req, &llc_cmd_l1_req);

         (void) memscpy(llc_cmd_l1_req, 
                        sizeof(tdsl1_req_cmd),
                        &llc_cmd_l1_req_temp, 
                        sizeof(tdsl1_req_cmd));

			/* Initialize the LLC command header first */
			rrc_upphy_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
			rrc_upphy_out_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_UPCC;
	      
			rrc_upphy_out_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
	      
			/* Now fill the L1 cmd header */
			rrc_upphy_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_UL_PHY_CHANNEL_CTRL_REQ;
	      
			/* act_time_type */
			rrc_upphy_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type =
			   TDSL1_ACTIVATION_TIME_NONE;
		  }

		  /* Send the Internal RRC Command - LLC should directly handle it */
		  tdsrrc_put_int_cmd( rrc_upphy_out_cmd_ptr );
		}
	  	}
	  }
	else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid msg: %d is forwarded",message_choice);
    }
   }
   else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d or SDU type: %d",cmd_ptr->cmd_hdr.cmd_id,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
  }

  return;
}/* end function tdsrrcupcc_procedure_event_handler */
