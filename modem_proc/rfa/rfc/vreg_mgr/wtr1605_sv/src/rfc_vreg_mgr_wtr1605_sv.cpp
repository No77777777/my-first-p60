/*!
   @file rfc_vreg_mgr_wtr1605_sv.c

   @brief


   @details
   This file implements the VREG Mgr for WTR1605_SV varients of rf-cards.

*/
/*===========================================================================
Copyright (c) 2012 - 2020 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:11:02 $ $Author: pwbldsvc $
$Header: //components/rel/rfa.mpss/4.11/rfc/vreg_mgr/wtr1605_sv/src/rfc_vreg_mgr_wtr1605_sv.cpp#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
03/20/20   cv      [CR2639875]Removing the attritbutes and resetting vote flag
03/11/20   ssl     [CR2639607]Requesting  NPA mode 1 in Rflte light sleep
02/21/20   cv      Handling of NPA Client for light sleep
12/16/19   ssl     Request for Creation of different npa_client for different techs 
12/09/19   ssl     Handling NPA Client handle request and Mode request
11/14/19   hk      Bandgap enablement via PAM. Cleaned up MPP.
11/10/17   pv      LTE resource management in MMCP based on clients
09/08/15   aak     Adding RF_PATH_TX_SLEEP_STATE to the RF vreg mgr
08/11/15   tks     Bypass lte-u band check in pmk8001 clock vote
07/23/15   aak     Reduce F3 messages in RF vreg mgr 
07/02/15   jfc     Add cases for TP160_3S
06/24/15   jfc     Add cases for TP160_3
06/18/15  aak/tks  Added support to vote for LTE-U NPA client
06/18/15   aak     Use only RF_CLK1 for RF_HW_WTR3925_3DLCA_NAEU3100V2_PW RF card
06/10/15   aak     Add IMT_2 NPA PAM table to TA
06/10/15   ag      Include W+W feature changes in TA
05/27/15   aak     New TP RF cards to not use RF_CLK2
05/12/15   rcui    initialize all paths to sleep
05/12/15   rcui    change  voting to be device based
04/28/15   aak     Changes to support MSM8996
04/15/15   ag      Featurizing W+W mode addition for Tabasco
04/15/15   aak     Changes to support second WCDMA mode for W+W
04/14/15   rcui    Only send sleep signal when all paths are off
04/03/15   ry      Changes to use the correct PAM tables for the wtr2955 and wtr4905 RFCs.
03/04/15   aak     Remove reference to WTR1605_SV
02/03/15   aak     Choose RF_CLK PAM mode depending on the RF card used
12/15/04   aak     Vote for RF_INIT_TECH PAM table during autocal for 9x45
12/04/14   aak     Turn off MPP in sleep, instead of putting into sink mode
11/26/14   ry      Fix compiler error
11/26/14   ry      Changes for Tabasco in PAM table usage
11/25/14   ry      Changes to use RF1 PAM tables for WTR2955 RFCs and
                   RF2 PAM tables for WTR4905 RFCs
11/03/14   ry      Changes to use RF1 PAM tables to power up WTR2955
10/13/14   dr      Adding Featurization for Tabasco
10/04/14   bm      Removed references to NA2, CHILE_SAWLESS and JAP RFC IDs
09/18/14   vv      API support to detect rf sleep status
09/17/14   dr      Adding JOLOKIA featurization
09/11/14   aak     Remove featurization for MDM9x45
08/12/14   aak     Move PAM calls for LTE light sleep to the vreg manager
08/12/14   sb      Set XO drive strength based on the WTRs present in an RFC
08/08/14   aak     Use 1x PAM table as the common PAM table instead of GSM
07/28/14   aak     Set drive strength for A0/A1 clocks
07/24/14   aak     Shut off MPP for DAC Vref in CDRX LTE sleep
07/23/14   aak     Use PAM mode to turn on RFCLK1/RFCLK2 depending on number of
                   WTR devices present on the RF card
07/14/14   aak     Use same PAM tables for all RF techs
07/09/14   aak     Changes for API to control MPP for DAC Vref
07/08/14   jr      Changes for TSTS support
07/07/14   aak     Choose sync client for RPM sleep set based on RF PAM table used
07/07/14   aak     Choose RF PAM tables based on the chipset
07/07/14   aak     Add 4905-only Jap and China RF cards to use RF2 PAM tables
06/23/14   sty     Made LDO9 changes specific to BOLT/9x35 modem - not THOR
06/23/14   sty     Made LDO9 changes specific to 9x35 modem
06/11/14   krg     Featurize out changes to enable/disable 1V rail for 8994
05/30/14   sty     Changes to enable/disable 1V rail based on RF Clocks
05/29/14   aak     Use RF2 PAM tables for 4905-only RF cards
05/29/14   aak     Use GSM PAM table for DSDS on DPM2.0
05/22/14   aak     Use EVDO PAM table instead of WCDMA in Autocal NPA request
05/20/14   aak     Use GSM PAM table for DSDS mode
05/04/14   aak     Choose correct PAM table based on PMIC/Modem and RF card used
12/26/13   aak     Choose GPS PAM table depending on the RF card
12/18/13   aak     MPP for DAC Vref needs to be turned on during DAC Cal
10/24/13   aak     Remove featurizations
10/22/13   aak     Remove TDSCDMA featurization from autocal function
10/04/13   aak     Voltage scaling always ON for WTR3925
10/03/13   aak     Remove FEATURE_DIME_MODEM from vreg_mgr files
09/09/13   aak     Do not set XO drive strength for SGLTE RF cards
09/03/13   aak     No wait in RF SW for Immediate PAM request if a tech is already ON
08/03/13   aak     Changes for voting for resources for concurrent techs
07/26/13   aak     Shut off DAC MPP when all techs are off
07/22/13   aak     Don't set XO drive strength for WTR1605 SGLTE RF card
07/10/13   JJ      Remove lagacy flag rumi_bup_cdma 
07/03/13   aak     MPP for DAC Vref always ON for SV RF cards
06/24/13   aak     Voltage scaling based on both device processes for SV RF cards
06/21/13   vs      Featurize PM_MPP_DAC_VREF calls for 8926 
05/13/13   aak     Voltage scaling based on Tx SAW for LTE
05/07/13   aak     Replace RFM_EGSM_MODE_2 with RFM_NUM_MODES
05/02/13   aak     Use GSM_2 tech only for Triton
04/30/13   aak     Changes for second GSM client for DSDS/DSDA
04/12/13   aak     Featurize changes to include GSM_2 client only for Triton
04/12/13   aak     Create LTE PAM table only if FEATURE_LTE is defined
04/13/13   aak     Resolve KW error
04/13/13   aak     Change to use different PAM handles for second GSM in DSDA
03/25/13   aak     8x26/8x10 does not need a MPP for the DAC Vref
03/17/13   lp      Disable setting lowest XO drive strength for Triton
03/12/13   ms      Featurize TDSCDMA for linker issues
03/11/13   sar     Updated file for APQ, GNSS only target.
03/07/13  shb/aak Set lowest XO drive strength at bootup
03/06/13   gy     Featurized GNSS NPA registration 
03/05/13   vrb    Replacing RFC accessor param init_req boolean by req enum
01/25/13   aak    Enable sending sleep PAM request from RF to NPA
                  RF NPA required sync client to enable forming of the sleep set
12/19/12   aak    Resolve KW errors
12/15/12   aak    Backing out changes for sending the sleep PAM
12/12/12   aak    Enable sending sleep PAM request from RF to NPA
12/12/12   aak    RF NPA required sync client to enable forming of the sleep set
11/29/12   aak    Fix to resolve concurrency issue between GPS and WAN technologies
11/27/12   aak    Change to make NPA client handles NPA_CLIENT_SUPPRESSIBLE from NPA_CLIENT_REQUIRED
10/26/12   gvn    Remove gsm & lte bring-up flags which were included in merge
10/25/12   gvn    Featurize LTE to support LTE FW API removal on Triton
10/11/12   php    LTE RUMI flag clean up
09/27/12   aak    Adding 100us delay for vregs to settle before RFFE transactions in autocal  
08/28/12   aak    Enable voting with NPA for resources. Disable sending sleep request for now.  
08/24/12   aak    Fixes for sending NPA requests from RF SW on Dime 
08/23/12   vss    Move flags from rfc constructor to autocal_vregs API 
08/02/12   kb     Ported missing changes from CL 2585079 from nikel and comented out nikel specific code. 
07/23/12   aak    Initial check-in for Dime  
07/20/12   tks    Fix compile errors  
07/18/12   sr     new struct type to capture the signal info.
07/09/12   sr     Commented-out the device info query from RFC..
07/09/12   aak    Move Quiet mode API from device to RFC  
07/02/12   aak    Use same PAM request for LTE, 1x and DO on SV RF cards 
06/20/12   aak    Use non-SV PAM for SVVZ on 9x15 
06/04/11   aak    Change for schedule wake-up for TDSCDMA 
05/22/12   aak    Change for PAM sent for SVDO/SVLTE 
05/14/12   aak    Change for LTE and 1x to send a combined PAM to MCPM in SV-LTE mode
05/03/12   sb     Fixed KW warning
04/27/12   aak    Change the check for max PAM id for GPS  
04/26/12   aak    Initialise NPA client handles to 0 in the constructor  
04/20/12   aak    SVOC and SVSP to use non-SV PAM since only A0 clock is present 
04/19/12   aak    Changes to change to C++ interface  
04/02/11   sr     Initial revision.
============================================================================

                           INCLUDE FILES
===========================================================================*/
extern "C" {
#include "rfa_variation.h"
#include "msg.h"
#include "pm.h"
#include "npa.h"
#include "pmapp_npa.h"
#include "DALSys.h"
#include "mcpm_api.h"
#ifdef FEATURE_RF_HAS_NPA_SCHEDULED_REQUEST_API
#include "npa_scheduler.h"
#endif
#include "rfm_internal.h"
}

