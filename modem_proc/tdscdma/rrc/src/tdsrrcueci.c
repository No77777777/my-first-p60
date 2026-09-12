/*===========================================================================
                   R R C  U E  C A P A B I L I T Y  I N F O R M A T I O N

DESCRIPTION
  This header file contains function prototypes for UE Capability Information
  procedure

EXTERNALIZED FUNCTIONS
  tdsrrcueci_procedure_event_handler
    This function is the event handler for all events that are sent to the
    IUE Capability Information procedure.

  tdsrrcueci_init_procedure
    This function initializes required parameters for UE Capability Information
    procedure.



INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcueci.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why 
--------   ---     ----------------------------------------------------------
11/24/11   zwj     Add support band E and F for R5 UE Capability.
06/14/11   yzh     Made R9 changes to set IE "UE RadioAccessCapability"(FDD->TDD).
03/10/11   bj      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/24/10   yzh     Add ((FEATURE_TDSCDMA_64QAM) || (FEATURE_TDSCDMA_MIMO)) in tdsrrc_macehs_cat
                   which set value in FEATURE_TDSCDMA_64QAM or FEATURE_TDSCDMA_MIMO.
11/17/10   bj      Made changes to set IE "UE RadioAccessCapability"(FDD->TDD).
11/11/10   yzh     (3GPP: 8.6.6.28a)Del Downlink F-DPCH info common for all radio links(FDD).
11/10/10   bj       Delete IE "Acceptance of requested change of capability".

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "rex.h"
#include "tdsrrccmd_v.h"              
#include "tdsrrcuece.h"
#include "tdsrrcueci.h"
#include "tdsrrcsmc.h"
#include "msg.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "tdsrrcdata.h"
#include "tdsrrclcm.h"
#include "tdsrrcsend.h"
#include "tdsrrcscmgr.h"
#include "tdsrrctmr.h"
#include "tdsrrccu.h"
#include "tdsrrcsibdb.h"
#include "tdsuecomdef.h"
#include "libprot.h"
#include "tdsrrcmcm.h"
#include "sys_stru.h"
#include "tdsrrccsp.h"
#include "tdsrrcrcr.h"
#include "tdsl1rrcif.h"
#include "tdsrrcasn1util.h"
#include "tdsrrcnv.h"
#include "mm_umts.h"
#include "sys_plmn.h"
/*#include "rr_rrc_if.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"
#ifdef FEATURE_TDS_DC_HSDPA
#ifdef FEATURE_TDSCDMA_BAND_SPEC_DC_HSDPA
#include "rfm_wcdma.h"
#include "tdsrrcccm.h"
#endif
#endif
#include "rr_multiband.h"

#include "tdsrrcnv.h"

#include "tdsrrclogging.h"


#ifdef FEATURE_TDSCDMA_TO_LTE
#include "tdsCFAParser.h"
#include "tdsCFAParser_i.h"
#endif
#include "tdsrrcwrm.h"
#include "tdsrrcmisc.h"
#include "tdsrrcccm.h"

#include "bit.h"

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
#include <lte_rrc_ext_api.h>
#endif

#include "assert.h"
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSRRCUECI_INVALID_TRANSACTION_ID 0xFF

#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
/*A variable used to track support of snow3g_security_algo support, by default set to FALSE*/
boolean    tdssnow3g_security_algo_supported = FALSE;
#endif

/*Boolean added to track if interfrequency and interrat measurements are to be processed or not*/
extern boolean tdsprocess_interfreq_interrat_meas_rel7;

extern boolean tdsprocess_interfreq_uei_meas_rel7;

#ifdef FEATURE_UMTS_PDCP
extern boolean tdsrrc_pdcp_enabled;
#endif /* FEATURE_UMTS_PDCP */

uint8 tdsnv_hsdpa_category;

/*Setting the default MAC-HS cat as 8 when R7 is defined.*/
uint8 tdsrrc_machs_cat = 10;

/*Various HSDPA Categories
13, 14 - 64QAM only
15, 16 - MIMO Only
17, 18 - Both 64QAM and MIMO supported but configured at a time
19, 20 - Both 64QAM and MIMO supported. And can be configured both
21, 22, 23, 24 - DC-HSDPA supported
*/
/*RRC_GKG: Will make changes for MIMO/64AQM etc validations.*/
#ifdef FEATURE_TDSCDMA_REL8
#if defined(FEATURE_TDSCDMA_64QAM) && defined(FEATURE_TDSCDMA_MIMO)
uint8 tdsrrc_macehs_cat = 18;
#elif defined(FEATURE_TDSCDMA_MIMO) && !defined(FEATURE_TDSCDMA_64QAM)
uint8 tdsrrc_macehs_cat = 16;
#elif defined FEATURE_TDSCDMA_64QAM
uint8 tdsrrc_macehs_cat = 14;
#else
/*temporary solution, need to remove when implemenation 64QAM*/
uint8 tdsrrc_macehs_cat = 13;
#endif
#endif /*FEATURE_TDSCDMA_REL8*/


#ifdef FEATURE_TD2G_NACC
boolean nv_td2g_nacc_enabled = FALSE;
#define RRC_TD2G_NACC_SUPPORTED 0x01
#endif

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA 
/* defined in tdsrrcgpsmeas.c */
extern tdsrrcgps_cgps_ue_pos_capability_cb_type  tdsrrcgps_cgps_ue_pos_capability_cb;
#endif
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*UE position measurement validity in PCH states*/
boolean tdsrrc_ue_pos_cap_pch = FALSE;
#endif

#if defined (FEATURE_TDSCDMA_64QAM) || defined (FEATURE_TDSCDMA_MIMO)
boolean tdsrrc_mimo_64qam_cm_enabled = FALSE;
#endif
#define TDSCLASSMARK3_DATA_MAX_LENGTH 14 /* Referenced from structure classmark_3_ie_T in sys_stru.h */

extern boolean tds_nv_mmcp_enable_thin_ui_config;

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/*==========================================================================
  Type for defining the substates of UE Capability Information Procedure.
===========================================================================*/

typedef enum 
{
  TDSRRCUECI_INITIAL,               /* UECI initial substate      */
  TDSRRCUECI_WAIT_FOR_CELL_UPDATE_COMP_CNF,
                                 /* UECI waits for CELL UPDATE
                                    procedure to complete */
  TDSRRCUECI_WAIT_FOR_T304_TO_EXPIRE 
                                 /* UECI waits for T304 to expire */
} tdsrrcueci_substate_e_type;

#ifdef FEATURE_TDSCDMA_TO_LTE
typedef struct
{
  boolean lte_req_posted;
  boolean lte_rsp_received;

  uint32 cap_cont_length;
  byte   *cap_cont_ptr;
} tdsrrcueci_interrat_lte_capability_info_type;
#endif



/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* This variable holds substate for UE Capability Information Procedure
 */
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcueci_substate_e_type tdsrrcueci_substate;

boolean tdsrrcueci_support_prio_based_resel;

static tdsrrc_RRC_TransactionIdentifier tdsrrcueci_ue_cap_enquiry_trans_id;
                                      /* This stores the transaction Id 
                                         for UE Capability Enquiry message */

#ifdef FEATURE_TDSCDMA_REL7_SPECRS
static  tdsrrc_ue_capability_originator_e_type tdsrrcueci_ue_cap_originator;                                   
#endif

#ifdef FEATURE_TDSCDMA_TO_LTE
tdsrrcueci_interrat_lte_capability_info_type tdsrrcueci_interrat_lte_capability_info;
#endif




/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================
FUNCTION tdsrrcueci_reset_ueci_procedure

DESCRIPTION
  This resets the UECI procedure i.e. stops T304 timer, reset the value of variables
  used in this procedure to default values.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcueci_reset_ueci_procedure(void)
{
  /* Set conter V304 to zero */
  tdsrrc_v_304 = 0;

  /* Stop T304 timer - no need to check whether this timer is running or not because its
       taken care inside this function 
  */
  tdsrrctmr_stop_timer(TDSRRCTMR_T_304_TIMER);

  /* Reset the UECI state machine */
  tdsrrcueci_substate = TDSRRCUECI_INITIAL;

#ifdef FEATURE_TDSCDMA_REL7_SPECRS
  tdsrrcueci_ue_cap_originator = TDSRRC_UE_CAPABILITY_ORIGINATOR_NONE;
#endif
#ifdef FEATURE_TDSCDMA_TO_LTE
  tdsrrcueci_interrat_lte_capability_info.lte_req_posted = tdsrrcueci_interrat_lte_capability_info.lte_rsp_received = FALSE;
#endif



}

/*===========================================================================
FUNCTION tdsrrcueci_get_support_am_entity

DESCRIPTION
  This function appends the Radio Access Capability to the message.
DEPENDENCIES
  None

RETURN VALUE                        h
  None

SIDE EFFECTS
  None
===========================================================================*/
static tdsrrc_MaximumAM_EntityNumberRLC_Cap tdsrrcueci_get_support_am_entity(void)
{
  tdsrrc_MaximumAM_EntityNumberRLC_Cap ueci_am_entity = tdsrrc_MaximumAM_EntityNumberRLC_Cap_dummy;
  uint8  support_am_entity = TDSUE_MAX_AM_ENTITY;

  switch (support_am_entity)
  {
    case UE_MAX_NUM_OF_AM_ENTITIES_4:
      ueci_am_entity = tdsrrc_MaximumAM_EntityNumberRLC_Cap_am4;
      break;
    case UE_MAX_NUM_OF_AM_ENTITIES_5: 
      ueci_am_entity = tdsrrc_MaximumAM_EntityNumberRLC_Cap_am5;
      break;
    case UE_MAX_NUM_OF_AM_ENTITIES_6: 
      ueci_am_entity = tdsrrc_MaximumAM_EntityNumberRLC_Cap_am6;
      break;
    case UE_MAX_NUM_OF_AM_ENTITIES_7:
    case UE_MAX_NUM_OF_AM_ENTITIES_8:
      ueci_am_entity = tdsrrc_MaximumAM_EntityNumberRLC_Cap_am8;
      break;
    default:
      {
        ueci_am_entity = (support_am_entity <= UE_MAX_NUM_OF_AM_ENTITIES_16) ? tdsrrc_MaximumAM_EntityNumberRLC_Cap_am16 : tdsrrc_MaximumAM_EntityNumberRLC_Cap_am30;
        break;
      }
  }
  
  return ueci_am_entity;
}

/*===========================================================================
FUNCTION tdsrrcueci_verify_security_capability

DESCRIPTION
  This function checks the Security Capability.
DEPENDENCIES
  None

RETURN VALUE
  Returns TRUE if check is successful. Otherwise returns FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcueci_verify_security_capability
( 
  tdsrrc_SecurityCapability * securityCapability
)
{
  boolean status = TRUE;  /* local varibale for status */
  uint32 int_algo_bit_mask = 0;
  uint32 cipher_algo_bit_mask = 0;
  
  /*lint -save -e778 */
  if (tdsnv_ciphering_enabled_status) 
  {
    cipher_algo_bit_mask = TDSUEA_1_SUPPORTED + TDSUEA_0_SUPPORTED;
#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
    if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) && tdssnow3g_security_algo_supported)
    {
      cipher_algo_bit_mask += TDSUEA_2_SUPPORTED;
    }
#endif
    /*lint -save -e572 */
    if ( (securityCapability->cipheringAlgorithmCap.numbits != 16) ||
         ( *(securityCapability->cipheringAlgorithmCap.data) !=
            (uint8)( (cipher_algo_bit_mask & 0xFF00) >> 8)) || 
         ( *(securityCapability->cipheringAlgorithmCap.data+1) != 
            (uint8)( cipher_algo_bit_mask & 0xFF) ) )
    /*lint -restore */
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NV Ciph ON.Security Cap. Check Failed");
      status = FALSE;
    }
    else
    { 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NV Ciph ON.Security Cap. Check Pass");
    }
  }
  else
  {
    /*lint -save -e572 */
    if ( (securityCapability->cipheringAlgorithmCap.numbits != 16) ||
         ( *(securityCapability->cipheringAlgorithmCap.data) !=
            ((TDSUEA_0_SUPPORTED & 0xFF00) >> 8)) || 
         ( *(securityCapability->cipheringAlgorithmCap.data+1) != 
            (TDSUEA_0_SUPPORTED & 0xFF)) )
    /*lint -restore */
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NV Ciph OFF. Security Cap. Check Failed");
      status = FALSE;
    }
    else
    { 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NV Ciph OFF. Security Cap. Check Pass");  
    }
  }
  if( (tdsnv_integrity_enabled) && (status != FALSE) )
  { /*lint -save -e572 */
    int_algo_bit_mask = TDSKASUMI_SUPPORTED;
#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
    if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) && tdssnow3g_security_algo_supported)
    {
      int_algo_bit_mask += TDSSNOW_3G_SUPPORTED;
    }
#endif
    if ( (securityCapability->integrityProtectionAlgorithmCap.numbits != 16) ||
         ( *(securityCapability->integrityProtectionAlgorithmCap.data) !=
            (uint8)( (int_algo_bit_mask & 0xFF00) >> 8)) || 
         ( *(securityCapability->integrityProtectionAlgorithmCap.data+1) != 
            (uint8)( int_algo_bit_mask & 0xFF) ) )
    /*lint -restore */
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Security Cap. Check Failed");
      status = FALSE;

    }
    else
    { 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Security Cap. Check Pass");
      
    }

  }
  /*lint -restore */
  return (status);
}

/*===========================================================================
FUNCTION tdsrrcueci_verify_inter_rat_security_capability

DESCRIPTION
  This function checks the GSM Specific Capability.
DEPENDENCIES
  None

RETURN VALUE
  Returns TRUE if check is successful. Otherwise returns FALSE.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcueci_verify_inter_rat_security_capability
(
  tdsrrc_InterRAT_UE_SecurityCapList * ue_SystemSpecificSecurityCap
)
/*lint -e715*/
{
  /* This dummy fucntion */
  return TRUE;
}

/*===========================================================================

FUNCTION        TDSRR_IS_SUPPORTED_BAND_INTERNAL

DESCRIPTION     This function will call appropriate RR function based on DSDS flag

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
static boolean tdsrr_is_supported_band_internal( rr_internal_band_T band)
{
#ifdef FEATURE_DUAL_SIM
  return  rr_ds_is_supported_band(band, tdsrrc_get_as_id());
#else
  return  rr_is_supported_band(band);
#endif
}

/*lint +e715*/

/*===========================================================================
FUNCTION tdsrrcueci_append_ue_radio_access_capability_non_ext

DESCRIPTION
  This function appends the Radio Access Capability to the message.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcueci_append_ue_radio_access_capability_non_ext
(
  tdsrrc_UE_RadioAccessCapability *ue_radio_access_capability_ptr,  
                                                /* Pointer to UE Radio Access Capability */
  boolean irat_ho_info,
  tdsrrcueci_internal_cap_update_cause_e_type update_cause  /* Capability query cause*/
  
)
{
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
  tm_umts_cp_tds_ue_pos_capability_struct_type pos_capability;
#endif

  uint32 int_algo_bit_mask = 0;
  uint32 cipher_algo_bit_mask = 0;

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
  /*Reset and update from position capability*/
  tdsrrc_ue_pos_cap_pch = FALSE;
#endif
  /* Check whether Radio Access capability is a valid pointer */
  if(ue_radio_access_capability_ptr != NULL)
  {

    /* only for capability update send measurement info.. for 
       HO INFO transfer, this will be sent in extension */
    if(update_cause == TDSRRCUECI_INT_CAP_UPDATE_2100)
    {
      /*Measurement Capability is supported */
     TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(ue_radio_access_capability_ptr,
        tdsrrc_UE_RadioAccessCapability,measurementCapability);
    }
    else
    {
      /*Measurement Capability is supported */
      TDSRRC_RESET_MSG_IE_PRESENT_PTR(ue_radio_access_capability_ptr);
    }

    /*UE-RadioAccessCapability is compatible with R99 , although accessStratumReleaseIndicator 
     * is removed from this IE , since its encoding did not does in bits . The 
     * accessStratumReleaseIndicator is provided in the relevant REL-4 extension IEs . */

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)

    /* Stand alone Location Methods are not supported */
    ue_radio_access_capability_ptr->ue_positioning_Capability.
      standaloneLocMethodsSupported = FALSE;

    /* Network assisted GPS is not supported */
    ue_radio_access_capability_ptr->ue_positioning_Capability.
      networkAssistedGPS_Supported = tdsrrc_NetworkAssistedGPS_Supported_noNetworkAssistedGPS;
  
#else
    /* Stand alone Location Methods are not supported */
    ue_radio_access_capability_ptr->ue_positioning_Capability.
      standaloneLocMethodsSupported = FALSE;

    /* Network assisted GPS is not supported */
    ue_radio_access_capability_ptr->ue_positioning_Capability.
      networkAssistedGPS_Supported = tdsrrc_NetworkAssistedGPS_Supported_noNetworkAssistedGPS;
  
    /* Capability to measure GPS Reference is not supported */
    ue_radio_access_capability_ptr->ue_positioning_Capability.
       supportForUE_GPS_TimingOfCellFrames = FALSE;
#endif

    /*Capability to use IPDL is not supported */
    ue_radio_access_capability_ptr->ue_positioning_Capability.
      supportForIPDL = FALSE;

    /* UE Based OTDOA is not supported */
    ue_radio_access_capability_ptr->ue_positioning_Capability.
      ue_BasedOTDOA_Supported = FALSE;
   
    /* only for capability update send measurement info.. for 
       HO INFO transfer, this will be sent in extension */
    if(update_cause == TDSRRCUECI_INT_CAP_UPDATE_2100)
    {    
      /* Update Measurement Capability.
       */  
      /* Downlink FDD,GSM and Multi Carrier Measurements are not supported */
      TDSRRC_RESET_MSG_IE_PRESENT(ue_radio_access_capability_ptr->measurementCapability.
        downlinkCompressedMode);
  
      if (tdsrrcmcm_is_dualmode_enabled())
      {
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ue_radio_access_capability_ptr->measurementCapability.
          downlinkCompressedMode,gsm_Measurements);
  
        //query rr for 900E/P band support and set the 900 supported flag to true
        if(tdsrr_is_supported_band_internal(BAND_EGSM) || tdsrr_is_supported_band_internal(BAND_PGSM))
        {
          ue_radio_access_capability_ptr->measurementCapability.
            downlinkCompressedMode.gsm_Measurements.gsm900 = TRUE;
        }
  
        //set appropriate flags for dcs and pcs bands..  850 band does not have an enum
        //hence not included in this bitmask
        ue_radio_access_capability_ptr->measurementCapability.
          downlinkCompressedMode.gsm_Measurements.dcs1800 = tdsrr_is_supported_band_internal(BAND_DCS);
  
        ue_radio_access_capability_ptr->measurementCapability.
          downlinkCompressedMode.gsm_Measurements.gsm1900 = tdsrr_is_supported_band_internal(BAND_PCS);
      }
    
      /* Downlink FDD Mesaurements are not supported */
      ue_radio_access_capability_ptr->measurementCapability.
        downlinkCompressedMode.fdd_Measurements = FALSE;

      /* Uplink FDD,GSM and Multi Carrier Measurements are not supported */
      TDSRRC_RESET_MSG_IE_PRESENT(ue_radio_access_capability_ptr->measurementCapability.
        uplinkCompressedMode);
  
      if (tdsrrcmcm_is_dualmode_enabled())
      {
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ue_radio_access_capability_ptr->measurementCapability.
          uplinkCompressedMode,gsm_Measurements);
  
        ue_radio_access_capability_ptr->measurementCapability.uplinkCompressedMode.
          gsm_Measurements.gsm900 = 
          ue_radio_access_capability_ptr->measurementCapability.downlinkCompressedMode.
          gsm_Measurements.gsm900;
  
        ue_radio_access_capability_ptr->measurementCapability.uplinkCompressedMode.
          gsm_Measurements.dcs1800 =
          ue_radio_access_capability_ptr->measurementCapability.downlinkCompressedMode.
          gsm_Measurements.dcs1800;

        ue_radio_access_capability_ptr->measurementCapability.uplinkCompressedMode.
          gsm_Measurements.gsm1900 =
          ue_radio_access_capability_ptr->measurementCapability.downlinkCompressedMode.
          gsm_Measurements.gsm1900;

      }
  
      /* Uplink FDD Mesaurements are not supported */
      ue_radio_access_capability_ptr->measurementCapability.
        uplinkCompressedMode.fdd_Measurements = FALSE;
    }

