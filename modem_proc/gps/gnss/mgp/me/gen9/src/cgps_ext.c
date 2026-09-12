/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          C-GPS External Source file

GENERAL DESCRIPTION
  This module contains the interface functions exported outside the library

EXTERNALIZED FUNCTIONS
  cgpd_XoSupported


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2007 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2013 - 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

  $Header: //components/rel/gnss8.mpss/11.1/gnss/mgp/me/gen9/src/cgps_ext.c#4 $
  $DateTime: 2021/04/12 04:33:58 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/23/09   jd      Use new GNSS RF APIs.
06/17/09   jd      Added FEATURE_TRM_API_V3 - GPS support for new TRM APIs 
                   done for SVDO and PAM
09/20/07   va      Create wrapper for clk_regime_cgps_freq_offset_ppt
09/07/07   va      Initial implementation.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*
 * Include files
*/
#include "aries_os_api.h"
#include "gps_variation.h"
#include "cgps_api.h"
#include "gnss_api.h"
#include "pmapp_npa.h"
#ifdef FEATURE_XO_TASK
#include "tcxomgr_task.h"
#endif /* FEATURE_XO_TASK */
#if defined(FEATURE_BATTERY_CHARGER)
#include "charger.h"
#endif

/*
 * Constant definitions
*/

/*
 * Structure definitions
*/

/*
 * Local data declarations.
*/


/*
 * Local function prototypes
*/

/*=============================================================================

       Variables

=============================================================================*/



/*
sizeof(meu_SigProcMem) = sizeof(cc_ResMgrExtDirectModeMemLayoutType) +  
                         sizeof(cc_ResMgrExtInDirectModeMemLayoutType) + 
                         sizeof(cc_ResMgrBitEdgeMemLayout)+
                         sizeof(cc_ResMgrExtDPOMemLayoutType ) +
                         CC_RESMGR_TOTAL_EMEM_CMD_MEMORY_SIZE; 
+ GAL 80 MS PDI additional size 

                  //cc_resmgr.c/cc_ResMgrInitialize()
Number of Correlation buffers is defined by Number of GLONASS tasks - when direct mode is being used
otherwise, correlation buffers and energygrid is defined by number of max tasks only.

*/