#include "rfc_vreg_mgr_wtr1605_sv.h" 
#include "rfcommon_nv_mm.h"
#include "rfdevice_lte_interface.h"
#include "rfc_common_data.h"
#include "DDIChipInfo.h"

#include "rf_power_reporting.h"
#include "ftm.h"

boolean  rfc_onlinemode;

static rfm_mode_enum_type tech_resource_data[RFM_NUM_MODES]; 

static volatile boolean rfc_sv_npa_voting = TRUE;

static volatile boolean rf_sleep_set_sv_client_status = FALSE;

//RF NPA sync client to enable forming of the sleep set
static npa_client_handle rf_sleep_set_sv_client_handle = NULL;

static npa_client_handle tx_dac_msm_vref_client_handle = NULL;

//Count number of WTRs on the RF card
static uint8 num_trx = 0;
static boolean num_trx_once = FALSE;

//For VoLTE light sleep/wakeup
static npa_client_handle rf_light_sleep_lte_client_handle = NULL;

/*===========================================================================
                           Functions
===========================================================================*/
rfc_vreg_mgr * rfc_vreg_mgr_wtr1605_sv::get_instance()
{

  MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"rfc_vreg_mgr_wtr1605_sv::get_instance:");

  if (vreg_mgr_ptr == NULL)
  {
    vreg_mgr_ptr = (rfc_vreg_mgr *)new rfc_vreg_mgr_wtr1605_sv();
  }

  return(vreg_mgr_ptr);
}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   constructor.

   @details

*/
rfc_vreg_mgr_wtr1605_sv::rfc_vreg_mgr_wtr1605_sv(void)
: rfc_vreg_mgr()
{
  uint8 tech = 0;

  //Wait in case needed after command send to NPA for vregs/clocks to settle. Set to 650us right now.
  vreg_por_wait = 650;

  MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"rfc_vreg_mgr_wtr1605_sv::rfc_vreg_mgr_wtr1605_sv:");

  /* initialize NPA ID for all techs and all voltage scaling  */
  for (tech = RFM_PARKED_MODE; tech < RFM_NUM_MODES; tech++ )
  {
    tech_resource_data[tech] = (rfm_mode_enum_type)tech;
    rf_wan_gps_npa_handle[tech] = 0;
    last_rf_wan_gps_npa_id[tech] = PMIC_NPA_MODE_ID_RF_MODE_0;
  }

  init_rf_npa_handle();
  init_chip_info();
  //init_npa_rxtx_table();
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   Destructor.

   @details