#ifdef FEATURE_UMTS_PDCP
    if(tdsrrc_pdcp_enabled)
    { 
      /* Update PDCP Capability   
       */
      /* Lossless SRNS Relocation is not supported */
      ue_radio_access_capability_ptr->pdcp_Capability.
        losslessSRNS_RelocationSupport = FALSE;

      /* RFC2507 is not supported */
      ue_radio_access_capability_ptr->pdcp_Capability.
        supportForRfc2507.t = T_tdsrrc_PDCP_Capability_supportForRfc2507_supported;

      ue_radio_access_capability_ptr->pdcp_Capability.
        supportForRfc2507.u.supported = tdsrrc_MaxHcContextSpace_by1024;
    }
    else  
#endif /*FEATURE_UMTS_PDCP*/
    { 
      /* Update PDCP Capability   
       */
      /* Lossless SRNS Relocation is not supported */
      ue_radio_access_capability_ptr->pdcp_Capability.
        losslessSRNS_RelocationSupport = FALSE;

      /* RFC2507 is not supported */
      ue_radio_access_capability_ptr->pdcp_Capability.
        supportForRfc2507.t = T_tdsrrc_PDCP_Capability_supportForRfc2507_notSupported;

    }

    /* Physiscal Channel Capability is sent in ext */
     TDSRRC_RESET_MSG_IE_PRESENT(ue_radio_access_capability_ptr->physicalChannelCapability);
      /* RF capability is sent in ext */
      TDSRRC_RESET_MSG_IE_PRESENT(ue_radio_access_capability_ptr->rf_Capability);

    if(irat_ho_info)
    {
      /* Physiscal Channel Capability */
      ue_radio_access_capability_ptr->physicalChannelCapability.m.tddPhysChCapabilityPresent = 1;
      
      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        uplinkPhysChCapability.maxPhysChPerTimeslot = tdsrrc_MaxPhysChPerTimeslot_ts2;
      
      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        uplinkPhysChCapability.maxTS_PerFrame = 5;
      
      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        uplinkPhysChCapability.minimumSF = tdsrrc_MinimumSF_UL_sf1;
      
      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        uplinkPhysChCapability.supportOfPUSCH = FALSE;


      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        downlinkPhysChCapability.maxPhysChPerFrame = 64;
      
      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        downlinkPhysChCapability.maxPhysChPerTS = 16;
      
      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        downlinkPhysChCapability.maxTS_PerFrame = 4;
      
      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        downlinkPhysChCapability.minimumSF = tdsrrc_MinimumSF_DL_sf1;
      
      ue_radio_access_capability_ptr->physicalChannelCapability.tddPhysChCapability.
        downlinkPhysChCapability.supportOfPDSCH = FALSE;

      /* RF capability */
      ue_radio_access_capability_ptr->rf_Capability.m.tddRF_CapabilityPresent = 1;
      
      ue_radio_access_capability_ptr->rf_Capability.tddRF_Capability.
        chipRateCapability = tdsrrc_ChipRateCapability_mcps1_28;
      
      ue_radio_access_capability_ptr->rf_Capability.tddRF_Capability.
        radioFrequencyTDDBandList = tdsrrc_RadioFrequencyBandTDDList_a;
      
      ue_radio_access_capability_ptr->rf_Capability.tddRF_Capability.
        ue_PowerClass = 2;
      
    }

    /* Update RLC Capability */
    /* Maximum no of AM entities supported is 4 */
    ue_radio_access_capability_ptr->rlc_Capability.
      maximumAM_EntityNumber = tdsrrcueci_get_support_am_entity();
    if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
      /* Total AM buffer size is 1000KB */
      ue_radio_access_capability_ptr->rlc_Capability.
        totalRLC_AM_BufferSize = tdsrrc_TotalRLC_AM_BufferSize_kb1000;
    }
    else
    if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
      /* Total AM buffer size is 500KB */
      ue_radio_access_capability_ptr->rlc_Capability.
        totalRLC_AM_BufferSize = tdsrrc_TotalRLC_AM_BufferSize_kb500;
    }
    else
    {
      /* Total AM buffer size is 150KB */
      ue_radio_access_capability_ptr->rlc_Capability.
         totalRLC_AM_BufferSize = tdsrrc_TotalRLC_AM_BufferSize_kb150;
    }
    ue_radio_access_capability_ptr->rlc_Capability.
      maximumRLC_WindowSize = tdsrrc_MaximumRLC_WindowSize_mws2047; /* Not Sure about this value */

    /* Security Capability 
     */
    /* Allocate memory for Ciphering Algorithm.Ciphering Algorithm is
       a 16 bit string. Hence 2 bytes are allocated */

/*lint -save -e778 -e572 */
    if (tdsnv_ciphering_enabled_status)
    {
      cipher_algo_bit_mask = TDSUEA_1_SUPPORTED + TDSUEA_0_SUPPORTED;
#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
      if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) && tdssnow3g_security_algo_supported)
      {
        cipher_algo_bit_mask += TDSUEA_2_SUPPORTED;
      }
#endif
      /* UEA1 Encryption is supoorted in the current release*/
      ue_radio_access_capability_ptr->securityCapability.
       cipheringAlgorithmCap.numbits = 16;
      *(ue_radio_access_capability_ptr->securityCapability.
        cipheringAlgorithmCap.data) = 
       (uint8)(( cipher_algo_bit_mask & 0xFF00) >> 8 );
      *(ue_radio_access_capability_ptr->securityCapability.
        cipheringAlgorithmCap.data + 1) = 
        (uint8)( cipher_algo_bit_mask & 0xFF);
    }
    else
    {
      /* No Encryption is supoorted in the current release*/
      ue_radio_access_capability_ptr->securityCapability.
       cipheringAlgorithmCap.numbits = 16;
      *(ue_radio_access_capability_ptr->securityCapability.
        cipheringAlgorithmCap.data) = 
        ((TDSUEA_0_SUPPORTED & 0xFF00) >> 8 );
      *(ue_radio_access_capability_ptr->securityCapability.
        cipheringAlgorithmCap.data + 1) = 
        (TDSUEA_0_SUPPORTED & 0xFF);
    }
  


    /* No Integrity protection is supported in the current release. Actually 
       there is no way to inform this to UTRAN */ 
    ue_radio_access_capability_ptr->securityCapability.
      integrityProtectionAlgorithmCap.numbits = 16;

    int_algo_bit_mask = TDSKASUMI_SUPPORTED;
#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
    if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) && tdssnow3g_security_algo_supported)
    {
      int_algo_bit_mask += TDSSNOW_3G_SUPPORTED;
    }
#endif

    *(ue_radio_access_capability_ptr->securityCapability.
      integrityProtectionAlgorithmCap.data) = 
      (uint8)( (int_algo_bit_mask & 0xFF00) >> 8 );
    *(ue_radio_access_capability_ptr->securityCapability.
      integrityProtectionAlgorithmCap.data + 1) = 
      (uint8)(int_algo_bit_mask & 0xFF);
  
/*lint -restore */


    /* Update downlink Transport Channel Capability */
#if  !defined(FEATURE_DISABLE_384K_ON_20MS_TTI_UE_CAPABILITY)
    /* Max no of bits convolutionally coded Transport Blocks that can be
       received any time is 8960 */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.maxConvCodeBitsReceived = tdsrrc_MaxNoBits_b8960;

    /* Max no of bits of all Transport Blocks that can be
       received any time is 8960 */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.maxNoBitsReceived = tdsrrc_MaxNoBits_b8960;
#else
    /* Max no of bits convolutionally coded Transport Blocks that can be
       received any time is 6400 */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.maxConvCodeBitsReceived = tdsrrc_MaxNoBits_b6400;

    /* Max no of bits of all Transport Blocks that can be
       received any time is 6400 */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.maxNoBitsReceived = tdsrrc_MaxNoBits_b6400;
#endif  

    /* Max nomber of Transport Formats */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.maxNumberOfTF = tdsrrc_MaxNumberOfTF_tf64;
  
    /* Max number of TFC in the TFCS */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.maxNumberOfTFC = 
      tdsrrc_MaxNumberOfTFC_DL_tfc128;
    
    if (TDSRRC_DL_MAX_TRANSPORT_BLOCKS == 32)
    {
      /* Max number of Transport Blocks that can be received in TTI */
      ue_radio_access_capability_ptr->transportChannelCapability.
        dl_TransChCapability.maxReceivedTransportBlocks = 
        tdsrrc_MaxTransportBlocksDL_tb32;
    }

    /* Max no of simultaneous transport channels */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.maxSimultaneousTransChs = 
      tdsrrc_MaxSimultaneousTransChsDL_e8;

    /* Maximum Simultaneous CCTrCHs Count */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.maxSimultaneousCCTrCH_Count = 1;


    /* Turbo Coding is supported */
    ue_radio_access_capability_ptr->
      transportChannelCapability.dl_TransChCapability.turboDecodingSupport.
      t = T_tdsrrc_TurboSupport_supported;

#if  !defined(FEATURE_DISABLE_384K_ON_20MS_TTI_UE_CAPABILITY)
    /* Max no of bits supported for Turbo coding */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.turboDecodingSupport.u.supported = tdsrrc_MaxNoBits_b8960;
#else
    /* Max no of bits supported for Turbo coding */
    ue_radio_access_capability_ptr->transportChannelCapability.
      dl_TransChCapability.turboDecodingSupport.u.supported = tdsrrc_MaxNoBits_b6400;
#endif

    /* Update uplink Transport Channel Capability */

#if  !defined(FEATURE_DISABLE_384K_ON_20MS_TTI_UE_CAPABILITY)    
  /* Max no of bits of all convolutionally coded tranport blocks */
    ue_radio_access_capability_ptr->transportChannelCapability.
      ul_TransChCapability.maxConvCodeBitsTransmitted = tdsrrc_MaxNoBits_b8960;

    /* Max no of bits of all Transport Blocks */
    ue_radio_access_capability_ptr->transportChannelCapability.
      ul_TransChCapability.maxNoBitsTransmitted = tdsrrc_MaxNoBits_b8960;
#else
    /* Max no of bits of all convolutionally coded tranport blocks */
    ue_radio_access_capability_ptr->transportChannelCapability.
      ul_TransChCapability.maxConvCodeBitsTransmitted = tdsrrc_MaxNoBits_b6400;

    /* Max no of bits of all Transport Blocks */
    ue_radio_access_capability_ptr->transportChannelCapability.
      ul_TransChCapability.maxNoBitsTransmitted = tdsrrc_MaxNoBits_b6400;
#endif

    /* Max no of Transport Formats */
    ue_radio_access_capability_ptr->transportChannelCapability.
      ul_TransChCapability.maxNumberOfTF = tdsrrc_MaxNumberOfTF_tf64;

    /* Max no of TFC in TFCS */
    ue_radio_access_capability_ptr->transportChannelCapability.
      ul_TransChCapability.maxNumberOfTFC = tdsrrc_MaxNumberOfTFC_UL_tfc64;

    /* Max no of Tranport Blocks that can be transmitted */
    ue_radio_access_capability_ptr->transportChannelCapability.
      ul_TransChCapability.maxTransmittedBlocks = tdsrrc_MaxTransportBlocksUL_tb32;

    /* Max no of simultaneous Transport Channels */
    ue_radio_access_capability_ptr->transportChannelCapability.ul_TransChCapability.
      maxSimultaneousTransChs = tdsrrc_MaxSimultaneousTransChsUL_e8;
  
    /* Turbo coding is supported */
     ue_radio_access_capability_ptr->transportChannelCapability.
      ul_TransChCapability.turboEncodingSupport.t =
      T_tdsrrc_TurboSupport_supported;

#if  !defined(FEATURE_DISABLE_384K_ON_20MS_TTI_UE_CAPABILITY)
    /* Max no of bits supported for Turbo coding */
    ue_radio_access_capability_ptr->transportChannelCapability.ul_TransChCapability.
      turboEncodingSupport.u.supported = tdsrrc_MaxNoBits_b8960;
#else
    /* Max no of bits supported for Turbo coding */
    ue_radio_access_capability_ptr->transportChannelCapability.ul_TransChCapability.
      turboEncodingSupport.u.supported = tdsrrc_MaxNoBits_b6400;
#endif

    /* Set Mode Specific Info in UL Tranport Channel Capability  to TDD*/
    ue_radio_access_capability_ptr->transportChannelCapability.ul_TransChCapability.
      modeSpecificInfo.t = T_tdsrrc_UL_TransChCapability_modeSpecificInfo_tdd;

    ue_radio_access_capability_ptr->transportChannelCapability.ul_TransChCapability.modeSpecificInfo.
      u.tdd = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_UL_TransChCapability_tdd);
    
    ue_radio_access_capability_ptr->transportChannelCapability.ul_TransChCapability.
      modeSpecificInfo.u.tdd->maxSimultaneousCCTrCH_Count = 1;

    /* Update Multi RAT capability 
    */
    /* TDD mode only supported */
    ue_radio_access_capability_ptr->ue_MultiModeRAT_Capability.
      multiModeCapability = tdsrrc_MultiModeCapability_tdd;
    
    if (tdsrrcmcm_is_dualmode_enabled())
    {
      ue_radio_access_capability_ptr->ue_MultiModeRAT_Capability.
        multiRAT_CapabilityList.supportOfGSM = TRUE;
    }
    else
    { /* GSM is not supported */
      ue_radio_access_capability_ptr->ue_MultiModeRAT_Capability.
        multiRAT_CapabilityList.supportOfGSM = FALSE;
    }

    /* Multi Carrier is not supported */
    ue_radio_access_capability_ptr->ue_MultiModeRAT_Capability.
      multiRAT_CapabilityList.supportOfMulticarrier = FALSE;

  } /* Radio Access Capability is a valid pointer */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Radio Capability pointer");
  }

}

#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================
FUNCTION rrcueci_append_lte_specific_capability

DESCRIPTION
  This function appends the EUTRA specifc capability
DEPENDENCIES
  None

