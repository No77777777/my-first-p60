/*===========================================================================

                L3 - RRC Lower Layer Controller Physical Channel IEs

DESCRIPTION
  This file contains functions necessary for the RRC Lower Layer Controller
  Ordered Config Physical Channel Information Element Processing.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcllcpcie.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
12/10/11   ttl      Set TPC Step size w/ the value from OTA otherwise set to TDSL1_TPC_STEP_SIZE_0DB if missed
                    from OTA.  
12/07/11   hx       Update CFN and DOFF handling for fault-tolerance cases   
11/22/11   zy       Fixed UL timeslot number out of range when UplinkAdditionalTimeslots is same as last.
10/25/11   ysh      Modify req_mak in cphy_setup_req.
08/11/11   zwj      reconfig HSUPA when CV IEs changed.(3GPP 10.3.6.41c and 10.3.6.78d).
08/04/11   ysh      delete save pdsch info.
06/16/11   bj       Added R9 version IE: HS-PDSCH info and UL E-DCH info.
06/14/11   yzh      Made changes from FDD to TDD in R8
03/23/11   rl       Fixed compiler error on enabling CPC_DRX feature
03/18/11   ad       Fixed the bug to check the no of elements in the list 
                    in set_compressed_mode_info_r8 function
03/17/11   rl       Fixed the compiler errors/warning after enabling 
                    FEATURE_WCDMA_CM_LTE_SEARCH
03/11/11   as       Fixed bug to increment loop variable correctly
                    in dl_info_per_rl for objsys
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
03/04/11   rl       Merged with VU_MODEM_WCDMA_RRC.01.92.50
03/02/11   ad       Added changes to check if DC-HSDPA is active 
                    if DL_SecondaryCellInfoFDD config is chosen as continue.
02/07/11   rl       Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/27/11   rm       Added code to support BC19 under FEATURE_WCDMA_BC19
01/28/11   rl       Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   bj       (3GPP: 10.3.6.55)Added code to store IE "e-RUCCH-Sync-UL-Codes-Bitmap" of 
                          "PRACH-RACH-Info-LCR-v770ext".
01/11/11   bj       (3GPP: 8.5.28)Modify actions related to.E_DCH_TRANSMISSION variable from FDD to TDD.
01/17/11   zwj      3GPP: 8.6.6.37 &10.3.6.97. Add E-DCH info for HSUPA.
01/12/11   ad       Added changes for RRC SW decoupling
01/08/11   ysh     IE "Downlink information for each radio link" modify from FDD to TDD. 3GPP-8.6.6.4(3.6.17).
01/06/11   ysh     IE "E-DCH Transmission Time Interval" delete. 3GPP-8.6.5.5(3.6.16). 
                               Delete save handling.
01/04/11   su       Made changes to configure mac-hs with bit aligned on Mac-ehs to Mac-hs 
                    transition when Downlink HS-PDSCH information IE is missing
12/31/10   rl       Merged with VU_MODEM_WCDMA_RRC.01.86.50
12/11/10   rl       Fixed compiler errors for FEATURE_TDSCDMA_CPC_DTX
11/12/10   yzh      (3GPP: 8.5.25&10.3.6.23a)Modify Actions related to HS_DSCH_RECEPTION variable
                    and Downlink HS-PDSCH Information(FDD->TDD).
11/18/10   ysh     Frequency info save. 3GPP-8.6.6.1(3.4.2).
                        Two freq save.
11/11/10   as       Fixed compiler errors and ASN 9.3 Migration changes for MIMO
11/11/10   yzh      (3GPP: 8.6.6.27)Modify IE Downlink information common for all radio links(FDD->TDD)
                     and (3GPP: 8.6.6.28a)Del Downlink F-DPCH info common for all radio links(FDD),
                     and (3GPP: 10.3.6.85a)Add code for TSTD indicator in DL-informationperRL.
11/09/10   rl       Merged with VU_MODEM_WCDMA_RRC.01.84.50
11/09/10   ysh      RLC size of Transport Format Set calculate method FDD->TDD. 3GPP-8.6.5.1/11.1(3.2.32)
                                   rach_TransportFormatSet -> rach_TransportFormatSet_LCR
11/04/10    yzh    (3GPP 10.3.6.21) modify Downlink DPCH info for each RL from FDD to TDD.
11/01/10    yzh    (3GPP 8.6.6.6) modify Uplink DPCH info from FDD to TDD
11/05/10   yzh      (3GPP: 8.6.6.15)Delete DPCH Compressed mode info(FDD only, under FEATURE_CM_SUPPORTED)(3.2.20).
11/03/10   ysh      IE "RLC size index" of RB mapping info, FDD->TDD, means change.  3GPP-8.6.4.8(3.2.1).
11/02/10   su       Fixed Klocwork warnings.
11/02/10   rm       Added code to support W2L measurements in DCH state under
                    FEATURE_WCDMA_CM_LTE_SEARCH
11/02/10   zwj      Added code to handle REL4 OTA message.  
10/11/01   bj       Made changes to PRACH Selection. 3GPP: 8.5.17.
10/30/10   ysh      IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/29/10   bj       Made changes to Open loop power control.Delete FDD IE "Primary 
                    CPICH Tx power","Constant value" and "UL interference". 
                    Add TDD IE "Primary CCPCH Tx Power".3GPP:8.5.7. 
10/28/10   prk      Fixed KW warnings. 
10/25/10   zwj      Added code to process SCCPCHinfolist and SCCPCHinfolist-LCR-R4 for TDD(3GPP 10.3.6.71).
10/24/10   yzh      According to 3GPP25.331v716 8.6.5.12a, delete IE  "Additional RACH TFCS for CCCH" (FDD only)(3.2.11).
10/25/10   ysh      FDD only IE"DPCH frame offset" delete  3GPP-8.6.6.14(3.2.4).
10/22/10   ysh      When Physical channel reconfiguration, compressed mode runtime error, reconfig failure,
                                 this function is FDD only, should delete. 3GPP-8.2.11.2(3.2.7)

===========================================================================*/


/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "err.h"
#include "tdsl1rrcif.h"
#include "tdsl1mtask.h"
#include "tdsmathutil.h"
#include "tdsmacrrcif.h"
#include "msg.h"
#include "tdsrlcrrcif.h"
#include "tdsrrcasn1.h"
#include "tdsrrccommon.h"
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcccm.h"
#include "tdsuecomdef.h"
#include "tdsrrccsp.h"
#include "tdsrrccspi.h"
#include "tdsrrcsmc.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcrce.h"
#include "tdsrrcmeas.h"
#include "ran.h"
#include "secapi.h"
#include "tdsrrcnv.h"

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER) 
#include "tdsrrcdata.h"
#endif

/*===================================================================
                        DATA DECLARATIONS
=====================================================================*/

/*===================================================================
                        CONSTANTS
=====================================================================*/

#ifdef FEATURE_TDSCDMA_MIMO
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
int8 tdsrrc_mimo_s_cpich_po = 0;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/
#endif /*FEATURE_TDSCDMA_MIMO*/


/* Valid additional channels for 1900 MHz band */
/* NOTE: These arrays must be zero-terminated. */
tdsrrc_UARFCN tdsrrc_ul_band_b_addl_channels[] = { 12, 37, 62, 87, 112, 137,
  162, 187, 212, 237, 262, 287, 0 };
tdsrrc_UARFCN tdsrrc_dl_band_b_addl_channels[] = { 412, 437, 462, 487, 512,
  537, 562, 587, 612, 637, 662, 687, 0 };

/* Valid additional channels for 850 MHz band */
/* NOTE: These arrays must be zero-terminated. */
tdsrrc_UARFCN tdsrrc_ul_band_c_addl_channels[] = { 782, 787, 807,
812, 837, 862, 0 };
tdsrrc_UARFCN tdsrrc_dl_band_c_addl_channels[] = { 1007, 1012, 1032,
1037, 1062, 1087, 0 };

/* Valid additional channels for 800 MHz band */
/* NOTE: These arrays must be zero-terminated. */
tdsrrc_UARFCN tdsrrc_ul_band_d_addl_channels[] = { 812, 837, 0 };
tdsrrc_UARFCN tdsrrc_dl_band_d_addl_channels[] = { 1037, 1062, 0 };

/* Valid additional channels for 1700_2100 AWS MHz band (band IV) */
/* NOTE: These arrays must be zero-terminated. */
/*lint -esym(714,tdsrrc_ul_band_h_addl_channels) */
/*lint -esym(714,tdsrrc_dl_band_h_addl_channels) */
tdsrrc_UARFCN tdsrrc_ul_band_h_addl_channels[] = { 1662, 1687, 1712, 1737, 1762, 
  1787, 1812, 1837, 1862, 0 };
tdsrrc_UARFCN tdsrrc_dl_band_h_addl_channels[] = { 1887, 1912, 1937, 1962, 1987,
  2012, 2037, 2062, 2087, 0 };

extern uint32 tdscurr_sccpch_index;

#ifdef FEATURE_TDSCDMA_HSUPA
uint32 TDSDEBUG_EUL_SI_ONLY_POWER_OFFSET = 0;
uint32 TDSRRCEUL_SET_NO_GRANT_DEFAULT_PERIODICITY = TRUE;
uint32 TDSRRCEUL_NO_GRANT_PERIODICITY_DEFAULT_VALUE = 500;
boolean TDSDEBUG_EUL_RRC_VALIDATE_STEP_INDEX_THRESHOLD = TRUE;

#endif

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
/* LTDTBD: TD-LTE might need different value range */
/*Definitions of FDL offset in 100KHz, used for deriving frequency from ARFCN*/
#define TDSRRC_DEFAULT_FDL_OFFSET 0
#define TDSRRC_FDL_OFFSET_BAND2_ADDL 18501
#define TDSRRC_FDL_OFFSET_BAND3 15750
#define TDSRRC_FDL_OFFSET_BAND4 18050
#define TDSRRC_FDL_OFFSET_BAND4_ADDL 17351
#define TDSRRC_FDL_OFFSET_BAND5_BAND6_ADDL 6701
#define TDSRRC_FDL_OFFSET_BAND8 3400
#define TDSRRC_FDL_OFFSET_BAND11 7360
#define TDSRRC_FDL_OFFSET_BAND19 7350
#define TDSRRC_FDL_OFFSET_BAND19_ADDL 7201
#endif



tdsprach_config_type   tdsprach_working_db;


tdsrrcllc_prach_sel_info_type   *  tdsprach_sel_info[UE_MAX_PRACH];

/*--------------------------------------------------------------------
tdsprach_5ms_tti_index_array will store the 5ms tti PRACH's indices
tdsprach_10ms_tti_index_array will store the 10ms tti PRACH's indices
tdsprach_20ms_tti_index_array will store the 20ms tti PRACH's indices
This will aid in Random selection of PRACH once the TTI is selected.
-------------------------------------------------------------------*/
uint8               tdsprach_5ms_tti_index_array[UE_MAX_PRACH];
uint8               tdsprach_10ms_tti_index_array[UE_MAX_PRACH];
uint8               tdsprach_20ms_tti_index_array[UE_MAX_PRACH];

typedef struct
{
  uint8                   num_valid_prachs_found;
/*----------------------------------------------------------------
  current_5ms_tti_prach_cnt will advance beginning from 0.
  ----------------------------------------------------------------*/
  uint8               current_5ms_tti_prach_cnt;
/*----------------------------------------------------------------
  current_10ms_tti_prach_cnt will advance beginning from 0.
  ----------------------------------------------------------------*/
  uint8               current_10ms_tti_prach_cnt;
/*----------------------------------------------------------------
  current_10ms_tti_prach_cnt will advance beginning from 0.
  ----------------------------------------------------------------*/
  uint8               current_20ms_tti_prach_cnt;

}tdsprach_sys_info_list_status_type;

tdsprach_sys_info_list_status_type       tdsprach_status_info;


/*===================================================================
                        STATIC VARIABLES
=====================================================================*/

/*===================================================================
                        FORWARD DECLARATIONS
=====================================================================*/

/*===================================================================
*                       FUNCTION PROTOTYPES
====================================================================*/

/*===================================================================
*                           FUNCTIONS
====================================================================*/
/*============================================================================
FUNCTION: tdsrrcllcpcie_update_uldpch_mode_specific_info()

DESCRIPTION:
 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_update_uldpch_mode_specific_info
(
 tdsrrc_UL_DPCH_Info_r6_tdd * ul_dpch_info
)
{

  /* Get the UL Timing Advance */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ul_dpch_info, ul_TimingAdvancePresent))
  {
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_LOW,"This OTA MSG include ul_TimingAdvanceControl");
    if (FAILURE == tdsrrcllcpcie_get_ul_tim_adv_contrl_r4(&ul_dpch_info->ul_TimingAdvance))
    {
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"This OTA MSG not include ul_TimingAdvanceControl");
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = FALSE;
  }
  
  /* Get Uplink CCTrCH LIST from OTA message */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ul_dpch_info, ul_CCTrCHListPresent))
  {
    if (1 < ul_dpch_info->ul_CCTrCHList.n)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ul_CCTrCHList %d is invalid",(ul_dpch_info->ul_CCTrCHList.n));
      return FAILURE;
    }
  
    if (FAILURE == tdsrrcllcpcie_get_ul_cctrch_list_r4(&ul_dpch_info->ul_CCTrCHList))
    {
      return FAILURE;
    }
  }
  /** TDD support only one CCTrCH. If OTA message send ul_CCTrCHListToRemove,
         RRC consider it invalid */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ul_dpch_info, ul_CCTrCHListToRemovePresent))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDD only have one CCTrCH, ul_CCTrCHListToRemove %d is invalid",(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ul_dpch_info,ul_CCTrCHListToRemovePresent)));
    return FAILURE;
  }


    return SUCCESS;
  }


/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_tim_adv_contrl_r7()

DESCRIPTION   This function translates the Uplink Timing Advance Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.96 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_tim_adv_contrl_r7
(
  tdsrrc_UL_TimingAdvanceControl_r7 *ie_ptr
)
{
  tdsrrc_UL_TimingAdvanceControl_r7_tdd128  *tmp_ie_ptr;

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr, rrc_UL_TimingAdvanceControl_r7_enabled))
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->u.enabled->tddOption,
                       rrc_UL_TimingAdvanceControl_r7_tddOption_tdd128))
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_enabled = TRUE;
      tmp_ie_ptr = ie_ptr->u.enabled->tddOption.u.tdd128;

      /* Get ul_SynchronisationParameters */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ie_ptr, ul_SynchronisationParameters))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 
                                          tmp_ie_ptr->ul_SynchronisationParameters.frequency;
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 
                                          tmp_ie_ptr->ul_SynchronisationParameters.stepSize;
      }
      else
      {
        /* from the 10.3.6.96:
        Default: Uplink synchronisation step size is 1. Uplink synchronisation frequency is 1. */
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 1;
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 1;
      }

      /* Get synchronisationParameters */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ie_ptr, synchronisationParameters))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = TRUE;

        /* Get SYNC_UL codes bitmap */
        /* translate sync_UL_CodesBitmap string to uint8(refer to tdsrrc_translate_ura_id) */
        if (8 != tmp_ie_ptr->synchronisationParameters.sync_UL_CodesBitmap.numbits)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received sync_UL_CodesBitmap Length: %d is invalid",tmp_ie_ptr->synchronisationParameters.sync_UL_CodesBitmap.numbits);
        }
        /* Mask of SYNC_UL codes. Each bit indicates availability of a SYNC_UL code,
         * where the SYNC_UL codes are numbered "code0" to "code7". The value 1 of a 
         * bit indicates that the corresponding SYNC_UL code can be used */

        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_sync_code_mask = 
                         tmp_ie_ptr->synchronisationParameters.sync_UL_CodesBitmap.data[0];

        /* Get FPACH info */
        tdsrrcllcpcie_get_fpach_info_r4(&tmp_ie_ptr->synchronisationParameters.fpach_Info);

        /* Get prxUpPCHdes(10.3.6.96) info */
        /* -- Actual value prxUpPCHdes = IE value - 120 */
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des = 
                             tmp_ie_ptr->synchronisationParameters.prxUpPCHdes - 120;

        /* Get SYNC_UL procedure info */
        if (TDSRRC_MSG_COMMON_BITMASK_IE(tmp_ie_ptr->synchronisationParameters, 
                                           tdsrrc_SynchronisationParameters_r7, sync_UL_Procedure))
        {
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx =
              tdsMaxSynUlTrans[tmp_ie_ptr->synchronisationParameters.sync_UL_Procedure.max_SYNC_UL_Transmissions];
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step =
              tmp_ie_ptr->synchronisationParameters.sync_UL_Procedure.powerRampStep;
        }
        else
        {
        /** from 10.3.6.96:
                 Default is:Max SYNC_UL Transmission is 2. Power Ramp Step is 2.*/
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx = 2;
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step = 2;
        }
#ifdef FEATURE_TDSCDMA_HSUPA
        /* In 25.331,10.3.6.78d, IE 'PRXUpPCHdes', 'Power Ramp Step' and 'Max SYNC_UL Transmissions' are CV type,
                  HSUPA scheduled E-RUCCH information will be affected if one of them changed*/
        if ((TRUE == tdscurrent_config_ptr->e_dch_transmission) && 
             (tdscurrent_config_ptr->hsupa_start_mask & TDSRRC_SCHED_START))
        {
          if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des != 
               tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.prx_upPch_des))
          {
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.prx_upPch_des =
               tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des;
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;           
            tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);    
          }
          if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx != 
               tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.max_sync_ul_tx))
          {
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.max_sync_ul_tx =
               tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx;
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;            
            tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);    
          }
          if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step != 
               tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.pwr_ramp_step))
          {
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.pwr_ramp_step =
               tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step;
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;            
            tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);    
          }
        }
#endif
      }
      else
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = FALSE;
      }
    }
    else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->u.enabled->tddOption,
                       rrc_UL_TimingAdvanceControl_r7_tddOption_tdd384))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd384,not tdd128");
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd768,not tdd128");
      return FAILURE;
    }
  }
  else
  {
    /* 8.6.6.26 : 1> if the IE "Uplink Timing Advance Control" has the value "disabled":
         2> reset timing advance to 0; */
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_enabled = FALSE;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 0;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 0;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = FALSE;
    memset(&(tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms), 0,
        sizeof(tdsl1_ul_sync_parms_struct_type));
  }

  return SUCCESS;
}
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r7()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r7
(
  tdsrrc_UL_TS_ChannelisationCodeList_r7 *src_ptr,
  tdsl1_ul_dpch_ts_struct_type *dst_cfg
)
{
  uint8    i;

  dst_cfg->num_chan_codes = src_ptr->n;
  for (i = 0; i < src_ptr->n; i++)
  {
    switch (src_ptr->elem[i].ul_TS_Channelisation_Code)
    {
      case  tdsrrc_UL_TS_ChannelisationCode_cc1_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_1;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc2_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_2;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc2_2:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_2;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_2;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc4_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_4;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc4_2:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_4;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_2;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc4_3:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_4;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_3;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc4_4:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_4;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_4;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_2:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_2;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_3:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_3;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_4:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_4;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_5:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_5;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_6:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_6;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_7:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_7;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_8:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_8;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_2:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_2;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_3:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_3;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_4:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_4;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_5:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_5;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_6:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_6;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_7:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_7;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_8:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_8;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_9:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_9;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_10:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_10;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_11:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_11;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_12:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_12;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_13:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_13;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_14:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_14;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_15:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_15;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_16:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_16;
          continue;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_timeslot_and_code choice is out of range");
        continue;
    }
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_r7()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.94a in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_r7
(
  tdsrrc_UL_CCTrCH_r7_tdd128 *ie_ptr,
  tdsl1_ul_dpch_cfg_struct_type *dpch_cfg
)
{
  uint8      i, j;
  tdsrrc_UplinkTimeslotsCodes_LCR_r7_additionalTimeslots    *tmp_ie_ptr;
  tdsrrc_UplinkTimeslotsCodes_LCR_r7_timeslotList    *ts_list_short_ptr;
  tdsrrc_UplinkAdditionalTimeslots_LCR_r7_newParameters    *new_para_list_ptr;
  tdsl1_ul_dpch_ts_struct_type      ts_list_temp;

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
           tdsrrc_UL_CCTrCH_r7_tdd128, ul_CCTrCH_TimeslotsCodes))
  {
    dpch_cfg->dynamicSfUse = ie_ptr->ul_CCTrCH_TimeslotsCodes.dynamicSFusage;

    /* if OTA message has ul_CCTrCH_TimeslotsCode, num of timeslot be set. */
    dpch_cfg->num_ts = 0;

    /* Get First Individual timeslot info */
    if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4(&ie_ptr->
                  ul_CCTrCH_TimeslotsCodes.firstIndividualTimeslotInfo,
                  &dpch_cfg->ts_list[0]))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4 return error");
      return FAILURE;
    }

    if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r7(&ie_ptr->
                  ul_CCTrCH_TimeslotsCodes.ul_TS_ChannelisationCodeList,
                  &dpch_cfg->ts_list[0]))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r7 return error");
      return FAILURE;
    }
    /* we get first timeslot info, so num of timeslot add 1 */
    dpch_cfg->num_ts += 1;

    /* Get more timeslots info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots,
          rrc_UplinkTimeslotsCodes_LCR_r7_moreTimeslots_noMore))
    {
      /* no more means no data */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Receive ul_CCTrCH_TimeslotsCodes moreTimeslots noMore,no data");
    }
    else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots,
          rrc_UplinkTimeslotsCodes_LCR_r7_moreTimeslots_additionalTimeslots))
    {
      tmp_ie_ptr = ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots.u.additionalTimeslots;

      /* Get consecutive times slot info */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(tmp_ie_ptr,
                           rrc_UplinkTimeslotsCodes_LCR_r7_additionalTimeslots_consecutive))
      {
        dpch_cfg->num_ts += tmp_ie_ptr->u.consecutive->numAdditionalTimeslots;

        for (i = 1; i <= tmp_ie_ptr->u.consecutive->numAdditionalTimeslots; i++)
        {
          memscpy(
            &(dpch_cfg->ts_list[i]),sizeof(tdsl1_ul_dpch_ts_struct_type),
            &(dpch_cfg->ts_list[0]),
            sizeof(tdsl1_ul_dpch_ts_struct_type)
          );
          /* consecutive means continuous, so from first times slot,
             the following timeslots one by one be add 1. */
          /* ts_num is 1~5, first ts_num does not start from 0. So max UL ts_num is 5, not 4. */
          dpch_cfg->ts_list[i].ts_num = ((dpch_cfg->ts_list[0].ts_num) + i) % 
                                   (TDSL1_UL_MAX_NUM_TS + 1);

          /* ts_num is 1~5 in UL */
          if ((0 == dpch_cfg->ts_list[i].ts_num) || (6 == dpch_cfg->ts_list[i].ts_num))
          {
            ERR_FATAL("ts_num is %d, UL ts_num is 1~5",dpch_cfg->ts_list[i].ts_num,0,0);
            return FAILURE;
          }
        }
      }
      /* Get timeslot list info */
      else if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(tmp_ie_ptr, rrc_UplinkTimeslotsCodes_LCR_r7_additionalTimeslots_timeslotList))
      {
        ts_list_short_ptr = tmp_ie_ptr->u.timeslotList;
        dpch_cfg->num_ts += ts_list_short_ptr->n;
        for (i = 0; i < ts_list_short_ptr->n; i++)
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE(ts_list_short_ptr->elem[i].parameters,
                rrc_UplinkAdditionalTimeslots_LCR_r7_parameters_sameAsLast))
          {
          /* same as last means (i+1) same as (i). */
            memscpy(
                     &(dpch_cfg->ts_list[i + 1]),sizeof(tdsl1_ul_dpch_ts_struct_type),
                     &(dpch_cfg->ts_list[i]),
                     sizeof(tdsl1_ul_dpch_ts_struct_type)
            );
            /* ts_num is 1~5 in UL. But ASN1 value of timeslot number is 0~14 */
            if ((1 > ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber) ||
                (5 < ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber))
            {
              TDSRRC_MSG1(MSG_LEGACY_LOW,"UL AdditionalTimeslot number is %d,But UL timeslot range is 1~5",dpch_cfg->ts_list[i].ts_num);
              ERR_FATAL("ts_num is %d, UL ts_num is 1~5",ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber,0,0);
              return FAILURE;
            }
            dpch_cfg->ts_list[i + 1].ts_num =
              ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber;
          }
          else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ts_list_short_ptr->elem[i].parameters,
                rrc_UplinkAdditionalTimeslots_LCR_r7_parameters_newParameters))
          {
          /* new para means we get new values from second timeslot. */
            new_para_list_ptr = ts_list_short_ptr->elem[i].parameters.u.newParameters;
            if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4(
                             &(new_para_list_ptr->individualTimeslotInfo),
                             &(dpch_cfg->ts_list[i + 1])))
            {
              return FAILURE;
            }
            if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r7(
                             &(new_para_list_ptr->ul_TS_ChannelisationCodeList),
                             &(dpch_cfg->ts_list[i + 1])))
            {
              return FAILURE;
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"parameters choice is invalid");
            return FAILURE;
          }
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_CCTrCH_TimeslotsCodes.moreTimeslots.u.additionalTimeslots choice is invalid");
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_CCTrCH_TimeslotsCodes.moreTimeslots choice is invalid");
      return FAILURE;
    }
    for (i = 1; i < dpch_cfg->num_ts; i++)
    {
      for (j = 0; j < dpch_cfg->num_ts - i; j++)
      {
        if (dpch_cfg->ts_list[j].ts_num > dpch_cfg->ts_list[j + 1].ts_num)
        {
          ts_list_temp = dpch_cfg->ts_list[j];
          dpch_cfg->ts_list[j] = dpch_cfg->ts_list[j + 1];
          dpch_cfg->ts_list[j + 1]= ts_list_temp;
        }
      }
    }
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_cctrch_list_r7()

DESCRIPTION   This function translates the Uplink CCTrCH List
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.88 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_ul_cctrch_list_r7
(
  tdsrrc_UL_CCTrCHList_r7 *ie_ptr
)
{
  /* Get ul-TargetSIR info */
  /* -- The IE ul-TargetSIR corresponds to PRX-DPCHdes for 1.28Mcps TDD
-- Actual value PRX-DPCHdes = (value of IE "ul-TargetSIR" - 120) */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.prx_dpch_des= ie_ptr->elem[0].ul_TargetSIR - 120;

  /* check If no "activation time" IE  for TDM, we can reject */
  if((ie_ptr->elem[0].timeInfo.m.activationTimePresent == 0)&&
       ((ie_ptr->elem[0].m.commonTimeslotInfoPresent == 1)&&
        (ie_ptr->elem[0].commonTimeslotInfo.m.repetitionPeriodAndLengthPresent == 1)&&
        (ie_ptr->elem[0].commonTimeslotInfo.repetitionPeriodAndLength.t 
                          != T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod1)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"no activation time for TDM,reject");
    return FAILURE;
  }

  /* Get Active time info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[0].timeInfo, tdsrrc_TimeInfo, activationTime))
  {
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.act_time =
                                   ie_ptr->elem[0].timeInfo.activationTime;
  }
  else
  {
  /** from 10.3.6.83:
         Frame number start of the physical channel existence. Default value is "Now" */
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.act_time =
                                   TDSRRCLLC_ACTIVATION_TIME_NOW;
  }

  /* Get common timing slot info(10.3.6.10) */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[0], tdsrrc_UL_CCTrCH_r7,
                                      commonTimeslotInfo))
  {
    if(FAILURE == tdsrrcllcpcie_get_com_ts_info_r4(&ie_ptr->elem[0].commonTimeslotInfo,
              &tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.common_ts_info, FALSE))
    {
      return FAILURE;
    }
  }

  /* Get Uplink DPCH timeslots and codes LCR(10.3.6.94a) */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->elem[0].tddOption, rrc_UL_CCTrCH_r7_tddOption_tdd128))
  {
    if(FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_r7(ie_ptr->elem[0].tddOption.u.tdd128,
            &tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg))
    {
      return FAILURE;
    }
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->elem[0].tddOption, rrc_UL_CCTrCH_r7_tddOption_tdd768))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd768,not tdd128");
    return FAILURE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd384,not tdd128");
    return FAILURE;
  }

  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_update_uldpch_mode_specific_info_r7()

DESCRIPTION:
 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
=============================================================================*/

static uecomdef_status_e_type tdsrrcllcpcie_update_uldpch_mode_specific_info_r7
(
 struct tdsrrc_UL_DPCH_Info_r7_tdd * ul_dpch_info
)
{

  /* Get the UL Timing Advance */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ul_dpch_info, ul_TimingAdvancePresent))
  {
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_LOW,"This OTA MSG include ul_TimingAdvanceControl");
    if (FAILURE == tdsrrcllcpcie_get_ul_tim_adv_contrl_r7(&ul_dpch_info->ul_TimingAdvance))
    {
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"This OTA MSG not include ul_TimingAdvanceControl");
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = FALSE;
  }
  /* Get Uplink CCTrCH LIST from OTA message */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ul_dpch_info, ul_CCTrCHListPresent))
  {
    if (1 < ul_dpch_info->ul_CCTrCHList.n)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ul_CCTrCHList %d is invalid",(ul_dpch_info->ul_CCTrCHList.n));
      return FAILURE;
    }

    if (FAILURE == tdsrrcllcpcie_get_ul_cctrch_list_r7(&ul_dpch_info->ul_CCTrCHList))
    {
      return FAILURE;
    }
  }

/* TDD support only one CCTrCH. If OTA message send ul_CCTrCHListToRemove, RRC consider it invalid */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ul_dpch_info, ul_CCTrCHListToRemovePresent))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDD only have one CCTrCH, ul_CCTrCHListToRemove %d is invalid",(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ul_dpch_info,ul_CCTrCHListToRemovePresent)));
    return FAILURE;
  }


  return SUCCESS;
}

#ifdef FEATURE_TDSCDMA_HSUPA

/*============================================================================
FUNCTION: tdsrrcllcpcie_clean_l1_e_info_on_non_r7_hho()

DESCRIPTION:
  This function cleans up any stored L1 EUL related info when non-R7 HHO msg
  is received.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
static void tdsrrcllcpcie_clean_l1_e_info_on_non_r7_hho( void )
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: Cleaning up EUL info due to non-r7 HHO msg");

  tdsrrcllc_init_l1_e_dl_info(tdsordered_config_ptr);

}

/*============================================================================
FUNCTION: tdsrrcllcpcie_update_e_hich_info_and_mask()

DESCRIPTION:
  This function updates e-HICH information in a given rl index of e-dch active set
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_update_e_hich_info_and_mask
(
  tdsrrc_E_HICH_Information_TDD128 *e_hich_info_ptr
)
{
  uint8         temp_i;
  tdsl1_e_dl_per_rl_info_struct_type *e_dl_info_ptr = 
       &tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info;

  /* If we are inside this funtion it indicates that E-HICH information is
     provided by the NW */
  /* Notify L1 the IE present in this OTA message */
  tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_channel_mask |= TDSL1_E_HICH_INFO_INCL;
  TDSRRC_MSG0(MSG_LEGACY_MED,"Begin save E_HICH. set e_dl_channel_mask |= TDSL1_E_HICH_INFO_INCL");
  /* e_dl_info_ptr->e_dl_channel_mask |= TDSRRCLLC_E_HICH_INFO_PRESENT; */

  /* Min number of slot between last E-DCH TTI and E-HICH carrying ACK/NACK */
  e_dl_info_ptr->e_hich_info.n_e_hich = e_hich_info_ptr->n_E_HICH;

  /* In 25.331 10.3.6.41c, non-scheduled transmission grant info, 
      N_EHICH is “CV-Scheduled E-PUCH” type, which means, 
      if scheduled transmission is configured, this IE could be absent,
      and both scheduled and non-scheduled transmission uses the same N_EICH value */
  /* Scheduled and non-scheduled transmission configuration are independent in L1, so RRC should
        deal this situation if NW only reconfig N-E-HICH in scheduled E-PUCH information */
  if ((TRUE == tdscurrent_config_ptr->e_dch_transmission) && 
       (tdscurrent_config_ptr->hsupa_start_mask & TDSRRC_NON_SCHED_START))
  {
    if (e_hich_info_ptr->n_E_HICH != tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich)
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich = e_hich_info_ptr->n_E_HICH;
      tdsordered_config_ptr->l1_e_req_mask |= TDSRRCLLC_NON_SCHED_GRANT_START;
    }
  }

  /* Total number of EHICH channels(not more than 4) */
  e_dl_info_ptr->e_hich_info.num_ehich = 
           e_hich_info_ptr->e_HICH_InfoList.n;

  for (temp_i = 0; temp_i < e_hich_info_ptr->e_HICH_InfoList.n; temp_i++)
  {
    /* E-HICH index */
    e_dl_info_ptr->e_hich_info.sched_e_hich_slot_info[temp_i]
       .ei = e_hich_info_ptr->e_HICH_InfoList.elem[temp_i].ei;

    /* Time slot number for E-HICH */
    e_dl_info_ptr->e_hich_info.sched_e_hich_slot_info[temp_i]
       .ts_num = e_hich_info_ptr->e_HICH_InfoList.elem[temp_i].timeslotNumber;

    /* Channelization code for E-HICH */
    e_dl_info_ptr->e_hich_info.sched_e_hich_slot_info[temp_i]
       .chan_code = e_hich_info_ptr->e_HICH_InfoList.elem[temp_i].channelisation_Code;

    if (T_tdsrrc_E_HICH_Information_LCR_midambleAllocationMode_ueSpecificMidamble == 
          e_hich_info_ptr->e_HICH_InfoList.elem[temp_i].midambleAllocationMode.t)
    {
      e_dl_info_ptr->e_hich_info.sched_e_hich_slot_info[temp_i]
         .ehich_midamble.allocation_mode = TDSL1_UE_SPECIFIC_MIDAMBLE;
      e_dl_info_ptr->e_hich_info.sched_e_hich_slot_info[temp_i]
         .ehich_midamble.shift = e_hich_info_ptr->e_HICH_InfoList.elem[temp_i]
         .midambleAllocationMode.u.ueSpecificMidamble;
    }
    else
    {
      e_dl_info_ptr->e_hich_info.sched_e_hich_slot_info[temp_i]
         .ehich_midamble.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
    }
    e_dl_info_ptr->e_hich_info.sched_e_hich_slot_info[temp_i]
       .ehich_midamble.config = tdsMidambleConfiguration[e_hich_info_ptr->e_HICH_InfoList.elem[temp_i]
       .midambleConfiguration];
  }
  TDSRRC_MSG0(MSG_LEGACY_MED,"E_HICH save finish.");

  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_hich_info()

DESCRIPTION:
  This function processes e-HICH information and updates the information
  in a given e_rl_index
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_process_e_hich_info
(
  uint32 e_hich_info_present,
  tdsrrc_E_HICH_Information_TDD128 *e_hich_info_ptr
)
{
  if (e_hich_info_present)
  {
    if (e_hich_info_ptr != NULL )
    {
      if (FAILURE == tdsrrcllcpcie_update_e_hich_info_and_mask(e_hich_info_ptr))
      {
        return FAILURE;
      }
    }
  }

  return(SUCCESS);
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r6()

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r6
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r6 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* In R6, e_hich and e_agch is FDD, should not save */

  return(SUCCESS);
}

#ifdef FEATURE_TDSCDMA_REL8
#if 0
/*============================================================================
FUNCTION: tdsrrcllcpcie_convert_asn1_minreduced_e_dpdch_gainfactor()

DESCRIPTION:
  This function converts the ENUM type of minReduced_E_DPDCH_GainFactor 
  received in reconfig message to actual internally defined enumration.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  minReduced_E_DPDCH_GainFactor
  
SIDE EFFECTS:
=============================================================================*/
tdsl1_e_min_bed_enum_type tdsrrcllcpcie_convert_asn1_minreduced_e_dpdch_gainfactor
(
  tdsrrc_MinReduced_E_DPDCH_GainFactor minReduced_E_DPDCH_GainFactor
)
{
  /*Set it to the default value 8/15 */
  tdsl1_e_min_bed_enum_type converted_e_min_bed = TDSL1_E_MIN_BED_08;

  switch(minReduced_E_DPDCH_GainFactor)
  {
    case tdsrrc_MinReduced_E_DPDCH_GainFactor_m8_15:
      converted_e_min_bed = TDSL1_E_MIN_BED_08;
      break;
      
    case tdsrrc_MinReduced_E_DPDCH_GainFactor_m11_15:
      converted_e_min_bed = TDSL1_E_MIN_BED_11;
      break;
      
    case tdsrrc_MinReduced_E_DPDCH_GainFactor_m15_15:
      converted_e_min_bed = TDSL1_E_MIN_BED_15;
      break;

    case tdsrrc_MinReduced_E_DPDCH_GainFactor_m21_15:
      converted_e_min_bed = TDSL1_E_MIN_BED_21;
      break;

    case tdsrrc_MinReduced_E_DPDCH_GainFactor_m30_15:
      converted_e_min_bed = TDSL1_E_MIN_BED_30;
      break;

    case tdsrrc_MinReduced_E_DPDCH_GainFactor_m42_15:
      converted_e_min_bed = TDSL1_E_MIN_BED_42;
      break;

    case tdsrrc_MinReduced_E_DPDCH_GainFactor_m60_15:
      converted_e_min_bed = TDSL1_E_MIN_BED_60;
      break;

    case tdsrrc_MinReduced_E_DPDCH_GainFactor_m84_15:
      converted_e_min_bed = TDSL1_E_MIN_BED_84;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"minReduced_E_DPDCH_GainFactor out of range: %d",minReduced_E_DPDCH_GainFactor);
      converted_e_min_bed = TDSL1_E_MIN_BED_08;
      break;
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:minReduced_E_DPDCH_GainFactor: %d",converted_e_min_bed);

  return(converted_e_min_bed);
}
#endif
#endif

/*===========================================================================

FUNCTION          tdsrrcllcpcie_convert_asn1_oss_bitstring_to_uint8

DESCRIPTION       
  This function translates oss-bitstring to uint8 and retruned the computed uint8 value. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint8 tdsrrcllcpcie_convert_asn1_oss_bitstring_to_uint8
(
  ASN1BitStr32  *bitstring_ptr
)
{
  if (bitstring_ptr->numbits != 8)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received BITSTRING Length: %d is invalid(expected 8)",bitstring_ptr->numbits);
  }

  return((uint8) *(bitstring_ptr->data));
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_e_dch_information()

DESCRIPTION:
  This function updates IE "ul e dch information" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_e_dch_information
(
  uint32 e_dch_info_present,
  tdsrrc_UL_EDCH_Information_r6 *e_dch_info_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Look into this IE only if next UE state is DCH */
  if (e_dch_info_present && 
      (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
     )
  {
    if ((e_dch_info_ptr->m.mac_es_e_resetIndicatorPresent) &&
        (e_dch_info_ptr->mac_es_e_resetIndicator == tdsrrc_UL_EDCH_Information_r6_mac_es_e_resetIndicator_true_))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"mac_es_e_resetIndicator set to TRUE");
      tdsordered_config_ptr->mac_e_reset_indicator = TRUE;
    }
    else
    {
      tdsordered_config_ptr->mac_e_reset_indicator = FALSE;
    }
  }

  return(SUCCESS);
}

#endif /* FEATURE_TDSCDMA_HSUPA*/

/*====================================================================
FUNCTION       tdsrrc_get_mac_d_hfn_cucnf()

DESCRIPTION    This function takes a pointer to the cu_confirm_ptr for R5
                         and extract mac_d_hfn field.

DEPENDENCIES   None

RETURN VALUE   value of mac_d_hfn if present else, invalid value

====================================================================*/
uint32 tdsrrc_get_mac_d_hfn_cucnf(tdsrrc_CellUpdateConfirm_r5_IEs *cu_confirm_ptr)
{

  uint32 mac_d_hfn = TDSRRCSMC_INVALID_HFN_VALUE;
  uecomdef_status_e_type loc_ret_val=SUCCESS;
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(cu_confirm_ptr,
              dl_CommonInformationPresent))
  {
    if(TDSRRC_CHECK_MSG_TYPE_IE(cu_confirm_ptr->dl_CommonInformation,
               dl_DPCH_InfoCommonPresent))
    {
      loc_ret_val = tdsrrcrb_get_mac_d_hfn(
                TDSRRC_CHECK_MSG_TYPE_IE(cu_confirm_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
                    mac_d_HFN_initial_valuePresent),
                &cu_confirm_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
                &mac_d_hfn);

      if(loc_ret_val == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
      }
    }
  }
  return (mac_d_hfn);
}

/*====================================================================
FUNCTION       tdsrrcllcpcie_get_prach_sys_info_list()

DESCRIPTION    This function takes a pointer to the PRACH system
               information list and copies all applicable IEs for
               MAC, RLC, and L1 into the TDSORDERED_CONFIG database
               (internal format).

DEPENDENCIES   None

RETURN VALUE   SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS   The current contents of TDSORDERED_CONFIG will be
               overwritten.
====================================================================*/

/*====================================================================
FUNCTION: RRCLLC_INIT_PRACH_IE_PTRS

DESCRIPTION:
  This function initializes fields of PRACH Config DB & adjusts the 
  embedded pointers.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllcpcie_init_prach_ie_ptrs
(
  uint8 rrc_num_of_prach
)
{
  uint32 i = 0;
  uint8  ix = 0;

  for(; i < rrc_num_of_prach; i++)
  {

    if(tdsprach_sel_info[i] == NULL)
    {
      ERR_FATAL("PRACH_OPT: Null Pointer for %d PRACH in tdsprach_sel_info",i,0,0);
    }

    memset((void *)tdsprach_sel_info[i], 0, sizeof(tdsrrcllc_prach_sel_info_type)); 

    for(; ix < TDSUE_MAX_TRCH; ix++)
    {
      /* L1 UL Semi static parameter data pointers */
      tdsprach_sel_info[i]->l1_ul_cctrch_parms.semi_static_parms_ptr[ix] =
          &tdsprach_sel_info[i]->semi_static_parms[ix];
    } 

    for(ix = 0; ix < TDSL1_UL_TFC_MAX; ix++)
    {
      /* Init the UL Gain Factors that are TFC specific */
      tdsprach_sel_info[i]->l1_ul_cctrch_parms.gain_factor_parms_ptr[ix] =
          &tdsprach_sel_info[i]->gain_factor_parms[ix];
    } /* end TFC initialization */
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initialized internal PRACH dB");
} /* tdsrrcllcpcie_init_prach_ie_ptrs */

/*====================================================================
FUNCTION: RRCLLC_UPDATE_OC_WITH_PRACH_INFO

DESCRIPTION:
  This function updates OC with PRACH data.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllcpcie_update_oc_with_prach_info
(
  tdsordered_config_type *config_ptr,
  tdsrrcllc_prach_sel_info_type *prach_sel_info_ptr
)
{
  uint8   ix;  
  uint8   trch_idx;
  uint8   num_of_asc = 1;
  uint8   asc_idx;
  uint32  pval = 0;
  /* Indicate to uplink that the current channel is a PRACH */
  config_ptr->l1_ul_cctrch_parms.chan = TDSL1_UL_PHYCHAN_PRACH;

  /* Though some IE contents were extracted
     for L1 PRACH TTI selection earlier, all the 
     IEs are again extracted to OC for convenience */

  if(prach_sel_info_ptr->persistence_scaling_factor_list == NULL)
  {
    (void)tdsrrcllcpcie_get_persist_scaling_default_factors(config_ptr);
  }
#ifdef FEATURE_TDSCDMA_HSUPA
  config_ptr->sync_code_erucch_770ext_indicator = FALSE;
  /* get e-RUCCH-Sync-UL-Codes-Bitmap */
  if (prach_sel_info_ptr->prach_RACH_Info_LCR != NULL)
  {
  /* Bitmap of SYNC_UL codes. Each bit indicates availability of a SYNC_UL code,
     * where the SYNC_UL codes are numbered "code0" to "code7". The value 1 of a 
     * bit indicates that the corresponding SYNC_UL code can be used, vice versa.
     * See 25.331 v9.1.0, 10.3.6.52 and 10.3.6.78a 
     */
    if (prach_sel_info_ptr->prach_RACH_Info_LCR->e_RUCCH_Sync_UL_Codes_Bitmap.numbits == 8)
    {
      config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.ul_sync_code_mask = 
        prach_sel_info_ptr->prach_RACH_Info_LCR->e_RUCCH_Sync_UL_Codes_Bitmap.data[0];
      
      config_ptr->sync_code_erucch_770ext_indicator = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"sync_UL_Codes_Bitmap Bitstring number is %d, not 8(prach rach info),Error",prach_sel_info_ptr->prach_RACH_Info_LCR->e_RUCCH_Sync_UL_Codes_Bitmap.numbits);
    }
  }
#endif /*FEATURE_TDSCDMA_HSUPA*/

  if(prach_sel_info_ptr->ac_to_asc_mapping_table != NULL && 
     prach_sel_info_ptr->rach_info != NULL &&
     prach_sel_info_ptr->rach_tfs != NULL &&  
     prach_sel_info_ptr->prach_partitioning != NULL)  
  {
    (void)tdsrrcllcpcie_get_prach_rach_info(&config_ptr->l1_ul_chan_parms.prach_cfg, prach_sel_info_ptr->rach_info);
    trch_idx = tdsrrcllc_get_ul_rach_trch_idx(config_ptr, (tr_ch_id_type)(prach_sel_info_ptr->transport_channel_id));
    if(trch_idx < TDSUE_MAX_TRCH)
    {
      (void)tdsrrcllc_get_ul_transport_format_set(config_ptr, NULL, prach_sel_info_ptr->rach_tfs, trch_idx);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR," Invalid Trch index for Trch id %d",prach_sel_info_ptr->transport_channel_id);
    }
    /* set default value according to MAC */
    config_ptr->ul_tfcs.ntfc = 1;
    config_ptr->ul_tfcs.tfc_info[0].control_flag = TDSTFC_ALLOWED;
    config_ptr->ul_tfcs.tfc_info[0].ctfc = 0;
    config_ptr->ul_tfcs.tfc_info[0].tfi[0] = 0; 

    (void)tdsrrcllcpcie_get_prach_partitioning(config_ptr->asc_info, &config_ptr->l1_ul_chan_parms.prach_cfg, 
        prach_sel_info_ptr->prach_partitioning, &num_of_asc);

    if(prach_sel_info_ptr->persistence_scaling_factor_list != NULL)
    {
      (void)tdsrrcllcpcie_get_persist_scaling_factors(config_ptr, prach_sel_info_ptr->persistence_scaling_factor_list);
    }

    (void)tdsrrcllcpcie_get_ac_to_asc_mapping(config_ptr, prach_sel_info_ptr->ac_to_asc_mapping_table); 
    
    /* Give L1 the number of ASCs rcv'd over the air */
    config_ptr->l1_ul_chan_parms.prach_cfg.asc_cfg.num_asc = (uint8)num_of_asc;

    /* Setup the Persistence values for all ASCs */
    for(asc_idx = 0; ((asc_idx < num_of_asc)&& (asc_idx < MAX_ASC)); asc_idx++)
    {
      switch(asc_idx)
      {
        case 0:
//          pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR *
//            TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
//          break;
          pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR;
          break;
        case 1:
//          pval = (TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>
//                  (config_ptr->dpl - 1)) *
//                  TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
//          break;
          pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR>>(config_ptr->dpl - 1);
          break;
        default:
//          if((asc_idx - 2) >=0)
//          {
//            pval = (TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>
//                    (config_ptr->dpl - 1)) *
//                   config_ptr->psf[asc_idx - 2]; /*lint !e676 */
//                   /* lint mistakenly believes there is a possibility for a
//                      negative subscript here. */
//          }
//          break;
          /*psf from spec goes from 0.9..0.2, we have scaled it to 9...2(factor of 10), this is why we 
              divide by RRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL*/
          pval = ((TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>(config_ptr->dpl - 1)) *
                  config_ptr->psf[asc_idx - 2])/
                  TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL; 
          break;
      }
      config_ptr->mac_ul_parms.rach_tx_cntl_info.asc[asc_idx].pval = pval;
//      {
//        config_ptr->mac_ul_parms.rach_tx_cntl_info.asc[asc_idx].pval =
//        pval / (((TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR * 10) / 0xffff) + 1);
//      }
    }

    /* Indicate the number of Access Service Classes supported */
    config_ptr->mac_ul_parms.rach_tx_cntl_info.nASC = num_of_asc;

    (void)tdsrrcllcpcie_select_asc_from_ac(config_ptr);
    /* Calculate the UL TFCS for the RACH */
    tdsrrcllc_compute_ul_tfis_from_ctfc(&config_ptr->ul_tfcs);

    config_ptr->mac_ul_parms.tfcs_ptr = &config_ptr->ul_tfcs;
    config_ptr->mac_ul_parms.cctrch_ptr = &config_ptr->ul_cctrch;
    
    for(ix = 0 ; ix < TDSUE_MAX_TRCH; ix++)
    {
      /* L1 UL Semi static parameter data pointers */
      config_ptr->l1_ul_cctrch_parms.semi_static_parms_ptr[ix] =
        &config_ptr->rrc_ul_cctrch_params.semi_static_parms[ix];
    }

    for(ix = 0; ix < TDSL1_UL_TFC_MAX; ix++)
    {
      /* Init the UL Gain Factors that are TFC specific */
      config_ptr->l1_ul_cctrch_parms.gain_factor_parms_ptr[ix] =
        &config_ptr->rrc_ul_cctrch_params.gain_factor_parms[ix];

    } /* end TFC initialization */

    /* Init the ASC data pointers */
    for(ix = 0; ix < MAX_ASC; ix ++)
    { 
      config_ptr->l1_ul_chan_parms.prach_cfg.asc_cfg.asc_info_ptr[ix] =
        &config_ptr->asc_info[ix];
    } /* end ASC initialization */
  }
  else 
  { 
    ERR_FATAL("This shouldn't happen .. something fishy in extracting PRACH Info from SIB5/6",0,0,0);
  } 

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updated OC with PRACH specific info");
}/* tdsrrcllcpcie_update_oc_with_prach_info */

/*====================================================================
FUNCTION: tdsrrcllcpcie_copy_to_prach_type

DESCRIPTION:
  This function copies PRACH info from OC to new PRACH working DB.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllcpcie_copy_to_prach_type
(
  tdsprach_config_type *prach_ptr,
  tdsordered_config_type *oc_ptr
)
{
  uint8 ix;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Selectively copy OC to new PRACH WORKING DB.");
  
  for(ix=0; ix < TDSUE_MAX_TRCH; ix++)
  {
    prach_ptr->ul_rach_trch_info[ix] = oc_ptr->ul_rach_trch_info[ix];
      prach_ptr->ul_rach_trch_idx_in_use[ix] = oc_ptr->ul_rach_trch_idx_in_use[ix];
  }
  
  prach_ptr->prach_cfg = oc_ptr->l1_ul_chan_parms.prach_cfg;
  
  for(ix = 0; ix < MAX_ASC; ix++)
  {
    prach_ptr->asc_info[ix] = oc_ptr->asc_info[ix];
      prach_ptr->prach_cfg.asc_cfg.asc_info_ptr[ix] = &prach_ptr->asc_info[ix];
  }
  
  return;
}

/*====================================================================
FUNCTION: tdsrrcllcpcie_update_prach_l1_selection_info_for_prach_db

DESCRIPTION:
  This function updates PRACH specific DB with fields of interest
  that are used for choosing PRACH 10 msecs or 20 msecs TTI.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllcpcie_update_prach_l1_selection_info_for_prach_db
(
  tdsrrcllc_prach_sel_info_type *prach_sel_info_ptr
)
{
  uint8  ix = 0;



  for(; ix < TDSUE_MAX_TRCH; ix++)
  {
   /* L1 UL Semi static parameter data pointers */
    prach_sel_info_ptr->l1_ul_cctrch_parms.semi_static_parms_ptr[ix] =
      &prach_sel_info_ptr->semi_static_parms[ix];
  }

  for(ix = 0; ix < TDSL1_UL_TFC_MAX; ix++)
  {
    /* Init the UL Gain Factors that are TFC specific */
    prach_sel_info_ptr->l1_ul_cctrch_parms.gain_factor_parms_ptr[ix] =
      &prach_sel_info_ptr->gain_factor_parms[ix];
  } /* end TFC initialization */

} /* tdsrrcllc_update_prach_l1_selection_info */

/*====================================================================
FUNCTION: tdsrrcllcpcie_get_prach_sys_info_list

DESCRIPTION:
  This function populates the prach system information from the SIB5/6

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_prach_sys_info_list
(
  tdsrrc_PRACH_SystemInformationList   *prach_sys_info_ptr,
  tdsrrc_PRACH_SystemInformationList_LCR_r4   *prach_sys_lcr_r4,
#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrc_PRACH_SystemInformationList_LCR_v770ext  *prach_sys_info_lcr_r7_ptr,
#endif /*FEATURE_TDSCDMA_HSUPA*/
  tdsordered_config_type               *oc_config_ptr,
  uint8  rrc_num_of_prach
)
{
  uint8  trch_idx;   /* Local TrCH index */
  uint8  num_of_prach = 1;   /* count of the number of PRACHs included */
  uint8  num_of_asc = 1; /* count of the number of Access Service Classes (ASC) included */
  tdsrrc_TransportFormatSet_LCR          *prev_prach_tfs_ie_ptr = NULL;
  tdsrrc_PRACH_Partitioning_LCR_r4       *prev_prach_partitioning_ie_ptr = NULL;
  tdsrrc_PRACH_Partitioning_LCR_r4        temp_prach_partitioning_ie;
  tdsrrc_ASCSetting_TDD_LCR_r4            temp_ascsetting_ie;
  tdsrrc_AC_To_ASC_MappingTable   *prev_ac_to_asc_mapping_ie_ptr = NULL;
  tdsrrc_PersistenceScalingFactorList *prev_prach_persistencesf_list_ie_ptr = NULL;
  tdsrrc_PRACH_SystemInformationList   *ie_ptr = prach_sys_info_ptr;   /* local PRACH System Info pointer */
  tdsrrc_PRACH_SystemInformationList_LCR_r4   *ie_lcr_ptr = prach_sys_lcr_r4;
#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrc_PRACH_SystemInformationList_LCR_v770ext *ie_lcr_v770ext_ptr = prach_sys_info_lcr_r7_ptr;
#endif /*FEATURE_TDSCDMA_HSUPA*/
  uint32 n_index;
  
  if ((ie_ptr == NULL)||(ie_lcr_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH System Info not present");
    return(FAILURE);
  }

  TDSRRC_MSG0(MSG_LEGACY_MED,"Reading SIB PRACH Sys Info");

  /*---------------------------------------------------------------
  * Initialize the values of where the PRACH's index would be
  * stored
  ----------------------------------------------------------------*/
  tdsprach_status_info.current_5ms_tti_prach_cnt = 0;
  tdsprach_status_info.current_10ms_tti_prach_cnt = 0;
  tdsprach_status_info.current_20ms_tti_prach_cnt = 0;
  tdsprach_status_info.num_valid_prachs_found = 0;

  tdsrrcllcpcie_init_prach_ie_ptrs(rrc_num_of_prach);

  /*
  * Note that the ASN.1 generates a Linked List. Hence, loop until the
  * the end of the list. Also make sure that not more than UE_MAX_PRACH
  * linked elements are included.
  */


  n_index = 0;
  
  for(; ((ie_ptr->n >  n_index) && (num_of_prach <= rrc_num_of_prach) && (num_of_prach <= UE_MAX_PRACH)); n_index++)
  {

    if(tdsprach_sel_info[num_of_prach-1] == NULL)
    {
      ERR_FATAL("PRACH_OPT: Null Pointer for %d PRACH in tdsprach_sel_info",num_of_prach,0,0);
    }

    num_of_asc = 1;

    /* Save copies of this Transport Channel ID and Index to Ordered Config */
    if(((tr_ch_id_type)ie_ptr->elem[n_index].transportChannelIdentity >
        TDSRRCLLC_TRCH_ID_MAX_VALUE) ||
       ((tr_ch_id_type)ie_ptr->elem[n_index].transportChannelIdentity <
        TDSRRCLLC_TRCH_ID_MIN_VALUE))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"invalid TrCH ID %d",(tr_ch_id_type)ie_ptr->elem[n_index].transportChannelIdentity);
      continue;
    }

    memset(&tdsprach_working_db, 0x0, sizeof(tdsprach_config_type));
    tdsrrcllcpcie_copy_to_prach_type(&tdsprach_working_db, oc_config_ptr);
    tdsprach_sel_info[num_of_prach-1]->l1_ul_cctrch_parms.chan = TDSL1_UL_PHYCHAN_PRACH;  

    /* Only need FDD for PRACH to RACH info */
    if(tdsrrcllcpcie_get_prach_rach_info(&tdsprach_working_db.prach_cfg, &ie_lcr_ptr->elem[n_index].prach_RACH_Info_LCR) == FAILURE)
    {
      continue;
    }

    /* Get the transport channel Index */
    if((trch_idx = tdsrrcllc_get_ul_rach_trch_idx_for_prach_db(&tdsprach_working_db, tdsprach_sel_info[num_of_prach-1], 
          (tr_ch_id_type)ie_ptr->elem[n_index].transportChannelIdentity)) >= TDSUE_MAX_TRCH)
    {
      continue;
    }    

    /* for TFS */
    if(TDSRRC_CHECK_MSG_TYPE_IE(ie_lcr_ptr->elem[n_index], 
              rach_TransportFormatSet_LCRPresent))
    {
      /* Get all the RACH TFS data */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get UL TFS for TrCH ID %d",ie_ptr->elem[n_index].transportChannelIdentity);
    
      if(tdsrrcllc_get_ul_transport_format_set_for_prach_db(tdsprach_sel_info[num_of_prach-1], 
           &ie_lcr_ptr->elem[n_index].rach_TransportFormatSet_LCR, trch_idx) == FAILURE)
      {
        continue;
      }

      /* Store the value of this ie*/
      prev_prach_tfs_ie_ptr = &ie_lcr_ptr->elem[n_index].rach_TransportFormatSet_LCR;
    }
    else  /* RACH TFS not present */
    {
      /* For the first PRACH it is MP, for the next, pick it from the
       * previous one.
       */
      if(num_of_prach == 1)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RACH TFS absent for 1st PRACH");
        return(FAILURE);
      }
      else
      {
        if (prev_prach_tfs_ie_ptr != NULL)
        {
          if(tdsrrcllc_get_ul_transport_format_set_for_prach_db(tdsprach_sel_info[num_of_prach-1], prev_prach_tfs_ie_ptr, trch_idx)
              == FAILURE)
          {
            /* The IE was not included, copy from the previous IE */
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Copy from Previous PRACH TFS data failed");
            continue;
          }
        }
        else
        {
            /* The IE was not included, copy from the previous IE */
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Previous PRACH TFS data not available");
            continue; 
        }
      }
    } /* RACH TFS not present */    

    /* Only need FDD for TFCS, For TDD in this release there is no TFCS required */

    /* Get the PRACH partioning information, as this is per ASC */
    if(TDSRRC_CHECK_MSG_TYPE_IE(ie_lcr_ptr->elem[n_index],
             prach_Partitioning_LCRPresent))
    {
      /* We now have the number of ASCs supported. */
      if(tdsrrcllcpcie_get_prach_partitioning(tdsprach_working_db.asc_info, &tdsprach_working_db.prach_cfg, 
        &ie_lcr_ptr->elem[n_index].prach_Partitioning_LCR, &num_of_asc) == FAILURE)
      {
        /* Bad values in this IE */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"First PRACH partitioning data save failed");
        continue;
      }
      prev_prach_partitioning_ie_ptr = &ie_lcr_ptr->elem[n_index].prach_Partitioning_LCR;
    } /* end if IE present and TDD mode chosen */
    else
    {
      if(num_of_prach == 1)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PRACH partitioning data absent for 1st PRACH,set 1 to prev_prach_partitioning_ie_ptr");
        prev_prach_partitioning_ie_ptr = &temp_prach_partitioning_ie;
        temp_prach_partitioning_ie.n = 1;
        temp_prach_partitioning_ie.elem = &temp_ascsetting_ie;
        temp_ascsetting_ie.m.accessServiceClass_TDD_LCRPresent = 1;
        temp_ascsetting_ie.accessServiceClass_TDD_LCR.m.availableSYNC_UlCodesIndicsPresent = 0;
        temp_ascsetting_ie.accessServiceClass_TDD_LCR.subchannelSize.t 
            = T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size1;
        if(tdsrrcllcpcie_get_prach_partitioning(tdsprach_working_db.asc_info, &tdsprach_working_db.prach_cfg, 
          prev_prach_partitioning_ie_ptr, &num_of_asc) == FAILURE)
        {
          /* Bad values in this IE */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"First PRACH partitioning data save failed");
          continue;
        }
      }
      else
      {
        if (prev_prach_partitioning_ie_ptr != NULL)
        {
              /* We now have the number of ASCs supported. */
              if(tdsrrcllcpcie_get_prach_partitioning(tdsprach_working_db.asc_info, &tdsprach_working_db.prach_cfg, 
                prev_prach_partitioning_ie_ptr, &num_of_asc) == FAILURE)
              {
                /* The IE was not included, copy from the previous IE */
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"Copy from previous PRACH PARTITIONING data failed");
                continue;
              }
        }
        else
        {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Previous PRACH PARTITIONING data not available");
            continue;
        }
      }
    }
    /* end Partitioning data */    

    tdsprach_working_db.prach_cfg.asc_cfg.num_asc = (uint8)num_of_asc;

    /*
    * If there are more than 2 access service classes, then we get all
    * the Persistence value stuff.
    */
    if(num_of_asc > 2)
    {
      if(TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                  persistenceScalingFactorListPresent))
      {
        prev_prach_persistencesf_list_ie_ptr = &ie_ptr->elem[n_index].persistenceScalingFactorList;
      } /* persistence scaling factor is present */
      else
      {
        if(num_of_prach == 1)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring absence of Persistence sclng factor");
        }
        else
        {
          if (prev_prach_persistencesf_list_ie_ptr == NULL)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Previous PRACH persistence scaling factors not available");
            continue;
          }
        }
      } /* Persistence scaling factor list is not present */
    }  /* Number of ASC > 2 */

    /* get the AC to ASC Mapping info */
    if(TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                ac_To_ASC_MappingTablePresent))
    {
      prev_ac_to_asc_mapping_ie_ptr = &ie_ptr->elem[n_index].ac_To_ASC_MappingTable;
    }
    else
    {
      if(num_of_prach == 1)
      {
        /* Ignore at this point if ASC to AC mapping is absent.
         * It is not used at present. Need to change spec to make it an MP for
         * 1st PRACH.
         */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"First ASC-to-AC mapping is absence");
        return(FAILURE);
      }
      else
      {
        if (prev_ac_to_asc_mapping_ie_ptr == NULL)
        {
          /* The IE was not included, copy from the previous IE */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No previous ACCESS SERVICE CLASS MAPPING data to get");
          continue;
        }
      }
    }
    /* end AC to ASC mapping */    

    /* Make sure we have TDD parameters or else report a failure */
    if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[n_index].modeSpecificInfo,
                             T_tdsrrc_PRACH_SystemInformation_modeSpecificInfo_fdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD mode choice Rcv'd");
      continue;
    }
    
    if (tdsprach_sel_info[num_of_prach-1]->ul_tfcs.tfs_info[0].tti_info
          == TDSL1_TTI_5MS)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Found 5ms PRACH,putting %d prach in index %d",(num_of_prach-1),tdsprach_status_info.current_5ms_tti_prach_cnt);
      tdsprach_5ms_tti_index_array[tdsprach_status_info.current_5ms_tti_prach_cnt++] =
        num_of_prach-1;
    }
    else if (tdsprach_sel_info[num_of_prach-1]->ul_tfcs.tfs_info[0].tti_info
          == TDSL1_TTI_10MS)
    {
      if(tdsprach_status_info.current_10ms_tti_prach_cnt < UE_MAX_PRACH)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Found 10ms PRACH,putting %d prach in index %d",(num_of_prach-1),tdsprach_status_info.current_10ms_tti_prach_cnt);
        tdsprach_10ms_tti_index_array[tdsprach_status_info.current_10ms_tti_prach_cnt++] =
          num_of_prach-1;
      }
    }
    else
    {
      if(tdsprach_status_info.current_20ms_tti_prach_cnt < UE_MAX_PRACH)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Found 20ms PRACH,putting %d prach in index %d",(num_of_prach-1),tdsprach_status_info.current_20ms_tti_prach_cnt);
        tdsprach_20ms_tti_index_array[tdsprach_status_info.current_20ms_tti_prach_cnt++] =
          num_of_prach-1;
      }
    }
#ifdef FEATURE_TDSCDMA_HSUPA
    /* get e-RUCCH-Sync-UL-Codes-Bitmap */
    if (ie_lcr_v770ext_ptr != NULL)
    {
        tdsprach_sel_info[num_of_prach-1]->prach_RACH_Info_LCR = 
          &ie_lcr_v770ext_ptr->elem[n_index].prach_RACH_Info_LCR;
    }
#endif /*FEATURE_TDSCDMA_HSUPA*/

    tdsrrcllcpcie_update_prach_l1_selection_info_for_prach_db(tdsprach_sel_info[num_of_prach-1]);    
    
    /* Record sub-IE pointers */
    tdsprach_sel_info[num_of_prach-1]->rach_info = &ie_lcr_ptr->elem[n_index].prach_RACH_Info_LCR;
    tdsprach_sel_info[num_of_prach-1]->transport_channel_id = ie_ptr->elem[n_index].transportChannelIdentity;
    tdsprach_sel_info[num_of_prach-1]->rach_tfs = prev_prach_tfs_ie_ptr;
    tdsprach_sel_info[num_of_prach-1]->prach_partitioning = prev_prach_partitioning_ie_ptr;
    tdsprach_sel_info[num_of_prach-1]->persistence_scaling_factor_list = prev_prach_persistencesf_list_ie_ptr;
    tdsprach_sel_info[num_of_prach-1]->ac_to_asc_mapping_table = prev_ac_to_asc_mapping_ie_ptr;

    /* get the next element in the linked list */
    num_of_prach++;
  } 
  /* end while loop processing PRACH System Information IEs */

  if (num_of_prach == 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not find any valid PRACH for this cell");
    return(FAILURE);
  }

  tdsprach_status_info.num_valid_prachs_found = num_of_prach - 1;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number of PRACH's populated for this cell %d",num_of_prach-1);

  return SUCCESS;

} /* end tdsrrcllcpcie_get_prach_sys_info_list() */

/*====================================================================
FUNCTION       tdsrrcllcpcie_get_prach_sys_info_com()

DESCRIPTION    This function takes a pointer to the PRACH system
               information list and copies all applicable IEs for
               MAC, RLC, and L1 into the TDSORDERED_CONFIG database
               (internal format).

DEPENDENCIES   None

RETURN VALUE   SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS   The current contents of TDSORDERED_CONFIG will be
               overwritten.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_prach_sys_info_com
(
  tdsordered_config_type *config_ptr,
  tdsrrc_PRACH_SystemInformationList *prach_sys_info_ptr,
  tdsrrc_PRACH_SystemInformationList_LCR_r4 *prach_sys_info_lcr_r4_ptr
#ifdef FEATURE_TDSCDMA_HSUPA
  ,tdsrrc_PRACH_SystemInformationList_LCR_v770ext *prach_sys_info_lcr_r7_ptr
#endif /*FEATURE_TDSCDMA_HSUPA*/
)
{

  /* Local TrCH index */
  uint8                             trch_idx;

  /* count of the number of PRACHs included */
  uint8                               num_of_prach = 1;

  /* count of the number of Access Service Classes (ASC) included */
  uint8                               num_of_asc = 1;

  /* index of ASCs for calculation of Persistence values */
  uint8                               asc_idx;

  /* calculated Persistence value */
  uint32                              pval = 0;
  /* local PRACH System Info pointer */
  tdsrrc_PRACH_SystemInformationList   *ie_ptr = prach_sys_info_ptr;
  tdsrrc_PRACH_SystemInformationList_LCR_r4   *ie_lcr_ptr = prach_sys_info_lcr_r4_ptr;
#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrc_PRACH_SystemInformationList_LCR_v770ext   *ie_lcr_v770ext_ptr = prach_sys_info_lcr_r7_ptr;
#endif /*FEATURE_TDSCDMA_HSUPA*/
  uint32 n_index = 0;

  if ((NULL == ie_ptr)||(NULL == ie_lcr_ptr))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH System Info not present");
    return(FAILURE);
  }

  /*
  * To fill up the PRACH array the following algorithm is used as defined
  * in the RRC  spec. Note that from 1 to MAX_PRACH IEs can be included
  * in the PRACH system information list. The algorithm is:
  *
  *   int num_of_prach = 1;
  *
  *   while ((PRACH_ptr != NULL) && (num_of_prach <= MAX_PRACH))
  *
  *     PRACH_Info[num_of_prach - 1] = PRACH_Info(num_of_prach)
  *
  *     TrCh_Id[num_of_prach - 1]    = TrCH_Id(num_of_prach)
  *
  *     if(rrc_PRACH_SystemInformation_rach_TransportFormatSet_present) || num_of_prach == 1)
  *       RACH_TFS[num_of_prach - 1] = RACH_TFS(num_of_prach)
  *     else
  *       RACH_TFS[num_of_prach - 1] = RACH_TFS[num_of_prach - 2]
  *
  *     if(rrc_PRACH_SystemInformation_rach_TFCS_present) || num_of_prach == 1)
  *       RACH_TFCS[num_of_prach - 1] = RACH_TFCS(num_of_prach)
  *     else
  *       RACH_TFCS[num_of_prach - 1] = RACH_TFCS[num_of_prach - 2]
  *
  *     if(rrc_PRACH_SystemInformation_prach_Partitioning_present) || num_of_prach == 1)
  *       PRACH_PARTITIONING[num_of_prach - 1] =
  *         PRACH_PARTITIONING(num_of_prach)
  *     else
  *       PRACH_PARTITIONING[num_of_prach - 1] =
  *         PRACH_PARTITIONING[num_of_prach - 2]
  *
  *     ... And so on for the remaing IEs for FDD mode ...
  *
  *  end while loop.
  */

  TDSRRC_MSG0(MSG_LEGACY_MED,"Reading SIB PRACH Sys Info");

  /*
  * Note that the ASN.1 generates a Linked List. Hence, loop until the
  * the end of the list. Also make sure that not more than MAX_PRACH
  * linked elements are included.
  */
  while((ie_ptr->n > n_index) && (num_of_prach <= UE_MAX_PRACH))
  {
    /* Indicate to uplink that the current channel is a PRACH */
    config_ptr->l1_ul_cctrch_parms.chan = TDSL1_UL_PHYCHAN_PRACH;

    /* Only need FDD for PRACH to RACH info */
    if(tdsrrcllcpcie_get_prach_rach_info(&config_ptr->l1_ul_chan_parms.prach_cfg, 
                   &ie_lcr_ptr->elem[n_index].prach_RACH_Info_LCR) == FAILURE)
    {
      return FAILURE;
    }

    /* Save copies of this Transport Channel ID and Index to Ordered Config */
    if(((tr_ch_id_type)ie_ptr->elem[n_index].transportChannelIdentity >
        TDSRRCLLC_TRCH_ID_MAX_VALUE) ||
       ((tr_ch_id_type)ie_ptr->elem[n_index].transportChannelIdentity <
        TDSRRCLLC_TRCH_ID_MIN_VALUE))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"invalid TrCH ID %d",(tr_ch_id_type)ie_ptr->elem[n_index].transportChannelIdentity);
      return(FAILURE);
    }

   /* Get the transport channel Index */
    if((trch_idx = tdsrrcllc_get_ul_rach_trch_idx(
                     config_ptr,
                    (tr_ch_id_type)ie_ptr->elem[n_index].transportChannelIdentity))
        >= TDSUE_MAX_TRCH)
    {
      return FAILURE;
    }

    /* ... and make sure we have not exceeded UE capaabilities for UL TRCH's */
    if((TDSRRCLLC_UPLINK_TRCH_COUNT(config_ptr)) >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Exceeded maximum number of UL TRCHs");
      return FAILURE;
    }

    /* Only need FDD for TFS */
    if (ie_lcr_ptr->elem[n_index].m.rach_TransportFormatSet_LCRPresent)
    {
      /* Get all the RACH TFS data */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get UL TFS for TrCH ID %d",ie_ptr->elem[n_index].transportChannelIdentity);
      if(tdsrrcllc_get_ul_transport_format_set(config_ptr, NULL,
           (&ie_lcr_ptr->elem[n_index].rach_TransportFormatSet_LCR), trch_idx)
        == FAILURE)
      {
        return FAILURE;
      }
    }
    else  /* RACH TFS not present */
    {
      /* For the first PRACH it is MP, for the next, pick it from the
       * previous one.
       */
      if(num_of_prach == 1)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RACH TFS absent for 1st PRACH");
        return(FAILURE);
      }
      else
      {
        /* The IE was not included, copy from the previous IE */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Copy from Previous PRACH TFS data not supported");
        return (FAILURE);
      }
    } /* RACH TFS not present */

    /* Only need FDD for TFCS, For TDD in this release there is no TFCS required */
    /* set default value according to MAC */
    config_ptr->ul_tfcs.ntfc = 1;
    config_ptr->ul_tfcs.tfc_info[0].control_flag = TDSTFC_ALLOWED;
    config_ptr->ul_tfcs.tfc_info[0].ctfc = 0;
    config_ptr->ul_tfcs.tfc_info[0].tfi[0] = 0;

    /* Get the PRACH partioning information, as this is per ASC */
    if (ie_lcr_ptr->elem[n_index].m.prach_Partitioning_LCRPresent)
    {
      /* We now have the number of ASCs supported. */
      if (tdsrrcllcpcie_get_prach_partitioning(config_ptr->asc_info, &config_ptr->l1_ul_chan_parms.prach_cfg, 
        &ie_lcr_ptr->elem[n_index].prach_Partitioning_LCR, &num_of_asc) == FAILURE)
      {
        /* Bad values in this IE */
        return FAILURE;
      }
    } /* end if IE present and FDD mode chosen */
    else
    {
      if(num_of_prach == 1)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH partitioning data absent for 1st PRACH");
        return(FAILURE);
      }
      else
      {
        /* The IE was not included, copy from the previous IE */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Copy from previous PRACH PARTITIONING data not supported");
        return (FAILURE);
      }
    }
    /* end Partitioning data */

    /* Give L1 the number of ASCs rcv'd over the air */
    config_ptr->l1_ul_chan_parms.prach_cfg.asc_cfg.num_asc = (uint8)num_of_asc;

    /*
    * Get the Persistence scaling factors. Since the number of ASC's was
    * determined by the prach partitioning info, and the Dynamic Persistance
    * value, N, was taken from Sib 7. P(N) = 2e-(N-1), the following
    * table holds per ASC where SX equals the persistence scaling factor
    * which is up tp. If only ASC 0 and 1 are sent, no Persistence
    * scaling factor is present. However, if more than 2 ASC's are included
    * the following holds:
    *
    * -----------------------------------------------------------------------
    * | ASC idx |    2    |    3    |    4    |    5    |    6    |    7    |
    * -----------------------------------------------------------------------
    * |  P2     | S2*P(N) | S3*P(N) | S4*P(N) | S5*P(N) | S6*P(N) | S7*P(N) |
    * |  P3     | S2*P(N) | S3*P(N) | S4*P(N) | S5*P(N) | S6*P(N) | S7*P(N) |
    * |  P4     | S2*P(N) | S3*P(N) | S4*P(N) | S5*P(N) | S6*P(N) | S7*P(N) |
    * |  P5     | S2*P(N) | S3*P(N) | S4*P(N) | S5*P(N) | S6*P(N) | S7*P(N) |
    * |  P6     | S2*P(N) | S3*P(N) | S4*P(N) | S5*P(N) | S6*P(N) | S7*P(N) |
    * |  P7     | S2*P(N) | S3*P(N) | S4*P(N) | S5*P(N) | S6*P(N) | S7*P(N) |
    * -----------------------------------------------------------------------
    *
    * For P2 - P7 ASC 0, the table value is 1.
    * For P2 - P7 ASC 1, the table value is just P(N) without a scaling factor.
    *
    * If only ASC 0, and ASC 1 are sent, the Persistence Scaling Factors are
    * not to be included in SIB 5, and only 1 Dynamic Persistence Value should
    * be present in SIB 7 for ASC 1. The assumption is that if more than 1
    * Dynamic Persistence Value is included, then the remaining are ignored.
    * If any Persistence Scaling Factors are included, then they too are
    * ignored.
    *
    * NOTE: for ASC 0 and ASC 1, the above table does not hold.
    *
    */

    /*
    * If there are more than 2 access service classes, then we get all
    * the Persistence value stuff.
    */
    if(num_of_asc > 2)
    {
      if(TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                   persistenceScalingFactorListPresent))
      {
        /* Get persistence values */
        if(tdsrrcllcpcie_get_persist_scaling_factors(config_ptr, &ie_ptr->elem[n_index].persistenceScalingFactorList)
          == FAILURE)
        {
          return FAILURE;
        }
      } /* persistence scaling factor is present */
      else
      {
        if(num_of_prach == 1)
        {
          /* Set default value of 1 to all ASCs as per spec */
          tdsrrcllcpcie_get_persist_scaling_default_factors(config_ptr);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring absence of Persistence sclng factor");
        }
        else
        {
          /* Persistence scaling factor data not present AND this is NOT the
           * 1st PRACH. Get the persistence value from previous PRACH
           */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Previous PERSISTENCE SCALING FACTOR data not supported");
          return FAILURE;
        }
      } /* Persistence scaling factor list is not present */
    }  /* Number of ASC > 2 */

    /* Setup the Persistence values for all ASCs */
    for(asc_idx = 0; ((asc_idx < num_of_asc)&& (asc_idx < MAX_ASC)); asc_idx++)
    {
      switch(asc_idx)
      {
        case 0:
//          pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR *
//            TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
//          break;
          pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR;
          break;
        case 1:
//          pval = (TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>
//                  (config_ptr->dpl - 1)) *
//                  TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
//          break;
          pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR>>(config_ptr->dpl - 1);
          break;
        default:
//          if((asc_idx - 2) >=0)
//          {
//            pval = (TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>
//                    (config_ptr->dpl - 1)) *
//                   config_ptr->psf[asc_idx - 2]; /*lint !e676 */
//                   /* lint mistakenly believes there is a possibility for a
//                      negative subscript here. */
//          }
//          break;
          /*psf from spec goes from 0.9..0.2, we have scaled it to 9...2(factor of 10), this is why we 
              divide by RRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL*/
          pval = ((TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR>>(config_ptr->dpl - 1)) * config_ptr->psf[asc_idx - 2])/
                  TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL; 
          break;
      }
      config_ptr->mac_ul_parms.rach_tx_cntl_info.asc[asc_idx].pval = pval;
//      {
//        config_ptr->mac_ul_parms.rach_tx_cntl_info.asc[asc_idx].pval =
//        pval / (((TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR * 10) / 0xffff) + 1);
//      }
    }

    /* Indicate the number of Access Service Classes supported */
    config_ptr->mac_ul_parms.rach_tx_cntl_info.nASC = num_of_asc;

    /* get the AC to ASC Mapping info */
    if(TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                ac_To_ASC_MappingTablePresent))
    {
      if(tdsrrcllcpcie_get_ac_to_asc_mapping(config_ptr, &ie_ptr->elem[n_index].ac_To_ASC_MappingTable) == FAILURE)
      {
        return FAILURE;
      }
    }
    else
    {
      if(num_of_prach == 1)
      {
        /* Ignore at this point if ASC to AC mapping is absent.
         * It is not used at present. Need to change spec to make it an MP for
         * 1st PRACH.
         */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring absence of ASC-to-AC mapping");
      }
      else
      {
        /* The IE was not included, copy from the previous IE */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No previous ACCESS SERVICE CLASS MAPPING data to get");
        return FAILURE;
      }
    }
    /* end AC to ASC mapping */

    /* Now select the ASC for CCCH. This is for initial access
     * for RRC Connection Setup message on CCCH.
     */
    tdsrrcllcpcie_select_asc_from_ac(config_ptr);
#ifdef FEATURE_TDSCDMA_HSUPA
    config_ptr->sync_code_erucch_770ext_indicator = FALSE;
    /* get e-RUCCH-Sync-UL-Codes-Bitmap */
    if (ie_lcr_v770ext_ptr != NULL)
    {
    /* Bitmap of SYNC_UL codes. Each bit indicates availability of a SYNC_UL code,
        * where the SYNC_UL codes are numbered "code0" to "code7". The value 1 of a 
        * bit indicates that the corresponding SYNC_UL code can be used, vice versa.
        * See 25.331 v9.1.0, 10.3.6.52 and 10.3.6.78a 
        */
      if (ie_lcr_v770ext_ptr->elem[n_index].prach_RACH_Info_LCR.e_RUCCH_Sync_UL_Codes_Bitmap.numbits == 8)
      {
        config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.ul_sync_code_mask = 
          ie_lcr_v770ext_ptr->elem[n_index].prach_RACH_Info_LCR.e_RUCCH_Sync_UL_Codes_Bitmap.data[0];
        config_ptr->sync_code_erucch_770ext_indicator = TRUE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"sync_UL_Codes_Bitmap Bitstring number is %d, not 8(prach rach info),Error",ie_lcr_v770ext_ptr->elem[n_index].prach_RACH_Info_LCR.e_RUCCH_Sync_UL_Codes_Bitmap.numbits);
        return FAILURE;
      }
    }
#endif /*FEATURE_TDSCDMA_HSUPA*/

    /* Make sure we have TDD parameters or else report a failure */
    if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[n_index].modeSpecificInfo,
                T_tdsrrc_PRACH_SystemInformation_modeSpecificInfo_fdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD mode choice Rcv'd");
      return FAILURE;
    }

    /* get the next element in the linked list */
    num_of_prach++;
    n_index++;
  } /* end while loop processing PRACH System Information IEs */

  /* Calculate the UL TFCS for the RACH */
  tdsrrcllc_compute_ul_tfis_from_ctfc(&config_ptr->ul_tfcs);

  /* Indicate that L1 UL needs to be reconfigured */
  config_ptr->reconfig_needed.uplink_l1 = TRUE;

  return SUCCESS;

}

/*====================================================================
FUNCTION       tdsrrcllcpcie_get_prach_sys_info_for_one_prach()

DESCRIPTION    This function takes a pointer to the PRACH system
               information list and copies all applicable IEs for
               MAC, RLC, and L1 into the TDSORDERED_CONFIG database
               (internal format).

DEPENDENCIES   None

RETURN VALUE   SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS   The current contents of TDSORDERED_CONFIG will be
               overwritten.
====================================================================*/

static uecomdef_status_e_type tdsrrcllcpcie_get_prach_sys_info_for_one_prach
(
  tdsordered_config_type *config_ptr,
  tdsrrc_PRACH_SystemInformationList *prach_sys_info_ptr,
  tdsrrc_PRACH_SystemInformationList_LCR_r4 *prach_sys_info_lcr_r4_ptr
#ifdef FEATURE_TDSCDMA_HSUPA
  ,tdsrrc_PRACH_SystemInformationList_LCR_v770ext  *prach_sys_info_lcr_r7_ptr
#endif /*FEATURE_TDSCDMA_HSUPA*/
)
{

  /* common function */
  if (FAILURE == tdsrrcllcpcie_get_prach_sys_info_com(config_ptr, prach_sys_info_ptr, prach_sys_info_lcr_r4_ptr
#ifdef FEATURE_TDSCDMA_HSUPA
               , prach_sys_info_lcr_r7_ptr
#endif /*FEATURE_TDSCDMA_HSUPA*/
                ))
  {
    return FAILURE;
  }

  return SUCCESS;

}

/*====================================================================
FUNCTION       tdsrrcllc_get_prach_sys_info()

DESCRIPTION    This function takes a pointer to the PRACH system
               information list and copies all applicable IEs for
               MAC, RLC, and L1 into the TDSORDERED_CONFIG database
               (internal format).

DEPENDENCIES   None

RETURN VALUE   SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS   The current contents of TDSORDERED_CONFIG will be
               overwritten.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_prach_sys_info
(
  tdsordered_config_type *config_ptr,
  void                   *sib5_6ptr,
  /* sib6_prach_present is ture means get prach info from sib6, 
     sib6_prach_present is false means get prach info from sib5. */
  uint8   sib6_prach_present
)
{
  tdsrrc_SysInfoType5    *sib5_ptr = NULL;
  tdsrrc_SysInfoType6    *sib6_ptr = NULL;


  tdsrrcllc_prach_sel_info_type                   *selected_config_ptr = NULL;
  tdsl1_rach_tti_allowed_enum_type         allowed_tti = TDSL1_PRACH_INVALID;
#ifndef T_WINNT
  uint32                                random_num;
#else
  #error code not present
#endif
  uint8                                 selected_index = 0;
#ifndef T_WINNT
    uint16                              start_index, end_index;
#endif
  tdsrrc_PRACH_SystemInformationList   * temp_ie_ptr;
  uint8  rrc_num_of_prach = 0;    

  tdsrrc_PRACH_SystemInformationList   *ie_ptr = NULL;
  tdsrrc_PRACH_SystemInformationList_LCR_r4   *ie_lcr_ptr = NULL;
#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrc_PRACH_SystemInformationList_LCR_v770ext   *ie_lcr_v770ext_ptr = NULL;
#endif /*FEATURE_TDSCDMA_HSUPA*/

  /* local PRACH System Info pointer */
  if (TRUE == sib6_prach_present)
  {
    sib6_ptr = (tdsrrc_SysInfoType6*)sib5_6ptr;
    ie_ptr =  &sib6_ptr->prach_SystemInformationList;
    if ((sib6_ptr->m.v4b0NonCriticalExtensionsPresent)&&
        (sib6_ptr->v4b0NonCriticalExtensions.m.sysInfoType6_v4b0extPresent)&&
        (sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.m.prach_SystemInformationList_LCR_r4Present))
    {
      ie_lcr_ptr =  &sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.prach_SystemInformationList_LCR_r4;
    }
    temp_ie_ptr = &sib6_ptr->prach_SystemInformationList;;
  }
  else
  {
    sib5_ptr = (tdsrrc_SysInfoType5*)sib5_6ptr;
    ie_ptr = &sib5_ptr->prach_SystemInformationList;
    if ((sib5_ptr->m.v4b0NonCriticalExtensionsPresent)&&
        (sib5_ptr->v4b0NonCriticalExtensions.m.sysInfoType5_v4b0extPresent)&&
        (sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.m.prach_SystemInformationList_LCR_r4Present))
    {
      ie_lcr_ptr = &sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.prach_SystemInformationList_LCR_r4;
    }
#ifdef FEATURE_TDSCDMA_HSUPA
    if ((sib5_ptr->m.v4b0NonCriticalExtensionsPresent)&&
        (sib5_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent)&&
        (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v650NonCriticalExtensionsPresent)&&
        (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.m.
        v680NonCriticalExtensionsPresent)&&
        (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.
        v680NonCriticalExtensions.m.v690NonCriticalExtensionsPresent)&&
        (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.v680NonCriticalExtensions.
        v690NonCriticalExtensions.m.v770NonCriticalExtensionsPresent))
    {
      if (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.v680NonCriticalExtensions.
        v690NonCriticalExtensions.v770NonCriticalExtensions.sysInfoType5_v770ext.m.modeSpecificInfoPresent)
      {
        if (T_tdsrrc_SysInfoType5_v770ext_IEs_modeSpecificInfo_tdd128 == sib5_ptr->v4b0NonCriticalExtensions.
          v590NonCriticalExtensions.v650NonCriticalExtensions.v680NonCriticalExtensions.v690NonCriticalExtensions.
          v770NonCriticalExtensions.sysInfoType5_v770ext.modeSpecificInfo.t)
        {
          if (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.v680NonCriticalExtensions
            .v690NonCriticalExtensions.v770NonCriticalExtensions.sysInfoType5_v770ext.modeSpecificInfo.u.
            tdd128->m.prach_SystemInformationListPresent)
          {
            ie_lcr_v770ext_ptr = &sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.
              v680NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.sysInfoType5_v770ext.
              modeSpecificInfo.u.tdd128->prach_SystemInformationList;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH System Info FDD mode or 7.68 TDD received");
          return(FAILURE);
        }
      }
    }
#endif /*FEATURE_TDSCDMA_HSUPA*/
    temp_ie_ptr = &sib5_ptr->prach_SystemInformationList;;
  }

  /* prach_SystemInformationList or prach_SystemInformationList_LCR not present */
  if ((ie_ptr == NULL)||(ie_lcr_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH System Info not present");
    return(FAILURE);
  }

  /* prach_SystemInformationList and prach_SystemInformationList_LCR not matching */
  if (ie_ptr->n != ie_lcr_ptr->n )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"prach_SystemInformationList and prach_SystemInformationList_LCR not matching");
    return(FAILURE);
  }
#ifdef FEATURE_TDSCDMA_HSUPA
  /* PRACH_SystemInformationList_LCR_v770ext and prach_SystemInformationList_LCR not matching, 
       ignore PRACH_SystemInformationList_LCR_v770ext */
  if ((ie_lcr_v770ext_ptr != NULL) && (ie_lcr_v770ext_ptr ->n != ie_lcr_ptr->n ))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH_SystemInformationList_LCR_v770ext and prach_SystemInformationList_LCR not matching");
    ie_lcr_v770ext_ptr = NULL;
  }
#endif /*FEATURE_TDSCDMA_HSUPA*/
  /* Init some PRACH info before starting to process new IEs */
  tdsrrcllc_init_rach_info(config_ptr, FALSE);

#ifdef T_WINNT
  #error code not present
#endif
  
  if ( ie_ptr->n  == 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is only one PRACH in sys information message");
    return (tdsrrcllcpcie_get_prach_sys_info_for_one_prach(config_ptr, ie_ptr, ie_lcr_ptr
#ifdef FEATURE_TDSCDMA_HSUPA
                    , ie_lcr_v770ext_ptr
#endif /*FEATURE_TDSCDMA_HSUPA*/
                    ));
  }
  
  // Initialize the PRACH status info fields
  tdsprach_status_info.current_5ms_tti_prach_cnt = 0;
  tdsprach_status_info.current_10ms_tti_prach_cnt = 0;
  tdsprach_status_info.current_20ms_tti_prach_cnt = 0;
  tdsprach_status_info.num_valid_prachs_found = 0;

  if(temp_ie_ptr != NULL)
  {
    rrc_num_of_prach = temp_ie_ptr->n;
  }
  rrc_num_of_prach++;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"PRACH_OPT: Total no. of PRACHs in SIB = %d",rrc_num_of_prach);

  tdsrrcllc_allocate_mem_for_prach_sel_info(rrc_num_of_prach);  
  
  // Populate the number of PRACH's from the sys info message
  if (tdsrrcllcpcie_get_prach_sys_info_list(ie_ptr, ie_lcr_ptr,
#ifdef FEATURE_TDSCDMA_HSUPA 
                 ie_lcr_v770ext_ptr, 
#endif /*FEATURE_TDSCDMA_HSUPA*/
                 config_ptr, rrc_num_of_prach) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not read the system info pointer");
    tdsrrcllc_deallocate_mem_for_prach_sel_info(rrc_num_of_prach);
    return (FAILURE);
  }
  if (tdsprach_status_info.current_20ms_tti_prach_cnt > 0)
  {
    allowed_tti = TDSL1_PRACH_20MS_ALLOWED;
  }
  else if (tdsprach_status_info.current_10ms_tti_prach_cnt > 0)
  {
    allowed_tti = TDSL1_PRACH_10MS_ALLOWED;
  }
  else if (tdsprach_status_info.current_5ms_tti_prach_cnt > 0)
  {
    allowed_tti = TDSL1_PRACH_5MS_ALLOWED;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error while populating PRACH sys information msg");
    tdsrrcllc_deallocate_mem_for_prach_sel_info(rrc_num_of_prach);
    return FAILURE;
  }
  /* Don't need the random selection part since there is only 1 PRACH in the
  sys info message */
  if (tdsprach_status_info.num_valid_prachs_found == 1)
  {
    selected_config_ptr = tdsprach_sel_info[0];
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Number of valid PRACH's found is 1");
  }
  else if (tdsprach_status_info.num_valid_prachs_found > 1)
  {
    /* Switch on the type of the TTI selected */
    switch (allowed_tti)
    {
    case  TDSL1_PRACH_5MS_ALLOWED:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PRACH with 5ms TTI length chosen");
      
#ifndef T_WINNT
      if(E_SUCCESS != secapi_get_random(SECAPI_SECURE_RANDOM, (uint8*)&random_num, sizeof(uint32)))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"secapi_get_random failure,use random_num=%d",random_num);
      }
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Random number generated was %d",random_num);
      /* Normalize this random number to a value between 0 & 65535 */
      
      start_index = 0;
      end_index = tdsprach_status_info.current_5ms_tti_prach_cnt;
      random_num = ran_dist(random_num, start_index, end_index);
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RanNext generated is %d %d %d", random_num,
        start_index,end_index);
#else
      #error code not present
#endif
      selected_index = tdsprach_5ms_tti_index_array[random_num];
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Selected PRACH index %d",selected_index);
      
      selected_config_ptr = tdsprach_sel_info[selected_index];
      break;
    case  TDSL1_PRACH_10MS_ALLOWED:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PRACH with 10ms TTI length chosen");
      
#ifndef T_WINNT
      if(E_SUCCESS != secapi_get_random(SECAPI_SECURE_RANDOM, (uint8*)&random_num, sizeof(uint32)))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"secapi_get_random failure,use random_num=%d",random_num);
      }
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Random number generated was %d",random_num);
      /* Normalize this random number to a value between 0 & 65535 */
      
      start_index = 0;
      end_index = tdsprach_status_info.current_10ms_tti_prach_cnt;
      random_num = ran_dist(random_num, start_index, end_index);
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RanNext generated is %d %d %d", random_num,
        start_index,end_index);
#else
      #error code not present
#endif
      selected_index = tdsprach_10ms_tti_index_array[random_num];
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Selected PRACH index %d",selected_index);
      
      selected_config_ptr = tdsprach_sel_info[selected_index];
      break;
      
    case TDSL1_PRACH_20MS_ALLOWED:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PRACH with 20ms TTI length chosen");
#ifndef T_WINNT
      if(E_SUCCESS != secapi_get_random(SECAPI_SECURE_RANDOM, (uint8*)&random_num, sizeof(uint32)))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"secapi_get_random failure,use random_num=%d",random_num);
      }

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Random number generated was %d",random_num);
      
      start_index = 0;
      end_index = tdsprach_status_info.current_20ms_tti_prach_cnt;
      
      /* Normalize this random number to a value between 20ms tti prach
      index to UE_MAX_PRACH */
      random_num = ran_dist(random_num, start_index, end_index);
      
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RanNext generated is %d %d %d", random_num,
        start_index, end_index);
#else
      #error code not present
#endif
      selected_index = tdsprach_20ms_tti_index_array[random_num];
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Selected index is %d",selected_index);
      
      selected_config_ptr = tdsprach_sel_info[selected_index];
      break;
    default:
      
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No PRACH was selected");
      tdsrrcllc_deallocate_mem_for_prach_sel_info(rrc_num_of_prach);
      return FAILURE;
    }
  }
  else // 0 PRACH's were found
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No PRACH was found !");
      tdsrrcllc_deallocate_mem_for_prach_sel_info(rrc_num_of_prach);
    return FAILURE;
  }
  
  /* Copy the valid parameters of the PRACH to the oc_config_ptr. */
  tdsrrcllcpcie_update_oc_with_prach_info(config_ptr, selected_config_ptr);
  tdsrrcllc_deallocate_mem_for_prach_sel_info(rrc_num_of_prach);
  return SUCCESS;
  

} /* end tdsrrcllc_get_prach_sys_info() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_prach_rach_info()

DESCRIPITON     This function takes a pointer to the PRACH to RACH
                info IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    The current Ordered Config data is overwritten.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_prach_rach_info
(
  tdsl1_ul_prach_cfg_struct_type *prach_cfg_ptr,
  tdsrrc_PRACH_RACH_Info_LCR_r4 *ie_lcr_ptr
)
{
  uint8  loop_i, loop_j;

  /* Bitmap of SYNC_UL codes. Each bit indicates availability of a SYNC_UL code,
   * where the SYNC_UL codes are numbered "code0" to "code7". The value 1 of a 
   * bit indicates that the corresponding SYNC_UL code can be used, vice versa.
   * See 25.331 v9.1.0, 10.3.6.52 and 10.3.6.78a 
   */
  if (ie_lcr_ptr->sync_UL_Info.sync_UL_Codes_Bitmap.numbits == 8)
  {
    prach_cfg_ptr->sync_ul_codes_bit_map = ie_lcr_ptr->sync_UL_Info.sync_UL_Codes_Bitmap.data[0];
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"sync_UL_Codes_Bitmap Bitstring number is %d, not 8(prach rach info),Error",ie_lcr_ptr->sync_UL_Info.sync_UL_Codes_Bitmap.numbits);
    return FAILURE;
  }

  /* Desired UpPCH receiving power, in dBm (-120..-58) 
   * See 25.331 v9.1.0, 10.3.6.78a, INTEGER (0..62) */
  prach_cfg_ptr->prx_upPch_des = (int8)(ie_lcr_ptr->sync_UL_Info.prxUpPCHdes - 120);

  /* UL sync code power ramp step when no ACK, in dB (0..3) */
  prach_cfg_ptr->pwr_ramp_step = ie_lcr_ptr->sync_UL_Info.powerRampStep;

  /* Maximum UL sync code transmission, (1, 2, 4, 8) */
  prach_cfg_ptr->max_sync_ul_tx = tdsMaxSynUlTrans[ie_lcr_ptr->sync_UL_Info.max_SYNC_UL_Transmissions];

  /* Maximum number of synchronisation attempts 10.3.6.78a (1...32)*/
  /* This should go to MAC */
  /* Maximum number of RACH preamble ramping cycles */
  /* Get the Max # of RACH preamble ramping cycles */
  if((ie_lcr_ptr->sync_UL_Info.mmax >= TDSRRCLLC_MIN_MMAX) &&
     (ie_lcr_ptr->sync_UL_Info.mmax <= TDSRRCLLC_MAX_MMAX)
    )
  {
    tdsordered_config_ptr->mac_ul_parms.rach_tx_cntl_info.mmax = ie_lcr_ptr->sync_UL_Info.mmax;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Max # of Preamble Cyles %d is out of range",ie_lcr_ptr->sync_UL_Info.mmax);
    return FAILURE;
  }

  /* Is it possible for UE support a less number than 8? In that case, we need check the maximal n here. */
  /* The number of PRACH-FPACH pairs (maximum pairs=8) */
  if (TDSL1_UL_PRACH_FPACH_MAX < ie_lcr_ptr->prach_DefinitionList.n)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unsupport prach_DefinitionList number is %d in SIB,the UE support number %d ",ie_lcr_ptr->prach_DefinitionList.n,TDSL1_UL_PRACH_FPACH_MAX);
    return FAILURE;
  }
  prach_cfg_ptr->num_prach_fpach = ie_lcr_ptr->prach_DefinitionList.n;
  /* PRACH and FPACH mapping info
   * See 25.331 v9.1.0, 10.3.6.52
   */
  for (loop_i = 0; loop_i < prach_cfg_ptr->num_prach_fpach; loop_i++)
  {
    /* PRACH time slot number */
    prach_cfg_ptr->prach_fpach_info[loop_i].prach_ts_num = ie_lcr_ptr->prach_DefinitionList.elem[loop_i].timeslot;

    /* The number of PRACH channelisation codes */
    prach_cfg_ptr->prach_fpach_info[loop_i].num_prach_codes = ie_lcr_ptr->prach_DefinitionList.elem[loop_i].
                                  prach_ChanCodes_LCR.n;
    /* PRACH channelisation code list. See 25.331 v9.1.0, 10.3.6.51a */
    for (loop_j = 0; loop_j < prach_cfg_ptr->prach_fpach_info[loop_i].num_prach_codes; loop_j++)
    {
      /* Spreading factor. (1, 2, 4, 8, 16) for DPCH, (4, 8, 16) for PRACH, (16) for FPACH */
      prach_cfg_ptr->prach_fpach_info[loop_i].prach_chan_codes[loop_j].sf = tdsTddPrachCode_R4[ie_lcr_ptr->
           prach_DefinitionList.elem[loop_i].prach_ChanCodes_LCR.elem[loop_j]][0];
      /* Channelisation code (1..16) */
      prach_cfg_ptr->prach_fpach_info[loop_i].prach_chan_codes[loop_j].chan_code = tdsTddPrachCode_R4[ie_lcr_ptr->
           prach_DefinitionList.elem[loop_i].prach_ChanCodes_LCR.elem[loop_j]][1];
    }
    /* PRACH midamble shift and burst. See 25.331 v9.1.0, 10.3.6.41 */
    switch (ie_lcr_ptr->prach_DefinitionList.elem[loop_i].midambleShiftAndBurstType.midambleAllocationMode.t)
    {
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_defaultMidamble:
        /*Midamble allocation mode. (10.3.6.41)*/
        prach_cfg_ptr->prach_fpach_info[loop_i].midamble_and_burst.allocation_mode 
             = TDSL1_DEFAULT_MIDAMBLE;
        break;
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_commonMidamble:
        prach_cfg_ptr->prach_fpach_info[loop_i].midamble_and_burst.allocation_mode 
             = TDSL1_COMMON_MIDAMBLE;
        break;
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble:
        prach_cfg_ptr->prach_fpach_info[loop_i].midamble_and_burst.allocation_mode 
             = TDSL1_UE_SPECIFIC_MIDAMBLE;
        /*Integer (0..15).*/
        prach_cfg_ptr->prach_fpach_info[loop_i].midamble_and_burst.shift 
             = ie_lcr_ptr->prach_DefinitionList.elem[loop_i].midambleShiftAndBurstType
                            .midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error midambleAllocationMode is included");
        return FAILURE;
    }
    /*Midamble configuration, as defined in TS 25.221. Actual value midambleConfiguration = IE value * 2*/
    prach_cfg_ptr->prach_fpach_info[loop_i].midamble_and_burst.config = 
          tdsMidambleConfiguration[ie_lcr_ptr->prach_DefinitionList.elem[loop_i]
          .midambleShiftAndBurstType.midambleConfiguration];

    /* FPACH info. See 25.331 v9.1.0, 10.3.6.35a */
    /* FPACH timeslot number (0..6) */
    prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.fpach_ts_num = 
         ie_lcr_ptr->prach_DefinitionList.elem[loop_i].fpach_Info.timeslot;

    /* FPACH channelisation code */
    /* Spreading factor. (1, 2, 4, 8, 16) for DPCH, (4, 8, 16) for PRACH, (16) for FPACH */
    prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.fpach_chan_code.sf = TDSL1_SF_16;
    /* Channelisation code (1..16), in msg is (0...15) */
    prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.fpach_chan_code.chan_code = (uint8)(ie_lcr_ptr->
         prach_DefinitionList.elem[loop_i].fpach_Info.channelisationCode + 1);

    /* FPACH midamble shift and burst. See 25.331 v9.1.0, 10.3.6.41 */
    switch (ie_lcr_ptr->prach_DefinitionList.elem[loop_i].fpach_Info.midambleShiftAndBurstType.midambleAllocationMode.t)
    {
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_defaultMidamble:
        /*Midamble allocation mode. (10.3.6.41)*/
        prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.midamble_and_burst.allocation_mode 
             = TDSL1_DEFAULT_MIDAMBLE;
        break;
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_commonMidamble:
        prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.midamble_and_burst.allocation_mode 
             = TDSL1_COMMON_MIDAMBLE;
        break;
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble:
        prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.midamble_and_burst.allocation_mode 
             = TDSL1_UE_SPECIFIC_MIDAMBLE;
        /*Integer (0..15).*/
        prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.midamble_and_burst.shift 
             = ie_lcr_ptr->prach_DefinitionList.elem[loop_i].fpach_Info.midambleShiftAndBurstType
                            .midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error midambleAllocationMode is included");
        return FAILURE;
    }
    /*Midamble configuration, as defined in TS 25.221. Actual value midambleConfiguration = IE value * 2*/
    prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.midamble_and_burst.config =
          tdsMidambleConfiguration[ie_lcr_ptr->prach_DefinitionList.elem[loop_i].fpach_Info
          .midambleShiftAndBurstType.midambleConfiguration];

    /* WAIT TIME. The number of sub-frames to wait for FPACH ACK, following
     * the sub-frame in which the SYNC_UL is transmitted. (1..4) */
    prach_cfg_ptr->prach_fpach_info[loop_i].fpach_info.wt = ie_lcr_ptr->
         prach_DefinitionList.elem[loop_i].fpach_Info.wi;
  }

  /* finished copying all IEs successfully */
  return SUCCESS;

} /* end prach_rach_info() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_prach_partitioning()

DESCRIPITON     This function takes a pointer to the PRACH Partitioning
                info IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    The current Ordered Config data is overwritten.
                The num_of_asc is updated to reflect the number of
                asc's rcv'd over the air.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_prach_partitioning
(
  tdsl1_ul_asc_struct_type  *tdsasc_info,
  tdsl1_ul_prach_cfg_struct_type  *prach_cfg_ptr,
  tdsrrc_PRACH_Partitioning_LCR_r4   *asc_ptr,
  uint8                    *num_of_asc
)
{
  /* Loop counter */
  uint32 i, n_index;

  n_index = 0;

  while((asc_ptr->n > n_index) && (*num_of_asc <= MAX_ASC) && (*num_of_asc > 0))
  {
    /* save the pointer */
    prach_cfg_ptr->asc_cfg.asc_info_ptr[(*num_of_asc)-1] =
        &tdsasc_info[(*num_of_asc)-1];

    if (asc_ptr->elem[n_index].m.accessServiceClass_TDD_LCRPresent)
    {
      if (asc_ptr->elem[n_index].accessServiceClass_TDD_LCR.m.availableSYNC_UlCodesIndicsPresent)
      {
        /* Indicate what UL sync codes are available */
        if (8 == asc_ptr->elem[n_index]
                     .accessServiceClass_TDD_LCR.availableSYNC_UlCodesIndics.numbits)
        {
          tdsasc_info[(*num_of_asc)-1].avail_ul_sync_code_mask = asc_ptr->elem[n_index]
             .accessServiceClass_TDD_LCR.availableSYNC_UlCodesIndics.data[0];
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error accessServiceClass_TDD_LCR.availableSYNC_UlCodesIndics config.");
          return(FAILURE);
        }
      }
      else
      {
        /* Default is that all SYNC_UL codes defined in SYNC_UL Info are available. */
        tdsasc_info[(*num_of_asc)-1].avail_ul_sync_code_mask = 0xFF;
      }

      /* The number of sub-channels (1, 2, 4, 8) */
      switch (asc_ptr->elem[n_index].accessServiceClass_TDD_LCR.subchannelSize.t)
      {
        case T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size1:
          tdsasc_info[(*num_of_asc)-1].sub_chan_size = TDSRRC_ASC_SUB_CHAN_SIZE_1;
          /* Default value of Size1 is that all subchannels within the size are available for the ASC. MD bit = 11111111 */
          /* tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 0xFF; */
          /* In this case, since there is only one RACH, the value is 1, if RRC has to set it. */
          tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 1;
          break;
        case T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size2:
          tdsasc_info[(*num_of_asc)-1].sub_chan_size = TDSRRC_ASC_SUB_CHAN_SIZE_2;
          /* Indicate what sub-channels are available */
          if (asc_ptr->elem[n_index].accessServiceClass_TDD_LCR.subchannelSize.u.size2->m.subchannelsPresent)
          {
            if (tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannels_subch0 == 
                 asc_ptr->elem[n_index].accessServiceClass_TDD_LCR.subchannelSize.u.size2->subchannels)
            {
              /* position 0 is valid */
              tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 1;
            }
            else
            {
              /* position 1 is valid */
              tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 2;
            }
          }
          else
          {
            /* Default value of the IE is that all subchannels within the size are available for the ASC. MD bit = 11 */
            tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 3;
          }
          break;
        case T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size4:
          tdsasc_info[(*num_of_asc)-1].sub_chan_size = TDSRRC_ASC_SUB_CHAN_SIZE_4;
          /* Indicate what sub-channels are available */
          if (asc_ptr->elem[n_index].accessServiceClass_TDD_LCR.subchannelSize.u.size4->m.subchannelsPresent)
          {
            tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 
                 asc_ptr->elem[n_index].accessServiceClass_TDD_LCR.subchannelSize.u.size4->subchannels.data[0] >> TDSRRC_MAX_NUMBITS_4;
          }
          else
          {
            /* Default value of the IE is that all subchannels within the size are available for the ASC. MD bit = 1111 */
            tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 0x0F;
          }
          break;
        case T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size8:
          tdsasc_info[(*num_of_asc)-1].sub_chan_size = TDSRRC_ASC_SUB_CHAN_SIZE_8;
          if (asc_ptr->elem[n_index].accessServiceClass_TDD_LCR.subchannelSize.u.size8->m.subchannelsPresent)
          {
            tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 
                 asc_ptr->elem[n_index].accessServiceClass_TDD_LCR.subchannelSize.u.size8->subchannels.data[0];
          }
          else
          {
            /* Default value of the IE is that all subchannels within the size are available for the ASC. MD bit = 11111111 */
            tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 0xFF;
          }
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error accessServiceClass_TDD_LCR.subchannelSize type config.");
          return(FAILURE);
      }
    } /* end if ASC present */
    else
    {
      /* Access Service Class data is absent for this ASC */
      /* Set default values for the 1st Access Service Class */
      if(*num_of_asc == 1)
      {
        /* -- Default value is previous ASC
         * -- If this is the first ASC, the default value is all available SYNC_UL codes and
         * -- all available sub-channels with subchannelSize=size1
         */
        /* SYNC_UL codes */
        tdsasc_info[(*num_of_asc)-1].avail_ul_sync_code_mask = 0xFF;

        /* means all ul_sub_chan are valid */
        tdsasc_info[(*num_of_asc)-1].sub_chan_size = TDSRRC_ASC_SUB_CHAN_SIZE_1;
        /* In this case, since there is only one RACH, the value is 1, if RRC has to set it. */
        tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask = 1;

        TDSRRC_MSG2(MSG_LEGACY_LOW,"what UL sync codes are available for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].avail_ul_sync_code_mask);
        TDSRRC_MSG2(MSG_LEGACY_LOW,"The number of sub-channels for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].sub_chan_size);
        TDSRRC_MSG2(MSG_LEGACY_LOW,"what sub-channels are available for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask);
      }
      else
      {
        /* For any ASC other than ASC0, copy data from previous Access Service Class */
        tdsasc_info[(*num_of_asc)-1].avail_ul_sync_code_mask =
                tdsasc_info[(*num_of_asc)-2].avail_ul_sync_code_mask;
        tdsasc_info[(*num_of_asc)-1].sub_chan_size =
                tdsasc_info[(*num_of_asc)-2].sub_chan_size;
        tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask =
                tdsasc_info[(*num_of_asc)-2].avail_ul_sub_chan_mask;

        TDSRRC_MSG2(MSG_LEGACY_LOW,"what UL sync codes are available for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].avail_ul_sync_code_mask);
        TDSRRC_MSG2(MSG_LEGACY_LOW,"The number of sub-channels for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].sub_chan_size);
        TDSRRC_MSG2(MSG_LEGACY_LOW,"what sub-channels are available for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask);
      }
    }

    /* get the next element */
    (*num_of_asc)++;
    n_index++;
  } /* end while getting all Partioning Mode info */

  /* Reset the number of Access Service class back to correct value */
  (*num_of_asc)--;

  /* -- TABULAR: If only "NumASC+1" (with, NumASC+1 < maxASC) ASCSetting-TDD-LCR-r4 are listed,
   * -- the remaining (NumASC+2 through maxASC) ASCs are unspecified.
   */

  /* Fill out to the end of the array with copies of the last value */
  for(i = *num_of_asc; i < MAX_ASC; i++)
  {
    tdsasc_info[i].avail_ul_sync_code_mask =
            tdsasc_info[(*num_of_asc)-1].avail_ul_sync_code_mask;
    tdsasc_info[i].sub_chan_size =
            tdsasc_info[(*num_of_asc)-1].sub_chan_size;
    tdsasc_info[i].avail_ul_sub_chan_mask =
            tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask;

    TDSRRC_MSG2(MSG_LEGACY_LOW,"what UL sync codes are available for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].avail_ul_sync_code_mask);
    TDSRRC_MSG2(MSG_LEGACY_LOW,"The number of sub-channels for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].sub_chan_size);
    TDSRRC_MSG2(MSG_LEGACY_LOW,"what sub-channels are available for ASC %d is %d",(*num_of_asc),tdsasc_info[(*num_of_asc)-1].avail_ul_sub_chan_mask);
  }

  /* We now have the number of ASCs supported in num_of_asc */
  return SUCCESS;

} /* end tdsrrcllcpcie_get_prach_partitioning */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_persist_scaling_default_factors()

DESCRIPITON     This function sets default values of persistence
                scaling factors for all ASCs.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
====================================================================*/
void  tdsrrcllcpcie_get_persist_scaling_default_factors(tdsordered_config_type *config_ptr)
{
  uint8 index = 0;
  do
  {
    config_ptr->psf[index] = TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
    index++;
  }while(index < MAX_ASC_PERSIST);
} /*  tdsrrcllcpcie_get_persist_scaling_default_factors */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_persist_scaling_factors()

DESCRIPITON     This function takes a pointer to the Persistence Scaling
                Factor info IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    Dynamic Persistence Values from SIB 7 are then needed
                to correctly calculate the Persistence Value needed
                by MAC.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_persist_scaling_factors
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_PersistenceScalingFactorList  *ie_ptr
)
{
  /* local variables */
  uint32 index = 0, n_index = 0;
  tdsrrc_PersistenceScalingFactorList  *psflist_ptr = ie_ptr;

  if(psflist_ptr == NULL)
  {
    ERR_FATAL("Null pointer received",0,0,0);
  }
  

  n_index = 0;
  while((psflist_ptr->n > n_index) && (index < MAX_ASC_PERSIST))
  {
    /*
    * All the Persistence scaling factors a real numbers in the range
    * from 0.2 to 0.9. The stored values are stored in the ordered config
    * data base according to the following equation:
    *
    * tdsordered_config psf values = (rcv'd values * 10).
    *
    */
    /* Since this is sent as an enumerated type, time to do the mapping */
    switch(psflist_ptr->elem[n_index])
    {
      case tdsrrc_PersistenceScalingFactor_psf0_2:
        config_ptr->psf[index] = (uint32) 2;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_3:
        config_ptr->psf[index] = (uint32) 3;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_4:
        config_ptr->psf[index] = (uint32) 4;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_5:
        config_ptr->psf[index] = (uint32) 5;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_6:
        config_ptr->psf[index] = (uint32) 6;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_7:
        config_ptr->psf[index] = (uint32) 7;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_8:
        config_ptr->psf[index] = (uint32) 8;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_9:
        config_ptr->psf[index] = (uint32) 9;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Persistance Scaling Factor enum %d",psflist_ptr->elem[n_index]);
        return FAILURE;
    } /* end switch */

    /*
    * Note that the first 2 values are calculated outside of this loop, since they
    * are known values.
    */
    /* get the next element */
    n_index++;
    index++;
  } /* end while MAX_ASC_PERSIST */

  /* Set the default value for the rest of ASCs */
  if(index < MAX_ASC_PERSIST)
  {
    do
    {
      config_ptr->psf[index] = TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
      index++;
    }while(index < MAX_ASC_PERSIST);
  }

  return SUCCESS;
} /* end tdsrrcllcpcie_get_persist_scaling_factors() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_ac_to_asc_mapping()

DESCRIPITON     This function takes a pointer to the AC to ASC mapping
                info IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    Previous values in Ordered Config are overwritten.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ac_to_asc_mapping
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_AC_To_ASC_MappingTable        *ie_ptr
)
{
  /* local variables */
  uint32 index = 0,  n_index = 0;
  tdsrrc_AC_To_ASC_MappingTable   *ascmplist_ptr = ie_ptr;

  /**********************************************************************/
  /* The Access service class mapping follows the table below:          */
  /* ------------------------------------------------------------------ */
  /* |   AC   | 0 - 9 |   10  |   11  |   12  |   13  |   14  |   15  | */
  /* |--------|-------|-------|-------|-------|-------|-------|-------| */
  /* |  ASC   | 1stIE | 2ndIE | 3rdIE | 4thIE | 5thIE | 6thIE | 7thIE | */
  /* ------------------------------------------------------------------ */
  /**********************************************************************/
  /* In other words if the UE supports only AC 0-9 then it selects the  */
  /* Access Service Class given in the first array element.             */
  /* If the UE only supports Access Class 15, then the UE selects the   */
  /* Access Service Class given in the last array element.              */
  /* The IE values are just stored here, and the RRC Connection         */
  /* Establishment procedure must do this mapping correctly.            */
  /**********************************************************************/
  if(ascmplist_ptr == NULL)
  {
    ERR_FATAL("NULL pointer received",0,0,0);
  } 


  n_index = 0;
  
  while((ascmplist_ptr->n > n_index  ) && (index < MAX_ASC_MAP))
  {
    if(ascmplist_ptr->elem[n_index] <= TDSRRCLLC_MAX_AC_TO_ASC_MAPPING)
    {
      config_ptr->ac_to_asc_mapping[index] = (uint8)ascmplist_ptr->elem[n_index];
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"AC to ASC Mapping value %d is out of range",ascmplist_ptr->elem[n_index]);
      return(FAILURE);
    }

    /* Get the next element */
    n_index++;
    index++;
  } /* end while */

  return SUCCESS;
} /* end tdsrrcllcpcie_get_ac_to_asc_mapping() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_select_asc_from_ac()

DESCRIPITON     This function selects the ASC from the UE's access
                service class. It uses the AC to ASC mapping to come up
                with the chosen ASC.

DEPENDENCIES    AC to ASC mapping should be present.
                UE's Access class should be available.

RETURN VALUE    None

SIDE EFFECTS    None
====================================================================*/
void  tdsrrcllcpcie_select_asc_from_ac(tdsordered_config_type *config_ptr)
{
  /* loop index variables */
  uint8 ac_map_index;
  uint8 ac_index;

  /* mask to test which hi access classes are set */
  rrc_hi_access_class_type ac_mask;

  if(tdsrrcrce_get_rrc_est_cause() == RRC_EST_EMERGENCY_CALL) 
  {
    config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_asc =
      0;
  }
  /* If we have no low access class, then we use the second mapping in the
     table.  This is the special "Access Class 10" case referred to in
     section 4.4 of 22.011. */
  else if(tdsrrc_lo_access_class == RRC_LO_ACCESS_CLASS_ABSENT)
  {
    config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_asc =
      config_ptr->ac_to_asc_mapping[1];
  }
  /* If we only have a low access class (0-9), then we always use the first
     mapping in the table, as per section 8.5.13 of 25.331. */
  else if(tdsrrc_hi_access_class == RRC_HI_ACCESS_CLASS_ABSENT)
  {
    config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_asc =
      config_ptr->ac_to_asc_mapping[0];
  }
  /* Otherwise, we use the mapping for the highest-numbered access class, as
     per section 8.5.13 of 25.331. */
  else
  {
    ac_map_index = MAX_ASC_MAP - 1;
    ac_mask = RRC_ACCESS_CLASS_15_BIT;
    for(ac_index = (uint8)TDSRRC_INT_ACCESS_CLASS_15_INDEX;
        ac_index >= (uint8)TDSRRC_INT_ACCESS_CLASS_11_INDEX;
        ac_index--, ac_map_index--, ac_mask >>= 1)
    {
      if(tdsrrc_hi_access_class & ac_mask)
      {
        config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_asc =
          config_ptr->ac_to_asc_mapping[ac_map_index];
        break;
      }
    }
  }
}

/*====================================================================
FUNCTION      tdsrrcllc_get_max_allow_ul_tx_pwr()

DESCRIPTION   This function stores the maximum allowed Uplink Tx Power,
              and stores it in TDSORDERED_CONFIG
              It checks to make sure it is in range.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_max_allow_ul_tx_pwr
(
  tdsrrc_MaxAllowedUL_TX_Power max_ul_tx_pwr
)
{

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get Max Allowed UL Tx Pwr");

  if((max_ul_tx_pwr >= TDSRRCLLC_MIN_ALLOW_UL_TX_PWR) &&
     (max_ul_tx_pwr <= TDSRRCLLC_MAX_ALLOW_UL_TX_PWR)
    )
  {
    tdsordered_config_ptr->max_tx_pwr = (int8) max_ul_tx_pwr;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcv'd invalid UL TX PWR %d",max_ul_tx_pwr);
    return FAILURE;
  }

  /* The presence of a valid value for MAX UL Power implies L1 needs to
   * be reconfigured.
   */
  /*RRC_GKG: If Physical channel IE "maxAllowedUL-TX-Power" is give in the OTA, 
  it means L1 UL needs to be configured.*/
  tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;

  return SUCCESS;
} /* end tdsrrcllc_get_max_allow_ul_tx_pwr() */

/*====================================================================
FUNCTION      tdsrrcllc_get_ul_dpch_info()

DESCRIPTION   This function translates the Uplink DPCH info common for
              all RL (radio links) IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_dpch_info
(
  tdsrrc_UL_DPCH_Info *ie_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL DPCH Info");

  /* Get the UL DPCH power control information if present */
  if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
             ul_DPCH_PowerControlInfoPresent))
  {
    if(tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info(&ie_ptr->ul_DPCH_PowerControlInfo) == FAILURE)
    {
      return FAILURE;
    }
  }
/* add comment for 10.3.6.88 begin(r3 del) */
#if 0
  /* Make sure of FDD and get the right parameters */
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
              T_tdsrrc_UL_DPCH_Info_modeSpecificInfo_fdd))
  {
    /* Get the Uplink DPCH Scrambling Code Type */
    switch (ie_ptr->modeSpecificInfo.u.fdd->scramblingCodeType)
    {
      case tdsrrc_ScramblingCodeType_shortSC:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.scrambling_code_type = TDSL1ULCFG_SC_SHORT;
        break;
      case tdsrrc_ScramblingCodeType_longSC:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.scrambling_code_type = TDSL1ULCFG_SC_LONG;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL DPCH Scrambling Code Type %d is invalid",ie_ptr->modeSpecificInfo.u.fdd->scramblingCodeType);
        return FAILURE;
    }

    /* Get the scrambling code */
    if(ie_ptr->modeSpecificInfo.u.fdd->scramblingCode <= TDSRRCLLC_MAX_UL_SCR_CODE)
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.cpid = (uint32)
        ie_ptr->modeSpecificInfo.u.fdd->scramblingCode;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL Scrambling Code %d is out of range",ie_ptr->modeSpecificInfo.u.fdd->scramblingCode);
      return FAILURE;
    }

    /* Get the number of DPDCHs allowed */
    /* There is a mandatory default value of 1; therefore, the need to check for
       it's presence */

      if((ie_ptr->modeSpecificInfo.u.fdd->numberOfDPDCH <= MAX_DPDCH) &&
         (ie_ptr->modeSpecificInfo.u.fdd->numberOfDPDCH >= TDSRRCLLC_DEFAULT_NUM_DPDCH_ALLOWED))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.num_dpdch = (uint8)
          ie_ptr->modeSpecificInfo.u.fdd->numberOfDPDCH;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"# of UL DPDCHs %d is out of range",ie_ptr->modeSpecificInfo.u.fdd->numberOfDPDCH);
        return FAILURE;
      }


    /* Get the spreading factor */
    switch(ie_ptr->modeSpecificInfo.u.fdd->spreadingFactor)
    {
      case tdsrrc_SpreadingFactor_sf4:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.sf_min = TDSL1_SF_4;
        break;
      case tdsrrc_SpreadingFactor_sf8:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.sf_min = TDSL1_SF_8;
        break;
      case tdsrrc_SpreadingFactor_sf16:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.sf_min = TDSL1_SF_16;
        break;
      case tdsrrc_SpreadingFactor_sf32:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.sf_min = TDSL1_SF_32;
        break;
      case tdsrrc_SpreadingFactor_sf64:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.sf_min = TDSL1_SF_64;
        break;
      case tdsrrc_SpreadingFactor_sf128:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.sf_min = TDSL1_SF_128;
        break;
      case tdsrrc_SpreadingFactor_sf256:
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.sf_min = TDSL1_SF_256;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Spreading Factor %d received",ie_ptr->modeSpecificInfo.u.fdd->spreadingFactor);
        return FAILURE;
    } /* end spreading factor switch */

    /* get the TFCI existence */
    if(ie_ptr->modeSpecificInfo.u.fdd->tfci_Existence)
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tfci_present = TRUE;
    }
    else
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tfci_present = FALSE;
    }

    /* get the FBI bits if necessary */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.fdd,
               numberOfFBI_BitsPresent))
    {
      switch(ie_ptr->modeSpecificInfo.u.fdd->numberOfFBI_Bits)
      {
        case 1:
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.fbi_len = TDSL1ULCFG_FBI_LEN_1;
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid # of FBI bits %d received",ie_ptr->modeSpecificInfo.u.fdd->numberOfFBI_Bits);
          return FAILURE;
      }
    }
    else
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.fbi_len = TDSL1ULCFG_FBI_LEN_0;
    }

    /*
    * Get the UL DPCH Puncturing Limit. The OTA Msg receives this value as an enum.
    * The puncturing limit is a real number between 0.4 and 1.0 in steps of 0.04.
    * L1 expects this value to be from 40 to 100; in other words, L1 expects
    * l1_puncturing_limit = puncturing_limit * 100;
    * Therefore in the ASN.1 enum type the first enum value 0 corresponds to 0.4,
    * 1 corresponds to 0.44, etc... . Hence the following equation is derived:
    * l1_puncturing_limit = (ASN.1_enum_type * 4) + 40;
    * If this enum changes, this equation may need tweaking or else, this can
    * be broken into a switch with multiple cases.
    */
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.punct_lim = (uint8)
      (((uint8)(ie_ptr->modeSpecificInfo.u.fdd->puncturingLimit) * 4) + 40);

    if((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.punct_lim < TDSRRCLLC_MIN_PUNCTURING_LIMIT)
      || (tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.punct_lim > TDSRRCLLC_MAX_PUNCTURING_LIMIT))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL DPCH puncturing limit %d is out of range",tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.punct_lim);
      return FAILURE;
    }

#ifdef FEATURE_TDSCDMA_CPC_DTX
    /*Intialize numtpc to 2 Since this  I.E will only come as part of Rel-7 OTA message*/
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.numtpc = L1_UL_NUM_TPC_BITS_2;
#endif

  } /* end FDD if */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No FDD UL DPCH information present");
    return FAILURE;
  }
#endif
/* add comment for 10.3.6.88 end */
  /* If made it here, all is well */
  /* Indicate that Uplink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
  return SUCCESS;
} /* end tdsrrcllc_get_ul_dpch_info() */


/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_r5()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r5
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r5 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  boolean is_hho = FALSE;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_DL_InformationPerRL_List_r5 *tmp_ie_ptr = dl_info_per_rl_list_ptr;
  tdsrrc_state_e_type curr_rrc_state = tdsrrc_get_state();

  uint8               cpid = TDSRRC_INVALID_CPID;

  /* If state from CELL_DCH/CELL_FACH/CONNECTING to CELL_DCH */
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"curr_rrc_state = %d",curr_rrc_state);
    cpid = (uint8)tdsrrcccm_get_curr_camped_cpid();

    /* If two FreqInfo are included in the OTA */
    if ((tdsordered_config_ptr->p_freq_present)&&(tdsordered_config_ptr->s_freq_present))
    {
      /*  If the first frequency got changed. */
      if (rrc_freq != tdsordered_config_ptr->p_freq)
      {
        /* It is a inter handover */
        is_hho = TRUE;
        tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different freq %d",tdsordered_config_ptr->p_freq);
      }
      if (FALSE == is_hho)
      {
        if (dl_info_per_rl_list_present)
        {
          if (tmp_ie_ptr == NULL)
          {
            ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
          }
          /*  If CPID got changed. */
          if ((T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd ==
                 tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
                 (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                     tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                     (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
          {
            if(cpid != tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
            {
              /* It is a intra handover */
              is_hho = TRUE;
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
            }
          }
        }
      }
    }
    /* If only one FreqInfo is included in the OTA */
    else if ((tdsordered_config_ptr->p_freq_present)||(tdsordered_config_ptr->s_freq_present))
    {
      if (dl_info_per_rl_list_present)
      {
        if (tmp_ie_ptr == NULL)
        {
          ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
        }
        /*  If CPID got changed. */
        if ((T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd ==
               tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
               (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                   tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                   (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          if(cpid != 
                tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
          {
            /* It is a handover */
            is_hho = TRUE;
            if (tdsordered_config_ptr->p_freq != rrc_freq)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a inter handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a intra handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
            }
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
          }
        }
      }
    }
#ifdef FEATURE_TDSCDMA_SINGLE_FREQ_NO_CPID_HO_SUPPORT

      if (tdsordered_config_ptr->p_freq_present)
      {
        /*  If the first frequency got changed. */
        if (rrc_freq != tdsordered_config_ptr->p_freq)
        {
          /* It is a inter handover */
          is_hho = TRUE;
          tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Support none cpid and one freq info HO. p_freq %d different with current freq %d",tdsordered_config_ptr->p_freq,rrc_freq);
        }
      }

#endif
  }

  if (TRUE == is_hho)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO detected different freq different cell para id ");
  }

  tdsordered_config_ptr->is_hho = is_hho;

  return(SUCCESS);
}


#ifdef FEATURE_TDSCDMA_HSUPA

/*===========================================================================

FUNCTION          tdsrrc_is_eul_active


DESCRIPTION       

  This function returns whether HSUPA / EUL is active or not at a given moment.
  for input, if OC is not set then use tdscurrent_config_ptr, else use tdsordered_config_ptr.

DEPENDENCIES


RETURN VALUE
                  
  TRUE/FALSE

SIDE EFFECTS
                  
  None

===========================================================================*/
boolean tdsrrc_is_eul_active
(
  void
)
{

  tdsordered_config_type *config_ptr;

  if( tdsconfig_ptr_in_use == TDSCURRENT_CONFIG)
  {
    config_ptr = tdscurrent_config_ptr;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Using cc to get edch transmission");
  }
  else 
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"Using oc to get edch transmission");
    config_ptr = tdsordered_config_ptr;
  }
  if(config_ptr == NULL )
    return FALSE;
  return(config_ptr->e_dch_transmission);
}

#endif /* FEATURE_TDSCDMA_HSUPA */

/* tdsget_ul_dpch_info_per_r4_rrc_connection merge to tdsrrcllc_get_ul_dpch_info_r4 */

/*====================================================================
FUNCTION      tdsrrcllc_set_meas_feedback_info()

DESCRIPTION   This function sets the Meas Feedback Information in
              L1 structure in OC.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/

uecomdef_status_e_type tdsrrcllc_set_meas_feedback_info
(
  tdsrrc_Measurement_Feedback_Info* meas_feedback_ptr
)
{
  if (TDSRRC_CHECK_MSG_TYPE(meas_feedback_ptr->modeSpecificInfo,
              T_tdsrrc_Measurement_Feedback_Info_modeSpecificInfo_tdd))
  {
    /* choice TDD, Measurement Feedback Info no data */

  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid Meas Feedback Info");
    return FAILURE;
  }
  return SUCCESS;

}
/*====================================================================
FUNCTION      tdsrrcllc_set_hspdsch_midamble_configuration_tdd128()

DESCRIPTION   This function get hspdsch_midamble_configuration info.

DEPENDENCIES  None

RETURN VALUE  void

SIDE EFFECTS  None.
====================================================================*/

void tdsrrcllc_set_hspdsch_midamble_configuration_tdd128
(
  tdsrrc_HS_PDSCH_Midamble_Configuration_TDD128 *midamble_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,uint8 multiCarrier_flg
#endif
)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  if (multiCarrier_flg)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MultiCarrier present,ignore midamble configuration");
    return;
  }
#endif

  tdsordered_config_ptr->l1_hsdpa_info.hs_pdsch_midamble_cfg.allocation_mode = 
    tdsMidambleAllocMode[midamble_ptr->midambleAllocationMode.t];

  /* Get midamble_shift */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(midamble_ptr->midambleAllocationMode,
      rrc_HS_SCCH_TDD128_midambleAllocationMode_ueSpecificMidamble))
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_pdsch_midamble_cfg.shift =
        midamble_ptr->midambleAllocationMode.u.ueSpecificMidamble;
  }

  /* Get midamble cfg */
  tdsordered_config_ptr->l1_hsdpa_info.hs_pdsch_midamble_cfg.config = 
    tdsMidambleConfiguration[midamble_ptr->midambleConfiguration];

}

/*====================================================================
FUNCTION      tdsrrcllc_check_if_hspdsch_midamble_changed()

DESCRIPTION   This function check hspdsch_midamble_configuration info.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/

boolean tdsrrcllc_check_if_hspdsch_midamble_changed
(
  tdsrrc_HS_PDSCH_Midamble_Configuration_TDD128 *midamble_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg
#endif
)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  if (multicarrier_flg)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MultiCarrier present,don't check midamble");
    return FALSE;
  }
#endif

  /* Check midambleAllocationMode */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_pdsch_midamble_cfg.allocation_mode !=
        tdsMidambleAllocMode[midamble_ptr->midambleAllocationMode.t])
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"midambleAllocationMode is different");
    return TRUE;
  }

  /* Check midamble_shift */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(midamble_ptr->midambleAllocationMode,
      rrc_HS_SCCH_TDD128_midambleAllocationMode_ueSpecificMidamble))
  {
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_pdsch_midamble_cfg.shift != 
          midamble_ptr->midambleAllocationMode.u.ueSpecificMidamble)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"midamble_shift is different");
      return TRUE;
    }
  }

  /* Check midambleConfiguration */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_pdsch_midamble_cfg.config !=
        tdsMidambleConfiguration[midamble_ptr->midambleConfiguration])
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"midambleConfiguration is different");
    return TRUE;
  }

  return FALSE;
}

/*====================================================================
FUNCTION      tdsrrcllc_check_if_hsscch_changed_r5()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllc_check_if_hsscch_changed_r5
(
  tdsrrc_HS_SCCH_Info* scch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg
#endif
)
{
  uint8 cnt = 0;
  tdsrrc_HS_SCCH_Info_tdd128   *local_scch_ptr = NULL;

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  if (multicarrier_flg)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MultiCarrier present,don't check hsscch info");
    return FALSE;
  }
#endif

  if (TDSRRC_CHECK_MSG_TYPE(scch_info_ptr->modeSpecificInfo,
               T_tdsrrc_HS_SCCH_Info_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:FDD selected. Bail out");
    return FALSE;
  }
  if (!TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(scch_info_ptr->modeSpecificInfo.u.tdd,
               rrc_HS_SCCH_Info_tdd_tdd128))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:not TDD128 selected. Bail out");
    return FALSE;
  }
  
  local_scch_ptr = (tdsrrc_HS_SCCH_Info_tdd128*)
                    scch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128;

  /* Check hsscch num */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich != 
      local_scch_ptr->n)
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"num_hs_scch_sich is different");
    return TRUE;
  }

  for (cnt = 0; cnt < local_scch_ptr->n; cnt++)
  {
    /* Check timeslotNumber */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].ts_num_scch !=
      local_scch_ptr->elem[cnt].timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"timeslotNumber is different");
      return TRUE;
    }

    /* Check firstChannelisationCode */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_1st !=
      local_scch_ptr->elem[cnt].firstChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"firstChannelisationCode is different");
      return TRUE;
    }

    /* Check secondChannelisationCode */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_2nd !=
      local_scch_ptr->elem[cnt].secondChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"secondChannelisationCode is different");
      return TRUE;
    }

    /* Check midambleAllocationMode */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.allocation_mode !=
          tdsMidambleAllocMode[local_scch_ptr->elem[cnt].midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"midambleAllocationMode is different");
      return TRUE;
    }

    /* Check midamble_shift */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(local_scch_ptr->elem[cnt].midambleAllocationMode,
        rrc_HS_SCCH_TDD128_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.shift != 
            local_scch_ptr->elem[cnt].midambleAllocationMode.u.ueSpecificMidamble)
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"midamble_shift is different");
        return TRUE;
      }
    }

    /* Check midambleConfiguration */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.config !=
          tdsMidambleConfiguration[local_scch_ptr->elem[cnt].midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"midambleConfiguration is different");
      return TRUE;
    }

    /* Check bler_target */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].bler_target !=
          local_scch_ptr->elem[cnt].bler_target)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"bler_target is different");
      return TRUE;
    }

    /* Check HS-SICH configuration */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.ts_num_sich !=
          local_scch_ptr->elem[cnt].hs_sich_configuration.timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"hs_sich_configuration timeslotNumber is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.sich_chcode !=
          local_scch_ptr->elem[cnt].hs_sich_configuration.channelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"hs_sich_configuration channelisationCode is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.allocation_mode !=
          tdsMidambleAllocMode[local_scch_ptr->elem[cnt].hs_sich_configuration.midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"hs_sich_configuration midambleAllocationMode is different");
      return TRUE;
    }

    /* Check midamble_shift */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(local_scch_ptr->elem[cnt].hs_sich_configuration.midambleAllocationMode,
        rrc_HS_SCCH_TDD128_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.shift != 
            local_scch_ptr->elem[cnt].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift)
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"hs_sich_configuration midamble_shift is different");
        return TRUE;
      }
    }
    
    /* Check midambleConfiguration */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.config !=
          tdsMidambleConfiguration[local_scch_ptr->elem[cnt].hs_sich_configuration.midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"midambleConfiguration is different");
      return TRUE;
    }

    /* Check nack_ack_power_offset */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.ack_nack_powoffset !=
          local_scch_ptr->elem[cnt].hs_sich_configuration.nack_ack_power_offset)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"nack_ack_power_offset is different");
      return TRUE;
    }

    /* Check power_level_HSSICH */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.prx_sich !=
          local_scch_ptr->elem[cnt].hs_sich_configuration.power_level_HSSICH)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"power_level_HSSICH is different");
      return TRUE;
    }

    /* Check tpc_step_size */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.tpc_ssize !=
          (local_scch_ptr->elem[cnt].hs_sich_configuration.tpc_step_size+1))
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"tpc_step_size is different");
      return TRUE;
    }
  }

  return FALSE;
}

/*====================================================================
FUNCTION      tdsrrcllc_check_if_hsscch_changed_r6()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllc_check_if_hsscch_changed_r6
(
  tdsrrc_HS_SCCH_Info_r6* scch_info_ptr
)
{
  uint8 cnt = 0;
  tdsrrc_HS_SCCH_Info_r6_tdd128   *local_scch_ptr = NULL;
  
  if (TDSRRC_CHECK_MSG_TYPE(scch_info_ptr->modeSpecificInfo,
               T_tdsrrc_HS_SCCH_Info_r6_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:FDD selected. Bail out");
    return FALSE;
  }
  if (!TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(scch_info_ptr->modeSpecificInfo.u.tdd,
               rrc_HS_SCCH_Info_r6_tdd_tdd128))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:not TDD128 selected. Bail out");
    return FALSE;
  }
  
  local_scch_ptr = (tdsrrc_HS_SCCH_Info_r6_tdd128*)
                    scch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128;

  //* Check nack_ack_power_offset info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.ack_nack_powoffset !=
      local_scch_ptr->nack_ack_power_offset)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"nack_ack_power_offset is different");
    return TRUE;
  }

  /* Check PRXHS-SICH info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.prx_sich !=
      local_scch_ptr->power_level_HSSICH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PRXHS_SICH is different");
    return TRUE;
  }

  /* Check TPC step size info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize !=
      (local_scch_ptr->tpc_step_size+1))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TPC step size is different");
    return TRUE;
  }

  /* Check BLER target info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.bler_target !=
      local_scch_ptr->bler_target)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BLER target is different");
    return TRUE;
  }

  /* Check HS-SCCH Set Configuration list same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich !=
        local_scch_ptr->hS_SCCH_SetConfiguration.n)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"l1_dl_hs_scch_sich_config num is different");
    return TRUE;
  }

  for (cnt = 0; cnt < local_scch_ptr->hS_SCCH_SetConfiguration.n; cnt++)
  {
    /* Check timeslotNumber same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].ts_num_scch !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"timeslotNumber is different");
      return TRUE;
    }

    /* Check firstChannelisationCode same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_1st !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].firstChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"firstChannelisationCode is different");
      return TRUE;
    }

    /* Check secondChannelisationCode same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_2nd !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].secondChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"secondChannelisationCode is different");
      return TRUE;
    }

    /* Check midamble info same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.allocation_mode !=
          tdsMidambleAllocMode[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"midambleAllocationMode is different");
      return TRUE;
    }

    /* Check midamble shift same or not */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode,
      rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.shift !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode.u.ueSpecificMidamble)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"midamble_shift is different");
        return TRUE;
      }
    }

    /* Check midamble cfg same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.config !=
          tdsMidambleConfiguration[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"midambleConfiguration is different");
      return TRUE;
    }

    /* Check sich config same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.ts_num_sich !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration timeslotNumber is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.sich_chcode !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.channelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration channelisationCode is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.allocation_mode !=
          tdsMidambleAllocMode[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midambleAllocationMode is different");
      return TRUE;
    }

    if (TDSRRC_CHECK_COMMON_MSG_TYPE(local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode,
      rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.shift !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midamble_shift is different");
        return TRUE;
      }
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.config !=
          tdsMidambleConfiguration[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midambleConfiguration is different");
      return TRUE;
    }
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:SCCH not changed");

  return FALSE;
}

/*====================================================================
FUNCTION      tdsrrcllc_check_if_hsscch_changed_r7()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllc_check_if_hsscch_changed_r7
(
  tdsrrc_HS_SCCH_Info_r7* scch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg
#endif
)
{
  uint8 cnt = 0;
  tdsrrc_HS_SCCH_Info_r7_tdd128   *local_scch_ptr = NULL;

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  if (multicarrier_flg)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MultiCarrier present,don't check hsscch info");
    return FALSE;
  }
#endif

  if (TDSRRC_CHECK_MSG_TYPE(scch_info_ptr->modeSpecificInfo,
               T_tdsrrc_HS_SCCH_Info_r7_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:FDD selected. Bail out");
    return FALSE;
  }
  if (!TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(scch_info_ptr->modeSpecificInfo.u.tdd,
               rrc_HS_SCCH_Info_r7_tdd_tdd128))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:not TDD128 selected. Bail out");
    return FALSE;
  }

  local_scch_ptr = (tdsrrc_HS_SCCH_Info_r7_tdd128*)
                    scch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128;

  /* Check nack_ack_power_offset info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.ack_nack_powoffset !=
      local_scch_ptr->nack_ack_power_offset)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"nack_ack_power_offset is different");
    return TRUE;
  }

  /* Check PRXHS-SICH info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.prx_sich !=
      local_scch_ptr->power_level_HSSICH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PRXHS-SICH is different");
    return TRUE;
  }

  /* Check TPC step size info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize !=
      (local_scch_ptr->tpc_step_size+1))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TPC step size is different");
    return TRUE;
  }

  /* Check BLER target info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.bler_target !=
      local_scch_ptr->bler_target)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BLER target is different");
    return TRUE;
  }

  /* Check Power Control GAP info same or not */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(local_scch_ptr, powerControlGAP))
  {
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap !=
        local_scch_ptr->powerControlGAP)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Power Control GAP is different");
      return TRUE;
    }
  }
  else
  {
    /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
    if (1 != tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Power Control GAP is different");
      return TRUE;
    }
  }

  /* Check Pathloss compensation switch same or not */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(local_scch_ptr, pathlossCompensationSwitch))
  {
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens !=
        local_scch_ptr->pathlossCompensationSwitch)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pathloss compensation switch is different");
      return TRUE;
    }
  }
  else
  {
    /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */
    if (FALSE != tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pathloss compensation switch is different");
      return TRUE;
    }
  }

  /* Check HS-SCCH Set Configuration list same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich !=
        local_scch_ptr->hS_SCCH_SetConfiguration.n)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"l1_dl_hs_scch_sich_config num is different");
    return TRUE;
  }

  for (cnt = 0; cnt < local_scch_ptr->hS_SCCH_SetConfiguration.n; cnt++)
  {
    /* Check timeslotNumber same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].ts_num_scch !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"timeslotNumber is different");
      return TRUE;
    }

    /* Check firstChannelisationCode same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_1st !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].firstChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"firstChannelisationCode is different");
      return TRUE;
    }

    /* Check secondChannelisationCode same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_2nd !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].secondChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"secondChannelisationCode is different");
      return TRUE;
    }

    /* Check midamble info same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.allocation_mode !=
          tdsMidambleAllocMode[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"midambleAllocationMode is different");
      return TRUE;
    }

    /* Check midamble shift same or not */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode,
      rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.shift !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode.u.ueSpecificMidamble)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"midamble_shift is different");
        return TRUE;
      }
    }

    /* Check midamble cfg same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.config !=
          tdsMidambleConfiguration[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"midambleConfiguration is different");
      return TRUE;
    }

    /* Check sich config same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.ts_num_sich !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration timeslotNumber is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.sich_chcode !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.channelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration channelisationCode is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.allocation_mode !=
          tdsMidambleAllocMode[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midambleAllocationMode is different");
      return TRUE;
    }

    if (TDSRRC_CHECK_COMMON_MSG_TYPE(local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode,
      rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.shift !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midamble_shift is different");
        return TRUE;
      }
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.config !=
          tdsMidambleConfiguration[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midambleConfiguration is different");
      return TRUE;
    }
  }

  return FALSE;
}

/*====================================================================
FUNCTION      tdsrrcllc_set_hsscch_info()

DESCRIPTION   This function sets the Channelization code required
              for HSPDSCH in L1 structure in OC. IEs refer to 10.3.6.36a.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_set_hsscch_info
(
  tdsrrc_HS_SCCH_Info* hsscch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg
#endif
)
{
  struct tdsrrc_HS_SCCH_Info_tdd128 *tmp_ptr = NULL;
  uint8 i = 0;

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  if (multicarrier_flg)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MultiCarrier present,ignore hsscch info");
    return SUCCESS;
  }
#endif

  if (TDSRRC_CHECK_MSG_TYPE(hsscch_info_ptr->modeSpecificInfo,
          T_tdsrrc_HS_SCCH_Info_modeSpecificInfo_tdd))
  {
    /* Get HSSCCH info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(hsscch_info_ptr->modeSpecificInfo.u.tdd,
          rrc_HS_SCCH_Info_tdd_tdd128))
    {
      tmp_ptr = hsscch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128;

      /** Set common_scch_sich_params_present info 
             TRUE indicate the current IE using shared ck_nack_powoffset,etc */
      tdsordered_config_ptr->l1_hsdpa_info.common_scch_sich_params_present = FALSE;

      /* Get HS-SCCH Set Configuration list(R5) */
      tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich = tmp_ptr->n;
    
      for (i = 0; i < tmp_ptr->n; i++)
      {
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].ts_num_scch =
            tmp_ptr->elem[i].timeslotNumber;

        /** OC: scch_chcode_1st = 0, L1_SF16_CH_CODE_1;
                    ASN1: firstChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_1st =
            tmp_ptr->elem[i].firstChannelisationCode;

        /** OC: scch_chcode_2nd = 0, L1_SF16_CH_CODE_1;
                     ASN1: secondChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_2nd =
            tmp_ptr->elem[i].secondChannelisationCode;

        /* Get midamble info */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.allocation_mode =
            tdsMidambleAllocMode[tmp_ptr->elem[i].midambleAllocationMode.t];

        if (TDSRRC_CHECK_COMMON_MSG_TYPE(tmp_ptr->elem[i].midambleAllocationMode,
            rrc_HS_SCCH_TDD128_midambleAllocationMode_ueSpecificMidamble))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.shift =
            tmp_ptr->elem[i].midambleAllocationMode.u.ueSpecificMidamble;
        }

        /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
                     ASN1: midambleConfiguration   Integer (1...8), */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.config =
            tdsMidambleConfiguration[tmp_ptr->elem[i].midambleConfiguration];

        /* Get BLER target info */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].bler_target =
            tmp_ptr->elem[i].bler_target;

        /* Get HS-SICH configuration info */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.ts_num_sich =
          tmp_ptr->elem[i].hs_sich_configuration.timeslotNumber;

        /* OC: sich_chcode = 0, L1_SF16_CH_CODE_1;
                     ASN1: channelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1*/
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.sich_chcode =
            tmp_ptr->elem[i].hs_sich_configuration.channelisationCode;

        /* Get midamble info */
        switch (tmp_ptr->elem[i].hs_sich_configuration.midambleAllocationMode.t)
        {
          case T_tdsrrc_HS_SICH_Configuration_TDD128_midambleAllocationMode_defaultMidamble:
              tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
                TDSL1_DEFAULT_MIDAMBLE;
              break;

          case T_tdsrrc_HS_SICH_Configuration_TDD128_midambleAllocationMode_ueSpecificMidamble:
              tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
                TDSL1_UE_SPECIFIC_MIDAMBLE;

              tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.shift =
                tmp_ptr->elem[i].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
              break;
          default:
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"hs_sich_configuration.midambleAllocationMode is invalid");
            return FAILURE;
        }

        /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
                     ASN1: midambleConfiguration   Integer (1...8), */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.config =
            tdsMidambleConfiguration[tmp_ptr->elem[i].hs_sich_configuration.midambleConfiguration];

        /* Get Ack-Nack Power Offset info */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.ack_nack_powoffset =
            tmp_ptr->elem[i].hs_sich_configuration.nack_ack_power_offset;

        /* Get power_level_HSSICH info */
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.prx_sich =
            tmp_ptr->elem[i].hs_sich_configuration.power_level_HSSICH;

        /* Get TPC step size */
        if (tmp_ptr->elem[i].hs_sich_configuration.tpc_step_size < 
              tdsrrc_HS_SICH_Configuration_TDD128_tpc_step_size_spare1)
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.tpc_ssize =
              tmp_ptr->elem[i].hs_sich_configuration.tpc_step_size+1;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TPC Step Size is absent in OTA message");
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config
               .tpc_ssize = TDSL1_TPC_STEP_SIZE_0DB;
        }
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd choice is not tdd128");
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo choice FDD,not TDD");
    return FAILURE;
  }
  return SUCCESS;

}

/*====================================================================
FUNCTION      tdsrrcllc_get_ul_dpch_info_r5()

DESCRIPTION   This function translates the Uplink DPCH info common for
              all RL (radio links) IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_dpch_info_r5
(
  tdsrrc_UL_DPCH_Info_r5 *ie_ptr
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL DPCH Info");

  /* Get the UL DPCH power control information if present */
  if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                 ul_DPCH_PowerControlInfoPresent))
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_DPCH_PowerControlInfo,
                                rrc_UL_DPCH_PowerControlInfo_r5_tdd))
    {
      if(FAILURE == tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r4((tdsrrc_UL_DPCH_PowerControlInfo_r4_tdd *)ie_ptr->ul_DPCH_PowerControlInfo.u.tdd))
      {
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No valid FDD data for UL DPCH Power Control Info");
      return FAILURE;
    }
  }
  else
  {
    /* both in FDD and TDD;
  2> if the IE "Uplink DPCH power control info" is not included in a message used to enter CELL_DCH:
  3> set the variable INVALID_CONFIGURATION to true. */
    if ((tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH) &&
          (rrc_state != TDSRRC_STATE_CELL_DCH))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"no UlDpchPwrInfo from nonDCH to DCH");
      return FAILURE;
    }
  }

  /* Make sure of TDD and get the right parameters */
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_UL_DPCH_Info_r5_modeSpecificInfo_tdd))
  {
    /* Get the UL Timing Advance */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd, ul_TimingAdvancePresent))
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = TRUE;
      TDSRRC_MSG0(MSG_LEGACY_LOW,"This OTA MSG include ul_TimingAdvanceControl");
      if (FAILURE == tdsrrcllcpcie_get_ul_tim_adv_contrl_r4(&ie_ptr->modeSpecificInfo.u.tdd->ul_TimingAdvance))
      {
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"This OTA MSG not include ul_TimingAdvanceControl");
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = FALSE;
    }
    /* Get Uplink CCTrCH LIST from OTA message */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd, ul_CCTrCHListPresent))
    {
      if (1 < ie_ptr->modeSpecificInfo.u.tdd->ul_CCTrCHList.n)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ul_CCTrCHList %d is invalid",(ie_ptr->modeSpecificInfo.u.tdd->ul_CCTrCHList.n));
        return FAILURE;
      }

      if (FAILURE == tdsrrcllcpcie_get_ul_cctrch_list_r4(&ie_ptr->modeSpecificInfo.u.tdd->ul_CCTrCHList))
      {
        return FAILURE;
      }
    }
    /* TDD support only one CCTrCH. If OTA message send ul_CCTrCHListToRemove, RRC consider it invalid */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd, ul_CCTrCHListToRemovePresent))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDD only have one CCTrCH, ul_CCTrCHListToRemove %d is invalid",(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,ul_CCTrCHListToRemovePresent)));
      return FAILURE;
    }
  } /* end TDD if */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No FDD UL DPCH information present");
    return FAILURE;
  }

  /* If made it here, all is well */
  /* Indicate that Uplink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
  return SUCCESS;
} /* end tdsrrcllc_get_ul_dpch_info_r5() */


/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info()

DESCRIPTION   This function translates the Uplink DPCH Power Control
              info common for all RL (radio links) IE into the RRC
              config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info
(
  tdsrrc_UL_DPCH_PowerControlInfo *ie_ptr
)
{
/* add comment for 10.3.6.88 begin(r3 will be del) */
#if 0
  if(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,
              rrc_UL_DPCH_PowerControlInfo_fdd))
  {
    /* Set the valid bit if made it this far */
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.dpch_pwr_ctrl_info.valid = TRUE;

    /* Get the DPCCH Power Offset (value in dB) */
    if((ie_ptr->u.fdd->dpcch_PowerOffset <= TDSRRCLLC_UL_DPCCH_PWR_OFFSET_MAX) &&
       (ie_ptr->u.fdd->dpcch_PowerOffset >= TDSRRCLLC_UL_DPCCH_PWR_OFFSET_MIN))
    {
      /* The value sent over the air needs to be doubled */
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.dpch_pwr_ctrl_info.dpcch_pwr_offset =
        (int16) (ie_ptr->u.fdd->dpcch_PowerOffset * 2);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL DPCCH Power Offset %d is out of range",ie_ptr->u.fdd->dpcch_PowerOffset);
      return FAILURE;
    }

    /* Get the PC Preamble */

    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.dpch_pwr_ctrl_info.pcp_len
      = (uint8)ie_ptr->u.fdd->pc_Preamble;

    /* Get SRB_Delay */
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.dpch_pwr_ctrl_info.srb_delay
      = (uint8)ie_ptr->u.fdd->sRB_delay;

    /* Get the Power Control Algorithm */
    if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->u.fdd->powerControlAlgorithm, 
                  T_tdsrrc_PowerControlAlgorithm_algorithm1))
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.dpch_pwr_ctrl_info.pca =
        TDSL1ULCFG_PCA_ONE;
      if(ie_ptr->u.fdd->powerControlAlgorithm.u.algorithm1 == 0)
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.dpch_pwr_ctrl_info.tpc_step_size =
          TDSL1ULCFG_TPC_STEP_ONE;
      }
      else if(ie_ptr->u.fdd->powerControlAlgorithm.u.algorithm1 == 1)
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.dpch_pwr_ctrl_info.tpc_step_size =
          TDSL1ULCFG_TPC_STEP_TWO;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"TPC Step size %d is out of range",ie_ptr->u.fdd->powerControlAlgorithm.u.algorithm1);
        return FAILURE;
      } /* end if-else check for Algorithm 1 and TPC step size */
    }
    else if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->u.fdd->powerControlAlgorithm,
                    T_tdsrrc_PowerControlAlgorithm_algorithm2))
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.dpch_pwr_ctrl_info.pca =
        TDSL1ULCFG_PCA_TWO;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid UL Power Control Algorithm specified");
      return FAILURE;
    } /* end if-else check for UL power control algorithm check */
  } /* end FDD check */

  else /* only TDD */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No valid FDD data for UL DPCH Power Control Info");
    return FAILURE;
  }
#endif
/* add comment for 10.3.6.88 end */
  return SUCCESS;
} /* end tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info() */

/*====================================================================
FUNCTION       tdsrrcllc_verify_sccpch_exists()

DESCRIPTION    This function takes a pointer to the SCCPCH system
               information list and RRC state, checks
               to see if at least one SCCPCH exists that has a FACH
               (for RRC states CELL_FACH and CONNECTING) or a PCH
               (for RRC states DISCONNECTED, CELL_PCH or URA_PCH)
               mapped to it.

DEPENDENCIES   None

RETURN VALUE   TRUE: SCCPCH with right type of TrCH exists.
               FALSE: SCCPCH with right kind of TrCH does NOT exist.

SIDE EFFECTS   None
====================================================================*/
boolean tdsrrcllc_verify_sccpch_exists
(     
  tdsrrc_SysInfoType6    *sib6_ptr,
  tdsrrc_state_e_type  next_rrc_state
)
{
  uint8 num_of_sccpch        = 1;

  uint8 sccpch_sib_fach_cnt = 0;  /* count of SIB 5/6 SCCPCH->PCH mappings */
  uint8 sccpch_sib_pch_cnt  = 0;  /* count of SIB 5/6 SCCPCH->PCH mappings */

  tdsrrc_SCCPCH_SystemInformationList  *passed_ie_ptr;
  tdsrrc_SCCPCH_SystemInformationList_LCR_r4_ext  *passed_ie_lcr_ptr;
  boolean ret_val;
  uint32  n_index = 0;

  if (sib6_ptr == NULL)
  {
  return FALSE;
  }
  /* Store the passed IE pointer for later use */
  passed_ie_ptr = &sib6_ptr->sCCPCH_SystemInformationList;
  passed_ie_lcr_ptr = &sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.tdd128SpecificInfo.sCCPCH_LCR_ExtensionsList;

  
  if((passed_ie_lcr_ptr == NULL ) || (passed_ie_lcr_ptr->n == 0) ||
      (passed_ie_ptr == NULL ) || (passed_ie_lcr_ptr->n != passed_ie_ptr->n))
  {
    /* Something went wrong reading the SCCPCH_SYS_INFO LCR IE from SIB5 or SIB6. */
    return FALSE;
  }


 while ((passed_ie_lcr_ptr->n >  n_index) && (num_of_sccpch <= TDSUE_MAX_SCCPCH))
 {
    /* Checking for PCH in a given SCCPCH */
   if (TDSRRC_CHECK_MSG_TYPE_IE(passed_ie_lcr_ptr->elem[n_index],
                 pich_InfoPresent))
    {
      sccpch_sib_pch_cnt++;

      /* Now also check for a FACH on the same SCCPCH */
      /*
      * Note that this is a linked list and only one PCH can
      * can be present for a given SCCPCH, so if there is more
      * than one Transport channel per SCCPCH, the second one
      * has to be a FACH, i.e. the next pointer is not NULL.
      */
      if(TDSRRC_CHECK_MSG_TYPE_IE(passed_ie_ptr->elem[n_index],
                  fach_PCH_InformationListPresent))
      {
        if (passed_ie_ptr->elem[n_index].fach_PCH_InformationList.n > 1)
        {
          sccpch_sib_fach_cnt++;
        }
      }
    }
    /* FACH only SCCPCH case */
    else
    {
      sccpch_sib_fach_cnt++;
    }

    /* get the next element in the linked list */
    num_of_sccpch++;
    n_index++;
  } /* end SIB SCCPCH check while */

  /* Now check if FACH/PCH count exists for the given RRC state */
  switch(next_rrc_state)
  {
    case TDSRRC_STATE_CELL_FACH:
    case TDSRRC_STATE_CONNECTING:
      if(sccpch_sib_fach_cnt)
      {
        ret_val = TRUE;
      }
      else
      {
        ret_val = FALSE;
      }
      break;

    case TDSRRC_STATE_DISCONNECTED:
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      if(sccpch_sib_pch_cnt)
      {
        ret_val = TRUE;
      }
      else
      {
        ret_val = FALSE;
      }
      break;

    default:
      ret_val = FALSE;
      break;
  }

  return(ret_val);
} /* tdsrrcllc_verify_sccpch_exists */


/*====================================================================
FUNCTION       tdsrrcllc_get_sccpch_sys_info()

DESCRIPTION    This function takes a pointer to the SCCPCH system
               information list for SIB 5, determines the state, checks
               to see if SIB 6 is also available, and selects the
               applicable SCCPCH information to use, and copies all
               applicable IEs for MAC, RLC, and L1 into the
               TDSORDERED_CONFIG database (internal format).

               SIB 5 must be there or else the UE cannot get pages or
               enter connected mode. This SIB contains the Inormation
               necessary to setup the SCCPCH an PRACH physical channels,
               and the PCH, FACH and RACH transport channels.

               SIB 6 is optional, and if present is only used to get
               SCCPCH information while in connected mode, i.e.
               CELL_FACH, CELL_PCH and URA_PCH.

DEPENDENCIES   rrcllc_init_ordered_config should always be called
               before a call to this function.

RETURN VALUE   SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS   This function presumes that Ordered Config is initialized
               before a call to this function. It updates all ordered
               config structures based on this assumption.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_sccpch_sys_info
(
  tdsordered_config_type  *config_ptr,
  /* the pointer should be transform to sib5 or sib6, is decided by sib6_present */
  void                    *sib5_6ptr,
  /* true means sib5_6ptr is sib6, false means sib5_6ptr is sib5 */ 
  uint8                   sib6_present,                  
  tdsrrc_state_e_type  next_rrc_state
)
{
  /* local pointer */
  tdsrrc_SCCPCH_SystemInformationList *ie_ptr ;
  tdsrrc_SCCPCH_SystemInformationList_LCR_r4_ext   *ie_lcr_ptr;
  tdsrrc_SysInfoType5     *sib5_ptr;
  tdsrrc_SysInfoType6     *sib6_ptr;


  /* local ptr to the fach_pch_infomationlist and tfcs to use for
   * the selected sccpch
   */
  tdsrrc_FACH_PCH_InformationList *fach_pch_infolist_to_use = NULL;
  tdsrrc_TFCS tfcs_to_use ;


  /* Pointer for the FACH info */
  tdsrrc_FACH_PCH_InformationList     *fach_ptr = NULL;

  /* Local variables */
  uint8 num_of_sccpch = 1;  /* number of total SCCPCHs */
  uint8 sccpch_index = 1;


  /* local TrCH Index var */
  uint8 pch_trch_idx = 0;
  uint8 fach_trch_idx = 0;

  
  uint32 n_index = 0;
  uint32 fach_n_index = 0;
  memset(&(tfcs_to_use),0,sizeof(tdsrrc_TFCS));

  if (sib5_6ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL pointer passed for sib5 or sib6");
    return(FAILURE);
  }
  /* make a local copy of the IE pointer */
  if (TRUE == sib6_present)
  {
    sib6_ptr = (tdsrrc_SysInfoType6*)sib5_6ptr;
    ie_ptr =  &sib6_ptr->sCCPCH_SystemInformationList;
    if (TDSRRCLLC_SIB6_SCCPCH_LCR_EXT_PRESENT(sib6_ptr))
    {
      ie_lcr_ptr =  &sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.
                        tdd128SpecificInfo.sCCPCH_LCR_ExtensionsList;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SCCPCH system Info LCR invaild ");
      return(FAILURE);
    }
  }
  else
  {
    sib5_ptr = (tdsrrc_SysInfoType5*)sib5_6ptr;
    ie_ptr = &sib5_ptr->sCCPCH_SystemInformationList;
    if (TDSRRCLLC_SIB5_SCCPCH_LCR_EXT_PRESENT(sib5_ptr))
    {
      ie_lcr_ptr = &sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.
                      tdd128SpecificInfo.sCCPCH_LCR_ExtensionsList;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SCCPCH system Info LCR invaild ");
      return(FAILURE);
    }
  }
  
  if((ie_ptr == NULL) || (ie_lcr_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL pointer passed for SCCPCH system Info");
    return(FAILURE);
  }
  
  if(ie_ptr->n != ie_lcr_ptr->n)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No match between SCCPCH system Info and SCCPCH system Info LCR ");
    return(FAILURE);
  }

  /*
  * Initialize the FACH Pointer. If there is a PCH, the new FACH pointer will be
  * Updated after processing the PCH information.
  */
  fach_ptr = NULL;

  /*
  * Determine which SCCPCH to use and set Ordered Config with
  * those values.
  */
  sccpch_index = (uint8)tdsrrcllc_get_sccpch_index(config_ptr, sib5_6ptr, sib6_present, next_rrc_state);


  /*
  * Make sure that the SCCPCH to use is valid, i.e. does not exceed the maximum
  * number of supported SCCPCHs
  */
  if ((sccpch_index == TDSRRCLLC_INVALID_SCCPCH_IDX) || (sccpch_index > TDSUE_MAX_SCCPCH))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid SCCPCH selected,SCCPCH index %d",sccpch_index);
    return FAILURE;
  }

  /* Update curr sccpch index */
  tdscurr_sccpch_index = sccpch_index;

  /*
  * Select which SIB to read the SCCPCH from if in connected mode. If in
  * Idle mode, always use SIB5.
  */
  num_of_sccpch = 1;
 
  n_index = 0;


  /* Look for the SCCPCH Info for the SCCPCH that has been selected */
 
    n_index = sccpch_index -1;
    num_of_sccpch = sccpch_index;
  
  if(n_index >= ie_ptr->n)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"SCCPCH not found for idx %d",sccpch_index);
    return FAILURE;
  }

  

  /* Get the TFCS & fach_pch_info to use for current sccpch */
  if( (TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
               tfcsPresent)) &&
      (TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
               fach_PCH_InformationListPresent))
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCS and FACH_PCH info present in sccpch");

    fach_pch_infolist_to_use = &(ie_ptr->elem[n_index].fach_PCH_InformationList);

    fach_n_index = 0;
    tfcs_to_use = ie_ptr->elem[n_index].tfcs;
  }
  else
  {
    if(num_of_sccpch == 1)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFCS & PCH/FACH data needed for 1st SCCPCH");
      return FAILURE;
    }
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"calling function tdsrrcllcpcie_get_tfcs_and_fach_pch_info");
    if (SUCCESS == tdsrrcllcpcie_get_tfcs_and_fach_pch_info(*ie_ptr,
                                          *ie_lcr_ptr,
                                              &fach_pch_infolist_to_use,
                                              &tfcs_to_use,
                                              sccpch_index ))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"retrieved tfcs and fach_pch_info_list");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to get tfcs and fach_pch_info");
      return(FAILURE);
    }
  }


  /* NOTE: Always get the TFCS info first, before getting the SCCPCH info. This
   * is because TFCS info may initialize the CTFC info that includes the "TFCI
   * exits" field. That field is set by tdsrrcllcpcie_get_sccpch_info().
   */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Get DL TFCS");
  if(tdsrrcllc_get_dl_tfcs_info(config_ptr,
                      &tfcs_to_use,
                      &config_ptr->dl_ctfc)
     == FAILURE)
  {
    return FAILURE;
  }

  /* Must now have the correct SCCPCH index, lets get the IEs */
  /* Get the SCCPCH info, only support 1 SCCPCH for now */
  if (tdsrrcllcpcie_get_sccpch_info(&ie_ptr->elem[n_index].secondaryCCPCH_Info,
                       &ie_lcr_ptr->elem[n_index].secondaryCCPCH_LCR_Extensions,
                      &config_ptr->l1_dl_chan_parms.sccpch[0],
                      &config_ptr->l1_dl_ctfc_parms,
                      &config_ptr->l1_dl_cctrch_parms
                     ) == FAILURE)
  {
    /* Error getting SCCPCH information.  Return FAILURE to indicate
       error and bail out of routine */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error getting SCCPCCH info from SCCPCH sys info");
    return FAILURE;
  }


  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Getting PCH TRCH_IDX");

  /* Get the TrCH data for this SCCPCH - PCH is first, if present */
  if(TDSRRC_CHECK_MSG_TYPE_IE(ie_lcr_ptr->elem[n_index],
                 pich_InfoPresent))
  {
    /* Got SCCPCH Info, lets get the TrCH channel Index for the PCH */
    pch_trch_idx =
        tdsrrcllc_get_dl_pch_trch_idx(config_ptr,
                                   (tr_ch_id_type)fach_pch_infolist_to_use->elem[fach_n_index].transportChannelIdentity);

    /* Let LLC know that there is a PCH on this SCCPCH */
    tdsrrcllc_set_pch_included(TRUE);

    /* ... and make sure we have not exceeded UE capaabilities for DL TRCH's */
    if((TDSRRCLLC_DOWNLINK_TRCH_COUNT(config_ptr)) > TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Exceeded maximum number of DL TRCHs");
      return FAILURE;
    }

    /* Get the TFS info for this TrCH */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Get TFS for DL TrCH Id %d",fach_pch_infolist_to_use->elem[fach_n_index].transportChannelIdentity);

    if(tdsrrcllc_get_dl_transport_format_set( &fach_pch_infolist_to_use->elem[fach_n_index].transportFormatSet,
                                    (tdsl1_dl_cctrchcfg_trch_info_struct_type *) NULL,  /* dedicated trch */
                                    FALSE, /* mulitple logical channels not supported */
                                    config_ptr->l1_dl_cctrch_parms.
                                      trch_info_array[pch_trch_idx]
                                   ) == FAILURE)
    {
      return FAILURE;
    }

    /* Increment the pch_trch_idx to index correctly into the common TrCH array */
    /* And get the FACH TrCH Pointer */
    
    pch_trch_idx++;
    fach_n_index++;

  } /* end PCH */
  else
  {
    /* The SCCPCH selected does not have a PCH */
    /* Let LLC know that there is no PCH on this SCCPCH */
    tdsrrcllc_set_pch_included(FALSE);
    /* Set the fach_ptr to point to the fach TrCH info */
    
    fach_n_index = 0;
  }

  /*
  * Get the Transport channel data for this SCCPCH - FACH is next, if it is there
  * since there can only be one PCH per SCCPCH.
  */
  if(fach_n_index >= fach_pch_infolist_to_use->n)
  {
    /* The selected SCCPCH does not have a FACH */
    /* Let LLC know that there is no FACH on this SCCPCH */
    tdsrrcllc_set_fach_included(FALSE);
  }
  else
  {
    fach_ptr =fach_pch_infolist_to_use;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Getting fach_trch_idx");
    while(((TDSRRCLLC_DOWNLINK_TRCH_COUNT(config_ptr)) <= TDSUE_MAX_TRCH)
           && (fach_n_index < fach_pch_infolist_to_use->n))
    {
      /* Got SCCPCH Info, lets get the TrCH channel ID for the PCH */
      /* If PCH is present, pch_trch_idx is 1. Else when PCH is not present,
       * pch_trch_idx is 0. So we can always add pch_trch_idx while updating
       * TrCH info in L1 data structure.
       */
      fach_trch_idx = tdsrrcllc_get_dl_fach_trch_idx(
                              config_ptr,
                              (tr_ch_id_type)fach_ptr->elem[fach_n_index].transportChannelIdentity,
                               pch_trch_idx);

      if(fach_trch_idx >= TDSUE_MAX_TRCH)
      {
        /* return an error here */
        return FAILURE;
      }

      /* ... and make sure we have not exceeded UE capabilities for DL TRCH's */
      if((TDSRRCLLC_DOWNLINK_TRCH_COUNT(config_ptr)) > TDSUE_MAX_TRCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Exceeded maximum number of DL TRCHs");
        return FAILURE;
      }

      /* Let LLC know that there is a FACH on this SCCPCH */
      tdsrrcllc_set_fach_included(TRUE);

      /* Get the TFS info for this TrCH */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Get TFS for DL TrCH Id %d",fach_ptr->elem[fach_n_index].transportChannelIdentity);

      if (TDSL1_DL_MAX_TRCH <= (pch_trch_idx + fach_trch_idx))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Buffer overflow");
        return FAILURE;
      }

      if(tdsrrcllc_get_dl_transport_format_set(&fach_ptr->elem[fach_n_index].transportFormatSet,
                                      (tdsl1_dl_cctrchcfg_trch_info_struct_type *) NULL,  /* dedicated trch */
                                      FALSE, /* mulitple logical channels not supported */
                                      config_ptr->l1_dl_cctrch_parms.
                                        trch_info_array[pch_trch_idx + fach_trch_idx]   /* common trch */
                                     ) == FAILURE)
      {
        return FAILURE;
      }


      /* Next FACH ptr */
      fach_n_index++;

    } /* end FACH */
  } /* end else */

  /* Set the RM restriction info to Invalid since it is not present for
   * Common Physical Channels.
   */

  /* Calculate the DL TFCS */
  tdsrrcllc_update_rm_restriction_info(&config_ptr->l1_dl_cctrch_parms,
                             &config_ptr->dl_ctfc,
                             &config_ptr->l1_dl_ctfc_parms
                            );

  /* Get PICH information from SCCPCH sys information IE */
  if(TDSRRC_CHECK_MSG_TYPE_IE(ie_lcr_ptr->elem[n_index],
                 pich_InfoPresent))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"Get PICH Info");
    if (tdsrrcllcpcie_get_pich_info(&ie_lcr_ptr->elem[n_index].pich_Info, &config_ptr->l1_dl_chan_parms.pich)
      == FAILURE )
    {
      /* Error getting PICH information.  Return FAILURE to indicate
         error and bail out of routine */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error getting PICH info from SCCPCH sys info");
      return FAILURE;
    }
    /* Tell L1 to use the PICH */
    if((next_rrc_state == TDSRRC_STATE_DISCONNECTED) ||
       (next_rrc_state == TDSRRC_STATE_CELL_PCH) ||
       (next_rrc_state == TDSRRC_STATE_URA_PCH))
    {
      config_ptr->l1_dl_chan_parms.use_pich = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Use PICH FALSE for st %d",next_rrc_state);
      config_ptr->l1_dl_chan_parms.use_pich = FALSE;
    }

  } /* end PICH check */
  else
  {
    config_ptr->l1_dl_chan_parms.use_pich = FALSE;
  }
  /* Indicate that L1 DL needs to be reconfigured */
  config_ptr->reconfig_needed.downlink_l1 = TRUE;
  /* Although no new parameters for MAC have been received, MAC
   * still needs to be reconfigured so as to get the new CCTrCH
   * ID whenever Downlink L1 is reconfigured. This is due to the
   * interface requirements.
   */
  config_ptr->reconfig_needed.downlink_mac = TRUE;

  return SUCCESS;
} /* end tdsrrcllc_get_sccpch_sys_info() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_tfcs_and_fach_pch_info()

DESCRIPTION     This function will return the tfcs and fach_pch
                info list to use for the sccpch identitfied by the
                index.

RETURN VALUE    SUCCESS or FAILURE

====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_tfcs_and_fach_pch_info
(
  tdsrrc_SCCPCH_SystemInformationList                    sccpch_sys_info_ptr,
  tdsrrc_SCCPCH_SystemInformationList_LCR_r4_ext         sccpch_sys_lcr_info_ptr,
  tdsrrc_FACH_PCH_InformationList                        **fach_pch_infolist_to_use,
  tdsrrc_TFCS                                            *tfcs_to_use,
  uint8                                                  index
)
{

  /* local pointer */
  tdsrrc_SCCPCH_SystemInformationList *ie_ptr = NULL;
  tdsrrc_SCCPCH_SystemInformationList_LCR_r4_ext   *ie_lcr_ptr = NULL;

  /* boolean to indicate whether SCCPCH includes PICH*/
  boolean pich_included = FALSE;

  tdsrrc_TFCS prv_tfcs_and_pich;
  tdsrrc_TFCS prv_tfcs_and_no_pich;

  /* Local copy of the previous SCCPCH with fach_pch_infolist present that has PICH INFO as well */
  tdsrrc_FACH_PCH_InformationList *prv_fach_pch_list_and_pich = NULL;
  tdsrrc_FACH_PCH_InformationList *prv_fach_pch_list_and_no_pich = NULL;

  uint8 count_sccpch = 0;
  uint32 n_index = 0;

  memset(&(prv_tfcs_and_pich),0,sizeof(tdsrrc_TFCS));
  memset(&(prv_tfcs_and_no_pich),0,sizeof(tdsrrc_TFCS));

  ie_ptr = &sccpch_sys_info_ptr;
  ie_lcr_ptr = &sccpch_sys_lcr_info_ptr;
  
  if((ie_ptr == NULL) || (ie_lcr_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL pointer passed for SCCPCH system Info");
    return(FAILURE);
  }

  if (ie_ptr->n != ie_lcr_ptr->n)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"no match between SCCPCH system Info and SCCPCH system Info LCR ");
    return(FAILURE);
  }
  
  /* Look if SCCPCH included PCH or not*/
  while((ie_ptr->n > n_index) && (count_sccpch != index))
  {
    count_sccpch++;

    /* Maintain the TFCS from previous SCCPCH with PICH and the previous SCCPCH with No PICH */
    if((TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                tfcsPresent)) && 
        (TDSRRC_CHECK_MSG_TYPE_IE(ie_lcr_ptr->elem[n_index],
                 pich_InfoPresent)))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Saving TFCS from sccpch %d with PICH",count_sccpch);
      prv_tfcs_and_pich = ie_ptr->elem[n_index].tfcs;
    }
    else if (TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                tfcsPresent))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Saving TFCS from sccpch %d without PICH",count_sccpch);
      prv_tfcs_and_no_pich = ie_ptr->elem[n_index].tfcs;
    }
    /* Maintain the fach_pch list from previous SCCPCH with PICH and the previous SCCPCH with No PICH */
    if((TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                fach_PCH_InformationListPresent)) && 
       (TDSRRC_CHECK_MSG_TYPE_IE(ie_lcr_ptr->elem[n_index],
                 pich_InfoPresent)))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Saving fach_pch_infolist from sccpch %d with PICH",count_sccpch);
      prv_fach_pch_list_and_pich = &(ie_ptr->elem[n_index].fach_PCH_InformationList);
    }
    else if(TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                fach_PCH_InformationListPresent))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Saving fach_pch_infolist from sccpch %d without PICH",count_sccpch);
      prv_fach_pch_list_and_no_pich = &(ie_ptr->elem[n_index].fach_PCH_InformationList);
    }

     /* IF SCCPCH selected by UE includes PCH, the UE should use TFCS and fach/pch info list
    * from the previous sccpch carrying PCH */
    if (count_sccpch == index)
    {
      if(TDSRRC_CHECK_MSG_TYPE_IE(ie_lcr_ptr->elem[n_index],
                  pich_InfoPresent))
      {
        pich_included = TRUE;
      }
      break;
    }

    n_index++;
  } /*while */

  if((ie_ptr == NULL) || count_sccpch != index)
  {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"failed to get tfcs and fach/pch info list");
      return (FAILURE);
  }


  if (pich_included)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PICH INFO included.  getting from last SCCPCH with PICH info");

    *fach_pch_infolist_to_use = prv_fach_pch_list_and_pich;
    *tfcs_to_use = prv_tfcs_and_pich;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PICH INFO not included.  getting from last SCCPCH with no PICH info");
    *fach_pch_infolist_to_use = prv_fach_pch_list_and_no_pich;
    *tfcs_to_use = prv_tfcs_and_no_pich;
  }

  if (*fach_pch_infolist_to_use == NULL || (tfcs_to_use->t ==0))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error: No fach_pch_infolist or tfcs to use");
    return (FAILURE);
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION        tdsrrcllc_get_sccpch_index()

DESCRIPTION     This function will determine which SCCPCH to read from
                SIB 5, and returns this index.

DEPENDENCIES    The U-RNTI should have been read and filled in
                Ordered Config

RETURN VALUE    The index (starting at 1) of the SCCPCH to read. An
                index of zero indicates a problem reading the
                SCCPCH_SYS_INFO IE from Sib 5 or Sib 6.

SIDE EFFECTS    This function also sets the imsi_div_pch_cnt(IMSI/K)
                parameter for L1 to perform the DRX calculations.
====================================================================*/
int tdsrrcllc_get_sccpch_index
(
  tdsordered_config_type  *config_ptr,
  /* the point should be transform to sib5 or sib6, is decided by sib6_present */
  void                    *sib5_6ptr,
  /* true means sib5_6ptr is sib6, false means sib5_6ptr is sib5 */ 
  uint8                    sib6_present,      
  tdsrrc_state_e_type                next_rrc_state
)
{
  /* local variables */
  uint32 tmp_sccpch_index = TDSRRCLLC_INVALID_SCCPCH_IDX;  /* This index is 1 based, not zero based */
  uint32 sccpch_pch_count  = 0;
  uint32 sccpch_fach_count = 0;
  uint64 imsi;

  uint8 sccpch_index         = 1;
  uint8 num_of_sccpch        = 1;

  uint8 sccpch_sib_fach_cnt = 0;  /* count of SIB 5/6 SCCPCH->PCH mappings */
  uint8 sccpch_sib_pch_cnt  = 0;  /* count of SIB 5/6 SCCPCH->PCH mappings */

  tdsrrc_SCCPCH_SystemInformationList  *passed_ie_ptr;
  tdsrrc_SCCPCH_SystemInformationList_LCR_r4_ext  *passed_lcr_ie_ptr;
  tdsrrc_SysInfoType5   *sib5_ptr;
  tdsrrc_SysInfoType6   *sib6_ptr;

  /* Local copy of the previous fach_pch_infolist from an SCCPCH that has PICH INFO.
   * default can be the fach_pch_infolist from the first sccpch since it is MP for the 1st sccpch
   */
  tdsrrc_FACH_PCH_InformationList *prvs_fach_pch_list_with_pich = NULL;

  /* This indicates if an SCCPCH index was found successfully */
  boolean  sccpch_index_found = FALSE;


  uint32 n_index = 0;
  /* Figure out how many SCCPCHs contain a PCH or a FACH.
   * Just loop through all the SCCPCH entries and count PCH and FACH.
   */

  if (sib5_6ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL pointer passed for sib5 or sib6");
    return TDSRRCLLC_INVALID_SCCPCH_IDX;
  }
  /* Store the passed IE pointer for later use */
  if (TRUE == sib6_present)
  {
    sib6_ptr = (tdsrrc_SysInfoType6 *)sib5_6ptr;
    passed_ie_ptr = &sib6_ptr->sCCPCH_SystemInformationList;
    if (TDSRRCLLC_SIB6_SCCPCH_LCR_EXT_PRESENT(sib6_ptr))
    {
      passed_lcr_ie_ptr = &sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.
                                  tdd128SpecificInfo.sCCPCH_LCR_ExtensionsList;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SCCPCH system Info LCR invaild ");
      return TDSRRCLLC_INVALID_SCCPCH_IDX;
    }    
  }
  else
  { 
    sib5_ptr = (tdsrrc_SysInfoType5 *)sib5_6ptr;
    passed_ie_ptr = &sib5_ptr->sCCPCH_SystemInformationList;
    if (TDSRRCLLC_SIB5_SCCPCH_LCR_EXT_PRESENT(sib5_ptr))
    {      
      passed_lcr_ie_ptr = &sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.tdd128SpecificInfo.sCCPCH_LCR_ExtensionsList;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SCCPCH system Info LCR invaild ");
      return TDSRRCLLC_INVALID_SCCPCH_IDX;
    }    
  }
    
  /* Below are several places where lint error 414 is suppressed.  This is a
     "possible division by zero" warning.  The part of the expression that lint
     thinks might be zero is the count of elements in the ie_ptr list.  Since
     we check this pointer for NULLness below, this count is never zero, but
     lint isn't quite that clever. */

  /* The number of list should be match between SCCPCH system Info and SCCPCH system Info LCR */
  if((passed_ie_ptr == NULL )|| (passed_lcr_ie_ptr == NULL ) || 
     (passed_ie_ptr->n == 0) || (passed_ie_ptr->n != passed_lcr_ie_ptr->n))
  {
    /* Something went wrong reading the SCCPCH_SYS_INFO IE from SIB5 or SIB6. */
    return TDSRRCLLC_INVALID_SCCPCH_IDX;
  }

  prvs_fach_pch_list_with_pich = &(passed_ie_ptr->elem[n_index].fach_PCH_InformationList);

  while((passed_ie_ptr->n >  n_index) && (num_of_sccpch <= TDSUE_MAX_SCCPCH))
  {
    /* Checking for PCH in a given SCCPCH */
    if(TDSRRC_CHECK_MSG_TYPE_IE(passed_lcr_ie_ptr->elem[n_index],
                pich_InfoPresent))
    {
      sccpch_sib_pch_cnt++;

      /* Now also check for a FACH on the same SCCPCH */
      /*
      * Note that this is a linked list and only one PCH can
      * can be present for a given SCCPCH, so if there is more
      * than one Transport channel per SCCPCH, the second one
      * has to be a FACH, i.e. the next pointer is not NULL.
      */

      if(TDSRRC_CHECK_MSG_TYPE_IE(passed_ie_ptr->elem[n_index],
                  fach_PCH_InformationListPresent))
      {
        /* store fach_pch_informationList. */
        prvs_fach_pch_list_with_pich = &(passed_ie_ptr->elem[n_index].fach_PCH_InformationList);
        if (passed_ie_ptr->elem[n_index].fach_PCH_InformationList.n > 1)
        {
          sccpch_sib_fach_cnt++;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Checking previous SCCPCH to inc fach_cnt");
        if (prvs_fach_pch_list_with_pich->n > 1)
        {
          sccpch_sib_fach_cnt++;
        }
      }
    }
    /* FACH only SCCPCH case */
    else
    {
      sccpch_sib_fach_cnt++;
    }

    /* get the next element in the linked list */
    num_of_sccpch++;
    n_index++;
  } /* end SIB SCCPCH check while */

  /* Get a local copy of the imsi */
  imsi = tdsrrc_convert_gsm_map_imsi_into_int_value(tdsrrc_imsi);

  /*
  * Use the current state to determine which SCCPCH to use. Note that
  * everytime an SCCPCH is needed to be configured, the current state
  * is needed to ensure that the right SCCPCH is selected. The selection
  * algorithm is this (taken from 25.304 for Idle Mode, and 25.331 for
  * Connected Mode):
  */
  switch (next_rrc_state)
  {
    case TDSRRC_STATE_DISCONNECTED:
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:

      /* save imsi div K, where k = # SCCPCH that contain a PCH, for L1 */
      if(sccpch_sib_pch_cnt == 0)
      {
        sccpch_index = TDSRRCLLC_INVALID_SCCPCH_IDX;
      }
      else
      {
        if (next_rrc_state == TDSRRC_STATE_DISCONNECTED)
        {
          /* Get the SCCPCH that contains PCH to listen to based on equation below */
          tmp_sccpch_index = (uint32)imsi % sccpch_sib_pch_cnt;
        }
        else
        {
          tmp_sccpch_index = config_ptr->mac_dl_parms.rnti_info.urnti
                                 % sccpch_sib_pch_cnt;
        }

        config_ptr->l1_dl_chan_parms.pich.imsi_div_pch_count =
          imsi / sccpch_sib_pch_cnt;
        
        /* Save IMSI divided by 8192, it is used in calculating Paging Occasion. */
        config_ptr->l1_dl_chan_parms.pich.imsi_div_8192 =
          imsi / 8192;
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"imsi_div_pch_cnt=%d;imsi_div_8192=%d",config_ptr->l1_dl_chan_parms.pich.imsi_div_pch_count,config_ptr->l1_dl_chan_parms.pich.imsi_div_8192);

        tmp_sccpch_index++; /* since 1 based index, not zero based */

        /* Now get the real index that is based on all SCCPCHs */
        //passed_ie_ptr = ie_ptr;
        for(n_index=0;
            passed_ie_ptr->n > n_index;
             n_index++ , sccpch_index++)
        {
          if(TDSRRC_CHECK_MSG_TYPE_IE(passed_lcr_ie_ptr->elem[n_index],
                      pich_InfoPresent))
          {
            sccpch_pch_count++;
            if(sccpch_pch_count == tmp_sccpch_index)
            {
              sccpch_index_found = TRUE;
              break;
            }
          }
        }
      } /* sccpch_sib_pch_cnt != 0*/
      break;

    case TDSRRC_STATE_CONNECTING:
    case TDSRRC_STATE_CELL_FACH:
      /* Get the SCCPCH that contains FACH to listen to based on equation below */
      if(sccpch_sib_fach_cnt == 0)
      {
        sccpch_index = TDSRRCLLC_INVALID_SCCPCH_IDX;
      }
      else
      {
        if(next_rrc_state == TDSRRC_STATE_CONNECTING)
        {
          tmp_sccpch_index = (uint32)(tdsrrc_convert_initial_ue_identity_into_int_value()
                    % sccpch_sib_fach_cnt);
        }
        else if(next_rrc_state == TDSRRC_STATE_CELL_FACH)
        {
          tmp_sccpch_index = config_ptr->mac_dl_parms.rnti_info.urnti
                                 % sccpch_sib_fach_cnt;
        }
        else /* State is Cell_PCH or URA_PCH*/
        {
          if (sccpch_sib_pch_cnt == 0)
          {
            sccpch_index = TDSRRCLLC_INVALID_SCCPCH_IDX;
          }
          else
          {
            tmp_sccpch_index = config_ptr->mac_dl_parms.rnti_info.urnti
                       % sccpch_sib_pch_cnt;
          }
        }

        tmp_sccpch_index++; /* since 1 based index, not zero based */

        /* Now get the real index that is based on all SCCPCHs */
        //passed_ie_ptr = ie_ptr;
     
        for(n_index = 0;
            n_index < passed_ie_ptr->n;
            n_index++ , sccpch_index++)
        {
          if(TDSRRC_CHECK_MSG_TYPE_IE(passed_lcr_ie_ptr->elem[n_index],
                      pich_InfoPresent))
          {
            if(TDSRRC_CHECK_MSG_TYPE_IE(passed_ie_ptr->elem[n_index],
                      fach_PCH_InformationListPresent))
            {
              /* store fach_pch_informationList. */
              prvs_fach_pch_list_with_pich = &(passed_ie_ptr->elem[n_index].fach_PCH_InformationList);
              if (passed_ie_ptr->elem[n_index].fach_PCH_InformationList.n >1 )
              {
                sccpch_fach_count++;
              }
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Checking previous SCCPCH to inc fach_cnt");
              if (prvs_fach_pch_list_with_pich->n > 1)
              {
                sccpch_fach_count++;
              }
            }
          }
          else
          {
            sccpch_fach_count++;
          }

          if(sccpch_fach_count == tmp_sccpch_index)
          {
            sccpch_index_found = TRUE;
            break;
          }
        }
      } /* sccpch_sib_fach_cnt != 0*/
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown RRC State %d when picking an SCCPCH",next_rrc_state);
      sccpch_index = TDSRRCLLC_INVALID_SCCPCH_IDX;
      break;

  } /* end switch */

  if(sccpch_index_found == FALSE)
  {
    sccpch_index = TDSRRCLLC_INVALID_SCCPCH_IDX;
  }

  return sccpch_index;
} /* end tdsrrcllc_get_sccpch_index */


/*====================================================================
FUNCTION        tdsrrcllcpcie_get_sccpch_info()

DESCRIPTION     This function transfers information from ASN.1
                output to the RRC config database for the SCCPCH

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    The config structure pointed to by the input
                parameter *ord_cfg is overwritten.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_sccpch_info
(
  /* pointer to the SCCPCH info in the ASN.1 decoder output */
  tdsrrc_SecondaryCCPCH_Info *ie_ptr,
  tdsrrc_SecondaryCCPCH_Info_LCR_r4_ext       *ie_lcr_ptr,
  /* pointer to the SCCPCH parameters structure */
  tdsl1_dl_phycfg_sccpch_parms_struct_type *psccpch,

  /* pointer to the CTFC info struture */
  tdsl1_dl_cctrchcfg_ctfc_info_struct_type *ctfc_info,

  /* pointer to the CCTrCH info structure */
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type *cctrch_info
)
{
  uint8         channel_cnt;
  tdsrrc_RepetitionPeriodLengthAndOffset   *repetition_ptr;
  tdsrrc_MidambleShiftAndBurstType_LCR_r4  *burstType_lcr;

  if((ie_ptr == NULL) || (ie_lcr_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL pointer passed for SCCPCH system Info");
    return(FAILURE);
  }
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_SecondaryCCPCH_Info_modeSpecificInfo_tdd))
  {
    /* TDD was chosen so let's rock */
    /* add new TDD handle of ie_ptr */
    /* save  commonTimeslotInfo */
    /* set the value of the second interleaving mode */
    psccpch->common_ts_info.second_interleaving_mode = 
            ie_ptr->modeSpecificInfo.u.tdd->commonTimeslotInfo.secondInterleavingMode;
  
    /* set the value of the puncturing Limit */
    /** Get the Puncturing Limit. The OTA Msg receives this value as an enum.
        * The puncturing limit is a real number between 0.4 and 1.0 in steps of 0.04.
        * L1 expects this value to be from 40 to 100; in other words, L1 expects
        * l1_puncturing_limit = puncturing_limit * 100;
        * Therefore in the ASN.1 enum type the first enum value 0 corresponds to 0.4,
        * 1 corresponds to 0.44, etc... . Hence the following equation is derived:
        * l1_puncturing_limit = (ASN.1_enum_type * 4) + 40;
        * If this enum changes, this equation may need tweaking or else, this can
        * be broken into a switch with multiple cases.
        */
    psccpch->common_ts_info.punct_limit = (uint8)
        (((uint8)(ie_ptr->modeSpecificInfo.u.tdd->commonTimeslotInfo.puncturingLimit) * 4) + 40);
    /* set the value of the tfci coding */
    if (ie_ptr->modeSpecificInfo.u.tdd->commonTimeslotInfo.m.tfci_CodingPresent)
    {
      switch (ie_ptr->modeSpecificInfo.u.tdd->commonTimeslotInfo.tfci_Coding)
      {
        case tdsrrc_TFCI_Coding_tfci_bits_4:
          psccpch->common_ts_info.tfci_coding = TDSL1_TFCI_CODING_BITS_4;
          break;
        
        case tdsrrc_TFCI_Coding_tfci_bits_8:
          psccpch->common_ts_info.tfci_coding = TDSL1_TFCI_CODING_BITS_8;
          break;
        
        case tdsrrc_TFCI_Coding_tfci_bits_16:
          psccpch->common_ts_info.tfci_coding = TDSL1_TFCI_CODING_BITS_16;
          break;
        
        case tdsrrc_TFCI_Coding_tfci_bits_32:
          psccpch->common_ts_info.tfci_coding = TDSL1_TFCI_CODING_BITS_32;
          break;
        
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfci_Coding is out of range");
          return FAILURE;
      }
    }
    else
    {
      psccpch->common_ts_info.tfci_coding = 0;
    }
    
    /* set the value of the repetitionPeriodLengthAndOffset */    
    if (ie_ptr->modeSpecificInfo.u.tdd->commonTimeslotInfo.m.repetitionPeriodLengthAndOffsetPresent)
    {   
      repetition_ptr = &ie_ptr->modeSpecificInfo.u.tdd->commonTimeslotInfo.repetitionPeriodLengthAndOffset;
      switch (repetition_ptr->t)
      {
        case T_tdsrrc_RepetitionPeriodLengthAndOffset_repetitionPeriod1:
          psccpch->common_ts_info.repetition_period = TDSL1_REPEAT_PRD_1;
          /* for repetitionPeriod1, length is null, so set initial is zero  */
          psccpch->common_ts_info.repetition_length = 0;     
          break;
        
        case T_tdsrrc_RepetitionPeriodLengthAndOffset_repetitionPeriod2:
          /* for CR 365022, Length is 0, L1 treat period is 1, can continuous receive sccpch */
          psccpch->offset = repetition_ptr->u.repetitionPeriod2->offset;
          psccpch->common_ts_info.repetition_period = TDSL1_REPEAT_PRD_1;
          psccpch->common_ts_info.repetition_length = 0;
          break;
      
        case T_tdsrrc_RepetitionPeriodLengthAndOffset_repetitionPeriod4:
          psccpch->offset = repetition_ptr->u.repetitionPeriod4->offset;
          psccpch->common_ts_info.repetition_period = TDSL1_REPEAT_PRD_4;
          psccpch->common_ts_info.repetition_length = repetition_ptr->u.repetitionPeriod4->length;
          break;
      
        case T_tdsrrc_RepetitionPeriodLengthAndOffset_repetitionPeriod8:
          psccpch->offset = repetition_ptr->u.repetitionPeriod8->offset;
          psccpch->common_ts_info.repetition_period = TDSL1_REPEAT_PRD_8;
          psccpch->common_ts_info.repetition_length = repetition_ptr->u.repetitionPeriod8->length;
          break;
     
        case T_tdsrrc_RepetitionPeriodLengthAndOffset_repetitionPeriod16:
          psccpch->offset = repetition_ptr->u.repetitionPeriod16->offset;
          psccpch->common_ts_info.repetition_period = TDSL1_REPEAT_PRD_16;
          psccpch->common_ts_info.repetition_length = repetition_ptr->u.repetitionPeriod16->length;
          break;

        case T_tdsrrc_RepetitionPeriodLengthAndOffset_repetitionPeriod32:
          psccpch->offset = repetition_ptr->u.repetitionPeriod32->offset;
          psccpch->common_ts_info.repetition_period = TDSL1_REPEAT_PRD_32;
          psccpch->common_ts_info.repetition_length = repetition_ptr->u.repetitionPeriod32->length;
          break;
        
        case T_tdsrrc_RepetitionPeriodLengthAndOffset_repetitionPeriod64:
          psccpch->offset = repetition_ptr->u.repetitionPeriod64->offset;
          psccpch->common_ts_info.repetition_period = TDSL1_REPEAT_PRD_64;
          psccpch->common_ts_info.repetition_length = repetition_ptr->u.repetitionPeriod64->length;
          break;
      
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad enum %u for RepetitionPeriodLengthAndOffset ",TDSRRC_GET_MSG_TYPE_PTR(repetition_ptr));
              return FAILURE;
      }
    }
    else
    {
      /* 10.3.6.10, Default is continuous allocation. Value 1 indicate continuous */
      psccpch->common_ts_info.repetition_period = TDSL1_REPEAT_PRD_1;
      /* for repetitionPeriod1, length is null, so set initial is zero  */
      psccpch->common_ts_info.repetition_length = 0;     
    }

    /* save IndividualTimeslotInfo */
    if (ie_ptr->modeSpecificInfo.u.tdd->individualTimeslotInfo.timeslotNumber == 1)
    {
      ERR_FATAL("ts_num is %d, DL ts_num is 0, 2, 3, 4, 5, 6",ie_ptr->modeSpecificInfo.u.tdd->individualTimeslotInfo.timeslotNumber,0,0);
      return FAILURE;
    }
    psccpch->individual_ts_info.ts_num = ie_ptr->modeSpecificInfo.u.tdd->individualTimeslotInfo.timeslotNumber;
    
    /* set to TRUE if TFCI is used */
    psccpch->individual_ts_info.tfci_exists = ie_ptr->modeSpecificInfo.u.tdd->individualTimeslotInfo.tfci_Existence;

    /* Initial channellistcode*/
    psccpch->individual_ts_info.chan_code_bitmap = 0;
    /* save channel code info */
    for (channel_cnt = 0; 
         channel_cnt < ie_ptr->modeSpecificInfo.u.tdd->channelisationCode.n; 
         channel_cnt++)
    {
      psccpch->individual_ts_info.chan_code_bitmap |= 
          (1 << ie_ptr->modeSpecificInfo.u.tdd->channelisationCode.elem[channel_cnt]);
    }
      
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Non TDD choice %u in SCCPCH info",TDSRRC_GET_MSG_TYPE(ie_ptr->modeSpecificInfo));
    return FAILURE;
  }

  /* add new tdd hadle of ie_lcr_ptr */ 
  /* save midambleShiftAndBurstType info from LCR */
  burstType_lcr = &ie_lcr_ptr->individualTimeslotLCR_Ext.midambleShiftAndBurstType;
  switch (burstType_lcr->midambleAllocationMode.t)
  {
    case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_defaultMidamble:
      psccpch->individual_ts_info.midamble_ss_burst_type.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
      break;
 
    case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_commonMidamble:
      psccpch->individual_ts_info.midamble_ss_burst_type.allocation_mode = TDSL1_COMMON_MIDAMBLE;
      break;
    case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble:
      psccpch->individual_ts_info.midamble_ss_burst_type.allocation_mode = TDSL1_UE_SPECIFIC_MIDAMBLE;
      psccpch->individual_ts_info.midamble_ss_burst_type.shift = 
           burstType_lcr->midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
      break;
      
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad enum %u for midambleAllocationMode ",TDSRRC_GET_MSG_TYPE(burstType_lcr->midambleAllocationMode));
      return FAILURE;
  }
  /* save midambleConfiguration info from LCR */
  psccpch->individual_ts_info.midamble_ss_burst_type.config = 
       burstType_lcr->midambleConfiguration - 1;
  psccpch->individual_ts_info.ss_tpc_symbols = 
      ie_lcr_ptr->individualTimeslotLCR_Ext.ss_TPC_Symbols;
  /* SCCPCH no need to set additional_ss_tpc_symbols */
  return SUCCESS;
} /* tdsrrcllcpcie_get_sccpch_info() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_pich_info()

DESCRIPTION     This function copies PICH information from ASN.1
                output to the RRC config database

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_pich_info
(
  /* pointer to the PICH info in the ASN.1 decoder output */
  tdsrrc_PICH_Info_LCR_r4   *ie_lcr_ptr,
  /* pointer to the PICH parameters structure */
  tdsl1_dl_phycfg_pich_parms_struct_type *ppich
)
{
  uint8    channel_cnt;
  tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode *midam_mode;
  if(ie_lcr_ptr == NULL)
  {
    ERR_FATAL("tdsrrc_PICH_Info pointer is NULL",0,0,0);
  }
 
  /* set the value of timeslot info, Default value is 0 */
  if(ie_lcr_ptr->m.timeslotPresent)
  {
    ppich->ts_num = ie_lcr_ptr->timeslot;
  }
  else
  {                                                      
    ppich->ts_num = 0;
  }     

  /* set the value of repetitionPeriodLengthOffset */
  if(ie_lcr_ptr->m.repetitionPeriodLengthOffsetPresent)
  {
    switch (ie_lcr_ptr->repetitionPeriodLengthOffset.t)
     {
       case T_tdsrrc_RepPerLengthOffset_PICH_rpp4_2:
         ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp4_2;
         ppich->repetition_period_length = TDSL1_PICH_REPEAT_4_2;
         break;
  
       case T_tdsrrc_RepPerLengthOffset_PICH_rpp8_2:
         ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp8_2;
         ppich->repetition_period_length = TDSL1_PICH_REPEAT_8_2;
         break;
  
       case T_tdsrrc_RepPerLengthOffset_PICH_rpp8_4:
        ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp8_4;
        ppich->repetition_period_length = TDSL1_PICH_REPEAT_8_4;
        break;
      
      case T_tdsrrc_RepPerLengthOffset_PICH_rpp16_2:
        ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp16_2;
        ppich->repetition_period_length = TDSL1_PICH_REPEAT_16_2;
        break;    
        
      case T_tdsrrc_RepPerLengthOffset_PICH_rpp16_4:
        ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp16_4;
        ppich->repetition_period_length = TDSL1_PICH_REPEAT_16_4;
        break;  
        
      case T_tdsrrc_RepPerLengthOffset_PICH_rpp32_2:
        ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp32_2;
        ppich->repetition_period_length = TDSL1_PICH_REPEAT_32_2;
        break;
        
      case T_tdsrrc_RepPerLengthOffset_PICH_rpp32_4:
        ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp32_4;
        ppich->repetition_period_length = TDSL1_PICH_REPEAT_32_4;
        break;   
        
      case T_tdsrrc_RepPerLengthOffset_PICH_rpp64_2:
        ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp64_2;
        ppich->repetition_period_length = TDSL1_PICH_REPEAT_64_2;
        break;
     
      case T_tdsrrc_RepPerLengthOffset_PICH_rpp64_4:
        ppich->offset = ie_lcr_ptr->repetitionPeriodLengthOffset.u.rpp64_4;
        ppich->repetition_period_length = TDSL1_PICH_REPEAT_64_4;
        break;
  
       default:     
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad enum %u for repetitionPeriodLengthOffset of pich ",TDSRRC_GET_MSG_TYPE(ie_lcr_ptr->repetitionPeriodLengthOffset));
        return FAILURE;
     }  
  }
  else
  {
    /* 10.3.6.49, Default value is "(64/2)". */
    ppich->offset = 0;
    ppich->repetition_period_length = TDSL1_PICH_REPEAT_64_2;
  }
  
  ppich->midamble_and_burst.config = 
       ie_lcr_ptr->midambleShiftAndBurstType.midambleConfiguration - 1;

  /* set the value of midambleAllocationMode */
  midam_mode = &ie_lcr_ptr->midambleShiftAndBurstType.midambleAllocationMode;
  switch (midam_mode->t)
  {
  case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_defaultMidamble:
    ppich->midamble_and_burst.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
    break;
  case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_commonMidamble:
    ppich->midamble_and_burst.allocation_mode = TDSL1_COMMON_MIDAMBLE;
    break;
  case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble:
    ppich->midamble_and_burst.allocation_mode = TDSL1_UE_SPECIFIC_MIDAMBLE;
    ppich->midamble_and_burst.shift = 
          midam_mode->u.ueSpecificMidamble->midambleShift;
    break;
  default:
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad enum %u for midambleAllocationMode for pich ",TDSRRC_GET_MSG_TYPE_PTR(midam_mode));
    return FAILURE;
    }

  /* Initial channelcode bitmap */
  ppich->ch_code_bitmap = 0;
  
  /* set the value of channel code */
  for (channel_cnt = 0; channel_cnt < ie_lcr_ptr->pichChannelisationCodeList_LCR_r4.n; channel_cnt++)
  {
    ppich->ch_code_bitmap |= (1 << ie_lcr_ptr->pichChannelisationCodeList_LCR_r4.elem[channel_cnt]);
  }

  ppich->paging_indicator_length = ie_lcr_ptr->pagingIndicatorLength;
  ppich->num_gap = ie_lcr_ptr->n_GAP;
  ppich->num_paging_group =  ie_lcr_ptr->n_PCH;
      
  return SUCCESS;
} /* tdsrrcllcpcie_get_pich_info() */

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_dpch_common_info_r4()

DESCRIPTION   This function translates the Downlink DPCH info common for
              all RL (radio links) IE into the RRC config database. IEs refer to 10.3.6.19

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_dpch_common_info_r4
(
  /* pointer to the IE */
  tdsrrc_DL_DPCH_InfoCommon_r4 *ie_ptr,

  /* pointer to the desired config page */
  tdsl1_dl_phycfg_dpch_common_info_struct_type *common_info
)
{
  tdsrrc_state_e_type rrc_state;

  /* initialize the optional field bit mask */
  common_info->opt_parm_present_mask = 0;

  /* TBD: rrc_mac_d_HFN_initial_value_present to be used LATER */

  /* Get the timing indication maintenance elem[n_index]. */
  switch (TDSRRC_GET_MSG_TYPE(ie_ptr->cfnHandling))
  {
    case T_tdsrrc_DL_DPCH_InfoCommon_r4_cfnHandling_initialise:
    {
      uint32 mac_d_hfn_l = TDSRRCSMC_INVALID_HFN_VALUE;


      if ((!tdsordered_config_ptr->is_hho)&&(tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)) 
      {
       /* For fault-tolerance, we may accept this case for reconfiguration in same cell*/
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"We do fault-tolerance for the cfn=initialized when it is not for HHO");
       common_info->timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_INIT;
      }
      else
      {
  
        common_info->timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_INIT;
        /* DL-DPCH-InfoCommon-r4: IE dummy is not used in this version of the specification
                The IE should not be sent and if received it should be ignored */
  
  
         /* From Spec, NW should include mac-d-hfn only when TM bearers are
              present for HHO or non-DCH->DCH. */
        if(TRUE == tdsrrcsmc_tm_ciph_active())
        {
          rrc_state = tdsrrc_get_state();
          if( (rrc_state == TDSRRC_STATE_CELL_FACH) &&
             (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
          {
            /* It will be called in the procedure itself */
          }
          else
          {
            if(FAILURE == tdsrrcrb_get_mac_d_hfn(
               (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                        mac_d_HFN_initial_valuePresent)),
               &ie_ptr->mac_d_HFN_initial_value,
               &mac_d_hfn_l))
            {
              return FAILURE;
            }
            if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l)       
               == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"process_cipher_config call failed");
            }
          }
        }
      }
          
    }

      break;

    case T_tdsrrc_DL_DPCH_InfoCommon_r4_cfnHandling_maintain:
      rrc_state = tdsrrc_get_state();
      /*Remove the hack for cfnHandling, unless it is needed for conformance test again */
/*
      if(rrc_state != TDSRRC_STATE_CELL_DCH &&
          tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring TM HHO on state transition");
        common_info->timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_INIT;
      }
      else
*/
      {
        common_info->timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad CFN handling choice %u",TDSRRC_GET_MSG_TYPE(ie_ptr->cfnHandling));
      return FAILURE;
  }

  //common_info->opt_parm_present_mask |= TDSL1DLPHYCFG_DPCH_COMMON_INFO_TIMING_IND_PRESENT;

  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_DPCH_InfoCommon_r4_modeSpecificInfo_tdd))
  {

    /* Get the Power control info if present(IE refer to 10.3.6.23) */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
                  dl_DPCH_PowerControlInfoPresent))
    {
      if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->
                            dl_DPCH_PowerControlInfo.modeSpecificInfo,
                  T_tdsrrc_DL_DPCH_PowerControlInfo_modeSpecificInfo_tdd))
      {
        /* TPC Step Size info */
        if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd->dl_DPCH_PowerControlInfo.modeSpecificInfo.u.tdd,
                tpc_StepSizeTDDPresent))
        {
          /** the value range of tpc_step_size is 1, 2, 3.
                The first num of tpc_step_size in OC struct is zero */
          common_info->tpc_step_size = 
            ie_ptr->modeSpecificInfo.u.tdd->dl_DPCH_PowerControlInfo.modeSpecificInfo.u.tdd->tpc_StepSizeTDD;
        }
        else
        {
          common_info->tpc_step_size = TDSL1_TPC_STEP_SIZE_0DB;
        }
        /* Indicate the DPCH Pwr Ctrl presence in the bitmask */
        common_info->opt_parm_present_mask |=
          TDSL1_DL_PHYCFG_DPCH_COMMON_INFO_TPC_STEP_SIZE_PRESENT;
      }
      else
      {
        /* bad news.  we're in the TDD portion of the DL Common info IE
           but the DPCH power control info IE hasn't selected TDD, instead
           it selected FDD */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"DL Common TDD but DPCH Power ctrl not TDD");
        return FAILURE;
      }
    }


  } /* end DPCH TDD chsoen */
  else
  {
    /* we don't support FDD, return FALSE indicating an error */
    return FAILURE;
  }

  /* if we made it here all is well */
  return SUCCESS;
} /* tdsrrcllcpcie_get_dl_dpch_common_info_r4() */

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r5()

FILE NAME     This function gets the DL DPCH info for each RL

DESCRIPTION   This function translates the Downlink DPCH info for each
              RL IE to the RRC config database.  Note that it is assumed
              that a higher level routine is doing the translation from
              the linked list present in the ASN.1 decoder output to
              the array present in the RRC config database.  The radio
              link index is passed into this function and must be
              generated by that higher level routine. IEs refer to 10.3.6.21.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if everything is ok, FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all of the parameters
              that are present in the IE.  For those parameters not
              present the values currently existing are untouched.  For
              each optional parameter that is present the corresponding
              bit in the optional parameters presence bitmask is set.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r5
(
  /* pointer to the IE */
  tdsrrc_DL_DPCH_InfoPerRL_r5 *ie_ptr,

  /* pointer to the desired DL PhyChan parameter structure */
  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,
              rrc_DL_DPCH_InfoPerRL_r5_tdd))
  {
    /** TDD support only one CCTrCH. If OTA message send ul_CCTrCHListToRemove,
         RRC consider it invalid */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr->u.tdd,
               dl_CCTrChListToRemove))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDD only have one CCTrCH,dl_CCTrCHListToRemove is invalid");
      return FAILURE;
    }

    /* Get Downlink CCTrCH LIST from OTA message */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr->u.tdd,
               dl_CCTrChListToEstablish))
    {
      if (1 < ie_ptr->u.tdd->dl_CCTrChListToEstablish.n)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"dl_CCTrCHList %d is invalid",(ie_ptr->u.tdd->dl_CCTrChListToEstablish.n));
        return FAILURE;
      }

      if (FAILURE == tdsrrcllcpcie_get_dl_cctrch_list_r4(
                       &ie_ptr->u.tdd->dl_CCTrChListToEstablish.elem[0],
                                 &tdsordered_config_ptr->l1_dl_chan_parms.dpch))
      {
        return FAILURE;
      }
    }
    else
    {
    /*dl cctrch list is optional, and it must be present from non_DCH to DCH.
         So RRC add this check. DL_DPCH_InfoPerRL only be used in DCH.
         Then next state must be DCH. So here RRC only check current state. */
      if (rrc_state != TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"dl_CCTrCHList is not present from non-DCH to DCH");
        return FAILURE;
      }
    }
  }
  else
  {
    /* TDD choice not selected, FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD not supported");
    return FAILURE;
  }

  /* everything went ok */
  return SUCCESS;
} /* tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r5() */

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_dpch_info_for_each_rl()

FILE NAME     This function gets the DL DPCH info for each RL

DESCRIPTION   This function translates the Downlink DPCH info for each
              RL IE to the RRC config database.  Note that it is assumed
              that a higher level routine is doing the translation from
              the linked list present in the ASN.1 decoder output to
              the array present in the RRC config database.  The radio
              link index is passed into this function and must be
              generated by that higher level routine.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if everything is ok, FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all of the parameters
              that are present in the IE.  For those parameters not
              present the values currently existing are untouched.  For
              each optional parameter that is present the corresponding
              bit in the optional parameters presence bitmask is set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_dpch_info_for_each_rl
(
  /* pointer to the IE */
  tdsrrc_DL_DPCH_InfoPerRL *ie_ptr,

  /* pointer to the desired DL PhyChan parameter structure */
  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan,

  /* radio link index */
  uint8 rl_idx,

  /* are we setting up a new radio link? */
  boolean is_new_rl
)
{
/* add comment for 10.3.6.21 begin(r3 del) */
#if 0
  tdsl1dl_info_per_rl_struct_type *rl_info;

  tdsrrc_DL_ChannelisationCodeList *code_list_ptr;
  uint16 code_ch;
  uint32 n_index = 0;

  tdsrrc_SF512_AndCodeNumber sf_choice;

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,
              rrc_DL_DPCH_InfoPerRL_fdd))
  {
    if (rl_idx < TDSUE_MAX_RL)
    {
      rl_info = &dl_phychan->dl_per_rl[rl_idx];
    }
    else
    {
      /* rl_idx too large for supported number of radio links */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rl_idx out of range");
      return FAILURE;
    }

    /* initialize the optional field bit mask */
    if(is_new_rl)
    {
      rl_info->dpch_per_rl.opt_parms_present_mask = 0;
    }


    /* Get the CPICH usage for channel estimation */
    switch (ie_ptr->u.fdd->pCPICH_UsageForChannelEst)
    {
      case tdsrrc_PCPICH_UsageForChannelEst_mayBeUsed:
        rl_info->dpch_per_rl.use_pri_cpich = TDSL1DLPHYCFG_USE_PRI_CPICH;
        break;

      case tdsrrc_PCPICH_UsageForChannelEst_shallNotBeUsed:
        rl_info->dpch_per_rl.use_pri_cpich = TDSL1DLPHYCFG_DO_NOT_USE_PRI_CPICH;
        break;

      default:
        /* bad enum value, return FALSE immediately */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Bad CPICH usage enum");
        return FAILURE;
    }

    /* Get the TPC Combination index */
    rl_info->dpch_per_rl.tpc_idx =
      (uint8) ie_ptr->u.fdd->tpc_CombinationIndex;


    /* Get the closed loop timing adjustment type */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->u.fdd,
closedLoopTimingAdjModePresent))
    {
      if (dl_phychan->dl_common.tx_div_mode == TDSL1DLPHYCFG_TX_DIV_NONE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore CLT info in %d TXDivMode",dl_phychan->dl_common.tx_div_mode);
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Indicate CLT info in %d TXDivMode",dl_phychan->dl_common.tx_div_mode);
        /* a cast here is ok as the mapping is as expected */
        rl_info->dpch_per_rl.closed_loop_timing_adj_mode =
          (uint8) ie_ptr->u.fdd->closedLoopTimingAdjMode;

        /* mark the Closed loop timing adjustment info as present */
        rl_info->dpch_per_rl.opt_parms_present_mask |=
          DL_DPCH_INFO_PER_CLOSED_LOOP_TIMING_ADJ_PRESENT;
      }
    }
    else
    {
      if(rl_info->dpch_per_rl.opt_parms_present_mask & DL_DPCH_INFO_PER_CLOSED_LOOP_TIMING_ADJ_PRESENT)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"clearing CLT ");
        rl_info->dpch_per_rl.opt_parms_present_mask &=
         ~DL_DPCH_INFO_PER_CLOSED_LOOP_TIMING_ADJ_PRESENT;
      }
    }

    /* Get the secondary CPICH information */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->u.fdd,
                secondaryCPICH_InfoPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Secondary CPICH Info not supported");
      return FAILURE;
    }
    /* secondary cpich info is not present. It might have been present
    for the same radiolink before. Make sure that L1 gets
    correct bit masks */
    else
    {
      if (rl_info->dpch_per_rl.opt_parms_present_mask &
        DL_DPCH_INFO_PER_RL_SEC_CPICH_INFO_PRESENT)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sec CPICH was present before rl_id:%d",rl_idx);

        /* mark the secondary CPICH information as present */
        rl_info->dpch_per_rl.opt_parms_present_mask &=
          ~DL_DPCH_INFO_PER_RL_SEC_CPICH_INFO_PRESENT;
      }
    }

    /* Get all of the DPCH code channel list information */
    code_list_ptr = &(ie_ptr->u.fdd->dl_ChannelisationCodeList);
    code_ch = 0;
    
    while ((code_list_ptr->n > n_index) && (code_ch < UE_MAX_DPCH))
    {
      /* Initialize the optional parameters for each code channel */
      if(is_new_rl)
      {
        rl_info->dpch_per_rl.dl_chan_code[code_ch].opt_parm_present_mask = 0;
      }

      /* decode the Spreading factor and OVSF code */
      sf_choice = code_list_ptr->elem[n_index].sf_AndCodeNumber;
      switch (TDSRRC_GET_MSG_TYPE(sf_choice))
      {
        case T_tdsrrc_SF512_AndCodeNumber_sf4:
          rl_info->dpch_per_rl.dl_chan_code[code_ch].code_number =
            (uint16)sf_choice.u.sf4;
          break;

        case T_tdsrrc_SF512_AndCodeNumber_sf8:
          rl_info->dpch_per_rl.dl_chan_code[code_ch].code_number =
            (uint16)sf_choice.u.sf8;
          break;

        case T_tdsrrc_SF512_AndCodeNumber_sf16:
          rl_info->dpch_per_rl.dl_chan_code[code_ch].code_number =
            (uint16)sf_choice.u.sf16;
          break;

        case T_tdsrrc_SF512_AndCodeNumber_sf32:
          rl_info->dpch_per_rl.dl_chan_code[code_ch].code_number =
            (uint16)sf_choice.u.sf32;
          break;

        case T_tdsrrc_SF512_AndCodeNumber_sf64:
          rl_info->dpch_per_rl.dl_chan_code[code_ch].code_number =
            (uint16)sf_choice.u.sf64;
          break;

        case T_tdsrrc_SF512_AndCodeNumber_sf128:
          rl_info->dpch_per_rl.dl_chan_code[code_ch].code_number =
            (uint16)sf_choice.u.sf128;
          break;

        case T_tdsrrc_SF512_AndCodeNumber_sf256:
          rl_info->dpch_per_rl.dl_chan_code[code_ch].code_number =
            (uint16)sf_choice.u.sf256;
          break;

        case T_tdsrrc_SF512_AndCodeNumber_sf512:
          rl_info->dpch_per_rl.dl_chan_code[code_ch].code_number =
            (uint16)sf_choice.u.sf512;
          break;

        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Bad SF code %u for RL #%u, DPCH #%u",
                    TDSRRC_GET_MSG_TYPE(sf_choice), rl_idx, code_ch);
          return FAILURE;
      }

      /* Get the secondary scrambling code if present */
      if (TDSRRC_CHECK_MSG_TYPE_IE(code_list_ptr->elem[n_index],
                     secondaryScramblingCodePresent))
      {
        rl_info->dpch_per_rl.dl_chan_code[code_ch].sec_scr_code =
          (uint16) code_list_ptr->elem[n_index].secondaryScramblingCode;

        rl_info->dpch_per_rl.dl_chan_code[code_ch].opt_parm_present_mask |=
          DL_DPCH_CODE_CH_SEC_SCR_CODE_PRESENT;
      }
      /* Get the secondary scrambling code if present */
      else
      {
        if (rl_info->dpch_per_rl.dl_chan_code[code_ch].opt_parm_present_mask &
          DL_DPCH_CODE_CH_SEC_SCR_CODE_PRESENT)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sec SCR was present before %d",rl_info->dpch_per_rl.dl_chan_code[code_ch].sec_scr_code);
        }
        rl_info->dpch_per_rl.dl_chan_code[code_ch].opt_parm_present_mask &=
          ~DL_DPCH_CODE_CH_SEC_SCR_CODE_PRESENT;
      }


      /* point to the next code channel and increment the code channel
         index variable */
      n_index++;
      code_ch++;
    }

    /* set the number of DPCH code channels used for this radio link */
    rl_info->dpch_per_rl.num_dpch = code_ch;
  }
  else
  {
    /* FDD choice not selected, TDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDD not supported");
    return FAILURE;
  }
#endif
/* add comment for 10.3.6.21 end */
  /* everything went ok */
  return SUCCESS;
} /* tdsrrcllcpcie_get_dl_dpch_info_for_each_rl() */

/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_common_for_all_rl()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_rl
(
  /* Pointer to the DL CommonInformation IE */
  tdsrrc_DL_CommonInformation *ie_ptr,

  /* Pointer to the desirec DL PhyChan common information structure */
  tdsl1_dl_phycfg_dpch_parms_struct_type *dl_common
)
{
/* add comment for 10.3.6.24 begin(r3 del, HandoverToUTRANCommand) */
#if 0
  tdsrrc_state_e_type   rrc_state;
  boolean compressed_mode_config_ignore = FALSE;
  uint32 n_index = 0;
  
  struct tdsrrc_TGP_SequenceList *local_tgp_SequenceList_ptr = NULL;

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Info Common for all RL");

  /* initialize the optional field bit mask */
  /* Don't reset DOFF bitmask in DCH state.*/
  dl_common->opt_parm_presence_bitmask = 
      dl_common->opt_parm_presence_bitmask & TDSL1DLPHYCFG_COMMON_INFO_DEFAULT_DPCH_OFFSET_PRESENT;

  /* Get the info common for all DL DPCH if present */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
               dl_DPCH_InfoCommonPresent))
  {
    if(tdsget_dl_dpch_common_info(&ie_ptr->dl_DPCH_InfoCommon,
                               &dl_common->dpch_common,
                               l1_dl_cctrch_parms,
                               l1_dl_ctfc_info
                              ) == FAILURE
      )
    {
      return FAILURE;
    }
  } /* end if dl dpch common info present */

  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_CommonInformation_modeSpecificInfo_fdd))
  {
    /* Get the TX Diversity Mode info. */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.fdd,
                  tx_DiversityModePresent))
    {
      if(tdsget_tx_diversity_mode_info(ie_ptr->modeSpecificInfo.u.fdd->tx_DiversityMode,
                                    dl_common) == FAILURE)
      {
        return FAILURE;
      }
    } /* end Tx Diversity Prescence */

      /* Get the default DPCH offset if present */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.fdd,
                    defaultDPCH_OffsetValuePresent))
    {
      dl_common->default_dpch_offset =
        (uint16) ie_ptr->modeSpecificInfo.u.fdd->defaultDPCH_OffsetValue;

      if(dl_common->default_dpch_offset > TDSRRCLLC_ASN1_DOFF_MAX_VAL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Doff ASN1 value %d out of range");
        return(FAILURE);
      }
      /* Multiply the ASN1 value with multiplication factor before sending
       * it to L1.
       */
      dl_common->default_dpch_offset  *= TDSRRCLLC_DOFF_MULTIPLICATION_FACTOR;
      dl_common->opt_parm_presence_bitmask |=
        TDSL1DLPHYCFG_COMMON_INFO_DEFAULT_DPCH_OFFSET_PRESENT;
    }
    else
    {
      rrc_state = tdsrrc_get_state();
      if(rrc_state != TDSRRC_STATE_CELL_DCH)
      {
        if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
        {
          /* This is an error case since we are going to Cell_DCH from
           * another state and Doff is not signalled
           */
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"Doff absent RRC state curr%d next%d",rrc_state,tdsordered_config_ptr->rrc_state_indicator);
          return(FAILURE);
        }
      }

      if(dl_common->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_INIT &&
          check_hho_cfn_handling)
      {
        /* This is an error case since we are doing a timing initialized
         * hard handover and Doff is not signalled
         */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Doff absent during timing-init HHO");
        return FAILURE;
      }
    }  /* Doff is absent */

    /* We're done with this flag now. */
    check_hho_cfn_handling = FALSE;

  } /* end if common FDD */
  else
  {
    /* FDD not selected.  Error as TDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No FDD info present in DL_INFO_COMMON_FOR_ALL_RL");
    return FAILURE;
  }
  /* Indicate that Downlink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
 /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
#endif
  return SUCCESS;
} /* tdsrrcllc_get_dl_info_common_for_all_rl() */
/* add comment for 10.3.6.24 end */


/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_common_for_all_rl_r5()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_rl_r5
(
  /* Pointer to the DL CommonInformation IE */
  tdsrrc_DL_CommonInformation_r5 *ie_ptr,

  /* Pointer to the desirec DL PhyChan common information structure */
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
)
{
  tdsrrc_state_e_type   rrc_state;
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Info Common for all RL");

  /* Check if HS Reset is indicated */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                mac_hsResetIndicatorPresent))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSReset Indicator TRUE");
    /* Set it in MAC params */
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = TRUE;
    tdshsdpa_msg_params.msg_params_present = TRUE;
  }

  /* Get the info common for all DL DPCH if present */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                dl_DPCH_InfoCommonPresent))
  {
    if(tdsrrcllcpcie_get_dl_dpch_common_info_r4(&ie_ptr->dl_DPCH_InfoCommon,
                               &dpch->dpch_common
                              ) == FAILURE
      )
    {
      return FAILURE;
    }
  } /* end if dl dpch common info present */

/* Get Downlink information common for all radio links info(10.3.6.24) */
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_CommonInformation_r5_modeSpecificInfo_tdd))
  {
        /* Get tstd indicator */
      if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->tddOption,
            T_tdsrrc_DL_CommonInformation_r5_tddOption_tdd128))
      {
        dpch->dpch_common.tstd_indicator =
          ie_ptr->modeSpecificInfo.u.tdd->tddOption.u.tdd128->tstd_Indicator;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd->tddOption is not tdd128");
        return FAILURE;
      }

      /* Get the default DPCH offset if present */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
                    defaultDPCH_OffsetValuePresent))
    {
      /* DefaultDPCH-OffsetValueTDD ::= INTEGER (0..7) */
      dpch->doff =
        (uint8) ie_ptr->modeSpecificInfo.u.tdd->defaultDPCH_OffsetValue;

      if(dpch->doff > TDSRRCLLC_ASN1_DOFF_MAX_VAL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Doff ASN1 value out of range");
        return(FAILURE);
      }
      dpch->opt_parm_presence_bitmask |=
        TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
    }
    else /*DOFF is not set*/
    {
      rrc_state = tdsrrc_get_state();

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_INIT)
      {
        /*For HHO, assign DOFF = 0 to L1*/
        if((tdsordered_config_ptr->is_hho)&&(rrc_state == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Non-DCH ->DCH case. Pending clarification from NW. If yes, merge with above case*/
        else if(rrc_state != TDSRRC_STATE_CELL_DCH)
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Return failure since DOFF is missing, e.g. DCH->DCH reconfig case*/
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"We do fault-tolerance for DOFF is missing");
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;

        }

      }

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN)
      {
        /*For Non-DCH -> DCH case, make DOFF = 0.*/
        if((rrc_state != TDSRRC_STATE_CELL_DCH)&&(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
          dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
      }

    }  /* Doff is absent */
  } /* end if common FDD */
  else
  {
    /* TDD not selected.  Error as FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_COMMON_FOR_ALL_RL");
    return FAILURE;
  }

  /* Indicate that Downlink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
 /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  return SUCCESS;
} /* tdsrrcllc_get_dl_info_common_for_all_rl_r5() */






/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_for_each_rl_r5()

DESCRIPTION     This function translates the Downlink information for
                each radio link IE to the RRC config database

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllc_get_dl_info_for_each_rl_r5
(
  tdsrrc_DL_InformationPerRL_r5 *ie_ptr,

  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
)
{
  /* In TDD, RL is only one, will check in "tdsrrcllc_get_dl_info_per_rl_r5" */

  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Get Dl Info per RL");
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
              T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd))
  {
    /* First check if the DL DPCH Info per RL is included. If not,
    * this is NOT a valid Radio link - ignore it.
    */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
              dl_DPCH_InfoPerRLPresent))
    {
      /* dl_phychan->dl_per_rl[rl_idx].pri_cpich_scr_code do not need */

      /* save tstd info */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
        rrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption,
            rrc_PrimaryCCPCH_Info_r4_tddOption_tdd128))
        {
          dl_phychan->pccpch.tstd_indicator =
            ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption.u.tdd128->tstd_Indicator;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD128 info present in DL_INFO_FOR_EACH_RL");
          return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }

      /*If Cell_id is present for this RL, store it in tdscell_id_per_rl */
      if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                cell_idPresent))
      {
        tdscell_id_per_rl[0] = (uint32) tdsrrc_translate_cell_id(&(ie_ptr->cell_id));
      }       

      /* Get the DL DPCH info for this radio link */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get DL DPCH Info for RL",1);

      if (tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r5(&ie_ptr->dl_DPCH_InfoPerRL,
        dl_phychan) == FAILURE)
      {
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }
      /* if we made it here we must be ok */
      /* Indicate that Downlink L1 needs to be configured */
      tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
      * still needs to be reconfigured so as to get the new CCTrCH
      * ID whenever Downlink L1 is reconfigured. This is due to the
      * interface requirements.
      */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
      return TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS;
    }  /* DL DPCH Info is present */
    else
    {
      /* DL DPCH Info per RL is NOT present - ignore this IE */
      return TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
    }
  }
  else
  {
    /* No TDD info.  We don't support FDD so return indicating an error */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
    return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
  }
} /* tdsrrcllc_get_dl_info_for_each_rl_r5() */





/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_dl_rl_list

DESCRIPTION:

      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
#if 0
uecomdef_status_e_type tdsrrcllc_get_predefined_dl_rl_list
(
  struct tdsrrc_DL_InformationPerRL_List   *dl_rl_ptr,
  tdsrrc_DL_InformationPerRL_ListPostFDD  *dl_rl_post_ptr,
  tdsrrc_DL_CommonInformation        dl_common_info
)  
{
  uint8 idx = 0;
  struct tdsrrc_DL_ChannelisationCodeList *tmp_dl_chan_code;
  uint32 n_index = 0;

  n_value = dl_rl_post_ptr->n;
  
  while((dl_rl_post_ptr->n > n_index) && idx < TDSUE_MAX_RL)
  {
    TDSRRC_RESET_MSG_IE_PRESENT(dl_rl_ptr[idx].elem[n_index]);
    dl_rl_ptr[idx].elem[n_index].bit_mask |= rrc_DL_InformationPerRL_dl_DPCH_InfoPerRL_present;
    dl_rl_ptr[idx].elem[n_index].modeSpecificInfo.u.fdd->bit_mask = 0;
    dl_rl_ptr[idx].elem[n_index].modeSpecificInfo.choice =
      rrc_DL_InformationPerRL_modeSpecificInfo_fdd_chosen;
    dl_rl_ptr[idx].elem[n_index].modeSpecificInfo.u.fdd->primaryCPICH_Info =
      dl_rl_post_ptr->elem[n_index].primaryCPICH_Info;
    dl_rl_ptr[idx].elem[n_index].dl_DPCH_InfoPerRL.u.fdd->bit_mask = 0;
    dl_rl_ptr[idx].elem[n_index].dl_DPCH_InfoPerRL.choice =
      rrc_DL_DPCH_InfoPerRL_fdd_chosen;
    dl_rl_ptr[idx].elem[n_index].dl_DPCH_InfoPerRL.u.fdd->pCPICH_UsageForChannelEst =
      dl_rl_post_ptr->elem[n_index].dl_DPCH_InfoPerRL.pCPICH_UsageForChannelEst;
    dl_rl_ptr[idx].elem[n_index].dl_DPCH_InfoPerRL.u.fdd->pCPICH_UsageForChannelEst =
       tdsrrc_PCPICH_UsageForChannelEst_mayBeUsed;
    dl_rl_ptr[idx].elem[n_index].dl_DPCH_InfoPerRL.u.fdd->dpch_FrameOffset = 
     ((dl_common_info.modeSpecificInfo.u.fdd->defaultDPCH_OffsetValue * 512)
       % 38400) / 256; /* 25.331, clause 8.3.5.1.2 */


    tmp_dl_chan_code.next = NULL;
    tmp_dl_chan_code.value =
      dl_rl_post_ptr->elem[n_index].dl_DPCH_InfoPerRL.dl_ChannelisationCode;
    dl_rl_ptr[idx].elem[n_index].dl_DPCH_InfoPerRL.u.fdd->dl_ChannelisationCodeList =
      &tmp_dl_chan_code;
    dl_rl_ptr[idx].elem[n_index].dl_DPCH_InfoPerRL.u.fdd->tpc_CombinationIndex =
      dl_rl_post_ptr->elem[n_index].dl_DPCH_InfoPerRL.tpc_CombinationIndex;
    
    n_index++;
    idx++;
  }
  dl_rl_ptr[idx > 0 ? idx - 1 : 0].next = NULL;

  return SUCCESS;
}
#endif

/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_dl_common_info

DESCRIPTION:

      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_predefined_dl_common_info
(
  tdsrrc_DL_CommonInformation        *dl_common_info_ptr,
  tdsrrc_DL_CommonInformationPost    *dl_common_info_post_ptr,
  tdsrrc_PreDefPhyChConfiguration *predef_phych,
  tdsrrc_U_RNTI_Short *new_urnti_ptr
)  
{
  uint16     srnti2  = 0; /* S-RNTI 2 portion of the U-RNTI in the message,
                             used later to calculate the default DPCH offset
                             value in preconfigured mode */

  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd = tdsrrc_malloc(sizeof(tdsrrc_DL_DPCH_InfoCommon_fdd));
  if (dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
    return FAILURE;
  }
  dl_common_info_ptr->modeSpecificInfo.u.fdd = tdsrrc_malloc(sizeof(tdsrrc_DL_CommonInformation_fdd));
  if (dl_common_info_ptr->modeSpecificInfo.u.fdd == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
    return FAILURE;
  }

  TDSRRC_RESET_MSG_IE_PRESENT_PTR(dl_common_info_ptr);
  TDSRRCSET_MSG_TYPE(dl_common_info_ptr->dl_DPCH_InfoCommon.cfnHandling,
               T_tdsrrc_DL_DPCH_InfoCommon_cfnHandling_maintain);
  TDSRRCSET_MSG_TYPE(dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo,
              T_tdsrrc_DL_CommonInformation_modeSpecificInfo_fdd);

  TDSRRC_RESET_MSG_IE_PRESENT_PTR(dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd);
  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->dl_DPCH_PowerControlInfo =
  dl_common_info_post_ptr->dl_DPCH_InfoCommon.dl_DPCH_PowerControlInfo;
  TDSRRCSET_MSG_TYPE_IE_PTR(dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd,
              dl_DPCH_PowerControlInfoPresent);
  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->powerOffsetPilot_pdpdch = 0; /* 25.331, clause 8.3.6.3 */
  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->spreadingFactorAndPilot =
      predef_phych->dl_CommonInformationPredef.dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->spreadingFactorAndPilot;
  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->positionFixedOrFlexible =
      predef_phych->dl_CommonInformationPredef.dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->positionFixedOrFlexible;
  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->tfci_Existence =
      predef_phych->dl_CommonInformationPredef.dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->tfci_Existence;
  TDSRRCSET_MSG_TYPE_IE_PTR(dl_common_info_ptr,
              dl_DPCH_InfoCommonPresent);
  TDSRRCSET_MSG_TYPE(dl_common_info_ptr->modeSpecificInfo,
              T_tdsrrc_DL_CommonInformation_modeSpecificInfo_fdd);
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(dl_common_info_ptr->modeSpecificInfo.u.fdd);

  srnti2 = *(new_urnti_ptr->s_RNTI_2.data) << 8;
  srnti2 |= *(new_urnti_ptr->s_RNTI_2.data + 1);
  srnti2 >>= 6;

  dl_common_info_ptr->modeSpecificInfo.u.fdd->defaultDPCH_OffsetValue =
    (srnti2 % 600); /* 25.331, clause 8.3.6.3 */


  TDSRRCSET_MSG_TYPE_IE_PTR(dl_common_info_ptr->modeSpecificInfo.u.fdd,
               defaultDPCH_OffsetValuePresent);

  return SUCCESS;
}
/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_ul_dpch_info

DESCRIPTION:

      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_predefined_ul_dpch_info
(
  tdsrrc_UL_DPCH_Info *ul_dpch_info_ptr,
  tdsrrc_UL_DPCH_InfoPostFDD *ul_dpch_info_post_ptr,
  tdsrrc_PreDefPhyChConfiguration *predef_phych
)  
{
/* add comment for 10.3.6.88 begin(HandoverToUTRANCommand) */

  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd = tdsrrc_malloc(sizeof(tdsrrc_UL_DPCH_PowerControlInfo_fdd));
  if (ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
    return FAILURE;
  }
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd = tdsrrc_malloc(sizeof(tdsrrc_UL_DPCH_Info_fdd));
  if (ul_dpch_info_ptr->modeSpecificInfo.u.fdd == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
    return FAILURE;
  }

  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ul_dpch_info_ptr);
  TDSRRCSET_MSG_TYPE(ul_dpch_info_ptr->ul_DPCH_PowerControlInfo,
                T_tdsrrc_UL_DPCH_PowerControlInfo_fdd);
  /* This value is provided as an tdsrrc_DPCCH_PowerOffset2 type, which must be converted to the actual elem[n_index].
     * See ASN.1 definition for details.  Then, we must divide by 2 to make the value compatible with existing RRC functions. 
     */ 
  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd->dpcch_PowerOffset =
      (2 + (ul_dpch_info_post_ptr->ul_DPCH_PowerControlInfo.dpcch_PowerOffset * 4)) / 2; 
  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd->pc_Preamble =
    ul_dpch_info_post_ptr->ul_DPCH_PowerControlInfo.pc_Preamble;
  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd->sRB_delay =
    ul_dpch_info_post_ptr->ul_DPCH_PowerControlInfo.sRB_delay;
  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd->powerControlAlgorithm =
      predef_phych->ul_DPCH_InfoPredef.ul_DPCH_PowerControlInfo.u.fdd->powerControlAlgorithm;
  TDSRRCSET_MSG_TYPE_IE_PTR(ul_dpch_info_ptr,
                 ul_DPCH_PowerControlInfoPresent);
  TDSRRCSET_MSG_TYPE(ul_dpch_info_ptr->modeSpecificInfo,
T_tdsrrc_UL_DPCH_Info_modeSpecificInfo_fdd);
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ul_dpch_info_ptr->modeSpecificInfo.u.fdd);
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd->scramblingCodeType =
    ul_dpch_info_post_ptr->scramblingCodeType;
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd->scramblingCode =
    ul_dpch_info_post_ptr->reducedScramblingCodeNumber;
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd->spreadingFactor =
    ul_dpch_info_post_ptr->spreadingFactor;
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd->tfci_Existence =
      predef_phych->ul_DPCH_InfoPredef.modeSpecificInfo.u.fdd->tfci_Existence;
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd->puncturingLimit =
      predef_phych->ul_DPCH_InfoPredef.modeSpecificInfo.u.fdd->puncturingLimit;

/* add comment for 10.3.6.88 end */
  
  return SUCCESS;
}   
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_dl_common_info_srb5

DESCRIPTION: This function updates the DL common info received in Handover to 
             Utran message and the SRB5 Default config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_predefined_dl_common_info_srb5
(
  tdsrrc_DL_CommonInformation        *dl_common_info_ptr,
  tdsrrc_DL_CommonInformationPost    *dl_common_info_post_ptr,
  tdsrrc_DL_DPCH_InfoCommon_r4 *predef_phych,
  tdsrrc_U_RNTI_Short *new_urnti_ptr
)  
{
  uint16     srnti2  = 0; /* S-RNTI 2 portion of the U-RNTI in the message,
                             used later to calculate the default DPCH offset
                             value in preconfigured mode */

  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd = tdsrrc_malloc(sizeof(tdsrrc_DL_DPCH_InfoCommon_fdd));
  if (dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
    return FAILURE;
  }
  dl_common_info_ptr->modeSpecificInfo.u.fdd = tdsrrc_malloc(sizeof(tdsrrc_DL_CommonInformation_fdd));
  if (dl_common_info_ptr->modeSpecificInfo.u.fdd == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
    return FAILURE;
  }

  TDSRRC_RESET_MSG_IE_PRESENT_PTR(dl_common_info_ptr);
  
  TDSRRCSET_MSG_TYPE(dl_common_info_ptr->dl_DPCH_InfoCommon.cfnHandling,
              T_tdsrrc_DL_DPCH_InfoCommon_cfnHandling_maintain);
  
  TDSRRCSET_MSG_TYPE(dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo,
              T_tdsrrc_DL_DPCH_InfoCommon_modeSpecificInfo_fdd);
  
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd);
  
  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->dl_DPCH_PowerControlInfo =
  dl_common_info_post_ptr->dl_DPCH_InfoCommon.dl_DPCH_PowerControlInfo;
  
  TDSRRCSET_MSG_TYPE_IE_PTR(dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd,
              dl_DPCH_PowerControlInfoPresent);
  dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->powerOffsetPilot_pdpdch = 0; /* 25.331, clause 8.3.6.3 */
  
  if(TDSRRC_CHECK_MSG_TYPE(predef_phych->modeSpecificInfo,
              T_tdsrrc_DL_DPCH_InfoCommon_r4_modeSpecificInfo_fdd))
    {
    dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->spreadingFactorAndPilot =
        predef_phych->modeSpecificInfo.u.fdd->spreadingFactorAndPilot;
    dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->positionFixedOrFlexible =
        predef_phych->modeSpecificInfo.u.fdd->positionFixedOrFlexible;
    dl_common_info_ptr->dl_DPCH_InfoCommon.modeSpecificInfo.u.fdd->tfci_Existence =
        predef_phych->modeSpecificInfo.u.fdd->tfci_Existence;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo not present ");
    return FAILURE;
  }
  TDSRRCSET_MSG_TYPE_IE_PTR(dl_common_info_ptr,
              dl_DPCH_InfoCommonPresent);
  
  TDSRRCSET_MSG_TYPE(dl_common_info_ptr->modeSpecificInfo,
              T_tdsrrc_DL_CommonInformation_modeSpecificInfo_fdd);
  
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(dl_common_info_ptr->modeSpecificInfo.u.fdd);

  srnti2 = *(new_urnti_ptr->s_RNTI_2.data) << 8;
  srnti2 |= *(new_urnti_ptr->s_RNTI_2.data + 1);
  srnti2 >>= 6;

  dl_common_info_ptr->modeSpecificInfo.u.fdd->defaultDPCH_OffsetValue =
    (srnti2 % 600); /* 25.331, clause 8.3.6.3 */


  TDSRRCSET_MSG_TYPE_IE_PTR(dl_common_info_ptr->modeSpecificInfo.u.fdd,
             defaultDPCH_OffsetValuePresent);

  return SUCCESS;
}
/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_ul_dpch_info_srb5

DESCRIPTION: This function updates the UL DPCH info received in Handover to 
             Utran message and the SRB5 Default config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_predefined_ul_dpch_info_srb5
(
  tdsrrc_UL_DPCH_Info *ul_dpch_info_ptr,
  tdsrrc_UL_DPCH_InfoPostFDD *ul_dpch_info_post_ptr,
  tdsrrc_UL_DPCH_Info_r5 *predef_phych
)  
{
/* add comment for 10.3.6.88 begin(HandoverToUTRANCommand) */

  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd = tdsrrc_malloc(sizeof(tdsrrc_UL_DPCH_PowerControlInfo_fdd));
  if (ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
    return FAILURE;
  }
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd = tdsrrc_malloc(sizeof(tdsrrc_UL_DPCH_Info_fdd));
  if (ul_dpch_info_ptr->modeSpecificInfo.u.fdd == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to allocate memory");
    return FAILURE;
  }

  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ul_dpch_info_ptr);
  TDSRRCSET_MSG_TYPE(ul_dpch_info_ptr->ul_DPCH_PowerControlInfo,
                T_tdsrrc_UL_DPCH_PowerControlInfo_fdd);
  
  /* This value is provided as an tdsrrc_DPCCH_PowerOffset2 type, which must be converted to the actual elem[n_index].
     * See ASN.1 definition for details.  Then, we must divide by 2 to make the value compatible with existing RRC functions. 
     */ 
     
  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd->dpcch_PowerOffset =
      (2 + (ul_dpch_info_post_ptr->ul_DPCH_PowerControlInfo.dpcch_PowerOffset * 4)) / 2; 
  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd->pc_Preamble =
    ul_dpch_info_post_ptr->ul_DPCH_PowerControlInfo.pc_Preamble;
  ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd->sRB_delay =
    ul_dpch_info_post_ptr->ul_DPCH_PowerControlInfo.sRB_delay;
  if((TDSRRC_CHECK_MSG_TYPE_IE_PTR(predef_phych,
                ul_DPCH_PowerControlInfoPresent)) && 
      (TDSRRC_CHECK_MSG_TYPE(predef_phych->ul_DPCH_PowerControlInfo,
                T_tdsrrc_UL_DPCH_PowerControlInfo_r5_fdd)))
  {
    ul_dpch_info_ptr->ul_DPCH_PowerControlInfo.u.fdd->powerControlAlgorithm =
      predef_phych->ul_DPCH_PowerControlInfo.u.fdd->powerControlAlgorithm;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_DPCH_PowerControlInfo_present not present ");
    return FAILURE;
  }
  TDSRRCSET_MSG_TYPE_IE_PTR(ul_dpch_info_ptr,
              ul_DPCH_PowerControlInfoPresent);
  
  TDSRRCSET_MSG_TYPE(ul_dpch_info_ptr->modeSpecificInfo,
              T_tdsrrc_UL_DPCH_Info_modeSpecificInfo_fdd);
  
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ul_dpch_info_ptr->modeSpecificInfo.u.fdd);
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd->scramblingCodeType =
    ul_dpch_info_post_ptr->scramblingCodeType;
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd->scramblingCode =
    ul_dpch_info_post_ptr->reducedScramblingCodeNumber;
  ul_dpch_info_ptr->modeSpecificInfo.u.fdd->spreadingFactor =
    ul_dpch_info_post_ptr->spreadingFactor;
  if(TDSRRC_CHECK_MSG_TYPE(predef_phych->modeSpecificInfo,
              T_tdsrrc_UL_DPCH_Info_r5_modeSpecificInfo_fdd))
  {
    ul_dpch_info_ptr->modeSpecificInfo.u.fdd->tfci_Existence =
      predef_phych->modeSpecificInfo.u.fdd->tfci_Existence;
    ul_dpch_info_ptr->modeSpecificInfo.u.fdd->puncturingLimit =
      predef_phych->modeSpecificInfo.u.fdd->puncturingLimit;
  }

  /* add comment for 10.3.6.88 end */
  return SUCCESS;
}
#endif

/*====================================================================
FUNCTION      tdsrrcllc_set_hsscch_info_r6()

DESCRIPTION   This function sets the Channelization code required
              for HSPDSCH in L1 structure in OC. IEs refer to 10.3.6.36a.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_set_hsscch_info_r6
(
  tdsrrc_HS_SCCH_Info_r6* hsscch_info_ptr
)
{
  struct tdsrrc_HS_SCCH_Info_r6_tdd128 *tmp_ptr = NULL;
  uint8 i = 0;

  if (TDSRRC_CHECK_MSG_TYPE(hsscch_info_ptr->modeSpecificInfo,
        T_tdsrrc_HS_SCCH_Info_r6_modeSpecificInfo_tdd))
  {
    /* Get HSSCCH info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(hsscch_info_ptr->modeSpecificInfo.u.tdd,
          rrc_HS_SCCH_Info_r6_tdd_tdd128))
    {
       tmp_ptr = hsscch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128;
       
       /** Set common_scch_sich_params_present info 
                TRUE indicate the current IE using shared ck_nack_powoffset,etc */
       tdsordered_config_ptr->l1_hsdpa_info.common_scch_sich_params_present = TRUE;

       /* Get Ack-Nack Power Offset info(R6) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.ack_nack_powoffset =
         tmp_ptr->nack_ack_power_offset;

       /* Get PRXHS-SICH info(R6) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.prx_sich =
          tmp_ptr->power_level_HSSICH;

       /* Get TPC step size(R6) info */
       if (tmp_ptr->tpc_step_size < tdsrrc_HS_SCCH_Info_r6_tpc_step_size_spare1)
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize =
            tmp_ptr->tpc_step_size+1;
       }
       else
       {
         TDSRRC_MSG0(MSG_LEGACY_LOW,"TPC Step Size is absent in OTA message");
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize = TDSL1_TPC_STEP_SIZE_0DB;
       }

       /* Get BLER target(R6) info */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.bler_target =
          tmp_ptr->bler_target;
	   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Set bler_target = %d",tmp_ptr->bler_target);

       /* Get HS-SCCH Set Configuration list(R5) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich =
         tmp_ptr->hS_SCCH_SetConfiguration.n;

       for (i = 0; i < tmp_ptr->hS_SCCH_SetConfiguration.n; i++)
       {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].ts_num_scch =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].timeslotNumber;

          /** OC: scch_chcode_1st = 0, L1_SF16_CH_CODE_1;
                    ASN1: firstChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_1st =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].firstChannelisationCode;

          /** OC: scch_chcode_2nd = 0, L1_SF16_CH_CODE_1;
                     ASN1: secondChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_2nd =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].secondChannelisationCode;

          /* Get midamble info */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.allocation_mode =
            tdsMidambleAllocMode[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode.t];

          if (TDSRRC_CHECK_COMMON_MSG_TYPE(tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode,
                rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.shift =
                tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode.u.ueSpecificMidamble;
          }

          /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
                     ASN1: midambleConfiguration   Integer (1...8), */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.config =
            tdsMidambleConfiguration[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleConfiguration];

          /* Get HS-SICH configuration info */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.ts_num_sich =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.timeslotNumber;

          /* OC: sich_chcode = 0, L1_SF16_CH_CODE_1;
                     ASN1: channelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1*/
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.sich_chcode =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.channelisationCode;

          /* Get midamble info */
          switch (tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleAllocationMode.t)
          {
            case T_tdsrrc_HS_SICH_Configuration_TDD128_r6_midambleAllocationMode_defaultMidamble:
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
                  TDSL1_DEFAULT_MIDAMBLE;
                break;
          
            case T_tdsrrc_HS_SICH_Configuration_TDD128_r6_midambleAllocationMode_ueSpecificMidamble:
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
                  TDSL1_UE_SPECIFIC_MIDAMBLE;
          
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.shift =
                  tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
                break;
            default:
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"hs_sich_configuration.midambleAllocationMode is invalid");
              return FAILURE;
          }

          /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
                     ASN1: midambleConfiguration   Integer (1...8), */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.config =
            tdsMidambleConfiguration[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleConfiguration];
       }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd choice is not tdd128");
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD mode choosen,Bail out");
    return FAILURE;
  }

  return SUCCESS;

}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r6()

DESCRIPTION   This function translates the Uplink DPCH Power Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs can refer to 10.3.6.91 in 25331v716
              of 3GPP.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r6
(
 tdsrrc_UL_DPCH_PowerControlInfo_r6_tdd *tdd_ul_pwrcntrl_info
)
{
/**     2> if the IE "Uplink DPCH power control info" is included in the IE "Uplink DPCH info":
  3> if the IE " UL target SIR " is included ignore the parameter. */

  /* According to 8.6.6.11 in 3GPP 25331v716, if the CHOICE UL OL PC info is set to
       'Broadcast UL OL PC info', set the variable INVALID_CONFIGURATION to true. */
  if (TDSRRC_CHECK_MSG_TYPE(tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling,
              T_tdsrrc_UL_DPCH_PowerControlInfo_r6_ul_OL_PC_Signalling_broadcast_UL_OL_PC_info))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"1.28TDD unsupport broadcast UL OL PC info,failure ");
    return FAILURE;
  }

  /* if tdd choice is tdd384, return failure */
  if (TDSRRC_CHECK_MSG_TYPE(tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->tddOption,
                  T_tdsrrc_UL_DPCH_PowerControlInfo_r6_tddOption_tdd384))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"dpch pwr select unsupport tdd3.84,failure ");
    return FAILURE;
  }

  //save beaconPLEst
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128,
        beaconPLEst))
  {
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
  }

    /** the value range of tpc_step_size is 1, 2, 3.
         The first num of tpc_step_size in OC struct is zero */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tpc_step_size = 
    tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128->tpc_StepSize;

  /* in 3GPP 10.3.6.91, primaryCCPCH_TX_Power is optional. It is not correct.
       Because in 11th chapter, it is not optional. */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.pccpch_tx_pwr = 
    tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->primaryCCPCH_TX_Power;

  return SUCCESS;
} /* end tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r6() */


/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_dpch_info_r6()

DESCRIPTION   This function translates the Uplink DPCH info IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_dpch_info_r6
(
  tdsrrc_UL_DPCH_Info_r6 *ie_ptr
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL DPCH Info");

  /* Get the UL DPCH power control information if present */
  if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
              ul_DPCH_PowerControlInfoPresent))
  {
    if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->ul_DPCH_PowerControlInfo,
              T_tdsrrc_UL_DPCH_PowerControlInfo_r6_tdd))
    {
      if(FAILURE == tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r6(ie_ptr->ul_DPCH_PowerControlInfo.u.tdd))
      {
        return FAILURE;
      }
    }
    else /* only FDD */
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No valid TDD data for UL DPCH Power Control Info");
      return FAILURE;
    }
  }
  else
  {
    /* both in FDD and TDD;
  2> if the IE "Uplink DPCH power control info" is not included in a message used to enter CELL_DCH:
  3> set the variable INVALID_CONFIGURATION to true. */
    if ((tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH) &&
          (rrc_state != TDSRRC_STATE_CELL_DCH))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"no UlDpchPwrInfo from nonDCH to DCH");
      return FAILURE;
    }
  }


  /* Make sure of FDD and get the right parameters */
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
              T_tdsrrc_UL_DPCH_Info_r6_modeSpecificInfo_tdd))
  {
    if(tdsrrcllcpcie_update_uldpch_mode_specific_info(ie_ptr->modeSpecificInfo.u.tdd)
        == FAILURE)
    {
      return FAILURE;
    }
  } /* end TDD if */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD UL DPCH information present");
    return FAILURE;
  }

  /* If made it here, all is well */
  /* Indicate that Uplink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
  return SUCCESS;
} /* end tdsrrcllcpcie_get_ul_dpch_info_r6() */

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_dpch_info_r6()

DESCRIPTION:
  This function updates IE "ul dpch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_dpch_info_r6
(
  uint32 ul_dpch_info_present,
  tdsrrc_UL_DPCH_Info_r6 *ul_dpch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Get Uplink DPCH info if included, Ignore CPCH info */
  if (ul_dpch_info_present)
  {
    if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
    {
      if(tdsrrcllcpcie_get_ul_dpch_info_r6(ul_dpch_info_ptr) == FAILURE)
      {
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: Ignoring rcvd ulDpchInfo when state-indicator is NOT set to DCH");
    }
  }
  return(SUCCESS);
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_validate_transition_to_cell_dch()

DESCRIPTION:
  This function validates whether DL common Info and DL per RL info is present or not,
  when UE is transitioned from non-cell DCH to cell-DCH in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_validate_transition_to_cell_dch
(
  uint32 ul_dpch_info_present,
  uint32 dl_common_info_present,
  uint32 dl_per_rl_info_list_present,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Make sure that if we are transitioning to Cell DCH state that we have
     DL DPCH info in the message. */
  if((rcvd_state_indicator == tdsrrc_RRC_StateIndicator_cell_DCH) &&
     (tdscurrent_config_ptr->rrc_state_indicator != TDSRRC_STATE_CELL_DCH) &&
     (!((dl_common_info_present) && (dl_per_rl_info_list_present) && 
        (ul_dpch_info_present))))
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Incomplete Physical ChInfo for DCH. dlCommonInfo: %d, dlPerRl_info: %d, ulDpchInfo: %d",
      (dl_common_info_present != 0), (dl_per_rl_info_list_present !=0), 
      (ul_dpch_info_present != 0));
    return FAILURE;
  }

  return(SUCCESS);
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_dpch_common_info_r6()

DESCRIPTION   This function translates the Downlink DPCH info common for
              all RL (radio links) IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_dpch_common_info_r6
(
  /* pointer to the IE */
  tdsrrc_DL_DPCH_InfoCommon_r6 *ie_ptr,

  /* pointer to the desired config page */
  tdsl1_dl_phycfg_dpch_common_info_struct_type *common_info
)
{
  tdsrrc_state_e_type rrc_state;

  /* initialize the optional field bit mask */
  common_info->opt_parm_present_mask = 0;

  /* TBD: rrc_mac_d_HFN_initial_value_present to be used LATER */

  /* Get the timing indication maintenance elem[n_index]. */
  switch (TDSRRC_GET_MSG_TYPE(ie_ptr->cfnHandling))
  {
    case  T_tdsrrc_DL_DPCH_InfoCommon_r6_cfnHandling_initialise:
    {
      uint32 mac_d_hfn_l = TDSRRCSMC_INVALID_HFN_VALUE;

      
      if ((!tdsordered_config_ptr->is_hho)&&(tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)) 
      {
       /* For fault-tolerance, we may accept this case for reconfiguration in same cell*/
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"We do fault-tolerance for the cfn=initialized when it is not for HHO");
       common_info->timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_INIT;
      }
      else
      {
  
        common_info->timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_INIT;
  
         /* From Spec, NW should include mac-d-hfn only when TM bearers are
              present for HHO or non-DCH->DCH. */
        if(TRUE == tdsrrcsmc_tm_ciph_active())
        {
          rrc_state = tdsrrc_get_state();
          if( (rrc_state == TDSRRC_STATE_CELL_FACH) &&
             (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
          {
            /* It will be called in the procedure itself */
          }
          else
          {
            if (FAILURE == tdsrrcrb_get_mac_d_hfn(
                (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                          mac_d_HFN_initial_valuePresent)), 
                &ie_ptr->mac_d_HFN_initial_value, &mac_d_hfn_l))
            {
              return FAILURE;
            }
            if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l)
               == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"process_cipher_config call failed");
            }
          }
        }
      }
    }

    break;

    case T_tdsrrc_DL_DPCH_InfoCommon_r6_cfnHandling_maintain:
      rrc_state = tdsrrc_get_state();
      /*Remove the hack for cfnHandling, unless it is needed for conformance test again */
/*
      if(rrc_state != TDSRRC_STATE_CELL_DCH &&
          tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring TM HHO on state transition");
        common_info->timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_INIT;
      }
      else       
*/             
      {
        common_info->timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad CFN handling choice %u",TDSRRC_GET_MSG_TYPE(ie_ptr->cfnHandling));
      return FAILURE;
  }

  //common_info->opt_parm_present_mask |= TDSL1DLPHYCFG_DPCH_COMMON_INFO_TIMING_IND_PRESENT;

  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_DPCH_InfoCommon_r6_modeSpecificInfo_tdd))
  {
    /* Get the Power control info if present(IE refer to 10.3.6.23) */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
                  dl_DPCH_PowerControlInfoPresent))
    {
      if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->
                          dl_DPCH_PowerControlInfo.modeSpecificInfo,
                  T_tdsrrc_DL_DPCH_PowerControlInfo_modeSpecificInfo_tdd))
      {
          /* TPC Step Size info */
          if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd->
                dl_DPCH_PowerControlInfo.modeSpecificInfo.u.tdd,
                  tpc_StepSizeTDDPresent))
          {
            /** the value range of tpc_step_size is 1, 2, 3.
                   The first num of tpc_step_size in OC struct is zero */
            common_info->tpc_step_size = 
              ie_ptr->modeSpecificInfo.u.tdd->dl_DPCH_PowerControlInfo.modeSpecificInfo.u.tdd->tpc_StepSizeTDD;
          }
          else
          {
            common_info->tpc_step_size = TDSL1_TPC_STEP_SIZE_0DB;
          }

        /* Indicate the DPCH Pwr Ctrl presence in the bitmask */
        common_info->opt_parm_present_mask |=
          TDSL1_DL_PHYCFG_DPCH_COMMON_INFO_TPC_STEP_SIZE_PRESENT;
      }
      else
      {
        /* bad news.  we're in the TDD portion of the DL Common info IE
           but the DPCH power control info IE hasn't selected TDD, instead
           it selected FDD */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"DL Common TDD but DPCH Power ctrl not TDD");
        return FAILURE;
      }
    }
  } /* end DPCH TDD chsoen */
  else
  {
    /* we don't support FDD, return FALSE indicating an error */
    return FAILURE;
  }
#ifdef FEATURE_TDSCDMA_CPC_DTX
  if(tdsrrcllcpcie_calculate_dpcch_slot_format() == 4)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCCPC:Slot format#4 config not valid for DL-DPCH");
    return FAILURE;
  }
#endif
  /* if we made it here all is well */
  return SUCCESS;
} /* tdsrrcllcpcie_get_dl_dpch_common_info_r6() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_common_for_all_rl_r6()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_info_common_for_all_rl_r6
(
  /* Pointer to the DL CommonInformation IE */
  tdsrrc_DL_CommonInformation_r6 *ie_ptr,

  /* Pointer to the desirec DL PhyChan common information structure */
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
)
{
  tdsrrc_state_e_type rrc_state;
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Info Common for all RL");


  /* Check if HS Reset is indicated */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                mac_hsResetIndicatorPresent))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSReset Indicator TRUE");
    /* Set it in MAC params */
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = TRUE;
    tdshsdpa_msg_params.msg_params_present = TRUE;
  }
  /* Get the info common for all DL DPCH if present */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                dl_dpchInfoCommonPresent))
  {
    if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->dl_dpchInfoCommon,
                T_tdsrrc_DL_CommonInformation_r6_dl_dpchInfoCommon_dl_DPCH_InfoCommon))
    {
      if(tdsrrcllcpcie_get_dl_dpch_common_info_r6(ie_ptr->dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
                                 &dpch->dpch_common
                                ) == FAILURE
        )
      {
        return FAILURE;
      }
    }
    else /* rrc_dl_FDPCH_InfoCommon_chosen */
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: rrc_dl_FDPCH_InfoCommon_chosen not yet supported");
      return(FAILURE);
    }
  } /* end if dl dpch common info present */

  /* Get modeSpecificInfo(10.3.6.24) */
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_CommonInformation_r6_modeSpecificInfo_tdd))
  {
    /* Get tstd indicator */
    if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->tddOption,
          T_tdsrrc_DL_CommonInformation_r6_tddOption_tdd128))
    {
      dpch->dpch_common.tstd_indicator =
        ie_ptr->modeSpecificInfo.u.tdd->tddOption.u.tdd128->tstd_Indicator;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd->tddOption is not tdd128");
      return FAILURE;
    }

    /* Get the default DPCH offset if present */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
                    defaultDPCH_OffsetValuePresent))
    {
      /* DefaultDPCH-OffsetValueTDD ::= INTEGER (0..7) */
      dpch->doff =
        (uint8) ie_ptr->modeSpecificInfo.u.tdd->defaultDPCH_OffsetValue;
  
      if(dpch->doff > TDSRRCLLC_ASN1_DOFF_MAX_VAL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Doff ASN1 value out of range");
        return(FAILURE);
      }
      dpch->opt_parm_presence_bitmask |=
        TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
    }

    else
    {
      rrc_state = tdsrrc_get_state();

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_INIT)
      {
        /*For HHO, assign DOFF = 0 to L1*/
        if((tdsordered_config_ptr->is_hho)&&(rrc_state == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Non-DCH ->DCH case. Pending clarification from NW. If yes, merge with above case*/
        else if(rrc_state != TDSRRC_STATE_CELL_DCH)
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Return failure since DOFF is missing, e.g. DCH->DCH reconfig case*/
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"We do fault-tolerance for DOFF is missing");
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }

      }

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN)
      {
        /*For Non-DCH -> DCH case, make DOFF = 0.*/
        if((rrc_state != TDSRRC_STATE_CELL_DCH)&&(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
          dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
      }
    }  /* Doff is absent */
  
  } /* end if common TDD */
  else
  {
    /* TDD not selected.  Error as FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No FDD info present in DL_INFO_COMMON_FOR_ALL_RL");
    return FAILURE;
  }

  /* Indicate that Downlink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
 /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  return SUCCESS;
} /* tdsrrcllcpcie_get_dl_info_common_for_all_rl_r6() */


/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_common_info_r6()

DESCRIPTION:
  This function updates IE "dl common info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_common_info_r6
(
  uint32 dl_common_info_present,
  tdsrrc_DL_CommonInformation_r6 *dl_common_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Get the DL information for all Radio Links */
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
  /* Initialize the L1 bitmask for the parameters in the "Downlink
  * information for all radio links" IE.  Since the IE is optional,
  * we want to make sure we don't signal L1 erroneously that we have
  * received information when we haven't.
    */
    /* Init DL Common Info to Timing Maintained */
    tdsrrcllc_init_dl_common_info(tdsordered_config_ptr);
    
    if (dl_common_info_present)
    {
      if(tdsrrcllcpcie_get_dl_info_common_for_all_rl_r6(dl_common_info_ptr,
        &tdsordered_config_ptr->l1_dl_chan_parms.dpch
        )
        == FAILURE)
      {
        return FAILURE;
      }
    } /* end DL common RL info */
    
  } /* If next state is Cell_DCH */
  /* Calculate the DL TFCS */
  tdsrrcllc_update_rm_restriction_info(&tdsordered_config_ptr->l1_dl_cctrch_parms,
    &tdsordered_config_ptr->dl_ctfc,
    &tdsordered_config_ptr->l1_dl_ctfc_parms
    );
   
  return(SUCCESS);
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r6()

FILE NAME     This function gets the DL DPCH info for each RL

DESCRIPTION   This function translates the Downlink DPCH info for each
              RL IE to the RRC config database.  Note that it is assumed
              that a higher level routine is doing the translation from
              the linked list present in the ASN.1 decoder output to
              the array present in the RRC config database.  The radio
              link index is passed into this function and must be
              generated by that higher level routine. IEs refer to 10.3.6.21.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if everything is ok, FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all of the parameters
              that are present in the IE.  For those parameters not
              present the values currently existing are untouched.  For
              each optional parameter that is present the corresponding
              bit in the optional parameters presence bitmask is set.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r6
(
  /* pointer to the IE */
  tdsrrc_DL_DPCH_InfoPerRL_r6 *ie_ptr,

  /* pointer to the desired DL PhyChan parameter structure */
  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr, 
              rrc_DL_DPCH_InfoPerRL_r6_tdd))
  {
    /** TDD support only one CCTrCH. If OTA message send ul_CCTrCHListToRemove,
         RRC consider it invalid */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->u.tdd,
              dl_CCTrChListToRemovePresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDD only have one CCTrCH,ul_CCTrCHListToRemove is invalid");
      return FAILURE;
    }

    /* Get Downlink CCTrCH LIST from OTA message */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->u.tdd,
               dl_CCTrChListToEstablishPresent))
    {
      if (1 < ie_ptr->u.tdd->dl_CCTrChListToEstablish.n)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ul_CCTrCHList %d is invalid",(ie_ptr->u.tdd->dl_CCTrChListToEstablish.n));
        return FAILURE;
      }

      if (FAILURE == tdsrrcllcpcie_get_dl_cctrch_list_r4(&ie_ptr->u.tdd->dl_CCTrChListToEstablish.elem[0],
                                 &tdsordered_config_ptr->l1_dl_chan_parms.dpch))
      {
        return FAILURE;
      }
    }
    else
    {
      /*dl cctrch list is optional, and it must be present from non_DCH to DCH.
             So RRC add this check. DL_DPCH_InfoPerRL only be used in DCH.
             Then next state must be DCH. So here RRC only check current state. */
      if (rrc_state != TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"dl_CCTrCHList is not present from non-DCH to DCH");
        return FAILURE;
      }
    }
  }
  else
  {
    /* TDD choice not selected, FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD not supported");
    return FAILURE;
  }

  /* everything went ok */
  return SUCCESS;

} /* tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r6() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_for_each_rl_r6()

DESCRIPTION     This function translates the Downlink information for
                each radio link IE to the RRC config database

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
static tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllcpcie_get_dl_info_for_each_rl_r6
(
  tdsrrc_DL_InformationPerRL_r6 *ie_ptr,

  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan

)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Get Dl Info per RL");
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd))
  {
    /* First check if the DL DPCH Info per RL is included. If not,
    * this is NOT a valid Radio link - ignore it.
    */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
                dl_dpchInfoPresent))
    {
      /* save tstd info */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
        rrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption,
            rrc_PrimaryCCPCH_Info_r4_tddOption_tdd128))
        {
          dl_phychan->pccpch.tstd_indicator =
            ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption.u.tdd128->tstd_Indicator;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD128 info present in DL_INFO_FOR_EACH_RL");
          return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }
      /*If Cell_id is present for this RL, store it in tdscell_id_per_rl */
      if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr, cell_idPresent))
      {
        tdscell_id_per_rl[0] = (uint32) tdsrrc_translate_cell_id(&(ie_ptr->cell_id));
      }


      /* Get the DL DPCH info for this radio link */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get DL DPCH Info for RL ",1);

      if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->dl_dpchInfo, 
                    T_tdsrrc_DL_InformationPerRL_r6_dl_dpchInfo_dl_DPCH_InfoPerRL))
      {
        if (tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r6(ie_ptr->dl_dpchInfo.u.dl_DPCH_InfoPerRL,
             dl_phychan) == FAILURE)
        {
          return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDPCH Unsupported");
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }

      /* if we made it here we must be ok */
      /* Indicate that Downlink L1 needs to be configured */
      tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
      * still needs to be reconfigured so as to get the new CCTrCH
      * ID whenever Downlink L1 is reconfigured. This is due to the
      * interface requirements.
      */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
      return TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS;
    }  /* DL DPCH Info is present */
    else
    {
      /* DL DPCH Info per RL is NOT present - ignore this IE */
      return TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
    }
  }
  else
  {
    /* No TDD info.  We don't support FDD so return indicating an error */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
    return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
  }
} /* tdsrrcllc_get_dl_info_for_each_rl_r5() */

/*===========================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_per_rl_r6()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_info_per_rl_r6
(
  tdsrrc_DL_InformationPerRL_List_r6 *ie_ptr,
  tdsrrc_RRC_StateIndicator next_state

)
{
  /* is_inter_freq flag indicates if it is an Inter Freq HHO */
  /*boolean is_inter_freq = FALSE;*/

  tdsrrcllcoc_dl_info_per_rl_status_e_type status =
    TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();



  tdsordered_config_ptr->cpid_present = FALSE;
  /* servingHSDSCH_RL_indicator is FDD only, delete */
  if (ie_ptr->n > 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"more than one RL not supported");
    return FAILURE;
  }
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[0].modeSpecificInfo, 
                  T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL FDD not supported");
    return FAILURE;
  }
  if (T_tdsrrc_PrimaryCCPCH_Info_r4_fdd == ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL PrimaryCCPCH FDD not supported");
    return FAILURE;
  }

 /* We can say with certainty that if frequency info is included, and
  * it is different than the current frequency, then we are doing a
  * hard handover.  If this is not the case, we revisit this flag
  * later.
  */
 /*  is_inter_freq = (tdsordered_config_ptr->p_freq_present &&
     tdsordered_config_ptr->p_freq != rrc_freq); */


  if(next_state == tdsrrc_RRC_StateIndicator_cell_DCH)
  {
    /* If DPCH Info per RL is not present within DL Info per RL, then ignore it and
     * do not change the RL Info list in OC when going from DCH->DCH.
     * When going from non-DCH->DCH, the above condition should result in a Failure.
     */
    if(!((TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[0], dl_dpchInfoPresent)) &&
            ((TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[0].dl_dpchInfo,
                  T_tdsrrc_DL_InformationPerRL_r6_dl_dpchInfo_dl_DPCH_InfoPerRL))
        )))
    {
      if(rrc_state == TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring DL Info per RL Info absent");
        if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          /* when state change from CELL_DCH to CELL_DCH, save new cellParametersID if present. Else maintain */
          tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          tdsordered_config_ptr->cpid_present = TRUE;
        }
        return SUCCESS;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failing,DPCH Info per RL not present");
        return FAILURE;
      }
    }

    /* state to CELL_DCH, dl_dpchInfo Present, save new cellParametersID if present. Else maintain */
    if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      tdsordered_config_ptr->cpid_present = TRUE;
    }

    if(rrc_state != TDSRRC_STATE_CELL_DCH)
    {
      /* OTA msg have no cell_param_id */
      if (FALSE == tdsordered_config_ptr->cpid_present)
      {
        /* set current cell_param_id to OC structrue */
        tdsordered_config_ptr->cell_param_id = (uint8)tdsrrcccm_get_curr_camped_cpid();
      }
      /* Going from non-DCH state to CELL DCH */
      /* Always set new_rl_seen when coming to DCH from a non-DCH state.
      * That ensures the RL Info is populated after initializing.
      */
      /* have not HHO */
      if ((TRUE == tdsordered_config_ptr->p_freq_present)&&
          (tdsordered_config_ptr->p_freq == rrc_freq))
      {
        /* check if the current camped on ParaID is msg's RL.
         * If so, force Frequency_info to FALSE.
         */
        if ((ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)&&
           (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID != tdsrrcccm_get_curr_camped_cpid()))
        {
          /* Intra freq HHO */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"intra-f HHO FACH->DCH");
        }
        else
        {
          tdsordered_config_ptr->p_freq_present = FALSE;
        }
      }
    }
    else       /* Going from DCH->DCH */
    {
      /* have HHO */
      if(tdsordered_config_ptr->is_hho)
      {
        /* This radio link is not be setup */
        tdshsdpa_msg_params.msg_params_present = TRUE;
      }
    }

    /* Process the IE and place the info into our database. */
    status = tdsrrcllcpcie_get_dl_info_for_each_rl_r6(&ie_ptr->elem[0],
        &tdsordered_config_ptr->l1_dl_chan_parms
        );

    if(status == TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE)
    {
      return FAILURE;
    }
    else if(status == TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED)
    {
      //continue;
    }
    else if(status == TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS)
    {
      /* either inter HHO or intra HHO */
      if(tdsordered_config_ptr->is_hho)
      {
        /* If we succeeded in processing the IE, and we are doing a
         * hard handover, we must be sure that L1 is informed by
         * sending frequency info, even if we didn't get frequency
         * info in the reconfiguration message.
         */
        if(!tdsordered_config_ptr->p_freq_present)
        {
          tdsordered_config_ptr->p_freq_present = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO triggered");
        }
      }
    }
    
#ifdef FEATURE_TDSCDMA_HSUPA
    /* Call below function to wipe out any stored L1 information
         * on EUL
         */
    if (tdsordered_config_ptr->is_hho) 
    {
      tdsrrcllcpcie_clean_l1_e_info_on_non_r7_hho();
    }
#endif /* FEATURE_TDSCDMA_HSUPA */
  }
  else /* All states other than Cell_DCH */
  {
    if(ie_ptr->n > 0)
    {
      if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
      {
        tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        tdsordered_config_ptr->cpid_present = TRUE;
      }
    }
  }

  return SUCCESS;

} /* end tdsrrcllcpcie_get_dl_info_per_rl_r6() */
/*============================================================================
FUNCTION: rrcllcpcie_process_dl_hspdsch_info_and_update_hsdpa_msg_params()

DESCRIPTION:
  This function updates IE "dl hspdsch info " in ordered config and update
  hsdpa_msg_param variable
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_hspdsch_info_and_upd_hsdpa_msg_params
(
  uint32 dl_hspdsch_info_present,
  tdsrrc_DL_HSPDSCH_Information_r6 *dl_hspdsch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    if (dl_hspdsch_info_present)
    {
#ifdef FEATURE_TDSCDMA_64QAM
      if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE)
      {
        tdshsdpa_msg_params.hsdsch_info_present = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set HS-DSCh bitmask as 64 QAM is stopped");
      }
#endif

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"HSPDSCH Info present");
      tdshsdpa_msg_params.msg_params_present = TRUE;
      tdshsdpa_msg_params.hspdsch_info_present = TRUE;
      
    }
    /* Determine if IFHHO is involved */
    if (tdsordered_config_ptr->is_hho)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO is happening thru OTA.");
      tdshsdpa_msg_params.msg_params_present = TRUE;
    }
    
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if(tdsrrcllc_set_macehs_or_machs_and_check_if_coexist()== TRUE)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHSPA+: MAC EHS %d and MAC HS %d cant co-exist ",tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue,tdsordered_config_ptr->mac_dl_parms.num_dflow);
      return FAILURE;
    }
  
    tdsrrcllc_set_machs_ehs_transition();
#endif
    
    /* Set HSDPA Action & Physical Channel Parameters associated with it */
    if (tdsrrcllc_determine_hsdpa_action_r6(dl_hspdsch_info_ptr) == SUCCESS)
    {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_set_hsdpa_action_in_machs_macehs();

      if(((tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_HS_TO_EHS )
        ||(tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_EHS_TO_HS ))
        && (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"MAC-HS <->  MAC-EHS transition %d, and Mac Reset Ind not set %d,reject config",tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type,tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
        return FAILURE;
      }
     tdsrrcllcpcie_update_hs_tfri_table_type_non_r7_ota();
#else
      tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = tdsrrc_get_hsdpa_action();
#endif
#ifdef FEATURE_TDSCDMA_64QAM
      if((tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE)
        &&((tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_START) ||
               (tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_RECONFIG) ||
               ((tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_NOOP) &&
                (tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE))))
      {
        if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS )
        {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"64 QAm cannot be started on MAC-HS");
            return FAILURE;
        }
      }
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:HSDPA Action %d set in MAC",tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Error during HSDPA action processing");  
      return FAILURE;
    }
  }

  return(SUCCESS);
}


/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_6()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r6
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r6 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  boolean is_hho = FALSE;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_DL_InformationPerRL_List_r6 *tmp_ie_ptr = dl_info_per_rl_list_ptr;
  tdsrrc_state_e_type curr_rrc_state = tdsrrc_get_state();

  uint8               cpid = TDSRRC_INVALID_CPID;

  /* If state from CELL_DCH/CELL_FACH/CONNECTING to CELL_DCH */
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"curr_rrc_state = %d",curr_rrc_state);
    cpid = (uint8)tdsrrcccm_get_curr_camped_cpid();
    /* If two FreqInfo are included in the OTA */
    if ((tdsordered_config_ptr->p_freq_present)&&(tdsordered_config_ptr->s_freq_present))
    {
      /*  If the first frequency got changed. */
      if (rrc_freq != tdsordered_config_ptr->p_freq)
      {
        /* It is a inter handover */
        is_hho = TRUE;
        tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different freq %d",tdsordered_config_ptr->p_freq);
      }
      if (FALSE == is_hho)
      {
        if (dl_info_per_rl_list_present)
        {
          if (tmp_ie_ptr == NULL)
          {
            ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
          }
          /*  If CPID got changed. */
          if ((T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd ==
                 tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
                 (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                     tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                     (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
          {
            if(cpid != tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
            {
              /* It is a intra handover */
              is_hho = TRUE;
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
            }
          }
        }
      }
    }
    /* If only one FreqInfo is included in the OTA */
    else if ((tdsordered_config_ptr->p_freq_present)||(tdsordered_config_ptr->s_freq_present))
    {
      if (dl_info_per_rl_list_present)
      {
        if (tmp_ie_ptr == NULL)
        {
          ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
        }
        /*  If CPID got changed. */
        if ((T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd ==
               tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
               (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                   tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                   (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          if(tdsrrcccm_get_curr_camped_cpid() != 
                tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
          {
            /* It is a handover */
            is_hho = TRUE;
            if (tdsordered_config_ptr->p_freq != rrc_freq)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a inter handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a intra handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
            }
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
          }
        }
      }
    }
#ifdef FEATURE_TDSCDMA_SINGLE_FREQ_NO_CPID_HO_SUPPORT

      if (tdsordered_config_ptr->p_freq_present)
      {
        /*  If the first frequency got changed. */
        if (rrc_freq != tdsordered_config_ptr->p_freq)
        {
          /* It is a inter handover */
          is_hho = TRUE;
          tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Support none cpid and one freq info HO. p_freq %d different with current freq %d",tdsordered_config_ptr->p_freq,rrc_freq);
        }
      }

#endif
  }

  if (TRUE == is_hho)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO detected different freq different cell para id ");
  }
  tdsordered_config_ptr->is_hho = is_hho;

  return SUCCESS;
}


/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_info_per_rl_list_r6()

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_info_per_rl_list_r6
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r6 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator

)
{
  if (dl_info_per_rl_list_present)
  {
    if(FAILURE == tdsrrcllcpcie_get_dl_info_per_rl_r6(dl_info_per_rl_list_ptr, rcvd_state_indicator

        ))
    {
      return (FAILURE);
    }
  }

#ifdef FEATURE_TDSCDMA_HSUPA
  /* process EUL related information */
  if (FAILURE == tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r6(
      dl_info_per_rl_list_present, 
      dl_info_per_rl_list_ptr,
      rcvd_state_indicator))
  {
    return(FAILURE);
  }
#endif /* FEATURE_TDSCDMA_HSUPA */

  return(SUCCESS);
}


/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_for_each_rl()

DESCRIPTION     This function translates the Downlink information for
                each radio link IE to the RRC config database

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllc_get_dl_info_for_each_rl
(
  tdsrrc_DL_InformationPerRL *ie_ptr,

  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan,

  uint8 rl_idx,

  boolean is_new_rl
)
{
/* r3 del begin */

  /*tdsl1dl_info_per_rl_struct_type *rl_info;*/

  if (rl_idx >= TDSUE_MAX_RL)
  {
    /* radio link index too large, bail */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many Radio Links %d",rl_idx);
    return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
  }

  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                  T_tdsrrc_DL_InformationPerRL_modeSpecificInfo_fdd))
  {
    /* First check if the DL DPCH Info per RL is included. If not,
     * this is NOT a valid Radio link - ignore it.
     */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
                dl_DPCH_InfoPerRLPresent))
    {
      /* snap a local pointer to the proper radio link element in the
         RRC config page */
      /*rl_info = &dl_phychan->dl_per_rl[rl_idx];*/

      /* Get the DL DPCH info for this radio link */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get DL DPCH Info for RL %d",(rl_idx + 1));

      if (tdsrrcllcpcie_get_dl_dpch_info_for_each_rl(&ie_ptr->dl_DPCH_InfoPerRL,
                                        dl_phychan, rl_idx,
                                        is_new_rl) == FAILURE)
      {
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }
      /* if we made it here we must be ok */
      /* Indicate that Downlink L1 needs to be configured */
      tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
       * still needs to be reconfigured so as to get the new CCTrCH
       * ID whenever Downlink L1 is reconfigured. This is due to the
       * interface requirements.
       */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
      return TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS;
    }  /* DL DPCH Info is present */
    else
    {
      /* DL DPCH Info per RL is NOT present - ignore this IE */
      return TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
    }
  }
  else
  {
    /* No FDD info.  We don't support TDD so return indicating an error */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No FDD info present in DL_INFO_FOR_EACH_RL");
    return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
  }
  /* r3 del end */
} /* tdsrrcllc_get_dl_info_for_each_rl() */

/*===========================================================================

FUNCTION        IS_FREQ_IN_OVERLAPPING_850_AND_800_BANDS

DESCRIPTION     This function determines if the UARFCN passed to it is in 
                the overlapping region of 850 and 800 bands or not.


RETURN VALUE    TRUE:  UARFCN is in the overlapping region
                FALSE: Otherwise

SIDE EFFECTS    

===========================================================================*/
boolean tdsrrc_is_freq_in_overlapping_850_and_800_bands(uint32 freq)
{
  if(   (freq == tdsrrc_dl_band_d_addl_channels[0])
     || (freq == tdsrrc_dl_band_d_addl_channels[1])
     )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
} /* tdsrrc_is_freq_in_overlapping_850_and_800_bands */


/*===========================================================================

FUNCTION        RRC_GET_FREQUENCY_BAND

DESCRIPTION     This function looks at the passed in frequency and
                returns the band the frequency belongs to. 

RETURN VALUE    Returns TDSRRC_CSP_BAND_MAX if UARFCN is invalid.

SIDE EFFECTS    None

===========================================================================*/
tdsrrc_csp_band_class_type tdsrrc_get_frequency_band
(
  tdsrrc_freq_type freq
)
{

  /* Band a */
  if ((freq >=  TDSL1_BAND_A_CHAN_MIN)&&
           (freq <= TDSL1_BAND_A_CHAN_MAX))
  {
    return RRC_CSP_TDS_A_BAND;
  }

  /* Band e */
  if ((freq >= TDSL1_BAND_E_CHAN_MIN)&&
           (freq <= TDSL1_BAND_E_CHAN_MAX))
  {
    return RRC_CSP_TDS_E_BAND;
  }

  /* Lower band f */
  if ((freq >= TDSL1_BAND_F_CHAN_MIN)&&
           (freq <= TDSL1_BAND_F_CHAN_FS_LOWER_MAX))
  {
    return RRC_CSP_TDS_F_LOWER_BAND;
  }

  /* Upper band f */
  if ((freq >= TDSL1_BAND_F_CHAN_FS_UPPER_MIN)&&
           (freq <= TDSL1_BAND_F_CHAN_MAX))
  {
    return RRC_CSP_TDS_F_UPPER_BAND;
  }

  TDSRRC_MSG1(MSG_LEGACY_ERROR,"No valid band for freq %d,set to NONE",freq);
  return TDSRRC_CSP_BAND_MAX;

} /* tdsrrc_get_frequency_band */


/*===========================================================================

FUNCTION        tdsrrc_validate_dl_arfcn()

DESCRIPTION     This function validates the DL UARFCN values.

RETURN VALUE    FAILURE if UARFCN is invalid,
                SUCCESS otherwise

SIDE EFFECTS    None

===========================================================================*/
uecomdef_status_e_type tdsrrc_validate_dl_arfcn
(
  tdsrrc_freq_type uarfcn_dl
)
{
  uecomdef_status_e_type rc = FAILURE;


  if(tdsrrccsp_is_supported_band(RRC_CSP_TDS_A_BAND))
  {
    rc = SUCCESS;

    /* Band a */
    if ((uarfcn_dl >= TDSL1_BAND_A_CHAN_MIN)&&
             (uarfcn_dl <= TDSL1_BAND_A_CHAN_MAX))
    {
    }
    else
    {
      rc = FAILURE;
    }
  }
  /* If we've already found that both are valid, go ahead and return here.
   * Otherwise, go on and check band b.
   */

  if(rc == SUCCESS)
  {
    return rc;
  }

  if(tdsrrccsp_is_supported_band(RRC_CSP_TDS_E_BAND))
  {
    rc = SUCCESS;

    /* Band e */
    if(uarfcn_dl < TDSL1_BAND_E_CHAN_MIN ||
         uarfcn_dl > TDSL1_BAND_E_CHAN_MAX)
    {
      rc = FAILURE;
    }
  }

  if(rc == SUCCESS)
  {
    return rc;
  }

  if(tdsrrccsp_is_supported_band(RRC_CSP_TDS_F_BAND))
  {
    rc = SUCCESS;

    /* Band f */
    if(uarfcn_dl <  TDSL1_BAND_F_CHAN_MIN ||
       uarfcn_dl > TDSL1_BAND_F_CHAN_MAX)
    {
      rc = FAILURE;
    }
  }

  if(rc == SUCCESS)
  {
    return rc;
  }

  TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received invalid DL UARFCN %d",uarfcn_dl);

  return rc;

} /* tdsrrc_validate_dl_arfcn */


/*===========================================================================

FUNCTION        tdsrrc_validate_tds_frequency()

DESCRIPTION     This function validates the UARFCN in the provided
                rrc_FrequencyFDD IE.
                               
RETURN VALUE    Returns FAILURE if UARFCN is invalid.

SIDE EFFECTS    None

===========================================================================*/
uecomdef_status_e_type tdsrrc_validate_tds_frequency
(
  tdsrrc_UARFCN             uarfcn_Nt
)
{
  uecomdef_status_e_type rc = FAILURE;


  /* The valid set of UARFCN values differs for different frequency bands.
   * If the frequency band is not supported by the UE, return failure
   */

  if(tdsrrccsp_is_preference_band(RRC_CSP_TDS_A_BAND))
  {
    rc = SUCCESS;
    /* Band a */
    if ((uarfcn_Nt >= TDSL1_BAND_A_CHAN_MIN)&&
             (uarfcn_Nt <= TDSL1_BAND_A_CHAN_MAX))
    {
    }
    else
    {
      rc = FAILURE;
    }
  }

  /* If we've already found that both are valid, go ahead and return here.
   * Otherwise, go on and check band b.
   */

  if(rc == SUCCESS)
  {
    return rc;
  }

  if(tdsrrccsp_is_preference_band(RRC_CSP_TDS_E_BAND))
  {
    /* Reinitialize the return value to its optimistic state. */
    rc = SUCCESS;

    /* Band e */
    if(uarfcn_Nt < TDSL1_BAND_E_CHAN_MIN ||
         uarfcn_Nt > TDSL1_BAND_E_CHAN_MAX)
    {
      rc = FAILURE;
    }
  }

  if(rc == SUCCESS)
  {
    return rc;
  }

  if(tdsrrccsp_is_preference_band(RRC_CSP_TDS_F_BAND))
  {
    rc = SUCCESS;
    /* Band f */
    if(uarfcn_Nt < TDSL1_BAND_F_CHAN_MIN ||
       uarfcn_Nt > TDSL1_BAND_F_CHAN_MAX)
    {
      rc = FAILURE;
    }
  }

  if(rc == SUCCESS)
  {
    return rc;
  }

  TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received invalid freq info,UARFCN DL %d",uarfcn_Nt);

  return rc;

} /* tdsrrc_validate_tds_frequency */

/*===========================================================================

FUNCTION        tdsrrc_validate_frequency_info()

DESCRIPTION     This function validates the UARFCN values in the provided
                Frequency Info IE according to table 5.2 in 25.101.

RETURN VALUE    Returns FAILURE if either UARFCN is invalid.

SIDE EFFECTS    None

===========================================================================*/
uecomdef_status_e_type tdsrrc_validate_frequency_info
(
  tdsrrc_FrequencyInfo *ie_ptr
)
{
  /* We only support TDD. */
  if(!(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo, 
              T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Non-FDD Frequency Info not supported");
    return FAILURE;
  }
  else
  {
    return (tdsrrc_validate_tds_frequency(ie_ptr->modeSpecificInfo.u.tdd->uarfcn_Nt));
  }
} /* tdsrrc_validate_frequency_info */

/*===========================================================================
FUNCTION        tdsrrcllc_get_dl_info_per_rl_r5()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_per_rl_r5
(
  tdsrrc_DL_InformationPerRL_List_r5 *ie_ptr,
  tdsrrc_RRC_StateIndicator next_state
)
{
  /* is_inter_freq flag indicates if it is an Inter Freq HHO */
/*  boolean is_inter_freq = FALSE; */
  tdsrrcllcoc_dl_info_per_rl_status_e_type status =
    TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  tdsordered_config_ptr->cpid_present = FALSE;

  /* servingHSDSCH_RL_indicator is FDD only, delete */
  if (ie_ptr->n > 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"more than one RL not supported");
    return FAILURE;
  }
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[0].modeSpecificInfo, 
                  T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL FDD not supported");
    return FAILURE;
  }
  if (T_tdsrrc_PrimaryCCPCH_Info_r4_fdd == ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL PrimaryCCPCH FDD not supported");
    return FAILURE;
  }

  /* We can say with certainty that if frequency info is included, and
   * it is different than the current frequency, then we are doing a
   * hard handover.  If this is not the case, we revisit this flag
   * later.
   */
  /*is_inter_freq = (tdsordered_config_ptr->p_freq_present &&
    tdsordered_config_ptr->p_freq != rrc_freq);*/
  

  if (next_state == tdsrrc_RRC_StateIndicator_cell_DCH)
  {
    /* If DPCH Info per RL is not present within DL Info per RL, then ignore it and
     * do not change the RL Info list in OC when going from DCH->DCH.
     * When going from non-DCH->DCH, the above condition should result in a Failure.
     */
    if ((TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[0], dl_DPCH_InfoPerRLPresent)) == 0)
    {
      if (TDSRRC_STATE_CELL_DCH == rrc_state)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring DL Info per RL Info absent");
        if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          /* when state change from CELL_DCH to CELL_DCH, save new cellParametersID if present. Else maintain */
          tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          tdsordered_config_ptr->cpid_present = TRUE;
        }
        return SUCCESS;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failing,DPCH Info per RL not present");
        return FAILURE;
      }
    }

    /* state to CELL_DCH, dl_dpchInfo Present, save new cellParametersID if present. Else maintain */
    if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      tdsordered_config_ptr->cpid_present = TRUE;
    }
    if (TDSRRC_STATE_CELL_DCH != rrc_state)
    {
      /* OTA msg have no cell_param_id */
      if (FALSE == tdsordered_config_ptr->cpid_present)
      {
        /* set current cell_param_id to OC structrue */
        tdsordered_config_ptr->cell_param_id = (uint8)tdsrrcccm_get_curr_camped_cpid();
      }
      /* Non CELL_DCH enter CELL_DCH state */
      if ((TRUE == tdsordered_config_ptr->p_freq_present)&&
            (tdsordered_config_ptr->p_freq == rrc_freq))
      {
        /* check if the current camped on ParaID is in RL Info.
         * If so, force Frequency_info to FALSE.
         */
        if ((ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)&&
           (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID != tdsrrcccm_get_curr_camped_cpid()))
        {
          /* Intra freq HHO */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"intra-f HHO FACH->DCH");
        }
        else
        {
          tdsordered_config_ptr->p_freq_present = FALSE;
        }
      }
    }
    else
    {
      /* DCH -> DCH AND HHO */
      if(tdsordered_config_ptr->is_hho)
      {
        tdshsdpa_msg_params.msg_params_present = TRUE;
      }
    }

    /* Process the IE and place the info into our database. */
    status = tdsrrcllc_get_dl_info_for_each_rl_r5(&ie_ptr->elem[0],
        &tdsordered_config_ptr->l1_dl_chan_parms);

    if(status == TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE)
    {
      return FAILURE;
    }
    else if(status == TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS)
    {
      /* either inter HHO or intra HHO */
      if(tdsordered_config_ptr->is_hho)
      {
        /* If we succeeded in processing the IE, and we are doing a
         * hard handover, we must be sure that L1 is informed by
         * sending frequency info, even if we didn't get frequency
         * info in the reconfiguration message.
         */
        /* tdsordered_config_ptr->is_hho maybe because cell parameter id difference,
         * OTA msg not include freq 
         */
        if(!tdsordered_config_ptr->p_freq_present)
        {
          tdsordered_config_ptr->p_freq_present = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO triggered");
        }
      }
    }
#ifdef FEATURE_TDSCDMA_HSUPA
    /* Call below function to wipe out any stored L1 information
     * on EUL
     */
    /* either inter HHO or intra HHO */
    if (tdsordered_config_ptr->is_hho) 
    {
      tdsrrcllcpcie_clean_l1_e_info_on_non_r7_hho();
    }
#endif /* FEATURE_TDSCDMA_HSUPA */

  }
  else /* All states other than Cell_DCH */
  {
    if(ie_ptr->n > 0)
    {
      if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
      {
        tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        tdsordered_config_ptr->cpid_present = TRUE;
      }
    }
  }

  return SUCCESS;

} /* end tdsrrcllc_get_dl_info_per_rl_r5() */

/*====================================================================
FUNCTION: tdsrrcllc_update_rm_restriction_info()

DESCRIPTION:
  This function do multiple jobs to update the TFCS in to RRC-L1
  interface structures.
  1. Calculate the TFI from the CTFC values using corresponding
     CCTrCh info.
  2. Update the TDSALLOWED TFC in the RRC-L1 TFCS/CTFC info interface
     structures.
  3. Set the TDSINVALID (unused/removed) or TDSRESTRICTED TFCI to invalid
     in the RRC-L1 interface TFCS/CTFC info structures.

  The RM restriction info should be a valid structure pointer. Though
  it may optional in Downlink DPCH info. In this case the structure
  member restriction_info_exist must be set to FALSE by the caller
  before calling this function.

  This function compares the TrCh ids in the RM restriction information
  and CCTrCh information. The TrCh Id should be based on the same
  information base. If TrCh Ids are translated to some other scheme
  after getting from IEs that should be applied to both.

  The RM restriction can be applied to the CCTrCh that doesn't have
  the CTFC/TFCS existence. In this case the follwoing function
  parameter can be passed as NULL.
    DL CTFC info structure pointer
    tdsrrcllc_dl_ctfc_info_struct_type *dl_tfcs_info,
  However the parameter
    DL TFCS info for TDSL1_CPHY_SETUP_REQ
    tdsl1_dl_cctrchcfg_ctfc_info_struct_type *tfcs_info_rrc_l1_inf
  SHOULD NOT be NULL. This member of this structure are filled to indicate
  no TFCI existence.


  References: 25.331 (2000-12) 10.3.6.18, 10.3.6.31

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_rm_restriction_info
(
  /* CCTrCh setup information. */
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type       *cctrch_info,
  /* DL CTFC info structure pointer */
  tdsrrcllc_dl_ctfc_info_struct_type     *dl_tfcs_info,
  /* RM restriction info structure pointer */
  /* DL TFCS info for TDSL1_CPHY_SETUP_REQ */
  tdsl1_dl_cctrchcfg_ctfc_info_struct_type         *tfcs_info_rrc_l1_inf
)
{
  /* function local variables */
  /* ------------------------ */

  /* for loop index for iterating over all transport channels, CTFCs */
  uint16 index_ctfc;
  int16  index_trch;
  /* pointer to the TF table for previous TrCh with reference to the TrCh
     for which the P value is being calculated */
  tdsl1_dl_cctrchcfg_trch_info_struct_type* prev_trch_table;
  /* This array stores the P value for all TrChs in the given CCTrCh. This
     is used for calculating the TFI from CTFC */
  uint32 p_value[TDSUE_MAX_TRCH];
  /* temporary value (m) of CTFC value in current CTFC entry to calculate
     the TFIs */
  uint32 temp_ctfc_val;
  /* the P value of the TrCh for which TFI are currently being calculated */
  uint32 this_trch_p_value;

  /* TrCh Id if the TrCh current at the current index in loop */
  /*uint8 this_trch_id;*/
  /* TFI in a TFCS */
  uint8 this_tfi;
  /* pointer to the CTFC entry of the CTFC for which the TFI of all TrCh are
     currently being calculated in the loop */
  tdsrrcllc_dl_ctfc_entry_info_struct_type *this_ctfc_entry;

  /* last valid CTFC entry */
  int16 last_valid_entry;
  boolean ctfc_invalid_flag  = FALSE;

  /*** function code starts here ***/

  /* Initialize local arrays */
  memset(p_value, 0, sizeof(p_value));

  /* Check if the TFCS information exists then only calculate
    the P value for decoding TFI iondices from CTFC values */
  if (dl_tfcs_info)
  {
    /* ------------------------------------------------ */
    /* Calculation of TFI for each TFC from CTFC values */
    /* ------------------------------------------------ */

    /* initialization */
    /* -------------- */

    /* set the P value of the first transport channel to 0 */
    p_value[0] = 1;

    /* calculation of the P value */
    /* -------------------------- */

    /* loop over all the transport channel to multiply the number of  TF
        in all TrChs prior to current TrCh.
       Note: The first TrCh has the P value = 1.
       Calculation method:
         Set L0 = 1
         TrCh#1 Number of TF = L1 Pvalue = L0 = 1
         TrCh#2 Number of TF = L2 Pvalue = L0 x L1 = PvalueTrCh#1 X L1
         TrCh#3 Number of TF = L3 Pvalue = L0 x L1 X L2= PvalueTrCh#2 X L2

         For TrCh#n Pvalue = L0 x L1 x L2 x ..... L(n-1)
                           = TrCh#(n-1) Pvalue * L(n-1)
    */

    for (index_trch = 1; index_trch < cctrch_info->num_trch; index_trch++)
    {
      /* get the pointer to the TrCh info of the previous channel with
         reference to the current TrCh for which the P value is to be
         calculated */
      prev_trch_table = cctrch_info->trch_info_array[index_trch - 1];

      /* calculate the p value for this TrCh */
      p_value[index_trch] =
        p_value[index_trch - 1] * prev_trch_table->num_tf;
    }
  } /* end if, DL TFCS info exist, then calc the P value */

  /* Get the RM restriction transport channel information aligned in
     array as per the TrCh Ids in CcTrCh info */
  /* --------------------------------------------------------------- */

  /* Get the RM restriction info for all TrCh */
  /*
  for (index_trch = 0; index_trch < cctrch_info->num_trch; index_trch++)
  {*/
    /* Get the TrCh id for this TrCh and initialize the
      restriction info pointer to NULL */
   /* this_trch_id = cctrch_info->trch_info_array[index_trch]->trch_id;*/

 /* }*/ /* End for, get the RM restriction info for all TrCh in this CCTrCh */

  /* calculation of the TFI from CTFC and P value calculated above */
  /* ------------------------------------------------------------- */

  /* initialize the last valid CTFC entry array index to -1 */
  last_valid_entry = -1;

  /* Check if the TFCS information exists then only process it */
  if (dl_tfcs_info)
  {
    /* loop to calculate the TFI's  for all TrCh in each CTFC. */
    for (index_ctfc = 0;
        index_ctfc < dl_tfcs_info->num_ctfc_entry;
        index_ctfc++)
    {
      ctfc_invalid_flag = FALSE;
      this_ctfc_entry = &dl_tfcs_info->ctfc_entry_data[index_ctfc];

      /* if the CTFC is not valid then skip the calculation */
      if (this_ctfc_entry->state == TDSINVALID)
        continue;

      /* initialize the temporary value of this CTFC (m in the
         documentation) */
      temp_ctfc_val = this_ctfc_entry->ctfc_value;

      /* loop to calculate the TFIs for all TrCh for this CTFC */
      for (index_trch = cctrch_info->num_trch - 1;
          index_trch >= 0;
          index_trch--)
      {
        this_trch_p_value = p_value[index_trch];
        if (!this_trch_p_value)
        {
          ERR_FATAL("Trying to divide by zero",0,0,0);
        }
        this_tfi = (uint8) (temp_ctfc_val / this_trch_p_value);
        if (this_tfi >= cctrch_info->trch_info_array[index_trch]->num_tf)
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid CTFC value of TFCH ID %d , TF NUM %d and TRCH Index %d", cctrch_info->trch_info_array[index_trch]->trch_id,cctrch_info->trch_info_array[index_trch]->num_tf, index_trch);
          ctfc_invalid_flag = TRUE;
          break;
        }
        this_ctfc_entry->tfi_table[index_trch] = this_tfi;

        temp_ctfc_val %= this_trch_p_value;
      } /* end for loop, calculate the TFIs for this CTFC */

      if (this_ctfc_entry->state == TDSALLOWED)
      {
        if (FALSE == ctfc_invalid_flag)
        {
        tfcs_info_rrc_l1_inf->ctfc_entry_array[index_ctfc]->valid_ctfc = TRUE;
        tfcs_info_rrc_l1_inf->ctfc_entry_array[index_ctfc]->tfi_table =
          this_ctfc_entry->tfi_table;
        last_valid_entry = (int16)index_ctfc;
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid CTFC value %d received,index is %d",this_ctfc_entry->ctfc_value,index_ctfc);
          tfcs_info_rrc_l1_inf->ctfc_entry_array[index_ctfc]->valid_ctfc = FALSE;
        }
      }
      else
        tfcs_info_rrc_l1_inf->ctfc_entry_array[index_ctfc]->valid_ctfc = FALSE;
    } /* end for loop, calculate the TFI for all valid CTFCs */
  }

  /* Update the TFCS information for RRC L1 TFCS information */
  /* ------------------------------------------------------- */

  if(last_valid_entry >= 0)
  {
    tfcs_info_rrc_l1_inf->num_ctfc_entry = last_valid_entry + 1;
  }

} /* end tdsrrcllc_update_rm_restriction_info() */

/*============================================================================
FUNCTION: rrcllcpcie_process_dl_hspdsch_info_and_update_hsdpa_msg_params_r7()

DESCRIPTION:
  This function updates IE "dl hspdsch info " in ordered config and update
  hsdpa_msg_param variable
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_hspdsch_info_and_upd_hsdpa_msg_params_r7
(
  uint32 dl_hspdsch_info_present,
  tdsrrc_DL_HSPDSCH_Information_r7 *dl_hspdsch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    if (dl_hspdsch_info_present)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"HSPDSCH Info present");
      tdshsdpa_msg_params.msg_params_present = TRUE;
      tdshsdpa_msg_params.hspdsch_info_present = TRUE;

#ifdef FEATURE_TDSCDMA_64QAM
      if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured != 
        tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured)
      {
        tdshsdpa_msg_params.hsdsch_info_present = TRUE;
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Set HS-DSCh bitmask as 64 QAM status is changed: Before %d,current %d (0:Not present/1:Present)",tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured ,tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured);
      }
#endif
    }
    /* Determine if IFHHO is involved */
    if (tdsordered_config_ptr->is_hho)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO is happening thru OTA.");
      tdshsdpa_msg_params.msg_params_present = TRUE;
    }
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if(tdsrrcllc_set_macehs_or_machs_and_check_if_coexist()== TRUE)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: MAC EHS %d and MAC HS %d cant co-exist ",tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue,tdsordered_config_ptr->mac_dl_parms.num_dflow);
      return FAILURE;
    }
  
    tdsrrcllc_set_machs_ehs_transition();
#endif
    
    /* Set HSDPA Action & Physical Channel Parameters associated with it */
    if (tdsrrcllc_determine_hsdpa_action_r7(dl_hspdsch_info_ptr) == SUCCESS)
    {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_set_hsdpa_action_in_machs_macehs();

      if(((tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_HS_TO_EHS )
        ||(tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_EHS_TO_HS ))
        && (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"MAC-HS <->  MAC-EHS transition %d, and Mac Reset Ind not set %d,reject config",tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type,tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
        return FAILURE;
      }
      
#else
      tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = tdsrrc_get_hsdpa_action();
#endif
#ifdef FEATURE_TDSCDMA_64QAM
      if((tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE)
        &&((tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_START) ||
               (tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_RECONFIG) ||
               ((tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_NOOP) &&
                (tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE))))
      {
        if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS )
        {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"64 QAm cannot be started on MAC-HS");
            return FAILURE;
        }
      }
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:HSDPA Action %d set in MAC",tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Error during HSDPA action processing");  
      return FAILURE;
    }
  }

  return(SUCCESS);
}

#ifdef FEATURE_TDSCDMA_HSUPA
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_agch_info_r7()

DESCRIPTION:
  This function processes e-AGCH information and updates the information
  in a given e_rl_index
      
DEPENDENCIES:
  This function needs to be called only after calling function to process E-HICH info.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_process_e_agch_info_r7
(
  uint32 e_agch_info_present,
  tdsrrc_E_AGCH_Information_r7 *e_agch_info_ptr
)
{
  uint8 temp_i;
  tdsl1_e_dl_per_rl_info_struct_type *e_dl_info_ptr 
         = &tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info;

  if ((e_agch_info_present) && 
       (T_tdsrrc_E_AGCH_Information_r7_modeSpecific_fdd == e_agch_info_ptr->modeSpecific.t))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL E_AGCH FDD not supported");
    return(FAILURE);
  }
  if ((e_agch_info_present) && 
       (T_tdsrrc_E_AGCH_Information_r7_tdd_tdd128 != e_agch_info_ptr->modeSpecific.u.tdd->t))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL E_AGCH 3.84/7.68TDD not supported");
    return(FAILURE);
  }
  if (e_agch_info_present)
  {
    /* store AGCH info only if E-HICH info is already stored */
    if (tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich != 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Begin save E_AGCH");
      /* If true, RDI will present in E-AGCH payload.otherwise, no RDI present */
      e_dl_info_ptr->e_agch_info.rdi_pre 
           = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->rdi_Indicator;
      /* TPC step size for E-PUCH power control */
      e_dl_info_ptr->e_agch_info.tpc_step 
           = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->tpc_StepSize;
      /* E-AGCH Bler Target. -3.15 to 0 step 0.05. 0 mapped to bler target and -63 mapped to -3.15*/
      e_dl_info_ptr->e_agch_info.bler_target 
           = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_BLER_Target;
      /* Total number of EAGCH channels(not more than 4) */
      e_dl_info_ptr->e_agch_info.num_eagch 
           = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.n;

      for (temp_i = 0; temp_i < e_dl_info_ptr->e_agch_info.num_eagch; temp_i++)
      {
        /* Time slot number for E-AGCH According to message from system team,
         * E-AGCH won't be sent on TS0. */
        e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i].ts_num
          = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.elem[temp_i].timeslotNumber;
        /* Channelization code for E-AGCH1 */
        e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i].first_code
          = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.elem[temp_i].firstChannelisationCode;
        /* Channelization code for E-AGCH2 */
        e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i].second_code
          = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.elem[temp_i].secondChannelisationCode;

        /*Midamble allocation mode. (10.3.6.41)*/
        switch (e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.elem[temp_i].midambleAllocationMode.t)
        {
        case T_tdsrrc_E_AGCH_Individual_LCR_midambleAllocationMode_defaultMidamble:
          e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
          break;
        case T_tdsrrc_E_AGCH_Individual_LCR_midambleAllocationMode_commonMidamble:
          e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.allocation_mode = TDSL1_COMMON_MIDAMBLE;
          break;
        case T_tdsrrc_E_AGCH_Individual_LCR_midambleAllocationMode_ueSpecificMidamble:
          e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.allocation_mode = TDSL1_UE_SPECIFIC_MIDAMBLE;
          e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.shift = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->
             e_AGCH_Set_Config.elem[temp_i].midambleAllocationMode.u.ueSpecificMidamble;
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL E_AGCH Midamble allocation mode not supported");
          return FAILURE;
        }
        e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.config = 
              tdsMidambleConfiguration[e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->
             e_AGCH_Set_Config.elem[temp_i].midambleConfiguration];

      }/* for */
    }
    else
    {
      /* as per our current understanding NW should not provide E-AGCH info
         unless E-HICH is provided in config message or already stored in UE */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: NW trying to provide E-AGCH info without E-HICH Info");
      return(FAILURE);
    }

    tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_channel_mask |= TDSL1_E_AGCH_INFO_INCL;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"E_AGCH save finish. set e_dl_channel_mask |= TDSRRCLLC_E_AGCH_INFO_PRESENT");
  }

  return(SUCCESS);
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r7()

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r7
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r7 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  tdsrrc_E_HICH_Information_TDD128 * e_hich_info_ptr =NULL;

  /* Look into this IE only if next UE state is DCH */
  if (dl_info_per_rl_list_present && 
      (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
     )
  {
    boolean is_e_dl_info_present = FALSE;
    tdsrrc_DL_InformationPerRL_List_r7 *tmp_ie_ptr = dl_info_per_rl_list_ptr;

    if (T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo2_fdd == 
            tmp_ie_ptr->elem[0].modeSpecificInfo2.t)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL FDD not supported");
      return(FAILURE);
    }
    if (T_tdsrrc_DL_InformationPerRL_r7_tdd_tdd128 != tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->t)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL tdd384_tdd768 not supported");
      return(FAILURE);
    }
    /* check if any E-Channel related info is present in this RL Info */
    if ((tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->u.tdd128->m.e_HICH_InfoPresent)
             ||(tmp_ie_ptr->elem[0].m.e_AGCH_InformationPresent))
    {
      /* if we are inside this if check then it means that RRC needs to indicate
       * to L1 that some of the parameters in DL has changed */
          is_e_dl_info_present = TRUE;
      if(tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->u.tdd128->m.e_HICH_InfoPresent)
      {
        e_hich_info_ptr = &tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->u.tdd128->e_HICH_Info;
      }
      else
      {
        e_hich_info_ptr = NULL;
      }

      if (FAILURE == tdsrrcllcpcie_process_e_hich_info(
          tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->u.tdd128->m.e_HICH_InfoPresent, 
          e_hich_info_ptr))
      {
        return(FAILURE);
      }

      if (FAILURE == tdsrrcllcpcie_process_e_agch_info_r7(
          TDSRRC_CHECK_MSG_TYPE_IE(tmp_ie_ptr->elem[0], 
                   e_AGCH_InformationPresent), 
          &tmp_ie_ptr->elem[0].e_AGCH_Information))
      {
        return(FAILURE);
      }
    }/* if any E-info IE is present */

    /* update the l1_e_req_mask */
    tdsrrcllc_update_l1_e_req_mask_for_e_dl_info(is_e_dl_info_present);
  }/* dl_info_per_rl_list_present */
  return(SUCCESS);
}

#endif /*FEATURE_TDSCDMA_HSUPA*/

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_cctrch_list_r7()

DESCRIPTION   This function translates the Downlink CCTrCH List
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.21 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_cctrch_list_r7
(
  tdsrrc_DL_CCTrCh_r7 *ie_ptr,
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
)
{
  /* check If no "activation time" IE  for TDM, we can reject */
  if((ie_ptr->timeInfo.m.activationTimePresent == 0)&&
       ((ie_ptr->m.commonTimeslotInfoPresent == 1)&&
        (ie_ptr->commonTimeslotInfo.m.repetitionPeriodAndLengthPresent == 1)&&
        (ie_ptr->commonTimeslotInfo.repetitionPeriodAndLength.t 
                      != T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod1)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"no activation time for TDM,reject");
    return FAILURE;
  }

  /* Get Active time info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->timeInfo, tdsrrc_TimeInfo, activationTime))
  {
    dpch->dl_cctrch_list.time_info.activation_time =
                                   ie_ptr->timeInfo.activationTime;
  }
  else
  {
  /** from 10.3.6.83:
         Frame number start of the physical channel existence. Default value is "Now" */
    dpch->dl_cctrch_list.time_info.activation_time =
                                   TDSRRCLLC_ACTIVATION_TIME_NOW;
  }

  /* Get common timing slot info(10.3.6.10) */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr, tdsrrc_UL_CCTrCH_r4,
                                      commonTimeslotInfo))
  {
    if(FAILURE == tdsrrcllcpcie_get_com_ts_info_r4(&ie_ptr->commonTimeslotInfo,
              &dpch->dl_cctrch_list.common_ts_info, TRUE))
    {
      return FAILURE;
    }
  }

  /* Get TPC list */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr, tdsrrc_DL_CCTrCh_r7,
                                      ul_CCTrChTPCList))
  {
    /* 10.3.6.21  This list is not required for 1.28 Mcps TDD and is to be ignored by the UE. */
  }

  /* Get Downlink DPCH timeslots and codes info(10.3.6.32) */
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->tddOption,
                    T_tdsrrc_DL_CCTrCh_r7_tddOption_tdd128))
  {
    if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_r4((tdsrrc_DL_CCTrCh_r4_tdd128 *)ie_ptr->tddOption.u.tdd128,
                     &(dpch->dl_cctrch_list)))
    {
      return FAILURE;
    }
  }
  else if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->tddOption,
                    T_tdsrrc_DL_CCTrCh_r7_tddOption_tdd384))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd384,not tdd128");
    return FAILURE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd768,not tdd128");
    return FAILURE;
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r7()

FILE NAME     This function gets the DL DPCH info for each RL

DESCRIPTION   This function translates the Downlink DPCH info for each
              RL IE to the RRC config database.  Note that it is assumed
              that a higher level routine is doing the translation from
              the linked list present in the ASN.1 decoder output to
              the array present in the RRC config database.  The radio
              link index is passed into this function and must be
              generated by that higher level routine. IEs refer to 10.3.6.21.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if everything is ok, FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all of the parameters
              that are present in the IE.  For those parameters not
              present the values currently existing are untouched.  For
              each optional parameter that is present the corresponding
              bit in the optional parameters presence bitmask is set.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r7
(
  /* pointer to the IE */
  tdsrrc_DL_DPCH_InfoPerRL_r7 *ie_ptr,

  /* pointer to the desired DL PhyChan parameter structure */
  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr, 
              rrc_DL_DPCH_InfoPerRL_r7_tdd))
  {
    /** TDD support only one CCTrCH. If OTA message send ul_CCTrCHListToRemove,
         RRC consider it invalid */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->u.tdd,
              dl_CCTrChListToRemovePresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDD only have one CCTrCH,ul_CCTrCHListToRemove is invalid");
      return FAILURE;
    }

    /* Get Downlink CCTrCH LIST from OTA message */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->u.tdd,
               dl_CCTrChListToEstablishPresent))
    {
      if (1 < ie_ptr->u.tdd->dl_CCTrChListToEstablish.n)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ul_CCTrCHList %d is invalid",(ie_ptr->u.tdd->dl_CCTrChListToEstablish.n));
        return FAILURE;
      }

      if (FAILURE == tdsrrcllcpcie_get_dl_cctrch_list_r7(&ie_ptr->u.tdd->dl_CCTrChListToEstablish.elem[0],
                                 &tdsordered_config_ptr->l1_dl_chan_parms.dpch))
      {
        return FAILURE;
      }
    }
    else
    {
      /*dl cctrch list is optional, and it must be present from non_DCH to DCH.
             So RRC add this check. DL_DPCH_InfoPerRL only be used in DCH.
             Then next state must be DCH. So here RRC only check current state. */
      if (rrc_state != TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"dl_CCTrCHList is not present from non-DCH to DCH");
        return FAILURE;
      }
    }
  }
  else
  {
    /* TDD choice not selected, FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD not supported");
    return FAILURE;
  }

  /* everything went ok */
  return SUCCESS;
}

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_for_each_rl_r7()

DESCRIPTION     This function translates the Downlink information for
                each radio link IE to the RRC config database

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
static tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllcpcie_get_dl_info_for_each_rl_r7
(
  tdsrrc_DL_InformationPerRL_r7 *ie_ptr,

  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan

)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Get Dl Info per RL");
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd))
  {
    /* First check if the DL DPCH Info per RL is included. If not,
    * this is NOT a valid Radio link - ignore it.
    */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
                dl_dpchInfoPresent))
    {
      /* save tstd info */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
        rrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption,
            rrc_PrimaryCCPCH_Info_r4_tddOption_tdd128))
        {
          dl_phychan->pccpch.tstd_indicator =
            ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption.u.tdd128->tstd_Indicator;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD128 info present in DL_INFO_FOR_EACH_RL");
          return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }
      /*If Cell_id is present for this RL, store it in tdscell_id_per_rl */
      if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                cell_idPresent))
      {
        tdscell_id_per_rl[0] = (uint32) tdsrrc_translate_cell_id(&(ie_ptr->cell_id));
      }       
      /* Get the DL DPCH info for this radio link */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get DL DPCH Info for RL %d",1);

      if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->dl_dpchInfo, 
                  T_tdsrrc_DL_InformationPerRL_r7_dl_dpchInfo_dl_DPCH_InfoPerRL))
      {
        if (tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r7(ie_ptr->dl_dpchInfo.u.dl_DPCH_InfoPerRL,
          dl_phychan) == FAILURE)

        {
          return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDPCH Unsupported");
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;

      }

      /* if we made it here we must be ok */
      /* Indicate that Downlink L1 needs to be configured */
      tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
      * still needs to be reconfigured so as to get the new CCTrCH
      * ID whenever Downlink L1 is reconfigured. This is due to the
      * interface requirements.
      */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
      return TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS;
    }  /* DL DPCH Info is present */
    else
    {
      /* DL DPCH Info per RL is NOT present - ignore this IE */
      return TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
    }
  }
  else
  {
    /* No TDD info.  We don't support FDD so return indicating an error */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
    return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
  }
} /* tdsrrcllc_get_dl_info_for_each_rl_r5() */

/*===========================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_per_rl_r7()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_info_per_rl_r7
(
  tdsrrc_DL_InformationPerRL_List_r7 *ie_ptr,
  tdsrrc_RRC_StateIndicator next_state
)
{
  /* is_inter_freq flag indicates if it is an Inter Freq HHO */
 /* boolean is_inter_freq = FALSE; */
  tdsrrcllcoc_dl_info_per_rl_status_e_type status =
    TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  tdsordered_config_ptr->cpid_present = FALSE;

  /* servingHSDSCH_RL_indicator is FDD only, delete */
  if (ie_ptr->n > 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"more than one RL not supported");
    return FAILURE;
  }
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[0].modeSpecificInfo, 
                  T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL FDD not supported");
    return FAILURE;
  }
  if (T_tdsrrc_PrimaryCCPCH_Info_r4_fdd == ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL PrimaryCCPCH FDD not supported");
    return FAILURE;
  }

  /* We can say with certainty that if frequency info is included, and
  * it is different than the current frequency, then we are doing a
  * hard handover.  If this is not the case, we revisit this flag
  * later.
  */
   /* is_inter_freq = (tdsordered_config_ptr->p_freq_present &&
      tdsordered_config_ptr->p_freq != rrc_freq);*/

  
  if(next_state == tdsrrc_RRC_StateIndicator_cell_DCH)
  {
  /* If DPCH Info per RL is not present within DL Info per RL, then ignore it and
  * do not change the RL Info list in OC when going from DCH->DCH.
  * When going from non-DCH->DCH, the above condition should result in a Failure.
    */
    if(!((TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[0], 
                    dl_dpchInfoPresent)) &&
            ((TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[0].dl_dpchInfo, 
                     T_tdsrrc_DL_InformationPerRL_r7_dl_dpchInfo_dl_DPCH_InfoPerRL))
      )))
    {
      if(rrc_state == TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring DL Info per RL Info absent");
        if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          /* when state change from CELL_DCH to CELL_DCH, save new cellParametersID if present. Else maintain */
          tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          tdsordered_config_ptr->cpid_present = TRUE;
        }
        return SUCCESS;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failing,DPCH Info per RL not present");
        return FAILURE;
      }
    }

    /* state to CELL_DCH, dl_dpchInfo Present, save new cellParametersID if present. Else maintain */
    if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      tdsordered_config_ptr->cpid_present = TRUE;
    }

    if(rrc_state != TDSRRC_STATE_CELL_DCH)
    {
      /* msg have no cell_param_id */
      if (FALSE == tdsordered_config_ptr->cpid_present)
      {
        /* set current cell_param_id to OC structrue */
        tdsordered_config_ptr->cell_param_id = (uint8)tdsrrcccm_get_curr_camped_cpid();
      }
      /* Going from non-DCH state to CELL DCH */
      /* Always set new_rl_seen when coming to DCH from a non-DCH state.
      * That ensures the RL Info is populated after initializing.
      */
      if ((TRUE == tdsordered_config_ptr->p_freq_present)&&
          (tdsordered_config_ptr->p_freq == rrc_freq))
      {
        /* check if the current camped on ParaID is msg's RL.
         * If so, force Frequency_info to FALSE.
         */
        if ((ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)&&
           (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID != tdsrrcccm_get_curr_camped_cpid()))
        {
          /* Intra freq HHO */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"intra-f HHO FACH->DCH");
        }
        else
        {
          tdsordered_config_ptr->p_freq_present = FALSE;
        }
      }
    }
    else /* DCH -> DCH and hho */
    {
      if(tdsordered_config_ptr->is_hho)
      {
        tdshsdpa_msg_params.msg_params_present = TRUE;
      }
    }

    /* Process the IE and place the info into our database. */
    status = tdsrrcllcpcie_get_dl_info_for_each_rl_r7(&ie_ptr->elem[0],
        &tdsordered_config_ptr->l1_dl_chan_parms);

    if(status == TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE)
    {
      return FAILURE;
    }
    else if(status == TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED)
    {
      //continue;
    }
    else if(status == TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS)
    {
      /* either inter HHO or intra HHO */
      if(tdsordered_config_ptr->is_hho)
      {
        /* If we succeeded in processing the IE, and we are doing a
         * hard handover, we must be sure that L1 is informed by
         * sending frequency info, even if we didn't get frequency
         * info in the reconfiguration message.
         */
        if(!tdsordered_config_ptr->p_freq_present)
        {
          tdsordered_config_ptr->p_freq_present = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO triggered");
        }
      }
    }

  }
  else /* All states other than Cell_DCH */
  {
    if(ie_ptr->n > 0)
    {
      if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
      {
        tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        tdsordered_config_ptr->cpid_present = TRUE;
      }
    }
  }

  return SUCCESS;

} /* end tdsrrcllcpcie_get_dl_info_per_rl_r6() */


/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_info_per_rl_list_r7)

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_info_per_rl_list_r7
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r7 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  if (dl_info_per_rl_list_present)
  {
    if(FAILURE == tdsrrcllcpcie_get_dl_info_per_rl_r7(dl_info_per_rl_list_ptr, rcvd_state_indicator))
    {
      return (FAILURE);
    }
  }

#ifdef FEATURE_TDSCDMA_HSUPA
  /* process EUL related information */
  if (FAILURE == tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r7(
      dl_info_per_rl_list_present, 
      dl_info_per_rl_list_ptr,
      rcvd_state_indicator))
  {
    return(FAILURE);
  }
#endif /* FEATURE_TDSCDMA_HSUPA */

  return(SUCCESS);
}
/*====================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_common_for_all_rl_r7()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_info_common_for_all_rl_r7
(
  /* Pointer to the DL CommonInformation IE */
  tdsrrc_DL_CommonInformation_r7 *ie_ptr,

  /* Pointer to the desirec DL PhyChan common information structure */
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
)
{
  tdsrrc_state_e_type rrc_state;
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Info Common for all RL");

  /* Check if HS Reset is indicated */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
              mac_hsResetIndicatorPresent))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSReset Indicator TRUE");
    /* Set it in MAC params */
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = TRUE;
    tdshsdpa_msg_params.msg_params_present = TRUE;
  }
  /* Get the info common for all DL DPCH if present */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
              dl_dpchInfoCommonPresent))
  {
    if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->dl_dpchInfoCommon, 
               T_tdsrrc_DL_CommonInformation_r7_dl_dpchInfoCommon_dl_DPCH_InfoCommon))
    {
      if(tdsrrcllcpcie_get_dl_dpch_common_info_r6(ie_ptr->dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
                                 &dpch->dpch_common
                                ) == FAILURE
        )
      {
        return FAILURE;
      }
    }
    else /* rrc_dl_FDPCH_InfoCommon_chosen */
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: rrc_dl_FDPCH_InfoCommon_chosen not yet supported");
      return(FAILURE);
    }
  } /* end if dl dpch common info present */

  /* Get modeSpecificInfo(10.3.6.24) */
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
               T_tdsrrc_DL_CommonInformation_r7_modeSpecificInfo_tdd))
  {
    /* Get tstd indicator */
    if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->tddOption,
          T_tdsrrc_DL_CommonInformation_r7_tddOption_tdd128))
    {
      dpch->dpch_common.tstd_indicator =
        ie_ptr->modeSpecificInfo.u.tdd->tddOption.u.tdd128->tstd_Indicator;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd->tddOption is not tdd128");
      return FAILURE;
    }

    /* Get the default DPCH offset if present */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
                  defaultDPCH_OffsetValuePresent))
    {
      /* DefaultDPCH-OffsetValueTDD ::= INTEGER (0..7) */
      dpch->doff =
        (uint8) ie_ptr->modeSpecificInfo.u.tdd->defaultDPCH_OffsetValue;
  
      if(dpch->doff > TDSRRCLLC_ASN1_DOFF_MAX_VAL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Doff ASN1 value out of range");
        return(FAILURE);
      }
      dpch->opt_parm_presence_bitmask |=
        TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
    }

    else
    {
      rrc_state = tdsrrc_get_state();

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_INIT)
      {
        /*For HHO, assign DOFF = 0 to L1*/
        if((tdsordered_config_ptr->is_hho)&&(rrc_state == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Non-DCH ->DCH case. Pending clarification from NW. If yes, merge with above case*/
        else if(rrc_state != TDSRRC_STATE_CELL_DCH)
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Return failure since DOFF is missing, e.g. DCH->DCH reconfig case*/
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"We do fault-tolerance for DOFF is missing");
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }

      }

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN)
      {
        /*For Non-DCH -> DCH case, make DOFF = 0.*/
        if((rrc_state != TDSRRC_STATE_CELL_DCH)&&(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
          dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
      }
    }  /* Doff is absent */
  }
  /* end if common TDD */
  else
  {
    /* TDD not selected.  Error as FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_COMMON_FOR_ALL_RL");
    return FAILURE;
  }
  /* Indicate that Downlink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
 /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_common_info_r7()

DESCRIPTION:
  This function updates IE "dl common info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_common_info_r7
(
  uint32 dl_common_info_present,
  tdsrrc_DL_CommonInformation_r7 *dl_common_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Get the DL information for all Radio Links */
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
  /* Initialize the L1 bitmask for the parameters in the "Downlink
  * information for all radio links" IE.  Since the IE is optional,
  * we want to make sure we don't signal L1 erroneously that we have
  * received information when we haven't.
    */

    /* Init DL Common Info to Timing Maintained */
    tdsrrcllc_init_dl_common_info(tdsordered_config_ptr);
    
    if (dl_common_info_present)
    {
      if(tdsrrcllcpcie_get_dl_info_common_for_all_rl_r7(dl_common_info_ptr,
        &tdsordered_config_ptr->l1_dl_chan_parms.dpch
        )
        == FAILURE)
      {
        return FAILURE;
      }
    } /* end DL common RL info */
    
  } /* If next state is Cell_DCH */
  /* Calculate the DL TFCS */
  tdsrrcllc_update_rm_restriction_info(&tdsordered_config_ptr->l1_dl_cctrch_parms,
    &tdsordered_config_ptr->dl_ctfc,
    &tdsordered_config_ptr->l1_dl_ctfc_parms
    );
   
  return(SUCCESS);
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_r7()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r7
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r7 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  boolean is_hho = FALSE;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_DL_InformationPerRL_List_r7 *tmp_ie_ptr = dl_info_per_rl_list_ptr;
  tdsrrc_state_e_type curr_rrc_state = tdsrrc_get_state();

  uint8               cpid = TDSRRC_INVALID_CPID;

  /* If state from CELL_DCH/CELL_FACH/CONNECTING to CELL_DCH */
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"curr_rrc_state = %d",curr_rrc_state);
    cpid = (uint8)tdsrrcccm_get_curr_camped_cpid();
    /* If two FreqInfo are included in the OTA */
    if ((tdsordered_config_ptr->p_freq_present)&&(tdsordered_config_ptr->s_freq_present))
    {
      /*  If the first frequency got changed. */
      if (rrc_freq != tdsordered_config_ptr->p_freq)
      {
        /* It is a inter handover */
        is_hho = TRUE;
        tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different freq %d",tdsordered_config_ptr->p_freq);
      }
      if (FALSE == is_hho)
      {
        if (dl_info_per_rl_list_present)
        {
          if (tmp_ie_ptr == NULL)
          {
            ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
          }
          /*  If CPID got changed. */
          if ((T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd ==
                 tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
                 (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                     tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                     (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
          {
            if(cpid != tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
            {
              /* It is a intra handover */
              is_hho = TRUE;
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
            }
          }
        }
      }
    }
    /* If only one FreqInfo is included in the OTA */
    else if ((tdsordered_config_ptr->p_freq_present)||(tdsordered_config_ptr->s_freq_present))
    {
      if (dl_info_per_rl_list_present)
      {
        if (tmp_ie_ptr == NULL)
        {
          ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
        }
        /*  If CPID got changed. */
        if ((T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd ==
               tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
               (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                   tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                   (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          if(tdsrrcccm_get_curr_camped_cpid() != 
                tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
          {
            /* It is a handover */
            is_hho = TRUE;
            if (tdsordered_config_ptr->p_freq != rrc_freq)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a inter handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a intra handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
            }
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
          }
        }
      }
    }
#ifdef FEATURE_TDSCDMA_SINGLE_FREQ_NO_CPID_HO_SUPPORT

      if (tdsordered_config_ptr->p_freq_present)
      {
        /*  If the first frequency got changed. */
        if (rrc_freq != tdsordered_config_ptr->p_freq)
        {
          /* It is a inter handover */
          is_hho = TRUE;
          tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Support none cpid and one freq info HO. p_freq %d different with current freq %d",tdsordered_config_ptr->p_freq,rrc_freq);
        }
      }

#endif
  }

  if (TRUE == is_hho)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO detected different freq different cell para id ");
  }
  tdsordered_config_ptr->is_hho = is_hho;
  
  return SUCCESS;
}
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r7()

DESCRIPTION   This function translates the Uplink DPCH Power Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs can refer to 10.3.6.91 in 25331v716
              of 3GPP.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r7
(
 tdsrrc_UL_DPCH_PowerControlInfo_r7_tdd *tdd_ul_pwrcntrl_info
)
{
/**     2> if the IE "Uplink DPCH power control info" is included in the IE "Uplink DPCH info":
  3> if the IE " UL target SIR " is included ignore the parameter. */

  /* According to 8.6.6.11 in 3GPP 25331v716, if the CHOICE UL OL PC info is set to
       'Broadcast UL OL PC info', set the variable INVALID_CONFIGURATION to true. */
  if (TDSRRC_CHECK_MSG_TYPE(tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling,
              T_tdsrrc_UL_DPCH_PowerControlInfo_r7_ul_OL_PC_Signalling_broadcast_UL_OL_PC_info))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"1.28TDD unsupport broadcast UL OL PC info,failure ");
    return FAILURE;
  }

  /* if tdd choice is tdd384, return failure */
  if (TDSRRC_CHECK_MSG_TYPE(tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->tddOption,
                  T_tdsrrc_UL_DPCH_PowerControlInfo_r7_tddOption_tdd384))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"dpch pwr select unsupport tdd3.84,failure ");
    return FAILURE;
  }

  /* if tdd choice is tdd768, return failure */
    if (TDSRRC_CHECK_MSG_TYPE(tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->tddOption,
                  T_tdsrrc_UL_DPCH_PowerControlInfo_r7_tddOption_tdd768))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"dpch pwr select unsupport tdd7.68,failure ");
    return FAILURE;
    }

    //save beaconPLEst
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128,
          beaconPLEst))
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
    }

    /** the value range of tpc_step_size is 1, 2, 3.
         The first num of tpc_step_size in OC struct is zero */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tpc_step_size = 
    tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128->tpc_StepSize;

  /* in 3GPP 10.3.6.91, primaryCCPCH_TX_Power is optional. It is not correct.
       Because in 11th chapter, it is not optional. */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.pccpch_tx_pwr = 
    tdd_ul_pwrcntrl_info->ul_OL_PC_Signalling.u.individuallySignalled->primaryCCPCH_TX_Power;

  return SUCCESS;
} /* end tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r7() */

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_dpch_info_r7()

DESCRIPTION   This function translates the Uplink DPCH info IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_ul_dpch_info_r7
(
  tdsrrc_UL_DPCH_Info_r7 *ie_ptr
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL DPCH Info");

  /* Get the UL DPCH power control information if present */
  if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
              ul_DPCH_PowerControlInfoPresent))
  {
    if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->ul_DPCH_PowerControlInfo, 
               T_tdsrrc_UL_DPCH_PowerControlInfo_r7_tdd))
    {
      if(tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r7((tdsrrc_UL_DPCH_PowerControlInfo_r7_tdd *)
      ie_ptr->ul_DPCH_PowerControlInfo.u.tdd
      ) == FAILURE)
      {
        return FAILURE;
      }
    }
    else /* only FDD */
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No valid TDD data for UL DPCH Power Control Info");
      return FAILURE;
    }
  }
  else
  {
    /* both in FDD and TDD;
  2> if the IE "Uplink DPCH power control info" is not included in a message used to enter CELL_DCH:
  3> set the variable INVALID_CONFIGURATION to true. */
    if ((tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH) &&
          (rrc_state != TDSRRC_STATE_CELL_DCH))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"no UlDpchPwrInfo from nonDCH to DCH");
      return FAILURE;
    }
  }

  /* Make sure of FDD and get the right parameters */
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo, 
              T_tdsrrc_UL_DPCH_Info_r7_modeSpecificInfo_tdd))
  {
    if(tdsrrcllcpcie_update_uldpch_mode_specific_info_r7(ie_ptr->modeSpecificInfo.u.tdd) == FAILURE)
    {
      return FAILURE;
    }
  }
   /* end TDD if */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD UL DPCH information present");
    return FAILURE;
  }

  /* If made it here, all is well */
  /* Indicate that Uplink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
  return SUCCESS;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_dpch_info_r7()

DESCRIPTION:
  This function updates IE "ul dpch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_dpch_info_r7
(
  uint32 ul_dpch_info_present,
  tdsrrc_UL_DPCH_Info_r7 *ul_dpch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Get Uplink DPCH info if included, Ignore CPCH info */
  if (ul_dpch_info_present)
  {
    if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
    {
      if(tdsrrcllcpcie_get_ul_dpch_info_r7(ul_dpch_info_ptr) == FAILURE)
      {
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: Ignoring rcvd ulDpchInfo when state-indicator is NOT set to DCH");
    }
  }
  return(SUCCESS);
}

#ifdef FEATURE_TDSCDMA_HSUPA
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_e_dch_information_r7()

DESCRIPTION:
  This function updates IE "ul e dch information" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_e_dch_information_r7
(
  uint32 e_dch_info_present,
  tdsrrc_UL_EDCH_Information_r7 *e_dch_info_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Look into this IE only if next UE state is DCH */
  if (e_dch_info_present && 
      (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
     )
  {
    if ((TDSRRC_CHECK_MSG_TYPE_IE_PTR(e_dch_info_ptr, 
                   mac_es_e_resetIndicatorPresent)) &&
        (tdsrrc_UL_EDCH_Information_r7_mac_es_e_resetIndicator_true_ == e_dch_info_ptr->mac_es_e_resetIndicator))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"mac_es_e_resetIndicator set to TRUE");
      tdsordered_config_ptr->mac_e_reset_indicator = TRUE;
    }
    else
    {
      tdsordered_config_ptr->mac_e_reset_indicator = FALSE;
    }
    if(TDSRRC_CHECK_MSG_TYPE(e_dch_info_ptr->modeSpecificInfo, 
                  T_tdsrrc_UL_EDCH_Information_r7_modeSpecificInfo_tdd))
    {
      if (FAILURE == tdsrrcllcpcie_process_e_rucch_info(
           e_dch_info_ptr->modeSpecificInfo.u.tdd->m.e_RUCCH_InfoPresent, 
              &e_dch_info_ptr->modeSpecificInfo.u.tdd->e_RUCCH_Info))
      {
        return(FAILURE);
      }

      if (FAILURE == tdsrrcllcpcie_process_e_puch_info(
          e_dch_info_ptr->modeSpecificInfo.u.tdd->m.e_PUCH_InfoPresent, 
            &e_dch_info_ptr->modeSpecificInfo.u.tdd->e_PUCH_Info))
      {
        return(FAILURE);
      }

      
      if (FAILURE == tdsrrcllcpcie_process_non_sched_info(
           e_dch_info_ptr->modeSpecificInfo.u.tdd->m.non_ScheduledTransGrantInfoPresent,
            &e_dch_info_ptr->modeSpecificInfo.u.tdd->non_ScheduledTransGrantInfo))
      {
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport mode FDD,failure");
      return FAILURE;
    }
  }

  return(SUCCESS);
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_non_sched_info()

DESCRIPTION:
  This function updates IE "Non-scheduled transmission grant info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_non_sched_info
(
  uint32 e_non_sched_info_present,
  tdsrrc_Non_ScheduledTransGrantInfoTDD *e_non_si_ptr
)
{
  if (e_non_sched_info_present == TRUE)
  {
    if (e_non_si_ptr == NULL)
    {
      ERR_FATAL("Null pointer received",0,0,0);
      return FAILURE;
    }
    
    if (e_non_si_ptr->t == T_tdsrrc_Non_ScheduledTransGrantInfoTDD_tdd384_768)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Non-SI unsupport mode TDD384 and TDD768,failure");
      return FAILURE;
    }
    /* Number of EUCCH and TPC instances within an E-DCH TTI. Default = 1. */
    if (e_non_si_ptr->u.tdd128->m.n_E_UCCHPresent)
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.num_e_ucch = 
          e_non_si_ptr->u.tdd128->n_E_UCCH;
    }
    else
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.num_e_ucch = TDSRRC_DEFAULT_N_E_UCCH;
    }
    
    /* Minimum number of slots between start last active slot of E-DCH TTI and start of ACK/NACK on EHICH. */
    if (e_non_si_ptr->u.tdd128->m.n_E_HICHPresent)
    {
      /* Invalid N-E-HICH in scheduled E-PUCH information */
      if (tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich == 0)
      {
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich = 
          e_non_si_ptr->u.tdd128->n_E_HICH;
      }
      else
      {
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich = 
           tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"N-E-HICH present non_sched,but use sched value = %d",tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich);
      }
    }
    else
    {
      /* Invalid N-E-HICH in scheduled E-PUCH information */
      /* If NE-HICH is given in scheduled E-PUCH information, this IE is not needed, otherwise it is mandatory */
      if (tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich == 0)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR," N-e-hich not match CV condition ");
        return FAILURE;
      }
      else
      {
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich = 
           tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"N-E-HICH absent non_sched,so use sched value = %d",tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich);
      }
    }

    /* Bitmask  indicating  which of the Timeslots configured for EDCH are allocated for non-scheduled transmissions */
    if (e_non_si_ptr->u.tdd128->timeslotResourceRelatedInfo.numbits != TDSRRC_MAX_NUMBITS_5)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received timeslotResourceRelatedInfo Length: %d is invalid",e_non_si_ptr->u.tdd128->timeslotResourceRelatedInfo.numbits);
      return FAILURE;
    }
    else
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.trri = 
         e_non_si_ptr->u.tdd128->timeslotResourceRelatedInfo.data[0] >> 3;
      TDSRRC_MSG1(MSG_LEGACY_LOW,"trri in nonsched is %d",tdsordered_config_ptr->l1_e_info.non_sched_grant_info.trri);
    }

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.prri = 
          e_non_si_ptr->u.tdd128->powerResourceRelatedInfo;

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.activation_time = 
          e_non_si_ptr->u.tdd128->activationTime;

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.subframe_num = 
          e_non_si_ptr->u.tdd128->sfnNum;
    
    if (e_non_si_ptr->u.tdd128->m.repetitionPeriodAndLengthPresent)
    {
      switch (e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.t)
      {
      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod1:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_1;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length = 0;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod2:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_2;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod2;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod4:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_4;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod4;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod8:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_8;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod8;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod16:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_16;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod16;
        break;
      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod32:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_32;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length=
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod32;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod64:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_64;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod64;
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"repetitionPeriod is out of range");
        return FAILURE;
      } 
    }
    else
    {
      /* Default is "Repetition period" = 1 (continuous) */
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_1;
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length = 0;
    }
      

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.crri = 
          e_non_si_ptr->u.tdd128->codeResourceInfo;
    

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.ts_num = 
         e_non_si_ptr->u.tdd128->e_HICH_Info.timeslotNumber;

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.sig_group_index = 
         e_non_si_ptr->u.tdd128->e_HICH_Info.signatureSequenceGroupIndex;

    if (e_non_si_ptr->u.tdd128->e_HICH_Info.midambleAllocationMode.t == 
              T_tdsrrc_Non_ScheduledTransGrantInfoTDD_midambleAllocationMode_defaultMidamble)
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.
           mid_code.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
    }
    else
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.
          mid_code.allocation_mode = TDSL1_UE_SPECIFIC_MIDAMBLE;
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.mid_code.shift = 
          e_non_si_ptr->u.tdd128->e_HICH_Info.midambleAllocationMode.u.ueSpecificMidamble;
    }
    
    /* L1:Integer (2, 4, 6, 8, 10, 12, 14, 16);
        ASN1: midambleConfiguration   Integer (1...8), */
    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.mid_code.config =
          tdsMidambleConfiguration[e_non_si_ptr->u.tdd128->e_HICH_Info.midambleConfiguration];    

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.chan_code = 
         e_non_si_ptr->u.tdd128->e_HICH_Info.channelisation_Code;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting E-PUCH non-scheduled trans grant info success");
    
    /* Since L1 parameters are present hence update the bit mask to indicate the same */
    tdsordered_config_ptr->l1_e_req_mask |= TDSRRCLLC_NON_SCHED_GRANT_START;
    tdsrrcllc_update_mac_eul_action(TRUE);
  }

  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_puch_info()

DESCRIPTION:
  This function updates IE "E-PUCH Info" (3GPP-25.331-10.3.6.104) in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type  tdsrrcllcpcie_process_e_puch_info
(
  uint32 e_puch_info_present,
  tdsrrc_E_PUCH_Info *e_puch_info_ptr
)
{

  uint8  i, j;
  tdsl1_ref_e_tfci_struct_type   ReferBetaTemp;

  if (e_puch_info_present == FALSE)
  {
    /* No need to save E-PUCH info to OC */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"E-PUCH info absent");
    return SUCCESS;
  }
  
  if (e_puch_info_ptr == NULL)
  {
    ERR_FATAL("Null pointer received",0,0,0);
    return FAILURE;
  }

  /* Handle mode info */ 
  if (e_puch_info_ptr->modeSpecificInfo.t == T_tdsrrc_E_PUCH_Info_modeSpecificInfo_tdd348_tdd768)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"E-PUCH Info unsupport mode TDD384 and TDD768,failure");
    return FAILURE;
  }
  else
  {
    if (e_puch_info_ptr->modeSpecificInfo.u.tdd128->m.snpl_ReportTypePresent)
    {
      if (e_puch_info_ptr->modeSpecificInfo.u.tdd128->snpl_ReportType == 
                 tdsrrc_E_PUCH_Info_snpl_ReportType_type1)
      {
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.snpl_rep_type = TDSL1_SNPL_REP_TYPE_1;
      }
      else
      {
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.snpl_rep_type = TDSL1_SNPL_REP_TYPE_2;
      }
    }
    else
    {
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.snpl_rep_type = TDSL1_SNPL_REP_TYPE_NONE;
    }
    
    /* Reference desired E-PUCH Rx power, Int8(-112..-50 by step of 1) */
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.prx_des_base = 
         e_puch_info_ptr->modeSpecificInfo.u.tdd128->prxBASEdes;
    
    /* True indicates that UE may take into account path loss 
          estimation from beacon function physical channels.Default value is false. */
    if (e_puch_info_ptr->modeSpecificInfo.u.tdd128->m.beaconPLEstPresent)
    {
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.beacon_pl_est = 
           e_puch_info_ptr->modeSpecificInfo.u.tdd128->beaconPLEst;
    }
    else
    {
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.beacon_pl_est = FALSE;
    }

    /* Enum, TPC step size in db(1,2,3) */
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.tpc_step = 
           e_puch_info_ptr->modeSpecificInfo.u.tdd128->tpc_StepSize;

    if (e_puch_info_ptr->modeSpecificInfo.u.tdd128->m.pebase_PowerControlGAPPresent)
    {
      /* When time since last TPC command is longer than p-base_cont_gap,  L1 sets  Pe-base to prx_des_base */
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.p_base_contr_gap = 
           e_puch_info_ptr->modeSpecificInfo.u.tdd128->pebase_PowerControlGAP;
    }
    else
    {
      /*10.3.6.104. Default value is 1. */
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.p_base_contr_gap = TDSRRC_DEFAULT_P_BASE_GAP;
    }

    if (e_puch_info_ptr->modeSpecificInfo.u.tdd128->m.ul_SynchronisationParametersPresent) 
    {
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ul_sync_step = 
         e_puch_info_ptr->modeSpecificInfo.u.tdd128->ul_SynchronisationParameters.stepSize;
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ul_sync_freq = 
         e_puch_info_ptr->modeSpecificInfo.u.tdd128->ul_SynchronisationParameters.frequency;
    }
    else
    {
      /* Default: Uplink synchronisation step size 1. Uplink synchronisation frequency 1. */
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ul_sync_step = TDSRRC_DEFAULT_UL_SYNC_STEP;
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ul_sync_freq = TDSRRC_DEFAULT_UL_SYNC_FREQ;
    }

    /* Save IE "e_PUCH_TS_ConfigurationList" */
    if (tdsrrcllcpcie_e_puch_tscfg_update(e_puch_info_ptr->modeSpecificInfo.u.
         tdd128->e_PUCH_TS_ConfigurationList) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"e_PUCH_TS_ConfigurationList,failure");
      return FAILURE;
    }
    
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.max_code_rate = 
        e_puch_info_ptr->modeSpecificInfo.u.tdd128->maximum_Allowed_Code_Rate;
    
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.min_code_rate = 
        e_puch_info_ptr->modeSpecificInfo.u.tdd128->minimum_Allowed_Code_Rate;

    tdsordered_config_ptr->mac_e_config.eul_si_info.e_max_num_of_retx_for_si_only =
         e_puch_info_ptr->modeSpecificInfo.u.tdd128->maximumNumOfRetransSchedInfo;

    tdsordered_config_ptr->mac_e_config.eul_si_info.e_retx_tmr_for_si_only =
       tdsRetransTimerForSI[e_puch_info_ptr->modeSpecificInfo.u.tdd128->retransTimerForSchedInfo];
    
  }

  /*E-TFCI info */
  tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.num_ref_e_tfci_16qam = 
          e_puch_info_ptr->e_TFCS_Info.reference_Beta_16QAM_List.n;
  for (i = 0; i < e_puch_info_ptr->e_TFCS_Info.reference_Beta_16QAM_List.n; i++)
  {
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_16qam[i].ref_code_rate = 
       e_puch_info_ptr->e_TFCS_Info.reference_Beta_16QAM_List.elem[i].reference_Code_Rate;
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_16qam[i].ref_gain_factor = 
       e_puch_info_ptr->e_TFCS_Info.reference_Beta_16QAM_List.elem[i].reference_Beta;
  }

  tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.num_ref_e_tfci_qpsk= 
          e_puch_info_ptr->e_TFCS_Info.reference_Beta_QPSK_List.n;
  for (i = 0; i < e_puch_info_ptr->e_TFCS_Info.reference_Beta_QPSK_List.n; i++)
  {
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_qpsk[i].ref_code_rate = 
       e_puch_info_ptr->e_TFCS_Info.reference_Beta_QPSK_List.elem[i].reference_Code_Rate;
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_qpsk[i].ref_gain_factor = 
       e_puch_info_ptr->e_TFCS_Info.reference_Beta_QPSK_List.elem[i].reference_Beta;
  }
  /* interface requirement: ascending order 16QAM */
  for (i = 1; i < tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.num_ref_e_tfci_16qam; i++)
  {
      for (j = 0; j < tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.num_ref_e_tfci_16qam - i; j++)
      {
          if (tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_16qam[j].ref_code_rate >
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_16qam[j + 1].ref_code_rate) 
          {
              ReferBetaTemp  = tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_16qam[j];
              tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_16qam[j]   =
                   tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_16qam[j+1];     
              tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_16qam[j+1] = ReferBetaTemp;
          }
      }        
  }

  /* interface requirement:  ascending order QPSK */
  for (i = 1; i < tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.num_ref_e_tfci_qpsk; i++)
  {
      for (j = 0; ((j < tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.num_ref_e_tfci_qpsk - i) && (j < TDSL1_MAX_REF_E_TFCI_LIST - 1)); j++)
      {
          if (tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_qpsk[j].ref_code_rate >
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_qpsk[j + 1].ref_code_rate) 
          {
              ReferBetaTemp  = tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_qpsk[j];
              tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_qpsk[j]   =
                   tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_qpsk[j+1];     
              tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.ref_e_tfci_list_qpsk[j+1] = ReferBetaTemp;
          }
      }        
  }
  
  TDSRRC_MSG0(MSG_LEGACY_MED,"Setting E-PUCH infot success");

  tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_EPUCH_INFO_INCL;
  /* Since L1 parameters are present hence update the bit mask to indicate the same */
  tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);    
  tdsrrcllc_update_mac_eul_action(TRUE);
  return SUCCESS;
  
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_e_puch_tscfg_update()

DESCRIPTION:
  This function updates IE "E-RUCCH Info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_puch_tscfg_update
(
  tdsrrc_E_PUCH_Info_e_PUCH_TS_ConfigurationList_1 e_puch_ts_cfg
)
{
  uint8  i;
  tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.num_epuch_ts = e_puch_ts_cfg.n;
  for (i = 0; i < e_puch_ts_cfg.n; i++)
  {
    /* ts_num is 1~5 in UL */
    if ((0 == e_puch_ts_cfg.elem[i].timeslotNumber) ||
        (6 == e_puch_ts_cfg.elem[i].timeslotNumber))
    {
      ERR_FATAL("ts_num is %d, UL ts_num is 1~5",e_puch_ts_cfg.elem[i].timeslotNumber,0,0);
      return FAILURE;
    }
    
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.epuch_ts_cfg_list[i].ts_num = 
        e_puch_ts_cfg.elem[i].timeslotNumber;

    switch(e_puch_ts_cfg.elem[i].midambleAllocationMode.t)
    {
      case T_tdsrrc_E_PUCH_TS_Slots_LCR_midambleAllocationMode_defaultMidamble:
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.epuch_ts_cfg_list[i].
          midamble_info.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
      break;

      case T_tdsrrc_E_PUCH_TS_Slots_LCR_midambleAllocationMode_ueSpecificMidamble:
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.epuch_ts_cfg_list[i].
          midamble_info.allocation_mode = TDSL1_UE_SPECIFIC_MIDAMBLE;
        
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.epuch_ts_cfg_list[i].midamble_info.shift =
          e_puch_ts_cfg.elem[i].midambleAllocationMode.u.ueSpecificMidamble;
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error midambleAllocationMode type");
        return FAILURE;
    }
    
    /* L1:Integer (2, 4, 6, 8, 10, 12, 14, 16);
          ASN1: midambleConfiguration   Integer (1...8), */
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_puch_info.epuch_ts_cfg_list[i].midamble_info.config =
          tdsMidambleConfiguration[e_puch_ts_cfg.elem[i].midambleConfiguration];    
    TDSRRC_MSG0(MSG_LEGACY_MED,"Setting E-PUCH TS ConfigurationList SUCCESS");
  }
  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_rucch_info()

DESCRIPTION:
  This function updates IE "E-RUCCH Info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_e_rucch_info
(
  uint32 e_rucch_info_present,
  tdsrrc_E_RUCCH_Info *e_rucch_info_ptr
)
{
  uint8  i;
  if (e_rucch_info_present)
  {  
    /* IEs in 3GPP-10.3.6.103 */
    if (e_rucch_info_ptr->modeSpecificInfo.t == T_tdsrrc_E_RUCCH_Info_modeSpecificInfo_1_tdd384_tdd768)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"E-RUCCH Info unsupport mode TDD384 and TDD768,failure");
      return FAILURE;
    }

    /* 25.331-8.5.28, The PRACH configuration is signalled directly to the 
         UE in "E-RUCCH Info" IE in case of E-DCH serving cell change. It means 
         if there is a serving cell change then the n\W has to provide the E-RUCCH 
         config (The above Note clarifies it) unless maybe there is no scheduled 
         flow configured*/
     /* If UE goes from  CELL_DCH(SCHED UPA does not start)->CELL_DCH,
             If UE goes from  CELL_FACH->CELL_DCH,
             cell changed but OTA configuration without some optional IEs, RRC will reject it. */
    if ((tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH) &&
        (!(tdscurrent_config_ptr->hsupa_start_mask & TDSRRC_SCHED_START))&&
          (tdsordered_config_ptr->is_hho) &&
        ((!e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.e_RUCCH_AccessServiceClassPresent)
          ||(!e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.e_RUCCH_PersistenceScalingFactorPresent)
          || (!e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.sync_UL_InfoForE_RUCCHPresent)
          || (!e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.prach_informationPresent)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DCH->DCH,important IE absent in E-RUCCH");
      return FAILURE;
    }

    if (e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.t_RUCCHPresent)
    {
      tdsordered_config_ptr->mac_e_config.e_rucch_info.t_rucch_tmr =
          tdsRucchTimer[e_rucch_info_ptr->modeSpecificInfo.u.tdd128->t_RUCCH];
    }
    else
    {
      /* Default = 120 ms */
      tdsordered_config_ptr->mac_e_config.e_rucch_info.t_rucch_tmr = TDSRRC_DEFAULT_T_RUCCH_TIMER;
    }

    if (e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.n_RUCCHPresent)
    {
      tdsordered_config_ptr->mac_e_config.e_rucch_info.n_rucch =
          e_rucch_info_ptr->modeSpecificInfo.u.tdd128->n_RUCCH;
    }
    else
    {
      /* Default = 3 */
      tdsordered_config_ptr->mac_e_config.e_rucch_info.n_rucch = TDSRRC_DEFAULT_N_RUCCH;
    }

    if (e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.t_WAITPresent)
    {
      tdsordered_config_ptr->mac_e_config.eul_si_info.t_wait_tmr =
          tdsTWaitimer[e_rucch_info_ptr->modeSpecificInfo.u.tdd128->t_WAIT];
    }
    else
    {
      /* Default = 40 ms*/
      tdsordered_config_ptr->mac_e_config.eul_si_info.t_wait_tmr = TDSRRC_DEFAULT_T_WAIT_TIMER;
    }

    if (e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.t_SIPresent)
    {
      tdsordered_config_ptr->mac_e_config.eul_si_info.t_Sched_periodic_si_tmr =
          tdsSITimer[e_rucch_info_ptr->modeSpecificInfo.u.tdd128->t_SI];
    }
    else
    {
      /* Default = 40 ms*/
      tdsordered_config_ptr->mac_e_config.eul_si_info.t_Sched_periodic_si_tmr = TDSRRC_DEFAULT_T_SI_TIMER;
    }

    if (e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.extendedEstimationWindowPresent)
    {
      tdsordered_config_ptr->mac_e_config.eul_si_info.ext_est_window =
           e_rucch_info_ptr->modeSpecificInfo.u.tdd128->extendedEstimationWindow;
    }
    else
    {
      /* Invalid value */
      tdsordered_config_ptr->mac_e_config.eul_si_info.ext_est_window = TDSRRC_INVALID_EXT_EST_WINDOW;
    }

    /* save IE "E-RUCCH Access Service class" */
    if (FAILURE == tdsrrcllcpcie_e_rucch_asc_update(
            e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.e_RUCCH_AccessServiceClassPresent,
            &e_rucch_info_ptr->modeSpecificInfo.u.tdd128->e_RUCCH_AccessServiceClass))
    {
      return FAILURE;
    }

    /* save IE "E-RUCCH persistence scaling factor list" */
    if (e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.e_RUCCH_PersistenceScalingFactorPresent)
    {
      if (FAILURE == tdsrrcllcpcie_e_rucch_psf_update(
                &e_rucch_info_ptr->modeSpecificInfo.u.tdd128->e_RUCCH_PersistenceScalingFactor))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Persistance Scaling Factor");
        return FAILURE;
      }
    }
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER) 
    else if (TRUE == tdsrrc_is_x2td_irat_hho_pending())
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"ERUCCH psf absent during IRAT-PSHO,set default value: 1");
      for (i = 0; i < MAX_ASC_PERSIST; i++)
      {
        /* The default values are set to 1. */
        tdsordered_config_ptr->erucch_psf[i]= TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
      }
    }
#endif
    else
    {
      /* initial: When enter CELL_DCH, RRC will initial psf.
             1. CELL_FACH -> CELL_DCH, NW not config psf, RRC will get info from PRACH  
             2. CELL_DCH ->CELL_DCH, NW ont config psf, RRC first use stored eRUCCH psf previously;
                 If no valid eRUCCH psf, RRC will get info from PRACH */
      if (tdsordered_config_ptr->erucch_psf[0] == 0 )
      {
        for (i = 0; i < MAX_ASC_PERSIST; i++)
        {
          /* The default values are same as the PRACH psf. */
          tdsordered_config_ptr->erucch_psf[i]= (uint8)tdsordered_config_ptr->psf[i];
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"ERUCCH psf absent");
    }
    tdsrrcllcpcie_get_erucch_pval();

    if (e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.sync_UL_InfoForE_RUCCHPresent)
    {
      if (tdsrrcllcpcie_e_rucch_ul_syn_update(&e_rucch_info_ptr->modeSpecificInfo.u.
            tdd128->sync_UL_InfoForE_RUCCH) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"E-RUCCH ul syn Info ,failure");
        return FAILURE;
      }
    }
    else
    {
      /* 1. CELL_FACH -> CELL_DCH, NW not config sync_ul_code, RRC will get info from PRACH  
             2. CELL_DCH ->CELL_DCH, NW ont config sync_ul_code, RRC first use stored eRUCCH psf previously;
                 If no valid eRUCCH sync_ul_code, RRC will get info from PRACH */
      if (tdsordered_config_ptr->mac_e_config.e_rucch_info.mmax == 0)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SYNC UL absent,get from PARCH");
        /* UE goes from  FACH->DCH and also sets up EUL in the same config, then 
                if “E-RUCCH info” is not provided by the N\W , then RRC should take the 
                optional IEs who are "OFF" from the PRACH config.   */
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.pwr_ramp_step = 
          tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.pwr_ramp_step;
      
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.prx_upPch_des = 
          tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.prx_upPch_des;
      
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.max_sync_ul_tx = 
          tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.max_sync_ul_tx;
        /*sync_code_erucch_770ext_indicator is TRUE means ul sync code from sib5 770ext, not from PRACH.*/
        if (tdscurrent_config_ptr->sync_code_erucch_770ext_indicator == FALSE)
        {
          tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.ul_sync_code_mask =
             tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.sync_ul_codes_bit_map;
        }
      
        tdsordered_config_ptr->mac_e_config.e_rucch_info.mmax = 
             tdscurrent_config_ptr->mac_ul_parms.rach_tx_cntl_info.mmax;
      }
    }

    if (e_rucch_info_ptr->modeSpecificInfo.u.tdd128->m.prach_informationPresent)
    {
      if(tdsrrcllcpcie_e_rucch_prach_info_update(
               &e_rucch_info_ptr->modeSpecificInfo.u.tdd128->prach_information) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"prach Info in erucch ,failure");
        return FAILURE;
      }
    }    
    else
    {
      /* 1. CELL_FACH -> CELL_DCH, NW not config prach_fpach, RRC will get info from PRACH  
             2. CELL_DCH ->CELL_DCH, NW ont config prach_fpach, RRC first use stored eRUCCH psf previously;
                 If no valid eRUCCH prach_fpach, RRC will get info from PRACH */
      if (tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.num_prach_fpach == 0)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"save ERUCCH:prach info absent,get from PARCH");
        /* UE goes from  FACH->DCH and also sets up EUL in the same config, then 
               if “E-RUCCH info” is not provided by the N\W , then RRC should take the 
               optional IEs who are "OFF" from the PRACH config.   */
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.num_prach_fpach = 
           tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.num_prach_fpach;
      
        if (tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.num_prach_fpach > TDSL1_UL_PRACH_FPACH_MAX )
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"number of PRACH-FPACH over MAX");
          return FAILURE;
        }
        memscpy(tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.prach_fpach_info, 
                (sizeof(tdsl1_ul_prach_fpach_struct_type) *
             tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.num_prach_fpach),
             tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.prach_fpach_info,
             (sizeof(tdsl1_ul_prach_fpach_struct_type) *
             tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.num_prach_fpach));
      }
    }

    /* 3GPP-25.321-11.9.1.5 
        "Grant Request" type Scheduling Information can be triggered if any of the following events occur:
      - The TEBS becomes larger than zero;
      - An E-DCH serving cell change occurs and the TEBS is larger than zero;
      - An E-DCH working frequency change occurs in the serving cell and the TEBS is larger than zero;     */

    /* If there is a serving cell change or working freq change in the serving cell, notify MAC */
    if ((tdsordered_config_ptr->is_hho) || 
           (tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
    {
      tdsordered_config_ptr->mac_e_config.eul_si_info.srv_cell_or_work_freq_change = TRUE;
    }
    else
    {
      tdsordered_config_ptr->mac_e_config.eul_si_info.srv_cell_or_work_freq_change = FALSE;
    }
    TDSRRC_MSG0(MSG_LEGACY_MED,"Setting E-RUCCH info success");

    tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;
    /* Since L1 parameters are present hence update the bit mask to indicate the same */
    tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);    
    tdsrrcllc_update_mac_eul_action(TRUE);
  }

  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_e_rucch_prach_info_update()

DESCRIPTION:
  This function updates the E-RUCCH PRACH info to OC.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_rucch_prach_info_update
(
  tdsrrc_PRACH_Information_LCR_List  *e_rucch_prach_ptr
)
{
  uint8 i, j;
  tdsl1_e_rucch_info_struct_type      *e_rucch_info_ptr;
  if (e_rucch_prach_ptr == NULL)
  {
    ERR_FATAL("Null pointer received",0,0,0);
    return FAILURE;
  }

  tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.num_prach_fpach = e_rucch_prach_ptr->n;
  
  e_rucch_info_ptr = &tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info;  
  for (i = 0; i < e_rucch_prach_ptr->n; i++)
  {
    /* PRACH time slot number */
    e_rucch_info_ptr->prach_fpach_info[i].prach_ts_num = e_rucch_prach_ptr->elem[i].timeslot;

    /* The number of PRACH channelisation codes */
    e_rucch_info_ptr->prach_fpach_info[i].num_prach_codes = 
                   e_rucch_prach_ptr->elem[i].prach_ChanCodes_list_LCR.n;
    /* PRACH channelisation code list. See 25.331 v9.1.0, 10.3.6.51a */
    for (j = 0; j < e_rucch_info_ptr->prach_fpach_info[i].num_prach_codes; j++)
    {
      /* Spreading factor. (1, 2, 4, 8, 16) for DPCH, (4, 8, 16) for PRACH, (16) for FPACH */
      e_rucch_info_ptr->prach_fpach_info[i].prach_chan_codes[j].sf = 
         tdsTddPrachCode_R4[e_rucch_prach_ptr->elem[i].prach_ChanCodes_list_LCR.elem[j]][0];
      /* Channelisation code (1..16) */
      e_rucch_info_ptr->prach_fpach_info[i].prach_chan_codes[j].chan_code = 
          tdsTddPrachCode_R4[e_rucch_prach_ptr->elem[i].prach_ChanCodes_list_LCR.elem[j]][1];

      TDSRRC_MSG1(MSG_LEGACY_LOW,"E-rucch PRACH sf is %d",e_rucch_info_ptr->prach_fpach_info[i].prach_chan_codes[j].sf);
      TDSRRC_MSG1(MSG_LEGACY_LOW,"E-rucch PRACH chan_code is %d",e_rucch_info_ptr->prach_fpach_info[i].prach_chan_codes[j].chan_code);
    }
    /* PRACH midamble shift and burst. See 25.331 v9.1.0, 10.3.6.41 */
    switch (e_rucch_prach_ptr->elem[i].midambleShiftAndBurstType.midambleAllocationMode.t)
    {
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_defaultMidamble:
        /*Midamble allocation mode. (10.3.6.41)*/
        e_rucch_info_ptr->prach_fpach_info[i].midamble_and_burst.allocation_mode 
             = TDSL1_DEFAULT_MIDAMBLE;
        break;
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_commonMidamble:
        e_rucch_info_ptr->prach_fpach_info[i].midamble_and_burst.allocation_mode 
             = TDSL1_COMMON_MIDAMBLE;
        break;
      case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble:
        e_rucch_info_ptr->prach_fpach_info[i].midamble_and_burst.allocation_mode 
             = TDSL1_UE_SPECIFIC_MIDAMBLE;
        /*Integer (0..15).*/
        e_rucch_info_ptr->prach_fpach_info[i].midamble_and_burst.shift 
             = e_rucch_prach_ptr->elem[i].midambleShiftAndBurstType
                            .midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error midambleAllocationMode is included");
        return FAILURE;
    }
    /*Midamble configuration, as defined in TS 25.221. Actual value midambleConfiguration = IE value * 2*/
    e_rucch_info_ptr->prach_fpach_info[i].midamble_and_burst.config = 
          tdsMidambleConfiguration[e_rucch_prach_ptr->elem[i]
          .midambleShiftAndBurstType.midambleConfiguration];

    if (e_rucch_prach_ptr->elem[i].m.fpach_InfoPresent)
    {
      /* FPACH info. See 25.331 v9.1.0, 10.3.6.35a */
      /* FPACH timeslot number (0..6) */
      e_rucch_info_ptr->prach_fpach_info[i].fpach_info.fpach_ts_num = 
           e_rucch_prach_ptr->elem[i].fpach_Info.timeslot;

      /* FPACH channelisation code */
      /* Spreading factor. (1, 2, 4, 8, 16) for DPCH, (4, 8, 16) for PRACH, (16) for FPACH */
      e_rucch_info_ptr->prach_fpach_info[i].fpach_info.fpach_chan_code.sf = TDSL1_SF_16;
      /* Channelisation code (1..16), in msg is (0...15) */
      e_rucch_info_ptr->prach_fpach_info[i].fpach_info.fpach_chan_code.chan_code = 
                (uint8)(e_rucch_prach_ptr->elem[i].fpach_Info.channelisationCode + 1);

      TDSRRC_MSG1(MSG_LEGACY_LOW,"E-rucch FPACH chan_code is %d",e_rucch_info_ptr->prach_fpach_info[i].fpach_info.fpach_chan_code.chan_code);
      /* FPACH midamble shift and burst. See 25.331 v9.1.0, 10.3.6.41 */
      switch (e_rucch_prach_ptr->elem[i].fpach_Info.midambleShiftAndBurstType.midambleAllocationMode.t)
      {
        case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_defaultMidamble:
          /*Midamble allocation mode. (10.3.6.41)*/
          e_rucch_info_ptr->prach_fpach_info[i].fpach_info.midamble_and_burst.allocation_mode 
               = TDSL1_DEFAULT_MIDAMBLE;
          break;
        case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_commonMidamble:
          e_rucch_info_ptr->prach_fpach_info[i].fpach_info.midamble_and_burst.allocation_mode 
               = TDSL1_COMMON_MIDAMBLE;
          break;
        case T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble:
          e_rucch_info_ptr->prach_fpach_info[i].fpach_info.midamble_and_burst.allocation_mode 
               = TDSL1_UE_SPECIFIC_MIDAMBLE;
          /*Integer (0..15).*/
          e_rucch_info_ptr->prach_fpach_info[i].fpach_info.midamble_and_burst.shift 
               = e_rucch_prach_ptr->elem[i].fpach_Info.midambleShiftAndBurstType
                              .midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error midambleAllocationMode is included");
          return FAILURE;
      }

      e_rucch_info_ptr->prach_fpach_info[i].fpach_info.midamble_and_burst.config = 
           tdsMidambleConfiguration[e_rucch_prach_ptr->elem[i].fpach_Info.
                     midambleShiftAndBurstType.midambleConfiguration];

      /* WAIT TIME. The number of sub-frames to wait for FPACH ACK, following
       * the sub-frame in which the SYNC_UL is transmitted. (1..4) */
      e_rucch_info_ptr->prach_fpach_info[i].fpach_info.wt = e_rucch_prach_ptr->elem[i].fpach_Info.wi;
    }
    else
    {
      
      /* 25.331-8.5.28 */            
      /* If UE goes from  CELL_DCH(SCHED UPA does not start)->CELL_DCH,
                   If UE goes from  CELL_FACH->CELL_DCH,
                   cell changed but OTA configuration without some optional IEs, RRC will reject it. */
      if ((tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH) &&
          (!(tdscurrent_config_ptr->hsupa_start_mask & TDSRRC_SCHED_START))&&
            (tdsordered_config_ptr->is_hho))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DCH->DCH,important IE absent in E-RUCCH");
        return FAILURE;
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"enter CELL_DCH & no HHO, fpach_Info absent,from PARCH");
      /* UE goes from  FACH->DCH and also sets up EUL in the same config, then 
            if “E-RUCCH info” is not provided by the N\W , then RRC should take the 
            optional IEs who are "OFF" from the PRACH config.   */
      /* IE "config" invalid value is zero */
      if (e_rucch_info_ptr->prach_fpach_info[i].fpach_info.midamble_and_burst.config == 0)
      {
        memscpy(&e_rucch_info_ptr->prach_fpach_info[i].fpach_info, sizeof(tdsl1_ul_fpach_info_struct_type),
             &tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.prach_fpach_info[i].fpach_info,
             sizeof(tdsl1_ul_fpach_info_struct_type) );
       }
    }
  }
  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_e_rucch_ul_syn_update()

DESCRIPTION:
  This function updates the E-RUCCH UL sync info to OC.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_rucch_ul_syn_update
(
  tdsrrc_SYNC_UL_InfoForE_RUCCH  *e_rucch_syn_ptr
)
{
  if (e_rucch_syn_ptr == NULL)
  {
    ERR_FATAL("Null pointer received",0,0,0);
    return FAILURE;
  }
  
  if (e_rucch_syn_ptr->m.prxUpPCHdesPresent)
  {
      /* (-120..-58) in dbm. See 25.331: 10.3.6.52 and 10.3.6.78a */ 
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch. 
                   prx_upPch_des = (int8)(e_rucch_syn_ptr->prxUpPCHdes - 120); 
  }
  else
  { 
    if (tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present == FALSE)
    {
      /* This IE is mandatory present if the IE "Synchronisation parameters does not exist,Otherwise it is not needed.*/
      TDSRRC_MSG0(MSG_LEGACY_ERROR," PRXUpPCHdes not match CV condition ");
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"E-rucch prxUpPCHdes absent,so use dpch prxUpPCHdes");
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.prx_upPch_des =  
            tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des; 
    }
  }

  if (e_rucch_syn_ptr->m.powerRampStepPresent)
  {
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.
                  sync_ul_code_info_erucch.pwr_ramp_step = e_rucch_syn_ptr->powerRampStep;
  }
  else
  {
    if (tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present == FALSE)
    {
      /* This IE is mandatory present if the IE "Synchronisation parameters" does not exist,Otherwise it is not needed.*/
      TDSRRC_MSG0(MSG_LEGACY_ERROR," Power Ramp Step not match CV condition ");
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"E-rucch powerRampStep absent,so use dpch powerRampStep");
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.pwr_ramp_step = 
                tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step;
    }
  }

  if (e_rucch_syn_ptr->m.max_SYNC_UL_TransmissionsPresent)
  {
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.
                max_sync_ul_tx = tdsMaxSynUlTrans[e_rucch_syn_ptr->max_SYNC_UL_Transmissions];
  }
  else
  {
    if (tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present == FALSE)
    {
      /* This IE is mandatory present if the IE "Synchronisation parameters" does not exist,Otherwise it is not needed.*/
      TDSRRC_MSG0(MSG_LEGACY_ERROR," Max SYNC_UL Transmissions not match CV condition ");
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"E-rucch max_SYNC_UL_Trans absent,so use dpch's value");
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.max_sync_ul_tx = 
                tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx;
    }
  }

  if (TDSRRC_MAX_NUMBITS_8 == e_rucch_syn_ptr->e_RUCCH_Sync_UL_Codes_Bitmap.numbits)
  {
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.
              ul_sync_code_mask = e_rucch_syn_ptr->e_RUCCH_Sync_UL_Codes_Bitmap.data[0];
    TDSRRC_MSG1(MSG_LEGACY_LOW,"UL sync codes are available for e-rucch info %d is %d",e_rucch_syn_ptr->e_RUCCH_Sync_UL_Codes_Bitmap.data[0]);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error e_RUCCH_Sync_UL_Codes_Bitmap config.");
    return FAILURE;
  }

  /* Maximum number of synchronisation attempts 10.3.6.78a (1...32)*/
  if((e_rucch_syn_ptr->mmax >= TDSRRCLLC_MIN_MMAX) &&
     (e_rucch_syn_ptr->mmax <= TDSRRCLLC_MAX_MMAX)
    )
  {
    tdsordered_config_ptr->mac_e_config.e_rucch_info.mmax = e_rucch_syn_ptr->mmax;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Max # of Preamble Cyles %d is out of range",e_rucch_syn_ptr->mmax);
    return FAILURE;;
  }
  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_erucch_pval()

DESCRIPTION:
  This function caulate Persistence values for all ASCs

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_get_erucch_pval(void)
{

  uint8   asc_idx;
  uint8   num_of_asc = 0;
  uint8   dpl = 1;
  uint32  pval = 0;
  /* 25.331-8.5.12, get dynamic persistence level N 
     The dynamic persistence value N shall be used only in the case that 
     1.the UEs with E-DCH and E-RUCCH configured on the Primary Frequency of the multi-frequency cell, 
     2.the E-DCH serving cell unchanged 
     3.and the UEs can obtain the dynamic persistence level N from SIB7 according 
        to the configuration of information element "E-RUCCH info",
      Otherwise, dynamic persistence level N=1 shall always be used.*/
      
  /* QC support HSUPA only in CELL_DCH, so can't get SIB7 according to the 
         configuration of information element "E-RUCCH info" 
         and dynamic persistence level N=1 shall always be used at present.
         QC can modify dpl value in future. */

  /* Setup the Persistence values for all ASCs */
  num_of_asc = tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.num_asc;
  for(asc_idx = 0; ((asc_idx < num_of_asc)&& (asc_idx < MAX_ASC)); asc_idx++)
  {
    switch(asc_idx)
    {
      case 0:
        pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR;
        break;
      case 1:
        pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>
                (dpl - 1);
        break;
      default:
        if((asc_idx - 2) >=0)
        {
          pval = ((TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>
                  (dpl - 1)) *
                 tdsordered_config_ptr->erucch_psf[asc_idx - 2])/
                  TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
;
        }
        break;
    }
    /* Note that since there is a multiplication by the Persistence Scaling
     Factor in the above calculation, the actual multiplier for the
     Persistence value is now TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR * 10.
     Now we need to make the value fit into 16 bits, so we divide it
     by ((TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR * 10) / 0xffff) + 1 before
     sending it on to MAC.  (The 1 is added to make sure we round up
     instead of truncating.) */
    tdsordered_config_ptr->mac_e_config.e_rucch_info.nASC = num_of_asc;
    
    {
      tdsordered_config_ptr->mac_e_config.e_rucch_info.asc[asc_idx].pval = pval ;
    }
  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_e_rucch_psf_update()

DESCRIPTION:
  This function updates the E-RUCCH PersistenceScalingFactorList to OC.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_rucch_psf_update
(
  tdsrrc_PersistenceScalingFactorList  *e_rucch_psf_ptr
)
{
  /* local variables */
  uint32 index = 0, n_index = 0;

  if (e_rucch_psf_ptr == NULL)
  {
    ERR_FATAL("Null pointer received",0,0,0);
    return FAILURE;
  }  

  n_index = 0;
  while((e_rucch_psf_ptr->n > n_index) && (index < MAX_ASC_PERSIST))
  {
    /*
  * All the Persistence scaling factors a real numbers in the range
  * from 0.2 to 0.9. The stored values are stored in the ordered config
  * data base according to the following equation:
  *
  * tdsordered_config erucch psf values = (rcv'd values * 10).
  *
  */
    /* Since this is sent as an enumerated type, time to do the mapping */
    switch(e_rucch_psf_ptr->elem[n_index])
    {
      case tdsrrc_PersistenceScalingFactor_psf0_2:
        tdsordered_config_ptr->erucch_psf[index] = 2;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_3:
        tdsordered_config_ptr->erucch_psf[index] = 3;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_4:
        tdsordered_config_ptr->erucch_psf[index] = 4;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_5:
        tdsordered_config_ptr->erucch_psf[index] = 5;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_6:
        tdsordered_config_ptr->erucch_psf[index] = 6;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_7:
        tdsordered_config_ptr->erucch_psf[index] = 7;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_8:
        tdsordered_config_ptr->erucch_psf[index] = 8;
        break;
      case tdsrrc_PersistenceScalingFactor_psf0_9:
        tdsordered_config_ptr->erucch_psf[index] = 9;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Persistance Scaling Factor enum %d",e_rucch_psf_ptr->elem[n_index]);
        return FAILURE;
    } /* end switch */

    /*
  * Note that the first 2 values are calculated outside of this loop, since they
  * are known values.
  */
    /* get the next element */
    n_index++;
    index++;
  } /* end while MAX_ASC_PERSIST */

  /* Set the default value for the rest of ASCs */
  if(index < MAX_ASC_PERSIST)
  {
    do
    {
      tdsordered_config_ptr->erucch_psf[index] = TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL;
      index++;
    }while(index < MAX_ASC_PERSIST);
  }
  return SUCCESS;
} /* end tdsrrcllcpcie_get_persist_scaling_factors() */


/*============================================================================
FUNCTION: tdsrrcllcpcie_e_rucch_asc_update()

DESCRIPTION:
  This function updates the E-RUCCH AccessServiceClass to be sent to MAC and L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_rucch_asc_update
(
  uint32         e_rucch_asc_present,
  tdsrrc_E_RUCCH_AccessServiceClass  *e_rucch_asc_ptr
)
{
  uint8   i = 0;
  if (e_rucch_asc_present == TRUE)
  {
    if (e_rucch_asc_ptr == NULL)
    {
      ERR_FATAL("e_rucch_asc_ptr is a null point ",0,0,0);
      return FAILURE;
    }
    
    tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.num_asc = e_rucch_asc_ptr->n;

    /* 3GPP-25.331-10.3.6.6 */
    for (i = 0; i < e_rucch_asc_ptr->n; i++)
    {
      if (e_rucch_asc_ptr->elem[i].m.accessServiceClass_TDD_LCRPresent)
      {
        if (e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.m.availableSYNC_UlCodesIndicsPresent)
        {
          /* Indicate what UL sync codes are available */
          if (TDSRRC_MAX_NUMBITS_8 == 
                e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.availableSYNC_UlCodesIndics.numbits)
          {
            tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sync_code_mask = 
                 e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.availableSYNC_UlCodesIndics.data[0];
            if (tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sync_code_mask == 0)
            {
              /*If the primary frequency is not the same as working frequency 
                , change the all zero erucch sync ul code to 0~7 all 1, which means all sync ul code available.
                If the primary frequency is the same as working frequency, change the all zero erucch sync ul code to 4~7 all 1.
              */
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"Change all zero sync ul code to other value based on primary freq %d and working freq %d on state %d", tdsordered_config_ptr->p_freq, tdsordered_config_ptr->w_freq, tdsordered_config_ptr->rrc_state_indicator);
              if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
              {
                if (tdsordered_config_ptr->p_freq != tdsordered_config_ptr->w_freq)
                {
                  tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sync_code_mask  = 0xFF;
                }
                else
                {
                  tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sync_code_mask  = 0xF0;
                }
              }
              
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"UlCodesIndics numbits error.");
            return FAILURE;
          }
        }
        else
        {
          /* Default is that all SYNC_UL codes defined in SYNC_UL Info are available. */
          tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sync_code_mask = 0xFF;
        }

        /* The number of sub-channels (1, 2, 4, 8) */
        switch (e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.t)
        {
          case T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size1:
            tdsordered_config_ptr->erucch_asc_info[i].sub_chan_size =
                  TDSRRC_ASC_SUB_CHAN_SIZE_1;

            /* Default value of the IE is that all subchannels within the size are available for the ASC. MD bit = 11111111 */
            /* tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.
                asc_cfg.asc_info_ptr[i]->avail_ul_sub_chan_mask = 0xFF; */
            /* In this case, since there is only one RACH, the value is 1, if RRC has to set it. */
            tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 1;

            break;
          case T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size2:
            tdsordered_config_ptr->erucch_asc_info[i].sub_chan_size = 
                  TDSRRC_ASC_SUB_CHAN_SIZE_2;
            /* Indicate what sub-channels are available */
            if (e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.u.size2->m.subchannelsPresent)
            {
              if (tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannels_subch0 == 
                   e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.u.size2->subchannels)
              {
                /* position 0 is valid */
                tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 1;
              }
              else
              {
                /* position 1 is valid */
                tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 2;
              }
            }
            else
            {
              /* Default value of the IE is that all subchannels within the size are available for the ASC. MD bit = 11 */
              tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 3;
            }
            break;
          case T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size4:
            tdsordered_config_ptr->erucch_asc_info[i].sub_chan_size = 
                   TDSRRC_ASC_SUB_CHAN_SIZE_4;
            /* Indicate what sub-channels are available */
            if (e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.u.size4->m.subchannelsPresent)
            {
              if (TDSRRC_MAX_NUMBITS_4 == 
                 e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.u.size4->subchannels.numbits)
              {
                tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 
                     e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.u.size4->subchannels.data[0]>>TDSRRC_MAX_NUMBITS_4;
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"SubchannelSize size4 numbits error in erucch");
                return FAILURE;
              }
            }
            else
            {
              /* Default value of the IE is that all subchannels within the size are available for the ASC. MD bit = 1111 */
              tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 0xF;
            }
            break;
          case T_tdsrrc_AccessServiceClass_TDD_LCR_r4_subchannelSize_size8:
            tdsordered_config_ptr->erucch_asc_info[i].sub_chan_size = 
                     TDSRRC_ASC_SUB_CHAN_SIZE_8;
            if (e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.u.size8->m.subchannelsPresent)
            {
              if (TDSRRC_MAX_NUMBITS_8 == 
                    e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.u.size8->subchannels.numbits)
              {
                tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 
                   e_rucch_asc_ptr->elem[i].accessServiceClass_TDD_LCR.subchannelSize.u.size8->subchannels.data[0];
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"subchannelSize size8 config error in erucch.");
                return FAILURE;
              }           
            }
            else
            {
              /* Default value of the IE is that all subchannels within the size 
                          are available for the ASC. MD bit = 11111111 */
              tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 0xFF;
            }
            break;
          default:
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid subchannelSize type config error.");
            return FAILURE;
        }
      }
      else
      {
        /* 3GPP-25.331-10.3.6.103 */
        if (i == 0)
        {
          /* If the "default" is used for the first ASC, the default values 
                   are "all available channelisation codes" and "all available
                   subchannels" with "subchannel size=Size 1" in TDD.*/
          tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sync_code_mask = 0xFF;
          tdsordered_config_ptr->erucch_asc_info[i].sub_chan_size = TDSRRC_ASC_SUB_CHAN_SIZE_1;
          /* In this case, since there is only one RACH, the value is 1, if RRC has to set it. */
          tdsordered_config_ptr->erucch_asc_info[i].avail_ul_sub_chan_mask = 1;
        }
        else
        {
          /* The default values are same as the PRACH ASC. */
          memscpy(&tdsordered_config_ptr->erucch_asc_info[i],sizeof(tdsl1_ul_asc_struct_type),
                 &tdsordered_config_ptr->asc_info[i],
                  sizeof(tdsl1_ul_asc_struct_type));
        }
      }
    }    
    
    for(i = 0; i < tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.num_asc; i++)
    { 
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.asc_info_ptr[i] =
        &tdsordered_config_ptr->erucch_asc_info[i];
    } /* end ASC initialization */
  }
  else
  {
    if (tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.num_asc == 0)
    {
      /* UE goes from  FACH->DCH and also sets up EUL in the same config, then 
            if “E-RUCCH info” is not provided by the N\W , then RRC should take the 
            optional IEs who are "OFF" from the PRACH config.   */
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.num_asc = 
          tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.asc_cfg.num_asc;
     
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"FACH->DCH, asc info absent,get from PARCH");
      for (i = 0; i < tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.num_asc; i++)
      {
        /* The default values are same as the PRACH ASC. */
        tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.asc_info_ptr[i] =
               tdscurrent_config_ptr->l1_ul_chan_parms.prach_cfg.asc_cfg.asc_info_ptr[i];
      }
    }
  }  

  return SUCCESS;
}

#endif /*FEATURE_TDSCDMA_HSUPA*/

#ifdef FEATURE_TDSCDMA_MIMO

/*============================================================================
FUNCTION: tdsrrcllcpcie_update_n_m_ratio()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_update_n_m_ratio
(
  tdsrrc_MIMO_N_M_Ratio ota_n_m_ratio
)
{
  switch(ota_n_m_ratio)
  {
    case tdsrrc_MIMO_N_M_Ratio_mnm12:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_1_2;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm23:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_2_3;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm34:
         tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_3_4;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm45:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_4_5;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm56:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_5_6;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm67:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_6_7;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm78:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_7_8;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm89:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_8_9;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm910:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_9_10;
        break;
    case tdsrrc_MIMO_N_M_Ratio_mnm11:
        tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_1_1;
        break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MIMO: Invalid mimo_n_m_ratio in OTA %d",ota_n_m_ratio);
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_INVALID;
      return FAILURE;

  }
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO: MIMO N/M ratio %d ",tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio);
 return SUCCESS;
}

#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
/*============================================================================
FUNCTION: tdsrrcllcpcie_update_mimo_scpich_po()

DESCRIPTION:
  This function updates IE "s-cpich-PowerOffset-Mimo" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_update_mimo_scpich_po
(
  tdsrrc_MIMO_Parameters_v7f0ext * mimo_params_ptr
)
{
  if(RRC_CHECK_MSG_TYPE_IE_PTR(mimo_params_ptr, s_cpich_PowerOffset_MimoPresent))
  {
    tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = mimo_params_ptr->s_cpich_PowerOffset_Mimo;
    TDSRRC_MSG1(MSG_LEGACY_LOW,"MIMO:S-CPICH PO from OTA = %d",mimo_params_ptr->s_cpich_PowerOffset_Mimo);
  }
}
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
/*============================================================================
FUNCTION: tdsrrcllcpcie_update_mimo_codebook_restriction()

DESCRIPTION:
  This function updates IE "precodingWeightSetRestriction" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_update_mimo_codebook_restriction
(
  boolean val
)
{
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_codebook_restriction_present = val;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO:Codebook Restriction = %d",val);  
}
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/


/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params()

DESCRIPTION:
  This function updates IE "Mimo Params" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type    tdsrrcllcpcie_process_mimo_params
(
  tdsrrc_MIMO_Parameters_r7 * mimo_params,
  tdsrrc_RRC_StateIndicator rrc_state,
  uint32 mimo_params_present_ota
)
{
  tdshsdpa_action_enum_type hs_action =tdsrrc_get_hsdpa_action() ;
  boolean mimo_reconfig = FALSE;
  if((TDSCHECK_WCDMA_OPTIONAL_FEATURE_MIMO_SUPPORTED ) && 
      ((hs_action == TDSHSDPA_START) ||
        ((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) && (hs_action != TDSHSDPA_STOP)) )
    )
  {
    tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_NOOP;
    if( mimo_params_present_ota )
    {
       
      if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(mimo_params, 
                 mimoN_M_RatioPresent))
      {
        if(tdsrrcllcpcie_update_n_m_ratio(mimo_params->mimoN_M_Ratio) == FAILURE)
        {
          return FAILURE;
        }
        mimo_reconfig = TRUE;
      }
    
      if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(mimo_params, 
                mimoPilotConfigurationPresent))
      {
        mimo_reconfig = TRUE;
        if(TDSRRC_CHECK_MSG_TYPE(mimo_params->mimoPilotConfiguration.secondCPICH_Pattern, 
                   T_tdsrrc_MIMO_PilotConfiguration_secondCPICH_Pattern_normalPattern))
        {
           tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern = TDSL1_MIMO_NORMAL_PATTERN;
           TDSRRC_MSG0(MSG_LEGACY_LOW,"MIMO: MIMO normal cipch pattern ");
        }
        else if(TDSRRC_CHECK_MSG_TYPE(mimo_params->mimoPilotConfiguration.secondCPICH_Pattern, 
                  T_tdsrrc_MIMO_PilotConfiguration_secondCPICH_Pattern_diversityPattern))
        {
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern = TDSL1_MIMO_DIVERSITY_PATTERN;
   
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.channelisation_code = 
                  mimo_params->mimoPilotConfiguration.secondCPICH_Pattern.u.diversityPattern->channelisationCode;
          TDSRRC_MSG1(MSG_LEGACY_LOW,"MIMO: MIMO diversity cipch pattern chan code %d",tdsordered_config_ptr->l1_hsdpa_info.mimo_info.channelisation_code);
        }
      }
       #ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;  
      #endif
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:MIMO params not present in OTA");
    }  
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"MIMO:MIMO cannot be started HS Action %d HS Status %d nv category %d",
            hs_action,tdsrrc_get_hsdpa_status(),tdsnv_hsdpa_category);
    mimo_params_present_ota = 0;
  }
   
  if((!mimo_params_present_ota) ||
    (tdsrrcllc_check_if_mimo_can_be_started(rrc_state) == FALSE )
     )
  {
    if(tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_ACTIVE )
    {
        
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:Set MIMO Action to STOP");
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_STOP;
      if(hs_action != TDSHSDPA_STOP)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Include HS bitmask as MIMO is going be stopped,but HS continues");

        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL |TDSL1_CPHY_HS_DSCH_CFG_INCL|TDSL1_CPHY_HS_DPCCH_CFG_INCL;;

          /* Set beta table and hs-dpcch bit mask */
        tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.info_present_bitmask = 
            HS_DPCCH_BETA_HS_INFO_PRESENT_BITMASK | HS_DPCCH_MEAS_FEEDBACK_INFO_PRESENT_BITMASK;
        if(hs_action == TDSHSDPA_NOOP)
        {
          tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
    #ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
          tdsrrcllc_set_hsdpa_action_in_machs_macehs();
    #endif
        }
           
      }
      else
      {
        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:Set MIMO Action to NOOP");
      tdsrrcllc_init_mimo_params(tdsordered_config_ptr);
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_NOOP;
    }
  }
  else
  {
    uint8 num_harq_proc =  tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc;
    /*If the IE “Number of processes" is not set to one of the values 12, 14 or 16 and the IE “MIMO
        operation(10.3.6.41a) is included and set to either “startor “continue the UE behaviour is
        unspecified.
      */
    if((num_harq_proc == 12) || (num_harq_proc == 14) || (num_harq_proc == 16))
    {
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO: No. Of harq process is %d as according to MIMO ",num_harq_proc);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MIMO: No. Of harq process is %d is not suitable for MIMO ",num_harq_proc);
      return FAILURE;
    }
    
    if(mimo_params->mimoOperation == tdsrrc_MIMO_Operation_start )
    {
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_START;      
      
      tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL|TDSL1_CPHY_HS_DSCH_CFG_INCL|TDSL1_CPHY_HS_DPCCH_CFG_INCL;

      /* Set beta table and hs-dpcch bit mask */
      tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.info_present_bitmask = 
            HS_DPCCH_BETA_HS_INFO_PRESENT_BITMASK | HS_DPCCH_MEAS_FEEDBACK_INFO_PRESENT_BITMASK;
      
      if(hs_action != TDSHSDPA_START)
      {
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
        tdsrrcllc_set_hsdpa_action_in_machs_macehs();
#endif
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO: MIMO params received with action start");
    }
    else if(mimo_params->mimoOperation == tdsrrc_MIMO_Operation_continue_ )
    {
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_NOOP;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO: MIMO params received with action continue");
      if(mimo_reconfig ==TRUE)
      {
        if(tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_INACTIVE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR," MIMO not active so MIMO Operation should be rrc_start,but received as rrc_continue  ");
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_START;
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_RECFG;
        }
        
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"MIMO: MIMO action reconfig ,second_cpich_pattern  %d chan code %d M/N ratio %d",
             tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern,
             tdsordered_config_ptr->l1_hsdpa_info.mimo_info.channelisation_code,
             tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio);
        
        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL |TDSL1_CPHY_HS_DSCH_CFG_INCL|TDSL1_CPHY_HS_DPCCH_CFG_INCL;;

        /* Set beta table and hs-dpcch bit mask */
        tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.info_present_bitmask = 
            HS_DPCCH_BETA_HS_INFO_PRESENT_BITMASK | HS_DPCCH_MEAS_FEEDBACK_INFO_PRESENT_BITMASK;
          
         if(hs_action != TDSHSDPA_START)
         {
           tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
           tdsrrcllc_set_hsdpa_action_in_machs_macehs();
#endif
         }
      }
      else
      {
        if((tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask & TDSL1_CPHY_HS_DSCH_CFG_INCL)
            &&(tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_ACTIVE))
        {
          /*This is as per L1's requirement that when MIMO is active, even if No MIMO params change
           * if HS-DSCH changes then indicate MIMO as RECONFIG*/
          
          tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL;
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_RECFG;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO: No new MIMO params but HSDSCH touched so MIMO action reconfig HS bitmask 0x%x",tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);
        }
        else if((tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask & TDSL1_CPHY_HS_PDSCH_RL_CFG_INCL) && 
          (tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_ACTIVE))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL |TDSL1_CPHY_HS_DSCH_CFG_INCL;
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_RECFG;        
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:HS Re-pointing so RECFGing MIMO");
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO: No new MIMO params received so action NOOP");
        }
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MIMO: Invalid MIMO Operation %d",mimo_params->mimoOperation);
      return FAILURE;
    }
  }
   
  return SUCCESS;
}

#if defined(FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT) || defined(FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION)
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_asu_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 ASU.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_asu_r7
(
  tdsrrc_ActiveSetUpdate_r7 * active_set_update_r7_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  if((active_set_update_r7_ota != NULL) 
     && (active_set_update_r7_ota->m.v780NonCriticalExtensionsPresent)
     && (active_set_update_r7_ota->v780NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent)
     && (active_set_update_r7_ota->v780NonCriticalExtensions.v7f0NonCriticalExtensions.activeSetUdpate_v7f0ext.m.mimoParametersPresent))
  {
    tdsrrcllcpcie_update_mimo_scpich_po(&active_set_update_r7_ota->v780NonCriticalExtensions.v7f0NonCriticalExtensions.
      activeSetUdpate_v7f0ext.mimoParameters);
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO:Setting S-CPICH PO = %d",tdsrrc_mimo_s_cpich_po);
  }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  if((active_set_update_r7_ota != NULL) &&
    (active_set_update_r7_ota->m.v780NonCriticalExtensionsPresent) &&
    (active_set_update_r7_ota->v780NonCriticalExtensionsm.v7f0NonCriticalExtensionsPresent) && 
    (active_set_update_r7_ota->v780NonCriticalExtensions.v7f0NonCriticalExtensions.m.
    v7g0NonCriticalExtensionsPresent) &&
    (active_set_update_r7_ota->v780NonCriticalExtensions.v7f0NonCriticalExtensions.
    v7g0NonCriticalExtensions.activeSetUdpate_v7g0ext.m.mimoParametersPresent) &&
    (active_set_update_r7_ota->v780NonCriticalExtensions.v7f0NonCriticalExtensions.v7g0NonCriticalExtensions.activeSetUdpate_v7g0ext.
    mimoParameters.m.precodingWeightSetRestrictionPresent))
  {
    tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
  }
  else
  {
    tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
  }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_asu_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 ASU.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_asu_r8
(
  tdsrrc_ActiveSetUpdate_r8 * active_set_update_r8_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  if((active_set_update_r8_ota != NULL)
     && (active_set_update_r8_ota->m.v890NonCriticalExtensionsPresent)
     && (active_set_update_r8_ota->v890NonCriticalExtensions.activeSetUpdate_v890ext.m.mimoParametersPresent))
  {
    tdsrrcllcpcie_update_mimo_scpich_po(&active_set_update_r8_ota->v890NonCriticalExtensions.activeSetUpdate_v890ext.mimoParameters);
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO: Setting S-CPICH PO = %d",tdsrrc_mimo_s_cpich_po);
  }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/


#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  if((active_set_update_r8_ota != NULL) && (active_set_update_r8_ota->m.v890NonCriticalExtensionsPresent) &&
    (active_set_update_r8_ota->v890NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
    (active_set_update_r8_ota->v890NonCriticalExtensions.v7g0NonCriticalExtensions.activeSetUpdate_v7g0ext.m. 
    mimoParametersPresent) &&
    (active_set_update_r8_ota->v890NonCriticalExtensions.v7g0NonCriticalExtensions.activeSetUpdate_v7g0ext.
    mimoParameters.m.precodingWeightSetRestrictionPresent))
  {
    tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
  }
  else
  {
    tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
  }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

}
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_cu_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 CU.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_cu_r7
(
  uint32 dl_sdu_num,
  void * cell_update_r7_msg_ptr
)
{
  tdsrrc_DL_CCCH_Message *ccch_ptr= NULL;
  tdsrrc_DL_DCCH_Message *dcch_ptr= NULL;  

#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if(tdsrrc_DL_CCCH_Message_PDU == dl_sdu_num)
  {
    ccch_ptr=  (tdsrrc_DL_CCCH_Message *)cell_update_r7_msg_ptr;

    if((ccch_ptr != NULL) && (ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.r7.m.cellUpdateConfirm_r7_add_extPresent) && 
      (ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
      u.r7.cellUpdateConfirm_r7_add_ext.m.v7f0NonCriticalExtensionsPresent)) 
    {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
      if(ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.r7.cellUpdateConfirm_r7_add_ext.v7f0NonCriticalExtensions.cellUpdateConfirm_v7f0ext.m.mimoParametersPresent)
      {
        tdsrrcllcpcie_update_mimo_scpich_po(&ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.cellUpdateConfirm_r7_add_ext.
          v7f0NonCriticalExtensions.cellUpdateConfirm_v7f0ext.mimoParameters);      
      }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
      if((ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.r7.cellUpdateConfirm_r7_add_ext.v7f0NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) && 
        (ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.r7.cellUpdateConfirm_r7_add_ext.v7f0NonCriticalExtensions.v7g0NonCriticalExtensions.cellUpdateConfirm_v7g0ext.m.
        mimoParametersPresent) && (ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.cellUpdateConfirm_r7_add_ext.v7f0NonCriticalExtensions.v7g0NonCriticalExtensions.
        cellUpdateConfirm_v7g0ext.mimoParameters.m.precodingWeightSetRestrictionPresent))
      {
        tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);      
      }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/
    }
  }
  {
    dcch_ptr = (tdsrrc_DL_DCCH_Message *)cell_update_r7_msg_ptr;

    if((dcch_ptr != NULL) && (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.r7.m.cellUpdateConfirm_r7_add_extPresent) && 
      (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
      u.r7.cellUpdateConfirm_r7_add_ext.m.v7f0NonCriticalExtensionsPresent))
    {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
      if(dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.r7.cellUpdateConfirm_r7_add_ext.v7f0NonCriticalExtensions.cellUpdateConfirm_v7f0ext.m.mimoParametersPresent)
      {
        tdsrrcllcpcie_update_mimo_scpich_po(&dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.cellUpdateConfirm_r7_add_ext.
          v7f0NonCriticalExtensions.cellUpdateConfirm_v7f0ext.mimoParameters);
      }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
      if((dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.r7.cellUpdateConfirm_r7_add_ext.v7f0NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) && 
        (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.r7.cellUpdateConfirm_r7_add_ext.
        v7f0NonCriticalExtensions.v7g0NonCriticalExtensions.cellUpdateConfirm_v7g0ext.m.mimoParametersPresent) 
        && (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.cellUpdateConfirm_r7_add_ext.
        v7f0NonCriticalExtensions.v7g0NonCriticalExtensions.
        cellUpdateConfirm_v7g0ext.mimoParameters.m.precodingWeightSetRestrictionPresent))
      {
        tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);      
      }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/
    }
  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_cu_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 CU.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_cu_r8
(
  uint32 dl_sdu_num,
  void * cell_update_r8_msg_ptr
)
{
  tdsrrc_DL_DCCH_Message *dcch_ptr= NULL;  

#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if(tdsrrc_DL_DCCH_Message_PDU == dl_sdu_num)
  {
    dcch_ptr = (tdsrrc_DL_DCCH_Message *)cell_update_r8_msg_ptr;
    if((dcch_ptr != NULL) && (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.m.v7d0NonCriticalExtensionsPresent) &&
      (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
    {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT    
      if(dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
        v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.cellUpdateConfirm_v7f0ext.m.mimoParametersPresent)
      {
        tdsrrcllcpcie_update_mimo_scpich_po(&dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
            criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.cellUpdateConfirm_v7f0ext.mimoParameters);
      }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
      if((dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
        v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.m.v890NonCriticalExtensionsPresent) &&
        (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.
        u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
        v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
        v890NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
        (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
        v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.v7g0NonCriticalExtensions.
        cellUpdateConfirm_v7g0ext.m.mimoParametersPresent) &&
        (dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
        v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.
        v7g0NonCriticalExtensions.cellUpdateConfirm_v7g0ext.mimoParameters.m.
        precodingWeightSetRestrictionPresent))
      {
        tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);      
      }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/  
    }
  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_tcrc_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 TCRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_tcrc_r7
(
  tdsrrc_TransportChannelReconfiguration_r7 * trch_reconfig_r7_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((trch_reconfig_r7_ota != NULL) && (trch_reconfig_r7_ota->m.v780NonCriticalExtensionsPresent) &&
    (trch_reconfig_r7_ota->v780NonCriticalExtensions.m.v7d0NonCriticalExtensionsPresent) &&
    (trch_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
    if(trch_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.transportChannelReconfiguration_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&trch_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
        transportChannelReconfiguration_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/  

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((trch_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.
    v7f0NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
    (trch_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.
    v7f0NonCriticalExtensions.v7g0NonCriticalExtensions.
    transportChannelReconfiguration_v7g0ext.m.mimoParametersPresent) &&
    (trch_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
    v7g0NonCriticalExtensions.transportChannelReconfiguration_v7g0ext.
    mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/
  }
}


/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_tcrc_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 TCRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_tcrc_r8
(
  tdsrrc_TransportChannelReconfiguration_r8 * trch_reconfig_r8_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((trch_reconfig_r8_ota != NULL) && (trch_reconfig_r8_ota->m.v7d0NonCriticalExtensionsPresent) &&
    (trch_reconfig_r8_ota->v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT  
    if(trch_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.transportChannelReconfiguration_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&trch_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.transportChannelReconfiguration_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/  

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((trch_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
    m.v890NonCriticalExtensionsPresent) &&
      (trch_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v890NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
      (trch_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v890NonCriticalExtensions.v7g0NonCriticalExtensions.
      transportChannelReconfiguration_v7g0ext.m.mimoParametersPresent ) &&
      (trch_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v890NonCriticalExtensions.v7g0NonCriticalExtensions.transportChannelReconfiguration_v7g0ext.
      mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/
  }
}


/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbrc_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 RBRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbrc_r7
(
  tdsrrc_RadioBearerReconfiguration_r7 * rb_reconfig_r7_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((rb_reconfig_r7_ota != NULL) &&
    (rb_reconfig_r7_ota->m.v780NonCriticalExtensionsPresent) && 
    (rb_reconfig_r7_ota->v780NonCriticalExtensions.m.v790NonCriticalExtensionsPresent) &&
    (rb_reconfig_r7_ota->v780NonCriticalExtensions.v790NonCriticalExtensions.m.v7d0NonCriticalExtensionsPresent) &&
    (rb_reconfig_r7_ota->v780NonCriticalExtensions.v790NonCriticalExtensions.v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent)) 
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT  
    if(rb_reconfig_r7_ota->v780NonCriticalExtensions.v790NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      radioBearerReconfiguration_v7f0ext.m.mimoParametersPresent)

    {
      tdsrrcllcpcie_update_mimo_scpich_po(&rb_reconfig_r7_ota->v780NonCriticalExtensions.v790NonCriticalExtensions.v7d0NonCriticalExtensions.
        v7f0NonCriticalExtensions.radioBearerReconfiguration_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((rb_reconfig_r7_ota->v780NonCriticalExtensions.v790NonCriticalExtensions.
    v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) && 
      (rb_reconfig_r7_ota->v780NonCriticalExtensions.v790NonCriticalExtensions.
      v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerReconfiguration_v7g0ext.m.mimoParametersPresent) &&
      (rb_reconfig_r7_ota->v780NonCriticalExtensions.v790NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerReconfiguration_v7g0ext.
      mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbrc_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 RBRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbrc_r8
(
  tdsrrc_RadioBearerReconfiguration_r8 * rb_reconfig_r8_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((rb_reconfig_r8_ota != NULL) && (rb_reconfig_r8_ota->m.v7d0NonCriticalExtensionsPresent) &&
  (rb_reconfig_r8_ota->v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT  
    if (rb_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.radioBearerReconfiguration_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&rb_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.radioBearerReconfiguration_v7f0ext.mimoParameters);
    } 
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if(( rb_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
    m.v890NonCriticalExtensionsPresent) &&
      (rb_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v890NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
      (rb_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerReconfiguration_v7g0ext.m.mimoParametersPresent) &&
      (rb_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerReconfiguration_v7g0ext.mimoParameters.
      m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);    
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/
  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbr_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 RBR.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbr_r7
(
  tdsrrc_RadioBearerRelease_r7 * rb_release_r7_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((rb_release_r7_ota != NULL) &&  (rb_release_r7_ota->m.v780NonCriticalExtensionsPresent) &&
    (rb_release_r7_ota->v780NonCriticalExtensions.m.v7d0NonCriticalExtensionsPresent) &&
    (rb_release_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT  
    if(rb_release_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.radioBearerRelease_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&rb_release_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.
        v7f0NonCriticalExtensions.radioBearerRelease_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((rb_release_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.
    v7f0NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
      (rb_release_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerRelease_v7g0ext.m.mimoParametersPresent) &&
      (rb_release_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerRelease_v7g0ext.mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbr_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 RBR.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbr_r8
(
  tdsrrc_RadioBearerRelease_r8 * rb_release_r8_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((rb_release_r8_ota != NULL) && (rb_release_r8_ota->m.v7d0NonCriticalExtensionsPresent) &&
    (rb_release_r8_ota->v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT  
    if(rb_release_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.radioBearerRelease_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&rb_release_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.radioBearerRelease_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((rb_release_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
    m.v890NonCriticalExtensionsPresent) &&
      (rb_release_r8_ota->v7d0NonCriticalExtensions.
      v7f0NonCriticalExtensions.v890NonCriticalExtensions.
      m.v7g0NonCriticalExtensionsPresent) &&
      (rb_release_r8_ota->v7d0NonCriticalExtensions.
      v7f0NonCriticalExtensions.v890NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerRelease_v7g0ext.m.mimoParametersPresent) &&
      (rb_release_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v890NonCriticalExtensions.v7g0NonCriticalExtensions.radioBearerRelease_v7g0ext.
      mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbe_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 RBE.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbe_r7
(
  tdsrrc_RadioBearerSetup_r7 * rb_setup_r7_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((rb_setup_r7_ota != NULL) && (rb_setup_r7_ota->m.radioBearerSetup_r7_add_extPresent) &&
  (rb_setup_r7_ota->radioBearerSetup_r7_add_ext.m.v7f0NonCriticalExtensionsPresent))
  {

#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT  
    if(rb_setup_r7_ota->radioBearerSetup_r7_add_ext.v7f0NonCriticalExtensions.radioBearerSetup_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&rb_setup_r7_ota->radioBearerSetup_r7_add_ext.v7f0NonCriticalExtensions.
        radioBearerSetup_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((rb_setup_r7_ota->radioBearerSetup_r7_add_ext.v7f0NonCriticalExtensions.
    m.v7g0NonCriticalExtensionsPresent) &&
      (rb_setup_r7_ota->radioBearerSetup_r7_add_ext.v7f0NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerSetup_v7g0ext.m.mimoParametersPresent) &&
      (rb_setup_r7_ota->radioBearerSetup_r7_add_ext.v7f0NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerSetup_v7g0ext.
      mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);    
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/  
  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbe_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 RBE.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbe_r8
(
  tdsrrc_RadioBearerSetup_r8 * rb_setup_r8_ota
)
{

#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((rb_setup_r8_ota != NULL) && (rb_setup_r8_ota->m.v7d0NonCriticalExtensionsPresent) &&
    (rb_setup_r8_ota->v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT    
    if (rb_setup_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.radioBearerSetup_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&rb_setup_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.radioBearerSetup_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((rb_setup_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.m.v890NonCriticalExtensionsPresent) &&
      (rb_setup_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
      (rb_setup_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerSetup_v7g0ext.m.mimoParametersPresent) &&
      (rb_setup_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.
      v7g0NonCriticalExtensions.radioBearerSetup_v7g0ext.mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_pcrc_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 PCRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_pcrc_r7
(
  tdsrrc_PhysicalChannelReconfiguration_r7 * phys_chan_reconfig_r7_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((phys_chan_reconfig_r7_ota != NULL) && (phys_chan_reconfig_r7_ota->m.v780NonCriticalExtensionsPresent) && 
    (phys_chan_reconfig_r7_ota->v780NonCriticalExtensions.m.v7d0NonCriticalExtensionsPresent) &&
    (phys_chan_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT  
    if(phys_chan_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.physicalChannelReconfiguration_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&phys_chan_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.
        v7f0NonCriticalExtensions.physicalChannelReconfiguration_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((phys_chan_reconfig_r7_ota->v780NonCriticalExtensions.
    v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
      (phys_chan_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v7g0NonCriticalExtensions.physicalChannelReconfiguration_v7g0ext.m.mimoParametersPresent) &&
      (phys_chan_reconfig_r7_ota->v780NonCriticalExtensions.v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
      v7g0NonCriticalExtensions.physicalChannelReconfiguration_v7g0ext.
      mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_pcrc_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 PCRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_pcrc_r8
(
  tdsrrc_PhysicalChannelReconfiguration_r8 * phys_chan_reconfig_r8_ota
)
{
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  tdsordered_config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  tdsrrcllcpcie_update_mimo_codebook_restriction(FALSE);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  if((phys_chan_reconfig_r8_ota != NULL) && (phys_chan_reconfig_r8_ota->m.v7d0NonCriticalExtensionsPresent) &&
    (phys_chan_reconfig_r8_ota->v7d0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent))
  {
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT  
    if(phys_chan_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.physicalChannelReconfiguration_v7f0ext.m.mimoParametersPresent)
    {
      tdsrrcllcpcie_update_mimo_scpich_po(&phys_chan_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
        physicalChannelReconfiguration_v7f0ext.mimoParameters);
    }
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
    if((phys_chan_reconfig_r8_ota->v7d0NonCriticalExtensions.
    v7f0NonCriticalExtensions.m.v890NonCriticalExtensionsPresent) &&
    (phys_chan_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.
    v890NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent) &&
    (phys_chan_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.
    v7g0NonCriticalExtensions.physicalChannelReconfiguration_v7g0ext.m.mimoParametersPresent) &&
    (phys_chan_reconfig_r8_ota->v7d0NonCriticalExtensions.v7f0NonCriticalExtensions.v890NonCriticalExtensions.
    v7g0NonCriticalExtensions.physicalChannelReconfiguration_v7g0ext.mimoParameters.m.precodingWeightSetRestrictionPresent))
    {
      tdsrrcllcpcie_update_mimo_codebook_restriction(TRUE);
    }
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

  }
}

#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT || FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

#endif

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
/*============================================================================
FUNCTION: tdsrrcllcpcie_update_hs_tfri_table_type_non_r7_ota()

DESCRIPTION:
  This function updates the TRFI table to be used for MAC-HS or MAC-EHS for Non-R7 OTA
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_update_hs_tfri_table_type_non_r7_ota
(
  void
)
{  
  tdshsdpa_action_enum_type hs_action =tdsrrc_get_hsdpa_action() ;
  if( (hs_action == TDSHSDPA_START) ||
       ((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) && (hs_action != TDSHSDPA_STOP)) 
   )
  {
    if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Bit aligned TFRI table to be used as MAC-HS configured");
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
               TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;
    }
    else if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_EHS)
    {
#ifdef FEATURE_TDSCDMA_64QAM
      if(tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE )
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:OCTET aligned TFRI table to be used as 64QAM configured");
         tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
                    TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_ALIGNED;
      }
      else
#endif
      if(tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Bit aligned TFRI table to be used as HS Pdsch IE is present");
        tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
               TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS:TFRI table to be untouched as MAC-EHS configure %d",tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type); 
      }
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Reset to Bit aligned TFRI table to be used as HS not active ");
    tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
               TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;
  }
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_update_hs_tfri_table_type()

DESCRIPTION:
  This function updates the TRFI table to be used for MAC-HS or MAC-EHS
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_update_hs_tfri_table_type
(
  boolean hs_tb_size_octet_aligned
)
{
  tdshsdpa_action_enum_type hs_action =tdsrrc_get_hsdpa_action() ;
  if( (hs_action == TDSHSDPA_START) ||
         ((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) && (hs_action != TDSHSDPA_STOP)) 
     )
  { 
    if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS)
    {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Bit aligned TFRI table to be used as MAC-HS configured");
       tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
               TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;
    }
    else if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_EHS)
    {
        /*MAC-EHS*/
#ifdef FEATURE_TDSCDMA_64QAM
      if(tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE )
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:OCTET aligned TFRI table to be used as 64QAM configured");
         tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
                    TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_ALIGNED;
      }
      else
#endif
      {
         if(hs_tb_size_octet_aligned == TRUE)
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:OCTET aligned TFRI table to be used");
             tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
                    TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_ALIGNED;
         }
         else
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Bit aligned TFRI table to be used");
             tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
                    TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;
         }
         if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type != 
            tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type)
         {
            tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_DSCH_CFG_INCL;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Set HS-DSCH bitmask as TFRI table is changed from %d to %d",tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type,tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type);
         }
      }
    }
  }
}
#endif

#ifdef FEATURE_TDSCDMA_CPC_DRX
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_internal_inact_thrshd_grant_monitoring()

DESCRIPTION:
  This function will provide inact_thrshd_grant_monitoring in internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  inact_thrshd_grant_monitoring in internal form 

SIDE EFFECTS:
=============================================================================*/
l1_inact_thrshd_for_ue_grant_monitoring_info_enum_type  
tdsrrcllcpcie_get_internal_inact_thrshd_grant_monitoring
(
  tdsrrc_UE_GrantMonitoring_InactivityThreshold ue_GrantMonitoring_InactivityThreshold
)
{
  l1_inact_thrshd_for_ue_grant_monitoring_info_enum_type inact_thrshd_grant_monitoring = 
  L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_INVALID;

  switch(ue_GrantMonitoring_InactivityThreshold)
  {
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_0:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_0;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_1:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_1;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_2:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_2;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_4:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_4;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_8:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_8;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_16:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_16;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_32:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_32;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_64:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_64;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_128:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_128;
      break;
      
    case tdsrrc_UE_GrantMonitoring_InactivityThreshold_e_dch_tti_256:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_256;
      break;
      
    default:
      inact_thrshd_grant_monitoring = L1_INACT_THRSHD_FOR_GRANT_MONIT_NUM_TTI_INVALID;
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"CPCDRX:Invalid inact_thrshd_grant_monitoring [rx]%d [internal form]%d",ue_GrantMonitoring_InactivityThreshold,inact_thrshd_grant_monitoring);
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CPCDRX:inact_thrshd_grant_monitoring [rx]%d [internal form]%d",ue_GrantMonitoring_InactivityThreshold,inact_thrshd_grant_monitoring);
     
  return (inact_thrshd_grant_monitoring);
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_internal_inact_thrshd_drx_cycle()

DESCRIPTION:
  This function will provide internal_inact_thrshd_drx_cycle in internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  internal_inact_thrshd_drx_cycle in internal form 

SIDE EFFECTS:
=============================================================================*/
l1_inact_thrshd_for_ue_drx_cycle_info_enum_type  
tdsrrcllcpcie_get_internal_inact_thrshd_drx_cycle
(
  tdsrrc_UE_DRX_Cycle_InactivityThreshold ue_drx_Cycle_InactivityThreshold
)
{
  l1_inact_thrshd_for_ue_drx_cycle_info_enum_type inact_thrshd_drx_cycle = 
  L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_INVALID;

  switch(ue_drx_Cycle_InactivityThreshold)
  {
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_0:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_0;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_1:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_1;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_2:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_2;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_4:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_4;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_8:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_8;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_16:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_16;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_32:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_32;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_64:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_64;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_128:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_128;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_256:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_256;
      break;
      
    case tdsrrc_UE_DRX_Cycle_InactivityThreshold_sub_frames_512:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_512;
      break;
      
    default:
      inact_thrshd_drx_cycle = L1_INACT_THRSHD_FOR_UE_DRX_CYCLE_NUM_SUBFM_INVALID;
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"CPCDRX:Invalid inact_thrshd_drx_cycle [rx]%d [internal form]%d",ue_drx_Cycle_InactivityThreshold,inact_thrshd_drx_cycle);
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CPCDRX:inact_thrshd_drx_cycle [rx]%d [internal form]%d",ue_drx_Cycle_InactivityThreshold,inact_thrshd_drx_cycle);
     
  return (inact_thrshd_drx_cycle);
}


/*============================================================================
FUNCTION: tdsrrcllcpcie_get_internal_drx_cycle()

DESCRIPTION:
  This function will provide drx_cycle in internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  drx_cycle in internal form 

SIDE EFFECTS:
=============================================================================*/
l1_ue_drx_cycle_info_enum_type  tdsrrcllcpcie_get_internal_drx_cycle
(
  tdsrrc_UE_DRX_Cycle ue_drx_Cycle_rx
)
{
  l1_ue_drx_cycle_info_enum_type ue_drx_cycle = L1_UE_DRX_CYCLE_NUM_SUBFM_MAX;
  switch(ue_drx_Cycle_rx)
  {
    case tdsrrc_UE_DRX_Cycle_sub_frames_4:
      ue_drx_cycle = L1_UE_DRX_CYCLE_NUM_SUBFM_4;
      break;
      
    case tdsrrc_UE_DRX_Cycle_sub_frames_5:
      ue_drx_cycle = L1_UE_DRX_CYCLE_NUM_SUBFM_5;
      break;
      
    case tdsrrc_UE_DRX_Cycle_sub_frames_8:
      ue_drx_cycle = L1_UE_DRX_CYCLE_NUM_SUBFM_8;
      break;
      
    case tdsrrc_UE_DRX_Cycle_sub_frames_10:
      ue_drx_cycle = L1_UE_DRX_CYCLE_NUM_SUBFM_10;
      break;
      
    case tdsrrc_UE_DRX_Cycle_sub_frames_16:
      ue_drx_cycle = L1_UE_DRX_CYCLE_NUM_SUBFM_16;
      break;
      
    case tdsrrc_UE_DRX_Cycle_sub_frames_20:
      ue_drx_cycle = L1_UE_DRX_CYCLE_NUM_SUBFM_20;
      break;
      
    default:
      ue_drx_cycle = L1_UE_DRX_CYCLE_NUM_SUBFM_MAX;
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"CPCDRX:Invalid ue_drx_cycle [rx]%d [internal form]%d",ue_drx_Cycle_rx,ue_drx_cycle);
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CPCDRX:ue_drx_Cycle [rx]%d [internal form]%d",ue_drx_Cycle_rx,ue_drx_cycle);
     
  return (ue_drx_cycle);
}
#endif

#ifdef FEATURE_TDSCDMA_CPC_DTX
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_dtx_drx_enabling_delay()

DESCRIPTION:
  This function converts dtx-drx enabling delay to internal form.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  internal form of dtx-drx enabling delay

SIDE EFFECTS:
=============================================================================*/
static l1_dtx_drx_enabling_delay_info_enum_type    tdsrrcllcpcie_get_dtx_drx_enabling_delay
(
  tdsrrc_EnablingDelay dtx_drx_enabling_delay
)
{
  l1_dtx_drx_enabling_delay_info_enum_type int_dtx_drx_enabling_delay;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Convert DTX-DRX enabling_delay to internal form");
  switch(dtx_drx_enabling_delay)
  {
    case tdsrrc_EnablingDelay_radio_frames_0:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_0;
      break;
      
    case tdsrrc_EnablingDelay_radio_frames_1:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_1;
      break;
    
    case tdsrrc_EnablingDelay_radio_frames_2:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_2;
      break;
    
    case tdsrrc_EnablingDelay_radio_frames_4:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_4;
      break;
  
    case tdsrrc_EnablingDelay_radio_frames_8:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_8;
      break;

    case tdsrrc_EnablingDelay_radio_frames_16:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_16;
      break;

    case tdsrrc_EnablingDelay_radio_frames_32:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_32;
      break;

    case tdsrrc_EnablingDelay_radio_frames_64:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_64;
      break;

    case tdsrrc_EnablingDelay_radio_frames_128:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_128;
      break;
  
    default:
      int_dtx_drx_enabling_delay = L1_NUM_RADIO_FRAMES_MAX;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid dtx_drx_enabling_delay =%d ",dtx_drx_enabling_delay);
      break;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:DTX-DRX enabling_delay internal form = %d",int_dtx_drx_enabling_delay);
  return int_dtx_drx_enabling_delay;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dtx_drx_timinginfo()

DESCRIPTION:
  This function processes DTX-DRX timing info in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type    tdsrrcllcpcie_process_dtx_drx_timinginfo
(
  tdsrrc_DTX_DRX_TimingInfo_r7 *dtx_drx_timingInfo,
  uint32 dtx_drx_timingInfo_present_ota,
  uint32 dtx_drx_Info_present_ota
)
{
  uecomdef_status_e_type status = SUCCESS;
  if(dtx_drx_timingInfo_present_ota)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Process DTX-DRX timinginfo I.E");
    /*Check to see if the timing chosen is continue or new timing*/
    if((dtx_drx_timingInfo->timing.t ==
      T_tdsrrc_DTX_DRX_TimingInfo_r7_timing_continue_))
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC:DTX-DRX timinginfo continue chosen");
      if(dtx_drx_Info_present_ota)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCCPC:dtx_drx_Info_present_ota,Send Failuer");
        status = FAILURE;
      }
      else if(tdsordered_config_ptr->is_hho && (tdscurrent_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
      {
        /* Firt time into DCH is also considered a HHO.  So only populate the release list of 
         * EDCH in the case of DCH->DCH only
         */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCCPC:HHO case,Timing Info is continue,Send Failuer");
        status = FAILURE;
      }
      else
      {
        tdsrrcllc_update_l1_dtx_req_mask_for_no_op(TRUE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC:DTX-DRX timinginfo Newtiming chosen");
      /*Check if dtx_drx_offset & enabling_delay are in valid range,If not send a failuer*/
      
      if((dtx_drx_timingInfo->timing.u.newTiming->ue_dtx_drx_Offset > TDSUE_MAX_DTX_DRX_OFFSET )||
        (tdsrrcllcpcie_get_dtx_drx_enabling_delay(dtx_drx_timingInfo->timing.u.newTiming->enablingDelay)
          == L1_NUM_RADIO_FRAMES_MAX))
      {
        status = FAILURE;
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC:Invalid values,dtx_drx_offset(In Sub-fm) =%d,enabling_delay(In radio frames) =%d",dtx_drx_timingInfo->timing.u.newTiming->ue_dtx_drx_Offset,tdsrrcllcpcie_get_dtx_drx_enabling_delay(dtx_drx_timingInfo->timing.u.newTiming->enablingDelay));
      }
      else
      {
        tdsordered_config_ptr->dtx_drx_timing_info.dtx_drx_offset =
        dtx_drx_timingInfo->timing.u.newTiming->ue_dtx_drx_Offset;
        
        tdsordered_config_ptr->dtx_drx_timing_info.enabling_delay =
          tdsrrcllcpcie_get_dtx_drx_enabling_delay(dtx_drx_timingInfo->timing.u.newTiming->enablingDelay);
        tdsrrcllc_update_l1_dtx_req_mask_for_reconfig(TRUE);
#ifdef FEATURE_TDSCDMA_CPC_DRX
        tdsrrcllc_update_l1_drx_req_mask_for_reconfig(tdsordered_config_ptr,TRUE);
#endif
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:Rxd value for dtx_drx_offset(In Sub-fm) =%d,enabling_delay(In radio frames) =%d",tdsordered_config_ptr->dtx_drx_timing_info.dtx_drx_offset,tdsordered_config_ptr->dtx_drx_timing_info.enabling_delay);
      }
    }

    if((!dtx_drx_Info_present_ota) && (TRUE == tdsordered_config_ptr->e_dch_transmission) &&
        (tdscurrent_config_ptr->mac_e_config.e_tti != NUM_E_TTI) && 
        (tdsordered_config_ptr ->mac_e_config.e_tti != NUM_E_TTI) && 
        (tdsordered_config_ptr->mac_e_config.e_tti!= tdscurrent_config_ptr ->mac_e_config.e_tti))
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"RRCCPC:dtx_drx_Info_present_ota %d and TTI change from %d to %d, Send Failure",
                 dtx_drx_Info_present_ota,tdscurrent_config_ptr->mac_e_config.e_tti,tdsordered_config_ptr ->mac_e_config.e_tti);
      status = FAILURE;
    }

  }
  return status;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ue_drx_cycle()

DESCRIPTION:
  This function converts UE DRX cycle to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  UE DRX cycle internal form 

SIDE EFFECTS:
=============================================================================*/
static uint8 tdsrrcllcpcie_get_ue_drx_cycle
(
  tdsrrc_UE_DRX_Cycle ue_drx_Cycle
)
{
  uint8 ue_drx_cycle_internal=0;

  switch(ue_drx_Cycle)
  {
    case tdsrrc_UE_DRX_Cycle_sub_frames_4:
      ue_drx_cycle_internal = 4;
      break;

    case tdsrrc_UE_DRX_Cycle_sub_frames_5:
      ue_drx_cycle_internal = 5;
      break;

    case tdsrrc_UE_DRX_Cycle_sub_frames_8:
      ue_drx_cycle_internal = 8;
      break;
      
    case tdsrrc_UE_DRX_Cycle_sub_frames_10:
      ue_drx_cycle_internal = 10;
      break;

    case tdsrrc_UE_DRX_Cycle_sub_frames_16:
      ue_drx_cycle_internal = 16;
      break;

    case tdsrrc_UE_DRX_Cycle_sub_frames_20:
      ue_drx_cycle_internal = 20;
      break;

    default:
      ue_drx_cycle_internal = 21;
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"ue_drx_Cycle=%d,internal form=%d",ue_drx_Cycle,ue_drx_cycle_internal);
  return ue_drx_cycle_internal;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_validate_drx_params()

DESCRIPTION:
  This function converts MAC DRX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_validate_drx_params
(
  tdsrrc_DRX_Info    *drx_Info
)
{
  uecomdef_status_e_type status = SUCCESS;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC: validate drx params");

  /*If the IE "UE DRX cycle" is not an integer multiple or a divisor of the 
  *value of the IE "UE DTX cycle 1"
  */
  if(status == SUCCESS)
  {
  if((((uint8)tdsordered_config_ptr->dtx_info.ue_dtx_cycle1) <
      tdsrrcllcpcie_get_ue_drx_cycle(drx_Info->ue_drx_Cycle)))
  {
    if((tdsrrcllcpcie_get_ue_drx_cycle(drx_Info->ue_drx_Cycle) %
      (uint8)tdsordered_config_ptr->dtx_info.ue_dtx_cycle1) != 0)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: UE drx_cycle(%d) is not an integer multiple of dtx_cycle1(%d) ",tdsrrcllcpcie_get_ue_drx_cycle(drx_Info->ue_drx_Cycle),tdsordered_config_ptr->dtx_info.ue_dtx_cycle1);
      status = FAILURE;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: UE drx_cycle(%d) is an integer multiple of dtx_cycle1(%d) ",tdsrrcllcpcie_get_ue_drx_cycle(drx_Info->ue_drx_Cycle),tdsordered_config_ptr->dtx_info.ue_dtx_cycle1);
    }
  }
  else
  {
    if(((uint8)tdsordered_config_ptr->dtx_info.ue_dtx_cycle1) %
      tdsrrcllcpcie_get_ue_drx_cycle(drx_Info->ue_drx_Cycle) != 0)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: dtx_cycle1(%d) is not an integer multiple of UE drx_cycle(%d) ",tdsordered_config_ptr->dtx_info.ue_dtx_cycle1,tdsrrcllcpcie_get_ue_drx_cycle(drx_Info->ue_drx_Cycle));
      status = FAILURE;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: dtx_cycle1(%d) is an integer multiple of UE drx_cycle(%d) ",tdsordered_config_ptr->dtx_info.ue_dtx_cycle1,tdsrrcllcpcie_get_ue_drx_cycle(drx_Info->ue_drx_Cycle));
    }
  }
  }
  
  return status;
}


/*============================================================================
FUNCTION: tdsrrcllcpcie_process_drx_info()

DESCRIPTION:
  This function processes DRX info in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static void  tdsrrcllcpcie_process_drx_info
(
  tdsrrc_DRX_Info    *drx_Info,
  uint32 drx_Info_present_in_ota
)
{
  if(drx_Info_present_in_ota)
  {
#ifdef FEATURE_TDSCDMA_CPC_DRX
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:RRCCPC:Process DRX info I.E");
    tdsrrcllc_init_drx_info_before_processing(tdsordered_config_ptr);
    
    tdsordered_config_ptr->drx_info.inact_thrshd_drx_cycle = 
      tdsrrcllcpcie_get_internal_inact_thrshd_drx_cycle(
      drx_Info->ue_drx_Cycle_InactivityThreshold);
    
    tdsordered_config_ptr->drx_info.inact_thrshd_grant_monitoring = 
      tdsrrcllcpcie_get_internal_inact_thrshd_grant_monitoring(
      drx_Info->ue_GrantMonitoring_InactivityThreshold);
    
    if(drx_Info->ue_drx_GrantMonitoring == TRUE)
    {
      tdsordered_config_ptr->drx_info.l1_ue_drx_grant_monitoring = TRUE;
    }
    else
    {
      tdsordered_config_ptr->drx_info.l1_ue_drx_grant_monitoring = FALSE;
    }
    
    tdsordered_config_ptr->drx_info.ue_drx_cycle = 
      tdsrrcllcpcie_get_internal_drx_cycle(drx_Info->ue_drx_Cycle);
#endif
  }
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_validate_dtx_params()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_validate_dtx_params
(
  tdsrrc_DTX_Info    *dtx_Info
)
{
  uecomdef_status_e_type status = SUCCESS;
  tdsl1_e_tti_enum_type             e_tti = TDSL1_NUM_E_TTI;
  uint8 ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_MAX;
  uint8 ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_MAX;
  uint8 l1_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_MAX;
  uint8 ue_dpcch_burst_1 = L1_UE_DPCCH_BURST_1_NUM_SUBFM_MAX;
  uint8 ue_dpcch_burst_2 = L1_UE_DPCCH_BURST_2_NUM_SUBFM_MAX;
  uint8 dtx_long_preamble_length = L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_MAX;
  uint16 inact_thrshd_dtx_cycle2 = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_INVALID;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC: validate dtx params");

  if((dtx_Info->e_dch_TTI_Length.t
    == T_tdsrrc_DTX_Info_e_dch_TTI_Length_dtx_e_dch_TTI_10ms))
  {
    e_tti = TDSL1_E_TTI_10MS;
  }
  else
  {
    e_tti = TDSL1_E_TTI_2MS;
  }
  
  if(tdsordered_config_ptr->mac_e_config.e_tti != e_tti)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"E-DCH TTI %d which is not same as DTX TTI =%d",tdsordered_config_ptr->mac_e_config.e_tti,e_tti);
    status = FAILURE;
  }

  /*1) Check    if the value of the IE "UE DTX cycle 2" is not an integer multiple 
  *of the value of the IE "UE DTX cycle 1".
  */
  if(status == SUCCESS)
  {
    if(e_tti == TDSL1_E_TTI_10MS)
    {
      ue_dtx_cycle1 = ((uint8)tdsrrcllcpcie_get_dtx_cycle1(
           &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_10ms->ue_dtx_Cycle1_10ms,e_tti));
      
      ue_dtx_cycle2 = ((uint8)tdsrrcllcpcie_get_dtx_cycle2(
           &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_10ms->ue_dtx_Cycle2_10ms,e_tti));
      
      l1_mac_dtx_cycle = (uint8)tdsrrcllcpcie_get_mac_dtx_cycle(
          &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_10ms->mac_dtx_Cycle_10ms,e_tti);
    }
    else
    {
      ue_dtx_cycle1 = ((uint8)tdsrrcllcpcie_get_dtx_cycle1(
           &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_2ms->ue_dtx_Cycle1_2ms,e_tti));
      
      ue_dtx_cycle2 = ((uint8)tdsrrcllcpcie_get_dtx_cycle2(
           &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_2ms->ue_dtx_Cycle2_2ms,e_tti));
      
      l1_mac_dtx_cycle = (uint8)tdsrrcllcpcie_get_mac_dtx_cycle(
          &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_2ms->mac_dtx_Cycle_2ms,e_tti);
    }
  }
  
  if(status == SUCCESS)
  {
    if(ue_dtx_cycle1 <= ue_dtx_cycle2)
    {
      if(ue_dtx_cycle2 % ue_dtx_cycle1 != 0)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: dtx_cycle2(%d) is not an integer multiple of dtx_cycle1(%d) ",ue_dtx_cycle2,ue_dtx_cycle1);
        status = FAILURE;
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: dtx_cycle2(%d) is an integer multiple of dtx_cycle1(%d) .check pass case",ue_dtx_cycle2,ue_dtx_cycle1);
      }
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: dtx_cycle1(%d) is greater then dtx_cycle2(%d) ",ue_dtx_cycle1,ue_dtx_cycle2);
      status = FAILURE;
    }
  }
  
  /*2) Check if the value of the IE "UE DTX cycle 2" is not an integer multiple or 
   * a divisor of the value of the IE "CQI Feedback cycle, k"divided by 2; .
  */
  if(status == SUCCESS)
  {
    if(tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle == 0)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: cqi_feedback_cycle(%d) is not an integer multiple of dtx_cycle2(%d) ",(tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle),ue_dtx_cycle2);
      status = FAILURE;
    }
    else
    {
      if(((tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle/2) <=
          ue_dtx_cycle2))
      {
        if((ue_dtx_cycle2 %
          (tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle/2)) != 0)
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: ue_dtx_cycle2(%d) is not an integer multiple of cqi_feedback_cycle(%d) ",ue_dtx_cycle2,(tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle/2));
          status = FAILURE;
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: cqi_feedback_cycle(%d) is an integer multiple of dtx_cycle2(%d) .check pass case",(tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle/2),ue_dtx_cycle2);
        }
      }
      else
      {
        if(((tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle/2) %
          ue_dtx_cycle2) != 0)
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: cqi_feedback_cycle(%d) is not an integer multiple of dtx_cycle2(%d) ",(tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle/2),ue_dtx_cycle2);
          status = FAILURE;
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: ue_dtx_cycle2(%d) is an integer multiple of cqi_feedback_cycle(%d) .check pass case",ue_dtx_cycle2,(tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.meas_feedback_info.cqi_feedback_cycle/2));
        }
      }
    }
  }
    
  /*3) Check if the value of the IE "UE DPCCH burst 1" is greater than the value of the IE "UE DTX cycle 1"; 
   */
  if(status == SUCCESS)
  {
    ue_dpcch_burst_1 = (uint8)tdsrrcllcpcie_get_ue_dpcch_burst_1_info(&dtx_Info->ue_dpcch_Burst1);
    ue_dpcch_burst_2 = (uint8)tdsrrcllcpcie_get_ue_dpcch_burst_2_info(&dtx_Info->ue_dpcch_Burst2);
    if(ue_dpcch_burst_1 > ue_dtx_cycle1)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: ue_dpcch_burst_1(%d) greater then ue_dtx_cycle1(%d) ",ue_dpcch_burst_1,ue_dtx_cycle1);
      status = FAILURE;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: ue_dpcch_burst_1(%d) is NOT greater then ue_dtx_cycle1(%d) .check pass case",ue_dpcch_burst_1,ue_dtx_cycle1);
    }
  }
  /*4) Check if the value of the IE "UE DPCCH burst 2" is greater than the value of the IE "UE DTX cycle 2"
  */
  if(status == SUCCESS)
  {
    if(ue_dpcch_burst_2 > ue_dtx_cycle2)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: ue_dpcch_burst_1(%d) greater then ue_dtx_cycle1(%d) ",ue_dpcch_burst_2,ue_dtx_cycle2);
      status = FAILURE;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: ue_dpcch_burst_2(%d) is NOT greater then ue_dtx_cycle2(%d)..check pass case",ue_dpcch_burst_2,ue_dtx_cycle2);
    }
  }
  /*5) if the value of the IE "UE DTX cycle 1" is not an integer multiple or a divisor of the value of the IE "MAC DTX cycle"
  */
  if(status == SUCCESS)
  {
    if((ue_dtx_cycle1 <= l1_mac_dtx_cycle))
    {
      if((l1_mac_dtx_cycle % ue_dtx_cycle1) != 0)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: ue_dtx_cycle1(%d) is not an integer multiple of l1_mac_dtx_cycle(%d) ",ue_dtx_cycle1,l1_mac_dtx_cycle);
        status = FAILURE;
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: ue_dtx_cycle1(%d) is an integer multiple of l1_mac_dtx_cycle(%d) ",ue_dtx_cycle1,l1_mac_dtx_cycle);
      }
    }
    else
    {
      if((ue_dtx_cycle1 % l1_mac_dtx_cycle) != 0)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCCPC: ue_dtx_cycle1(%d) is not an integer multiple of l1_mac_dtx_cycle(%d) ",ue_dtx_cycle1,l1_mac_dtx_cycle);
        status = FAILURE;
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC: ue_dtx_cycle1(%d) is an integer multiple of l1_mac_dtx_cycle(%d) ",ue_dtx_cycle1,l1_mac_dtx_cycle);
      }
    }
  }
  /*6) if the IE "UE DTX long preamble length" is set to 4 or 15 slots and the value of the 
   *IE "Inactivity Threshold for UE DTX cycle 2" is less than 4 TTIs (for 10ms E-DCH TTI) or 8 TTIs (for 2ms E-DCH TTI);  
  */
  if(status == SUCCESS)
  {
    if(dtx_Info->m.ue_dtx_long_preamble_lengthPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC: ue_dtx_long_preamble_length_present in the message");
    dtx_long_preamble_length = (uint8)tdsrrcllcpcie_get_ue_dtx_long_preamble_length(
       &dtx_Info->ue_dtx_long_preamble_length);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC: ue_dtx_long_preamble_length in NOT present ,set default val(2)");
      dtx_long_preamble_length = (uint8) L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_2;
    }

    inact_thrshd_dtx_cycle2 = (uint16)tdsrrcllcpcie_get_inact_thrshd_dtx_cycle2(
    &dtx_Info->ue_dtx_cycle2InactivityThreshold);

    if((dtx_long_preamble_length == (uint8)L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_4)||
      (dtx_long_preamble_length == (uint8)L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_15))
    {
      if(((e_tti == TDSL1_E_TTI_2MS)&&
        (inact_thrshd_dtx_cycle2 < (uint16)L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_8)
        )||
        ((e_tti == TDSL1_E_TTI_10MS)&&
        (inact_thrshd_dtx_cycle2 < (uint16)L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_4))
        )
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"RRCCPC: Validation of inact_thrshd_dtx_cycle2(%d) fail,E-TTI=%d, long_pre_len = %d",
           inact_thrshd_dtx_cycle2,
          e_tti,
          dtx_long_preamble_length);
        status = FAILURE;
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCCPC: Validation of inact_thrshd_dtx_cycle2(%d) Pass..,E-TTI=%d, long_pre_len = %d",
          inact_thrshd_dtx_cycle2,
          e_tti,
          dtx_long_preamble_length);
      }
    }
  }

  /*7) Only the values of the IE “UE DTX DRX Offsetthat fulfill the equation UE DTX DRX 
  *Offset mod 5=0 can be configured with 10ms E-DCH TTI. The UE behaviour is not specified 
  *if any other values are used with 10ms TTI.  
  */
  if(status == SUCCESS)
  {
    if(e_tti == TDSL1_E_TTI_10MS)
    {
      if((tdsordered_config_ptr->dtx_drx_timing_info.dtx_drx_offset % 5) != 0)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:dtx_drx_offset(%d) mod 5 != 0 for 10msec E-TTI,reject config",tdsordered_config_ptr->dtx_drx_timing_info.dtx_drx_offset);
        status = FAILURE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:dtx_drx_offset(%d) mod 5 = 0 for 10msec E-TTI,check pass",tdsordered_config_ptr->dtx_drx_timing_info.dtx_drx_offset);
      }
    }
  }
  return status;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ue_dpcch_burst_2_info()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_ue_dpcch_burst_2_info_enum_type tdsrrcllcpcie_get_ue_dpcch_burst_2_info
(
    tdsrrc_UE_DPCCH_Burst *ue_dpcch_Burst2
)
{
  l1_ue_dpcch_burst_2_info_enum_type int_ue_dpcch_burst_2_info = 
    L1_UE_DPCCH_BURST_2_NUM_SUBFM_MAX;

  switch(*ue_dpcch_Burst2)
  {
    case tdsrrc_UE_DPCCH_Burst_sub_frames_1:
      int_ue_dpcch_burst_2_info = L1_UE_DPCCH_BURST_2_NUM_SUBFM_1;
      break;

    case tdsrrc_UE_DPCCH_Burst_sub_frames_2:
      int_ue_dpcch_burst_2_info = L1_UE_DPCCH_BURST_2_NUM_SUBFM_2;
      break;

    case tdsrrc_UE_DPCCH_Burst_sub_frames_5:
      int_ue_dpcch_burst_2_info = L1_UE_DPCCH_BURST_2_NUM_SUBFM_5;
      break;

    default :
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for ue_dpcch_burst_2_info=%d ",*ue_dpcch_Burst2);
      int_ue_dpcch_burst_2_info = L1_UE_DPCCH_BURST_2_NUM_SUBFM_MAX;
      break;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:ue_dpcch_burst_2_info_ota=%d, ue_dpcch_burst_2_info_int=%d,",*ue_dpcch_Burst2,int_ue_dpcch_burst_2_info);

  return int_ue_dpcch_burst_2_info;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ue_dpcch_burst_1_info()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_ue_dpcch_burst_1_info_enum_type tdsrrcllcpcie_get_ue_dpcch_burst_1_info
(
    tdsrrc_UE_DPCCH_Burst *ue_dpcch_burst_1_info
)
{
  l1_ue_dpcch_burst_1_info_enum_type int_ue_dpcch_burst_1_info = 
    L1_UE_DPCCH_BURST_1_NUM_SUBFM_MAX;

  switch(*ue_dpcch_burst_1_info)
  {
    case tdsrrc_UE_DPCCH_Burst_sub_frames_1:
      int_ue_dpcch_burst_1_info = L1_UE_DPCCH_BURST_1_NUM_SUBFM_1;
      break;

    case tdsrrc_UE_DPCCH_Burst_sub_frames_2:
      int_ue_dpcch_burst_1_info = L1_UE_DPCCH_BURST_1_NUM_SUBFM_2;
      break;

    case tdsrrc_UE_DPCCH_Burst_sub_frames_5:
      int_ue_dpcch_burst_1_info = L1_UE_DPCCH_BURST_1_NUM_SUBFM_5;
      break;

    default :
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for ue_dpcch_burst_1_info=%d ",*ue_dpcch_burst_1_info);
      int_ue_dpcch_burst_1_info = L1_UE_DPCCH_BURST_1_NUM_SUBFM_MAX;
      break;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:ue_dpcch_burst_1_info_ota=%d, ue_dpcch_burst_1_info_int=%d,",*ue_dpcch_burst_1_info,int_ue_dpcch_burst_1_info);

  return int_ue_dpcch_burst_1_info;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_cqi_dtx_timer()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_cqi_dtx_timer_info_enum_type tdsrrcllcpcie_get_cqi_dtx_timer
(
  tdsrrc_CQI_DTX_Timer *cqi_dtx_Timer
)
{
  l1_cqi_dtx_timer_info_enum_type int_cqi_dtx_Timer = 
    L1_CQI_DTX_TIMER_NUM_SUBFM_MAX;

  switch(*cqi_dtx_Timer)
  {
    case tdsrrc_CQI_DTX_Timer_sub_frames_0:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_0;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_1:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_1;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_2:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_2;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_4:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_4;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_8:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_8;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_16:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_16;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_32:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_32;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_64:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_64;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_128:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_128;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_256:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_256;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_512:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_512;
      break;

    case tdsrrc_CQI_DTX_Timer_sub_frames_Infinity:
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_INFINITY;
      break;
      
    default :
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for cqi_dtx_Timer=%d ",*cqi_dtx_Timer);
      int_cqi_dtx_Timer = L1_CQI_DTX_TIMER_NUM_SUBFM_MAX;
      break;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:cqi_dtx_Timer_ota=%d, cqi_dtx_Timer_int=%d,",*cqi_dtx_Timer,int_cqi_dtx_Timer);

  return int_cqi_dtx_Timer;
}
  
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ue_dtx_long_preamble_length()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static l1_mac_inactivity_threshold_info_enum_type tdsrrcllcpcie_get_mac_inactivity_threshold
(
  tdsrrc_MAC_InactivityThreshold *mac_InactivityThreshold
)
{
  l1_mac_inactivity_threshold_info_enum_type int_mac_InactivityThreshold =
    L1_MAC_INACT_THRSHD_NUM_TTI_MAX;
  switch(*mac_InactivityThreshold)
  {
    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_1:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_1;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_2:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_2;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_4:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_4;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_8:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_8;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_16:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_16;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_32:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_32;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_64:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_64;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_128:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_128;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_512:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_512;
      break;

    case tdsrrc_MAC_InactivityThreshold_e_dch_tti_Infinity:
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_INFINITY;
      break;

      
    default :
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for mac_InactivityThreshold=%d ",*mac_InactivityThreshold);
      int_mac_InactivityThreshold = L1_MAC_INACT_THRSHD_NUM_TTI_MAX;
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:mac_InactivityThreshold_ota=%d, mac_InactivityThreshold_int=%d,",*mac_InactivityThreshold,int_mac_InactivityThreshold);

  return int_mac_InactivityThreshold;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ue_dtx_long_preamble_length()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_dtx_long_preamble_length_info_enum_type tdsrrcllcpcie_get_ue_dtx_long_preamble_length
(
  tdsrrc_UE_DTX_long_preamble_length *ue_dtx_long_preamble_length
)
{
  l1_dtx_long_preamble_length_info_enum_type int_ue_dtx_long_preamble_length
    = L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_MAX;
  switch(*ue_dtx_long_preamble_length)
  {
    case tdsrrc_UE_DTX_long_preamble_length_slots_4:
      int_ue_dtx_long_preamble_length = L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_4;
      break;
      
    case tdsrrc_UE_DTX_long_preamble_length_slots_15:
      int_ue_dtx_long_preamble_length = L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_15;
      break;

    default :
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for ue_dtx_long_preamble_length=%d ",*ue_dtx_long_preamble_length);
      int_ue_dtx_long_preamble_length = L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_2;
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:ue_dtx_long_preamble_length_ota=%d, ue_dtx_long_preamble_length_int=%d,",*ue_dtx_long_preamble_length,int_ue_dtx_long_preamble_length);
  return int_ue_dtx_long_preamble_length;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_inact_thrshd_dtx_cycle2()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_inactivity_threshold_for_ue_dtx_cycle2_info_enum_type tdsrrcllcpcie_get_inact_thrshd_dtx_cycle2
(
  tdsrrc_UE_DTX_Cycle2InactivityThreshold *ue_dtx_cycle2_inactiv_thrshd
)
{
  l1_inactivity_threshold_for_ue_dtx_cycle2_info_enum_type int_ue_dtx_cycle2_inactiv_thrshd =
    L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_INVALID;
  switch(*ue_dtx_cycle2_inactiv_thrshd)
  {
    case tdsrrc_UE_DTX_Cycle2InactivityThreshold_e_dch_tti_1:
      int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_1;
      break;

  case tdsrrc_UE_DTX_Cycle2InactivityThreshold_e_dch_tti_4:
    int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_4;
    break;

  case tdsrrc_UE_DTX_Cycle2InactivityThreshold_e_dch_tti_8:
    int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_8;
    break;

  case tdsrrc_UE_DTX_Cycle2InactivityThreshold_e_dch_tti_16:
    int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_16;
    break;

  case tdsrrc_UE_DTX_Cycle2InactivityThreshold_e_dch_tti_32:
    int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_32;
    break;

  case tdsrrc_UE_DTX_Cycle2InactivityThreshold_e_dch_tti_64:
    int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_64;
    break;

  case tdsrrc_UE_DTX_Cycle2InactivityThreshold_e_dch_tti_128:
    int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_128;
    break;

  case tdsrrc_UE_DTX_Cycle2InactivityThreshold_e_dch_tti_256:
    int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_256;
    break;

    default :
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for Inactivity thrshd for UE dtx cycle2=%d ",*ue_dtx_cycle2_inactiv_thrshd);
      int_ue_dtx_cycle2_inactiv_thrshd = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_INVALID;
      break;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:rxd inactiv_thrshd dtx cycl2 =%d[Value:0..7::SubFm:1/4/8/16/32/64/128/256]",*ue_dtx_cycle2_inactiv_thrshd);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:rxd inactiv_thrshd dtx cycl2 =%d[Value:0..8::SubFm:1/4/8/16/32/64/128/256/MAX]",int_ue_dtx_cycle2_inactiv_thrshd);
  return int_ue_dtx_cycle2_inactiv_thrshd;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_mac_dtx_cycle()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_mac_dtx_cycle_info_enum_type tdsrrcllcpcie_get_mac_dtx_cycle
(
  void    *mac_dtx_cycle,
  tdsl1_e_tti_enum_type e_tti
)
{
  tdsrrc_MAC_DTX_Cycle_10ms *mac_dtx_Cycle_10ms = NULL;
  tdsrrc_MAC_DTX_Cycle_2ms *mac_dtx_Cycle_2ms = NULL;
  l1_mac_dtx_cycle_info_enum_type int_mac_dtx_cycle = 
    L1_MAC_DTX_CYCLE_NUM_SUBFM_MAX;

if(e_tti == TDSL1_E_TTI_10MS )
{
  mac_dtx_Cycle_10ms = (tdsrrc_MAC_DTX_Cycle_10ms *)mac_dtx_cycle;
}
else
{
  mac_dtx_Cycle_2ms = (tdsrrc_MAC_DTX_Cycle_2ms *)mac_dtx_cycle;
}

if(e_tti == TDSL1_E_TTI_10MS )
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:rxd mac_dtx_Cycle_10ms = %d[Value:0..2::SubFm:5/10/20]",*mac_dtx_Cycle_10ms);
  switch(*mac_dtx_Cycle_10ms)
  {
    case tdsrrc_MAC_DTX_Cycle_10ms_sub_frames_5:
      int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_5;
      break;

      case tdsrrc_MAC_DTX_Cycle_10ms_sub_frames_10:
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_10;
        break;
        
        case tdsrrc_MAC_DTX_Cycle_10ms_sub_frames_20:
          int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_20;
          break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for mac dtx cycle=%d for 10msec E-DCH TTI",*mac_dtx_Cycle_10ms);
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_MAX;
        break;
  }
}
else
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:rxd mac_dtx_Cycle_2ms = %d[Value:0..6::SubFm:1/4/5/8/10/16/20]",*mac_dtx_Cycle_2ms);
  switch(*mac_dtx_Cycle_2ms)
  {
    case tdsrrc_MAC_DTX_Cycle_2ms_sub_frames_1:
      int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_1;
      break;

      case tdsrrc_MAC_DTX_Cycle_2ms_sub_frames_4:
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_4;
        break;

      case tdsrrc_MAC_DTX_Cycle_2ms_sub_frames_5:
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_5;
        break;

      case tdsrrc_MAC_DTX_Cycle_2ms_sub_frames_8:
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_8;
        break;

      case tdsrrc_MAC_DTX_Cycle_2ms_sub_frames_10:
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_10;
        break;

      case tdsrrc_MAC_DTX_Cycle_2ms_sub_frames_16:
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_16;
        break;

      case tdsrrc_MAC_DTX_Cycle_2ms_sub_frames_20:
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_20;
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for mac dtx cycle=%d for 2msec E-DCH TTI",*mac_dtx_Cycle_2ms);
        int_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_MAX;
        break;
  }
}
TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:int_mac_dtx_cycle = %d[Value:0..7::SubFm:1/4/5/8/10/16/20/MAX]",int_mac_dtx_cycle);
return int_mac_dtx_cycle;

}

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_dtx_cycle2()

DESCRIPTION:
  This function processes DTX info in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_ue_dtx_cycle2_info_enum_type  tdsrrcllcpcie_get_dtx_cycle2
(
  void    *ue_dtx_cycle2,
  tdsl1_e_tti_enum_type e_tti
)
{
  tdsrrc_UE_DTX_Cycle2_10ms *ue_dtx_Cycle2_10ms = NULL;
  tdsrrc_UE_DTX_Cycle2_2ms *ue_dtx_Cycle2_2ms = NULL;
  l1_ue_dtx_cycle2_info_enum_type int_ue_dtx_cycle2 = 
    L1_UE_DTX_CYCLE2_NUM_SUBFM_MAX;

  if(e_tti == TDSL1_E_TTI_10MS )
  {
    ue_dtx_Cycle2_10ms = (tdsrrc_UE_DTX_Cycle2_10ms *)ue_dtx_cycle2;
  }
  else
  {
    ue_dtx_Cycle2_2ms = (tdsrrc_UE_DTX_Cycle2_2ms *)ue_dtx_cycle2;
  }

  if(e_tti == TDSL1_E_TTI_10MS )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:rxd dtx_Cycle2_10ms = %d[Value:0...5::SubFm:5/10/20/40/80/160]",*ue_dtx_Cycle2_10ms);
    switch(*ue_dtx_Cycle2_10ms)
    {
      case tdsrrc_UE_DTX_Cycle2_10ms_sub_frames_5:
        int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_5;
        break;

        case tdsrrc_UE_DTX_Cycle2_10ms_sub_frames_10:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_10;
          break;

        case tdsrrc_UE_DTX_Cycle2_10ms_sub_frames_20:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_20;
          break;

        case tdsrrc_UE_DTX_Cycle2_10ms_sub_frames_40:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_40;
          break;

        case tdsrrc_UE_DTX_Cycle2_10ms_sub_frames_80:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_80;
          break;

        case tdsrrc_UE_DTX_Cycle2_10ms_sub_frames_160:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_160;
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for dtx cycle2=%d for 10msec E-DCH TTI",*ue_dtx_Cycle2_10ms);
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_MAX;
          break;
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:rxd dtx_Cycle2_2ms = %d[Value:0...11::SubFm:4/5/8/10/16/20/32/40/64/80/128/160]",*ue_dtx_Cycle2_2ms);
    switch(*ue_dtx_Cycle2_2ms)
    {
      case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_4:
        int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_4;
        break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_5:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_5;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_8:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_8;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_10:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_10;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_16:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_16;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_20:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_20;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_32:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_32;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_40:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_40;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_64:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_64;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_80:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_80;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_128:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_128;
          break;

        case tdsrrc_UE_DTX_Cycle2_2ms_sub_frames_160:
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_160;
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for dtx cycle2=%d for 2msec E-DCH TTI",*ue_dtx_Cycle2_2ms);
          int_ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_MAX;
          break;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:int dtx_cycle2 = %d[Value:0...13::SubFm:4/5/8/10/16/20/32/40/64/80/128/160/MAX]",int_ue_dtx_cycle2);
  return int_ue_dtx_cycle2;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ul_dpcch_slot_format()

DESCRIPTION:
  This function will provide ul_dpcch_slot_format in internal format
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_uplink_dpcch_slot_format_info_enum_type  tdsrrcllcpcie_get_ul_dpcch_slot_format
(
tdsrrc_Uplink_DPCCH_Slot_Format_Information ul_dcch_slot_format
)
{
  l1_uplink_dpcch_slot_format_info_enum_type int_ul_dcch_slot_format
    = L1_UL_DPCCH_SLOT_FORMAT_MAX;
  switch(ul_dcch_slot_format)
  {
    case tdsrrc_Uplink_DPCCH_Slot_Format_Information_slot_format_1:
      int_ul_dcch_slot_format = L1_UL_DPCCH_SLOT_FORMAT_1;
      break;
  
    case tdsrrc_Uplink_DPCCH_Slot_Format_Information_dummy:
      int_ul_dcch_slot_format = L1_UL_DPCCH_SLOT_FORMAT_3;
      break;
      
    case tdsrrc_Uplink_DPCCH_Slot_Format_Information_slot_format_4:
      int_ul_dcch_slot_format = L1_UL_DPCCH_SLOT_FORMAT_4;
      break;
   
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:ul_dcch_slot_format=%d NOT supported",ul_dcch_slot_format);
      break;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:ul_dcch_slot_format=%d[Value/SotF:0->1/1->3/2->4] ",int_ul_dcch_slot_format);
  return int_ul_dcch_slot_format;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_dtx_cycle1()

DESCRIPTION:
  This function processes DTX info in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_ue_dtx_cycle1_info_enum_type  tdsrrcllcpcie_get_dtx_cycle1
(
  void    *ue_dtx_cycle1,
  tdsl1_e_tti_enum_type e_tti
)
{
  tdsrrc_UE_DTX_Cycle1_10ms *ue_dtx_Cycle1_10ms = NULL;
  tdsrrc_UE_DTX_Cycle1_2ms *ue_dtx_Cycle1_2ms = NULL;
  l1_ue_dtx_cycle1_info_enum_type int_ue_dtx_cycle1 = 
    L1_UE_DTX_CYCLE1_NUM_SUBFM_MAX;

  if(e_tti == TDSL1_E_TTI_10MS )
  {
    ue_dtx_Cycle1_10ms = (tdsrrc_UE_DTX_Cycle1_10ms *)ue_dtx_cycle1;
  }
  else
  {
    ue_dtx_Cycle1_2ms = (tdsrrc_UE_DTX_Cycle1_2ms *)ue_dtx_cycle1;
  }

  if(e_tti == TDSL1_E_TTI_10MS )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:rxd ue_dtx_Cycle1_10ms = %d[Value:0/1/2/3::SubFm:1/5/10/20]",*ue_dtx_Cycle1_10ms);
    switch(*ue_dtx_Cycle1_10ms)
    {
      case tdsrrc_UE_DTX_Cycle1_10ms_sub_frames_1:
        int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_1;
        break;

        case tdsrrc_UE_DTX_Cycle1_10ms_sub_frames_5:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_5;
          break;

        case tdsrrc_UE_DTX_Cycle1_10ms_sub_frames_10:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_10;
          break;

        case tdsrrc_UE_DTX_Cycle1_10ms_sub_frames_20:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_20;
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for dtx cycle1=%d for 10msec E-DCH TTI",*ue_dtx_Cycle1_10ms);
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_MAX;
          break;
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:rxd ue_dtx_Cycle1_2ms = %d[Value:0/1/2/3/4/5/6::SubFm:1/4/5/8/10/16/20]",*ue_dtx_Cycle1_2ms);
    switch(*ue_dtx_Cycle1_2ms)
    {
      case tdsrrc_UE_DTX_Cycle1_2ms_sub_frames_1:
        int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_1;
        break;

        case tdsrrc_UE_DTX_Cycle1_2ms_sub_frames_4:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_4;
          break;

        case tdsrrc_UE_DTX_Cycle1_2ms_sub_frames_5:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_5;
          break;

        case tdsrrc_UE_DTX_Cycle1_2ms_sub_frames_8:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_8;
          break;

        case tdsrrc_UE_DTX_Cycle1_2ms_sub_frames_10:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_10;
          break;

        case tdsrrc_UE_DTX_Cycle1_2ms_sub_frames_16:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_16;
          break;

        case tdsrrc_UE_DTX_Cycle1_2ms_sub_frames_20:
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_20;
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Invalid value for dtx cycle1=%d for 2msec E-DCH TTI",*ue_dtx_Cycle1_2ms);
          int_ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_MAX;
          break;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:int_ue_dtx_cycle1 = %d[Value:0/1/2/3/4/5/6/7::SubFm:1/4/5/8/10/16/20/MAX]",int_ue_dtx_cycle1);
  return int_ue_dtx_cycle1;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_init_dtx_info_before_processing()

DESCRIPTION:
  This function will init stored DTX info before processing 
  DTX_info I.E
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
void  tdsrrcllcpcie_init_dtx_info_before_processing
(
tdsordered_config_type *config_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Initing any previouly stored dtx info");
  config_ptr->dtx_info.cqi_dtx_timer = L1_CQI_DTX_TIMER_NUM_SUBFM_MAX;
  config_ptr->dtx_info.default_sg_in_dtx_cycle2 = TDSL1_SERVING_GRANT_NOT_PRESENT;
  config_ptr->dtx_info.dtx_long_preamble_length = L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_MAX;
  config_ptr->dtx_info.e_tti = TDSL1_NUM_E_TTI;
  config_ptr->dtx_info.inact_thrshd_dtx_cycle2 = L1_INACT_THRSHD_FOR_CYCLE2_NUM_TTI_INVALID;
  config_ptr->dtx_info.l1_mac_dtx_cycle = L1_MAC_DTX_CYCLE_NUM_SUBFM_MAX;
  config_ptr->dtx_info.l1_mac_inactivity_threshold = L1_MAC_INACT_THRSHD_NUM_TTI_MAX;
  config_ptr->dtx_info.ue_dpcch_burst_1 = L1_UE_DPCCH_BURST_1_NUM_SUBFM_MAX;
  config_ptr->dtx_info.ue_dpcch_burst_2 = L1_UE_DPCCH_BURST_2_NUM_SUBFM_MAX;
  config_ptr->dtx_info.ue_dtx_cycle1 = L1_UE_DTX_CYCLE1_NUM_SUBFM_MAX;
  config_ptr->dtx_info.ue_dtx_cycle2 = L1_UE_DTX_CYCLE2_NUM_SUBFM_MAX;
  config_ptr->dtx_info.uplink_dpcch_slot_format_info = L1_UL_DPCCH_SLOT_FORMAT_MAX;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dtx_info()

DESCRIPTION:
  This function processes DTX info in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
void  tdsrrcllcpcie_process_dtx_info
(
  tdsrrc_DTX_Info    *dtx_Info,
  uint32 dtx_Info_present_in_ota
)
{
  tdsl1_e_tti_enum_type             e_tti = TDSL1_NUM_E_TTI;
  if(dtx_Info_present_in_ota)
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC:Process DTX info I.E");
    /*Fill dtx cycle length*/

    /* store the contents of the IE in the variable DTX_DRX_PARAMS, replacing or clearing 
    *any previously stored "DTX Information" or "DRX Information"*/
    tdsrrcllcpcie_init_dtx_info_before_processing(tdsordered_config_ptr);
    
    if((dtx_Info->e_dch_TTI_Length.t
      == T_tdsrrc_DTX_Info_e_dch_TTI_Length_dtx_e_dch_TTI_10ms ))
    {
      e_tti = TDSL1_E_TTI_10MS;
    }
    else
    {
      e_tti = TDSL1_E_TTI_2MS;
    }
    
    if(e_tti == TDSL1_E_TTI_10MS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Rxd DTX cycle lenght for 10msec E-DCH TTI");

      tdsordered_config_ptr->dtx_info.e_tti = TDSL1_E_TTI_10MS;
      
      tdsordered_config_ptr->dtx_info.ue_dtx_cycle1 = tdsrrcllcpcie_get_dtx_cycle1(
         &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_10ms->ue_dtx_Cycle1_10ms,
      tdsordered_config_ptr->dtx_info.e_tti);
  
      tdsordered_config_ptr->dtx_info.ue_dtx_cycle2 = tdsrrcllcpcie_get_dtx_cycle2(
         &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_10ms->ue_dtx_Cycle2_10ms,
      tdsordered_config_ptr->dtx_info.e_tti);

      tdsordered_config_ptr->dtx_info.l1_mac_dtx_cycle = tdsrrcllcpcie_get_mac_dtx_cycle(
        &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_10ms->mac_dtx_Cycle_10ms,
        tdsordered_config_ptr->dtx_info.e_tti
        );
    }
    else if(e_tti == TDSL1_E_TTI_2MS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Rxd DTX cycle lenght for 2msec E-DCH TTI");
      
      tdsordered_config_ptr->dtx_info.e_tti = TDSL1_E_TTI_2MS;
      
      tdsordered_config_ptr->dtx_info.ue_dtx_cycle1 = tdsrrcllcpcie_get_dtx_cycle1(
         &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_2ms->ue_dtx_Cycle1_2ms,
      tdsordered_config_ptr->dtx_info.e_tti);
  
      tdsordered_config_ptr->dtx_info.ue_dtx_cycle2 = tdsrrcllcpcie_get_dtx_cycle2(
         &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_2ms->ue_dtx_Cycle2_2ms,
      tdsordered_config_ptr->dtx_info.e_tti);

      tdsordered_config_ptr->dtx_info.l1_mac_dtx_cycle = tdsrrcllcpcie_get_mac_dtx_cycle(
        &dtx_Info->e_dch_TTI_Length.u.dtx_e_dch_TTI_2ms->mac_dtx_Cycle_2ms,
        tdsordered_config_ptr->dtx_info.e_tti
        );

    }

    tdsordered_config_ptr->dtx_info.inact_thrshd_dtx_cycle2 = 
       tdsrrcllcpcie_get_inact_thrshd_dtx_cycle2(&dtx_Info->ue_dtx_cycle2InactivityThreshold);
    

    if(dtx_Info->m.ue_dtx_cycle2DefaultSGPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC: Default SG present in the message");
      tdsordered_config_ptr->dtx_info.default_sg_in_dtx_cycle2 = 
        dtx_Info->ue_dtx_cycle2DefaultSG;
    }
    else
    {//kpatil:check this out for reconfig case
      if(tdsordered_config_ptr->dtx_info.default_sg_in_dtx_cycle2 != TDSL1_SERVING_GRANT_NOT_PRESENT)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC: Default SG NOT present in the message,retain prev =%d",tdsordered_config_ptr->dtx_info.default_sg_in_dtx_cycle2);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC: Default SG NOT present in the message");
        tdsordered_config_ptr->dtx_info.default_sg_in_dtx_cycle2 = TDSL1_SERVING_GRANT_NOT_PRESENT;
      }
    }

    if(dtx_Info->m.ue_dtx_long_preamble_lengthPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC: ue_dtx_long_preamble_length_present in the message");
      tdsordered_config_ptr->dtx_info.dtx_long_preamble_length = 
        tdsrrcllcpcie_get_ue_dtx_long_preamble_length(
        &dtx_Info->ue_dtx_long_preamble_length);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC: ue_dtx_long_preamble_length in NOT present in the message,set default val");
      tdsordered_config_ptr->dtx_info.dtx_long_preamble_length = 
        L1_LONG_PREAMBLE_LENGTH_NUM_SLOTS_2;
    }

   
   tdsordered_config_ptr->dtx_info.l1_mac_inactivity_threshold= 
     tdsrrcllcpcie_get_mac_inactivity_threshold (&dtx_Info->mac_InactivityThreshold);

   tdsordered_config_ptr->dtx_info.cqi_dtx_timer = 
     tdsrrcllcpcie_get_cqi_dtx_timer (&dtx_Info->cqi_dtx_Timer);

   tdsordered_config_ptr->dtx_info.ue_dpcch_burst_1= 
     tdsrrcllcpcie_get_ue_dpcch_burst_1_info (&dtx_Info->ue_dpcch_Burst1);

   tdsordered_config_ptr->dtx_info.ue_dpcch_burst_2= 
      tdsrrcllcpcie_get_ue_dpcch_burst_2_info(&dtx_Info->ue_dpcch_Burst2);

  }
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_calculate_dpcch_slot_format()

DESCRIPTION:
  This function calculate dpcch_slot_format
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uint8  tdsrrcllcpcie_calculate_dpcch_slot_format
(
  void
)
{
  uint8 slot_format = 0xFF;
  l1_ul_num_tpc_bits_enum_type numtpc;
  numtpc = tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.numtpc;
  /* Calculate DPCCH slot format */
  /* See 25.211 V3.4.0 2000-09 5.2.1, Table 2 (DPCCH fields) */
  switch (tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.fbi_len)
  {
    case TDSL1ULCFG_FBI_LEN_0:
      if(tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tfci_present)
      {
        slot_format = 0;
      }
      else
      {
        if(numtpc == L1_UL_NUM_TPC_BITS_4)
        {
          slot_format = 4;
        }
        else
        {
          slot_format = 1;
        }
      }
     break;

    case TDSL1ULCFG_FBI_LEN_1:
      slot_format = (tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tfci_present) ? 2 : 3;
      break;

    default:
      slot_format = 0xFF;
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:fbi_len=%d,tfci_present= %d",tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.fbi_len,tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tfci_present);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:slot_format = %d,numtpc=%d[0=NumTCP2,1=NumTCP4]",slot_format,numtpc);
  return slot_format;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_validate_ul_dpcch_slot_format()

DESCRIPTION:
  This function validate ul_dpcch_slot_format
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type  tdsrrcllcpcie_validate_ul_dpcch_slot_format
(
  l1_uplink_dpcch_slot_format_info_enum_type ul_dcch_slot_format
)
{
  uecomdef_status_e_type status = SUCCESS;
  uint8 slot_format = 0xFF;
  slot_format = tdsrrcllcpcie_calculate_dpcch_slot_format();
  switch(slot_format)
  {
    case 0:
      status = FAILURE;
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCCPC:UL_DPCCH slot format#0,But for cpc valid slot format's are 1,3,4");
      break;
   
    case 1:
      if(ul_dcch_slot_format == L1_UL_DPCCH_SLOT_FORMAT_1)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Slot Format is #1");
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Mismatch UL_DPCCH slot format#1,cpc slot format#%d[0->1/1->3/2->4]",ul_dcch_slot_format);
        status = FAILURE;
      }
      break;
   
    case 2:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCCPC:UL_DPCCH slot format#0,But for cpc valid slot format's are 1,3,4");
      status = FAILURE;
      break;
   
    case 3:
      if(ul_dcch_slot_format == L1_UL_DPCCH_SLOT_FORMAT_3)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Slot Format is #3");
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Mismatch UL_DPCCH slot format#3,cpc slot format#%d[0->1/1->3/2->4]",ul_dcch_slot_format);
        status = FAILURE;
      }
      break;
    
    case 4:
      if(ul_dcch_slot_format == L1_UL_DPCCH_SLOT_FORMAT_4)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Slot Format is #4");
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Mismatch UL_DPCCH slot format#4,cpc slot format#%d[0->1/1->3/2->4]",ul_dcch_slot_format);
        status = FAILURE;
      }
      break;
    
   default:
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCCPC:Mismatch UL_DPCCH slot format#Invalid,cpc slot format#%d[0->1/1->3/2->4]",ul_dcch_slot_format);
    status = FAILURE;
    break;
  }
  return status;
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dtx_drx_info()

DESCRIPTION:
  This function processes DTX-DRX timing info in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type  tdsrrcllcpcie_process_dtx_drx_info
(
  tdsrrc_DTX_DRX_Info_r7 *dtx_drx_Info,
  uint32 dtx_drx_Info_present_ota
)
{
  uecomdef_status_e_type status = SUCCESS;
  if(dtx_drx_Info_present_ota)
  {
    if((dtx_drx_Info->m.dtx_InfoPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC:Validate DTX info I.E");
      status = tdsrrcllcpcie_validate_dtx_params(&dtx_drx_Info->dtx_Info);
      if(status == SUCCESS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Process DTX info I.E");
        tdsrrcllcpcie_process_dtx_info(&dtx_drx_Info->dtx_Info,
        dtx_drx_Info->m.dtx_InfoPresent);
      }
    }
    else
    {
      /*IE "DTX information" is not included in DTX-DRX info I.E 
      */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCCPC:DTX info not present in DTX-DRX info I.E,So reject the config");
      status = FAILURE;
    }
    
    if(status == SUCCESS)
    {
      if((dtx_drx_Info->m.drx_InfoPresent))
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC:Validate DRX info I.E's");
        status = tdsrrcllcpcie_validate_drx_params(&dtx_drx_Info->drx_Info);
        if(status == SUCCESS)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Process DRX info I.E's");
          tdsrrcllcpcie_process_drx_info(&dtx_drx_Info->drx_Info,
          dtx_drx_Info->m.drx_InfoPresent);
        }
      }
#ifdef FEATURE_TDSCDMA_CPC_DRX
      else
      {
        if(tdsrrcllc_get_l1_cpc_drx_status(tdsordered_config_ptr) == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:DRX info I.E NOT presnt,set DRX action STOP");
          tdsordered_config_ptr->l1_cpc_drx_req_mask = L1_CPC_DRX_STOP;
        }
        else
        {
          /*Set action as NO_OP, as we would have set act as reconfig while handling dtx-drx timing info I.E*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:No DRX info I.E DRX action NO_OP");
          tdsordered_config_ptr->l1_cpc_drx_req_mask = L1_CPC_DRX_NO_OP;
        }
        tdsrrcllc_init_drx_info_before_processing(tdsordered_config_ptr);
      }
#endif
    }
    
    if(status == SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Validate UL DPCCH Slot format info I.E");
      status = tdsrrcllcpcie_validate_ul_dpcch_slot_format(tdsrrcllcpcie_get_ul_dpcch_slot_format
        (dtx_drx_Info->uplink_DPCCHSlotFormatInformation));
      if(status == SUCCESS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Process UL DPCCH Slot format info I.E");
        tdsordered_config_ptr->dtx_info.uplink_dpcch_slot_format_info = 
        tdsrrcllcpcie_get_ul_dpcch_slot_format(dtx_drx_Info->uplink_DPCCHSlotFormatInformation);
       }
    }
  }

  return status;
}


/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dtx_drx_params()

DESCRIPTION:
  This function processes 
  1) DTX-DRX timing info 
  2) dtx_drx_Info &
  3) evaluates DTX-DRX status in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type    tdsrrcllcpcie_process_dtx_drx_params
(
  tdsrrc_DTX_DRX_TimingInfo_r7 *dtx_drx_timingInfo,
  uint32 dtx_drx_timingInfo_present_ota,
  tdsrrc_DTX_DRX_Info_r7 *dtx_drx_Info,
  uint32 dtx_drx_Info_present_ota,
  tdsrrc_RRC_StateIndicator rrc_state
)
{
  uecomdef_status_e_type status = SUCCESS;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Check & process DTX-DRX params if any");
  /*For DTX-DRX to be ON, we need dtx_drx_timingInfo to be present*/
  if(dtx_drx_timingInfo_present_ota)
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC:Process dtx_drx_timingInfo_present_ota");
    status = tdsrrcllcpcie_process_dtx_drx_timinginfo(dtx_drx_timingInfo,
            dtx_drx_timingInfo_present_ota,
            dtx_drx_Info_present_ota);
  }

  if(status == SUCCESS)
  {
    if(dtx_drx_Info_present_ota)
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCCPC:Process dtx_drx_Info_present_ota");
      status = tdsrrcllcpcie_process_dtx_drx_info(dtx_drx_Info,
        dtx_drx_Info_present_ota);
    }
  }
  if(status == SUCCESS)
  {
    tdsrrcllc_evaluate_dtx_drx_status_var_and_set_lower_layers_action
      (rrc_state,
      dtx_drx_timingInfo_present_ota
      );
  }
  return status;
}

#endif

#ifdef FEATURE_TDSCDMA_REL8

/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_r8()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r8
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r8 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  boolean is_hho = FALSE;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_DL_InformationPerRL_List_r8 *tmp_ie_ptr = dl_info_per_rl_list_ptr;
  tdsrrc_state_e_type curr_rrc_state = tdsrrc_get_state();
  uint8               cpid = TDSRRC_INVALID_CPID;

  /* If state from CELL_DCH/CELL_FACH/CONNECTING to CELL_DCH */
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    TDSRRC_MSG1(MSG_LEGACY_MED,"curr_rrc_state = %d",curr_rrc_state);
    cpid = (uint8)tdsrrcccm_get_curr_camped_cpid();
    /* If two FreqInfo are included in the OTA */
    if ((tdsordered_config_ptr->p_freq_present)&&(tdsordered_config_ptr->s_freq_present))
    {
      /*  If the first frequency got changed. */
      if (rrc_freq != tdsordered_config_ptr->p_freq)
      {
        /* It is a inter handover */
        is_hho = TRUE;
        tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different freq %d",tdsordered_config_ptr->p_freq);
      }
      if (FALSE == is_hho)
      {
        if (dl_info_per_rl_list_present)
        {
          if (tmp_ie_ptr == NULL)
          {
            ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
          }
          /*  If CPID got changed. */
          if ((T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd ==
                 tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
                 (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                     tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                     (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
          {
            if(cpid != tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
            {
              /* It is a intra handover */
              is_hho = TRUE;
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
            }
          }
        }
      }
    }
    /* If only one FreqInfo is included in the OTA */
    else if ((tdsordered_config_ptr->p_freq_present)||(tdsordered_config_ptr->s_freq_present))
    {
      if (dl_info_per_rl_list_present)
      {
        if (tmp_ie_ptr == NULL)
        {
          ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
        }
        /*  If CPID got changed. */
        if ((T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd ==
               tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
               (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                   tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                   (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          if(tdsrrcccm_get_curr_camped_cpid() != 
                tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
          {
            /* It is a handover */
            is_hho = TRUE;
            if (tdsordered_config_ptr->p_freq != rrc_freq)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a inter handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a intra handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
            }
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
          }
        }
      }
    }
#ifdef FEATURE_TDSCDMA_SINGLE_FREQ_NO_CPID_HO_SUPPORT

      if (tdsordered_config_ptr->p_freq_present)
      {
        /*  If the first frequency got changed. */
        if (rrc_freq != tdsordered_config_ptr->p_freq)
        {
          /* It is a inter handover */
          is_hho = TRUE;
          tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Support none cpid and one freq info HO. p_freq %d different with current freq %d",tdsordered_config_ptr->p_freq,rrc_freq);
        }
      }

#endif
  }

  if (TRUE == is_hho)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO detected different freq different cell para id ");
  }
  tdsordered_config_ptr->is_hho = is_hho;
  return SUCCESS;
}

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_common_for_all_rl_r8()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_info_common_for_all_rl_r8
(
  /* Pointer to the DL CommonInformation IE */
  tdsrrc_DL_CommonInformation_r8 *ie_ptr,

  /* Pointer to the desirec DL PhyChan common information structure */
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
)
{
  tdsrrc_state_e_type rrc_state;
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Info Common for all RL");

  /* Check if HS Reset is indicated */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
               mac_hsResetIndicatorPresent))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:HSReset Indicator TRUE");
    /* Set it in MAC params */
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = TRUE;
    tdshsdpa_msg_params.msg_params_present = TRUE;
  }
  /* Get the info common for all DL DPCH if present */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
                dl_dpchInfoCommonPresent))
  {
    if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->dl_dpchInfoCommon, 
        T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon))
    {
      if(tdsrrcllcpcie_get_dl_dpch_common_info_r6(ie_ptr->dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
                                 &dpch->dpch_common
                                ) == FAILURE
        )
      {
        return FAILURE;
      }
    }
    else /* rrc_dl_FDPCH_InfoCommon_chosen */
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: rrc_dl_FDPCH_InfoCommon_chosen not yet supported");
      return(FAILURE);
    }
  } /* end if dl dpch common info present */

  /* Get modeSpecificInfo(10.3.6.24) */
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_CommonInformation_r8_modeSpecificInfo_tdd))
  {
    /* Get tstd indicator */
    if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->tddOption,
        T_tdsrrc_DL_CommonInformation_r8_tddOption_tdd128))
    {
      dpch->dpch_common.tstd_indicator =
        ie_ptr->modeSpecificInfo.u.tdd->tddOption.u.tdd128->tstd_Indicator;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd->tddOption is not tdd128");
      return FAILURE;
    }

    /* Get the default DPCH offset if present */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
                  defaultDPCH_OffsetValuePresent))
    {
      /* DefaultDPCH-OffsetValueTDD ::= INTEGER (0..7) */
      dpch->doff =
        (uint8) ie_ptr->modeSpecificInfo.u.tdd->defaultDPCH_OffsetValue;
    
      if(dpch->doff > TDSRRCLLC_ASN1_DOFF_MAX_VAL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Doff ASN1 value out of range");
        return(FAILURE);
      }
      dpch->opt_parm_presence_bitmask |=
        TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
    }
    else
    {
      rrc_state = tdsrrc_get_state();

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_INIT)
      {
        /*For HHO, assign DOFF = 0 to L1*/
        if((tdsordered_config_ptr->is_hho)&&(rrc_state == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Non-DCH ->DCH case. Pending clarification from NW. If yes, merge with above case*/
        else if(rrc_state != TDSRRC_STATE_CELL_DCH)
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Return failure since DOFF is missing, e.g. DCH->DCH reconfig case*/
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"We do fault-tolerance for DOFF is missing");
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }

      }

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN)
      {
        /*For Non-DCH -> DCH case, make DOFF = 0.*/
        if((rrc_state != TDSRRC_STATE_CELL_DCH)&&(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
          dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
      }
    }  /* Doff is absent */
  }
  /* end if common TDD */
  else
  {
    /* TDD not selected.  Error as FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_COMMON_FOR_ALL_RL");
    return FAILURE;
  }

  /* Indicate that Downlink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
 /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_common_info_r8()

DESCRIPTION:
  This function updates IE "dl common info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_common_info_r8
(
  uint32 dl_common_info_present,
  tdsrrc_DL_CommonInformation_r8 *dl_common_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Get the DL information for all Radio Links */
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
  /* Initialize the L1 bitmask for the parameters in the "Downlink
  * information for all radio links" IE.  Since the IE is optional,
  * we want to make sure we don't signal L1 erroneously that we have
  * received information when we haven't.
    */

    /* Init DL Common Info to Timing Maintained */
    tdsrrcllc_init_dl_common_info(tdsordered_config_ptr);
    
    if (dl_common_info_present)
    {
      if(tdsrrcllcpcie_get_dl_info_common_for_all_rl_r8(dl_common_info_ptr,
        &tdsordered_config_ptr->l1_dl_chan_parms.dpch
        )
        == FAILURE)
      {
        return FAILURE;
      }
    } /* end DL common RL info */
    
  } /* If next state is Cell_DCH */
  /* Calculate the DL TFCS */
  tdsrrcllc_update_rm_restriction_info(&tdsordered_config_ptr->l1_dl_cctrch_parms,
    &tdsordered_config_ptr->dl_ctfc,
    &tdsordered_config_ptr->l1_dl_ctfc_parms
    );
   
  return(SUCCESS);
}

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_for_each_rl_r8()

DESCRIPTION     This function translates the Downlink information for
                each radio link IE to the RRC config database

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
static tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllcpcie_get_dl_info_for_each_rl_r8
(
  tdsrrc_DL_InformationPerRL_r8 *ie_ptr,

  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Get Dl Info per RL");
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd))
  {
    /* First check if the DL DPCH Info per RL is included. If not,
    * this is NOT a valid Radio link - ignore it.
    */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,  
                dl_dpchInfoPresent))
    {
      /* save tstd info */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
        rrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption,
            rrc_PrimaryCCPCH_Info_r4_tddOption_tdd128))
        {
          dl_phychan->pccpch.tstd_indicator =
            ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption.u.tdd128->tstd_Indicator;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD128 info present in DL_INFO_FOR_EACH_RL");
          return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }
      /*If Cell_id is present for this RL, store it in tdscell_id_per_rl */
      if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                cell_idPresent))
      {
        tdscell_id_per_rl[0] = (uint32) tdsrrc_translate_cell_id(&(ie_ptr->cell_id));
      }       
      /* Get the DL DPCH info for this radio link */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get DL DPCH Info for RL %d",1);

      if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->dl_dpchInfo, 
                  T_tdsrrc_DL_InformationPerRL_r8_dl_dpchInfo_dl_DPCH_InfoPerRL))
      {
        if (tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r7(ie_ptr->dl_dpchInfo.u.dl_DPCH_InfoPerRL,
          dl_phychan) == FAILURE)

        {
          return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDPCH Unsupported");
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }

      /* if we made it here we must be ok */
      /* Indicate that Downlink L1 needs to be configured */
      tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
      * still needs to be reconfigured so as to get the new CCTrCH
      * ID whenever Downlink L1 is reconfigured. This is due to the
      * interface requirements.
      */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
      return TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS;
    }  /* DL DPCH Info is present */
    else
    {
      /* DL DPCH Info per RL is NOT present - ignore this IE */
      return TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
    }
  }
  else
  {
    /* No TDD info.  We don't support FDD so return indicating an error */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
    return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
  }
}/* tdsrrcllcpcie_get_dl_info_for_each_rl_r8() */

/*===========================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_per_rl_r8()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_info_per_rl_r8
(
  tdsrrc_DL_InformationPerRL_List_r8 *ie_ptr,
  tdsrrc_RRC_StateIndicator next_state
)
{
  /*boolean is_inter_freq = FALSE;*/
  tdsrrcllcoc_dl_info_per_rl_status_e_type status =
    TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  tdsordered_config_ptr->cpid_present = FALSE;

  /* servingHSDSCH_RL_indicator is FDD only, delete */
  if (ie_ptr->n > 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"more than one RL not supported");
    return FAILURE;
  }
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[0].modeSpecificInfo, 
                  T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL FDD not supported");
    return FAILURE;
  }
  if (T_tdsrrc_PrimaryCCPCH_Info_r4_fdd == ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL PrimaryCCPCH FDD not supported");
    return FAILURE;
  }

  /* We can say with certainty that if frequency info is included, and
  * it is different than the current frequency, then we are doing a
  * hard handover.  If this is not the case, we revisit this flag
  * later.
  */
  /*is_inter_freq = (tdsordered_config_ptr->p_freq_present &&
      tdsordered_config_ptr->p_freq != rrc_freq);*/

  if(next_state == tdsrrc_RRC_StateIndicator_cell_DCH)
  {
  /* If DPCH Info per RL is not present within DL Info per RL, then ignore it and
  * do not change the RL Info list in OC when going from DCH->DCH.
  * When going from non-DCH->DCH, the above condition should result in a Failure.
    */
    if(!((TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[0], 
                    dl_dpchInfoPresent)) &&
            ((TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[0].dl_dpchInfo, 
                     T_tdsrrc_DL_InformationPerRL_r8_dl_dpchInfo_dl_DPCH_InfoPerRL))
      )))
    {
      if(rrc_state == TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring DL Info per RL Info absent");
        if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          /* when state change from CELL_DCH to CELL_DCH, save new cellParametersID if present. Else maintain */
          tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          tdsordered_config_ptr->cpid_present = TRUE;
        }
        return SUCCESS;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failing,DPCH Info per RL not present");
        return FAILURE;
      }
    }

    /* state to CELL_DCH, dl_dpchInfo Present, save new cellParametersID if present. Else maintain */
    if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      tdsordered_config_ptr->cpid_present = TRUE;
    }

    if(rrc_state != TDSRRC_STATE_CELL_DCH)
    {
      /* msg have no cell_param_id */
      if (FALSE == tdsordered_config_ptr->cpid_present)
      {
        /* set current cell_param_id to OC structrue */
        tdsordered_config_ptr->cell_param_id = (uint8)tdsrrcccm_get_curr_camped_cpid();
      }
      /* Going from non-DCH state to CELL DCH */
      /* Always set new_rl_seen when coming to DCH from a non-DCH state.
      * That ensures the RL Info is populated after initializing.
      */
      if ((TRUE == tdsordered_config_ptr->p_freq_present)&&
          (tdsordered_config_ptr->p_freq == rrc_freq))
      {
        /* check if the current camped on ParaID is msg's RL.
         * If so, force Frequency_info to FALSE.
         */
        if ((ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)&&
           (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID != tdsrrcccm_get_curr_camped_cpid()))
        {
          /* Intra freq HHO */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"intra-f HHO FACH->DCH");
        }
        else
        {
          tdsordered_config_ptr->p_freq_present = FALSE;
        }
      }
    }
    else /* DCH -> DCH and hho */
    {
      if(tdsordered_config_ptr->is_hho)
      {
        tdshsdpa_msg_params.msg_params_present = TRUE;
      } /* idx == TDSUE_MAX_RL */
    } /* DCH -> DCH AND Not Inter Freq */

    /* Process the IE and place the info into our database. */
      status = tdsrrcllcpcie_get_dl_info_for_each_rl_r8(&ie_ptr->elem[0],
        &tdsordered_config_ptr->l1_dl_chan_parms);

      if(status == TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"status is TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE");
        return FAILURE;
      }
      else if(status == TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED)
      {
        //continue;
      }
      else if(status == TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS)
      {
      /* either inter HHO or intra HHO */
        if(tdsordered_config_ptr->is_hho)
        {
        /* If we succeeded in processing the IE, and we are doing a
        * hard handover, we must be sure that L1 is informed by
        * sending frequency info, even if we didn't get frequency
        * info in the reconfiguration message.
          */
        if(!tdsordered_config_ptr->p_freq_present)
          {
            tdsordered_config_ptr->p_freq_present = TRUE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO triggered");
          }
        }
      }

  }
  else /* All states other than Cell_DCH */
  {
    if(ie_ptr->n > 0)
    {
      if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
      {
        tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        tdsordered_config_ptr->cpid_present = TRUE;
      }
    }
  }

  return SUCCESS;
} /* end tdsrrcllcpcie_get_dl_info_per_rl_r8() */

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_agch_info_r8()

DESCRIPTION:
  This function processes e-AGCH information and updates the information
  in a given e_rl_index
      
DEPENDENCIES:
  This function needs to be called only after calling function to process E-HICH info.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_process_e_agch_info_r8
(
  uint32 e_agch_info_present,
  tdsrrc_E_AGCH_Information_r8 *e_agch_info_ptr
)
{
  uint8 temp_i;
  tdsl1_e_dl_per_rl_info_struct_type *e_dl_info_ptr 
         = &tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info;

  if ((e_agch_info_present) && 
       (T_tdsrrc_E_AGCH_Information_r8_modeSpecific_fdd == e_agch_info_ptr->modeSpecific.t))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL E_AGCH FDD not supported");
    return(FAILURE);
  }
  if ((e_agch_info_present) && 
       (T_tdsrrc_E_AGCH_Information_r8_tdd_tdd128 != e_agch_info_ptr->modeSpecific.u.tdd->t))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL E_AGCH 3.84/7.68TDD not supported");
    return(FAILURE);
  }
  if (e_agch_info_present)
  {
    /* store AGCH info only if E-HICH info is already stored */
    if (tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich != 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Begin save E_AGCH");
      /* If true, RDI will present in E-AGCH payload.otherwise, no RDI present */
      e_dl_info_ptr->e_agch_info.rdi_pre 
           = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->rdi_Indicator;
      /* TPC step size for E-PUCH power control */
      e_dl_info_ptr->e_agch_info.tpc_step 
           = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->tpc_StepSize;
      /* E-AGCH Bler Target. -3.15 to 0 step 0.05. 0 mapped to bler target and -63 mapped to -3.15*/
      e_dl_info_ptr->e_agch_info.bler_target 
           = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_BLER_Target;
      /* Total number of EAGCH channels(not more than 4) */
      e_dl_info_ptr->e_agch_info.num_eagch 
           = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.n;

      for (temp_i = 0; temp_i < e_dl_info_ptr->e_agch_info.num_eagch; temp_i++)
      {
        /* Time slot number for E-AGCH According to message from system team,
         * E-AGCH won't be sent on TS0. */
        e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i].ts_num
          = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.elem[temp_i].timeslotNumber;
        /* Channelization code for E-AGCH1 */
        e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i].first_code
          = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.elem[temp_i].firstChannelisationCode;
        /* Channelization code for E-AGCH2 */
        e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i].second_code
          = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.elem[temp_i].secondChannelisationCode;

        /*Midamble allocation mode. (10.3.6.41)*/
        switch (e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->e_AGCH_Set_Config.elem[temp_i].midambleAllocationMode.t)
        {
        case T_tdsrrc_E_AGCH_Individual_LCR_midambleAllocationMode_defaultMidamble:
          e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
          break;
        case T_tdsrrc_E_AGCH_Individual_LCR_midambleAllocationMode_commonMidamble:
          e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.allocation_mode = TDSL1_COMMON_MIDAMBLE;
          break;
        case T_tdsrrc_E_AGCH_Individual_LCR_midambleAllocationMode_ueSpecificMidamble:
          e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.allocation_mode = TDSL1_UE_SPECIFIC_MIDAMBLE;
          e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.shift = e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->
             e_AGCH_Set_Config.elem[temp_i].midambleAllocationMode.u.ueSpecificMidamble;
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL E_AGCH Midamble allocation mode not supported");
          return FAILURE;
        }
        e_dl_info_ptr->e_agch_info.eagch_slot_info[temp_i]
            .eagch_midamble.config = 
              tdsMidambleConfiguration[e_agch_info_ptr->modeSpecific.u.tdd->u.tdd128->
             e_AGCH_Set_Config.elem[temp_i].midambleConfiguration];
      }/* for */
    }
    else
    {
      /* as per our current understanding NW should not provide E-AGCH info
         unless E-HICH is provided in config message or already stored in UE */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: NW trying to provide E-AGCH info without E-HICH Info");
      return(FAILURE);
    }

    tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_channel_mask |= TDSL1_E_AGCH_INFO_INCL;
    TDSRRC_MSG0(MSG_LEGACY_MED,"E_AGCH save finish. set e_dl_channel_mask |= TDSRRCLLC_E_AGCH_INFO_PRESENT");
  }

  return(SUCCESS);
}

/*RRC_GKG: See whether the following function should be kept under HSUPA feature*/
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r8()

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r8
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r8 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  tdsrrc_E_HICH_Information_TDD128 * e_hich_info_ptr =NULL;

  /* Look into this IE only if next UE state is DCH */
  if (dl_info_per_rl_list_present && 
      (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
     )
  {
    boolean is_e_dl_info_present = FALSE;
    tdsrrc_DL_InformationPerRL_List_r8 *tmp_ie_ptr = dl_info_per_rl_list_ptr;

    if (T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo2_fdd == 
            tmp_ie_ptr->elem[0].modeSpecificInfo2.t)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL FDD not supported");
      return(FAILURE);
    }
    if (T_tdsrrc_DL_InformationPerRL_r8_tdd_tdd128 != tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->t)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL tdd384_tdd768 not supported");
      return(FAILURE);
    }
    /* check if any E-Channel related info is present in this RL Info */
    if ((tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->u.tdd128->m.e_HICH_InfoPresent)
             ||(tmp_ie_ptr->elem[0].m.e_AGCH_InformationPresent))
    {
      /* if we are inside this if check then it means that RRC needs to indicate
       * to L1 that some of the parameters in DL has changed */
      is_e_dl_info_present = TRUE;
      if(tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->u.tdd128->m.e_HICH_InfoPresent)
      {
        e_hich_info_ptr = &tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->u.tdd128->e_HICH_Info;
      }
      else
      {
        e_hich_info_ptr = NULL;
      }

      if (FAILURE == tdsrrcllcpcie_process_e_hich_info(
          tmp_ie_ptr->elem[0].modeSpecificInfo2.u.tdd->u.tdd128->m.e_HICH_InfoPresent, 
          e_hich_info_ptr))
      {
        return(FAILURE);
      }

      if (FAILURE == tdsrrcllcpcie_process_e_agch_info_r8(
          TDSRRC_CHECK_MSG_TYPE_IE(tmp_ie_ptr->elem[0], 
                   e_AGCH_InformationPresent), 
          &tmp_ie_ptr->elem[0].e_AGCH_Information))
      {
        return(FAILURE);
      }
    }/* if any E-info IE is present */

    /* update the l1_e_req_mask */
    tdsrrcllc_update_l1_e_req_mask_for_e_dl_info(is_e_dl_info_present);
  }/* dl_info_per_rl_list_present */
  return(SUCCESS);
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_info_per_rl_list_r8

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_info_per_rl_list_r8
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r8 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  if (dl_info_per_rl_list_present)
  {
    if(FAILURE == tdsrrcllcpcie_get_dl_info_per_rl_r8(dl_info_per_rl_list_ptr, rcvd_state_indicator))
    {
      return (FAILURE);
    }
  }

#ifdef FEATURE_TDSCDMA_HSUPA
  /* process EUL related information */
  if (FAILURE == tdsrrcllcpcie_process_e_info_in_dl_info_per_rl_list_r8(
      dl_info_per_rl_list_present, 
      dl_info_per_rl_list_ptr,
      rcvd_state_indicator))
  {
    return(FAILURE);
  }
#endif /* FEATURE_TDSCDMA_HSUPA */

  return(SUCCESS);
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_e_dch_information_r8()

DESCRIPTION:
  This function updates IE "ul e dch information" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_e_dch_information_r8
(
  uint32 e_dch_info_present,
  tdsrrc_UL_EDCH_Information_r8 *e_dch_info_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Look into this IE only if next UE state is DCH */
  if (e_dch_info_present && 
      (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
     )
  {
    if ((TDSRRC_CHECK_MSG_TYPE_IE_PTR(e_dch_info_ptr, 
               mac_es_e_resetIndicatorPresent)) &&
        (tdsrrc_UL_EDCH_Information_r8_mac_es_e_resetIndicator_true_ == e_dch_info_ptr->mac_es_e_resetIndicator))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"mac_es_e_resetIndicator set to TRUE");
      tdsordered_config_ptr->mac_e_reset_indicator = TRUE;
    }
    else
    {
      tdsordered_config_ptr->mac_e_reset_indicator = FALSE;
    }

    if(TDSRRC_CHECK_MSG_TYPE(e_dch_info_ptr->modeSpecificInfo, 
                  T_tdsrrc_UL_EDCH_Information_r8_modeSpecificInfo_tdd))
    {
      if (FAILURE == tdsrrcllcpcie_process_e_rucch_info(
         e_dch_info_ptr->modeSpecificInfo.u.tdd->m.e_RUCCH_InfoPresent, 
            &e_dch_info_ptr->modeSpecificInfo.u.tdd->e_RUCCH_Info))
      {
        return(FAILURE);
      }
      
      if (FAILURE == tdsrrcllcpcie_process_e_puch_info(
        e_dch_info_ptr->modeSpecificInfo.u.tdd->m.e_PUCH_InfoPresent, 
          &e_dch_info_ptr->modeSpecificInfo.u.tdd->e_PUCH_Info))
      {
        return(FAILURE);
      }
      
      
      if (FAILURE == tdsrrcllcpcie_process_non_sched_info(
         e_dch_info_ptr->modeSpecificInfo.u.tdd->m.non_ScheduledTransGrantInfoPresent,
          &e_dch_info_ptr->modeSpecificInfo.u.tdd->non_ScheduledTransGrantInfo))
      {
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport mode FDD,failure");
      return FAILURE;
    }
  }

  return(SUCCESS);
}


/*============================================================================
FUNCTION: rrcllcpcie_process_dl_hspdsch_info_and_update_hsdpa_msg_params_r8()

DESCRIPTION:
  This function updates IE "dl hspdsch info " in ordered config and update
  hsdpa_msg_param variable
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_hspdsch_info_and_upd_hsdpa_msg_params_r8
(
  uint32 dl_hspdsch_info_present,
  tdsrrc_DL_HSPDSCH_Information_r8 *dl_hspdsch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /*We need to process HSDPA related IE DL_HSPDSCH_Information only in Cell_DCH state.*/
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    /* If InterF HO is happening, then set the flags in tdshsdpa_msg_params appropriately.*/
    if (tdsordered_config_ptr->is_hho)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO is happening thru OTA.");
      tdshsdpa_msg_params.msg_params_present = TRUE;
    }

    /*If IE tdsrrc_DL_HSPDSCH_Information_r8 is present.*/
    if(dl_hspdsch_info_present)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:DL HS-PDSCH Info present.");
      tdshsdpa_msg_params.msg_params_present = TRUE;
      tdshsdpa_msg_params.hspdsch_info_present = TRUE;

      /*Process the 64 QAM IE if present.*/    
#ifdef FEATURE_TDSCDMA_64QAM
      /*If 64 QAM status is being changed by the OTA, then set the flag hsdsch_info_present in tdshsdpa_msg_params*/
      if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured != 
        tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured)
      {
        tdshsdpa_msg_params.hsdsch_info_present = TRUE;

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL8:Set HSDSCH bitmask for 64QAM. Before %d,current %d (0:Not present/1:Present)",tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured ,tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured);
      }
#endif /*FEATURE_TDSCDMA_64QAM*/

    }

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if(tdsrrcllc_set_macehs_or_machs_and_check_if_coexist()== TRUE)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL8:MAC-ehs %d and MAC-hs %d cant co-exist ",tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue,tdsordered_config_ptr->mac_dl_parms.num_dflow);
      return FAILURE;
    }
    tdsrrcllc_set_machs_ehs_transition();
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

    /* Set HSDPA Action & Physical Channel Parameters associated with it */
    if (tdsrrcllc_determine_hsdpa_action_r8(dl_hspdsch_info_ptr) == SUCCESS)
    {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_set_hsdpa_action_in_machs_macehs();

      if(((tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_HS_TO_EHS )
        ||(tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_EHS_TO_HS ))
        && (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL8:MAC-hs <-> MAC-ehs transition type %d,But MAC-reset ind is %d!",tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type,tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
        return FAILURE;
      }
      
#else /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = tdsrrc_get_hsdpa_action();
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

      /*Validation check for 64 QAM and HSDPA type. 64 QAM is supported for MAC-ehs only right now.*/
#ifdef FEATURE_TDSCDMA_64QAM
      if((tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE) &&
        ((tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_START) ||
        (tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_RECONFIG) ||
        ((tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_NOOP) &&
        (tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE))))
      {
        if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:64QAM cannot be started on MAC-HS.");
          return FAILURE;
        }
      }
#endif /*FEATURE_TDSCDMA_64QAM*/
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:HSDPA Action %d set in MAC",tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Error in determining HSDPA action!");  
      return FAILURE;
    }

    /*By now HSDPA action and type of HSDPA (hs or ehs) should have been decided*/
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if((TDSRRC_CHECK_MSG_TYPE(dl_hspdsch_info_ptr->modeSpecificInfo, 
        T_tdsrrc_DL_HSPDSCH_Information_r8_modeSpecificInfo_fdd))
        && (TDSRRC_CHECK_MSG_TYPE_IE_PTR(dl_hspdsch_info_ptr->modeSpecificInfo.u.fdd, 
             hs_DSCH_TBSizeTablePresent)))
    {
      tdsrrcllcpcie_update_hs_tfri_table_type(TRUE);
    }
    else
    {
         /*if the whole Downlink HS-PDSCH information IE is missing we should retain 
            the older TFRI table but if they send the IE with other parameters but not the 
            table info, we should switch to bit aligned as per section 10.3.6.23a
            Also if hs_64_qam_configured is TRUE we should set to octet aligned 
         */
         
      if((dl_hspdsch_info_present)
#ifdef FEATURE_TDSCDMA_64QAM
         || (tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE )
#endif
         || (tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS)
        )
      {
      tdsrrcllcpcie_update_hs_tfri_table_type(FALSE);
      }
    }
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
  } /*only if the next state is Cell_DCH*/

  return(SUCCESS);
}

#ifdef FEATURE_TDSCDMA_MIMO
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_r8()

DESCRIPTION:
  This function updates IE "Mimo Params" in ordered config for R8 OTAs.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type    tdsrrcllcpcie_process_mimo_params_r8
(
  tdsrrc_MIMO_Parameters_r8 * mimo_params,
  tdsrrc_RRC_StateIndicator rrc_state,
  uint32 mimo_params_present_ota
)
{
  tdshsdpa_action_enum_type hs_action =tdsrrc_get_hsdpa_action();
  boolean mimo_reconfig = FALSE;

  /*Make sure that HSDPA category supports MIMO and (either HSDPA is being started or reconfig) */
  if((TDSCHECK_WCDMA_OPTIONAL_FEATURE_MIMO_SUPPORTED) && ((hs_action == TDSHSDPA_START) ||
        ((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) && (hs_action != TDSHSDPA_STOP))))
  {
    tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_NOOP;
    if( mimo_params_present_ota )
    {
      if(TDSRRC_CHECK_MSG_TYPE(mimo_params->modeSpecificInfo, 
                   T_tdsrrc_MIMO_Parameters_r8_modeSpecificInfo_fdd))
      {
        if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(mimo_params->modeSpecificInfo.u.fdd, 
                   mimoN_M_RatioPresent))
        {
          if(tdsrrcllcpcie_update_n_m_ratio(mimo_params->modeSpecificInfo.u.fdd->mimoN_M_Ratio) == FAILURE)
          {
            return FAILURE;
          }
          mimo_reconfig = TRUE;
        }
    
        if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(mimo_params->modeSpecificInfo.u.fdd, 
                  mimoPilotConfigurationPresent))
        {
          mimo_reconfig = TRUE;
          if(TDSRRC_CHECK_MSG_TYPE(mimo_params->modeSpecificInfo.u.fdd->mimoPilotConfiguration.secondCPICH_Pattern, 
                   T_tdsrrc_MIMO_PilotConfiguration_secondCPICH_Pattern_normalPattern))
        {
           tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern = TDSL1_MIMO_NORMAL_PATTERN;
           TDSRRC_MSG0(MSG_LEGACY_LOW,"MIMO: MIMO normal cipch pattern ");
        }
        else if(TDSRRC_CHECK_MSG_TYPE(mimo_params->modeSpecificInfo.u.fdd->mimoPilotConfiguration.secondCPICH_Pattern, 
                   T_tdsrrc_MIMO_PilotConfiguration_secondCPICH_Pattern_diversityPattern))
        {
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern = TDSL1_MIMO_DIVERSITY_PATTERN;
   
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.channelisation_code = 
                  mimo_params->modeSpecificInfo.u.fdd->mimoPilotConfiguration.secondCPICH_Pattern.u.diversityPattern->channelisationCode;
          TDSRRC_MSG1(MSG_LEGACY_LOW,"MIMO: MIMO diversity cipch pattern chan code %d",tdsordered_config_ptr->l1_hsdpa_info.mimo_info.channelisation_code);
          }
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: TDD mode not supported");
        return FAILURE;
      }  
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:MIMO params not present in OTA");
    }  
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"REL8:MIMO can't be started HS-Action %d HS-Status %d NV HS-CAT %d",
            hs_action,tdsrrc_get_hsdpa_status(),tdsnv_hsdpa_category);
    mimo_params_present_ota = 0;
  }
   
  if((!mimo_params_present_ota) ||
    (tdsrrcllc_check_if_mimo_can_be_started(rrc_state) == FALSE ))
  {
    if(tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_ACTIVE )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:Set MIMO Action to STOP");
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_STOP;
      if(hs_action != TDSHSDPA_STOP)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Include HS bitmask as MIMO is going be stopped,but HS continues");

        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL |TDSL1_CPHY_HS_DSCH_CFG_INCL|TDSL1_CPHY_HS_DPCCH_CFG_INCL;;

          /* Set beta table and hs-dpcch bit mask */
        tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.info_present_bitmask = 
            HS_DPCCH_BETA_HS_INFO_PRESENT_BITMASK | HS_DPCCH_MEAS_FEEDBACK_INFO_PRESENT_BITMASK;
        if(hs_action == TDSHSDPA_NOOP)
        {
          tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
          tdsrrcllc_set_hsdpa_action_in_machs_macehs();
#endif
        }
           
      }
      else
      {
        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:Set MIMO Action to NOOP");
      tdsrrcllc_init_mimo_params(tdsordered_config_ptr);
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_NOOP;
    }
  }
  else
  {
    uint8 num_harq_proc =  tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc;
    /*If the IE “Number of processes" is not set to one of the values 12, 14 or 16 and the IE “MIMO
        operation(10.3.6.41a) is included and set to either “startor “continue the UE behaviour is
        unspecified.
      */
    if((num_harq_proc == 12) || (num_harq_proc == 14) || (num_harq_proc == 16))
    {
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO: No. Of harq process is %d as according to MIMO ",num_harq_proc);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MIMO: No. Of harq process is %d is not suitable for MIMO ",num_harq_proc);
      return FAILURE;
    }
    
    if(mimo_params->mimoOperation == tdsrrc_MIMO_Operation_start )
    {
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_START;      
      
      tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL|TDSL1_CPHY_HS_DSCH_CFG_INCL|TDSL1_CPHY_HS_DPCCH_CFG_INCL;

      /* Set beta table and hs-dpcch bit mask */
      tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.info_present_bitmask = 
            HS_DPCCH_BETA_HS_INFO_PRESENT_BITMASK | HS_DPCCH_MEAS_FEEDBACK_INFO_PRESENT_BITMASK;
      
      if(hs_action != TDSHSDPA_START)
      {
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
        tdsrrcllc_set_hsdpa_action_in_machs_macehs();
#endif
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO: MIMO params received with action start");
    }
    else if(mimo_params->mimoOperation == tdsrrc_MIMO_Operation_continue_ )
    {
      tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_NOOP;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO: MIMO params received with action continue");
      if(mimo_reconfig ==TRUE)
      {
        if(tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_INACTIVE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR," MIMO not active so MIMO Operation should be rrc_start,but received as rrc_continue  ");
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_START;
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_RECFG;
        }
        
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"MIMO: MIMO action reconfig ,second_cpich_pattern  %d chan code %d M/N ratio %d",
             tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern,
             tdsordered_config_ptr->l1_hsdpa_info.mimo_info.channelisation_code,
             tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio);
        
        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL |TDSL1_CPHY_HS_DSCH_CFG_INCL|TDSL1_CPHY_HS_DPCCH_CFG_INCL;;

        /* Set beta table and hs-dpcch bit mask */
        tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.info_present_bitmask = 
            HS_DPCCH_BETA_HS_INFO_PRESENT_BITMASK | HS_DPCCH_MEAS_FEEDBACK_INFO_PRESENT_BITMASK;
          
         if(hs_action != TDSHSDPA_START)
         {
           tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
           tdsrrcllc_set_hsdpa_action_in_machs_macehs();
#endif
         }
      }
      else
      {
        if((tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask & TDSL1_CPHY_HS_DSCH_CFG_INCL)
            &&(tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_ACTIVE))
        {
          /*This is as per L1's requirement that when MIMO is active, even if No MIMO params change
           * if HS-DSCH changes then indicate MIMO as RECONFIG*/
          
          tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL;
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_RECFG;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO: No new MIMO params but HSDSCH touched so MIMO action reconfig HS bitmask 0x%x",tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);
        }
        else if((tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask & TDSL1_CPHY_HS_PDSCH_RL_CFG_INCL) && 
          (tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_ACTIVE))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_MIMO_CFG_INCL |TDSL1_CPHY_HS_DSCH_CFG_INCL;
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_RECFG;        
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:HS Re-pointing so RECFGing MIMO");
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO: No new MIMO params received so action NOOP");
        }
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MIMO: Invalid MIMO Operation %d",mimo_params->mimoOperation);
      return FAILURE;
    }
  }
   
  return SUCCESS;
}
#endif /* FEATURE_TDSCDMA_MIMO */


#ifdef FEATURE_TDS_DC_HSDPA
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dc_hsdpa_params()

DESCRIPTION:
  This function processes IE "DL_SecondaryCellInfoFDD" content in ordered config for R8 OTAs. Basically,
  this function updates the OC with the IE content and does some valiadation.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dc_hsdpa_params
(
  tdsrrc_DL_SecondaryCellInfoFDD * dc_hsdpa_params,
  tdsrrc_RRC_StateIndicator rrc_state,
  uint32 dc_hsdpa_params_present_ota
)
{
  tdshsdpa_action_enum_type hs_action =tdsrrc_get_hsdpa_action();
  tdsrrc_DL_SecondaryCellInfoFDD_hS_SCCHChannelisationCodeInfo * hsscch_list_ptr = NULL;
  uint32 ix=0;
  uint8 cnt = 0;

    if((dc_hsdpa_params_present_ota) &&(!(TDSCHECK_WCDMA_OPTIONAL_FEATURE_DC_HSDPA_SUPPORTED)))
    {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA config can't be processed for NV HS-CAT %d.",tdsnv_hsdpa_category);
        return FAILURE;
    }

  /*By this time, HSDPA action should have been determined.*/
  /*Make sure that HSDPA category supports DC-HSDPA and (either HSDPA is being started or reconfig) */
  if((TDSCHECK_WCDMA_OPTIONAL_FEATURE_DC_HSDPA_SUPPORTED) && ((hs_action == TDSHSDPA_START) ||
    ((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) && (hs_action != TDSHSDPA_STOP))))
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action = TDSL1_DC_HSDPA_NOOP;

    if(dc_hsdpa_params_present_ota)
    {
      if(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(dc_hsdpa_params, rrc_DL_SecondaryCellInfoFDD_continue_))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:Continue Signaled in OTA.");
      }
      else if(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(dc_hsdpa_params, rrc_DL_SecondaryCellInfoFDD_newConfiguration))
      {
        if(tdsrrc_translate_hrnti(&dc_hsdpa_params->u.newConfiguration->new_H_RNTI, 
          &tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.h_rnti_sec_cell) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"DC-HSDPA:HRNTI Invalid!");
          return FAILURE;
        }

        tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.pri_cpich_scr_code = 
          (uint16)dc_hsdpa_params->u.newConfiguration->primaryCPICH_Info.primaryScramblingCode;

        tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.num_ch_code = 0;

        hsscch_list_ptr = &dc_hsdpa_params->u.newConfiguration->hS_SCCHChannelisationCodeInfo;
        ix= 0;;
        while((ix <  hsscch_list_ptr->n) && (cnt < TDSL1DLPHYCFG_HS_SCCH_CODES_MAX))
        {
          /* Spec allows allows channelisation code 0 also but since MSM6275 will not support secondary scrambling code, 
               it will collide with OVSF 0 and 1 on SF 256 for CPICH and PCCPCH channels. */
          /*RRC_GKG: Confirm whether this limitation still exists*/
          if((hsscch_list_ptr->elem[ix] > TDSRRCLLC_MAX_SCCH_CHAN_CODE) || (hsscch_list_ptr->elem[ix] == 0))
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"DC-HSDPA:Invalid HS-SCCH Code %d",hsscch_list_ptr->elem[ix]);
            return FAILURE;
          }
          tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.ch_code[cnt++] = 
            (uint8)hsscch_list_ptr->elem[ix];
          ix++;
        }
        tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.num_ch_code = cnt;

        tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.po_hs_dsch_sec_cell = 
          dc_hsdpa_params->u.newConfiguration->measurementPowerOffset;

        tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_dl_freq = 
          dc_hsdpa_params->u.newConfiguration->uarfcn_DL;
#ifdef FEATURE_TDSCDMA_64QAM

        if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(dc_hsdpa_params->u.newConfiguration, dl_64QAM_ConfiguredPresent))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_64_qam_configured = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_LOW,"DC-HSDPA:64QAM provided for Supplementary Cell.");
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_64_qam_configured = FALSE;
        }
#endif
        if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(dc_hsdpa_params->u.newConfiguration, hs_DSCH_TBSizeTablePresent))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_tb_size_alignment_type = 
            TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_ALIGNED;
          TDSRRC_MSG0(MSG_LEGACY_LOW,"DC-HSDPA:Octet Aligned TB Size provided.");
        }
        else
        {
#ifdef FEATURE_TDSCDMA_64QAM

          if(tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_64_qam_configured)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:Forcing Octet Aligned bcoz of 64QAM");
            tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_tb_size_alignment_type = 
              TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_ALIGNED;
          }
          else
#endif
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_tb_size_alignment_type = 
              TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;
          }
        }
        if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:MAC header type is MAC-hs,Set Bit-Aligned");
            tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_tb_size_alignment_type = 
              TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;          
        }
        if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(dc_hsdpa_params->u.newConfiguration, dl_ScramblingCodePresent))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.sec_scr_code_valid = TRUE;
          tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.sec_cpid = 
            dc_hsdpa_params->u.newConfiguration->dl_ScramblingCode;
          TDSRRC_MSG0(MSG_LEGACY_LOW,"DC-HSDPA:SSC provided for Supplementary Cell.");
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.sec_scr_code_valid = FALSE;
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"DC-HSDPA:Incorrect choice %d in OTA!",TDSRRC_GET_MSG_TYPE_PTR(dc_hsdpa_params));
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:Params not present in the OTA.");
    }
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"DC-HSDPA:Can't be started coz HS-Action %d HS-Status %d NV HS-CAT %d",
            hs_action,tdsrrc_get_hsdpa_status(),tdsnv_hsdpa_category);
    dc_hsdpa_params_present_ota = 0;
  }

  if(tdsrrcllcpcie_update_l1_action_for_dc_hsdpa_params(dc_hsdpa_params,
    rrc_state, dc_hsdpa_params_present_ota) == FALSE)
  {
    return FAILURE;
  }
  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_update_l1_action_for_dc_hsdpa_params()

DESCRIPTION:
  This function updates L1 action/configuration for DC-HSDPA. By this time, OC is already set with
  DC-HSDPA content.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllcpcie_update_l1_action_for_dc_hsdpa_params
(
  tdsrrc_DL_SecondaryCellInfoFDD * dc_hsdpa_params,
  tdsrrc_RRC_StateIndicator rrc_state,
  uint32 dc_hsdpa_params_present_ota  
)
{
  tdshsdpa_action_enum_type hs_action =tdsrrc_get_hsdpa_action();

  /*The first check for the presence of DC-HSDPA params is done to check whether OTA is trying
  to stop DC-HSDPA if already started.*/
  /*The second check is for the case when DC-HSDPA params are given in the OTA yet it can't be
  started because of validation issues.*/
  if((!dc_hsdpa_params_present_ota) ||
    (tdsrrcllc_check_if_dc_hsdpa_can_be_started(rrc_state) == FALSE))
  {
    /*If DC-HSDPA is already active, we need to stop it.*/
    if(tdsrrcllc_get_dc_hsdpa_status() == TDSRRC_DC_HSDPA_ACTIVE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:Set the L1 action to STOP. hs_req_mask 0X%x",tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);

      tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action = TDSL1_DC_HSDPA_STOP;

      /*If HSDPA action is STOP, then no need to set any info/action for DC-HSDPA. Because DC-HSDPA
      makes sense only if HSDPA is up. This handles the case when HSDPA continues but not DC-HSDPA.*/
      if(hs_action != TDSHSDPA_STOP)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:HSDPA Rx ON so Set hs_req_mask for Dual Cell.");

        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_DC_HSDPA_CFG_INCL |TDSL1_CPHY_HS_DSCH_CFG_INCL|
          TDSL1_CPHY_HS_DPCCH_CFG_INCL;

         /* Set beta table and hs-dpcch bit mask */
        tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.info_present_bitmask = 
            HS_DPCCH_BETA_HS_INFO_PRESENT_BITMASK | HS_DPCCH_MEAS_FEEDBACK_INFO_PRESENT_BITMASK;

        /*If nothing is being changed for normal HSDPA, changing the HSDPA action to RECONFIG so that DC-HSDPA can be stopped.*/
        /*RRC_GKG: By doing so, MAC also may get RECONFIG*/
        if(hs_action == TDSHSDPA_NOOP)
        {
          tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
          tdsrrcllc_set_hsdpa_action_in_machs_macehs();
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
        }
      }
      /*It means that HSDPA is being stopped, we can simply add the DC-HS mask, though DC-HS will be stopped
      anyway because HSDPA will be stopped.*/
      else
      {
        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_DC_HSDPA_CFG_INCL;
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:Set L1 action to NOOP. hs_req_mask 0X%x",tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);
      tdsrrcllc_init_dc_hsdpa_params(tdsordered_config_ptr);
      tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action = TDSL1_DC_HSDPA_NOOP;
    }
  }
  /*Coming here means that DC-HSDPA is being started/reconfig*/
  else
  {
    if(tdsrrcllc_validate_dc_hsdpa_config() == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:Validation failed for DC-HSDPA!");
      return FALSE;
    }

    if(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(dc_hsdpa_params, rrc_DL_SecondaryCellInfoFDD_continue_))
    {
      if(tdsrrcllc_get_dc_hsdpa_status() != RRC_DC_HSDPA_ACTIVE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:DL_SecondaryCellInfoFDD_continue_chosen when DC is Inactive!");
        return TRUE;
      }
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:OTA is trying to CONTINUE. hs_req_mask 0X%x",tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);

      if(tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask & TDSL1_CPHY_HS_DSCH_CFG_INCL)
      {
        tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_DC_HSDPA_CFG_INCL;
        tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action = TDSL1_DC_HSDPA_RECFG; 

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:Setting the HS Action from %d to RECONFIG",hs_action);
        /*RRC_GKG: Consider the case when HSDPA action is already set to STOP
        Ideally, in that case, we wouldn't come upto this point.*/
        /*RRC_GKG: In case HSDPA action is NOOP, MAC may get RECONFIG due to forcing it*/
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
        tdsrrcllc_set_hsdpa_action_in_machs_macehs();
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      }
    }
    else /*It means OTA has given new Config. */
    /*RRC_GKG: We don't check each IE against its old elem[n_index]. It can be done for Optimizations.*/
    {
      if(tdsrrcllc_get_dc_hsdpa_status() == TDSRRC_DC_HSDPA_INACTIVE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:OTA is trying to START. hs_req_mask 0X%x",tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);

        tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action = TDSL1_DC_HSDPA_START;    
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:OTA is trying to RECONFIG. hs_req_mask 0X%x",tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);

        tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action = TDSL1_DC_HSDPA_RECFG;        
      }

      tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask |= TDSL1_CPHY_HS_DC_HSDPA_CFG_INCL |TDSL1_CPHY_HS_DSCH_CFG_INCL|
        TDSL1_CPHY_HS_DPCCH_CFG_INCL;
  
        /* Set beta table and hs-dpcch bit mask */
      tdsordered_config_ptr->l1_hsdpa_info.hs_dpcch_info.info_present_bitmask = 
        HS_DPCCH_BETA_HS_INFO_PRESENT_BITMASK | HS_DPCCH_MEAS_FEEDBACK_INFO_PRESENT_BITMASK;

      if(tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:Setting the HS Action from %d to RECONFIG",hs_action);

        /*RRC_GKG: Consider the case when HSDPA action is already set to STOP
        Ideally, in that case, we wouldn't come upto this point.*/
        /*RRC_GKG: In case HSDPA action is NOOP, MAC may get RECONFIG due to forcing it*/
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
        tdsrrcllc_set_hsdpa_action_in_machs_macehs();
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      }
    }
  }
  return TRUE;
}
#endif /*FEATURE_TDSCDMA_DC_HSDPA*/

#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
/*====================================================================
FUNCTION      tdsrrcllcpcie_set_hsscch_info_r9()

DESCRIPTION   This function sets the Channelization code required
              for HSPDSCH in L1 structure in OC. IEs refer to 10.3.6.36a.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_set_hsscch_info_r9
(
  tdsrrc_HS_SCCH_Info_r9* hsscch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg
#endif
)
{
  struct tdsrrc_HS_SCCH_Info_r9_tdd128 *tmp_ptr = NULL;
  uint8 i = 0;

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  if (multicarrier_flg)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MultiCarrier present,ignore hsscch info");
    return SUCCESS;
  }
#endif

  if (TDSRRC_CHECK_MSG_TYPE(hsscch_info_ptr->modeSpecificInfo,
        T_tdsrrc_HS_SCCH_Info_r9_modeSpecificInfo_tdd))
  {
    /* Get HSSCCH info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(hsscch_info_ptr->modeSpecificInfo.u.tdd, rrc_HS_SCCH_Info_r9_tdd_tdd128))
    {
       tmp_ptr = hsscch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128;
       
       /** Set common_scch_sich_params_present info 
                TRUE indicate the current IE using shared ck_nack_powoffset,etc */
       tdsordered_config_ptr->l1_hsdpa_info.common_scch_sich_params_present = TRUE;

       /* Get Ack-Nack Power Offset info(R6) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.ack_nack_powoffset =
         tmp_ptr->nack_ack_power_offset;

       /* Get PRXHS-SICH info(R6) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.prx_sich =
          tmp_ptr->power_level_HSSICH;

       /* Get TPC step size(R6) info */
       if (tmp_ptr->tpc_step_size < tdsrrc_HS_SCCH_Info_r9_tpc_step_size_spare1)
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize =
            tmp_ptr->tpc_step_size+1;
       }
       else
       {
         TDSRRC_MSG0(MSG_LEGACY_LOW,"TPC Step Size is absent in OTA message");
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize = TDSL1_TPC_STEP_SIZE_0DB;
       }

       /* Get BLER target(R6) info */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.bler_target =
          tmp_ptr->bler_target;
       TDSRRC_MSG1(MSG_LEGACY_MED,"Set bler_target = %d",tmp_ptr->bler_target);

       /* hS-SCCH-tpc-step-size(R8) */
       if (tmp_ptr->m.hS_SCCH_tpc_step_sizePresent)
       {
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport IE hS SCCH tpc step size reveived");
         return FAILURE;
       }

       /* Get Power Control GAP(R7) info */
       if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ptr, powerControlGAP))
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
          tmp_ptr->powerControlGAP;
       }
       else
       {
         /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
       }

       /* Get Pathloss compensation switch(R7) info */
       if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ptr,
            pathlossCompensationSwitch))
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
            tmp_ptr->pathlossCompensationSwitch;
       }
       else
       {
         if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ptr, powerControlGAP))
         {
           TDSRRC_MSG0(MSG_LEGACY_LOW,"Set the do_pathloss_compens to 1,because powerControlGAP IE exists");
           tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = TRUE;
         }
         else
         {
           /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */
           tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
         }
       }

       /* Get HS-SCCH Set Configuration list(R5) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich =
         tmp_ptr->hS_SCCH_SetConfiguration.n;

       for (i = 0; i < tmp_ptr->hS_SCCH_SetConfiguration.n; i++)
       {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].ts_num_scch =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].timeslotNumber;

          /** OC: scch_chcode_1st = 0, L1_SF16_CH_CODE_1;
                    ASN1: firstChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_1st =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].firstChannelisationCode;

          /** OC: scch_chcode_2nd = 0, L1_SF16_CH_CODE_1;
                     ASN1: secondChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_2nd =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].secondChannelisationCode;

          /* Get midamble info */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.allocation_mode =
            tdsMidambleAllocMode[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode.t];

          if (TDSRRC_CHECK_COMMON_MSG_TYPE(tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode,
                rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.shift =
                tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode.u.ueSpecificMidamble;
          }

          /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
                     ASN1: midambleConfiguration   Integer (1...8), */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.config =
            tdsMidambleConfiguration[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleConfiguration];

          /* Get HS-SICH configuration info */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.ts_num_sich =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.timeslotNumber;

          /* OC: sich_chcode = 0, L1_SF16_CH_CODE_1;
                     ASN1: channelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1*/
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.sich_chcode =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.channelisationCode;

          /* Get midamble info */
          switch (tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleAllocationMode.t)
          {
            case T_tdsrrc_HS_SICH_Configuration_TDD128_r6_midambleAllocationMode_defaultMidamble:
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
                  TDSL1_DEFAULT_MIDAMBLE;
                break;
          
            case T_tdsrrc_HS_SICH_Configuration_TDD128_r6_midambleAllocationMode_ueSpecificMidamble:
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
                  TDSL1_UE_SPECIFIC_MIDAMBLE;
          
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.shift =
                  tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
                break;
            default:
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"hs_sich_configuration.midambleAllocationMode is invalid");
              return FAILURE;
          }

          /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
                     ASN1: midambleConfiguration   Integer (1...8), */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.config =
            tdsMidambleConfiguration[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleConfiguration];
       }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd choice is not tdd128");
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo choice FDD,not TDD");
    return FAILURE;
  }
  return SUCCESS;

}
/*====================================================================
FUNCTION      tdsrrcllcpcie_check_if_hsscch_changed_r9()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllcpcie_check_if_hsscch_changed_r9
(
  tdsrrc_HS_SCCH_Info_r9* scch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg
#endif
)
{
  uint8 cnt = 0;
  tdsrrc_HS_SCCH_Info_r9_tdd128   *local_scch_ptr = NULL;

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  if (multicarrier_flg)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MultiCarrier present,don't check hsscch");
    return SUCCESS;
  }
#endif

  if (TDSRRC_CHECK_MSG_TYPE(scch_info_ptr->modeSpecificInfo,
               T_tdsrrc_HS_SCCH_Info_r9_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:FDD selected. Bail out");
    return FALSE;
  }
  if (!TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(scch_info_ptr->modeSpecificInfo.u.tdd,
               rrc_HS_SCCH_Info_r9_tdd_tdd128))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:not TDD128 selected. Bail out");
    return FALSE;
  }

  local_scch_ptr = (tdsrrc_HS_SCCH_Info_r9_tdd128*)
                    scch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128;

  /* Check nack_ack_power_offset info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.ack_nack_powoffset !=
      local_scch_ptr->nack_ack_power_offset)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"nack_ack_power_offset is different");
    return TRUE;
  }

  /* Check PRXHS-SICH info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.prx_sich !=
      local_scch_ptr->power_level_HSSICH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PRXHS-SICH is different");
    return TRUE;
  }

  /* Check TPC step size info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize !=
      (local_scch_ptr->tpc_step_size+1))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TPC step size is different");
    return TRUE;
  }

  /* Check BLER target info same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.bler_target !=
      local_scch_ptr->bler_target)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BLER target is different");
    return TRUE;
  }

  /* hS-SCCH-tpc-step-size(R8) */
  if (local_scch_ptr->m.hS_SCCH_tpc_step_sizePresent)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport IE hS SCCH tpc step size reveived");
    return FAILURE;
  }


  /* Check Power Control GAP info same or not */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(local_scch_ptr, powerControlGAP))
  {
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap !=
        local_scch_ptr->powerControlGAP)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Power Control GAP is different");
      return TRUE;
    }
  }
  else
  {
    /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
    if (1 != tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Power Control GAP is different");
      return TRUE;
    }
  }

  /* Check Pathloss compensation switch same or not */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(local_scch_ptr, pathlossCompensationSwitch))
  {
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens !=
        local_scch_ptr->pathlossCompensationSwitch)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pathloss compensation switch is different");
      return TRUE;
    }
  }
  else
  {
    /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */
    if (FALSE != tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pathloss compensation switch is different");
      return TRUE;
    }
  }

  /* Check HS-SCCH Set Configuration list same or not */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich !=
        local_scch_ptr->hS_SCCH_SetConfiguration.n)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"l1_dl_hs_scch_sich_config num is different");
    return TRUE;
  }

  for (cnt = 0; cnt < local_scch_ptr->hS_SCCH_SetConfiguration.n; cnt++)
  {
    /* Check timeslotNumber same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].ts_num_scch !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"timeslotNumber is different");
      return TRUE;
    }

    /* Check firstChannelisationCode same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_1st !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].firstChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"firstChannelisationCode is different");
      return TRUE;
    }

    /* Check secondChannelisationCode same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_2nd !=
        local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].secondChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"secondChannelisationCode is different");
      return TRUE;
    }

    /* Check midamble info same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.allocation_mode !=
          tdsMidambleAllocMode[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"midambleAllocationMode is different");
      return TRUE;
    }

    /* Check midamble shift same or not */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode,
      rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.shift !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleAllocationMode.u.ueSpecificMidamble)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"midamble_shift is different");
        return TRUE;
      }
    }

    /* Check midamble cfg same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.config !=
          tdsMidambleConfiguration[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"midambleConfiguration is different");
      return TRUE;
    }

    /* Check sich config same or not */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.ts_num_sich !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration timeslotNumber is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.sich_chcode !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.channelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration channelisationCode is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.allocation_mode !=
          tdsMidambleAllocMode[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midambleAllocationMode is different");
      return TRUE;
    }

    if (TDSRRC_CHECK_COMMON_MSG_TYPE(local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode,
      rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.shift !=
          local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midamble_shift is different");
        return TRUE;
      }
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.config !=
          tdsMidambleConfiguration[local_scch_ptr->hS_SCCH_SetConfiguration.elem[cnt].hs_sich_configuration.midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hs_sich_configuration midambleConfiguration is different");
      return TRUE;
    }
  }

  return FALSE;
}

/*============================================================================
FUNCTION: rrcllcpcie_process_dl_hspdsch_info_and_update_hsdpa_msg_params_r9()

DESCRIPTION:
  This function updates IE "dl hspdsch info " in ordered config and update
  hsdpa_msg_param variable
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_hspdsch_info_and_upd_hsdpa_msg_params_r9
(
  uint32 dl_hspdsch_info_present,
  tdsrrc_DL_HSPDSCH_Information_r9 *dl_hspdsch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /*We need to process HSDPA related IE DL_HSPDSCH_Information only in Cell_DCH state.*/
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    /* If InterF HO is happening, then set the flags in tdshsdpa_msg_params appropriately.*/
    if (tdsordered_config_ptr->is_hho)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:HHO is happening thru OTA.");
      tdshsdpa_msg_params.msg_params_present = TRUE;
    }

    /*If IE tdsrrc_DL_HSPDSCH_Information_r8 is present.*/
    if(dl_hspdsch_info_present)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"REL9:DL HS-PDSCH Info present.");
      tdshsdpa_msg_params.msg_params_present = TRUE;
      tdshsdpa_msg_params.hspdsch_info_present = TRUE;
      
      if(TDSRRC_CHECK_MSG_TYPE(dl_hspdsch_info_ptr->modeSpecificInfo, 
                  T_tdsrrc_DL_HSPDSCH_Information_r9_modeSpecificInfo_fdd))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:FDD mode chosen for HSPDSCH Information");
        return FAILURE;
      }
      
#ifdef FEATURE_TDSCDMA_64QAM
      /*If 64 QAM status is being changed by the OTA, then set the flag hsdsch_info_present in tdshsdpa_msg_params*/
      if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured != 
        tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured)
      {
        tdshsdpa_msg_params.hsdsch_info_present = TRUE;

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL9:Set HSDSCH bitmask for 64QAM. Before %d,current %d (0:Not present/1:Present)",tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured ,tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured);
      }
#endif /*FEATURE_TDSCDMA_64QAM*/

    }

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if(tdsrrcllc_set_macehs_or_machs_and_check_if_coexist()== TRUE)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL9:MAC-ehs %d and MAC-hs %d cant co-exist ",tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue,tdsordered_config_ptr->mac_dl_parms.num_dflow);
      return FAILURE;
    }
    tdsrrcllc_set_machs_ehs_transition();
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

    /* Set HSDPA Action & Physical Channel Parameters associated with it */
    if (tdsrrcllc_determine_hsdpa_action_r9(dl_hspdsch_info_ptr) == SUCCESS)
    {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_set_hsdpa_action_in_machs_macehs();

      if(((tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_HS_TO_EHS )
        ||(tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_EHS_TO_HS ))
        && (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL8:MAC-hs <-> MAC-ehs transition type %d,But MAC-reset ind is %d!",tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type,tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
        return FAILURE;
      }
      
#else /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = tdsrrc_get_hsdpa_action();
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

      /*Validation check for 64 QAM and HSDPA type. 64 QAM is supported for MAC-ehs only right now.*/
#ifdef FEATURE_TDSCDMA_64QAM
      if((tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE) &&
        ((tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_START) ||
        (tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_RECONFIG) ||
        ((tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_NOOP) &&
        (tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE))))
      {
        if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:64QAM cannot be started on MAC-HS.");
          return FAILURE;
        }
      }
#endif /*FEATURE_TDSCDMA_64QAM*/
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Error in determining HSDPA action!");  
      return FAILURE;
    }
  } /*only if the next state is Cell_DCH*/

  return(SUCCESS);
}
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_non_sched_info_r9()

DESCRIPTION:
  This function updates IE "Non-scheduled transmission grant info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_process_non_sched_info_r9
(
  uint32 e_non_sched_info_present,
  tdsrrc_Non_ScheduledTransGrantInfoTDD_r9 *e_non_si_ptr
)
{
  if (e_non_sched_info_present == TRUE)
  {
    if (e_non_si_ptr == NULL)
    {
      ERR_FATAL("Null pointer received",0,0,0);
      return FAILURE;
    }
    
    if (e_non_si_ptr->t == T_tdsrrc_Non_ScheduledTransGrantInfoTDD_r9_tdd384_768)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Non-SI unsupport mode TDD384 and TDD768,failure");
      return FAILURE;
    }
    /* Number of EUCCH and TPC instances within an E-DCH TTI. Default = 1. */
    if (e_non_si_ptr->u.tdd128->m.n_E_UCCHPresent)
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.num_e_ucch = 
          e_non_si_ptr->u.tdd128->n_E_UCCH;
    }
    else
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.num_e_ucch = TDSRRC_DEFAULT_N_E_UCCH;
    }
    
    /* Minimum number of slots between start last active slot of E-DCH TTI and start of ACK/NACK on EHICH. */
    if (e_non_si_ptr->u.tdd128->m.n_E_HICHPresent)
    {
      /* Invalid N-E-HICH in scheduled E-PUCH information */
      if (tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich == 0)
      {
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich = 
          e_non_si_ptr->u.tdd128->n_E_HICH;
      }
      else
      {
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich = 
           tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"N-E-HICH present non_sched,but use sched value = %d",tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich);
      }
    }
    else
    {
      /* Invalid N-E-HICH in scheduled E-PUCH information */
      /* If NE-HICH is given in scheduled E-PUCH information, this IE is not needed, otherwise it is mandatory */
      if (tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich == 0)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR," N-e-hich not match CV condition ");
        return FAILURE;
      }
      else
      {
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich = 
           tdsordered_config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"N-E-HICH absent non_sched,so use sched value = %d",tdsordered_config_ptr->l1_e_info.non_sched_grant_info.n_e_hich);
      }
    }

    /* Bitmask  indicating  which of the Timeslots configured for EDCH are allocated for non-scheduled transmissions */
    if (e_non_si_ptr->u.tdd128->timeslotResourceRelatedInfo.numbits != TDSRRC_MAX_NUMBITS_5)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received timeslotResourceRelatedInfo Length: %d is invalid",e_non_si_ptr->u.tdd128->timeslotResourceRelatedInfo.numbits);
      return FAILURE;
    }
    else
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.trri = 
         e_non_si_ptr->u.tdd128->timeslotResourceRelatedInfo.data[0] >> 3;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"trri in nonsched is %d",tdsordered_config_ptr->l1_e_info.non_sched_grant_info.trri);
    }

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.prri = 
          e_non_si_ptr->u.tdd128->powerResourceRelatedInfo;

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.activation_time = 
          e_non_si_ptr->u.tdd128->activationTime;

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.subframe_num = 
          e_non_si_ptr->u.tdd128->sfnNum;
    
    if (e_non_si_ptr->u.tdd128->m.repetitionPeriodAndLengthPresent)
    {
      switch (e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.t)
      {
      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod1:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_1;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length = 0;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod2:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_2;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod2;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod4:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_4;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod4;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod8:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_8;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod8;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod16:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_16;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod16;
        break;
      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod32:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_32;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length=
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod32;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod64:
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_64;
        tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length =
              e_non_si_ptr->u.tdd128->repetitionPeriodAndLength.u.repetitionPeriod64;
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"repetitionPeriod is out of range");
        return FAILURE;
      } 
    }
    else
    {
      /* Default is "Repetition period" = 1 (continuous) */
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_period = TDSL1_REPEAT_PRD_1;
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.rep_length = 0;
    }
      

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.crri = 
          e_non_si_ptr->u.tdd128->codeResourceInfo;
    

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.ts_num = 
         e_non_si_ptr->u.tdd128->e_HICH_Info.timeslotNumber;

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.sig_group_index = 
         e_non_si_ptr->u.tdd128->e_HICH_Info.signatureSequenceGroupIndex;

    if (e_non_si_ptr->u.tdd128->e_HICH_Info.midambleAllocationMode.t == 
              T_tdsrrc_Non_ScheduledTransGrantInfoTDD_r9_midambleAllocationMode_defaultMidamble)
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.
           mid_code.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
    }
    else
    {
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.
          mid_code.allocation_mode = TDSL1_UE_SPECIFIC_MIDAMBLE;
      tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.mid_code.shift = 
          e_non_si_ptr->u.tdd128->e_HICH_Info.midambleAllocationMode.u.ueSpecificMidamble;
    }
    
    /* L1:Integer (2, 4, 6, 8, 10, 12, 14, 16);
        ASN1: midambleConfiguration   Integer (1...8), */
    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.mid_code.config =
          tdsMidambleConfiguration[e_non_si_ptr->u.tdd128->e_HICH_Info.midambleConfiguration];    

    tdsordered_config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.chan_code = 
         e_non_si_ptr->u.tdd128->e_HICH_Info.channelisation_Code;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting E-PUCH non-scheduled trans grant info success");

    
    if (e_non_si_ptr->u.tdd128->m.t_SI_nstPresent)
    {
      tdsordered_config_ptr->mac_e_config.eul_si_info.t_non_Sched_periodic_si_tmr = 
         tdsSITimer[e_non_si_ptr->u.tdd128->t_SI_nst];
    }
    /* Since L1 parameters are present hence update the bit mask to indicate the same */
    tdsordered_config_ptr->l1_e_req_mask |= TDSRRCLLC_NON_SCHED_GRANT_START;
    tdsrrcllc_update_mac_eul_action(TRUE);
  }

  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_e_dch_information_r9()

DESCRIPTION:
  This function updates IE "ul e dch information" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_e_dch_information_r9
(
  uint32 e_dch_info_present,
  tdsrrc_UL_EDCH_Information_r9 *e_dch_info_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  /* Look into this IE only if next UE state is DCH */
  if (e_dch_info_present && 
      (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
     )
  {
    if ((TDSRRC_CHECK_MSG_TYPE_IE_PTR(e_dch_info_ptr, 
               mac_es_e_resetIndicatorPresent)) &&
        (tdsrrc_UL_EDCH_Information_r9_mac_es_e_resetIndicator_true_ == e_dch_info_ptr->mac_es_e_resetIndicator))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"mac_es_e_resetIndicator set to TRUE");
      tdsordered_config_ptr->mac_e_reset_indicator = TRUE;
    }
    else
    {
      tdsordered_config_ptr->mac_e_reset_indicator = FALSE;
    }

    if(TDSRRC_CHECK_MSG_TYPE(e_dch_info_ptr->modeSpecificInfo, 
                  T_tdsrrc_UL_EDCH_Information_r9_modeSpecificInfo_tdd))
    {
      if (FAILURE == tdsrrcllcpcie_process_e_rucch_info(
           e_dch_info_ptr->modeSpecificInfo.u.tdd->m.e_RUCCH_InfoPresent, 
              &e_dch_info_ptr->modeSpecificInfo.u.tdd->e_RUCCH_Info))
      {
        return(FAILURE);
      }
      
      if (FAILURE == tdsrrcllcpcie_process_e_puch_info(
          e_dch_info_ptr->modeSpecificInfo.u.tdd->m.e_PUCH_InfoPresent, 
            &e_dch_info_ptr->modeSpecificInfo.u.tdd->e_PUCH_Info))
      {
        return(FAILURE);
      }

      if (FAILURE == tdsrrcllcpcie_process_non_sched_info_r9(
           e_dch_info_ptr->modeSpecificInfo.u.tdd->m.non_ScheduledTransGrantInfoPresent,
            &e_dch_info_ptr->modeSpecificInfo.u.tdd->non_ScheduledTransGrantInfo))
      {
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport mode FDD,failure");
      return FAILURE;
    }
  }

  return(SUCCESS);
}

#endif /*FEATURE_TDSCDMA_REL9*/

/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_r4()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r4
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r4 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
)
{
  boolean is_hho = FALSE;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_DL_InformationPerRL_List_r4 *tmp_ie_ptr = dl_info_per_rl_list_ptr;
  tdsrrc_state_e_type curr_rrc_state = tdsrrc_get_state();

  uint8               cpid = TDSRRC_INVALID_CPID;

  /* If state from CELL_DCH/CELL_FACH/CONNECTING to CELL_DCH */
  if (tdsrrc_RRC_StateIndicator_cell_DCH == rcvd_state_indicator)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"curr_rrc_state = %d",curr_rrc_state);
    cpid = (uint8)tdsrrcccm_get_curr_camped_cpid();
    /* If two FreqInfo are included in the OTA */
    if ((tdsordered_config_ptr->p_freq_present)&&(tdsordered_config_ptr->s_freq_present))
    {
      /*  If the first frequency got changed. */
      if (rrc_freq != tdsordered_config_ptr->p_freq)
      {
        /* It is a inter handover */
        is_hho = TRUE;
        tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different freq %d",tdsordered_config_ptr->p_freq);
      }
      if (FALSE == is_hho)
      {
        if (dl_info_per_rl_list_present)
        {
          if (tmp_ie_ptr == NULL)
          {
            ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
          }
          /*  If CPID got changed. */
          if ((T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd ==
                 tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
                 (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                     tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                     (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
          {
            if(cpid != tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
            {
              /* It is a intra handover */
              is_hho = TRUE;
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
            }
          }
        }
      }
    }
    /* If only one FreqInfo is included in the OTA */
    else if ((tdsordered_config_ptr->p_freq_present)||(tdsordered_config_ptr->s_freq_present))
    {
      if (dl_info_per_rl_list_present)
      {
        if (tmp_ie_ptr == NULL)
        {
          ERR_FATAL("dl_info_per_rl_list_ptr is NULL!",0,0,0);
        }
        /*  If CPID got changed. */
        if ((T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd ==
               tmp_ie_ptr->elem[0].modeSpecificInfo.t)&&
               (T_tdsrrc_PrimaryCCPCH_Info_r4_tdd == 
                   tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)&&
                   (tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          if(tdsrrcccm_get_curr_camped_cpid() != 
                tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
          {
            /* It is a handover */
            is_hho = TRUE;
            if (tdsordered_config_ptr->p_freq != rrc_freq)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a inter handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is a intra handover");
              tdsordered_config_ptr->hho_type = TDSRRC_INTRA_HHO;
            }
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO detected different cell para id %d",tmp_ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID);
          }
        }
      }
    }
#ifdef FEATURE_TDSCDMA_SINGLE_FREQ_NO_CPID_HO_SUPPORT

      if (tdsordered_config_ptr->p_freq_present)
      {
        /*  If the first frequency got changed. */
        if (rrc_freq != tdsordered_config_ptr->p_freq)
        {
          /* It is a inter handover */
          is_hho = TRUE;
          tdsordered_config_ptr->hho_type = TDSRRC_INTER_HHO;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Support none cpid and one freq info HO. p_freq %d different with current freq %d",tdsordered_config_ptr->p_freq,rrc_freq);
        }
      }

#endif
  }

  if (TRUE == is_hho)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO detected different freq different cell para id ");
  }
  tdsordered_config_ptr->is_hho = is_hho;

  return(SUCCESS);
}
/*====================================================================
FUNCTION      tdsrrcllc_get_ul_dpch_info_r4()

DESCRIPTION   This function translates the Uplink DPCH info common for
              all RL (radio links) IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_dpch_info_r4
(
  tdsrrc_UL_DPCH_Info_r4 *ie_ptr
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL DPCH Info");

  /* Get the UL DPCH power control information if present */
  if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                 ul_DPCH_PowerControlInfoPresent))
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_DPCH_PowerControlInfo,
                                rrc_UL_DPCH_PowerControlInfo_r4_tdd))
    {
      if(FAILURE == tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r4(ie_ptr->ul_DPCH_PowerControlInfo.u.tdd))
      {
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No valid FDD data for UL DPCH Power Control Info");
      return FAILURE;
    }
  }
  else
  {
    /* both in FDD and TDD;
2> if the IE "Uplink DPCH power control info" is not included in a message used to enter CELL_DCH:
3> set the variable INVALID_CONFIGURATION to true. */
    if ((tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH) &&
          (rrc_state != TDSRRC_STATE_CELL_DCH))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"no UlDpchPwrInfo from nonDCH to DCH");
      return FAILURE;
    }
  }

  /* Make sure of TDD and get the right parameters */
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_UL_DPCH_Info_r4_modeSpecificInfo_tdd))
  {
    /* Get the UL Timing Advance */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd, ul_TimingAdvancePresent))
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = TRUE;
      TDSRRC_MSG0(MSG_LEGACY_LOW,"This OTA MSG include ul_TimingAdvanceControl");
      if (FAILURE == tdsrrcllcpcie_get_ul_tim_adv_contrl_r4(&ie_ptr->modeSpecificInfo.u.tdd->ul_TimingAdvance))
      {
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"This OTA MSG not include ul_TimingAdvanceControl");
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = FALSE;
    }
    /* Get Uplink CCTrCH LIST from OTA message */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd, ul_CCTrCHListPresent))
    {
      if (1 < ie_ptr->modeSpecificInfo.u.tdd->ul_CCTrCHList.n)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ul_CCTrCHList %d is invalid",(ie_ptr->modeSpecificInfo.u.tdd->ul_CCTrCHList.n));
        return FAILURE;
      }

      if (FAILURE == tdsrrcllcpcie_get_ul_cctrch_list_r4(&ie_ptr->modeSpecificInfo.u.tdd->ul_CCTrCHList))
      {
        return FAILURE;
      }
    }
    /* TDD support only one CCTrCH. If OTA message send ul_CCTrCHListToRemove, RRC consider it invalid */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd, ul_CCTrCHListToRemovePresent))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDD only have one CCTrCH, ul_CCTrCHListToRemove %d is invalid",(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,ul_CCTrCHListToRemovePresent)));
      return FAILURE;
    }
  } /* end TDD if */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD UL DPCH information present");
    return FAILURE;
  }

  /* If made it here, all is well */
  /* Indicate that Uplink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
  return SUCCESS;
} /* end tdsrrcllc_get_ul_dpch_info_r4() */
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r4()

DESCRIPTION   This function translates the Uplink DPCH Power Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs can refer to 10.3.6.91 in 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r4
(
  tdsrrc_UL_DPCH_PowerControlInfo_r4_tdd *ie_ptr
)
{
/**     2> if the IE "Uplink DPCH power control info" is included in the IE "Uplink DPCH info":
  3> if the IE " UL target SIR " is included ignore the parameter. */

  /* According to 8.6.6.11 in 3GPP 25331v716, if the CHOICE UL OL PC info is set to
      'Broadcast UL OL PC info', set the variable INVALID_CONFIGURATION to true. */
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->ul_OL_PC_Signalling,
              T_tdsrrc_UL_DPCH_PowerControlInfo_r4_ul_OL_PC_Signalling_broadcast_UL_OL_PC_info))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"1.28TDD unsupport broadcast UL OL PC info,failure ");
    return FAILURE;
  }


  /* if tdd choice is tdd384, return failure */
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->ul_OL_PC_Signalling.u.individuallySignalled->tddOption,
              T_tdsrrc_UL_DPCH_PowerControlInfo_r4_tddOption_tdd384))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"dpch pwr select unsupport tdd3.84,failure ");
    return FAILURE;
  }

    /** the value range of tpc_step_size is 1, 2, 3.
         The first num of tpc_step_size in OC struct is zero */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tpc_step_size = 
    ie_ptr->ul_OL_PC_Signalling.u.individuallySignalled->tddOption.u.tdd128->tpc_StepSize;

    /* in 3GPP 10.3.6.91, primaryCCPCH_TX_Power is optional. It is not correct.
       Because in 11th chapter, it is not optional. */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.pccpch_tx_pwr = 
    ie_ptr->ul_OL_PC_Signalling.u.individuallySignalled->primaryCCPCH_TX_Power;

  return SUCCESS;
} /* end get_ul_dpch_pwr_ctrl_info_r4() */

/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_common_for_all_rl_r4()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.
                IEs refer to 10.3.6.24.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_rl_r4
(
  /* Pointer to the DL CommonInformation IE */
  tdsrrc_DL_CommonInformation_r4 *ie_ptr,

  /* Pointer to the desirec DL PhyChan common information structure */
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
)
{
  tdsrrc_state_e_type  rrc_state;

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Info Common for all RL");

  /* Get the info common for all DL DPCH if present */
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                dl_DPCH_InfoCommonPresent))
  {
    if(tdsrrcllcpcie_get_dl_dpch_common_info_r4(&ie_ptr->dl_DPCH_InfoCommon,
                               &dpch->dpch_common
                              ) == FAILURE
      )
    {
      return FAILURE;
    }
  } /* end if dl dpch common info present */
  
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
                T_tdsrrc_DL_CommonInformation_r4_modeSpecificInfo_tdd))
  {
      /* Get tstd indicator */
    if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->tddOption,
          T_tdsrrc_DL_CommonInformation_r4_tddOption_tdd128))
    {
      dpch->dpch_common.tstd_indicator =
        ie_ptr->modeSpecificInfo.u.tdd->tddOption.u.tdd128->tstd_Indicator;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd->tddOption is not tdd128");
      return FAILURE;
    }

      /* Get the default DPCH offset if present */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
                    defaultDPCH_OffsetValuePresent))
    {
      /* DefaultDPCH-OffsetValueTDD ::= INTEGER (0..7) */
      dpch->doff =
        (uint8) ie_ptr->modeSpecificInfo.u.tdd->defaultDPCH_OffsetValue;

      if(dpch->doff > TDSRRCLLC_ASN1_DOFF_MAX_VAL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Doff ASN1 value out of range");
        return(FAILURE);
      }
      dpch->opt_parm_presence_bitmask |=
        TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
    }
    else
    {
      rrc_state = tdsrrc_get_state();

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_INIT)
      {
        /*For HHO, assign DOFF = 0 to L1*/
        if((tdsordered_config_ptr->is_hho)&&(rrc_state == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Non-DCH ->DCH case. Pending clarification from NW. If yes, merge with above case*/
        else if(rrc_state != TDSRRC_STATE_CELL_DCH)
        {
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
        /*Return failure since DOFF is missing, e.g. DCH->DCH reconfig case*/
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"We do fault-tolerance for DOFF is missing");
          dpch->doff = 0;
            dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }

      }

      if(dpch->dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN)
      {
        /*For Non-DCH -> DCH case, make DOFF = 0.*/
        if((rrc_state != TDSRRC_STATE_CELL_DCH)&&(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
        {
          dpch->doff = 0;
          dpch->opt_parm_presence_bitmask |=
                  TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
        }
      }

    }  /* Doff is absent */
  } /* end if common TDD */
  else
  {
    /* TDD not selected.  Error as FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_COMMON_FOR_ALL_RL");
    return FAILURE;
  }
  /* Indicate that Downlink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
 /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  return SUCCESS;
} /* tdsrrcllc_get_dl_info_common_for_all_rl_r4() */

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r4()

FILE NAME     This function gets the DL DPCH info for each RL

DESCRIPTION   This function translates the Downlink DPCH info for each
              RL IE to the RRC config database.  Note that it is assumed
              that a higher level routine is doing the translation from
              the linked list present in the ASN.1 decoder output to
              the array present in the RRC config database.  The radio
              link index is passed into this function and must be
              generated by that higher level routine. IEs refer to 10.3.6.21.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if everything is ok, FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all of the parameters
              that are present in the IE.  For those parameters not
              present the values currently existing are untouched.  For
              each optional parameter that is present the corresponding
              bit in the optional parameters presence bitmask is set.
====================================================================*/
static uecomdef_status_e_type tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r4
(
  /* pointer to the IE */
  tdsrrc_DL_DPCH_InfoPerRL_r4 *ie_ptr,

  /* pointer to the desired DL PhyChan parameter structure */
  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,
            rrc_DL_DPCH_InfoPerRL_r4_tdd))
  {
    /** TDD support only one CCTrCH. If OTA message send ul_CCTrCHListToRemove,
       RRC consider it invalid */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->u.tdd, dl_CCTrChListToRemovePresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDD only have one CCTrCH,ul_CCTrCHListToRemove invalid");
      return FAILURE;
    }

    /* Get Downlink CCTrCH LIST from OTA message */
    if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr->u.tdd,
               dl_CCTrChListToEstablishPresent))
    {
      if (1 < ie_ptr->u.tdd->dl_CCTrChListToEstablish.n)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ul_CCTrCHList %d is invalid",(ie_ptr->u.tdd->dl_CCTrChListToEstablish.n));
        return FAILURE;
      }

      if (FAILURE == tdsrrcllcpcie_get_dl_cctrch_list_r4(
                       &ie_ptr->u.tdd->dl_CCTrChListToEstablish.elem[0],
                       &tdsordered_config_ptr->l1_dl_chan_parms.dpch))
      {
        return FAILURE;
      }
    }
    else
    {
      /*dl cctrch list is optional, and it must be present from non_DCH to DCH.
             So RRC add this check. DL_DPCH_InfoPerRL only be used in DCH.
             Then next state must be DCH. So here RRC only check current state. */
      if (rrc_state != TDSRRC_STATE_CELL_DCH)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"dl_CCTrCHList is not present from non-DCH to DCH");
        return FAILURE;
      }
    }
  }
  else
  {
    /* TDD choice not selected, FDD not supported */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FDD not supported");
    return FAILURE;
  }

  /* everything went ok */
  return SUCCESS;
} /* tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r5() */

/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_for_each_rl_r4()

DESCRIPTION     This function translates the Downlink information for
                each radio link IE to the RRC config database

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllc_get_dl_info_for_each_rl_r4
(
  tdsrrc_DL_InformationPerRL_r4 *ie_ptr,

  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
)
{

  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Get Dl Info per RL");
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->modeSpecificInfo,
              T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd))
  {
    /* First check if the DL DPCH Info per RL is included. If not,
    * this is NOT a valid Radio link - ignore it.
    */
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
              dl_DPCH_InfoPerRLPresent))
    {
      /* save tstd info */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr->modeSpecificInfo.u.tdd,
        rrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption,
            rrc_PrimaryCCPCH_Info_r4_tddOption_tdd128))
        {
          dl_phychan->pccpch.tstd_indicator =
            ie_ptr->modeSpecificInfo.u.tdd->u.tdd->tddOption.u.tdd128->tstd_Indicator;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD128 info present in DL_INFO_FOR_EACH_RL");
          return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }

      /*If Cell_id is present for this RL, store it in tdscell_id_per_rl */
      if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                cell_idPresent))
      {
        tdscell_id_per_rl[0] = (uint32) tdsrrc_translate_cell_id(&(ie_ptr->cell_id));
      }       

      /* Get the DL DPCH info for this radio link */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get DL DPCH Info for RL %d",1);

      if (tdsrrcllcpcie_get_dl_dpch_info_for_each_rl_r4(&ie_ptr->dl_DPCH_InfoPerRL,
        dl_phychan) == FAILURE)
      {
        return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
      }
      /* if we made it here we must be ok */
      /* Indicate that Downlink L1 needs to be configured */
      tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
      * still needs to be reconfigured so as to get the new CCTrCH
      * ID whenever Downlink L1 is reconfigured. This is due to the
      * interface requirements.
      */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
      return TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS;
    }  /* DL DPCH Info is present */
    else
    {
      /* DL DPCH Info per RL is NOT present - ignore this IE */
      return TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
    }
  }
  else
  {
    /* No TDD info.  We don't support FDD so return indicating an error */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No TDD info present in DL_INFO_FOR_EACH_RL");
    return TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE;
  }
} /* tdsrrcllc_get_dl_info_for_each_rl_r4() */

/*===========================================================================
FUNCTION        tdsrrcllc_get_dl_info_per_rl_r4()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

                The function is copy from tdsget_dl_info_per_rl

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_per_rl_r4
(
  tdsrrc_DL_InformationPerRL_List_r4 *ie_ptr,
  tdsrrc_RRC_StateIndicator next_state
)
{
  /* is_inter_freq flag indicates if it is an Inter Freq HHO */
  /*boolean is_inter_freq = FALSE;*/

  tdsrrcllcoc_dl_info_per_rl_status_e_type status =
    TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED;
  tdsrrc_freq_type rrc_freq = tdsrrcccm_get_curr_camped_freq();
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  tdsordered_config_ptr->cpid_present = FALSE;

  if (ie_ptr->n > 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"more than one RL not supported");
    return FAILURE;
  }
  if(TDSRRC_CHECK_MSG_TYPE(ie_ptr->elem[0].modeSpecificInfo, 
                  T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_fdd))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL FDD not supported");
    return FAILURE;
  }
  if (T_tdsrrc_PrimaryCCPCH_Info_r4_fdd == ie_ptr->elem[0].modeSpecificInfo.u.tdd->t)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL InfoPerRL PrimaryCCPCH FDD not supported");
    return FAILURE;
  }

  /* We can say with certainty that if frequency info is included, and
   * it is different than the current frequency, then we are doing a
   * hard handover.  If this is not the case, we revisit this flag
   * later.
   */
  /*is_inter_freq = (tdsordered_config_ptr->p_freq_present &&
      tdsordered_config_ptr->p_freq != rrc_freq);*/

  if (next_state == tdsrrc_RRC_StateIndicator_cell_DCH)
  {
    /* If DPCH Info per RL is not present within DL Info per RL, then ignore it and
     * do not change the RL Info list in OC when going from DCH->DCH.
     * When going from non-DCH->DCH, the above condition should result in a Failure.
     */
    if ((TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[0], dl_DPCH_InfoPerRLPresent)) == 0)
    {
      if (TDSRRC_STATE_CELL_DCH == rrc_state)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring DL Info per RL Info absent");
        if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          tdsordered_config_ptr->cpid_present = TRUE;
        }
        return SUCCESS;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failing,DPCH Info per RL not present");
        return FAILURE;
      }
    }

    if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      tdsordered_config_ptr->cpid_present = TRUE;
    }
    if (TDSRRC_STATE_CELL_DCH != rrc_state)
    {
      /* msg have no cell_param_id */
      if (FALSE == tdsordered_config_ptr->cpid_present)
      {
        /* set current cell_param_id to OC structrue */
        tdsordered_config_ptr->cell_param_id = (uint8)tdsrrcccm_get_curr_camped_cpid();
      }
      /* Going from non-DCH state to CELL DCH */
      /* Always set new_rl_seen when coming to DCH from a non-DCH state.
      * That ensures the RL Info is populated after initializing.
      */
      if ((TRUE == tdsordered_config_ptr->p_freq_present)&&
            (tdsordered_config_ptr->p_freq == rrc_freq))
      {
        /* check if the current camped on ParaID is in RL Info.
         * If so, force Frequency_info to FALSE.
         */
        if ((ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)&&
           (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID != tdsrrcccm_get_curr_camped_cpid()))
        {
          /* Intra freq HHO */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"intra-f HHO FACH->DCH");
        }
        else
        {
          tdsordered_config_ptr->p_freq_present = FALSE;
        }
      }
    }
    else
    {
      /* DCH -> DCH and hho */
      if(tdsordered_config_ptr->is_hho)
      {
        tdshsdpa_msg_params.msg_params_present = TRUE;
      }
    }

    /* Process the IE and place the info into our database. */
    status = tdsrrcllc_get_dl_info_for_each_rl_r4(&ie_ptr->elem[0],
        &tdsordered_config_ptr->l1_dl_chan_parms);

    if(status == TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE)
    {
      return FAILURE;
    }
    else if(status == TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS)
    {
      /* either inter HHO or intra HHO */
      if(tdsordered_config_ptr->is_hho)
      {
        /* If we succeeded in processing the IE, and we are doing a
         * hard handover, we must be sure that L1 is informed by
         * sending frequency info, even if we didn't get frequency
         * info in the reconfiguration message.
         */
        if(!tdsordered_config_ptr->p_freq_present)
        {
          tdsordered_config_ptr->p_freq_present = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"HHO triggered");
        }
      }
    }
#ifdef FEATURE_TDSCDMA_HSUPA
    /* Call below function to wipe out any stored L1 information
     * on EUL
     */
    if (tdsordered_config_ptr->is_hho) 
    {
      tdsrrcllcpcie_clean_l1_e_info_on_non_r7_hho();
    }
#endif /* FEATURE_TDSCDMA_HSUPA */

  }
  else /* All states other than Cell_DCH */
  {
    if(ie_ptr->n > 0)
    {
      if (ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
      {
        tdsordered_config_ptr->cell_param_id =
              ie_ptr->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        tdsordered_config_ptr->cpid_present = TRUE;
      }
    }
  }

  return SUCCESS;
} /* end tdsrrcllc_get_dl_info_per_rl_r4() */

/*====================================================================
FUNCTION       tdsrrc_get_mac_d_hfn_cucnf_r4()

DESCRIPTION    This function takes a pointer to the cu_confirm_ptr for R4
                         and extract mac_d_hfn field.

DEPENDENCIES   None

RETURN VALUE   value of mac_d_hfn if present else, invalid value

====================================================================*/
uint32 tdsrrc_get_mac_d_hfn_cucnf_r4(tdsrrc_CellUpdateConfirm_r4_IEs *cu_confirm_ptr)
{

  uint32 mac_d_hfn = TDSRRCSMC_INVALID_HFN_VALUE;
  uecomdef_status_e_type loc_ret_val=SUCCESS;
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(cu_confirm_ptr,
              dl_CommonInformationPresent))
  {
    if(TDSRRC_CHECK_MSG_TYPE_IE(cu_confirm_ptr->dl_CommonInformation,
               dl_DPCH_InfoCommonPresent))
    {
      loc_ret_val = tdsrrcrb_get_mac_d_hfn(
                TDSRRC_CHECK_MSG_TYPE_IE(cu_confirm_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
                    mac_d_HFN_initial_valuePresent),
                &cu_confirm_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
                &mac_d_hfn);

      if(loc_ret_val == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
      }
    }
  }
  return (mac_d_hfn);
}
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_fpach_info_r4()

DESCRIPTION   This function translates the FPACH
              info common for all RL (radio links) IE into the RRC
              config database. IE refer to 10.3.6.35a in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_fpach_info_r4
(
  tdsrrc_FPACH_Info_r4 *ie_ptr
)
{
  /* Get fpach_ts_num info */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_fpach_info.fpach_ts_num = 
                 ie_ptr->timeslot;

  /* 10.3.6.35a: Channelisation code--Enumerated((16/1)..(16/16) */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_fpach_info.fpach_chan_code.sf =
                 TDSL1_SF_16;
  /* channelisationCode is 1~16, ASN1 is enum, 0~15 */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_fpach_info.fpach_chan_code.chan_code = 
                 ie_ptr->channelisationCode + 1;

  /* Get midambleShiftAndBurstType info */
  /* when defaultMidamble, midambleAllocationMode.t = 1. But midamble_allct_type = 0. */

  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_fpach_info.midamble_and_burst.allocation_mode = 
                 tdsMidambleAllocMode[ie_ptr->midambleShiftAndBurstType.midambleAllocationMode.t];

  /** 10.3.6.41: midambleShift is mandatory present when the value of the IE "Midamble Allocation Mode"
     is "UE-specific midamble". */
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->midambleShiftAndBurstType.midambleAllocationMode,
            T_tdsrrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble))
  {
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_fpach_info.midamble_and_burst.shift = 
        ie_ptr->midambleShiftAndBurstType.midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
  }

  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_fpach_info.midamble_and_burst.config =
        tdsMidambleConfiguration[ie_ptr->midambleShiftAndBurstType.midambleConfiguration];

  /* Get wt info */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_fpach_info.wt = 
        ie_ptr->wi;

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_tim_adv_contrl_r4()

DESCRIPTION   This function translates the Uplink Timing Advance Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.96 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_tim_adv_contrl_r4
(
  tdsrrc_UL_TimingAdvanceControl_r4 *ie_ptr
)
{
  tdsrrc_UL_TimingAdvanceControl_r4_tdd128  *tmp_ie_ptr;

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr, rrc_UL_TimingAdvanceControl_r4_enabled))
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->u.enabled->tddOption,
                       rrc_UL_TimingAdvanceControl_r4_tddOption_tdd128))
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_enabled = TRUE;
      tmp_ie_ptr = ie_ptr->u.enabled->tddOption.u.tdd128;

      /* Get ul_SynchronisationParameters */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ie_ptr, ul_SynchronisationParameters))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 
                                          tmp_ie_ptr->ul_SynchronisationParameters.frequency;
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 
                                          tmp_ie_ptr->ul_SynchronisationParameters.stepSize;
      }
      else
      {
        /* from the 10.3.6.96:
        Default: Uplink synchronisation step size is 1. Uplink synchronisation frequency is 1. */
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 1;
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 1;
      }

      /* Get synchronisationParameters */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ie_ptr, synchronisationParameters))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = TRUE;

        /* Get SYNC_UL codes bitmap */
        /* translate sync_UL_CodesBitmap string to uint8(refer to tdsrrc_translate_ura_id) */
        if (8 != tmp_ie_ptr->synchronisationParameters.sync_UL_CodesBitmap.numbits)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received sync_UL_CodesBitmap Length: %d is invalid",tmp_ie_ptr->synchronisationParameters.sync_UL_CodesBitmap.numbits);
        }

        /* Mask of SYNC_UL codes. Each bit indicates availability of a SYNC_UL code,
         * where the SYNC_UL codes are numbered "code0" to "code7". The value 1 of a 
         * bit indicates that the corresponding SYNC_UL code can be used */
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_sync_code_mask = 
                         tmp_ie_ptr->synchronisationParameters.sync_UL_CodesBitmap.data[0];

        /* Get FPACH info */
        tdsrrcllcpcie_get_fpach_info_r4(&tmp_ie_ptr->synchronisationParameters.fpach_Info);

        /* Get prxUpPCHdes(10.3.6.96) info */
        /* -- Actual value prxUpPCHdes = IE value - 120 */
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des = 
                             tmp_ie_ptr->synchronisationParameters.prxUpPCHdes - 120;

        /* Get SYNC_UL procedure info */
        if (TDSRRC_MSG_COMMON_BITMASK_IE(tmp_ie_ptr->synchronisationParameters, 
                                           tdsrrc_SynchronisationParameters_r4, sync_UL_Procedure))
        {
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx =
              tdsMaxSynUlTrans[tmp_ie_ptr->synchronisationParameters.sync_UL_Procedure.max_SYNC_UL_Transmissions];
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step =
              tmp_ie_ptr->synchronisationParameters.sync_UL_Procedure.powerRampStep;
        }
        else
        {
        /** from 10.3.6.96:
                 Default is:Max SYNC_UL Transmission is 2. Power Ramp Step is 2.*/
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx = 2;
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step = 2;
        }
#ifdef FEATURE_TDSCDMA_HSUPA
        /* In 25.331,10.3.6.78d, IE 'PRXUpPCHdes', 'Power Ramp Step' and 'Max SYNC_UL Transmissions' are CV type,
                  HSUPA scheduled E-RUCCH information will be affected if one of them changed*/
        if ((TRUE == tdscurrent_config_ptr->e_dch_transmission) && 
             (tdscurrent_config_ptr->hsupa_start_mask & TDSRRC_SCHED_START))
        {
          if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des != 
               tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.prx_upPch_des))
          {
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.prx_upPch_des =
               tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des;
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;           
            tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);    
          }
          if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx != 
               tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.max_sync_ul_tx))
          {
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.max_sync_ul_tx =
               tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx;
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;            
            tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);    
          }
          if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step != 
               tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.pwr_ramp_step))
          {
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.pwr_ramp_step =
               tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step;
            tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;            
            tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);    
          }
        }
#endif
      }
      else
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = FALSE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd384,not tdd128");
      return FAILURE;
    }
  }
  else
  {
    /* 8.6.6.26 : 1> if the IE "Uplink Timing Advance Control" has the value "disabled":
          2> reset timing advance to 0; */
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_enabled = FALSE;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 0;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 0;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = FALSE;
    memset(&(tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms), 0,
        sizeof(tdsl1_ul_sync_parms_struct_type));
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_com_ts_info_r4()

DESCRIPTION   This function translates the Common Timeslot
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.10 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_com_ts_info_r4
(
  tdsrrc_CommonTimeslotInfo *ie_ptr,
  tdsl1_common_ts_info_struct_type *comts_ptr,
  boolean dl_indicator
)
{
  /* from 10.3.6.10: secondInterleavingMode is Enumerated(Frame, Timeslot) */
  comts_ptr->second_interleaving_mode = ie_ptr->secondInterleavingMode;

  /* Get tfci_coding info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
                                tdsrrc_CommonTimeslotInfo, tfci_Coding))
  {
    switch (ie_ptr->tfci_Coding)
    {
      case tdsrrc_TFCI_Coding_tfci_bits_4:
        comts_ptr->tfci_coding = TDSL1_TFCI_CODING_BITS_4;
        break;

      case tdsrrc_TFCI_Coding_tfci_bits_8:
        comts_ptr->tfci_coding = TDSL1_TFCI_CODING_BITS_8;
        break;

      case tdsrrc_TFCI_Coding_tfci_bits_16:
        comts_ptr->tfci_coding = TDSL1_TFCI_CODING_BITS_16;
        break;

      case tdsrrc_TFCI_Coding_tfci_bits_32:
        comts_ptr->tfci_coding = TDSL1_TFCI_CODING_BITS_32;
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfci_Coding is out of range");
        return FAILURE;
    }
  }
  else
  {
    if (TRUE == dl_indicator)
    {
      /* TFCI Coding is MD, default is no bit. 
               DL L1 does not support TFCI  bit =0. */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Tfci_Coding is not present(MD: TFCI 0 bit).But DL does not support BTFD. So RRC reject");
      return FAILURE;
    }
    else
    {
      /* TFCI Coding is MD, default is no bit. 
               RRC can configure TFCI  bit =0 for UL(need NW/Call Box support BTFD). */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Tfci_Coding is not present(MD: TFCI 0 bit).UL support BTFD.So tfci_coding is 0");
      comts_ptr->tfci_coding = 0;
    }
  }


  /** Get the Puncturing Limit. The OTA Msg receives this value as an enum.
        * The puncturing limit is a real number between 0.4 and 1.0 in steps of 0.04.
        * L1 expects this value to be from 40 to 100; in other words, L1 expects
        * l1_puncturing_limit = puncturing_limit * 100;
        * Therefore in the ASN.1 enum type the first enum value 0 corresponds to 0.4,
        * 1 corresponds to 0.44, etc... . Hence the following equation is derived:
        * l1_puncturing_limit = (ASN.1_enum_type * 4) + 40;
        * If this enum changes, this equation may need tweaking or else, this can
        * be broken into a switch with multiple cases.
        */

  comts_ptr->punct_limit = (uint8)
          (((uint8)(ie_ptr->puncturingLimit) * 4) + 40);

  /* Get Repetition period nad Length info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
                    tdsrrc_CommonTimeslotInfo, repetitionPeriodAndLength))
  {
    switch (ie_ptr->repetitionPeriodAndLength.t)
    {
      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod1:
        comts_ptr->repetition_period = TDSL1_REPEAT_PRD_1;
        comts_ptr->repetition_length = 0;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod2:
        comts_ptr->repetition_period = TDSL1_REPEAT_PRD_2;
        comts_ptr->repetition_length =
              ie_ptr->repetitionPeriodAndLength.u.repetitionPeriod2;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod4:
        comts_ptr->repetition_period = TDSL1_REPEAT_PRD_4;
        comts_ptr->repetition_length =
              ie_ptr->repetitionPeriodAndLength.u.repetitionPeriod4;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod8:
        comts_ptr->repetition_period = TDSL1_REPEAT_PRD_8;
        comts_ptr->repetition_length =
              ie_ptr->repetitionPeriodAndLength.u.repetitionPeriod8;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod16:
        comts_ptr->repetition_period = TDSL1_REPEAT_PRD_16;
        comts_ptr->repetition_length =
              ie_ptr->repetitionPeriodAndLength.u.repetitionPeriod16;
        break;
      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod32:
        comts_ptr->repetition_period = TDSL1_REPEAT_PRD_32;
        comts_ptr->repetition_length =
              ie_ptr->repetitionPeriodAndLength.u.repetitionPeriod32;
        break;

      case T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod64:
        comts_ptr->repetition_period = TDSL1_REPEAT_PRD_64;
        comts_ptr->repetition_length =
              ie_ptr->repetitionPeriodAndLength.u.repetitionPeriod64;
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"repetitionPeriod is out of range");
        return FAILURE;
    }
  }
  else
  {
   /* 10.3.6.10, Default is continuous allocation. Value 1 indicate continuous */
    comts_ptr->repetition_period =
        T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod1;
   /* Repetition length is empty if repetition period is set to 1. */
    comts_ptr->repetition_length = 0;
  }
  
  return SUCCESS;

}
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4
(
  tdsrrc_IndividualTimeslotInfo_LCR_r4 *src_ptr,
  tdsl1_ul_dpch_ts_struct_type *dst_ptr
)
{
  /* UL timeslot is 1~5*/
  if ((0 == src_ptr->timeslotNumber) || (6 == src_ptr->timeslotNumber))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR, "ts_num is %d,UL ts_num must be 1~5",src_ptr->timeslotNumber);
    return FAILURE;
  }
  dst_ptr->ts_num = src_ptr->timeslotNumber;
  dst_ptr->tfciExist = src_ptr->tfci_Existence;
/* l1 modify */
  /* Get Midamble Shift and burst type info */
  /* when defaultMidamble, midambleAllocationMode.t = 1. But midamble_allct_type = 0. */
  dst_ptr->midamble_and_burst.allocation_mode =
                  tdsMidambleAllocMode[src_ptr->midambleShiftAndBurstType.midambleAllocationMode.t];
  dst_ptr->midamble_and_burst.config =
                  tdsMidambleConfiguration[src_ptr->midambleShiftAndBurstType.midambleConfiguration];

  if (TDSRRC_CHECK_COMMON_MSG_TYPE(src_ptr->midambleShiftAndBurstType.midambleAllocationMode,
                  rrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble))
  {
    dst_ptr->midamble_and_burst.shift =
      src_ptr->midambleShiftAndBurstType.midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
  }
  
  /* Get modulation info */
  dst_ptr->modType = src_ptr->modulation;

  /* Get SS-TPC Symbols info */
  dst_ptr->ss_tpc_symbols = src_ptr->ss_TPC_Symbols;

  /* Get Additional TPC-SS Symbols info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(src_ptr, tdsrrc_IndividualTimeslotInfo_LCR_r4,
                                     additionalSS_TPC_Symbols))
  {
    dst_ptr->add_ss_tpc_symbols = src_ptr->additionalSS_TPC_Symbols;
  }

  return SUCCESS;

}
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4
(
  tdsrrc_UL_TS_ChannelisationCodeList *src_ptr,
  tdsl1_ul_dpch_ts_struct_type *dst_cfg
)
{
  uint8    i;

  dst_cfg->num_chan_codes = src_ptr->n;
  for (i = 0; i < src_ptr->n; i++)
  {
    switch (src_ptr->elem[i])
    {
      case  tdsrrc_UL_TS_ChannelisationCode_cc1_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_1;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc2_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_2;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc2_2:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_2;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_2;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc4_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_4;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc4_2:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_4;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_2;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc4_3:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_4;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_3;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc4_4:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_4;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_4;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_2:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_2;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_3:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_3;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_4:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_4;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_5:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_5;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_6:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_6;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_7:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_7;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc8_8:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_8;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_8;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_1:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_1;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_2:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_2;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_3:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_3;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_4:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_4;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_5:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_5;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_6:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_6;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_7:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_7;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_8:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_8;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_9:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_9;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_10:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_10;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_11:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_11;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_12:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_12;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_13:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_13;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_14:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_14;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_15:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_15;
          continue;

      case  tdsrrc_UL_TS_ChannelisationCode_cc16_16:
          dst_cfg->chan_code_list[i].sf = TDSL1_SF_16;
          dst_cfg->chan_code_list[i].chan_code = TDSL1_UL_CHAN_CODE_16;
          continue;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_timeslot_and_code choice is out of range");
        continue;
    }
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_r4()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.94a in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_r4
(
  tdsrrc_UL_CCTrCH_r4_tdd128 *ie_ptr,
  tdsl1_ul_dpch_cfg_struct_type *dpch_cfg
)
{
  uint8      i, j;
  tdsrrc_UplinkTimeslotsCodes_LCR_r4_additionalTimeslots    *tmp_ie_ptr;
  tdsrrc_UplinkTimeslotsCodes_LCR_r4_timeslotList    *ts_list_short_ptr;
  tdsrrc_UplinkAdditionalTimeslots_LCR_r4_newParameters    *new_para_list_ptr;
  tdsl1_ul_dpch_ts_struct_type      ts_list_temp;

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
           tdsrrc_UL_CCTrCH_r4_tdd128, ul_CCTrCH_TimeslotsCodes))
  {
    dpch_cfg->dynamicSfUse = ie_ptr->ul_CCTrCH_TimeslotsCodes.dynamicSFusage;

    /* if OTA message has ul_CCTrCH_TimeslotsCode, num of timeslot be set. */
    dpch_cfg->num_ts = 0;

    /* Get First Individual timeslot info */
    if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4(&ie_ptr->
                  ul_CCTrCH_TimeslotsCodes.firstIndividualTimeslotInfo,
                  &dpch_cfg->ts_list[0]))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4 return error");
      return FAILURE;
    }

    if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4(&ie_ptr->
                  ul_CCTrCH_TimeslotsCodes.ul_TS_ChannelisationCodeList,
                  &dpch_cfg->ts_list[0]))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4 return error");
      return FAILURE;
    }
    /* we get first timeslot info, so num of timeslot add 1 */
    dpch_cfg->num_ts += 1;

    /* Get more timeslots info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots,
          rrc_UplinkTimeslotsCodes_LCR_r4_moreTimeslots_noMore))
    {
      /* no more means no data */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Receive ul_CCTrCH_TimeslotsCodes moreTimeslots noMore,no data");
    }
    else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots,
          rrc_UplinkTimeslotsCodes_LCR_r4_moreTimeslots_additionalTimeslots))
    {
      tmp_ie_ptr = ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots.u.additionalTimeslots;

      /* Get consecutive times slot info */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(tmp_ie_ptr,
                           rrc_UplinkTimeslotsCodes_LCR_r4_additionalTimeslots_consecutive))
      {
        dpch_cfg->num_ts += tmp_ie_ptr->u.consecutive->numAdditionalTimeslots;

        for (i = 1; i <= tmp_ie_ptr->u.consecutive->numAdditionalTimeslots; i++)
        {
          memscpy(
            &(dpch_cfg->ts_list[i]),sizeof(tdsl1_ul_dpch_ts_struct_type),
            &(dpch_cfg->ts_list[0]),
            sizeof(tdsl1_ul_dpch_ts_struct_type)
          );
          /* consecutive means continuous, so from first times slot,
             the following timeslots one by one be add 1. */
          /* ts_num is 1~5, first ts_num does not start from 0. So max UL ts_num is 5, not 4. */
          dpch_cfg->ts_list[i].ts_num = ((dpch_cfg->ts_list[0].ts_num) + i) % 
                                   (TDSL1_UL_MAX_NUM_TS + 1);

          /* ts_num is 1~5 in UL */
          if ((0 == dpch_cfg->ts_list[i].ts_num) || (6 == dpch_cfg->ts_list[i].ts_num))
          {
            ERR_FATAL("ts_num is %d, UL ts_num is 1~5",dpch_cfg->ts_list[i].ts_num,0,0);
            return FAILURE;
          }
        }
      }
      /* Get timeslot list info */
      else if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(tmp_ie_ptr, rrc_UplinkTimeslotsCodes_LCR_r4_additionalTimeslots_timeslotList))
      {
        ts_list_short_ptr = tmp_ie_ptr->u.timeslotList;
        dpch_cfg->num_ts += ts_list_short_ptr->n;
        for (i = 0; i < ts_list_short_ptr->n; i++)
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE(ts_list_short_ptr->elem[i].parameters,
                rrc_UplinkAdditionalTimeslots_LCR_r4_parameters_sameAsLast))
          {
          /* same as last means (i+1) same as (i). */
            memscpy(
                     &(dpch_cfg->ts_list[i + 1]),sizeof(tdsl1_ul_dpch_ts_struct_type),
                     &(dpch_cfg->ts_list[i]),
                     sizeof(tdsl1_ul_dpch_ts_struct_type)
            );
            /* ts_num is 1~5 in UL, But ASN1 value of UL timeslot number is 0~14 */
            if ((1 > ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber) ||
                (5 < ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber))
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"UL AdditionalTimeslot number is %d,But UL timeslot range is 1~5",dpch_cfg->ts_list[i].ts_num);
              ERR_FATAL("ts_num is %d, UL ts_num is 1~5",ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber,0,0);
              return FAILURE;
            }
            dpch_cfg->ts_list[i + 1].ts_num =
              ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber;
          }
          else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ts_list_short_ptr->elem[i].parameters,
                rrc_UplinkAdditionalTimeslots_LCR_r4_parameters_newParameters))
          {
          /* new para means we get new values from second timeslot. */
            new_para_list_ptr = ts_list_short_ptr->elem[i].parameters.u.newParameters;
            if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4(
                             &(new_para_list_ptr->individualTimeslotInfo),
                             &(dpch_cfg->ts_list[i + 1])))
            {
              return FAILURE;
            }
            if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4(
                             &(new_para_list_ptr->ul_TS_ChannelisationCodeList),
                             &(dpch_cfg->ts_list[i + 1])))
            {
              return FAILURE;
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"parameters choice is invalid");
            return FAILURE;
          }
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_CCTrCH_TimeslotsCodes.moreTimeslots.u.additionalTimeslots choice is invalid");
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_CCTrCH_TimeslotsCodes.moreTimeslots choice is invalid");
      return FAILURE;
    }
    for (i = 1; i < dpch_cfg->num_ts; i++)
    {
      for (j = 0; j < dpch_cfg->num_ts - i; j++)
      {
        if (dpch_cfg->ts_list[j].ts_num > dpch_cfg->ts_list[j + 1].ts_num)
        {
          ts_list_temp = dpch_cfg->ts_list[j];
          dpch_cfg->ts_list[j] = dpch_cfg->ts_list[j + 1];
          dpch_cfg->ts_list[j + 1]= ts_list_temp;
        }
      }
    }
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_cctrch_list_r4()

DESCRIPTION   This function translates the Uplink CCTrCH List
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.88 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_cctrch_list_r4
(
  tdsrrc_UL_CCTrCHList_r4 *ie_ptr
)
{
  /* Get ul-TargetSIR info */
  /* -- The IE ul-TargetSIR corresponds to PRX-DPCHdes for 1.28Mcps TDD
-- Actual value PRX-DPCHdes = (value of IE "ul-TargetSIR" - 120) */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.prx_dpch_des= ie_ptr->elem[0].ul_TargetSIR - 120;

  /* check If no "activation time" IE  for TDM, we can reject */
  if((ie_ptr->elem[0].timeInfo.m.activationTimePresent == 0)&&
       ((ie_ptr->elem[0].m.commonTimeslotInfoPresent == 1)&&
        (ie_ptr->elem[0].commonTimeslotInfo.m.repetitionPeriodAndLengthPresent == 1)&&
        (ie_ptr->elem[0].commonTimeslotInfo.repetitionPeriodAndLength.t 
                  != T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod1)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"no activation time for TDM,reject");
    return FAILURE;
  }

  /* Get Active time info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[0].timeInfo, tdsrrc_TimeInfo, activationTime))
  {
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.act_time =
                                   ie_ptr->elem[0].timeInfo.activationTime;
  }
  else
  {
  /** from 10.3.6.83:
         Frame number start of the physical channel existence. Default value is "Now" */
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.act_time =
                                   TDSRRCLLC_ACTIVATION_TIME_NOW;
  }

  /* Get common timing slot info(10.3.6.10) */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[0], tdsrrc_UL_CCTrCH_r4,
                                      commonTimeslotInfo))
  {
    if(FAILURE == tdsrrcllcpcie_get_com_ts_info_r4(&ie_ptr->elem[0].commonTimeslotInfo,
              &tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.common_ts_info, FALSE))
    {
      return FAILURE;
    }
  }

  /* Get Uplink DPCH timeslots and codes LCR(10.3.6.94a) */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->elem[0].tddOption,
                       rrc_UL_CCTrCH_r4_tddOption_tdd128))
  {
    if(FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_r4((tdsrrc_UL_CCTrCH_r4_tdd128 *)ie_ptr->elem[0].tddOption.u.tdd128,
            &tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg))
    {
      return FAILURE;
    }
  }

  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd384,not tdd128");
    return FAILURE;
  }

  return SUCCESS;
}


/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_ts_and_code_indiv_r4()

DESCRIPTION   This function translates the Downlink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_ts_and_code_indiv_r4
(
  tdsrrc_IndividualTimeslotInfo_LCR_r4 *src_ptr,
  tdsl1_dl_phycfg_individual_ts_info_struct_type *dst_ptr
)
{
    /* DL ts_num is 0, 2, 3, 4, 5, 6 */
    if (1 == src_ptr->timeslotNumber)
    {
      ERR_FATAL("ts_num is %d, DL ts_num is 0, 2, 3, 4, 5, 6",src_ptr->timeslotNumber,0,0);
      return FAILURE;
    }
    dst_ptr->ts_num = src_ptr->timeslotNumber;

    /* set to TRUE if TFCI is used */
    dst_ptr->tfci_exists = src_ptr->tfci_Existence;

    dst_ptr->midamble_ss_burst_type.allocation_mode =
             tdsMidambleAllocMode[src_ptr->midambleShiftAndBurstType.midambleAllocationMode.t];
    dst_ptr->midamble_ss_burst_type.config =
             tdsMidambleConfiguration[src_ptr->midambleShiftAndBurstType.midambleConfiguration];

    if (TDSRRC_CHECK_COMMON_MSG_TYPE(src_ptr->midambleShiftAndBurstType.midambleAllocationMode,
                rrc_MidambleShiftAndBurstType_LCR_r4_midambleAllocationMode_ueSpecificMidamble))
    {
      dst_ptr->midamble_ss_burst_type.shift =
            src_ptr->midambleShiftAndBurstType.midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
    }
    dst_ptr->ss_tpc_symbols =
            src_ptr->ss_TPC_Symbols;

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(src_ptr,
                tdsrrc_IndividualTimeslotInfo_LCR_r4,
                additionalSS_TPC_Symbols))
    {
      dst_ptr->additional_ss_tpc_symbols =
             src_ptr->additionalSS_TPC_Symbols;
    }

    return SUCCESS;
}
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_ts_and_code_channelcode_r4()

DESCRIPTION   This function translates the Downlink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_ts_and_code_channelcode_r4
(
  tdsrrc_DL_TS_ChannelisationCodesShort_codesRepresentation *src_ptr,
  tdsl1_dl_phycfg_individual_ts_info_struct_type *dst_ptr
)
{
  uint8 i, j;
  
  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(src_ptr,
           rrc_DL_TS_ChannelisationCodesShort_codesRepresentation_consecutive))
  {
    
    /* consecutive means continous. j indicates channelisation code num from OTA message. */
    j = src_ptr->u.consecutive->lastChannelisationCode -
           src_ptr->u.consecutive->firstChannelisationCode + 1;

    dst_ptr->chan_code_bitmap = 0;

    /** in this case, sf = 16. L1 interface no need sf. so chan_code_bitmap save chan code(1...16) 
        1 in bit i (i=0,?15) means SF16 Code #i is used in current timeslot.
        All zeros in this bitmap means using SF=1 code in current timeslot. */
    for (i = 0; i < j; i++)
    {
       dst_ptr->chan_code_bitmap |=
         1 << (src_ptr->u.consecutive->firstChannelisationCode + i);
    }
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(src_ptr,
             rrc_DL_TS_ChannelisationCodesShort_codesRepresentation_bitmap))
  {
    if (16 != src_ptr->u.bitmap->numbits)
    {
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"bitmap from OTA message is %d,not 16bit",(src_ptr->u.bitmap->numbits));
    }
    /* get chan_code_bitmap from OSOCTET  data[2] */
    dst_ptr->chan_code_bitmap = src_ptr->u.bitmap->data[1] |
      (src_ptr->u.bitmap->data[0] << 8);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"dl_TS_ChannelisationCodesShort.codesRepresentation choice is %d,invalid",(src_ptr->t));
    return FAILURE;
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_ts_and_code_r4()

DESCRIPTION   This function translates the Downlink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.32 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_ts_and_code_r4
(
  tdsrrc_DL_CCTrCh_r4_tdd128 *ie_ptr,
  tdsl1_dl_phycfg_cctrch_list_struct_type *cctrch_ptr
)
{
  uint8 i, j;
  tdsrrc_DownlinkTimeslotsCodes_LCR_r4_additionalTimeslots *tmp_ptr;
  tdsrrc_DownlinkAdditionalTimeslots_LCR_r4_newParameters *new_para_list_ptr;
  tdsl1_dl_phycfg_individual_ts_info_struct_type individual_ts_info_temp;

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
                    tdsrrc_DL_CCTrCh_r4_tdd128,
                    dl_CCTrCH_TimeslotsCodes))
  {
    cctrch_ptr->num_ts = 0;

    /* Get first timeslot info(10.3.6.37) */
    if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_indiv_r4(
                        &(ie_ptr->dl_CCTrCH_TimeslotsCodes.firstIndividualTimeslotInfo),
                        &(cctrch_ptr->individual_ts_info[0])))
    {
      return FAILURE;
    }

    /* Get first timeslot chan code info(10.3.6.17) */
    if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_channelcode_r4(
                      &(ie_ptr->dl_CCTrCH_TimeslotsCodes.dl_TS_ChannelisationCodesShort.codesRepresentation),
                      &(cctrch_ptr->individual_ts_info[0])))
    {
      return FAILURE;
    }

    /* we have get first timeslot info, so num add 1 */
    cctrch_ptr->num_ts += 1;

    /* Get more timeslot info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(
           ie_ptr->dl_CCTrCH_TimeslotsCodes.moreTimeslots,
           rrc_DownlinkTimeslotsCodes_LCR_r4_moreTimeslots_noMore))
    {
      /* no more means no data */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Receive ul_CCTrCH_TimeslotsCodes moreTimeslots noMore,no data");
    }
    else
    {
      tmp_ptr = ie_ptr->dl_CCTrCH_TimeslotsCodes.moreTimeslots.u.additionalTimeslots;

      /* Get consecutive times slot info */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(tmp_ptr,
            rrc_DownlinkTimeslotsCodes_LCR_r4_additionalTimeslots_consecutive))
      {
        /* consecutive value is 1~5 */
        if (tmp_ptr->u.consecutive >= TDSL1_DL_MAX_NUM_TS)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"consecutive is %d,range is 1~5.",tmp_ptr->u.consecutive);
          tmp_ptr->u.consecutive = TDSL1_DL_MAX_NUM_TS - 1;
        }

        cctrch_ptr->num_ts += tmp_ptr->u.consecutive;
        for (i = 1; i <= tmp_ptr->u.consecutive; i++)
        {
          memscpy(&cctrch_ptr->individual_ts_info[i],sizeof(tdsl1_dl_phycfg_individual_ts_info_struct_type),
                 &cctrch_ptr->individual_ts_info[0],
                 sizeof(tdsl1_dl_phycfg_individual_ts_info_struct_type)
                 );
         /* consecutive means continuous, so from first times slot,
             the following timeslots one by one be add 1. */
         /* DL ts_num is 0, 2, 3, 4, 5, 6, not include 1. So max ts_num is 6. */
          cctrch_ptr->individual_ts_info[i].ts_num =
            (cctrch_ptr->individual_ts_info[0].ts_num + i) % (TDSL1_DL_MAX_NUM_TS + 1);
          /* DL ts_num is 0, 2, 3, 4, 5, 6 */
          if (1 == cctrch_ptr->individual_ts_info[i].ts_num)
          {
            ERR_FATAL("ts_num is %d, DL ts_num is 0, 2, 3, 4, 5, 6",cctrch_ptr->individual_ts_info[i].ts_num,0,0);
            return FAILURE;
          }
        }
      }
      /* Get timeslot list info */
      else
      {
        /* timeslotList->n is 1~5 */
        if ((tmp_ptr->u.timeslotList->n) >= TDSL1_DL_MAX_NUM_TS)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"timeslotList->n is %d,range is 1~5.",tmp_ptr->u.consecutive);
          tmp_ptr->u.timeslotList->n = TDSL1_DL_MAX_NUM_TS - 1;
        }
        cctrch_ptr->num_ts += tmp_ptr->u.timeslotList->n;
        for (i = 0; i < tmp_ptr->u.timeslotList->n; i++)
        {
          /* Get parameters_sameAsLast info */
          if (TDSRRC_CHECK_COMMON_MSG_TYPE(
                tmp_ptr->u.timeslotList->elem[i].parameters,
                rrc_DownlinkAdditionalTimeslots_LCR_r4_parameters_sameAsLast))
          {
            memscpy(&cctrch_ptr->individual_ts_info[i + 1],sizeof(tdsl1_dl_phycfg_individual_ts_info_struct_type),
                   &cctrch_ptr->individual_ts_info[i],
                   sizeof(tdsl1_dl_phycfg_individual_ts_info_struct_type)
                   );
            /* DL ts_num is 0, 2, 3, 4, 5, 6 */
            if (1 == tmp_ptr->u.timeslotList->elem[i].parameters.u.sameAsLast->timeslotNumber)
            {
              ERR_FATAL("ts_num is %d, DL ts_num is 0, 2, 3, 4, 5, 6",tmp_ptr->u.timeslotList->elem[i].parameters.u.sameAsLast->timeslotNumber,0,0);
              return FAILURE;
            }
            cctrch_ptr->individual_ts_info[i + 1].ts_num=
              tmp_ptr->u.timeslotList->elem[i].parameters.u.sameAsLast->timeslotNumber;
          }
          /* Get parameters_newParameters info */
          else
          {
            new_para_list_ptr = tmp_ptr->u.timeslotList->elem[i].parameters.u.newParameters;
            if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_indiv_r4(
                             &new_para_list_ptr->individualTimeslotInfo,
                             &cctrch_ptr->individual_ts_info[i + 1]))
            {
              return FAILURE;
            }
            if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_channelcode_r4(
                              &new_para_list_ptr->dl_TS_ChannelisationCodesShort.codesRepresentation,
                              &cctrch_ptr->individual_ts_info[i + 1]))
            {
              return FAILURE;
            }
          }
        }
      }
    }
    for (i = 1; i < cctrch_ptr->num_ts; i++)
    {
      for (j = 0; j < cctrch_ptr->num_ts - i; j++)
      {
        if (cctrch_ptr->individual_ts_info[j].ts_num > cctrch_ptr->individual_ts_info[j + 1].ts_num)
        {
          individual_ts_info_temp = cctrch_ptr->individual_ts_info[j];
          cctrch_ptr->individual_ts_info[j] = cctrch_ptr->individual_ts_info[j + 1];
          cctrch_ptr->individual_ts_info[j + 1]= individual_ts_info_temp;
        }
      }
    }
  }

  return SUCCESS;

}
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_cctrch_list_r4()

DESCRIPTION   This function translates the Downlink CCTrCH List
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.21 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_cctrch_list_r4
(
  tdsrrc_DL_CCTrCh_r4 *ie_ptr,
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
)
{

  /* check If no "activation time" IE  for TDM, we can reject */
  if((ie_ptr->timeInfo.m.activationTimePresent == 0)&&
       ((ie_ptr->m.commonTimeslotInfoPresent == 1)&&
        (ie_ptr->commonTimeslotInfo.m.repetitionPeriodAndLengthPresent == 1)&&
        (ie_ptr->commonTimeslotInfo.repetitionPeriodAndLength.t 
                  != T_tdsrrc_RepetitionPeriodAndLength_repetitionPeriod1)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"no activation time for TDM,reject");
    return FAILURE;
  }

  /* Get Active time info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->timeInfo, tdsrrc_TimeInfo, activationTime))
  {
    dpch->dl_cctrch_list.time_info.activation_time =
                                   ie_ptr->timeInfo.activationTime;
  }
  else
  {
  /** from 10.3.6.83:
         Frame number start of the physical channel existence. Default value is "Now" */
    dpch->dl_cctrch_list.time_info.activation_time =
                                   TDSRRCLLC_ACTIVATION_TIME_NOW;
  }
  /* Get common timing slot info(10.3.6.10) */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,
                                      commonTimeslotInfo))
  {
    if(FAILURE == tdsrrcllcpcie_get_com_ts_info_r4(&ie_ptr->commonTimeslotInfo,
              &dpch->dl_cctrch_list.common_ts_info, TRUE))
    {
      return FAILURE;
    }
  }

  /* Get TPC list */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,
                                      ul_CCTrChTPCList))
  {
    /* 10.3.6.21  This list is not required for 1.28 Mcps TDD and is to be ignored by the UE. */
  }

  /* Get Downlink DPCH timeslots and codes info(10.3.6.32) */
  if (TDSRRC_CHECK_MSG_TYPE(ie_ptr->tddOption,
                    T_tdsrrc_DL_CCTrCh_r4_tddOption_tdd128))
  {
    if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_r4(ie_ptr->tddOption.u.tdd128,
                     &(dpch->dl_cctrch_list)))
    {
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Timing Advance Control is tdd384,not tdd128");
    return FAILURE;
  }

  return SUCCESS;
}

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
/*===========================================================================

FUNCTION          RRCCSP_DERIVE_FREQ_FROM_UARFCN

DESCRIPTION       This function derivces 

DEPENDENCIES      None

RETURN VALUE      Frequency in multiple of 100KHZ

SIDE EFFECTS      None
===========================================================================*/
uint16 tdsrrccsp_derive_freq_from_uarfcn
(
  tdsrrc_freq_type freq,
  tdsrrc_csp_band_class_type band_class
)
{
   uint16 fdl;
   
   fdl = freq * 2;

   TDSRRC_MSG3(MSG_LEGACY_MED,"tdsrrccsp_derive_freq_from_uarfcn: ignore band class %d for TDS. UARFCN %d => %d * 100KHz", 
               band_class, 
               freq, 
               fdl);

   return fdl;

#if 0
  int16 fdl_offset,fdl; /*multiples of 10k*/
  fdl_offset = 0;

  switch(band_class)
  {
    case RRC_CSP_IMT2K_BAND:
#ifdef FEATURE_WCDMA_BC9
    case RRC_CSP_WCDMA_1700_BAND:
#endif
      fdl_offset = RRC_DEFAULT_FDL_OFFSET;
      break;

    case RRC_CSP_PCS_BAND:
      if(freq >= RRC_DL_BAND_B_UARFCN_MIN &&
          freq <= RRC_DL_BAND_B_UARFCN_MAX)
      {
        fdl_offset = RRC_DEFAULT_FDL_OFFSET;
      }
      else
      {
        fdl_offset = RRC_FDL_OFFSET_BAND2_ADDL;
      }
       break;

#ifdef FEATURE_WCDMA_1800
    case RRC_CSP_WCDMA_1800_BAND:
      fdl_offset = RRC_FDL_OFFSET_BAND3;
      break;
#endif

#ifdef FEATURE_WCDMA_BC4
    case RRC_CSP_WCDMA_1700_2100_BAND:
      if(freq >= RRC_DL_BAND_H_UARFCN_MIN &&
      freq <= RRC_DL_BAND_H_UARFCN_MAX)
      {
        fdl_offset = RRC_FDL_OFFSET_BAND4;
      }
      else
      {
        fdl_offset = RRC_FDL_OFFSET_BAND4_ADDL;
      }
      break;
#endif

    case RRC_CSP_WCDMA_850_BAND:
    case RRC_CSP_WCDMA_800_BAND:
      if(freq >= RRC_DL_BAND_C_UARFCN_MIN &&
          freq <= RRC_DL_BAND_C_UARFCN_MAX)
      {
        /*If present in 850 band then for sure it will be present in 800 band, so check for 850 band range*/
        fdl_offset = RRC_DEFAULT_FDL_OFFSET;
      }
      else
      {
        fdl_offset = RRC_FDL_OFFSET_BAND5_BAND6_ADDL;
      }
      break;

#ifdef FEATURE_WCDMA_900
    case RRC_CSP_WCDMA_900_BAND:
      fdl_offset = RRC_FDL_OFFSET_BAND8;
      break;
#endif

#ifdef FEATURE_WCDMA_BC11
     case RRC_CSP_WCDMA_1500_BAND:
       fdl_offset = RRC_FDL_OFFSET_BAND11;
       break;
#endif
#ifdef FEATURE_WCDMA_BC19
     case RRC_CSP_WCDMA_BC19_BAND:
       if(freq >= RRC_DL_BAND_J_UARFCN_MIN &&
           freq <= RRC_DL_BAND_J_UARFCN_MAX)
       {
         fdl_offset = RRC_FDL_OFFSET_BAND19;
       }
       else
       {
         fdl_offset = RRC_FDL_OFFSET_BAND19_ADDL;
       }
       break;
#endif
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid band,use Fdl_offset as zero");
      fdl_offset = 0;
      break;
  }

  /*
    UARFCN*10 = 5*(FDL-FDLoffset)   FDL and FDLoffset are in integral multiples of 100KHZ
    FDL = 2*UARFCN + FDLoffset
  */

  fdl = (2*freq + fdl_offset);
  return fdl;
#endif
}
#endif


#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_tim_adv_contrl_r4_iho()

DESCRIPTION   This function translates the Uplink Timing Advance Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.96 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_tim_adv_contrl_r4_iho
(
  tdsrrc_UL_TimingAdvanceControl_LCR_r4 *ie_ptr
)
{

  if (ie_ptr->t == T_tdsrrc_UL_TimingAdvanceControl_LCR_r4_enabled)
  {
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_enabled = TRUE;

    /* Get ul_SynchronisationParameters */
    if (ie_ptr->u.enabled->m.ul_SynchronisationParametersPresent)
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 
                      ie_ptr->u.enabled->ul_SynchronisationParameters.frequency;
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 
                      ie_ptr->u.enabled->ul_SynchronisationParameters.stepSize;
    }
    else
    {
      /* from the 10.3.6.96:
        Default: Uplink synchronisation step size is 1. Uplink synchronisation frequency is 1. */
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 1;
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 1;
    }

    /* Get synchronisationParameters */
    if (ie_ptr->u.enabled->m.synchronisationParametersPresent)
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = TRUE;

      /* Get SYNC_UL codes bitmap */
      /* translate sync_UL_CodesBitmap string to uint8(refer to tdsrrc_translate_ura_id) */
      if (8 != ie_ptr->u.enabled->synchronisationParameters.sync_UL_CodesBitmap.numbits)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received sync_UL_CodesBitmap Length: %d is invalid",ie_ptr->u.enabled->synchronisationParameters.sync_UL_CodesBitmap.numbits);
      }

      /* Mask of SYNC_UL codes. Each bit indicates availability of a SYNC_UL code,
       * where the SYNC_UL codes are numbered "code0" to "code7". The value 1 of a 
       * bit indicates that the corresponding SYNC_UL code can be used */
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.ul_sync_code_mask = 
                     ie_ptr->u.enabled->synchronisationParameters.sync_UL_CodesBitmap.data[0];

      /* Get FPACH info */
      tdsrrcllcpcie_get_fpach_info_r4(&ie_ptr->u.enabled->synchronisationParameters.fpach_Info);

      /* Get prxUpPCHdes(10.3.6.96) info */
      /* -- Actual value prxUpPCHdes = IE value - 120 */
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des = 
                     ie_ptr->u.enabled->synchronisationParameters.prxUpPCHdes - 120;

      /* Get SYNC_UL procedure info */
      if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->u.enabled->synchronisationParameters, 
                                       tdsrrc_SynchronisationParameters_r4, sync_UL_Procedure))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx =
            tdsMaxSynUlTrans[ie_ptr->u.enabled->synchronisationParameters.sync_UL_Procedure.max_SYNC_UL_Transmissions];
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step =
            ie_ptr->u.enabled->synchronisationParameters.sync_UL_Procedure.powerRampStep;
      }
      else
      {
      /** from 10.3.6.96:
               Default is:Max SYNC_UL Transmission is 2. Power Ramp Step is 2.*/
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx = 2;
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step = 2;
      }
#ifdef FEATURE_TDSCDMA_HSUPA
      /* In 25.331,10.3.6.78d, IE 'PRXUpPCHdes', 'Power Ramp Step' and 'Max SYNC_UL Transmissions' are CV type,
                HSUPA scheduled E-RUCCH information will be affected if one of them changed*/
      if ((TRUE == tdscurrent_config_ptr->e_dch_transmission) && 
           (tdscurrent_config_ptr->hsupa_start_mask & TDSRRC_SCHED_START))
      {
        if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des != 
             tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.prx_upPch_des))
        {
          tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.prx_upPch_des =
             tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.prx_upPch_des;
          tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;
          tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);
        }
        if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx != 
             tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.max_sync_ul_tx))
        {
          tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.max_sync_ul_tx =
             tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.max_sync_ul_tx;
          tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;
          tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);   
        }
        if ((tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step != 
             tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.pwr_ramp_step))
        {
          tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.sync_ul_code_info_erucch.pwr_ramp_step =
             tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.pwr_ramp_step;
          tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_ERUCCH_INFO_INCL;
          tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);
        }
      }
#endif
    }
    else
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = FALSE;
    }
  }
  else
  {
    /* 8.6.6.26 : 1> if the IE "Uplink Timing Advance Control" has the value "disabled":
          2> reset timing advance to 0; */
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_enabled = FALSE;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 0;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 0;
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = FALSE;
    memset(&(tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms), 0,
        sizeof(tdsl1_ul_sync_parms_struct_type));
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_r4_iho()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.94a in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_r4_iho
(
  tdsrrc_UL_DPCH_InfoPostTDD_LCR_r4 *ie_ptr,
  tdsl1_ul_dpch_cfg_struct_type *dpch_cfg
)
{
  uint8      i, j;
  tdsrrc_UplinkTimeslotsCodes_LCR_r4_additionalTimeslots    *tmp_ie_ptr;
  tdsrrc_UplinkTimeslotsCodes_LCR_r4_timeslotList    *ts_list_short_ptr;
  tdsrrc_UplinkAdditionalTimeslots_LCR_r4_newParameters    *new_para_list_ptr;
  tdsl1_ul_dpch_ts_struct_type      ts_list_temp;

  dpch_cfg->dynamicSfUse = ie_ptr->ul_CCTrCH_TimeslotsCodes.dynamicSFusage;

  /* if OTA message has ul_CCTrCH_TimeslotsCode, num of timeslot be set. */
  dpch_cfg->num_ts = 0;

  /* Get First Individual timeslot info */
  if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4(&ie_ptr->
                  ul_CCTrCH_TimeslotsCodes.firstIndividualTimeslotInfo,
                  &dpch_cfg->ts_list[0]))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4 return error");
    return FAILURE;
  }

  if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4(&ie_ptr->
                  ul_CCTrCH_TimeslotsCodes.ul_TS_ChannelisationCodeList,
                  &dpch_cfg->ts_list[0]))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4 return error");
    return FAILURE;
  }
  /* we get first timeslot info, so num of timeslot add 1 */
  dpch_cfg->num_ts += 1;

  /* Get more timeslots info */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots,
          rrc_UplinkTimeslotsCodes_LCR_r4_moreTimeslots_noMore))
  {
    /* no more means no data */
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Receive ul_CCTrCH_TimeslotsCodes moreTimeslots noMore,no data");
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots,
            rrc_UplinkTimeslotsCodes_LCR_r4_moreTimeslots_additionalTimeslots))
  {
    tmp_ie_ptr = ie_ptr->ul_CCTrCH_TimeslotsCodes.moreTimeslots.u.additionalTimeslots;

    /* Get consecutive times slot info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(tmp_ie_ptr,
                           rrc_UplinkTimeslotsCodes_LCR_r4_additionalTimeslots_consecutive))
    {
      dpch_cfg->num_ts += tmp_ie_ptr->u.consecutive->numAdditionalTimeslots;

      for (i = 1; i <= tmp_ie_ptr->u.consecutive->numAdditionalTimeslots; i++)
      {
        memscpy(
            &(dpch_cfg->ts_list[i]),sizeof(tdsl1_ul_dpch_ts_struct_type),
            &(dpch_cfg->ts_list[0]),
            sizeof(tdsl1_ul_dpch_ts_struct_type)
        );
        /* consecutive means continuous, so from first times slot,
           the following timeslots one by one be add 1. */
        /* ts_num is 1~5, first ts_num does not start from 0. So max UL ts_num is 5, not 4. */
        dpch_cfg->ts_list[i].ts_num = ((dpch_cfg->ts_list[0].ts_num) + i) % 
                                   (TDSL1_UL_MAX_NUM_TS + 1);

        /* ts_num is 1~5 in UL */
        if ((0 == dpch_cfg->ts_list[i].ts_num) || (6 == dpch_cfg->ts_list[i].ts_num))
        {
          ERR_FATAL("ts_num is %d, UL ts_num is 1~5",dpch_cfg->ts_list[i].ts_num,0,0);
          return FAILURE;
        }
      }
    }
    /* Get timeslot list info */
    else if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(tmp_ie_ptr, rrc_UplinkTimeslotsCodes_LCR_r4_additionalTimeslots_timeslotList))
    {
      ts_list_short_ptr = tmp_ie_ptr->u.timeslotList;
      dpch_cfg->num_ts += ts_list_short_ptr->n;
      for (i = 0; i < ts_list_short_ptr->n; i++)
      {
        if (TDSRRC_CHECK_COMMON_MSG_TYPE(ts_list_short_ptr->elem[i].parameters,
              rrc_UplinkAdditionalTimeslots_LCR_r4_parameters_sameAsLast))
        {
        /* same as last means (i+1) same as (i). */
          memscpy(
                   &(dpch_cfg->ts_list[i + 1]),sizeof(tdsl1_ul_dpch_ts_struct_type),
                   &(dpch_cfg->ts_list[i]),
                   sizeof(tdsl1_ul_dpch_ts_struct_type)
          );
          /* ts_num is 1~5 in UL, But ASN1 value of UL timeslot number is 0~14 */
          if ((1 > ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber) ||
              (5 < ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber))
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"UL AdditionalTimeslot number is %d,But UL timeslot range is 1~5",dpch_cfg->ts_list[i].ts_num);
            ERR_FATAL("ts_num is %d, UL ts_num is 1~5",ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber,0,0);
            return FAILURE;
          }
          dpch_cfg->ts_list[i + 1].ts_num =
            ts_list_short_ptr->elem[i].parameters.u.sameAsLast->timeslotNumber;
        }
        else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ts_list_short_ptr->elem[i].parameters,
              rrc_UplinkAdditionalTimeslots_LCR_r4_parameters_newParameters))
        {
        /* new para means we get new values from second timeslot. */
          new_para_list_ptr = ts_list_short_ptr->elem[i].parameters.u.newParameters;
          if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4(
                           &(new_para_list_ptr->individualTimeslotInfo),
                           &(dpch_cfg->ts_list[i + 1])))
          {
            return FAILURE;
          }
          if (FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4(
                           &(new_para_list_ptr->ul_TS_ChannelisationCodeList),
                           &(dpch_cfg->ts_list[i + 1])))
          {
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"parameters choice is invalid");
          return FAILURE;
        }
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_CCTrCH_TimeslotsCodes.moreTimeslots.u.additionalTimeslots choice is invalid");
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ul_CCTrCH_TimeslotsCodes.moreTimeslots choice is invalid");
    return FAILURE;
  }
  for (i = 1; i < dpch_cfg->num_ts; i++)
  {
    for (j = 0; j < dpch_cfg->num_ts - i; j++)
    {
      if (dpch_cfg->ts_list[j].ts_num > dpch_cfg->ts_list[j + 1].ts_num)
      {
        ts_list_temp = dpch_cfg->ts_list[j];
        dpch_cfg->ts_list[j] = dpch_cfg->ts_list[j + 1];
        dpch_cfg->ts_list[j + 1]= ts_list_temp;
      }
    }
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION      tdsrrcllc_get_ul_dpch_info_r5_iho()

DESCRIPTION   This function translates the Uplink DPCH info common for
              all RL (radio links) IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_dpch_info_r5_iho
(
  tdsrrc_UL_DPCH_InfoPostTDD_LCR_r4 *ie_ptr
)
{

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Get Handover OTA msg UL DPCH Info");

  /* 2> if the IE "Uplink DPCH power control info" is included in the IE "Uplink DPCH info":
    3> if the IE " UL target SIR " is included ignore the parameter. */
  /* msg_ptr->specificationMode.u.preconfiguration.modeSpecificInfo
      .u.tdd.u.tdd128.ul_DPCH_Info.ul_DPCH_PowerControlInfo.ul_TargetSIR */

  // ul_OL_PC_Signalling->tpc_step_size,
  /** the value range of tpc_step_size is 1, 2, 3. The first num of tpc_step_size in OC struct is zero */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.tpc_step_size = TDSL1_TPC_STEP_SIZE_1DB;
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"tpc_step_size is not present,so set TDSL1_TPC_STEP_SIZE_1DB");

  if (ie_ptr->m.ul_TimingAdvancePresent)
  {
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_LOW,"This Handover OTA MSG include ul_TimingAdvanceControl");
    tdsrrcllcpcie_get_ul_tim_adv_contrl_r4_iho(&ie_ptr->ul_TimingAdvance);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"This Handover OTA MSG not include ul_TimingAdvanceControl");
    tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = FALSE;
  }

  //ul-CCTrCH
  /* Get ul-TargetSIR info */
  /* -- The IE ul-TargetSIR corresponds to PRX-DPCHdes for 1.28Mcps TDD
  -- Actual value PRX-DPCHdes = (value of IE "ul-TargetSIR" - 120) */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.prx_dpch_des= -84;
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"TargetSIR is not present, so set 36,then prx_dpch_des equal (-84)");


  /* Get Active time info */
  /** from 10.3.6.83: Frame number start of the physical channel existence. Default value is "Now" */
  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.act_time = TDSRRCLLC_ACTIVATION_TIME_NOW;

  if(FAILURE == tdsrrcllcpcie_get_ul_ts_and_code_r4_iho(ie_ptr,
            &tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg))
  {
    return FAILURE;
  }

  /* Indicate that Uplink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
  return SUCCESS;
} /* end tdsrrcllc_get_ul_dpch_info_r5_iho() */

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_ts_and_code_r4_iho()

DESCRIPTION   This function translates the Downlink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.32 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_ts_and_code_r4_iho
(
  tdsrrc_DownlinkTimeslotsCodes_LCR_r4 *ie_ptr,
  tdsl1_dl_phycfg_cctrch_list_struct_type *cctrch_ptr
)
{
  uint8 i, j;
  tdsrrc_DownlinkTimeslotsCodes_LCR_r4_additionalTimeslots *tmp_ptr;
  tdsrrc_DownlinkAdditionalTimeslots_LCR_r4_newParameters *new_para_list_ptr;
  tdsl1_dl_phycfg_individual_ts_info_struct_type individual_ts_info_temp;

  cctrch_ptr->num_ts = 0;

  /* Get first timeslot info(10.3.6.37) */
  if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_indiv_r4(
                        &(ie_ptr->firstIndividualTimeslotInfo),
                        &(cctrch_ptr->individual_ts_info[0])))
  {
    return FAILURE;
  }

  /* Get first timeslot chan code info(10.3.6.17) */
  if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_channelcode_r4(
                    &(ie_ptr->dl_TS_ChannelisationCodesShort.codesRepresentation),
                    &(cctrch_ptr->individual_ts_info[0])))
  {
    return FAILURE;
  }

  /* we have get first timeslot info, so num add 1 */
  cctrch_ptr->num_ts += 1;

  /* Get more timeslot info */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(
         ie_ptr->moreTimeslots,
         rrc_DownlinkTimeslotsCodes_LCR_r4_moreTimeslots_noMore))
  {
    /* no more means no data */
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Receive ul_CCTrCH_TimeslotsCodes moreTimeslots noMore,no data");
  }
  else
  {
    tmp_ptr = ie_ptr->moreTimeslots.u.additionalTimeslots;

    /* Get consecutive times slot info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(tmp_ptr,
          rrc_DownlinkTimeslotsCodes_LCR_r4_additionalTimeslots_consecutive))
    {
      /* consecutive value is 1~5 */
      if (tmp_ptr->u.consecutive >= TDSL1_DL_MAX_NUM_TS)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"consecutive is %d,range is 1~5.",tmp_ptr->u.consecutive);
        tmp_ptr->u.consecutive = TDSL1_DL_MAX_NUM_TS - 1;
      }

      cctrch_ptr->num_ts += tmp_ptr->u.consecutive;
      for (i = 1; i <= tmp_ptr->u.consecutive; i++)
      {
        memscpy(&cctrch_ptr->individual_ts_info[i],sizeof(tdsl1_dl_phycfg_individual_ts_info_struct_type),
               &cctrch_ptr->individual_ts_info[0],
               sizeof(tdsl1_dl_phycfg_individual_ts_info_struct_type)
               );
       /* consecutive means continuous, so from first times slot,
           the following timeslots one by one be add 1. */
       /* DL ts_num is 0, 2, 3, 4, 5, 6, not include 1. So max ts_num is 6. */
        cctrch_ptr->individual_ts_info[i].ts_num =
          (cctrch_ptr->individual_ts_info[0].ts_num + i) % (TDSL1_DL_MAX_NUM_TS + 1);
        /* DL ts_num is 0, 2, 3, 4, 5, 6 */
        if (1 == cctrch_ptr->individual_ts_info[i].ts_num)
        {
          ERR_FATAL("ts_num is %d, DL ts_num is 0, 2, 3, 4, 5, 6",cctrch_ptr->individual_ts_info[i].ts_num,0,0);
          return FAILURE;
        }
      }
    }
    /* Get timeslot list info */
    else
    {
      /* timeslotList->n is 1~5 */
      if ((tmp_ptr->u.timeslotList->n) >= TDSL1_DL_MAX_NUM_TS)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"timeslotList->n is %d,range is 1~5.",tmp_ptr->u.consecutive);
        tmp_ptr->u.timeslotList->n = TDSL1_DL_MAX_NUM_TS - 1;
      }
      cctrch_ptr->num_ts += tmp_ptr->u.timeslotList->n;
      for (i = 0; i < tmp_ptr->u.timeslotList->n; i++)
      {
        /* Get parameters_sameAsLast info */
        if (TDSRRC_CHECK_COMMON_MSG_TYPE(
              tmp_ptr->u.timeslotList->elem[i].parameters,
              rrc_DownlinkAdditionalTimeslots_LCR_r4_parameters_sameAsLast))
        {
          memscpy(&cctrch_ptr->individual_ts_info[i + 1],sizeof(tdsl1_dl_phycfg_individual_ts_info_struct_type),
                 &cctrch_ptr->individual_ts_info[i],
                 sizeof(tdsl1_dl_phycfg_individual_ts_info_struct_type)
                 );
          /* DL ts_num is 0, 2, 3, 4, 5, 6 */
          if (1 == tmp_ptr->u.timeslotList->elem[i].parameters.u.sameAsLast->timeslotNumber)
          {
            ERR_FATAL("ts_num is %d, DL ts_num is 0, 2, 3, 4, 5, 6",tmp_ptr->u.timeslotList->elem[i].parameters.u.sameAsLast->timeslotNumber,0,0);
            return FAILURE;
          }
          cctrch_ptr->individual_ts_info[i + 1].ts_num=
            tmp_ptr->u.timeslotList->elem[i].parameters.u.sameAsLast->timeslotNumber;
        }
        /* Get parameters_newParameters info */
        else
        {
          new_para_list_ptr = tmp_ptr->u.timeslotList->elem[i].parameters.u.newParameters;
          if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_indiv_r4(
                           &new_para_list_ptr->individualTimeslotInfo,
                           &cctrch_ptr->individual_ts_info[i + 1]))
          {
            return FAILURE;
          }
          if (FAILURE == tdsrrcllcpcie_get_dl_ts_and_code_channelcode_r4(
                            &new_para_list_ptr->dl_TS_ChannelisationCodesShort.codesRepresentation,
                            &cctrch_ptr->individual_ts_info[i + 1]))
          {
            return FAILURE;
          }
        }
      }
    }
  }
  for (i = 1; i < cctrch_ptr->num_ts; i++)
  {
    for (j = 0; j < cctrch_ptr->num_ts - i; j++)
    {
      if (cctrch_ptr->individual_ts_info[j].ts_num > cctrch_ptr->individual_ts_info[j + 1].ts_num)
      {
        individual_ts_info_temp = cctrch_ptr->individual_ts_info[j];
        cctrch_ptr->individual_ts_info[j] = cctrch_ptr->individual_ts_info[j + 1];
        cctrch_ptr->individual_ts_info[j + 1]= individual_ts_info_temp;
      }
    }
  }

  return SUCCESS;

}

/*===========================================================================
FUNCTION        tdsrrcllc_get_dl_info_per_rl_r5_iho()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_per_rl_r5_iho
(
  tdsrrc_DL_InformationPerRL_PostTDD_LCR_r4 *ie_ptr
)
{
  tdsordered_config_ptr->cpid_present = TRUE;
  tdsordered_config_ptr->cell_param_id = ie_ptr->primaryCCPCH_Info.cellParametersID;

  tdsordered_config_ptr->l1_dl_chan_parms.pccpch.tstd_indicator =
                      ie_ptr->primaryCCPCH_Info.tstd_Indicator;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Get Handover OTA msg dl_DPCH_InfoPerRL");

  /* active time is MD, is now.*/
  tdsordered_config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.time_info.activation_time =
                                     TDSRRCLLC_ACTIVATION_TIME_NOW;
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"active time of dl-CCTrCH is not present, so set default value,now");

  if (tdsrrcllcpcie_get_dl_ts_and_code_r4_iho(&ie_ptr->dl_DPCH_InfoPerRL.dl_CCTrCH_TimeslotsCodes,
        &tdsordered_config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list) == FAILURE)
  {
    return FAILURE;
  }

  /* Indicate that Downlink L1 needs to be configured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;

  /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;

#ifdef FEATURE_TDSCDMA_HSUPA
  /* Call below function to wipe out any stored L1 information on EUL */
  /* either inter HHO or intra HHO */
  if (tdsordered_config_ptr->is_hho) 
  {
    tdsrrcllcpcie_clean_l1_e_info_on_non_r7_hho();
  }
#endif /* FEATURE_TDSCDMA_HSUPA */

  return SUCCESS;

} /* end tdsrrcllc_get_dl_info_per_rl_r5_iho() */

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_common_for_all_rl_iho()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_info_common_for_all_rl_iho
(
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch,
  tdsrrc_U_RNTI_Short *new_urnti_ptr
)
{
  uint16     srnti2  = 0; /* S-RNTI 2 portion of the U-RNTI in the message,
                             used later to calculate the default DPCH offset
                             value in preconfigured mode */
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Info Common for all RL");


  /* Check if HS Reset is indicated */
  /* TBD
  if (TDSRRC_CHECK_MSG_TYPE_IE_PTR(ie_ptr,
                mac_hsResetIndicatorPresent))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSReset Indicator TRUE");

    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = ???;
    tdshsdpa_msg_params.msg_params_present = TRUE;
  }
*/
  /* Get the info common for all DL DPCH if present */
  dpch->dpch_common.opt_parm_present_mask = 0;
  dpch->dpch_common.timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_INIT;
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"cfnHndling of dl-CommonInformation is not present,so set initial");

  dpch->dpch_common.tpc_step_size = TDSL1_TPC_STEP_SIZE_1DB;
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"tpc_step_size of dl-CommonInformation is not present,so set 1");

  /* Indicate the DPCH Pwr Ctrl presence in the bitmask */
  dpch->dpch_common.opt_parm_present_mask |=
    TDSL1_DL_PHYCFG_DPCH_COMMON_INFO_TPC_STEP_SIZE_PRESENT;

  /* Get tstd indicator */
  dpch->dpch_common.tstd_indicator = FALSE;
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"tstd_indicator of dl-CommonInformation is not present,so set FASLE");

  /* DefaultDPCH-OffsetValueTDD ::= INTEGER (0..7) */
  srnti2 = *(new_urnti_ptr->s_RNTI_2.data) << 8;
  srnti2 |= *(new_urnti_ptr->s_RNTI_2.data + 1);
  srnti2 >>= 6;

  dpch->doff = (srnti2 % 7); /* 25.331, clause 8.3.6.3 */

  if(dpch->doff > TDSRRCLLC_ASN1_DOFF_MAX_VAL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Doff ASN1 value out of range");
    return(FAILURE);
  }
  dpch->opt_parm_presence_bitmask |= TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;

  /*For Non-DCH -> DCH case, make DOFF = 0.*/
  if((rrc_state != TDSRRC_STATE_CELL_DCH)&&(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH))
  {
    dpch->doff = 0;
    dpch->opt_parm_presence_bitmask |=
            TDSL1_DL_PHYCFG_DEFAULT_DPCH_OFFSET_PRESENT;
  }

  /* Indicate that Downlink L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
 /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  return SUCCESS;
} /* tdsrrcllcpcie_get_dl_info_common_for_all_rl_r6() */
#endif