*/
rfc_vreg_mgr_wtr1605_sv::~rfc_vreg_mgr_wtr1605_sv()
{
  // Do nothing
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the VREG management for Self Cal

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::enable_autocal_vregs(boolean on_off)
{
   if (rfc_sv_npa_voting == FALSE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: No NPA voting for Autocal vregs. Rails need to be turned on "
                                      "in PMIC Init",0);
    return;
  }

   /* Find out number of WTRs on the RF card*/
   if (num_trx_once == FALSE) 
   {
      uint8 iter = 0;

      rfc_common_data *rfc_data = rfc_common_data::get_instance();
      rfc_logical_device_info_type *logical_devices_cfg = NULL;
      logical_devices_cfg = rfc_data->get_logical_device_cfg();

      if (logical_devices_cfg == NULL) 
      {
         ERR_FATAL( "logical_devices_cfgis NULL",0, 0, 0);
      }

      /* Find out number of RF transceiver devices on the RF card being used */
      for (iter = 0; logical_devices_cfg[iter].rf_device_id != RFDEVICE_INVALID; iter++)
      {
        if (logical_devices_cfg[iter].rf_device_type == RFDEVICE_TRANSCEIVER)
        {
           num_trx++;
        }
      }

      MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"enable_autocal_vregs: Number of WTR devices on the RF card = %d",num_trx);

      if (num_trx == 0) 
      {
         //Force number of WTRs to 1
         num_trx = 1;
      }

      num_trx_once = TRUE;
   }


  if(!rfc_set_xo_drive_strength())
  {

    RF_MSG( RF_ERROR, "enable_autocal_vregs:Set desired XO drive strength failed!");
  }

  init_npa_rxtx_table();

  MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"enable_autocal_vregs:enable_autocal_vregs: %d", on_off);

  if (on_off == TRUE)
  {
     npa_issue_required_request (rf_wan_gps_npa_handle[RFM_1X_MODE], 
                                 npa_rxtx_table[RFM_1X_MODE][RFC_VOLTAGE_SCALE_ON]);

     MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"Autocal PAM mode %d", npa_rxtx_table[RFM_1X_MODE][RFC_VOLTAGE_SCALE_ON]);

     DALSYS_BusyWait(vreg_por_wait);

     /* Extra delay to let vregs settle */
     DALSYS_BusyWait(100); 

     npa_issue_required_request ( tx_dac_msm_vref_client_handle, PMIC_NPA_MODE_ID_RF_MODE_2 );

     MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Vote for RF_INIT client mode=%d", PMIC_NPA_MODE_ID_RF_MODE_2);

     last_rf_wan_gps_npa_id[RFM_1X_MODE] = npa_rxtx_table[RFM_1X_MODE][RFC_VOLTAGE_SCALE_ON];
  }
  else
  {

    npa_issue_required_request (rf_wan_gps_npa_handle[RFM_1X_MODE], PMIC_NPA_MODE_ID_RF_MODE_0); 
      
    npa_issue_required_request ( tx_dac_msm_vref_client_handle, PMIC_NPA_MODE_ID_RF_MODE_0 );
	 
     MSG(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Issuing PMIC_NPA_MODE_ID_RF_SLEEP in rf_pwr_mgr::enable_autocal_vregs()");
     last_rf_wan_gps_npa_id[RFM_1X_MODE] =  PMIC_NPA_MODE_ID_RF_MODE_0;
  }
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::enable_mpp_dac_vref(boolean on_off)
{
   if (on_off == TRUE)
   {
 	  npa_issue_required_request ( tx_dac_msm_vref_client_handle, PMIC_NPA_MODE_ID_RF_MODE_2 );
	  MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"DAC Vref turned on",0);
   }
   else 
   {
 	  npa_issue_required_request ( tx_dac_msm_vref_client_handle, PMIC_NPA_MODE_ID_RF_MODE_0 );
      MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"DAC Vref turned off",0);
   }
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::lte_light_sleep_vregs(void)
{
   MSG(MSG_SSID_RF, MSG_LEGACY_MED,"rfc_vreg_mgr_wtr1605_sv::lte_light_sleep_vregs");

   npa_issue_required_request (rf_light_sleep_lte_client_handle, PMIC_NPA_MODE_ID_RF_MODE_1);

   npa_issue_required_request ( tx_dac_msm_vref_client_handle, PMIC_NPA_MODE_ID_RF_MODE_0 );

   rf_sleep_set_sv_client_status = FALSE;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::lte_light_sleep_wakeup_vregs(void)
{

   MSG(MSG_SSID_RF, MSG_LEGACY_MED,"rfc_vreg_mgr_wtr1605_sv::lte_light_sleep_wakeup_vregs");

   npa_issue_required_request (rf_light_sleep_lte_client_handle, PMIC_NPA_MODE_ID_RF_MODE_2);

   npa_issue_required_request ( tx_dac_msm_vref_client_handle, PMIC_NPA_MODE_ID_RF_MODE_2 );

   rf_sleep_set_sv_client_status = TRUE;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the VREG management for Self Cal

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::manage_power(rfc_vreg_param &vreg_param)
{
  rfm_mode_enum_type mode = RFM_INVALID_MODE;
  rfm_path_state rf_state = RF_PATH_MAX_STATE;
  rfc_voltage_scaling_type vol_scale = RFC_VOLTAGE_SCALE_INVALID;
  uint8 npa_id = PMIC_NPA_MODE_ID_RF_MODE_0;
  static boolean timer_run = FALSE;   
  uint8 band = 0;

  /* Acquire Critical Section Lock */
  rf_common_enter_critical_section(rfc_get_lock_data());
  
  mode = vreg_param.get_mode();
  band = vreg_param.get_band();

  //Check to see mode is valid
  if ((mode <  RFM_PARKED_MODE) || (mode >= RFM_INVALID_MODE))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"manage_power:Invalid mode passed(mode=%d)hence return",mode);
    return;
  }

  rf_state = vreg_param.get_rf_state();

  //Save info for future reference
  save_mode_info(vreg_param);

  if ((rf_state == RF_PATH_RX_STATE) || (rf_state == RF_PATH_RXTX_STATE))
  {
    vol_scale = get_voltage_scaling_state(vreg_param);
    npa_id = npa_rxtx_table[mode][vol_scale];
    if (npa_id == PMIC_NPA_MODE_ID_RF_MODE_0 )
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rf_pwr_mgr::manage_power() trying to use un-initialized npa_id = %d",npa_id);
    }

    send_npa_rf_id(mode, npa_id, vreg_param);

    if (rf_state == RF_PATH_RXTX_STATE)
    {
      if (rfc_sv_npa_voting == TRUE)
      {
        // DAC VREF
        npa_issue_required_request ( tx_dac_msm_vref_client_handle, PMIC_NPA_MODE_ID_RF_MODE_2 );
	 }
    }
    #ifdef FEATURE_RF_POWER_REPORTING
    if ( (ftm_get_mode() != FTM_MODE) && (rfm_get_calibration_state() != TRUE) ) 
    {
      rfc_onlinemode = 1;
    }
    else
    {
       rfc_onlinemode = 0;
    }

    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: enable_power_logging %d", rfc_onlinemode);
    #endif

    if ( rfc_state_info[vreg_param.get_mode()] != 0 && 
         !timer_run &&  
         rfc_onlinemode ) 
    {
      start_power_reporting_timer();
      timer_run = TRUE;
    }
  #endif
  }
  else if (rf_state == RF_PATH_SLEEP_STATE) 
  {
    /* If NPA is set to Mode 2 during light wakeup 
      it needs to be set to mode 0 before sending sleep to NPA */
    if( rf_sleep_set_sv_client_status == TRUE )
    {
      npa_issue_required_request (rf_light_sleep_lte_client_handle, PMIC_NPA_MODE_ID_RF_MODE_0);
      MSG(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Setting rf_light_sleep_lte_client_handle handle to MODE0");
      rf_sleep_set_sv_client_status = FALSE;
    }
  
    #ifdef FEATURE_RF_POWER_REPORTING
    if ( rfc_state_info[vreg_param.get_mode()] == 0 && 
         timer_run && 
         rfc_onlinemode) 
    {
      stop_power_reporting_timer();
      timer_run = FALSE;
    }
    #endif
    send_sleep_npa_id(vreg_param);
  }
  else if (rf_state == RF_PATH_TX_SLEEP_STATE) 
  {
     MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Tech vote Tx sleep:mode=%d device=%d", mode, vreg_param.get_device());
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid state in rf_pwr_mgr::manage_power() %d",rf_state);
  }

  /* Release Critical Section Lock */
  rf_common_leave_critical_section(rfc_get_lock_data());
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the VREG management for Self Cal

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::manage_sv_power(rfc_vreg_param &vreg_param)
{
  // Not needed
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the VREG management for Self Cal

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::cdma_manage_power(rfc_vreg_param &vreg_param)
{
  MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"cdma_manage_power");
  manage_power(vreg_param);
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the card specific VREG management for GSM mode

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::gsm_manage_power (rfc_vreg_param &vreg_param)
{
   MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"gsm_manage_power");
  manage_power(vreg_param);
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the card specific VREG management for WCDMA mode

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::wcdma_manage_power(rfc_vreg_param &vreg_param)
{
  MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"wcdma_manage_power");
  manage_power(vreg_param);
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the card specific VREG management for TDSCDMA mode

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::tdscdma_manage_power (rfc_vreg_param &vreg_param)
{
  MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"tdscdma_manage_power");
  manage_power(vreg_param);
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the card specific VREG management for GPS mode

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::gps_manage_power(rfc_vreg_param &vreg_param)
{
  rfm_path_state rf_state = RF_PATH_MAX_STATE;
  rfc_voltage_scaling_type vol_scale = RFC_VOLTAGE_SCALE_INVALID;
  uint8 npa_id = PMIC_NPA_MODE_ID_GENERIC_OFF;
  rfm_mode_enum_type mode = RFM_INVALID_MODE;

  mode = vreg_param.get_mode();
  //Check to see mode is valid
  if (( (mode <  RFM_PARKED_MODE) || (mode >= RFM_INVALID_MODE)))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"gps_manage_power:Invalid mode passed(mode=%d)hence return",mode);
    return;
  }

  rf_state = vreg_param.get_rf_state();

  MSG_5(MSG_SSID_RF, MSG_LEGACY_MED,"Enter gps_manage_power: mode=%d, band=%d, device_num=%d,path=%d, rf state=%d", 
        vreg_param.get_mode(), vreg_param.get_band(), vreg_param.get_device(),
        vreg_param.get_path(), vreg_param.get_rf_state());

  if (rf_state == RF_PATH_RX_STATE) 
  {
    vol_scale = get_voltage_scaling_state(vreg_param);
    npa_id = npa_rxtx_table[mode][vol_scale];
    send_npa_rf_gps_id(vreg_param.get_mode(), npa_id);
  }
  else
  {
    if (rf_state == RF_PATH_SLEEP_STATE) 
    {
      send_npa_rf_gps_id(vreg_param.get_mode(), PMIC_NPA_MODE_ID_GENERIC_OFF); 
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"gps_manage_power:Invalid state in rf_pwr_mgr::gps_manage_power() %d",rf_state);
    }

    //Save info for future reference
    save_mode_info(vreg_param);

    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"Exit RF_PWR_MGR GPS",0);
  }

}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function implements the card specific VREG management for LTE mode

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::lte_manage_power(rfc_vreg_param &vreg_param)
{
  MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"lte_manage_power:");
  manage_power(vreg_param);
}
/* ----------------------------------------------------------------------- */
/*!
   @brief
   

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::get_awake_resource_request(rfm_resource_info* params)
{
  uint8 npa_id = PMIC_NPA_MODE_ID_RF_MODE_0;
  rfc_voltage_scaling_type voltage_scaling = RFC_VOLTAGE_SCALE_ON;

  voltage_scaling = rfc_voltage_scaling_state[params->tech_A.mode];

  MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"get_awake_resource_request: Tech A mode = %d, state=%d",params->tech_A.mode, params->tech_A.path_state);

  switch (params->tech_A.mode) 
  {
    case RFCOM_WCDMA_MODE:
    case RFM_IMT_MODE_2:
    case RFCOM_GSM_MODE:
    case RFM_EGSM_MODE_2:
    case RFM_EGSM_MODE_3:
    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
    case RFCOM_LTE_MODE:
    case RFM_LTE_MODE_2:	
    case RFM_TDSCDMA_MODE:
      if ((params->tech_A.path_state == RF_PATH_RX_STATE) || (params->tech_A.path_state == RF_PATH_RXTX_STATE))
      {
        npa_id = npa_rxtx_table[params->tech_A.mode][voltage_scaling];
      }
      else if (params->tech_A.path_state == RF_PATH_SLEEP_STATE) 
      {
        npa_id = PMIC_NPA_MODE_ID_RF_MODE_0;   
      }
      else
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech mode state in rf_pwr_mgr::get_awake_resource_request() mode=%d, state=%d",
              params->tech_A.mode, params->tech_A.path_state);
      }

      params->npa_id     = npa_id;
      params->npa_handle = rf_wan_gps_npa_handle[params->tech_A.mode];
      break;

    default:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech mode in rf_pwr_mgr::get_awake_resource_request() mode=%d, state=%d",
              params->tech_A.mode, params->tech_A.path_state);
      break;
  }

  MSG_3(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: get_awake_resource_request() mode=%d, npa_id=%d, npa_handle=%d", 
        params->tech_A.mode, params->npa_id, params->npa_handle);
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::set_quiet_mode(rfm_mode_enum_type mode, boolean enable)
{
  // Quiet mode controlled via PAM
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   

   @details

*/
rfc_voltage_scaling_type rfc_vreg_mgr_wtr1605_sv::get_voltage_scaling_state(rfc_vreg_param &vreg_param)
{
  /* Voltage scaling always ON for WTR3925 */
  return (RFC_VOLTAGE_SCALE_ON);  
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::send_npa_rf_id(rfcom_mode_enum_type mode, uint8 npa_id, rfc_vreg_param &vreg_param)
{
  boolean mcpmnpa_status = MCPMNPA_STAT_NULL;

  if (rfc_sv_npa_voting == FALSE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"send_npa_rf_id: No NPA voting from RF. Rails need to be turned on in PMIC Init",0);
    /* no NPA support on RUMI */
    return;
  }

  if (npa_id >= PMIC_NPA_MODE_ID_RF_MODE_MAX) 
  {
    MSG_ERROR("send_npa_rf_id: Invalid WAN NPA id: %d",npa_id,0,0);
    return;
  }

  if (npa_id == last_rf_wan_gps_npa_id[mode]) //same NPA id as the last one
  {
    MSG_5(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Same WAN NPA id as last sent - new id=%d, mode=%d, band=%d, "
                                      "device_num=%d, rf state=%d", npa_id, vreg_param.get_mode(), vreg_param.get_band(), 
                                      vreg_param.get_device(), vreg_param.get_rf_state());
    return;
  }

  // Send NPA id only if different from last one

  // RF talks to MCPM instead of NPA
  mcpmnpa_status = MCPM_Send_RF_Req(mode, npa_id, rf_wan_gps_npa_handle[mode], 0, FALSE);
    
  if ((mcpmnpa_status != MCPMNPA_STAT_OPT_SUCCESS) && (mcpmnpa_status != MCPMNPA_STAT_OPT_IMM_REQ_SENT))
  {
    // RF sends PAM request to NPA
    npa_issue_required_request (rf_wan_gps_npa_handle[mode], npa_id);
    DALSYS_BusyWait(vreg_por_wait);
    print_npa_rf_id_msg(npa_id);
  }
  else if (mcpmnpa_status == MCPMNPA_STAT_OPT_IMM_REQ_SENT)
  {
    uint8 tech = 0;
    boolean wait_in_rfsw = TRUE;

    //Immediate request sent by MCPM. RF waits for 650us if not other techs are ON
    for (tech=0; tech<RFM_NUM_MODES; tech++) 
    {
       if (tech != mode) 
       {
          if (last_rf_wan_gps_npa_id[tech] != PMIC_NPA_MODE_ID_RF_MODE_0) 
          {
             wait_in_rfsw = FALSE;
             break;
          }
       }
    }

    if (wait_in_rfsw == TRUE) 
    {
       MSG_4(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Wait in RF SW for Immediate Req - new tech=%d, band=%d, "
                                      "device_num=%d, rf state=%d", mode, vreg_param.get_band(), vreg_param.get_device(), 
                                       vreg_param.get_rf_state());
       DALSYS_BusyWait(vreg_por_wait);
    }
    else
    {
       MSG_5(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: No Wait in RF SW for Immediate Req - old tech=%d, new tech=%d, "
                                         "band=%d, device_num=%d, rf state=%d", tech,mode,vreg_param.get_band(), 
                                          vreg_param.get_device(), vreg_param.get_rf_state());
    }
  }
  else if (mcpmnpa_status == MCPMNPA_STAT_OPT_SUCCESS)
  {
    MSG_5(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Scheduled NPA request made for RF PAM id=%d, mode=%d, band=%d,"
                                      "device_num=%d,rf state=%d", npa_id, vreg_param.get_mode(), 
                                      vreg_param.get_band(), vreg_param.get_device(), vreg_param.get_rf_state());
  }
    
  last_rf_wan_gps_npa_id[mode] = npa_id;
    
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::send_npa_rf_gps_id(rfcom_mode_enum_type mode, uint8 npa_id)
{
  if (rfc_sv_npa_voting == FALSE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: No NPA voting from RF. Rails need to be turned on in PMIC Init",0);
    /* no NPA support on RUMI */
    return;
  }

  // GPS_MODE_9 enum is no longer defined in pmapp_npa.h
  if (npa_id >= PMIC_NPA_MODE_ID_GENERIC_MAX) 
  {
    MSG_ERROR("Invalid WAN NPA id in rf_pwr_mgr::send_npa_gps_id() %d",npa_id,0,0);
    return;
  }

  if (npa_id == last_rf_wan_gps_npa_id[mode]) //same NPA id as the last one
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Same GPS NPA id as last sent - saved id=%d, new id=%d",
          last_rf_wan_gps_npa_id[mode],npa_id);
    return;
  }

    // Send NPA id only if different from last one
  npa_issue_required_request (rf_wan_gps_npa_handle[mode], npa_id);
  if (npa_id != PMIC_NPA_MODE_ID_GENERIC_OFF)
  {
    DALSYS_BusyWait(vreg_por_wait);
  }
  print_npa_rf_gps_id_msg(npa_id);
  last_rf_wan_gps_npa_id[mode] = npa_id;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::send_sleep_npa_id(rfc_vreg_param &vreg_param)
{
  uint8 band = 0;
  rfm_mode_enum_type mode = RFM_INVALID_MODE;

  if (rfc_sv_npa_voting == FALSE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: No NPA voting from RF. Rails need to be turned on in PMIC Init",0);
    /* no NPA support on RUMI */
    return;
  }

  band = vreg_param.get_band();
  mode = vreg_param.get_mode();

#ifdef FEATURE_RF_HAS_NPA_SLEEP_ENABLED
  boolean mcpmnpa_status = MCPMNPA_STAT_NULL;
  boolean send_sleep_pam = FALSE;

  if ( rfc_state_info[vreg_param.get_mode()] == RF_PATH_SLEEP_STATE) {
     send_sleep_pam = TRUE;
  }

  MSG_1(MSG_SSID_RF, MSG_LEGACY_MED, "send_sleep_npa_id: send_sleep_pam=%d", send_sleep_pam );

  if ((send_sleep_pam == TRUE) && (PMIC_NPA_MODE_ID_RF_MODE_0 != last_rf_wan_gps_npa_id[vreg_param.get_mode()]))
  {
    // RF talks to MCPM instead of NPA
    mcpmnpa_status = MCPM_Send_RF_Req(vreg_param.get_mode(), PMIC_NPA_MODE_ID_RF_MODE_0, 
                                      rf_wan_gps_npa_handle[vreg_param.get_mode()], 0, TRUE);

    if ((mcpmnpa_status != MCPMNPA_STAT_OPT_SUCCESS) && (mcpmnpa_status != MCPMNPA_STAT_OPT_IMM_REQ_SENT))
    {
       npa_issue_required_request (rf_wan_gps_npa_handle[vreg_param.get_mode()], PMIC_NPA_MODE_ID_RF_MODE_0);
       MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: sleep PAM sent for mode %d", vreg_param.get_mode(),0);
    }
    else
    {
      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: sleep PAM sent by MCPM for path %d", vreg_param.get_path(),0);
    }

    last_rf_wan_gps_npa_id[vreg_param.get_mode()] = PMIC_NPA_MODE_ID_RF_MODE_0;
    //save_mode_info(vreg_param);

   boolean shut_off_dac_mpp = TRUE;
   uint8 tech = 0;

   for (tech = RFM_PARKED_MODE; tech < RFM_NUM_MODES; tech++)
   {
      if (last_rf_wan_gps_npa_id[tech] != PMIC_NPA_MODE_ID_RF_MODE_0)
      {
         shut_off_dac_mpp = FALSE;
         MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: Tech %d not in sleep, keep DAC MPP on", tech,0);
         break;
      }
   }
   if (shut_off_dac_mpp ==  TRUE)
   {
      npa_issue_required_request ( tx_dac_msm_vref_client_handle, PMIC_NPA_MODE_ID_RF_MODE_0 );
      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: All techs in sleep, shut off DAC MPP", 0, 0);
   }

    print_npa_rf_id_msg(PMIC_NPA_MODE_ID_RF_MODE_0);
  }
  else
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR: sleep PAM not sent for mode %d", vreg_param.get_mode(),0);
  }