RETURN VALUE
  TURE is successfully appended,
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
static boolean tdsrrcueci_append_lte_specific_capability
(
  struct tdsrrc_InterRAT_UE_RadioAccessCapability_v860ext *irat_ue_cap_860ext
)
{
  uint8 count =0;
  boolean eutra_cap_req = FALSE;

  for(count = 0; count < tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count && count < 16;count++)
  {
    if(tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[count] == TDSRRC_EUTRA_CAP)
    {
      eutra_cap_req = TRUE;
      break;
    }
  }

  if(!eutra_cap_req)
  {
    /*EUTRA cap is not requested*/
    return FALSE;
  }
  // 2012/09/10 KW
  if((!tdsrrcueci_interrat_lte_capability_info.lte_rsp_received) ||
     (tdsrrcueci_interrat_lte_capability_info.cap_cont_length == 0) ||
     (tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr == NULL))
  {
    if((!tdsrrcueci_interrat_lte_capability_info.lte_rsp_received) && tdsrrcueci_interrat_lte_capability_info.lte_req_posted)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LTE_IRAT: Didnt receive capability response from LTE");
    }
    return FALSE;
  }
  else
  {
    irat_ue_cap_860ext->m.eutra_RadioAccessCapabilityPresent = 1;
    irat_ue_cap_860ext->eutra_RadioAccessCapability.ue_EUTRA_Capability.numocts =
         tdsrrcueci_interrat_lte_capability_info.cap_cont_length ;
    irat_ue_cap_860ext->eutra_RadioAccessCapability.ue_EUTRA_Capability.data = 
        /*tdsrrc_malloc((unsigned long)tdsrrcueci_interrat_lte_capability_info.cap_cont_length);*/
         rtxMemAllocZ (&tdsenc_ctxt, (unsigned long)tdsrrcueci_interrat_lte_capability_info.cap_cont_length);

    if(NULL == irat_ue_cap_860ext->eutra_RadioAccessCapability.ue_EUTRA_Capability.data)
    {
      ERR_FATAL("Unable to allocate memory for LTE specific capability",0,0,0);
    }
    memscpy((void*)irat_ue_cap_860ext->eutra_RadioAccessCapability.ue_EUTRA_Capability.data,
                  irat_ue_cap_860ext->eutra_RadioAccessCapability.ue_EUTRA_Capability.numocts,
                  tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr,
                  tdsrrcueci_interrat_lte_capability_info.cap_cont_length);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Appended LTE RAT specific capability");
    return TRUE;
  }
}
#endif

/*===========================================================================
FUNCTION tdsrrcueci_append_ue_radio_access_capability

DESCRIPTION
  This function appends the Radio Access Capability to the message.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcueci_append_ue_radio_access_capability
(
  tdsrrc_UE_RadioAccessCapability *ue_radio_access_capability_ptr,
  boolean irat_ho_info,
  tdsrrcueci_cap_update_cause_e_type update_cause                               // Capability query cause
)
{
  boolean is_extensions_sent = FALSE;                       //keeps track if extensions are appended


    tdsrrcueci_append_ue_radio_access_capability_non_ext(ue_radio_access_capability_ptr,
                                                         irat_ho_info,
                                                      TDSRRCUECI_INT_CAP_UPDATE_1900_MULTI_BAND);
  return is_extensions_sent;
} /* tdsrrcueci_append_ue_radio_access_capability */


#ifdef FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED
/*===========================================================================
FUNCTION RRCUECI_APPEND_UE_RADIO_ACCESS_CAPABILITY_COMPRESSED

DESCRIPTION
  This function fills in the Radio Access Capability compressed extensions.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcueci_append_ue_radio_access_capability_compressed
(
  tdsrrc_RF_CapabBandListFDDComp *rrc_RFCapabilityCompressed, // R5
  tdsrrc_RF_CapabBandListFDDComp_ext *rrc_RFCapabilityFDDComp, // R6
  boolean *is_fdd_comp_ext_present
)
{
  uint8 count = 0;                                          //counter for various purposes
  uint32 num_total_supported_bands = 0;                     //total supported bands fdd+fdd2
  tdsrrcueci_band_storage_type ueci_band;                      //type to get supported bands from csp


  //reset the ueci_band type
  memset (&ueci_band, 0, sizeof(tdsrrcueci_band_storage_type));

  //get the supported band list from csp
  tdsrrccsp_get_supported_band_list(&ueci_band);

  //Get the total number of bands supported = fdd + fdd2
  num_total_supported_bands = ueci_band.num_supported_bands + ueci_band.num_supported_bands_fdd2;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Num supported bands: FDD:%d, FDD2:%d, Total:%d",
        ueci_band.num_supported_bands,
        ueci_band.num_supported_bands_fdd2,
        num_total_supported_bands);

  /* If Rel-5 is enabled, then we can also support FDD2 bands. So check if total_num_bands
     (FDD + FDD2)is equal to zero and error fatal is none of them is selected */
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    //if no bands are supported, print MSG_ERROR here, this is a catastrophic problem
    if(num_total_supported_bands == 0)
    {
      ERR_FATAL("Unsupported band-config. No FDD/FDD2 bands selected",0,0,0);
    }
  }
  /* If Rel-5 is not enabled, then FDD2 bands cannot be signalled. So atleast one FDD band 
     needs to be enabled. Check only for FDD band equal to Zero */
  else if(ueci_band.num_supported_bands == 0)
  {
    ERR_FATAL("Unsupported band-config. Non-Rel5 UE. No FDD band selected",0,0,0);
  }
  

  rrc_RFCapabilityCompressed->n = TDSRRC_UECI_MAX_BANDS_ALLOWED;
  ALLOC_ASN1ARRAY1(&tdsenc_ctxt,rrc_RFCapabilityCompressed,tdsrrc_RF_CapabBandFDDComp);
  
  for(count = 0; count < TDSRRC_UECI_MAX_BANDS_ALLOWED; count++)
  {
   
    if(rrc_RFCapabilityCompressed->elem == NULL)
    {
      ERR_FATAL("Malloc failed for CapabBandFDDList, count: %d",count,0,0);
    }

    if(ueci_band.band_type_present[count] == TRUE)
    {
      rrc_RFCapabilityCompressed->elem[count] = tdsrrc_RF_CapabBandFDDComp_default_TxRx_separation;
    }
    else
    {
      rrc_RFCapabilityCompressed->elem[count] = tdsrrc_RF_CapabBandFDDComp_notSupported;
    }

   
  }

  

  if((ueci_band.num_supported_bands_fdd2 > 0) && 
     (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
  {
    *is_fdd_comp_ext_present = TRUE;

 
    rrc_RFCapabilityFDDComp->n = RRC_UECI_MAX_BANDS_ALLOWED_TDS2 - 1;
    ALLOC_ASN1ARRAY1(&tdsenc_ctxt,rrc_RFCapabilityFDDComp,tdsrrc_RF_CapabBandFDDComp);
    /* Append FDD2 bands if any */
    for(count = 0; count < RRC_UECI_MAX_BANDS_ALLOWED_TDS2 - 1; count++)
    {
      
      if(!rrc_RFCapabilityFDDComp->elem)
      {
        ERR_FATAL("Malloc failed for CapabBandFDDList2, count: %d",count,0,0);
      }

      if(ueci_band.band_type_present_fdd2[count] == TRUE)
      {
        rrc_RFCapabilityFDDComp->elem[count] = tdsrrc_RF_CapabBandFDDComp_default_TxRx_separation;
      }
      else
      {
        rrc_RFCapabilityFDDComp->elem[count] = tdsrrc_RF_CapabBandFDDComp_notSupported;
      }

      
    }
   

  }
  return;
}
#endif /* FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED */
#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================

FUNCTION rrcueci_send_eutra_capabilitites_req_to_lte

DESCRIPTION
  This  function sends EUTRA capabilities request to LTE

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcueci_send_eutra_capabilitites_req_to_lte
(
  void
)
{
  lte_rrc_eutra_capabilities_req_s eutra_cap_req;
  static uint8 count = 0;
  rrc_plmn_identity_type      plmn_id;
  memset(&eutra_cap_req,0,sizeof(lte_rrc_eutra_capabilities_req_s));

  if ((!tdsrrcueci_interrat_lte_capability_info.lte_rsp_received) && tdsrrcueci_interrat_lte_capability_info.lte_req_posted) 
  {
    count++;
    if (count >= 3)
    {
      ERR_FATAL("TDSTOL: Didnt receive capability response from LTE for %d time",count,0,0);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDSTOL: Didnt receive capability response from LTE for %d time ",count);
    }
  }
  else
  {
    count=0;
  }

  eutra_cap_req.rat_capabilities.lte_is_supported = TRUE;
  eutra_cap_req.rat_capabilities.lte_band_preference = tdsrrccsp_get_lte_band_pref();


  tdsrrccsp_get_tg_band_pref(&eutra_cap_req.rat_capabilities.tdscdma_band_preference,
    &eutra_cap_req.rat_capabilities.geran_band_preference);
  eutra_cap_req.rat_capabilities.tdscdma_is_supported = TRUE;
  if(tdsrrcmcm_is_dualmode_enabled())
  {
    eutra_cap_req.rat_capabilities.geran_is_supported = TRUE;
  }

  eutra_cap_req.src_rat = LTE_RRC_IRAT_UTRA_TDD;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRC mode : %d, RRC camped sts :%d",
                tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_CSP),
                tdsrrcmcm_get_rrc_mode());
  /* Update current camped PLMN to LTE as part of the capability request.
     For the case of handover, RRC won't be having a valid PLMN yet so fill invalid PLMN */
  if((TDSRRC_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_CSP)) && (TDSRRC_MODE_ACTIVE == tdsrrcmcm_get_rrc_mode()))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"T2L CAP REQ curr_sel_plmn: PLMN MCC %d MNC %d",
      TDSRRC_CSP_GET_INT_MCC(tdsrrcccm_get_curr_camped_plmn()),
      TDSRRC_CSP_GET_INT_MNC(tdsrrcccm_get_curr_camped_plmn()));
 
    eutra_cap_req.camped_plmn = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(tdsrrcccm_get_curr_camped_plmn());
  }
  else
  {
    /* Initialize PLMN to dummy value to keep lint happy */
    plmn_id.mcc[0] = 0xF;
    plmn_id.mcc[1] = 0xF;
    plmn_id.mcc[2] = 0xF;
    plmn_id.num_mnc_digits = 3;
    plmn_id.mnc[0] = 0xF;
    plmn_id.mnc[1] = 0xF;
    plmn_id.mnc[2] = 0xF;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"T2L CAP REQ curr_sel_plmn: PLMN MCC %d MNC %d", 
                  TDSRRC_CSP_GET_INT_MCC(plmn_id),
                  TDSRRC_CSP_GET_INT_MNC(plmn_id));
    eutra_cap_req.camped_plmn = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id);
  }

  /*LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_REQ*/
  msgr_init_hdr(&eutra_cap_req.msg_hdr,MSGR_TDSCDMA_RRC,LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_REQ);
#ifdef FEATURE_DUAL_SIM
#ifdef  FEATURE_TDSCDMA_MSIM_L_PLUS_L 
  msgr_set_hdr_variant(&eutra_cap_req.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&eutra_cap_req.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&eutra_cap_req.msg_hdr, sizeof(lte_rrc_eutra_capabilities_req_s));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&eutra_cap_req.msg_hdr, sizeof(lte_rrc_eutra_capabilities_req_s));
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2L: LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_REQ sent to LTE RRC");

  tdsrrcueci_interrat_lte_capability_info.lte_req_posted = TRUE;
  tdsrrcueci_interrat_lte_capability_info.lte_rsp_received = FALSE;
}

/*===========================================================================

FUNCTION rrcueci_save_eutra_capability

DESCRIPTION
  This  function saves the E-UTRA capability info

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcueci_save_eutra_capability
(
  lte_rrc_eutra_capabilities_rsp_s *lte_eutra_capabilities_rsp
)
{
  dsm_item_type *dsm_ptr;
  msgr_attach_struct_type *msgr_attach_ptr;
  uint16 dsm_pkt_len;
  
  msgr_attach_ptr = msgr_get_attach(((msgr_hdr_struct_type *)&lte_eutra_capabilities_rsp->msg_hdr),0);
  ASSERT(msgr_attach_ptr != NULL);
  msgr_get_dsm_attach(msgr_attach_ptr, &dsm_ptr);
  ASSERT(dsm_ptr != NULL);
  dsm_pkt_len = dsm_length_packet(dsm_ptr);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"PKT len %d",dsm_pkt_len);

  if (tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr != NULL)
  {
    tdsrrc_free(tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr);
	tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr = NULL;
  }

  tdsrrcueci_interrat_lte_capability_info.cap_cont_length = dsm_pkt_len;
  tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr = (uint8 *) tdsrrc_malloc(dsm_pkt_len);
  if(NULL == tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr)
  {
	ERR_FATAL("Cannot allocate memory",0,0,0);
  }

  if (dsm_pullup(&dsm_ptr,tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr,dsm_pkt_len) != dsm_pkt_len)
  {
	TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not pull up all the bytes from lte_eutra_capabilities_rsp");
	tdsrrc_free(tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr);
	tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr = NULL;
  }
  else
  {
  tdsrrcueci_interrat_lte_capability_info.lte_rsp_received = TRUE;
}
}
/*===========================================================================
FUNCTION rrcueci_append_ue_radio_access_capability_tdd

DESCRIPTION
  This function appends the Radio Access Capability to the message.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrcueci_append_ue_radio_access_capability_tdd
(
  tdsrrc_MeasurementCapabilityTDD *tdd_ptr,
  sys_band_mask_e_type lte_band_pref
)
{

  uint8 idx, lte_band_idx=0;
  sys_band_mask_e_type lte_band_chk=0;
  uint8 num_lte_bands=0;
#ifdef LTE_BAND_NUM
  sys_lte_band_mask_e_type lte_band_pref_extend; //xlong 0323
#endif
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSTOL: LTE band mask = H:0x%x,L:0x%x",QWORD_HIGH(lte_band_pref),QWORD_LOW(lte_band_pref));

  if(lte_band_pref == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No LTE band enabled!");

    return FALSE;
  }

  for(lte_band_idx =0; lte_band_idx < MIN(SYS_SBAND_LTE_EUTRAN_BAND_MAX, 0x3F); lte_band_idx++)
  {
    lte_band_chk = (sys_band_mask_e_type)(((uint64)1) << lte_band_idx);
	if(lte_band_chk & lte_band_pref)
    {
#ifdef LTE_BAND_NUM
      /*We should have no extend LTE band for OTA msg, extend here to be compatible with interface 
             of lte_rrc_lte_fdd_is_supported() and  lte_rrc_lte_tdd_is_supported*/
      memset(&lte_band_pref_extend, 0x00, sizeof(sys_lte_band_mask_e_type)); //xlong 0323
      #if(LTE_BAND_NUM == 256)
        lte_band_pref_extend.bits_1_64 = (uint64)lte_band_chk;
      #else
        lte_band_pref_extend = lte_band_chk;
      #endif

      if( !lte_rrc_lte_fdd_is_supported(lte_band_pref_extend) &&
	  	  !lte_rrc_lte_tdd_is_supported(lte_band_pref_extend) )
#else
      if( !lte_rrc_lte_fdd_is_supported(lte_band_chk) &&
	  	  !lte_rrc_lte_tdd_is_supported(lte_band_chk) )
#endif
      {
        TDSRRC_MSG1(MSG_LEGACY_MED,"Not supported LTE band,%d",lte_band_idx+1);
      }
      else
      {
        num_lte_bands++;
      }        
    }
  }

  if (num_lte_bands == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No good LTE band supported");
    return FALSE;
  }
 
  tdd_ptr->idleIntervalMeasCapabEUTRAList.n = 0;
  tdd_ptr->idleIntervalMeasCapabEUTRAList.elem = NULL;

  if (num_lte_bands > 0)
  {
    tdd_ptr->idleIntervalMeasCapabEUTRAList.n = num_lte_bands;
	ALLOC_ASN1ARRAY1(&tdsenc_ctxt,&tdd_ptr->idleIntervalMeasCapabEUTRAList,tdsrrc_IdleIntervalMeasCapabEUTRA); 
  }

  if(tdd_ptr->idleIntervalMeasCapabEUTRAList.elem == NULL)
  {
    ERR_FATAL("TDSTOL: Memory allocation failure",0,0,0);
  }

  tdd_ptr->m.idleIntervalMeasCapabEUTRAListPresent = 1;

  idx = 0;
  for(lte_band_idx =0 ; lte_band_idx < MIN(SYS_SBAND_LTE_EUTRAN_BAND_MAX, 0x3F); lte_band_idx++)
  {
	lte_band_chk = (sys_band_mask_e_type)(((uint64)1) << lte_band_idx);
	if(lte_band_chk & lte_band_pref)
	{
#ifdef LTE_BAND_NUM
      /*We should have no extend LTE band for OTA msg, extend here to be compatible with interface 
        of lte_rrc_lte_fdd_is_supported() and	lte_rrc_lte_tdd_is_supported*/
      memset(&lte_band_pref_extend, 0x00, sizeof(sys_lte_band_mask_e_type)); //xlong 0323
      #if(LTE_BAND_NUM == 256)
        lte_band_pref_extend.bits_1_64 = (uint64)lte_band_chk;
      #else
        lte_band_pref_extend = lte_band_chk;
      #endif
	  if(!lte_rrc_lte_fdd_is_supported(lte_band_pref_extend) &&
       !lte_rrc_lte_tdd_is_supported(lte_band_pref_extend) )
#else
	  if( !lte_rrc_lte_fdd_is_supported(lte_band_chk) &&
	  	  !lte_rrc_lte_tdd_is_supported(lte_band_chk) )
#endif
	  {
		/* If we dont support this band, skip filling this band */
		continue;
	  }
	  if ((idx < num_lte_bands) && (idx < SYS_SBAND_LTE_EUTRAN_BAND_MAX))
	  {
        tdd_ptr->idleIntervalMeasCapabEUTRAList.elem[idx].radioFrequencyBandEUTRA = lte_band_idx+1;
#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
             if (TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL)
             {
        tdd_ptr->idleIntervalMeasCapabEUTRAList.elem[idx].idleInterval = TRUE;
             }
             else
             {
#endif
             tdd_ptr->idleIntervalMeasCapabEUTRAList.elem[idx].idleInterval = FALSE;
#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
             }
#endif
		idx++;
	  }
	}
  }

  return TRUE;
}