#ifdef FEATURE_GNSS_LEAN_BUILD_DIRECT_MODE
  #ifndef FEATURE_GNSS_KAMORTA_HW
  	 /*for 100 tasks */
  	 /*Extra 544 added for dummy buffers[260bytes*2buffers] and 2 DMAC commands for flushing BUP*/

  	 /* ----------------------------------------------
  	 ** GLOBAL STATIC BUFFER Breakdown:
  	 ** 1. ACQUISITION indirect tasks (Correlation Buffer and Energy Grid)
  	 ** 2. ACQUISITION direct tasks (Energy Grid only)
  	 ** 3. BIT EDGE tasks (Correlation Buffer and Energy Grid)
  	 ** 4. CC COMMAND BUFFER (for both CP and DP)
  	 ** 5. L1 80ms PDI TRACKING tasks (Correlation Buffer/Temp IQ sums and Energy Grid)
  	 ** ---------------------------------------------- */

  	 /* ----------------------------------------------
  	 ** 1a] Correlation buffer for indirect tasks: 16384 * 35
  	 ** ref => CC_RESMGR_LO_MEM_TASK_CORR_BUFF * C_GNSS_ME_NORMAL_MAX_INDIRECT_TASKS
  	 ** 1b/2] Block Float Energy Grid for indirect and direct tasks: 5200 * 100
  	 ** ref => CC_RESMGR_LO_MEM_TASK_ENERGY_GRID * C_ME_NORMAL_MAX_TASKS
  	 ** 3a] Correlation buffer for Bit edge tasks: 3328 * 10
  	 ** ref => CC_RESMGR_BED_TASK_CORR_BUFF * CC_MAX_BIT_EDGE_RESOURCES_STATIC_ALLOCATED
  	 ** 3b] Block Float Energy Grid for Bit edge tasks: 7680 * 10
  	 ** ref => CC_RESMGR_BED_TASK_ENERGY_GRID * CC_MAX_BIT_EDGE_RESOURCES_STATIC_ALLOCATED
  	 ** 4] CC command buffer: 213,384
  	 ** ref => CC_RESMGR_TOTAL_EMEM_CMD_MEMORY_SIZE
  	 ** 5a] Correlation buffer/Temp IQ sums for L1 80ms PDI track tasks: [92*24*2] * 14
  	 ** ref => CC_RESMGR_80MS_PDI_TASK_CORR_BUFF * CC_MAX_GAL_80MS_RESOURCES_ALLOWED
  	 ** 5b] Half Float Energy Grid for L1 80ms PDI track tasks: [40*24*2] * 14
  	 ** ref => CC_RESMGR_80MS_PDI_TASK_ENERGY_GRID * CC_MAX_GAL_80MS_RESOURCES_ALLOWED
  	 ** ---------------------------------------------- */
  	 /* ----- **** ----- **** ----- **** ----- **** ----- **** ----- **** ----- **** -----
  	 ** DO NOT, REPEAT, DO NOT CHANGE THE VALUE OF THE #define below without understanding
  	 ** and documenting CLEARLY ABOVE that REFLECTS THE CHANGE. #define components below 
  	 ** MUST MATCH the documentations ABOVE!!!!
  	 ** 
  	 ** ===     CODE REVIEWER!! DO NOT **APPROVE** IF THAT IS NOT THE CASE!!           ===
  	 ** --------------------------------------------------------------------------------- */
  	 #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_35_INDIRECT_65_DIRECT \
    	 ((16384*35) + (5200*100) + (3328*10) + (7680*10) + 213384 + (4416*14) + (1920*14))

  	 #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_100_TASKS  ((16384*25) + (5280*100) + (3328*10) + (7680*10) + (1536*55) + 152852 + 1024 + 544+(92*24*2*9+40*2*24*9+92*4*2*9))  
  	 //92 ms * 64 hyp for Pilot and 4 hyp for Data * 9 tasks * 2 bypte per Hyp. For IQ Sum 92*4 hyp (Data only)

  	 /* With 20 DMAC commands going to DSM, we need static mem for the remaining 30 DM tasks (200T mode). Above allocates only 25. Create space for 5 more */
  	 #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_5_DM_TASKS 16384*5

  	 /* For 200 tasks use the 100 task memory configuration. The remaining comes from DSM. Some additions at the end is to account for larger than expected structures */
  	 #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_200_TASKS C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_100_TASKS+C_GNSS_ME_SIG_PROC_STATIC_BUFFER_5_DM_TASKS+3000+3808+3928+80

  	 /* 100T mode has 20IM CB. 100T mode in VT has 50IM CB. Adjust for the extra 30 corr buffers */
  	 #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_150_TASKS C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_100_TASKS+6*C_GNSS_ME_SIG_PROC_STATIC_BUFFER_5_DM_TASKS+3000+3808+3928+80
  
  #else
     /************VTV4L
     Total number of Indirect tasks and Bit_Edge tasks supported is 140 and 10 respectively
     //refer to cc_resmgr.h
     
     Indirect task Corr Buff   = CC_RESMGR_LO_MEM_TASK_CORR_BUFF   (4096) = 64 * 2B * (20 + 12) = 4096B
     Indirect task Energy Buff = CC_RESMGR_LO_MEM_TASK_ENERGY_GRID (1360) = (64 * 20 * 1B exp) + (20 * 4B mant) = 1280 + 80 = 1360B

     Bitedge task Corr buff    = CC_RESMGR_BED_TASK_CORR_BUFF   (1792) = 8 code hyps * 2B * (100ms + 12ms) = 1792B
     Bitedge task Energy Buff  = CC_RESMGR_BED_TASK_ENERGY_GRID (3840) = (2*32*20*1B) + (32*20*4B) = 1280 + 2560 = 3840B

     GAL 80ms Corr Buff        = CC_RESMGR_80MS_PDI_TASK_CORR_BUFF (4416) = 92 ms buffering* 24 Hyp* 2B per Hyp = 4416B
     GAL 80ms Energy Buff      = CC_RESMGR_80MS_PDI_TASK_ENERGY_GRID (1920) = 40 Freq bin * 24 hyp * 2B per Hyp = 1920B

     TOTAL static memory allocated for 280 tasks = 1067584 bytes 
     *************/
  	 #define C_GNSS_ME_INDIRECT_CP_TASK_SIZE_BYTES  (4096) 
  	 #define C_GNSS_ME_INDIRECT_EG_TASK_SIZE_BYTES  (1360)
  	 #define C_GNSS_ME_BITEDGE_CP_TASK_SIZE_BYTES   (1792)
  	 #define C_GNSS_ME_BITEDGE_EG_TASK_SIZE_BYTES   (3840)
	 #define C_GNSS_ME_GAL_80MS_CORR_BUFF_SZ        (4416)
	 #define C_GNSS_ME_GAL_80MS_ENERGY_BUFF_SZ      (1920)
	 
  	 #define C_GNSS_ME_NORMAL_MAX_TASKS             (280) 
  	 #define C_GNSS_ME_CC_NORMAL_MAX_INDIRECT_TASKS (140) 
  	 #define C_GNSS_ME_CC_GEN9_BITEDGE_TASKS        (10)
	 #define C_GNSS_ME_GEN9_GAL_80MS_TRACK_TASKS    (9)
	
  	 #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_280_TASKS ((C_GNSS_ME_INDIRECT_CP_TASK_SIZE_BYTES*C_GNSS_ME_CC_NORMAL_MAX_INDIRECT_TASKS) + \
  														      (C_GNSS_ME_INDIRECT_EG_TASK_SIZE_BYTES*C_GNSS_ME_NORMAL_MAX_TASKS) + \
  														      (C_GNSS_ME_BITEDGE_CP_TASK_SIZE_BYTES*C_GNSS_ME_CC_GEN9_BITEDGE_TASKS) + \
  														      (C_GNSS_ME_BITEDGE_EG_TASK_SIZE_BYTES*C_GNSS_ME_CC_GEN9_BITEDGE_TASKS) + \
  														      (C_GNSS_ME_GAL_80MS_CORR_BUFF_SZ*C_GNSS_ME_GEN9_GAL_80MS_TRACK_TASKS) + \
  														      (C_GNSS_ME_GAL_80MS_ENERGY_BUFF_SZ*C_GNSS_ME_GEN9_GAL_80MS_TRACK_TASKS))

	 /************
	 The DM command memory need for storing the CP/DP, Periodic read, etc of 400  tasks.
	 CP Commands memory includes the following:
	   1) CP DM commands storage.  sizeof(cc_DmCmdWrapperType)*2 = 	[8302(max cmds) *3(words) *4(bytes/word)]* 2(cmd buffers) = 99624*2 = 199248 B
	   2) RCTC Commands storage.   CC_RESMGR_RCTC_DM_BUFFER_LEN*2 = [21(max cmds) *3(words) *4(bytes/word)]* 2(cmd buffers)   = 252*2   = 504 B
	   3) PdRd register storage.   sizeof(cc_PeriodicReadEmemDataType)*2 = [128(max registers)* 4(bytes/reg) *20(period in msec)]* 2(cmd buffers) = 10240*2 = 20480 B
	   4) CP Status registers.	   sizeof(cc_CpStatusRegisterWrapperType)*2 = NA
	   5) CP DM Pointer storage    sizeof(cc_DmCmdPtrWrapperType)*2 = NA
       6) Wait RTC storage.        sizeof(cc_WaitRTCRegisterWrapperType)*2 = NA
       7) Dummy cycle programming  sizeof(cc_RCTCBufferType)*2 = [65(max words)* 4(bytes/word)]* 2(cmd buffers) = 520 B
       8) Alignment padding                                    = 64*6*2 = 768B
			 TOTAL for CP DM = 221520 bytes
	 DP Commands memory includes the following: 		 
	   1) DP DM commands storage sizeof(cc_DpCmdType)*2 = [(806(cgm dm cmds) + 2027(egm dm cmds))*3(words) *4(bytes/word) +272(fudge)]* 2(cmd buffers) = 34268*2 =  68536 B
	   2) DP output data storage sizeof(cc_DpTaskBufferOutType)*2 = [403(tasks)* (128bytes/task)]* 2(cmd buffers) = 103168 B
	   		 TOTAL DP DM = 171704 bytes
  
	 The C_GNSS_ME_SIG_PROC_CP_DP_DM_CMD_400_TASK_SIZE (393224 bytes) value should be same as CC_RESMGR_TOTAL_EMEM_CMD_MEMORY_SIZE in the CC code.
	 ************/

     #define C_GNSS_ME_SIG_PROC_CP_DP_DM_CMD_400_TASK_SIZE  (393224) 
  
     #define C_GNSS_ME_SIG_MEM_PLACEHOLDER  (8*1024)
     #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_400_TASKS  (C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_280_TASKS +\
															   C_GNSS_ME_SIG_PROC_CP_DP_DM_CMD_400_TASK_SIZE + \
															   C_GNSS_ME_SIG_MEM_PLACEHOLDER)

  #endif //FEATURE_GNSS_KAMORTA_HW
  