#endif
}


/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::save_mode_info(rfc_vreg_param &vreg_param)
{
      rfm_path_state state = vreg_param.get_rf_state();
      //MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR save device info: current rfc_state_info=%d", rfc_state_info[vreg_param.get_mode()]);
     if (state == RF_PATH_RX_STATE || state == RF_PATH_RXTX_STATE) 
     {
         rfc_state_info[vreg_param.get_mode()] |= 1<<(vreg_param.get_device());
      }

      if ((state == RF_PATH_SLEEP_STATE) || (state == RF_PATH_TX_SLEEP_STATE)) 
      {
         rfc_state_info[vreg_param.get_mode()] &= (~(1<<(vreg_param.get_device())));
      }

      MSG_4(MSG_SSID_RF, MSG_LEGACY_MED,"RF_PWR_MGR save device info: mode=%d, device=%d, state=%d, rfc_state_info=%x", 
            vreg_param.get_mode(), vreg_param.get_device(), vreg_param.get_rf_state(), rfc_state_info[vreg_param.get_mode()] );
					   
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::init_rf_npa_handle(void)
{
  if (rfc_sv_npa_voting == FALSE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"init_rf_npa_handle: No NPA handles requested from RF. Rails need to be turned on "
                                      "in PMIC Init",0);
    /* no NPA support on RUMI */
    return;
  }

    /* Use RF1 PAM tables for all the WTR2955 RFCs on JO/TA */
    /* Make sure to use RF2 PAM tables for all the WTR4905 RFCs on JO/TA by adding a seperate condition*/
    //One time set up a required sync client to have Sleep Set formed in the RPM
    rf_sleep_set_sv_client_handle = npa_create_sync_client( PMIC_NPA_GROUP_ID_RF1_TECH_1X, 
                                                            "rf_sv_sleep_config_client", NPA_CLIENT_REQUIRED);
    if (rf_sleep_set_sv_client_handle == NULL)
    {
      ERR_FATAL( "init_rf_npa_handle:NULL client handle not created for rf_sleep_set_sv_client_handle ",0, 0, 0);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"init_rf_npa_handle: rf_handle for rf_sleep_set_sv_client_handle has been created!!",0);
      npa_issue_required_request(rf_sleep_set_sv_client_handle, PMIC_NPA_MODE_ID_RF_MODE_0);// Requesting For particular mode settingds (0 - off, 1-Low power on, 2 - Active)
    }

    /* Commenting creation of this client as it is duplicate */

    /* Get NPA client for VoLTE light sleep/wakeup PAM table */
    rf_light_sleep_lte_client_handle = npa_create_sync_client( PMIC_NPA_GROUP_ID_RF1_TECH_1X, 
                                                              "rf_light_sleep_lte_config_client", NPA_CLIENT_REQUIRED);

    if (rf_light_sleep_lte_client_handle == NULL)
    {
      ERR_FATAL( "NULL client handle not created for rf_light_sleep_lte_client_handle ",0, 0, 0);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_MED," rf_light_sleep_lte_client_handle has been created!!",0);
    }

    //Enable WTR3925/2965 using RF_1 PAM tables
    rf_wan_gps_npa_handle[RFM_1X_MODE] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_1x", NPA_CLIENT_SUPPRESSIBLE);
    rf_wan_gps_npa_handle[RFM_IMT_MODE] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_wcdma", NPA_CLIENT_SUPPRESSIBLE);
    rf_wan_gps_npa_handle[RFM_IMT_MODE_2] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_wcdma_2", NPA_CLIENT_SUPPRESSIBLE);
    rf_wan_gps_npa_handle[RFM_1XEVDO_MODE] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_evdo", NPA_CLIENT_SUPPRESSIBLE);
    rf_wan_gps_npa_handle[RFM_LTE_MODE] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_lte", NPA_CLIENT_SUPPRESSIBLE);
	rf_wan_gps_npa_handle[RFM_LTE_MODE_2] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_lte_2", NPA_CLIENT_SUPPRESSIBLE);
    rf_wan_gps_npa_handle[RFM_TDSCDMA_MODE] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_tdscdma", NPA_CLIENT_SUPPRESSIBLE);
    rf_wan_gps_npa_handle[RFM_EGSM_MODE] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_gsm1", NPA_CLIENT_SUPPRESSIBLE);
    rf_wan_gps_npa_handle[RFM_EGSM_MODE_2] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_gsm2", NPA_CLIENT_SUPPRESSIBLE);
    rf_wan_gps_npa_handle[RFM_EGSM_MODE_3] = npa_create_schedulable_client(PMIC_NPA_GROUP_ID_RF1_TECH_1X, "rf_pwr_mgr_gsm3", NPA_CLIENT_SUPPRESSIBLE);

   //This is for Kamorta/Aggati
	tx_dac_msm_vref_client_handle =  npa_create_sync_client( PMIC_NPA_GROUP_ID_RF1_TECH_VREF_MSM, "tx_dac_msm_vref_config_client", NPA_CLIENT_REQUIRED );  // DAC ref signal

	if (tx_dac_msm_vref_client_handle == NULL)
    {
	  ERR_FATAL( " init_rf_npa_handle: NULL client handle not created for tx_dac_msm_vref_client_handle ",0, 0, 0 );
    }

    MSG(MSG_SSID_RF, MSG_LEGACY_MED," init_rf_npa_handle: rf_handle for tx_dac_msm_vref_client_handle has been created!!");

  rf_wan_gps_npa_handle[RFM_1X_MODE]->resource_data = &tech_resource_data[RFM_1X_MODE];
  rf_wan_gps_npa_handle[RFM_IMT_MODE]->resource_data = &tech_resource_data[RFM_IMT_MODE];
 
  rf_wan_gps_npa_handle[RFM_IMT_MODE_2]->resource_data = &tech_resource_data[RFM_IMT_MODE_2];

  rf_wan_gps_npa_handle[RFM_1XEVDO_MODE]->resource_data = &tech_resource_data[RFM_1XEVDO_MODE];
  rf_wan_gps_npa_handle[RFM_LTE_MODE]->resource_data = &tech_resource_data[RFM_LTE_MODE];
  rf_wan_gps_npa_handle[RFM_LTE_MODE_2]->resource_data = &tech_resource_data[RFM_LTE_MODE_2];
  rf_wan_gps_npa_handle[RFM_TDSCDMA_MODE]->resource_data = &tech_resource_data[RFM_TDSCDMA_MODE];
  rf_wan_gps_npa_handle[RFM_EGSM_MODE]->resource_data = &tech_resource_data[RFM_EGSM_MODE];
  rf_wan_gps_npa_handle[RFM_EGSM_MODE_2]->resource_data = &tech_resource_data[RFM_EGSM_MODE_2];
  rf_wan_gps_npa_handle[RFM_EGSM_MODE_3]->resource_data = &tech_resource_data[RFM_EGSM_MODE_3];

  if (rf_wan_gps_npa_handle[RFM_IMT_MODE] == NULL || 
      rf_wan_gps_npa_handle[RFM_IMT_MODE_2] == NULL || 
      rf_wan_gps_npa_handle[RFM_1X_MODE] == NULL  || 
      rf_wan_gps_npa_handle[RFM_1XEVDO_MODE] == NULL ||
      rf_wan_gps_npa_handle[RFM_LTE_MODE] == NULL ||
      rf_wan_gps_npa_handle[RFM_LTE_MODE_2] == NULL ||
      rf_wan_gps_npa_handle[RFM_TDSCDMA_MODE] == NULL || 
      rf_wan_gps_npa_handle[RFM_EGSM_MODE] == NULL   || 
      rf_wan_gps_npa_handle[RFM_EGSM_MODE_2] == NULL ||
      rf_wan_gps_npa_handle[RFM_EGSM_MODE_3] == NULL )
  {
    ERR_FATAL( "init_rf_npa_handle: NULL client handle created for rf_pwr_mgr_wan ",0, 0, 0);
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"init_rf_npa_handle: rf_handle for rf_pwr_mgr_wan has been created!!",0);
  }
  
  #ifdef FEATURE_RF_POWER_REPORTING
  if ( (ftm_get_mode() != FTM_MODE) && (rfm_get_calibration_state() != TRUE) ) 
  {
     rfc_onlinemode = 1;
  }
  else
  {
     rfc_onlinemode = 0;
  }

  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  MSG_1(MSG_SSID_RF, MSG_LEGACY_MED," online mode %d ",  rfc_onlinemode);
  #endif

  if (rfc_onlinemode) 
  {
  init_power_reporting_timer();
  }
  #endif
} 

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::init_chip_info(void)
{
   uint8 tech;
   for (tech = RFM_PARKED_MODE; tech < RFM_NUM_MODES; tech++ )
   {
      rfc_state_info[tech] = RF_PATH_SLEEP_STATE;     
   }
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::init_npa_rxtx_table(void)
{

  uint8 vol_scale = 0;
  uint8 tech = 0;

  /* initialize NPA ID for all techs and all voltage scaling  */
  for (tech = RFM_PARKED_MODE; tech < RFM_NUM_MODES; tech++ )
  {
    for (vol_scale = RFC_VOLTAGE_SCALE_ON; vol_scale < RFC_VOLTAGE_SCALE_MAX; vol_scale++ )
    {
       if (tech != RFM_GNSS_MODE) 
       {
         npa_rxtx_table[tech][vol_scale] = PMIC_NPA_MODE_ID_RF_MODE_0;
       }
       else
       {
         npa_rxtx_table[tech][vol_scale] = PMIC_NPA_MODE_ID_GENERIC_OFF;
       }
    }
  }

  for (tech = RFM_PARKED_MODE; tech < RFM_NUM_MODES; tech++ )
  {
     if (tech != RFM_GNSS_MODE) {
       npa_rxtx_table[tech][RFC_VOLTAGE_SCALE_ON] = PMIC_NPA_MODE_ID_RF_MODE_2;//PMIC_NPA_MODE_ID_GENERIC_ACTIVE --2
       npa_rxtx_table[tech][RFC_VOLTAGE_SCALE_OFF] = PMIC_NPA_MODE_ID_RF_MODE_0;//PMIC_NPA_MODE_ID_GENERIC_Sleep/OFF --0
     }
     else
     {
        npa_rxtx_table[tech][RFC_VOLTAGE_SCALE_ON] = PMIC_NPA_MODE_ID_GENERIC_STANDBY;
        npa_rxtx_table[tech][RFC_VOLTAGE_SCALE_OFF] = PMIC_NPA_MODE_ID_GENERIC_ACTIVE;
     }
  }

}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::print_npa_rf_id_msg(uint8 npa_id)
{
  switch(npa_id)
  {
    case PMIC_NPA_MODE_ID_RF_MODE_0:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"Issuing command to NPA with mode:PMIC_NPA_MODE_ID_RF_MODE_0",0,0);
      break;

    case PMIC_NPA_MODE_ID_RF_MODE_1:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"Issuing command to NPA with mode:PMIC_NPA_MODE_ID_RF_MODE_1",0,0);
      break;

    case PMIC_NPA_MODE_ID_RF_MODE_2:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"Issuing command to NPA with mode:PMIC_NPA_MODE_ID_RF_MODE_2",0,0);
      break;

    default:
      MSG_ERROR("Invalid NPA id being sent %d", npa_id,0,0);
      break;
  }}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