#endif

/*===========================================================================
FUNCTION tdsrrcueci_append_ue_radio_access_capability_later_than_r3

DESCRIPTION
  This function appends the Radio Access Capability to the message for
  Release 5.
  
  We should optimize the code later.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcueci_append_ue_radio_access_capability_later_than_r3
(
  /* Pointer to UE Radio Access Capability */
  tdsrrc_UE_RadioAccessCapability_v590ext *ue_radio_access_capability_ptr,  
   tdsrrc_UE_CapabilityContainer_IEs *ue_capability_container,
  tdsrrcueci_cap_update_cause_e_type update_cause  /* Capability query cause */
)
{
  tdsrrc_RadioFrequencyBandTDDextList *tdd128RF_Capability;
  tdsrrcueci_band_storage_type  ueci_band;         /* type to get supported bands from csp */
  uint8 i = 0;

  /* reset the ueci_band type */
  memset (&ueci_band, 0, sizeof(tdsrrcueci_band_storage_type));
  
  /* get the supported band list from csp */
  tdsrrccsp_get_supported_band_list(&ueci_band);


  /* Check whether Radio Access capability is a valid pointer */
  if(ue_radio_access_capability_ptr != NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Fill UE capability for REL5");
    
    /* The CapabilityWithSimultaneousHS_DSCHConfig_present is not given when 
    the access capability is not required by the UE capability. 
    */  
    if(update_cause == TDSRRCUECI_CAP_UPDATE)
    {
      TDSRRC_RESET_MSG_IE_PRESENT_PTR(ue_radio_access_capability_ptr);
      ue_radio_access_capability_ptr->physicalChannelCapability.tdd128_hspdsch.t = 
        T_tdsrrc_PhysicalChannelCapability_hspdsch_r5_tdd128_hspdsch_unsupported; 
      TDSRRC_RESET_MSG_IE_PRESENT(ue_radio_access_capability_ptr->rlc_Capability_r5_ext);
      /* CapabilityWithSimultaneousHS_DSCHConfig_present is 64 k */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(ue_radio_access_capability_ptr,
        tdsrrc_UE_RadioAccessCapability_v590ext,dl_CapabilityWithSimultaneousHS_DSCHConfig);
      ue_radio_access_capability_ptr->dl_CapabilityWithSimultaneousHS_DSCHConfig = tdsrrc_DL_CapabilityWithSimultaneousHS_DSCHConfig_kbps64;
      /*In the R3 part the RLC buffer size is 500kb, so dont update
       * the R5 path for RLC buffer size
       */
      if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
      {
         TDSRRC_RESET_MSG_IE_PRESENT(ue_radio_access_capability_ptr->rlc_Capability_r5_ext);
      }
      else
      {
        /* Fill in RLC info. */
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ue_radio_access_capability_ptr->rlc_Capability_r5_ext,totalRLC_AM_BufferSize);
        /*The relation between HSDPA categories and Total RLC AM buffer size is taken from Spec 25.306 8.10.0
                    (Table 5.1n - Total RLC and MAC-hs parameters for 1.28 Mcps TDD HS-DSCH and E-DCH physical
                     layer categories) */
        if(tdsnv_hsdpa_category >= 13)
        {
         ue_radio_access_capability_ptr->rlc_Capability_r5_ext.totalRLC_AM_BufferSize = tdsrrc_TotalRLC_AM_BufferSize_r5_ext_kb400;
        }
        else
        {
          ue_radio_access_capability_ptr->rlc_Capability_r5_ext.totalRLC_AM_BufferSize = tdsrrc_TotalRLC_AM_BufferSize_r5_ext_kb300;
        }
      }
      /* Fill in Phy */
      ue_radio_access_capability_ptr->physicalChannelCapability.tdd128_hspdsch.t =
        T_tdsrrc_PhysicalChannelCapability_hspdsch_r5_tdd128_hspdsch_supported;

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Set HSDPA Category to %d",tdsnv_hsdpa_category);
        
      if (tdsnv_hsdpa_category > 15)
      {
        ue_radio_access_capability_ptr->physicalChannelCapability.tdd128_hspdsch.u.supported = 15;
      }
      else
      {
        ue_radio_access_capability_ptr->physicalChannelCapability.tdd128_hspdsch.u.
         supported = tdsnv_hsdpa_category;
      }

#if defined (FEATURE_TDSCDMA_64QAM) || defined (FEATURE_TDSCDMA_MIMO)
      if(tdsrrc_mimo_64qam_cm_enabled == TRUE)
      {
        /*
           If IE "dummy2" is set to TRUE then:
             UE does not support 64 QAM and Compressed Mode simultaneously.
             UE does not support MIMO and Compressed Mode simultaneously
          */
        ue_radio_access_capability_ptr->physicalChannelCapability.fdd_hspdsch.u.
          supported->dummy2 = TRUE;
      }
#endif


      /* FDD not supported */
      ue_radio_access_capability_ptr->physicalChannelCapability.fdd_hspdsch.t =
        T_tdsrrc_PhysicalChannelCapability_hspdsch_r5_fdd_hspdsch_unsupported;

      /* 3.84TDD not supported */
      ue_radio_access_capability_ptr->physicalChannelCapability.tdd384_hspdsch.t =
        T_tdsrrc_PhysicalChannelCapability_hspdsch_r5_tdd384_hspdsch_unsupported;
      
#ifdef FEATURE_UMTS_PDCP
      if(tdsrrc_pdcp_enabled)
      { 
        /* Fill in PDCP Cap. */
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
        ue_radio_access_capability_ptr->pdcp_Capability_r5_ext,
        maxHcContextSpace);
        
        ue_radio_access_capability_ptr->pdcp_Capability_r5_ext.maxHcContextSpace = tdsrrc_MaxHcContextSpace_r5_ext_by16384;
        ue_radio_access_capability_ptr->pdcp_Capability_r5_ext.supportForRfc3095ContextRelocation = FALSE;
      }
      else
#endif /* FEATURE_UMTS_PDCP*/
      {
        /* Fill in PDCP Cap. */
        /* Not Support RFC 3095 for PDCP */
        ue_radio_access_capability_ptr->pdcp_Capability_r5_ext.supportForRfc3095ContextRelocation = 0;
        /* No maxHcContextSpace present */
        TDSRRC_RESET_MSG_IE_PRESENT(ue_radio_access_capability_ptr->pdcp_Capability_r5_ext);
      }
      /* Fill in MultiModeRAT Cap. */
#ifdef FEATURE_TD2G_NACC
      if(tdsrrcmcm_is_dualmode_enabled() && nv_td2g_nacc_enabled)
      {
        ue_radio_access_capability_ptr->multiModeRAT_Capability_v590ext.supportOfUTRAN_ToGERAN_NACC = TRUE;
      }
      else
#endif
      {
        ue_radio_access_capability_ptr->multiModeRAT_Capability_v590ext.supportOfUTRAN_ToGERAN_NACC = FALSE;
      }
      /* Set support band e and f for CCSA V2 */
      if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
      {
        TDSRRC_RESET_MSG_IE_PRESENT_PTR(ue_capability_container);
        
        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->
          ue_RadioAccessCapability_v690ext);

        TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(ue_capability_container,
         tdsrrc_UE_CapabilityContainer_IEs,v6b0NonCriticalExtensions);
               
        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions
             .ue_RadioAccessCapability_v6b0ext);
               
        TDSRRC_MSG_COMMON_SET_BITMASK_IE(ue_capability_container->v6b0NonCriticalExtensions,
           tdsrrc_UE_CapabilityContainer_IEs_v6b0NonCriticalExtensions,v6e0NonCriticalExtensions);
               
        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions);
         
        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.
           v6e0NonCriticalExtensions.ue_RadioAccessCapability_v6e0ext);

        ue_capability_container->ue_RadioAccessCapability_v690ext.m.deviceTypePresent = 0;
        ue_capability_container->ue_RadioAccessCapability_v690ext.physicalchannelcapability_edch.fdd_edch.t = 
            T_tdsrrc_PhysicalChannelCapability_edch_r6_fdd_edch_unsupported;

        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.m.
            v770NonCriticalExtensionsPresent=1; 

       
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
             v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.m.pdcp_CapabilityPresent = 0;
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
            v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.m.mac_ehsSupportPresent = 0;
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.m.ue_specificCapabilityInformationPresent = 0;
        
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.m.rf_CapabilityPresent = 1;
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.rf_Capability.m.tdd384RF_CapabilityPresent = 0;
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.rf_Capability.m.tdd768RF_CapabilityPresent = 0;        
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.rf_Capability.m.tdd128RF_CapabilityPresent = 1;
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.rf_Capability.tdd128RF_Capability = tdsrrc_RadioFrequencyBandTDDList_r7_a;

        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.rlc_Capability.supportOfTwoLogicalChannel = FALSE;

        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.multiModeRAT_Capability.m.supportOfPSHandoverToGANPresent = 0;

        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.physicalChannelCapability.m.fddPhysChCapabilityPresent = 0;
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.physicalChannelCapability.m.tddPhysChCapability_384Present = 0;
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.physicalChannelCapability.m.tddPhysChCapability_768Present = 0;
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.physicalChannelCapability.m.tddPhysChCapability_128Present = 0;
        
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext.ue_PositioningCapability.m.ue_GANSSPositioning_CapabilityPresent = 0;
  
        /* add v790NonCriticalExtensions */
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
           v770NonCriticalExtensions.m.v790NonCriticalExtensionsPresent = 1;
    
        /* add R8 extension v860NonCriticalExtensions,  CCSA requirement */
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.v790NonCriticalExtensions.m.v860NonCriticalExtensionsPresent = 1;
    
        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.v790NonCriticalExtensions.v860NonCriticalExtensions);
            
        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RadioAccessCapability_v860ext);
    
        if (0 != ueci_band.num_supported_bands_tddv860)
        {
          /* RF capability v860 */
          ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
            v770NonCriticalExtensions.v790NonCriticalExtensions.v860NonCriticalExtensions.
            ue_RadioAccessCapability_v860ext.m.rf_CapabilityPresent = 1;
          TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
            v770NonCriticalExtensions.v790NonCriticalExtensions.v860NonCriticalExtensions.
            ue_RadioAccessCapability_v860ext.rf_Capability);
    
          ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
            v770NonCriticalExtensions.v790NonCriticalExtensions.v860NonCriticalExtensions.
            ue_RadioAccessCapability_v860ext.rf_Capability.m.tdd128RF_CapabilityPresent = 1;
    
          tdd128RF_Capability = &ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
            v770NonCriticalExtensions.v790NonCriticalExtensions.v860NonCriticalExtensions.
            ue_RadioAccessCapability_v860ext.rf_Capability.tdd128RF_Capability;
    
          tdd128RF_Capability->n = ueci_band.num_supported_bands_tddv860;
          
          ALLOC_ASN1ARRAY1(&tdsenc_ctxt, tdd128RF_Capability, tdsrrc_RadioFrequencyBandTDDext);
          if (tdd128RF_Capability->elem == NULL)
          {
            ERR_FATAL("Unable to allocate memory",0,0,0);
          }
    
          for (i = 0; i < tdd128RF_Capability->n; i++)
          {
              tdd128RF_Capability->elem[i] = ueci_band.band_type_tddv860[i];
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No band E and band F selected");
        }
        
        if (tdsnv_hsdpa_category > 15)
        {
          /* HSDSCH physical layer category extension */
           ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
           v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RadioAccessCapability_v860ext.physicalChannelCapability.
           m.tddPhysChCapability_128Present = 1;
          
           ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
           v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RadioAccessCapability_v860ext.physicalChannelCapability.
           tddPhysChCapability_128.downlinkPhysChCapability.m.hSDSCH_physical_layer_category_extensionPresent = 1;
    
           ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
           v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RadioAccessCapability_v860ext.physicalChannelCapability.
           tddPhysChCapability_128.downlinkPhysChCapability.hSDSCH_physical_layer_category_extension = tdsnv_hsdpa_category;
        }
      }
    }
    else
    {
      TDSRRC_RESET_MSG_IE_PRESENT_PTR(ue_radio_access_capability_ptr);
    }
  } /* Radio Access Capability is a valid pointer */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Radio Capability pointer");
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    if(update_cause == TDSRRCUECI_CAP_UPDATE)
    {
      if(tds_nv_mmcp_enable_thin_ui_config == TRUE)
      {
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ue_capability_container->ue_RadioAccessCapability_v690ext,
        deviceType);
        ue_capability_container->ue_RadioAccessCapability_v690ext.deviceType =
                                                  tdsrrc_UE_RadioAccessCapability_v690ext_deviceType_doesNotBenefitFromBatteryConsumptionOptimisation;
      }
       ue_capability_container->ue_RadioAccessCapability_v690ext.
         physicalchannelcapability_edch.fdd_edch.t = T_tdsrrc_PhysicalChannelCapability_edch_r6_fdd_edch_unsupported;

      if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
      {  
        tdsrrcueci_append_ue_radio_access_capability_rel7_capability( ue_capability_container,
                                                                 TDSRRCUECI_CAP_UPDATE
                                                                 );
        
        if (tdsnv_hsdpa_category > 15)
        {
          /* HSDSCH physical layer category extension */
           ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
           v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RadioAccessCapability_v860ext.physicalChannelCapability.
           m.tddPhysChCapability_128Present = 1;
          
           ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
           v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RadioAccessCapability_v860ext.physicalChannelCapability.
           tddPhysChCapability_128.downlinkPhysChCapability.m.hSDSCH_physical_layer_category_extensionPresent = 1;

           ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
           v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RadioAccessCapability_v860ext.physicalChannelCapability.
           tddPhysChCapability_128.downlinkPhysChCapability.hSDSCH_physical_layer_category_extension = tdsnv_hsdpa_category;
        }
      }

#ifdef FEATURE_TDSCDMA_REL8
      if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
      {

        tdsrrcueci_append_ue_radio_access_capability_rel8_capability(ue_capability_container);

#ifdef FEATURE_TDSCDMA_TO_LTE
        /*Here populate the EUTRA capability octer string in OTA*/
        if(tdsrrcmcm_is_lte_mode_enabled() &&
            tdsrrcueci_append_lte_specific_capability(&ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
             v770NonCriticalExtensions.v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RATSpecificCapability_v860ext))
        {

          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions, ue_RATSpecificCapability_v860ext);
        }
#endif


#ifdef FEATURE_TDS_DC_HSDPA_DIFF_TXD_MODE
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions.m.v880NonCriticalExtensionsPresent = 1;

        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.
			v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.
          ue_RadioAccessCapability_v880ext);

        ue_capability_container->v6b0NonCriticalExtensions.
			v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions.
          v880NonCriticalExtensions.m.v890NonCriticalExtensionsPresent=1;

        TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.
			v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.
          v890NonCriticalExtensions);

        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v890NonCriticalExtensions.
          ue_RadioAccessCapability_v890ext.m.supportCellSpecificTxDiversityinDC_OperationPresent=1;

        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v890NonCriticalExtensions.
          ue_RadioAccessCapability_v890ext.supportCellSpecificTxDiversityinDC_Operation = 
          tdsrrc_UE_RadioAccessCapability_v890ext_IEs_supportCellSpecificTxDiversityinDC_Operation_true_;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:Support for Diff TxD mode signaled.");
#else
        /* ue_RadioAccessCapability_v880ext */
        ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790NonCriticalExtensions.v860NonCriticalExtensions.m.v880NonCriticalExtensionsPresent = 0;
#endif /*FEATURE_TDS_DC_HSDPA_DIFF_TXD_MODE*/

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
        if(tdsrrcueci_support_prio_based_resel)
        {
              if(!TDSRRC_CHECK_MSG_TYPE_IE(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
                v790NonCriticalExtensions.v860NonCriticalExtensions,v880NonCriticalExtensionsPresent))
              {
      
                TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
                  v790NonCriticalExtensions.v860NonCriticalExtensions, v880NonCriticalExtensions);
      
                TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
                  v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions);
      
                TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
                    v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.ue_RadioAccessCapability_v880ext);                  
              }
      
              TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
                 v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.ue_RadioAccessCapability_v880ext, supportForPriorityReselectionInUTRAN);
      
              ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
                v790NonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.ue_RadioAccessCapability_v880ext.
                supportForPriorityReselectionInUTRAN = tdsrrc_UE_RadioAccessCapability_v880ext_IEs_supportForPriorityReselectionInUTRAN_true_;
      
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"ABSOL_PRI: Set support for intra UTRA priority reselection IE");
        }
#endif

      }
#endif/* FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis: Add bit mask supportForSIB11bis to indicate SIB11bis support in ue_capability_container");
       if(!(ue_capability_container->m.v6b0NonCriticalExtensionsPresent))
      {
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(ue_capability_container,
          tdsrrc_UE_CapabilityContainer_IEs,v6b0NonCriticalExtensions);
   
      TDSRRC_RESET_MSG_IE_PRESENT(ue_capability_container->v6b0NonCriticalExtensions);
}
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(ue_capability_container->v6b0NonCriticalExtensions.ue_RadioAccessCapability_v6b0ext,
        tdsrrc_UE_RadioAccessCapability_v6b0ext_IEs,supportForSIB11bis);
      ue_capability_container->v6b0NonCriticalExtensions.
        ue_RadioAccessCapability_v6b0ext.supportForSIB11bis = 
        tdsrrc_UE_RadioAccessCapability_v6b0ext_IEs_supportForSIB11bis_true_;