#else

  /* for 100 tasks */ // one extra energy grid is declared since data structrures are static
  #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_100_TASKS  ((16384*100) + (5280*101) + (3328*10) + (7680*10) + (1536*55) + 152852 + 1024 + 544 + ((92*64*2)*9+(80*64+80*4*2)*9)+(92*24*2*9+40*24*2*9+92*4*2*9))  //92ms*64 hyp*2bit for CPRM, and for EGM

  /* for 200 tasks */  // one extra energy grid is declared since data structrures are static
                       //  #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_200_TASKS  ((16384*200) + (5280*201) + (3328*10) + (7680*10) + (1536*55) + 152852 + 1024)+2600+544

  /*for 200 tasks use the 100 task memory. The remaining comes from DSM. Some additions at the end is to account for larger than expected structures */
  #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_200_TASKS  C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_100_TASKS+2600+544

#endif

#ifdef FEATURE_GNSS_VT
  #ifdef FEATURE_GNSS_KAMORTA_HW
  	 #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE  C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_400_TASKS
  #else
  	 #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE  C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_35_INDIRECT_65_DIRECT
	 //#define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE  C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_150_TASKS
  #endif 
#else /* Default is HT target */
  #define C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE_200_TASKS
#endif


#ifdef FEATURE_GNSS_DYNAMIC_SP_MEM_ALLOCATION

  uint8 meu_SigProcMem[128];
  boolean mcu_HiMemFeature = FALSE;