void rfc_vreg_mgr_wtr1605_sv::print_npa_rf_gps_id_msg(uint8 npa_id)
{
  switch(npa_id)
  {
    case PMIC_NPA_MODE_ID_GENERIC_OFF:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"Issuing command to NPA with mode:PMIC_NPA_MODE_ID_GENERIC_OFF",0,0);
      break;

    case PMIC_NPA_MODE_ID_GENERIC_STANDBY:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"Issuing command to NPA with mode:PMIC_NPA_MODE_ID_GENERIC_STANDBY",0,0);
      break;

    case PMIC_NPA_MODE_ID_GENERIC_ACTIVE:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"Issuing command to NPA with mode:PMIC_NPA_MODE_ID_GENERIC_ACTIVE",0,0);
      break;

    default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid NPA id being sent %d", npa_id);
      break;
  }
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   Checks if the rf is asleep.
 
   @details
   Checks if the rf is asleep by looping through all rfm modes to check if they are in sleep.
 
   @return
   boolean TRUE for sleep and FALSE for wakeup
*/
boolean rfc_vreg_mgr_wtr1605_sv::rf_vreg_is_asleep()
{
  uint8 tech = RFM_PARKED_MODE;

  for (tech = RFM_PARKED_MODE; tech < RFM_NUM_MODES;tech++ )
  {

    if (last_rf_wan_gps_npa_id[tech] != PMIC_NPA_MODE_ID_RF_MODE_0)
    {
      return FALSE;
    }

  }

  return TRUE;

}


/* ----------------------------------------------------------------------- */
/*!
/*!
   @brief
   shim layer API(c function)to check if the rf is asleep.
 
   @details
   shim layer API(c function)to check if rf is asleep by calling the api rf_vreg_is_asleep() from the vreg_mgr
 
   @return
   boolean TRUE for sleep and FALSE for null pointer/wakeup
*/
boolean rf_is_asleep()
{

  rfc_vreg_mgr_wtr1605_sv* rfc_vreg_mgr =(rfc_vreg_mgr_wtr1605_sv*)rfc_vreg_mgr_wtr1605_sv::get_instance();

  /*NULL pointer check*/
  if (rfc_vreg_mgr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR,"rf_is_asleep()- rf_pwr_mgr::get_instance() returned NULL ");
    return FALSE;
  }

  return rfc_vreg_mgr->rf_vreg_is_asleep();

}