#endif
      
  
    }
  
  }
} /* rrcueci_append_ue_radio_access_capability_r5 */


/*===========================================================================
FUNCTION tdsrrcueci_append_gsm_specific_capability

DESCRIPTION
  This function appends the GSM Specific Capability to the message.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcueci_append_gsm_specific_capability
(
  tdsrrc_InterRAT_UE_RadioAccessCapability *inter_rat_capability_ptr   
                                /* Pointer to Inter RAT Capabilities List */
)
{
  classmark_2_ie_T classmark2;
  classmark_3_ie_T classmark3;
  uint32 counter;

#ifdef FEATURE_DUAL_SIM
  classmark_params_T classmark_params;
  classmark_params.gsm_supported = tdsrrcmcm_is_dualmode_enabled();
  classmark_params.wcdma_supported = TRUE;
  classmark_params.wcdma_in_use = TRUE;
  classmark_params.MSCR99 = TRUE;
  classmark_params.as_id = tdsrrc_get_as_id();
#endif
  
  inter_rat_capability_ptr->t = 
    T_tdsrrc_InterRAT_UE_RadioAccessCapability_gsm;

  /* Set Classmark 2 */
#ifdef FEATURE_DUAL_SIM
  gs_read_coded_classmark_2_per_subs(&classmark2,&classmark_params);
#else
  gs_read_coded_classmark_2(&classmark2,
    tdsrrcmcm_is_dualmode_enabled(), TRUE, TRUE, TRUE);
#endif
  
  
  
  if(classmark2.length > 5)
  {
    classmark2.length = 5;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Classmark2 Len is rcvd");
  }
  inter_rat_capability_ptr->u.gsm =
    rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_InterRAT_UE_RadioAccessCapability_gsm );
  inter_rat_capability_ptr->u.gsm->gsm_Classmark2.numocts= 5;

  for(counter = 2; counter < 5; counter++)
  {
    inter_rat_capability_ptr->u.gsm->
      gsm_Classmark2.data[counter] = classmark2.data[counter-2];
  }
  inter_rat_capability_ptr->u.gsm->gsm_Classmark2.data[0] = 0x33;
  inter_rat_capability_ptr->u.gsm->gsm_Classmark2.data[1] = 0x3;

  /* set Classmark 3 */
#ifdef FEATURE_DUAL_SIM
  (void)gs_read_coded_classmark_3_per_subs(&classmark3,&classmark_params);
#else
  (void)gs_read_coded_classmark_3(&classmark3,
    tdsrrcmcm_is_dualmode_enabled(), TRUE, TRUE, TRUE);
#endif

  if(classmark3.length < 1)
  {
    classmark3.length = 1;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Classmark3 Len is rcvd");
  }
  if(classmark3.length > 32)
  {
    classmark3.length = 32;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Classmark3 Len is rcvd");
  }
 

  
  inter_rat_capability_ptr->u.gsm->gsm_Classmark3.numocts= classmark3.length;
  for(counter = 0; (counter < classmark3.length) && (counter < TDSCLASSMARK3_DATA_MAX_LENGTH); counter++)
  {
    inter_rat_capability_ptr->u.gsm->
      gsm_Classmark3.data[counter] = classmark3.data[counter];
  }
 
}/* tdsrrcueci_append_gsm_specific_capability */
/*===========================================================================
FUNCTION tdsrrcueci_send_ue_capability_info_msg

DESCRIPTION
  This function builds the UE CAPABILITY INFORMATION message and calls the 
  the SEND CHAIN to transmit the message in the uplink

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcueci_status_e_type tdsrrcueci_send_ue_capability_info_msg
(
  void 
)
{  
  rlc_lc_id_type ul_lc_id;                              /* Uplink logical channel ID */
  tdsrrcueci_status_e_type status = TDSRRCUECI_FAILURE_MINOR; /* A local variable to store the status */
  tdsrrc_UL_DCCH_Message *msg_ptr;                         /* Pointer to uplink DCCH message */ 

  uint32 num_rat=0;
  boolean support_ho_to_gan = FALSE;

#ifdef FEATURE_TDSCDMA_TO_LTE
    boolean eutra_cap_req = FALSE;
#endif

#ifdef FEATURE_TDSCDMA_TX_DIVERSITY_DL_FOR_MIMO
  boolean support_tx_diversity_mimo = FALSE;
#endif /*FEATURE_TDSCDMA_TX_DIVERSITY_DL_FOR_MIMO*/

#ifdef FEATURE_TDSCDMA_TX_DIVERSITY_DL_FOR_MIMO
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
    support_tx_diversity_mimo = TRUE;
  }
#endif



  /* Checks whether Logical Channel entity setup for DCCH logical
     Channel type */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If RLC Logical Channel Id is already setup, the send UE CAPABILITY
     INFORMATION message through SEND_CHAIN */
  if(ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    /* Allocate a buffer for an uplink DCCH Message. This buffer should be released
       by SEND_CHAIN  */
    msg_ptr = (tdsrrc_UL_DCCH_Message *) tdsrrc_malloc ( 
                                       sizeof(struct tdsrrc_UL_DCCH_Message));
    if (msg_ptr == NULL)
    {
       ERR_FATAL("Memory alloc failed for UE Cap Info Msg",0,0,0);
    }
    /* Select the message as UE CAPABILITY INFORMATION message */
    msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_ueCapabilityInformation;
    msg_ptr->message.u.ueCapabilityInformation.ue_RATSpecificCapability.n = 0;

    /* Set optional fields to exclude */  
    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation);
  
    if(tdsrrcueci_ue_cap_enquiry_trans_id < 4)
    { 
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.ueCapabilityInformation,
            tdsrrc_UECapabilityInformation,rrc_TransactionIdentifier);

      /* Set the UE Capability Enquiry Transaction Id */
      msg_ptr->message.u.ueCapabilityInformation.rrc_TransactionIdentifier = 
        tdsrrcueci_ue_cap_enquiry_trans_id;
    }
    
#ifdef FEATURE_TDSCDMA_TO_LTE
        if(tdsrrcmcm_is_lte_mode_enabled())
        {
          uint8 count;
          for(count = 0; (count < tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count) 
                && (count < TDSRRC_MAX_SYS_CAPABILITY); count++)
          {
            if(tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[count] == TDSRRC_EUTRA_CAP)
            {
              eutra_cap_req = TRUE;
              break;
            }
          }
        }
#endif

    if ((tdsrrc_capability_update_requirement.
        ue_radio_capability_update_requirement == TRUE) 
#ifdef FEATURE_TDSCDMA_TO_LTE
          /* Update the radio access capabilities along with EUTRA system specfic capbabilities 
             even if ue_radio_capability_update_requirement=FALSE, as UE should send valid values for 
             critical extentions when sending system specific cap.*/
           || (eutra_cap_req)
#endif
      )
    {
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.ueCapabilityInformation,
        tdsrrc_UECapabilityInformation,ue_RadioAccessCapability);


      /* Appends the Radio Access Capability to the UE CAPABILITY INFORMATION message */  
      (void)tdsrrcueci_append_ue_radio_access_capability(
                  &msg_ptr->message.u.ueCapabilityInformation.ue_RadioAccessCapability,
                  FALSE,
                  TDSRRCUECI_CAP_UPDATE);

      // Set ueCapabilityInformation.bit_mask to include v370NonCriticalExtensions
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.ueCapabilityInformation,
          tdsrrc_UECapabilityInformation,v370NonCriticalExtensions);

      // Initialize ueCapabilityInformation_v370ext.bit_mask
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      ueCapabilityInformation_v370ext);


      // Set v370NonCriticalExtensions.bit_mask to include v380NonCriticalExtensions
      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      m.v380NonCriticalExtensionsPresent =1;

      // Set ueCapabilityInformation_v380ext.bit_mask to include RadioAccessCapability_v380ext
      /*TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      v380NonCriticalExtensions.ueCapabilityInformation_v380ext, 
        ue_RadioAccessCapability_v380ext);*/
      
      // Set rx_tx_TimeDifferenceType2Capable to FALSE. Revisit this section when UE supports this.
      /*msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      v380NonCriticalExtensions.ueCapabilityInformation_v380ext.ue_RadioAccessCapability_v380ext.
      ue_PositioningCapabilityExt_v380.rx_tx_TimeDifferenceType2Capable = FALSE;*/
      
      // Initialize bit_mask of dl_PhysChCapabilityFDD_v380ext 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      v380NonCriticalExtensions.ueCapabilityInformation_v380ext.
      dl_PhysChCapabilityFDD_v380ext);

      // Initialize v380NonCriticalExtensions.bit_mask 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      v380NonCriticalExtensions);
      
      /* We always attach the Rel 5 UE Cap msg. */
      //if (msg_version == TDSMSG_REL5)
      /* Append FDD2 list if present */

      
      // Select v3a0NonCriticalExtensions on v380NonCriticalExtensions 
      msg_ptr->message.u.ueCapabilityInformation.
      v370NonCriticalExtensions.v380NonCriticalExtensions.m.v3a0NonCriticalExtensionsPresent =1;

      // Set v3a0NonCriticalExtensions.bit_mask to include laterNonCriticalExtensions
      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.m.laterNonCriticalExtensionsPresent =1;

      // Initialize ueCapabilityInformation_v3a0ext.bit_mask 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.ueCapabilityInformation_v3a0ext);

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
      if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) && (tdsrrc_ue_pos_cap_pch))
      {
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.v380NonCriticalExtensions.
          v3a0NonCriticalExtensions.ueCapabilityInformation_v3a0ext, ue_RadioAccessCapability_v3a0ext);
        msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.v380NonCriticalExtensions.
          v3a0NonCriticalExtensions.ueCapabilityInformation_v3a0ext.ue_RadioAccessCapability_v3a0ext.ue_PositioningCapabilityExt_v3a0.
          validity_CellPCH_UraPCH = tdsrrc_UE_PositioningCapabilityExt_v3a0_validity_CellPCH_UraPCH_true_;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: PCH validity set");
      }
#endif
      // Initialize laterNonCriticalExtensions.bit_mask 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions);

      /* Select v4b0NonCriticalExtensions on laterNonCriticalExtensions */
      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent =1;
    
      /* Select ueCapabilityInformation_v4b0ext on v4b0NonCriticalExtensions for
             accessStratumReleaseIndicator */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext,
        ue_RadioAccessCapability_v4b0ext);
    
      /* Select accessStratumReleaseIndicator on ue_RadioAccessCapability_v4b0ext */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext,accessStratumReleaseIndicator);

      /* Set accessStratumReleaseIndicator to be Release 4. */
      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_4;

      /* Finish ue_RadioAccessCapability_v4b0ext */
      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.pdcp_Capability_r4_ext.supportForRfc3095.t =
        T_tdsrrc_PDCP_Capability_r4_ext_supportForRfc3095_notSupported;

#ifdef FEATURE_UMTS_PDCP
      if (tdsrrc_pdcp_enabled)
      { 
        msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.ue_RadioAccessCapability_v4b0ext.
          pdcp_Capability_r4_ext.supportForRfc3095.t = T_tdsrrc_PDCP_Capability_r4_ext_supportForRfc3095_supported;

        msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.ue_RadioAccessCapability_v4b0ext.
          pdcp_Capability_r4_ext.supportForRfc3095.u.supported = 
          rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_PDCP_Capability_r4_ext_supported );
   

        /* NOTE:  When changing the below IE, need to also update check in rrcllcrbie, func tdsrrcllc_get_pdcp_info(), that 
                 checks for the chosen MAX_CID signalled by NW is not greater than the value "Maximum number of ROHC context sessions" as
                 indicated in the IE "PDCP Capability" below.
                */
        msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.ue_RadioAccessCapability_v4b0ext.
          pdcp_Capability_r4_ext.supportForRfc3095.u.supported->maxROHC_ContextSessions = tdsrrc_MaxROHC_ContextSessions_r4_s4;
        
        
        msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.ue_RadioAccessCapability_v4b0ext.
          pdcp_Capability_r4_ext.supportForRfc3095.u.supported->reverseCompressionDepth = 0;
      }
#endif /* FEATURE_UMTS_PDCP */
      /* Set TDD RF capability */
      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.m.tdd_CapabilityExtPresent = 1;

      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability);

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability.m.tddRF_CapabilityPresent = 1;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability.tddRF_Capability.
        ue_PowerClass = 2;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability.tddRF_Capability.
        radioFrequencyBandTDDList = tdsrrc_RadioFrequencyBandTDDList_a;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability.tddRF_Capability.
        chipRateCapability = tdsrrc_ChipRateCapability_mcps1_28;

      /* Set physical Channel Capability */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR);

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.m.
        tdd128_PhysChCapabilityPresent = 1;

      /* DL physical channel capability */
      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.downlinkPhysChCapability.maxTS_PerSubFrame = 4;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.downlinkPhysChCapability.maxPhysChPerFrame = 64;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.downlinkPhysChCapability.minimumSF = tdsrrc_MinimumSF_DL_sf1;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.downlinkPhysChCapability.supportOfPDSCH = FALSE;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.downlinkPhysChCapability.maxPhysChPerTS = 16;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.downlinkPhysChCapability.supportOf8PSK = FALSE;

      /* UL physical channel capability */
      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.uplinkPhysChCapability.maxTS_PerSubFrame = 5;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.uplinkPhysChCapability.maxPhysChPerTimeslot = tdsrrc_MaxPhysChPerTimeslot_ts2;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.uplinkPhysChCapability.minimumSF = tdsrrc_MinimumSF_UL_sf1;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.uplinkPhysChCapability.supportOfPUSCH = FALSE;

      msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.uplinkPhysChCapability.supportOf8PSK = FALSE;

      /* Downlink TDD measurement are not supported   */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.
        measurementCapability_r4_ext.downlinkCompressedMode_LCR);

      /* Uplink TDD measurement are not supported   */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.
        measurementCapability_r4_ext.uplinkCompressedMode_LCR);
                

        
      if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
      {

        if((support_ho_to_gan)
          || (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
          )
        {
         

          /* Set the bitmask for r3_add_ext */
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions,
            ueCapabilityInformation_r3_add_ext);

    

          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          ueCapabilityInformation_r3_add_ext);

          if(support_ho_to_gan)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Populating r3_add_ext in  for GAN HO support");
            TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext,
            tdsrrc_UECapabilityInformation_r3_add_ext_IEs,v680NonCriticalExtensions);

            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions);

            TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions.
            ueCapabilityInformation_v680ext.ue_RadioAccessCapability_v680ext.
            multiModeRAT_Capability_v680ext,supportOfHandoverToGAN);

            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions.
            ueCapabilityInformation_v680ext.ue_RadioAccessCapability_v680ext.
            multiModeRAT_Capability_v680ext.supportOfHandoverToGAN = 
            tdsrrc_MultiModeRAT_Capability_v680ext_supportOfHandoverToGAN_doesSupportHandoverToGAN;
          }
          if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
          {
            if(!(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              ueCapabilityInformation_r3_add_ext.m.v680NonCriticalExtensionsPresent))
            {
              msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              ueCapabilityInformation_r3_add_ext.m.v680NonCriticalExtensionsPresent =1;
  
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions);
  
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions.
              ueCapabilityInformation_v680ext.ue_RadioAccessCapability_v680ext.
              multiModeRAT_Capability_v680ext);
            }

            /* CCSA structure is different for ueCapabilityInformation_r3_add_ext than in 3GPP.
               ueCapabilityInformation_TDD128ext field need to be filled for TDSCDMA */

            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions.m.
            laterNonCriticalExtensionsPresent = 1;

            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions.laterNonCriticalExtensions);

            /* Set ueCapabilityInformation_TDD128ex */
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_TDD128ext);

            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_TDD128ext.m.ue_SpecificCapabilityInformationPresent = 1;

            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_r3_add_ext.v680NonCriticalExtensions.laterNonCriticalExtensions.
            ueCapabilityInformation_TDD128ext.ue_SpecificCapabilityInformation = 
            tdsrrc_UE_SpecificCapabilityInformation_LCRTDD_nF;
          }
         
        }

        if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE Cap REL5");

#ifdef FEATURE_TDSCDMA_REL8
#ifdef FEATURE_TDSCDMA_REL9
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9)
          {
            /* Set accessStratumReleaseIndicator to be Release 9. */
            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
              ue_RadioAccessCapability_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_9;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-9");
          }
          else
#endif /*FEATURE_TDSCDMA_REL9*/
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
          {
            /* Set accessStratumReleaseIndicator to be Release 8. */
            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
              ue_RadioAccessCapability_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_8;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-8");
          }
          else