#else

  uint8 meu_SigProcMem[C_GNSS_ME_SIG_PROC_STATIC_BUFFER_SIZE]__attribute__((aligned(64)));
  boolean mcu_HiMemFeature = FALSE;

#endif //FEATURE_GNSS_DYNAMIC_SP_MEM_ALLOCATION

/*****************************************************************************
 * API functions for C-GPS capabilities
 *****************************************************************************
*/

/*
 ******************************************************************************
 * Function cgps_XOSupported
 *
 * Description:
 *
 *  This function is a peek on C-GPS capability for XO Support
 *
 * Parameters:
 *
 *  None.
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  TRUE if XO is supported
 *  FALSE if XO is not supported and VCTCXO is supported.
 *
 ******************************************************************************
*/
boolean cgps_XoSupported(void)
{
#ifdef FEATURE_XO
  return TRUE;
#else
  return FALSE;
#endif
}

/*
 ******************************************************************************
 * Function cgps_ClkRegimeFreqOffsetPpt
 *
 * Description:
 *
 *  This function calls the appropriate clk fuction for QSC targets,
 *  empty function otherwise.
 *
 * Parameters:
 *
 *  l_Offset - Freq Offset in ppt ( parts per trillion )
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void cgps_ClkRegimeFreqOffsetPpt(int32 l_Offset)
{
}

/*
 ******************************************************************************
 * Function cgps_SigProcMemSize
 *
 * Description:
 *
 * This function performs check on the buffer size of the signal processing
 * memory meu_SigProcMem[].
 *
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  Size of the the buffer meu_SigProcMem.
 *
 ******************************************************************************
*/
uint32 cgps_SigProcMemSize(void)
{
  return sizeof(meu_SigProcMem);
}

#ifndef FEATURE_GNSS_TCXOMGR
/*
 ******************************************************************************
 * Function cgps_XoEstimationState
 *
 * Description:
 *
 *  This function starts or stops XO frequency estimation loop
 *
 * Parameters: 
 *
 *  u_Enable : TRUE  : set start frequency estimation sig
 *             FALSE : set stop frequenmcy estimation sig
 *
 * Dependencies:
 * 
 *  None
 *
 * Return value: 
 *
 *  None
 *
 ******************************************************************************
*/
void cgps_XoEstimationState(boolean u_Enable)
{
#ifdef FEATURE_XO_TASK
  if (cgps_XoSupported())
  {
    #ifdef FEATURE_MCS_TCXO_REFACTORED
    tcxomgr_set_sigs(u_Enable ? TCXOMGR_START_UPDATE_TIMER_SIG :
                     TCXOMGR_STOP_UPDATE_TIMER_SIG);
    #endif //FEATURE_MCS_TCXO_REFACTORED
  }
#endif /* FEATURE_XO_TASK */
}
#endif /* ! FEATURE_GNSS_TCXOMGR */