#endif /*FEATURE_TDSCDMA_REL8*/
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
          {
            /* Set accessStratumReleaseIndicator to be Release 6. */
            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
              ue_RadioAccessCapability_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_7;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-7");
          }
          else
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
          {
            /* Set accessStratumReleaseIndicator to be Release 6. */
            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
              ue_RadioAccessCapability_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_6;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-6");
          }
          else
          {
            /* Set accessStratumReleaseIndicator to be Release 5. */
            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
              ue_RadioAccessCapability_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_5;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-5");
          }
        
          /* DL physical channel capability */
          msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
            ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
            tdd128_PhysChCapability.downlinkPhysChCapability.maxTS_PerSubFrame = 5;

          msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.ueCapabilityInformation_v4b0ext.
            ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
            tdd128_PhysChCapability.downlinkPhysChCapability.maxPhysChPerFrame = 80;

          /* Select v590NonCriticalExtensions on v4b0NonCriticalExtensions for accessing 590 extension */
          msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent =1;

          /* No 590 noncritical extension */
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.v590NonCriticalExtensions);
        
          /* Select RadioAccessCapability_v590ext on ueCapabilityInformation_v590ext */
          TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v4b0NonCriticalExtensions.v590NonCriticalExtensions.ueCapabilityInformation_v590ext,
            tdsrrc_UECapabilityInformation_v590ext,ue_RadioAccessCapability_v590ext);
    
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
          {
            /* v590 noncritical extension present */
            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v5c0NonCriticalExtensionsPresent = 1;
    
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.v590NonCriticalExtensions.
              v5c0NonCriticalExtensions.ueCapabilityInformation_v5c0ext);
    
            /* v5c0 noncritical extension present */
            msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.v590NonCriticalExtensions.
              v5c0NonCriticalExtensions.m.v690NonCriticalExtensionsPresent = 1;
    
            /* No v6xy noncritical extension */
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.v590NonCriticalExtensions.
              v5c0NonCriticalExtensions.v690NonCriticalExtensions);
    
            /* No v6xy noncritical extension */
            TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.ueCapabilityInformation.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.v590NonCriticalExtensions.
              v5c0NonCriticalExtensions.v690NonCriticalExtensions.ueCapabilityInformation_v690ext,
              tdsrrc_UECapabilityInformation_v690ext_IEs,ueCapabilityContainer);
          }

         /* Appends the Radio Access Capability to the RRC CONNECTION SETUP
                COMPLETE message */
          tdsrrcueci_append_ue_radio_access_capability_later_than_r3(&msg_ptr->message.u.ueCapabilityInformation.
                                                     v370NonCriticalExtensions.
                                                     v380NonCriticalExtensions.
                                                     v3a0NonCriticalExtensions.
                                                     laterNonCriticalExtensions.
                                                     v4b0NonCriticalExtensions.
                                                     v590NonCriticalExtensions.
                                                     ueCapabilityInformation_v590ext.
                                                     ue_RadioAccessCapability_v590ext,
                                                     &msg_ptr->message.u.ueCapabilityInformation.
                                                     v370NonCriticalExtensions.
                                                     v380NonCriticalExtensions.
                                                     v3a0NonCriticalExtensions.
                                                     laterNonCriticalExtensions.
                                                     v4b0NonCriticalExtensions.
                                                     v590NonCriticalExtensions.
                                                     v5c0NonCriticalExtensions.
                                                     v690NonCriticalExtensions.
                                                     ueCapabilityInformation_v690ext.
                                                     ueCapabilityContainer,
                                                     TDSRRCUECI_CAP_UPDATE);
        }
      }
    }
        if (tdsrrcmcm_is_dualmode_enabled())
        {
          if((tdsrrc_capability_update_requirement.system_specific_cap_update_req_present == TRUE)&&
          (tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[0] == TDSRRC_GSM_CAP))
          {
            num_rat++;
          }
        }
        if(num_rat !=0)
        {
          msg_ptr->message.u.ueCapabilityInformation.ue_RATSpecificCapability.n= num_rat;
          ALLOC_ASN1ARRAY1(&tdsenc_ctxt,&msg_ptr->message.u.ueCapabilityInformation.ue_RATSpecificCapability,tdsrrc_InterRAT_UE_RadioAccessCapability);
              if(msg_ptr->message.u.ueCapabilityInformation.ue_RATSpecificCapability.elem == NULL)
          {
            ERR_FATAL("Failed allocate memory ratspecCap",0,0,0);
          }
        }

    if (tdsrrcmcm_is_dualmode_enabled())
    {
      if (tdsrrc_capability_update_requirement.
          system_specific_cap_update_req_present == TRUE) 
      /* For Single mode operation IE "UE system specific capability" will not
         be included even though UTRAN requests the same */
      {   
        uint8 count =0;
        boolean gsm_cap_req = FALSE;
  
        for(count = 0; count < tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count && count < 16;count++)
        {
          if(tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[count] == TDSRRC_GSM_CAP)
          {
            gsm_cap_req = TRUE;
            break;
          }
        }
        if(gsm_cap_req)
        {
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.ueCapabilityInformation, ue_RATSpecificCapability);

          /* Make sure memory is allocated for the first element of the linked list */ 
          msg_ptr->message.u.ueCapabilityInformation.ue_RATSpecificCapability.n = 1;
          ALLOC_ASN1ARRAY1(&tdsenc_ctxt,&msg_ptr->message.u.ueCapabilityInformation.ue_RATSpecificCapability,tdsrrc_InterRAT_UE_RadioAccessCapability);

          if(msg_ptr->message.u.ueCapabilityInformation.ue_RATSpecificCapability.elem == NULL)
          {
            ERR_FATAL("Failed allocate memory ratspecCap",0,0,0);
          }      

          tdsrrcueci_append_gsm_specific_capability(
            &msg_ptr->message.u.ueCapabilityInformation.ue_RATSpecificCapability.elem[0]);
        }
      }
    }


    /* This places SDU into RLC watermark queue. */
    if(TDSRRCSEND_SUCCESS== 
       tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RCE,msg_ptr,ul_lc_id,
       TDSRRCSEND_L2ACK_NOT_REQUIRED) )
    {
      status = TDSRRCUECI_SUCCESS;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE Capability Info Msg is sent");
    }
  }
  else
  { 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
  } 
  
  return (status);            
} /* tdsrrcueci_send_ue_capability_info_msg */
/*===========================================================================

FUNCTION tdsrrcueci_convert_t304_to_ms

DESCRIPTION
  This function converts the tdsrrc_T_304 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
static int tdsrrcueci_convert_t304_to_ms
(
  tdsrrc_T_304 rrcueci_t_304                  /* enum for Timer T304 */
)
{
  int timer_value;                         /* Timer Value in milli seconds */

  switch (rrcueci_t_304)
  {
    case tdsrrc_T_304_ms100:
     timer_value = 100;
      break;
    case tdsrrc_T_304_ms200:
      timer_value = 200;
      break;      
    case tdsrrc_T_304_ms400:
      timer_value = 400;
      break;
    case tdsrrc_T_304_ms1000:
      timer_value = 1000;
      break;
    case tdsrrc_T_304_ms2000:
      timer_value = 2000;
      break;
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unknown Timer value received from SIB1");
      timer_value = 100;  /* Set lowest timer value as a default */
      break;
  }
  return (timer_value);
} /* tdsrrcueci_convert_t304_to_ms*/

/*===========================================================================

FUNCTION tdsrrcueci_increment_v304_start_t304

DESCRIPTION
  This function increments counter V304 and starts the timer T304.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcueci_increment_v304_start_t304
(
  void
)
{
  /* Increment the counter V304 */
   tdsrrc_v_304++;

  /* Start the Timer T304 */
  tdsrrctmr_start_timer(TDSRRCTMR_T_304_TIMER,
                     (uint32)tdsrrcueci_convert_t304_to_ms( TDSRRC_GET_T304() ));

} /* tdsrrcueci_increment_v304_start_t304*/

/*===========================================================================

FUNCTION tdsrrcueci_process_ueci_initial_substate

DESCRIPTION
  This function processes TDSRRC_INITIATE_UE_CAPABILITY_REQ event that is
  dispatched to UE Capability Information procedure in TDSRRCUECI_INITIAL substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcueci_process_ueci_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrc_cmd_type *int_cmd_ptr;            /* Pointer to the RRC Command */

  tdsrrc_state_e_type rrc_state;           /* Local var to get rrc state */

  rrc_state = tdsrrc_get_state();          /* initialize RRC state       */

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_STATUS_IND)
  {
    if(tdsrrcueci_ue_cap_enquiry_trans_id != TDSRRCUECI_INVALID_TRANSACTION_ID)
    {
      /* UECI initiated due to UE Capability changed
       * exmp: Mode change from TDS only to TDS+GSM or vice versa,
       * If condition is when there is cross over Between UECI initiated due to UE 
       * Capability changed & UTRAN has sent UE Cap enquiry, So set Trans I.D 
       * in UECI message to the one received in  UE Cap enquiry message
       */
      tdsrrcueci_ue_cap_enquiry_trans_id = (tdsrrcueci_ue_cap_enquiry_trans_id + 1) & 0x3;
    }
    else
    {
      /* UECI initiated due to UE Capability changed
       * exmp: Mode change from TDS only to TDS+GSM or vice versa,
       * So UECI should not carry Transation I.D, So set it to TDSRRCUECI_INVALID_TRANSACTION_ID,
       * Note: when we are in else then value of tdsrrcueci_ue_cap_enquiry_trans_id 
       * will be TDSRRCUECI_INVALID_TRANSACTION_ID, so below assignment operation is to
       * increase readability of code
       */
      tdsrrcueci_ue_cap_enquiry_trans_id = TDSRRCUECI_INVALID_TRANSACTION_ID;
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DUAL:TransId used during mode change is - %d",tdsrrcueci_ue_cap_enquiry_trans_id);

    /* As the bands would have changed moving from W to W+G.
      Band validation would have been done by CSP */
    memset(&tdsrrc_capability_update_requirement, 0, sizeof(tdsrrc_capability_update_requirement_type));
    tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement = TRUE;
    tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count = 1;
    tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[0] = TDSRRC_GSM_CAP;
    tdsrrc_capability_update_requirement.system_specific_cap_update_req_present = TRUE;
  }
  else
  {

    /* Verify the UE Capability Enquiry Transaction Id */
    if (tdsrrcueci_ue_cap_enquiry_trans_id == 
        cmd_ptr->cmd.initiate_ue_cap.transaction_id)
    {
      /* Ignore the Transaction */
      return;
    }

    /* Update the UE Capability Enquiry Transaction Id */
    tdsrrcueci_ue_cap_enquiry_trans_id = 
      cmd_ptr->cmd.initiate_ue_cap.transaction_id;
  }

  /* Set conter V304 to zero */
  tdsrrc_v_304 = 0;
#ifdef FEATURE_TDSCDMA_REL7_SPECRS
  if(cmd_ptr->cmd.initiate_ue_cap.originator == TDSRRC_UE_CAPABILITY_ORIGINATOR_COUEC)
  {
    tdsrrcueci_ue_cap_originator = TDSRRC_UE_CAPABILITY_ORIGINATOR_COUEC;
  }
#endif
  switch(tdsrrc_get_state())
  {
    case TDSRRC_STATE_URA_PCH:
    case TDSRRC_STATE_CELL_PCH:
      /* Allocates the buffer to RRC Internal command */
      if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
      {     
        /* Fill in the command parameters for TDSRRC_CELL_UPDATE_INITIATE_REQ */
        int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
        int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_UL_DATA_TRANSMISSION;
        int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
        int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_UECI;
 
        tdsrrc_put_int_cmd(int_cmd_ptr);             
      }
      else
      {
        ERR_FATAL("Unable to allocate memmory for: %d cmd",TDSRRC_CELL_UPDATE_INITIATE_REQ,0,0);
      }
      tdsrrcueci_substate = TDSRRCUECI_WAIT_FOR_CELL_UPDATE_COMP_CNF;
      break;

    case TDSRRC_STATE_CELL_FACH:

      if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_UECI) )
      {
        /* Wait until Cell update procedure is completed */
        tdsrrcueci_substate = TDSRRCUECI_WAIT_FOR_CELL_UPDATE_COMP_CNF;
      }
      else /* Cell Update procedure is inactive */
      {
        /* Build and send the UE Capability Information message to Send Chain */
        if(TDSRRCUECI_SUCCESS != tdsrrcueci_send_ue_capability_info_msg() )
        {
           TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to send UE Cap Info Msg: %d,%d",rrc_state,tdsrrcueci_substate);
        }
        else
        {
          /* Increment the counter V304 and start T304*/
          tdsrrcueci_increment_v304_start_t304();

          tdsrrcueci_substate = TDSRRCUECI_WAIT_FOR_T304_TO_EXPIRE;
        }
      }
      break;

    case TDSRRC_STATE_CELL_DCH:
      
      /* Build and send the UE Capability Information message to Send Chain */
      if(TDSRRCUECI_SUCCESS != tdsrrcueci_send_ue_capability_info_msg() )
      {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to send UE Cap Info Msg: %d,%d",rrc_state,tdsrrcueci_substate);
      }
      else
      {
        /* Increment the counter V304 and start T304*/
        tdsrrcueci_increment_v304_start_t304();

        tdsrrcueci_substate = TDSRRCUECI_WAIT_FOR_T304_TO_EXPIRE;
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the UECI Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
      break;
  }

} /* tdsrrcueci_process_ueci_initial_substate */

/*===========================================================================

FUNCTION rrcueci_process_ueci_non_initial_substate

DESCRIPTION
  This function processes TDSRRC_INITIATE_UE_CAPABILITY_REQ event that is
  dispatched to UE Capability Information procedure in substate other than
  TDSRRCUECI_INITIAL.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcueci_process_ueci_in_non_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_STATUS_IND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DUAL:%d(0: UE,1: UTRAN) initiated UECI procedure",TDSRRC_UE_CAPABILITY_ORIGINATOR_UE);
  }
  else    
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d(0: UE,1: UTRAN) initiated UECI procedure",cmd_ptr->cmd.initiate_ue_cap.originator);
  }
  /* If the originator is UE then it means that UE capabilities have changed, so UE should 
      restart UECI procedure. Otherwise no need to re-start UECI procedure.
  */
  if (TDSRRC_UE_CAPABILITY_ORIGINATOR_UE == cmd_ptr->cmd.initiate_ue_cap.originator 
      || cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_STATUS_IND
     )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reset and restart UECI procedure in UECI substate: %d(1:WaitCUCnf, 2:WaitT304Exp",tdsrrcueci_substate);

    /* if RRC reset UECI procedure before UECI procedure is complete then there is a possibility
        that UE may receive UECI confirm message from network with old transaction identifier.
        UE will drop this message and waits for the UECI confirm message with correct
        transaction identifier.
    */
    tdsrrcueci_reset_ueci_procedure();
    tdsrrcueci_process_ueci_initial_substate(cmd_ptr);
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in UECI Substate: %d in CELL_FACH",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);

  }          

  /*What if UECI procedure is active when INITIATE UE CAP REQ is initiated by COUEC Procedure.*/
}

/*===========================================================================

FUNCTION tdsrrcueci_process_ueci_wait_for_cell_update_comp_cnf

DESCRIPTION
  This function processes all events that are  dispatched
  to IUE Capability Information procedure  in 
  TDSRRCUECI_WAIT_FOR_CELL_UPDATE_COMP_CNF substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcueci_process_ueci_wait_for_cell_update_comp_cnf
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{

  tdsrrc_state_e_type rrc_state;           /* Local var to get rrc state */

  rrc_state = tdsrrc_get_state();          /* initialize RRC state       */

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      /* Build and send the UE Capability Information message to Send Chain */
      if(TDSRRCUECI_SUCCESS != tdsrrcueci_send_ue_capability_info_msg() )
      {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to send UE Cap Info Msg: %d,%d",rrc_state,tdsrrcueci_substate);
      }
      else
      {
        /* Increment the counter V304 and start T304*/
        tdsrrcueci_increment_v304_start_t304();

        tdsrrcueci_substate = TDSRRCUECI_WAIT_FOR_T304_TO_EXPIRE;
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the UECI Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
      break;
  }
 
} /* tdsrrcueci_process_ueci_wait_for_cell_update_comp_cnf */

/*===========================================================================

FUNCTION tdsrrcueci_process_ueci_wait_for_t304_to_expire

DESCRIPTION
  This function processes all events that are  dispatched
  to UE Capability Information procedure  in TDSRRCUECI_WAIT_FOR_T304_TO_EXPIRE
  substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcueci_process_ueci_wait_for_t304_to_expire
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrc_cmd_type *int_cmd_ptr;            /* Pointer to the RRC Command */

  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;  /* Pointer to Downlink DCCH */
  
  uint8 message_choice;                 /* Local varibale for message
                                           t */
  rlc_lc_id_type lc_id;                      /* Local variable to store 
                                                RLC LC Id */

  tdsrrc_state_e_type rrc_state;           /* Local var to get rrc state */

  rrc_state = tdsrrc_get_state();          /* initialize RRC state       */

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_T304_EXPIRED_IND:
      
      /* Gets N304 from tdsrrc_ue_conn_timers_and_constants database */
      if(tdsrrc_v_304 <= TDSRRC_GET_N304())               
      {        
        /* Build and send the UE Capability Information message to Send 
           Chain */
        if(TDSRRCUECI_SUCCESS != tdsrrcueci_send_ue_capability_info_msg() )
        {
           TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to send UE Cap Info Msg: %d,%d",rrc_state,tdsrrcueci_substate);
        }
        else
        {
          /* Increment the counter V304 and start T304*/
          tdsrrcueci_increment_v304_start_t304();

          tdsrrcueci_substate = TDSRRCUECI_WAIT_FOR_T304_TO_EXPIRE;
        }
      }
      else /* Radio Link Failure */
      {
        /* Allocates the buffer to RRC Internal command */
        if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        {
          /* Fill in the command parameters for TDSRRC_CELL_UPDATE_INITIATE_REQ */
          int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
          int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_RADIO_LINK_FAILURE;
          int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = FALSE;
          int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_UECI;
          tdsrrc_put_int_cmd(int_cmd_ptr); 

          /* Set Transaction Id to invalid */
          tdsrrcueci_ue_cap_enquiry_trans_id = TDSRRCUECI_INVALID_TRANSACTION_ID;          

          /* Reset the UECI state machine */
          tdsrrcueci_substate = TDSRRCUECI_INITIAL;
        }
        else
        {
          ERR_FATAL("Memory alloc failure for CELL UPDATE",0,0,0);
        }
      }
      break;

    case TDSRRC_DOWNLINK_SDU_IND:
      
      tdsrrc_protocol_error_reject = FALSE;
      
      /* Assert received message is Downlink DCCH message */
      if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == 
           tdsrrc_DL_DCCH_Message_PDU) 
      {
        message_choice = 
          tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

        if (cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
        {
          dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
            cmd_ptr->cmd.downlink_sdu_ind.decoded_msg; 
        }
        else
        { /* ASN1 decoding failed */
          tdsrrc_protocol_error_reject = TRUE;
        } 
        /* Assert the received message is UE Capability Information
           Confirm */
        if(  message_choice ==
              T_tdsrrc_DL_DCCH_MessageType_ueCapabilityInformationConfirm )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE Cap Info Confirm Msg is received,stored Trans I.D = 0x%x",tdsrrcueci_ue_cap_enquiry_trans_id);

          /* Stop timer T304 */
          tdsrrctmr_stop_timer(TDSRRCTMR_T_304_TIMER);

          if(dcch_msg_ptr == NULL) /* DCCH ASN1 decoding failed */
          {
#if(!defined(T_WINNT))
             /*log the protocol error*/
             tdsrrc_log_protocol_error( (uint8)tdsrrc_get_state(),
                                    (uint8)TDSRRC_PROCEDURE_UECI,
                                     (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                                     (uint8)TDSRRCLOG_ASN1_ENC_ERR
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
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cann't find the RLC Id for RB 2");
            }       
            else /* Valid RLC Id */
            {
              if(SUCCESS != tdsrrcsend_send_rrc_status_message(
                 T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError,
                                               // The protocol error cause to be placed in
                                               // the message
                 0,                            // Set transaction id to 0 since ASN1 
                                               // decoding failed 
                 tdsrrc_ReceivedMessageType_ueCapabilityInformationConfirm, 
                                               // Received message identification
                 TDSRRC_PROCEDURE_UECI,           // RRC procedure
                 lc_id,                        /* Logical Channel Id */
                 TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                         /* Indicates whether L2 Ack required or not */
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
              } 
            } /* Valid RLC Id */
          }
          else /* ASN1 decoding is successful */
          {
            if(dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.t !=
               T_tdsrrc_UECapabilityInformationConfirm_later_than_r3 )
            {
              /* If tdsrrcueci_ue_cap_enquiry_trans_id is TDSRRCUECI_INVALID_TRANSACTION_ID
               * Then it means ueci initiated due to UE Capability changed
               * exmp: Mode change from TDS only to TDS+GSM or vice versa,
               * So accept any transaction I.D in UECI Confirmation message.
               */
              if ((dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.
                  u.r3.ueCapabilityInformationConfirm_r3.
                  rrc_TransactionIdentifier ==  tdsrrcueci_ue_cap_enquiry_trans_id)||
                  (tdsrrcueci_ue_cap_enquiry_trans_id == TDSRRCUECI_INVALID_TRANSACTION_ID)
                  )
              {
#ifdef FEATURE_TDSCDMA_TO_LTE
#ifdef LTE_BAND_NUM
                sys_lte_band_mask_e_type bitmask_zero; 
                sys_lte_band_mask_e_type supported_lte_band;
                supported_lte_band = tdsrrccsp_get_lte_band_pref();
                memset(&bitmask_zero, 0x00, sizeof(sys_lte_band_mask_e_type));//xlong 0323
#endif                
#endif
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process UE Cap Info Confirm Msg as Trans I.D match");
                /* Procedure ends */
                tdsrrcueci_reset_ueci_procedure();

                /* Set Transaction Id to invalid */
                tdsrrcueci_ue_cap_enquiry_trans_id = TDSRRCUECI_INVALID_TRANSACTION_ID;

#ifdef FEATURE_TDSCDMA_TO_LTE
#ifdef LTE_BAND_NUM
                if(tdsrrcmcm_is_lte_mode_enabled() &&
                   !SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&supported_lte_band, &bitmask_zero))//xlong 0323
#else                   
                if(tdsrrcmcm_is_lte_mode_enabled() &&
                     (tdsrrccsp_get_lte_band_pref() != 0))
#endif                  
                {
                  tdsrrcueci_send_eutra_capabilitites_req_to_lte();
                }
#endif
              }
    
              else /* Invalid message is received */
              {
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
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cann't find the RLC Id for RB 2");
                }       
                else /* Valid RLC Id */
                {
                  if(SUCCESS != tdsrrcsend_send_rrc_status_message(
                     T_tdsrrc_ProtocolErrorMoreInformation_type1_ie_ValueNotComprehended,
                                                   // The protocol error cause to be placed in
                                                   // the message
                     0,                            // Set transaction id to 0 since ASN1 
                                                   // decoding failed 
                     tdsrrc_ReceivedMessageType_ueCapabilityInformationConfirm, 
                                                   // Received message identification
                     TDSRRC_PROCEDURE_UECI,           // RRC procedure
                     lc_id,                        /* Logical Channel Id */
                     TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                             /* Indicates whether L2 Ack required or not */
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
                  } 
                  else
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Tranmited RRC STATUS");
                    /* Start the Timer T304 */
                    tdsrrctmr_start_timer(TDSRRCTMR_T_304_TIMER,
                     (uint32)tdsrrcueci_convert_t304_to_ms( TDSRRC_GET_T304() ));
                  }
                } /* Valid RLC Id */


              }
            }
            else
            { 
              /* Critical extenisons are not supported.  */
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Critical extensions are not supported");

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
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cann't find the RLC Id for RB 2");
              }       
              else /* Valid RLC Id */
              {
                if(SUCCESS != tdsrrcsend_send_rrc_status_message(
                   T_tdsrrc_ProtocolErrorMoreInformation_type1_messageExtensionNotComprehended,
                                                 // The protocol error cause to be placed in
                                                 // the message
                   dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.
                   u.r3.ueCapabilityInformationConfirm_r3.
                   rrc_TransactionIdentifier,                            
                                                 // Set transaction id received transaction 

                   tdsrrc_ReceivedMessageType_ueCapabilityInformationConfirm, 
                                                 // Received message identification
                   TDSRRC_PROCEDURE_UECI,           // RRC procedure
                   lc_id,                        /* Logical Channel Id */
                   TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                           /* Indicates whether L2 Ack required or not */
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
                }
                else
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Tranmited RRC STATUS");
                  /* Start the Timer T304 */
                  tdsrrctmr_start_timer(TDSRRCTMR_T_304_TIMER,
                   (uint32)tdsrrcueci_convert_t304_to_ms( TDSRRC_GET_T304() ));
                }
              } /* Valid RLC Id */
            }
          }
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid message; % d is forwarded",message_choice);
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid SDU type %d is received",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
      }
      break;       
      
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the UECI Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
      break;
  }
 
} /* tdsrrcueci_process_ueci_wait_for_t304_to_expire */
/*===========================================================================

FUNCTION tdsrrcueci_process_disconnected_state

DESCRIPTION
  This function processes all events that are dispatched to UE Capability 
  Information procedure  in Disconnected state of Idle mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcueci_process_disconnected_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  if  (TDSRRC_STATE_CHANGE_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    /* if new_state is TDSRRC_STATE_DISCONNECTED then reset UECI procedure in
       in all UECI sub_states. So no need to check for UECI sub_state.
    */
    if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
    {
      if (TDSRRCUECI_INITIAL != tdsrrcueci_substate)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reset UECI procedure as RRC state is Disconneted. Cur UECI state: %d",tdsrrcueci_substate);
        /* Reset the UECI state machine */
        tdsrrcueci_reset_ueci_procedure();

        /* Set Transaction Id to invalid */
        tdsrrcueci_ue_cap_enquiry_trans_id = TDSRRCUECI_INVALID_TRANSACTION_ID;
      }

    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCSCMGR posted unregistered event to UECI procedure");
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unexpected cmd id: %d. to UECI procedure in Disconnected RRC State",cmd_ptr->cmd_hdr.cmd_id);
  }
}/* tdsrrcueci_process_disconnected_state */

/*===========================================================================

FUNCTION tdsrrcueci_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to UE Capability 
  Information procedure  in CELL_FACH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcueci_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcueci_substate)
  {
    case TDSRRCUECI_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_MODE_STATUS_IND:
        case TDSRRC_INITIATE_UE_CAPABILITY_REQ:
          tdsrrcueci_process_ueci_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in UECI Substate: %d in CELL_FACH",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
         break;
      }
      break;

    case TDSRRCUECI_WAIT_FOR_CELL_UPDATE_COMP_CNF:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
          tdsrrcueci_process_ueci_wait_for_cell_update_comp_cnf(cmd_ptr);
          break;
        
        case TDSRRC_MODE_STATUS_IND:
        case TDSRRC_INITIATE_UE_CAPABILITY_REQ:
          tdsrrcueci_process_ueci_in_non_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in UECI Substate: %d in CELL_FACH",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
          break;
      }
      break;

    case TDSRRCUECI_WAIT_FOR_T304_TO_EXPIRE: 
                                 /* UECI waits for T304 to expire */
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_T304_EXPIRED_IND:
          tdsrrcueci_process_ueci_wait_for_t304_to_expire(cmd_ptr);
          break;

        case TDSRRC_DOWNLINK_SDU_IND:
          tdsrrcueci_process_ueci_wait_for_t304_to_expire(cmd_ptr);
          break;

        case TDSRRC_MODE_STATUS_IND:
        case TDSRRC_INITIATE_UE_CAPABILITY_REQ:
          tdsrrcueci_process_ueci_in_non_initial_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in UECI Substate: %d in CELL_FACH",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
          break;
      }
      break;

   default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid UECI Substate: %d in CELL_FACH",tdsrrcueci_substate);
      break;
  }  
}/* tdsrrcueci_process_cell_fach_state */
/*===========================================================================

FUNCTION tdsrrcueci_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to UE Capability 
  Information procedure  in CELL_DCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcueci_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcueci_substate)
  {
    case TDSRRCUECI_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_MODE_STATUS_IND:
        case TDSRRC_INITIATE_UE_CAPABILITY_REQ:
          tdsrrcueci_process_ueci_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in UECI Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
          break;
      }
      break;

    case TDSRRCUECI_WAIT_FOR_T304_TO_EXPIRE: 
                                 /* UECI waits for T304 to expire */
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_T304_EXPIRED_IND:
          tdsrrcueci_process_ueci_wait_for_t304_to_expire(cmd_ptr);
          break;

        case TDSRRC_DOWNLINK_SDU_IND:
          tdsrrcueci_process_ueci_wait_for_t304_to_expire(cmd_ptr);
          break;  
     
        case TDSRRC_MODE_STATUS_IND:
        case TDSRRC_INITIATE_UE_CAPABILITY_REQ:
          tdsrrcueci_process_ueci_in_non_initial_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in UECI Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
          break;
      }
      break;

   default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid UECI Substate: %d in CELL_DCH",tdsrrcueci_substate);
      break;
  }  
}/* tdsrrcueci_process_cell_dch_state */

/*===========================================================================

FUNCTION tdsrrcueci_process_cell_ura_pch_state

DESCRIPTION
  This function processes all events that are dispatched to UE Capability 
  Information procedure  in either in CELL_PCH or URA_PCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcueci_process_cell_ura_pch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcueci_substate)
  {
    case TDSRRCUECI_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_MODE_STATUS_IND:
        case TDSRRC_INITIATE_UE_CAPABILITY_REQ:
          tdsrrcueci_process_ueci_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in UECI Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcueci_substate);
          break;
      }
      break;

    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid UECI Substate:%d in PCH",tdsrrcueci_substate);
      break;
  }  
}/* tdsrrcueci_process_cell_ura_pch_state */

/*===========================================================================

FUNCTION tdsrrcueci_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the UE Capability Enquiry procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC) containing UE 
    CAPABILITY INFORMATION CONFIRM message.

    TDSRRC_INITIATE_UE_CAPABILITY_REQ Primitive
    TDSRRC_STATE_CHANGE_IND Primitive
    TDSRRC_CELL_UPDATE_COMPLETE_CNF Primitive

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcueci_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  /* if RRC Connection release procedure is active then ignore the command,
      also reset UECI procedure variables.
  */
  
#ifdef FEATURE_TDSCDMA_TO_LTE
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MSGR_CMD) && 
     (cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_RSP))
  {
    if (cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.lte_rrc_eutra_capabilites_rsp.dest_rat == LTE_RRC_IRAT_UTRA_TDD)
    {
    tdsrrcueci_save_eutra_capability(&cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.lte_rrc_eutra_capabilites_rsp);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rcvd LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_RSP for other tech: %d. Ignored",cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.lte_rrc_eutra_capabilites_rsp.dest_rat);
      
    }
    return;
  }
#endif

  if (FALSE == tdsrrcrcr_is_proc_active())
  {
    switch (rrc_state)
    {
      case TDSRRC_STATE_DISCONNECTED: 
        tdsrrcueci_process_disconnected_state(cmd_ptr);
        break;
  
      case TDSRRC_STATE_CELL_FACH:    
        tdsrrcueci_process_cell_fach_state(cmd_ptr); 
        break;
  
      case TDSRRC_STATE_CELL_DCH:  
        tdsrrcueci_process_cell_dch_state(cmd_ptr); 
        break;
  
      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
        tdsrrcueci_process_cell_ura_pch_state(cmd_ptr);
        break;
  
      default:  
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Event: %d is fwded in invalid State: %d",cmd_ptr->cmd_hdr.cmd_id,rrc_state);
        break;
     }  
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reset UECI procedure as it received cmd when RCR is in progress");  
    tdsrrcueci_reset_ueci_procedure();

    /* Set Transaction Id to invalid */
    tdsrrcueci_ue_cap_enquiry_trans_id = TDSRRCUECI_INVALID_TRANSACTION_ID;

  }
} /* tdsrrcueci_procedure_event_handler */
/*===========================================================================

FUNCTION tdsrrcueci_init_procedure

DESCRIPTION
  This function initializes required parameters for UE Capability Information
  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcueci_init_procedure(void)
{
  /* Set UECI procedure state to TDSRRCUECI_INITIAL at powerup */
  tdsrrcueci_substate = TDSRRCUECI_INITIAL;

  /* Set the UE Capability Transaction Id to invalid */
  tdsrrcueci_ue_cap_enquiry_trans_id = TDSRRCUECI_INVALID_TRANSACTION_ID;

  /*set support for priority based resel to TRUE by default*/
  tdsrrcueci_support_prio_based_resel = TRUE;

#ifdef FEATURE_TDSCDMA_REL7_SPECRS
  tdsrrcueci_ue_cap_originator = TDSRRC_UE_CAPABILITY_ORIGINATOR_NONE;
#endif
  /* Register for transition from any state to Disconnected state */
  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_UECI,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_DISCONNECTED);
#ifdef FEATURE_TDSCDMA_TO_LTE
  tdsrrcueci_interrat_lte_capability_info.lte_req_posted = tdsrrcueci_interrat_lte_capability_info.lte_rsp_received = FALSE;
  tdsrrcueci_interrat_lte_capability_info.cap_cont_ptr = NULL;
#endif

}

/*===========================================================================

FUNCTION tdsrrcueci_get_nv_items

DESCRIPTION
  This  function gets NV item TDS_RRC_NV_HSDPA_CAT and stores it in the 
  global variable 'tdsnv_hsdpa_category'.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcueci_get_nv_items
(
void
)
{
  /*read HSDPA Category Variable from NV*/
  if (tdsrrcnv_efs_read(TDS_RRC_NV_HSDPA_CAT, 
                        (byte *) &tdsnv_hsdpa_category,
                        sizeof(tdsnv_hsdpa_category)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"HSDPACATEGORY %d",tdsnv_hsdpa_category);

    if (tdsnv_hsdpa_category > TDSRRC_HSDPA_DEFAULT_CATEGORY)
    {
      tdsnv_hsdpa_category = TDSRRC_HSDPA_DEFAULT_CATEGORY;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"HSDPACATEGORY beyond max.Setting to max: %d",tdsnv_hsdpa_category);
    }
    else if (tdsnv_hsdpa_category == 0)
    {
      tdsnv_hsdpa_category = TDSRRC_HSDPA_DEFAULT_CATEGORY;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_HSDPA_CAT NV setting error. Setting to default: %d",tdsnv_hsdpa_category);
    }
  }
  else
  { 
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_HSDPA_CAT,
                                   (byte *) &tdsnv_hsdpa_category,
                                   sizeof(tdsnv_hsdpa_category),
                                   TRUE))
    {
       /* Mismatch of NV item */
       tdsnv_hsdpa_category = TDSRRC_HSDPA_DEFAULT_CATEGORY;
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_HSDPA_CAT item error. Setting to default: %d",tdsnv_hsdpa_category);
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"HSDPACATEGORY not read. Def=%d",tdsnv_hsdpa_category);
  }
}