/*
 ******************************************************************************
 *
 * cgps_SetExternalChargerStatus( void )
 *
 * Function description:
 *  This function sets a static indication of the external charger status.
 *  The Charger status may be used by other functions to enable / disable functions
 *  (for example, the receiver may be configured to disable low power mode of operation
 *  when it is connected to charger to give better accuracy)
 *
 * Parameters: 
 *  e_ExtChargerStatus -- Indicates whether the receiver is connected to a charger
 * 
 * Return value: None
 *  
 *
 ******************************************************************************
*/

/* Static variable to store the external charger status */
static cgps_BatteryChargerStatusEnumType cgpse_ExtChargerStatus = CGPS_BATTERY_CHARGER_STATE_UNKNOWN;

void cgps_SetExternalChargerStatus(cgps_BatteryChargerStatusEnumType e_ExtChargerStatus)
{
  cgpse_ExtChargerStatus = e_ExtChargerStatus;
} /* end cgps_SetExternalChargerStatus() */

/*
 ******************************************************************************
 *
 * cgps_GetExternalChargerStatus( void )
 *
 * Function description:
 *  This function returns the value of external charger status.
 *
 * Parameters: 
 *  e_ExtChargerStatus -- Indicates whether the receiver is connected to a charger
 * 
 * Return value: None
 *  
 *
 ******************************************************************************
*/

static cgps_BatteryChargerStatusEnumType cgps_GetExternalChargerStatus(void)
{
  return (cgpse_ExtChargerStatus);
} /* end cgps_GetExternalChargerStatus() */

/*
 ******************************************************************************
 *
 * cgps_GetChargerStatus( void )
 *
 * Function description:
 *  This function obtains the Battery Charger status. It checks whether a charger
 *  is connected or not, and if the phone is still charging or not. 
 *  Possible return values are
 *  CGPS_BATTERY_CHARGER_STATE_UNKNOWN 
 *  CGPS_BATTERY_CHARGER_NOT_CONNECTED
 *  CGPS_BATTERY_CHARGER_CONNECTED_NOT_CHARGED
 *  CGPS_BATTERY_CHARGER_CONNECTED_CHARGED
 *
 * Parameters: 
 *  
 * Return value: cgps_BatteryChargerStatus
 *  
 *
 ******************************************************************************
*/
cgps_BatteryChargerStatusEnumType cgps_GetChargerStatus(void)
{
  cgps_BatteryChargerStatusEnumType e_Status = CGPS_BATTERY_CHARGER_STATE_UNKNOWN;

#if defined(FEATURE_BATTERY_CHARGER) || defined(FEATURE_CHG_DAL)
  if (chg_is_charging())
  {
    /* Batter Charger is connected. Now check it is is charging */
    if (CHG_UI_EVENT__DONE == chg_ui_event_read())
    {
      /* Charger is connected and charging is complete */
      e_Status = CGPS_BATTERY_CHARGER_CONNECTED_CHARGED;
    }
    else
    {
      /* Charger is connected, but not completely charged */
      e_Status = CGPS_BATTERY_CHARGER_CONNECTED_NOT_CHARGED;
    }
  }
  else
  {
    /* Charger is not connected */
    e_Status = CGPS_BATTERY_CHARGER_NOT_CONNECTED;
  }
#endif /* defined(FEATURE_BATTERY_CHARGER) || defined(FEATURE_CHG_DAL) */

  /* If internal charger status is unknown, use external charger status
     This status may be provided by application or other custom charging API's 
  */
  if (CGPS_BATTERY_CHARGER_STATE_UNKNOWN == e_Status)
  {
    /* Get the charger status from an external source */
    e_Status = cgps_GetExternalChargerStatus();
  }

  return e_Status;

} /* end cgps_GetChargerStatus() */

/*
******************************************************************************
* gnss_UpdateWwanTxAntennaStatus
*
* Function description:
*
*  gnss_UpdateWwanTxAntennaStatus is used to update the status of which
*  TX Antenna is being used for WWAN transmissions. This antenna selection
*  control is needed to estimate the impact of the WLAN and WWAN Tx on GPS.
*  Different antennas provide different levels of isolation from the GPS band.
*  Based on the antenna used, GNSS receiver selects appropriate thresholds
*  for cross-correlation, or IMD technique to mitigate the impact.
*
*  This function should be called at start of a GPS session, and whenever the 
*  antenna selection is changed. 
*  The function updates a GNSS global variable to set the Tx Antenna status
*  and returns immediately without performing any further processing.
*  Note: If the device has only a single antenna for WWAN Tx, this API should 
*        be called with "WWAN_TX_ANTENNA_MAIN" as the antenna type.
*
* Parameters:
*
*  e_TxAntUsed - Indicates which Tx Antenna is selected.
*
* Return value:
*
*  void
*
******************************************************************************
*/

gnss_TxAntennaEnumType gnsse_TxAntUsed = WWAN_TX_ANTENNA_MAIN;

void gnss_UpdateWwanTxAntennaStatus(gnss_TxAntennaEnumType e_TxAntUsed)
{
  /* Update the Tx Antenna used */
  gnsse_TxAntUsed = e_TxAntUsed;
}

/*
 ******************************************************************************
 *
 * gnss_SetExternalCtrlForGNSS
 *
 * Function description:
 *  This function provides an API by which customer can modify
 *  ELNA configuration.
 *
 * Parameters: 
 *  e_LnaCfg -- ELNA config that customer opts to use
 * 
 * Return value: None
 *  
 *
 ******************************************************************************
*/

/* Customer needs to change this variable in order to override NAVRF driver ELNA setting */
gnss_ExternalGNSSConfigStruct gnssz_ExtElnaCfg =
{ GNSS_RF_DEFAULT_CFG, C_GNSS_RF_ELNA_GPIO_NUM_DEFAULT,C_GNSS_RF_ELNA_GPIO_NUM_DEFAULT, NULL, C_NAVRF_NPA_RESOURCE_NAME_DEFAULT, C_PMIC_NPA_MODE_ID_RF_MODE_DEFAULT };

void gnss_SetExternalCtrlForGNSS(gnss_ExternalGNSSConfigStruct z_GnssCfg)
{
  gnssz_ExtElnaCfg.e_L1ExtElnaCtrl = z_GnssCfg.e_L1ExtElnaCtrl;
  gnssz_ExtElnaCfg.u_L1ElnaGpioNumber = z_GnssCfg.u_L1ElnaGpioNumber;
  gnssz_ExtElnaCfg.u_L5ElnaGpioNumber = z_GnssCfg.u_L5ElnaGpioNumber;
  gnssz_ExtElnaCfg.p_ExternalLnaFunc = z_GnssCfg.p_ExternalLnaFunc;
  (void)GNSS_STRLCPY(gnssz_ExtElnaCfg.gnss_ExternalNpaResourceName,
                     z_GnssCfg.gnss_ExternalNpaResourceName,
                     C_NAVRF_MAX_NPA_RESOURCE_NAME_LEN_DEFAULT);
  gnssz_ExtElnaCfg.gnss_ExternalNpaActiveState = z_GnssCfg.gnss_ExternalNpaActiveState;
}

/*
  ******************************************************************************
  *
  * gnss_GetExternalCtrlForGNSS
  *
  * Function description:
  *  This function is used to get ELNA config information. This will provide
  *  an interface by which driver knows if ELNA config information has been
  *  modified externally.
  *
  * Parameters:
  *   p_ElnaCfg
  *  
  * 
  * Return value:
  *   None
  *  
  *
  ******************************************************************************
*/

void gnss_GetExternalCtrlForGNSS(gnss_ExternalGNSSConfigStruct *p_GnssCfg)
{
  if (NULL != p_GnssCfg)
  {
    p_GnssCfg->e_L1ExtElnaCtrl = gnssz_ExtElnaCfg.e_L1ExtElnaCtrl;
    p_GnssCfg->u_L1ElnaGpioNumber = gnssz_ExtElnaCfg.u_L1ElnaGpioNumber;
    p_GnssCfg->u_L5ElnaGpioNumber = gnssz_ExtElnaCfg.u_L5ElnaGpioNumber;
    p_GnssCfg->p_ExternalLnaFunc = gnssz_ExtElnaCfg.p_ExternalLnaFunc;
    (void)GNSS_STRLCPY(p_GnssCfg->gnss_ExternalNpaResourceName,
                       gnssz_ExtElnaCfg.gnss_ExternalNpaResourceName,
                       C_NAVRF_MAX_NPA_RESOURCE_NAME_LEN_DEFAULT);

    p_GnssCfg->gnss_ExternalNpaActiveState = gnssz_ExtElnaCfg.gnss_ExternalNpaActiveState;
  }
}