/*===========================================================================
FUNCTION tdsrrcueci_append_ue_radio_access_capability_rel7_capability

DESCRIPTION
  This function appends the Radio Access Capability to the message for
  Release 7.
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
 void  tdsrrcueci_append_ue_radio_access_capability_rel7_capability
 (
   
    tdsrrc_UE_CapabilityContainer_IEs *ue_capability_container,
    tdsrrcueci_cap_update_cause_e_type update_cause
 )
{

  tdsrrc_UE_RadioAccessCapability_v770ext_IEs * rel7_ue_capablity = &ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
                                                           v770NonCriticalExtensions.ue_RadioAccessCapability_v770ext;
     
  if(rel7_ue_capablity != NULL )
  {
    TDSRRC_RESET_MSG_IE_PRESENT_PTR(rel7_ue_capablity);
    if(update_cause == TDSRRCUECI_CAP_UPDATE)
    {
      TDSRRC_RESET_MSG_IE_PRESENT_PTR(rel7_ue_capablity);

      TDSRRC_MSG_COMMON_SET_BITMASK_IE(rel7_ue_capablity->physicalChannelCapability,
        tdsrrc_PhysicalChannelCapability_v770ext,tddPhysChCapability_128);
		
      TDSRRC_RESET_MSG_IE_PRESENT(rel7_ue_capablity->physicalChannelCapability.
        tddPhysChCapability_128.downlinkPhysChCapability);

      rel7_ue_capablity->physicalChannelCapability.tddPhysChCapability_128.
        uplinkPhysChCapability.maxPhysChPerTimeslot = 2;

      rel7_ue_capablity->physicalChannelCapability.tddPhysChCapability_128.uplinkPhysChCapability.
        tdd128_edch.t = T_tdsrrc_UL_PhysChCapabilityTDD_128_v770ext_tdd128_edch_unsupported;

#ifdef FEATURE_TDSCDMA_HSUPA

      if (TDSCHECK_WCDMA_OPTIONAL_FEATURE_HSUPA_SUPPORTED)
      {
        rel7_ue_capablity->physicalChannelCapability.tddPhysChCapability_128.
          uplinkPhysChCapability.tdd128_edch.t = T_tdsrrc_UL_PhysChCapabilityTDD_128_v770ext_tdd128_edch_supported;

        rel7_ue_capablity->physicalChannelCapability.tddPhysChCapability_128.uplinkPhysChCapability.
          tdd128_edch.u.supported = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_UL_PhysChCapabilityTDD_128_v770ext_supported);

        rel7_ue_capablity->physicalChannelCapability.tddPhysChCapability_128.
          uplinkPhysChCapability.tdd128_edch.u.supported->tdd_edch_PhysicalLayerCategory = (uint8)tds_nv_hsupa_category;
      }
#endif /* FEATURE_TDSCDMA_HSUPA */
      /* RF capability v770 */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(rel7_ue_capablity,
        tdsrrc_UE_RadioAccessCapability_v770ext_IEs,rf_Capability);

      TDSRRC_RESET_MSG_IE_PRESENT(rel7_ue_capablity->rf_Capability);

      TDSRRC_MSG_COMMON_SET_BITMASK_IE(rel7_ue_capablity->rf_Capability,
        tdsrrc_RF_Capability_v770ext,tdd128RF_Capability);

      rel7_ue_capablity->rf_Capability.tdd128RF_Capability = tdsrrc_RadioFrequencyBandTDDList_r7_a;

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      /*MAC-EHS support can be used even for lower than 12 categories  */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(rel7_ue_capablity,
      tdsrrc_UE_RadioAccessCapability_v770ext_IEs,mac_ehsSupport);
    
      rel7_ue_capablity->mac_ehsSupport = tdsrrc_UE_RadioAccessCapability_v770ext_IEs_mac_ehsSupport_true_;
        
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
  
      /*PSR At present setting this FALSE*/
      rel7_ue_capablity->rlc_Capability.supportOfTwoLogicalChannel = FALSE;
      /*GAN capability */
      TDSRRC_RESET_MSG_IE_PRESENT(rel7_ue_capablity->multiModeRAT_Capability);
  
      TDSRRC_RESET_MSG_IE_PRESENT(rel7_ue_capablity->ue_PositioningCapability);
  
      /* N Frequency and Multi-Carrier capability */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(rel7_ue_capablity,
            tdsrrc_UE_RadioAccessCapability_v770ext_IEs,ue_specificCapabilityInformation);

      rel7_ue_capablity->ue_specificCapabilityInformation = tdsrrc_UE_SpecificCapabilityInformation_LCRTDD_nF;
    }
  }
}

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION tdsrrcueci_append_ue_radio_access_capability_rel8_capability

DESCRIPTION
  This function appends the Radio Access Capability to the message for
  Release 8.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  tdsrrcueci_append_ue_radio_access_capability_rel8_capability
(
  tdsrrc_UE_CapabilityContainer_IEs * ue_capability_container
)
{
  tdsrrc_UE_RadioAccessCapability_v860ext_IEs * v860_radioaccesscapability_ptr  = NULL;
#ifdef FEATURE_TDSCDMA_TO_LTE//xlong 0323
#ifdef LTE_BAND_NUM
  sys_lte_band_mask_e_type lte_band_pref;
  lte_band_pref = tdsrrccsp_get_lte_band_pref();
#else
  sys_band_mask_type lte_band_pref;
  lte_band_pref = tdsrrccsp_get_lte_band_pref();
#endif
#endif

  if(ue_capability_container == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Cap Container is NULL!");
    return;
  }

  v860_radioaccesscapability_ptr = &ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
    v770NonCriticalExtensions.v790NonCriticalExtensions.v860NonCriticalExtensions.ue_RadioAccessCapability_v860ext;

#ifdef FEATURE_TDS_DC_HSDPA
   if((TDSCHECK_WCDMA_OPTIONAL_FEATURE_DC_HSDPA_SUPPORTED) 
#ifdef FEATURE_TDSCDMA_BAND_SPEC_DC_HSDPA
     && (TRUE == tdsrrcueci_check_rfm_band_support_for_dual_carrier())
#endif
    )
  {
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(v860_radioaccesscapability_ptr->physicalChannelCapability.tddPhysChCapability_128.downlinkPhysChCapability,
      tdsrrc_DL_PhysChCapabilityTDD_128_v860ext,hSDSCH_physical_layer_category_extension);

    v860_radioaccesscapability_ptr->physicalChannelCapability.tddPhysChCapability_128.downlinkPhysChCapability.
      hSDSCH_physical_layer_category_extension = tdsnv_hsdpa_category;
  }
  else
#endif /*FEATURE_TDS_DC_HSDPA*/
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:HSDPA Category is set to 23.");
  }

  /*PhysicalChannelCapability-v860ext*/
  v860_radioaccesscapability_ptr->physicalChannelCapability.m.tddPhysChCapability_128Present = TRUE;
  /* hSDSCH_physical_layer_category_extension has report in R7*/
  v860_radioaccesscapability_ptr->physicalChannelCapability.tddPhysChCapability_128.downlinkPhysChCapability.m.multiCarrier_physical_layer_category_extensionPresent = FALSE;
  /* dual stream is a MIMO feature that is not supported. Set it to OFF */
  v860_radioaccesscapability_ptr->physicalChannelCapability.tddPhysChCapability_128.downlinkPhysChCapability.m.supportOfSFModeForHSPDSCHDualStreamPresent = FALSE;

  /* MultiModeRAT_Capability_v860ext: EUTRAN related, set to OFF for now */
  v860_radioaccesscapability_ptr->multiModeRAT_Capability.m.supportOfEUTRAFDDPresent = FALSE;
  v860_radioaccesscapability_ptr->multiModeRAT_Capability.m.supportOfInterRATHOToEUTRAFDDPresent = FALSE;
  v860_radioaccesscapability_ptr->multiModeRAT_Capability.m.supportOfEUTRATDDPresent = FALSE;
  v860_radioaccesscapability_ptr->multiModeRAT_Capability.m.supportOfInterRATHOToEUTRATDDPresent = FALSE;
  v860_radioaccesscapability_ptr->multiModeRAT_Capability.m.eutraFeatureGroupIndicatorsPresent = FALSE;

  /* UE_PositioningCapability_v860ext: GANSS capability set of OFF for now. 
        No requirement from CMCC and no NW SIB15bis has been observed in the field */
  v860_radioaccesscapability_ptr->ue_PositioningCapability.m.ue_GANSSPositioning_Capability_v860extPresent = 0;

  /* MeasurementCapability_v860ext: Compressed Mode, N/A. Set to OFF */
  v860_radioaccesscapability_ptr->m.measurementCapabilityPresent = FALSE;
  
  /* MeasurementCapabilityTDD: EUTRAN related, set to OFF for now */
  v860_radioaccesscapability_ptr->m.measurementCapabilityTDDPresent = FALSE;
  
  /* supportOfCommonEDCH */
  v860_radioaccesscapability_ptr->m.supportOfCommonEDCHPresent = FALSE;
  /* supportOfMACiis */
  v860_radioaccesscapability_ptr->m.supportOfMACiisPresent= FALSE;
  /* supportOfSPSOperation */
  v860_radioaccesscapability_ptr->m.supportOfSPSOperationPresent = FALSE;
  /* supportOfControlChannelDRXOperation */
  v860_radioaccesscapability_ptr->m.supportOfControlChannelDRXOperationPresent = FALSE;
  /* supportOfCSG */
  v860_radioaccesscapability_ptr->m.supportOfCSGPresent = FALSE;
  /* ue_RadioAccessCapabBandFDDList3 */
  v860_radioaccesscapability_ptr->m.ue_RadioAccessCapabBandFDDList3Present = FALSE;
  
  
  /* ue_RATSpecificCapability_v860ext */
  ue_capability_container->v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
    v790NonCriticalExtensions.v860NonCriticalExtensions.m.ue_RATSpecificCapability_v860extPresent = FALSE;

#ifdef FEATURE_TDSCDMA_TO_LTE
      TDSRRC_RESET_MSG_IE_PRESENT(v860_radioaccesscapability_ptr->multiModeRAT_Capability);
      if(tdsrrcmcm_is_lte_mode_enabled())
      {
        byte fgi=0;

#ifdef LTE_BAND_NUM
        /*Clear LTE band those are greater than 64 as TDS OTA doesn't want to extend LTE band*/
        #if(LTE_BAND_NUM == 256) //xlong 0323
        {
          lte_band_pref.bits_193_256 = 0;
          lte_band_pref.bits_129_192 = 0;
          lte_band_pref.bits_65_128  = 0;
        }
        #endif
        if(lte_rrc_lte_fdd_is_supported(lte_band_pref))//xlong 0323
#else        
        if(lte_rrc_lte_fdd_is_supported(tdsrrccsp_get_lte_band_pref()))
#endif        
        {
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(v860_radioaccesscapability_ptr->multiModeRAT_Capability,supportOfEUTRAFDD);
          v860_radioaccesscapability_ptr->multiModeRAT_Capability.supportOfEUTRAFDD = tdsrrc_MultiModeRAT_Capability_v860ext_supportOfEUTRAFDD_doesSupportEUTRAFDD;

          /* Do not support PSHO to LTE FDD for now, even LTE FDD is supported by UE */
		  //TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(v860_radioaccesscapability_ptr->multiModeRAT_Capability,supportOfInterRATHOToEUTRAFDD);
          //v860_radioaccesscapability_ptr->multiModeRAT_Capability.supportOfInterRATHOToEUTRATFDD = 
          //         tdsrrc_MultiModeRAT_Capability_v860ext_supportOfInterRATHOToEUTRAFDD_doesSupportInterRATHOToEUTRAFDD;
        }

        /* We will say TDD supported if LTE says so. */
#ifdef LTE_BAND_NUM
        if(lte_rrc_lte_tdd_is_supported(lte_band_pref))//xlong 0323
#else
        if(lte_rrc_lte_tdd_is_supported(tdsrrccsp_get_lte_band_pref()))
#endif
        {
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(v860_radioaccesscapability_ptr->multiModeRAT_Capability,supportOfEUTRATDD);
          v860_radioaccesscapability_ptr->multiModeRAT_Capability.supportOfEUTRATDD = tdsrrc_MultiModeRAT_Capability_v860ext_supportOfEUTRATDD_doesSupportEUTRATDD;

#ifdef FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT
          if(TDSCHECK_OPTIONAL_FEATURE_ENABLE_TD2L_PSHO) 
          {
            TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(v860_radioaccesscapability_ptr->multiModeRAT_Capability,supportOfInterRATHOToEUTRATDD);
            v860_radioaccesscapability_ptr->multiModeRAT_Capability.supportOfInterRATHOToEUTRATDD = 
                     tdsrrc_MultiModeRAT_Capability_v860ext_supportOfInterRATHOToEUTRATDD_doesSupportInterRATHOToEUTRATDD;
          }
#endif
        }

        v860_radioaccesscapability_ptr->multiModeRAT_Capability.m.eutraFeatureGroupIndicatorsPresent = 1;
        v860_radioaccesscapability_ptr->multiModeRAT_Capability.eutraFeatureGroupIndicators.numbits = 4;
        if (tdsrrc_csp_int_data.allow_lte_resel)
        {
        	fgi |= 0x08;
        }
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
#ifdef LTE_BAND_NUM
        if( (TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL || TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO) &&
			      lte_rrc_lte_tdd_is_supported(lte_band_pref) )//xlong 0323
#else
        if( (TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL || TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO) &&
			lte_rrc_lte_tdd_is_supported(tdsrrccsp_get_lte_band_pref()) )
#endif			
        {
        /*If UE supports LTE TDD, set this fgi bit*/
    
          fgi |= 0x04; /*Support for EUTRA measurements and reporting in connected mode*/
        }
#endif
    
        if (fgi)
        {
            b_packb(fgi,(byte *)v860_radioaccesscapability_ptr->multiModeRAT_Capability.eutraFeatureGroupIndicators.data,TDSRRC_DEFAULT_BIT_POS,4);
        }

//#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
#ifdef LTE_BAND_NUM
        if( //(TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL || TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO) &&
           lte_rrc_lte_tdd_is_supported(lte_band_pref) )//xlong 0323
#else
		if( //(TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL || TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO) &&
             lte_rrc_lte_tdd_is_supported(tdsrrccsp_get_lte_band_pref()) )
#endif
		{
		/*If UE supports LTE TDD, set this IE*/
          #if defined(LTE_BAND_NUM)&&(LTE_BAND_NUM == 256)
	    if(tdsrrcueci_append_ue_radio_access_capability_tdd
              (&v860_radioaccesscapability_ptr->measurementCapabilityTDD,(sys_band_mask_e_type)lte_band_pref.bits_1_64))
	    {
		  v860_radioaccesscapability_ptr->m.measurementCapabilityTDDPresent = 1;
	    }
          #else
            if(tdsrrcueci_append_ue_radio_access_capability_tdd
              (&v860_radioaccesscapability_ptr->measurementCapabilityTDD,lte_band_pref))
            {
              v860_radioaccesscapability_ptr->m.measurementCapabilityTDDPresent = 1;
            }
          #endif
      }
//#endif
      }
#endif

  return;
}

#ifdef FEATURE_TDS_DC_HSDPA
#ifdef FEATURE_TDSCDMA_BAND_SPEC_DC_HSDPA
/*===========================================================================
FUNCTION tdsrrcueci_check_rfm_band_support_for_dual_carrier

DESCRIPTION
  This function check the support for DC-hSDPA
  Release 8.

DEPENDENCIES
  None

RETURN VALUE
  booelan

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcueci_check_rfm_band_support_for_dual_carrier
(
  void
)
{
  boolean ret_val;
  ret_val =  rfm_band_supports_dual_carrier((uint16)tdsrrcccm_get_curr_camped_freq());
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA: DC supported is %d",ret_val);
  return ret_val;
}

#endif /* FEATURE_TDSCDMA_BAND_SPEC_DC_HSDPA */
#endif /* FEATURE_TDS_DC_HSDPA */

#endif /*FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TD2G_NACC
/*===========================================================================

FUNCTION tdsrrcueci_get_td2g_nacc_nv_item

DESCRIPTION
  This  function reads the NVs NV_GERAN_FEATURE_PACK_1_I and NV_INTERRAT_NACC_SUPPORT_I
  and based on these NV items updates global with TDSTOG NACC enabled status

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcueci_get_td2g_nacc_nv_item
(
  void
)
{
  boolean wtog_nacc_enabled = TRUE;
  uint8 geran_pack_1_enabled = 0;
  uint8 nacc_support;
  
  if (tdsrrcnv_efs_read(TDS_RRC_NV_GERAN_FEATURE_PACK, 
                        (byte *) &geran_pack_1_enabled,
                        sizeof(geran_pack_1_enabled)))
  {
    if ((geran_pack_1_enabled) && (tdsrrcnv_efs_read(TDS_RRC_NV_INTERRAT_NACC_SUPPORT, 
                                                     (byte *) &nacc_support,
                                                     sizeof(nacc_support))))
    {
      if((nacc_support & RRC_TD2G_NACC_SUPPORTED) != RRC_TD2G_NACC_SUPPORTED)
      {
        wtog_nacc_enabled = FALSE;
      }
    }
  }
  nv_td2g_nacc_enabled = wtog_nacc_enabled;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSTOG NACC NV Support %d",nv_td2g_nacc_enabled);
}
#endif

/*===========================================================================

FUNCTION tdsrrc_rr_absolute_priority_support

DESCRIPTION
  This  function tells whether supportForPriorityReselectionInUTRAN is supported by UE or not

DEPENDENCIES
  None.

RETURN VALUE
  TRUE supportForPriorityReselectionInUTRAN supported
  FALSE supportForPriorityReselectionInUTRAN not supported

===========================================================================*/
boolean tdsrrc_rr_absolute_priority_support(sys_modem_as_id_e_type tdsrrc_as_id)
{
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
  if(tdsrrcueci_support_prio_based_resel)
  {
    return TRUE;
  }
#endif
  return FALSE; 
}

