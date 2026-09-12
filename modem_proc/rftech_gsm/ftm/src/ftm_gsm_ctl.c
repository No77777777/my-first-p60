/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                           
                       F T M  G S M  C T L

GENERAL DESCRIPTION
  This is the FTM RF file which contains RF specific commands

Copyright (c) 2003 - 2017  by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_gsm.mpss/3.11/ftm/src/ftm_gsm_ctl.c#1 $ 
  $DateTime: 2021/03/09 05:10:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/10/17   bp      Fix to disable Tx in ftm_gsm_stop_geran()
09/24/15   ck      Fix and Enable ftm gsm fbrx selftest measurement
08/04/15   ck      Support GSM fbrx power meas
08/05/15   sc      Deprecate alternate path
07/31/15   hoh     adapt frame sweep cal to work with Rx/Tx device split architecture
07/30/15   tsr     Fix RX calibration on a chain where TX device is not supported
07/24/15   tsr     Fix Idle Frame processing in FTM
07/16/15   tsr     Added GSM Support for FTM Device manager 
07/07/15   tsr     Removed initialisation of ftm chains from gsm initialisation
07/02/15   ck      support band-to-band diversity control in RFGSM
07/06/15   zhw     Added support for FTM Device Manager
06/29/15   sg      SW changes to support device based BC config NV
06/26/15   sg      Fix crash in FTM GSM deact sequence
06/17/15   tsr     Set gl1_hw_init state to new state name to enable both RX and TX
06/17/15   tsr     Use new geran test mode api to obtain as id from ftm sub id
06/15/15   ck      Disable Drx specifically for NPT DTA case and GSM as a victim
09/06/15   piy     Using NV path instead of rfm device to retrieve BC config 
                   information in FTM mode
06/10/15   tsr     Added FTM and Non-Signalling Support on a chain if TX device is not supported
06/03/15   ck/zhw  NPT test. Disable Tx when being victim
05/11/15   tsr     Add split Rx/Tx support to Non-Signalling
04/23/15   zbz     Pass sub_id from chain vars thoughout tx cal callflow
04/21/15   zbz     Fix OFT error
04/20/15   zbz     Add support for tools to get correct NV container
04/16/15   sc      Enable RxD calibration based on diversity BC config NV
04/15/15   sc      Rely on chain vars for split Rx/Tx info throughout call-flow
04/14/15   sc      Split Rx/Tx device mapping geran API modifications
04/14/15   ck      add ftm command to return nv container 
04/08/15   tsr     Introduced the ftm gsm chains based framework for split RX/TX devices
03/02/15   ck      Remove ftm ccmgr cleanup during band/tech transition
02/27/15   sc      Ensure NULL NV error checking when processing Rx/Tx band
02/04/15   zbz     Fix GSM FTM concurrency manager cleanup
12/15/14   ck      Fix kw error
12/09/14   ck      add api for oeverwriting alt_path 
11/13/14   ck      add capability to disable/enable gsm diversity 
10/30/14   ck      store ftm mode id for hooking ftm concurrency manager for gsm diversity 
09/30/14   tsr     Fixed OFT compiler error
09/25/14   tsr     DR-DSDS: Add support to instruct GL1 to execute RX only HW init 
                   if device is not TX capable
11/19/13   cpm     Add EGSM support for FTM_PROGRAM_QFE_SCENARIO
09/09/14   tsr     DR-DSDS C2/C3 RX calibration support
08/14/14   tsr     Reenable FTM GSM RX Diversity for Multisim(Disabled from earlier checkin)
08/12/14   ec      Fix for DRx crash on DSDA targets DPM2
07/25/14   par     Waiting for a GSM NS exit FTM signal outside 
                   STOP_MODE_REQ
06/30/14   ec      Mainline Rx Diversity
06/13/14   tsr     Reenable  GSM RX Diversity
05/22/14   tsr     Disable GSM RX Diversity Temporarily  
04/18/14   zbz     Fix FTM RF failure during handover
04/16/14   tsr     Add support for GSM RX Diversity (GL1 Dependant)
03/26/14   sb      Add support for IP2 cal for Dime PM
02/26/14   sc      Pass core data through cmd processing and mc layer
02/17/14   sc      Clean up mDSP APIs
07/01/14   sw      Move the calls to the geran test mode api into rftech_gsm
09/09/13   sb      Added hooks for exit GSM mode in FTM
09/04/13   ck      Update ftm_current_rf_mode when entering GSM mode to fix enh therm reading
08/21/13   tws     Replace FEATURE_TRITON_MODEM with FEATURE_DUAL_SIM
07/24/13   ec	     Pass FTM task ID into MC layer
05/23/13   ck      Fix corrupted first IQ samples
05/07/13   ck      Add waiting for the signal to ensure GSM finish stopping
04/15/13   ec      Featurise geran_test_mode apis to use as_id only for Triton 
04/12/12   ec	     Use as_id in calls to geran_test_mode_api
04/03/13   ec	     Pass rfm_dev to NS functions
04/02/13   aka     Support for band change in frame sweep functionality. 
03/28/13   aka     GSM frame sweep feature changes.
02/26/13   tws     Make the TX frequency offset device specific.
02/14/13   tws     The ftm gsm vars are now created in the dispatch handler.
02/06/13   tws     Call rfgsm_mc_set_band with the device set correctly.
02/04/13   tws     Update for geran_test_mode_api change.
01/2/13    aka     Changes to support multi chain GSM operation in FTM mode
01/18/13   tws     Set TX frequency offset is not device dependent.
01/08/13   tws     Pass as_id to mdsp intf api.
01/3/13    av      Changes to support chain2 for GSM
12/10/12   tws     Allocate FTM variables in FTM mode only.
12/05/12   tws     Use device2_as_id to get the correct ID for GL1. Mainline
                   the RF task context flag.
12/04/12   aka     Featurization changes for Dual active
12/03/12   tws     Move the FTM variable initialisation back to core.
11/30/12   tws     Remove AS_ID mapping until dependency is fixed.
11/27/12   tws     Make global variables device dependent.
11/23/12   tws     Remove dependecy on legacy GERAN API.
11/21/12   av      Added rfm_device parameters to mc/core/mdsp/msm layers.
11/13/12   ec	   Update to use new geran api for DSDA 
10/10/12   ec      Replace call to rfgsm_core_set_tx_pow with new ftm wrapper
09/25/12   tc      Mainline RF_HAS_RFA_GSM_TASK. 
08/20/12   sc      Add back gl1_hw_turn_tx_on as requirement in RF due to GL1 not calling
08/14/12   ec      Removed gl1_hw_turn_tx_on call in ftm_gsm_set_mode to avoid gfw error 
08/13/12   ec      Commented out RXFE status check in ftm_gsm_set_mode to unblock bring-up 
08/07/12   ec      Unstub GL1 calls for ftm_gsm_set_mode, make ftm_gsm_rf_call_in_rf_task_context
                   feature switched 
07/12/12   ec      Fix compilation error with CCS defined, using extern prototype for gl1_hw_rf_set_tx_band 
                   instead if #include gl1_hw_g.h 
07/06/12   ec      Use gl1_hw_rf_set_tx_band instead of rfm call
06/27/12   sb      Made changes to remove unused MDSP APIs to the grfc status  
05/11/12   jr      Changed the sequence of code within ftm_gsm_set_mode().
04/26/12   sb      Add FTM recovery from RXFE stuck  
03/23/12   sr      Removed the FEATURE_GSM_MDSP_QDSP6 featurization.
03/22/12   dej     Initializing variable req to zero to avoid storing junk values
03/17/12   jfc     Remove clkrgm* header files
03/05/12   zc      Added call to ftm_gsm_rx_disable(); in ftm_gsm_set_mode
02/28/12   sb      Added changes to support disabling Idle frame in FTM mode 
02/16/12   sb      Fixing ftm_gsm_set_mode leaking rex critical sections
01/16/12   sr      Made changes not todo idle_frame processing 
                   (temp comp and vbatt comp) during calibration.
01/11/12   sr      Added "ftm_task" include file to fix compiler error.
01/05/12   sr      geran api changes for non-signaling.
12/07/11   sb      Support for GSM IQ capture in Online and FTM mode  
10/19/11   sb      Apply therm read in idle frame and enable FTM temp comp and vbatt comp during idle frame
09/28/11   sb      Mainline FEATURE_RF_COMMON_LM_RFM_INTERFACE and FEATURE_RF_WCDMA_LM_RFM_INTERFACE. 
09/20/11   av      Call rfm_set_tx_band() during FTM. 
06/21/11   aro     Moved ftm_sys_control functionality to ftm_common_control
04/28/11   rsr     Move GSM set mode functionality to ftm_rfmode_enter.
04/25/11   aro     Renamed FTM RFMODE to FTM State
04/21/11   rsr     Fix to send LM buffer index to rfm_enable_tx. Temporary fix to allow files to compile. Need to talk to 
                   L1 and fix properly later.
03/24/11   aro     Renamed FTM system control filename
03/24/11   aro     Renamed FTM Enter Mode function 
03/16/11   sr      Removed un-used code.
03/11/11   sr      Removed un-used featurization
03/02/11   aro     Renamed FTM and FTM-NS mode enums
03/01/11   aro     Moved ftm_gsm_enter() to ftm_sys_ctl.c
02/15/11   aro     Added direct call to ftm_rfmode_set()
01/25/11   sr      removed the clk.h dependency
01/18/11   mkv     Support WCDMA/GSM to LTE FTM mode transition. 
12/20/10   pv      Updated srch_mdsp_disable() with ftm_mdsp_disable(), to avoid 1x 
                   FW running in other techs RF Calibration.
10/08/10   ggs     CR257912 fix pulled in from QSC6295:
                   Made changes to turn-off the cont_tx in tdma_isr instead of from FTM task
08/20/10   tsr     Remove INTLOCK/INTFREE code 
08/12/10   rsr     Removing reference to unused Headers for QDSP6 targets.
07/20/10   ggs     Merged changes from from /dev/mdm9x00_bringup branch
03/19/10   lcl     Pull in fixes from QSC6295
03/09/10   lcl     Removed GPS app from ftm_gsm_enter.
01/06/10   sar     Renamed ftm_task.h to rf_test_task_v.h.
12/28/09   sr      High/Medium lint errors fix
12/22/09   sr      changed the debug msg macro from legacy to new one.
12/02/09   sr      Removed the tx_band setting during FTM Mode command.
10/12/09   sr      Fixed warnings
09/17/09   sar     Removed deprecated file, adie.h for CMI conformance.
09/17/09   sar     Updated clk interface for CMI conformance.
07/06/09   sr      change the rfgsmcard.tx_band to rfcom_gsm_band type
07/04/09   sr      set the rfgsmcard.tx_band in the set_mode.
04/01/09   sr      include file cleanup.
06/30/08   dw      Unload 1x mdsp image through SRCH
02/28/08   rmd     Added ANT_SEL temporary fix for TRF6285.
02/22/08   avi     Added check for FTM_RF_DB_IDLE when entering mode GSM (caused
                   FW download issues on MM targets)
12/14/07    ad     Defined ftm_gsm_toggle_modes() for EDGE only support
12/14/07    ad     added call to ftm_rfmode_gsm_cal() at the top of 
                   ftm_gsm_set_mode()- fixes mode switching crash
12/14/07    ad     Fix for uninitialized pointer issue in log packet in ftm_tdma_isr
12/13/07   adk     Fixed compiler warning.
12/07/07   adk     Added function-call ftm_rfmode_gsm() in ftm_gsm_set_mode()
                   to let 1x know that the 1x mode has been exited.
11/06/07    ka     Ensure 1x fw is unloaded to prevent crash.
08/08/07   jfc     Mainlined T_MSM6275 feature
08/08/07   jfc     Mainlined T_MSM6280 feature
08/06/07   jfc     Featurized code for EDGE-only builds
07/25/07   ycl     Lint warning fix.
12/13/06   jfc     Add ftm_send_FTM_STOP_GSM_MODE_REQ()
12/08/06   jfc     Specify MDSP app when starting WL1 for FEATURE_APP_W2G
10/18/06   ycl     Fixed rfm_enable_tx call in ftm_set_mode.
10/17/06   ycl     Modifications for initial MSM7600 build.
03/28/06   rsr     Lint cleanup. 
03/06/06   rsr     Fixes to remove compiler warnings.
02/21/06   tws     Change WCDMA_1800 to BC3. Add support for BC4 and BC9.
01/31/06   et      fixed pre-existing compile issue w/ rfm_enable_tx and added
                   HDR featurization
12/13/05   jfc     Reset tags_rd_index and tags_wr_index during ftm_set_mode()
08/23/05   rsr/gfr Changes to support gl1 related (DTM) changes. 
06/07/05   rsr     Removed calls to gl1_hw_schedule_reset_events and replaced by 
           gl1_hw_schedule_frame_tick in ftm_tdma_isr, per graham's suggestion. 
05/04/05   rsr     Removed warnings.Cleanup
04/21/05   rsr     Added support for new NV items
02/09/05   rsr/lcl  Renamed FEATURE_POLAR_RF to FEATURE_POLAR
02/07/05   xw      Added another signal to firmware download per gl1 interface change. 
10/07/04   bhas    update ftm_gsm_wait is not used anymore
01/14/03   lcl     If tx_cal_sweep is on, give it higher priority in
                   ftm_tdma_isr.
08/18/03   lcl     Added support for 850 and 1900 bands.
12/15/03   wd/jwh  Changed calls of gl1_hw_init() to specify FTM_SIG_WAIT_SIG.
12/10/02   xw      Code clean up and renamed it to ftm_gsm_ctl.c
08-27-02   rjr     Created.
===========================================================================*/
/*===========================================================================
                              INCLUDES
===========================================================================*/

#include "rfa_variation.h"
#include "comdef.h"
#include "target.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftmicap.h"
#include "rex.h"

#ifdef FTM_HAS_UMTS

#ifdef FEATURE_GSM

#include "ftm.h"
#include "ftm_gsm_ctl.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_gsm.h"
#include "rf.h"
#include "diagi.h"
#include "ftm.h"
#include "rf_test_task_v.h"
#include "ftm_task.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_gsm_rfprot.h"
#include "ftm_gsm_prot.h"
#include "rfgsm_core_util.h"
#include "ftm_common_control.h"
#include "ftm_gsm_device.h"
#include "ftm_gsm_rfctl.h"
#ifdef FEATURE_GERAN_RF_SELF_TEST
  #include "ftm_msg.h"
  #include "rfcommon_time_profile.h"
  #include "rfdevice_trx_gsm_tx.h"
  #include "rflm_dtr_rx_typedef_ag.h"
  #include "rfdevice_gsm_intf.h"
  #include "rfc_card_gsm.h"
  #include "mdsp_intf.h"
  #include "ftm_common_selftest.h"
  #include "rfcommon_mdsp.h"
  #include "rfgsm_msm.h"
#endif
#ifdef FEATURE_WCDMA
 #include "assert.h" 
 #include "l1task.h"
#endif

#include "rfgsm_mc.h"
#include "rfgsm_nv_cmn.h"
#include "rfgsm_mdsp_cal.h"
/* For reference to SYS_BAND macros. */
#include "sys_type.h"
#include "ftm_log.h"

#include "gl1_hw.h"
#include "geran_test_mode_api.h"

#ifdef FEATURE_GSM_RFLM
#include "rxlm_intf.h"
#include "txlm_intf.h"
#endif

#include "rfcommon_nv_mm.h"
#include "ftm_common_concurrency_manager.h"
#include "ftm_device_manager.h"
#include "ftm_msg.h"

/*===========================================================================
             GLOBAL VARIABLES 
=============================================================================*/

//static uint32 ftm_gsm_txlm_buff_idx = MDSP_RXLM_BUF_IDX_INVALID;
/*===========================================================================
             EXTERNAL VARIABLES 
=============================================================================*/
/* To track the current RF mode */
extern ftm_rf_mode_type ftm_current_rf_mode; 


/* Variables to keep track of default AMSS values */
LOCAL int8  therm_recorded;
LOCAL int8  temp_comp_bin_index;
LOCAL int8  temp_comp_rem;
LOCAL int8  temp_comp_val;
LOCAL int16  vbatt_recorded;
LOCAL int16  vbatt_backoff;



/*Function in ftm_gsm.c to disable Rx*/
extern void ftm_gsm_rx_disable( rfm_device_enum_type rfm_dev );

/*@Todo..to remove this once geran_test_mode_api.h is updated with this api */
extern sys_modem_as_id_e_type geran_test_mode_api_map_rf_sub_id_to_as_id(uint8 rf_sub_id);


#define FTM_GSM_IDLE_FRAME_NUM 26

/* FTM GSM mutex */

rex_crit_sect_type ftm_gsm_lock;

static rfcom_device_enum_type ftm_gsm_current_tx_cal_dev = RFM_INVALID_DEVICE;

void ftm_mutex_init(void);

void ftm_set_gsm_rf_db_state(rfgsm_band_type band);

#ifdef FEATURE_GERAN_RF_SELF_TEST
static ftm_rf_fbrx_meas_tx_params_rsp_pkt_type ftm_rf_fbrx_meas_tx_params_rsp_pkt;
static ftm_rf_fbrx_iq_capture_trigger_rsp_pkt_type ftm_rf_fbrx_iq_capture_trigger_rsp_pkt;
static ftm_rf_fbrx_iq_capture_get_samples_rsp_pkt_type ftm_rf_fbrx_iq_capture_get_samples_rsp_pkt;
ftm_lm_buf_type ftm_gsm_fbrx_path_buffer = {-1, FALSE};
#endif

/*===========================================================================
          EXTERNAL FUNCTION PROTOTYPES
===========================================================================*/


/*===========================================================================
          PUBLIC FUNCTION IMPLEMENTATIONS
===========================================================================*/




/*===========================================================================

FUNCTION FTM_MODE_WAS_GSM

DESCRIPTION
    This function will determine if the current mode is GSM or not.

DEPENDENCIES

RETURN VALUE
   TRUE if the current mode is GSM.
   FALSE if the current mode is not GSM. 

SIDE EFFECTS
   None

===========================================================================*/
boolean ftm_mode_was_gsm(void)
{
  if((ftm_get_current_state(RFM_DEVICE_0) == FTM_STATE_GSM) ||
     (ftm_get_current_state(RFM_DEVICE_0) == FTM_STATE_GSM_NS))
 {
    return(TRUE);
 }
 else
    return(FALSE);

}

/*===========================================================================

FUNCTION ftm_gsm_ctl_map_sub_id_to_as_id

DESCRIPTION
  ftm gsm wrapper api to obtain geran as id from ftm sub id. 
 

DEPENDENCIES
  None

PARAMETERS
  FTM Sub id

RETURN VALUE
  sys_modem_as_id_e_type  as_id

SIDE EFFECTS
  None
===========================================================================*/
sys_modem_as_id_e_type ftm_gsm_ctl_map_sub_id_to_as_id(uint8 sub_id)
{
   /*Obtain as_id based on the ftm sub id from GL1*/
   return(geran_test_mode_api_map_rf_sub_id_to_as_id(sub_id));
}


/*===========================================================================

FUNCTION FTM_GSM_SET_MODE

DESCRIPTION
    This function will switch modes to EGSM or DCS based
    on the entry parameter.   

DEPENDENCIES
   
RETURN VALUE
   None.

SIDE EFFECTS
   None

===========================================================================*/
void ftm_gsm_set_mode( rfm_device_enum_type dispatch_device,
                       ftm_mode_id_type mode)
{
  boolean change_mode = FALSE;
  uint16  default_channel =1;
  rfgsm_band_type gsm_band = RFGSM_BAND_GSM900;
  sys_band_T gsm_sys_band = INVALID_BAND;
  rfgsm_core_data_type rfgsm_core_data;
  rfgsm_core_handle_type* rfgsm_core_handle_ptr = NULL;
  boolean tx_is_supported = FALSE;
  boolean band_change = FALSE; // detect band change to avoid skip of ftm ccmgr
  rfcom_band_type_u rfcom_band;
  ftm_concurrency_manager_params_type conmgr_params;
  uint64 rxd_dev_band_mask = 0;
  boolean  diversity_enabled = FALSE;

  ftm_gsm_chain_vars_s * chain_vars_ptr = NULL;
  ftm_gsm_sub_vars_s *   sub_vars_ptr   = NULL;
  ftm_gsm_cmn_vars_s* cmn_vars_ptr = NULL;

  /*Update the GSM chain variables by retrieving RX/TX/DRX devices from RFC*/
  chain_vars_ptr = ftm_gsm_device_get_and_update_chain_vars(dispatch_device);
  if( chain_vars_ptr == NULL )
  {
    ERR_FATAL( "ftm_gsm_set_mode() chain_vars_ptr is NULL for dispatched dev %d", 
               dispatch_device, 0, 0 );
    return;
  }
  
  cmn_vars_ptr = chain_vars_ptr->cmn_vars_ptr;
  if( cmn_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_set_mode() cmn_vars_ptr is NULL for dispatched dev %d", 
              dispatch_device, 0, 0 );
  }

  if( chain_vars_ptr->rx_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_set_mode() rx_vars_ptr 0x%x is NULL for remapped PRx dev %d", 
              chain_vars_ptr->prx_dev, 0, 0 );
    return;
  }

  tx_is_supported = ftm_gsm_device_is_tx_supported(chain_vars_ptr->associated_tx_dev);

  if (tx_is_supported) 
  {
    if( chain_vars_ptr->tx_vars_ptr == NULL )
    {
      ERR_FATAL("ftm_gsm_set_mode() tx_vars_ptr is NULL for Tx dev %d", 
                chain_vars_ptr->associated_tx_dev, 0, 0 );
      return;
    }
  }
  else
  {
    MSG_1( MSG_SSID_FTM, MSG_LEGACY_MED, 
           "ftm_gsm_set_mode() TX device is not supported for ftm chain", 
           chain_vars_ptr->gsm_chain);
  }

  /* Set the GSM mode i.e GSM850 */
  cmn_vars_ptr->mode = mode;

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(chain_vars_ptr->prx_dev);
  if ( rfgsm_core_handle_ptr == NULL )
  {
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "rfgsm_core_handle_ptr is NULL", chain_vars_ptr->prx_dev);
    return;
  }
  
  ftm_rfmode_enter( chain_vars_ptr->prx_dev, FTM_STATE_GSM );

  switch ( mode )
  {
  case FTM_PHONE_MODE_GSM_850:
    if ( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_CELL_850 )
    {
      change_mode = TRUE;
      gsm_band = RFGSM_BAND_GSM850;
      gsm_sys_band = SYS_BAND_CELL_850;
      default_channel = 128;
    }
    break;

  case FTM_PHONE_MODE_GSM_900:
    if ( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_EGSM_900 )
    {
      change_mode = TRUE;
      gsm_band = RFGSM_BAND_GSM900;
      gsm_sys_band = SYS_BAND_EGSM_900;
      default_channel = 1;
    }
    break;

  case FTM_PHONE_MODE_GSM_1800:
    if ( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_DCS_1800 )
    {
      change_mode = TRUE;
      gsm_band = RFGSM_BAND_GSM1800;
      gsm_sys_band = SYS_BAND_DCS_1800;
      default_channel = 512;
    }
    break;

  case FTM_PHONE_MODE_GSM_1900:
    if ( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_PCS_1900 )
    {
      change_mode = TRUE;      
      gsm_band = RFGSM_BAND_GSM1900;
      gsm_sys_band = SYS_BAND_PCS_1900;
      default_channel = 512;
    }
    break;

  case FTM_PHONE_MODE_SLEEP:

    MSG_1(MSG_SSID_FTM, MSG_LEGACY_HIGH, "ftm_gsm_set_mode, mode:%d", mode);
    ftm_rfmode_exit( chain_vars_ptr->prx_dev, FTM_STATE_PARK );

    break;

  default:
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR,"unsupported band %d",mode);
    change_mode = FALSE;
    break;
  }

  if ( change_mode == TRUE )
  {
    if( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_NONE)
    {
      /* clean up ftm ccmgr */ 
      ftm_concurrency_manager_cleanup_device_allocation( chain_vars_ptr->prx_dev, RFM_EGSM_MODE );

      /*Query BC config NV for DRX to retrieve the band mask information. Query only if the device has any band support in GSM (i.e. is a valid device for GSM) */
      if( rfgsm_core_get_device_rx_capability( chain_vars_ptr->associated_drx_dev ) )
      {
        rxd_dev_band_mask = rfcommon_nv_get_rx_device_bc_config( chain_vars_ptr->associated_drx_dev);
      }
      if (rxd_dev_band_mask & (SYS_BAND_MASK_GSM_EGSM_900 |
                               SYS_BAND_MASK_GSM_DCS_1800 |
                               SYS_BAND_MASK_GSM_850      |
                               SYS_BAND_MASK_GSM_PCS_1900) )
      {
        if(chain_vars_ptr->rx_vars_ptr->diversity_on)
        {
          diversity_enabled = TRUE;
        }
      }
      if (diversity_enabled)
      {
        if (chain_vars_ptr->associated_drx_dev < RFM_MAX_DEVICES)
        {
          ftm_concurrency_manager_cleanup_device_allocation(chain_vars_ptr->associated_drx_dev, RFM_EGSM_MODE);
        }
        else
        {
          MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "Invalid diversity device %d", chain_vars_ptr->associated_drx_dev);
        }
      }
       
      /* call concurrency manager to check valid concurrency */
      rfcom_band.gsm_band = ftm_gsm_convert_ftm_mode_id_type_to_rfcom(mode); 
      conmgr_params.band = rfcom_band;
      conmgr_params.carrier_index = 0;
      conmgr_params.prx_drx = RFC_PRX_PATH;
      conmgr_params.rfm_dev = chain_vars_ptr->prx_dev;
      conmgr_params.tech = RFM_EGSM_MODE;
      if(!ftm_concurrency_manager_validate_operation(&conmgr_params))
      {
        MSG_1( MSG_SSID_FTM, MSG_LEGACY_ERROR, "Invalid concurrency on dev %d", chain_vars_ptr->prx_dev );
        return;
      }
      
      /*check rxd is used or not*/    
      if(diversity_enabled)
      {
        conmgr_params.prx_drx = RFC_DRX_PATH;
        conmgr_params.rfm_dev = chain_vars_ptr->associated_drx_dev;
        if(!ftm_concurrency_manager_validate_operation(&conmgr_params))
        {        
          MSG_1( MSG_SSID_FTM, MSG_LEGACY_ERROR, "Invalid concurrency on dev %d", chain_vars_ptr->associated_drx_dev );
          return;
        }
      }    
    }

    sub_vars_ptr = ftm_gsm_get_sub_vars_ptr(chain_vars_ptr->sub_id);

    /* reset the frame counter for TDMA ISR initialize RX, TX and Synth */
    sub_vars_ptr->ftm_frame_count = 0;

    if (tx_is_supported) 
    {
      /* Change the power table index step size so that FTM has a finer
       * power resolution than normal operation.
      */
      (void)ftm_gsm_set_tx_pow_dbm( chain_vars_ptr->associated_tx_dev, 2000,0);

      /*If transmiter is on, turn it off before switching mode.  Note, we cannot have this
       * API call inside an INTLOCK because ftm_FNmod4 will not increase due to the INTLOCK.
       * Thus, the phone will hang if we are changing mode while TX is on.  Not sure why we
       * need INTLOCK.  Do keep this in mind in case there is problem.
       */
      if ( chain_vars_ptr->tx_vars_ptr->ftm_gsm_state.ftm_tx_state )
      {
        MSG_1( MSG_SSID_FTM, MSG_LEGACY_ERROR, "HAN_DBG ftm_gsm_set_mode %d", chain_vars_ptr->tx_vars_ptr->ftm_gsm_state.ftm_tx_state );
        ftm_gsm_set_tx( chain_vars_ptr->associated_tx_dev, OFF);
      }
    }

    /*Disable GSM Rx and wait 20ms to prevent race condition observed during test*/
    ftm_gsm_rx_disable( chain_vars_ptr->prx_dev );

    /* update band in both rx and tx vars*/

    if (tx_is_supported) 
    {
      /*Disable TX if it is Turned ON*/
      chain_vars_ptr->tx_vars_ptr->ftm_num_bursts_tx = 0;
      chain_vars_ptr->tx_vars_ptr->ftm_cont_tx = FALSE;
    }
    cmn_vars_ptr->ftm_gl1_hw_gsm_band = gsm_sys_band;	

    /*Disable RX if it is Turned ON*/
    chain_vars_ptr->rx_vars_ptr->ftm_cont_rx = FALSE;
    chain_vars_ptr->rx_vars_ptr->ftm_num_bursts_rx = 0;
    chain_vars_ptr->rx_vars_ptr->ftm_gsm_state.ftm_rf_mode = (word)mode;

    /* Reset GSM burst tags index */
    chain_vars_ptr->rx_vars_ptr->tags_rd_index = 0;
    chain_vars_ptr->rx_vars_ptr->tags_wr_index = 0;

    /* Rx Burst is not on */
    chain_vars_ptr->rx_vars_ptr->rxBurstOn = FALSE;

    /* Set the core data */
    rfgsm_core_data.rf_task = RF_TASK_FTM;
    rfgsm_core_data.rfm_dev = chain_vars_ptr->prx_dev;
    rfgsm_core_data.sub_id = chain_vars_ptr->sub_id;
    rfgsm_core_data.as_id = ftm_gsm_ctl_map_sub_id_to_as_id(rfgsm_core_data.sub_id);
    rfgsm_core_data.triple_buffer_id = 0; //Default for now
    rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

    /* Set the band */
    rfgsm_mc_set_rx_band(&rfgsm_core_data, (rfgsm_band_type)gsm_band );

    if(tx_is_supported )
    {
      geran_test_mode_api_rf_set_tx_band( (int8)gsm_band, rfgsm_core_data.as_id );
    }

    ftm_set_gsm_rf_db_state(gsm_band);

    /* Set to default channel */
    ftm_gsm_set_arfcn( chain_vars_ptr, default_channel );

    ftm_clk_rex_wait(10);
  }
} /* end ftm_gsm_set_mode */


/*===========================================================================

FUNCTION FTM_SET_GSM_RF_DB_STATE

DESCRIPTION
    This function will update ftm_current_rf_mode according to corresponding GSM band

DEPENDENCIES
   
RETURN VALUE
   None.

SIDE EFFECTS
   None

===========================================================================*/


void ftm_set_gsm_rf_db_state(rfgsm_band_type band)
{
	
	switch ( band )
	{
	case RFGSM_BAND_GSM850:
      ftm_set_rf_db_state(FTM_DB_RF_GSM_850);
      break;
	
	case RFGSM_BAND_GSM900:
      ftm_set_rf_db_state(FTM_DB_RF_GSM_900);
      break;
	
	case RFGSM_BAND_GSM1800:		
      ftm_set_rf_db_state(FTM_DB_RF_GSM_1800);
      break;
	
	case RFGSM_BAND_GSM1900:		
      ftm_set_rf_db_state(FTM_DB_RF_GSM_1900);
      break;
	
	default:
	  MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR,"unsupported band %d",band);
      ftm_set_rf_db_state(FTM_DB_RF_IDLE);
      break;
	}
}



/*===========================================================================

FUNCTION FTM_GSM_FRAME_SWEEP_SET_BAND

DESCRIPTION
    This function sets a new GSM band. Rx and Tx should be off before calling this API 
    as it does not take care of any race conditions.

DEPENDENCIES
   
RETURN VALUE
   None.

SIDE EFFECTS
   None

===========================================================================*/
void ftm_gsm_frame_sweep_set_band( rfm_device_enum_type rfm_dev,
                                   ftm_mode_id_type mode )
{
  boolean change_mode = FALSE;
  uint16  default_channel =1;
  rfgsm_band_type gsm_band = RFGSM_BAND_GSM900;
  rfgsm_core_data_type rfgsm_core_data;
  rfgsm_core_handle_type* rfgsm_core_handle_ptr = NULL;
  ftm_gsm_chains_enum_type gsm_curr_chain = FTM_GSM_INVALID_CHAIN;
  ftm_gsm_chain_vars_s * chain_vars_ptr = NULL;
  ftm_gsm_sub_vars_s *   sub_vars_ptr   = NULL;
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_MAX;
  ftm_gsm_rfctl_vars_s * vars_ptr = ftm_gsm_get_vars_ptr(rfm_dev);  
  ftm_gsm_cmn_vars_s* cmn_vars_ptr;

  if( vars_ptr == NULL ) 
  {
     ERR_FATAL("ftm_gsm_frame_sweep_set_band() gvars NULL for remapped Tx device %d", rfm_dev, 0, 0 );
     return;
  }

  gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( vars_ptr->dispatch_device );
  chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( gsm_curr_chain );  

  if( chain_vars_ptr == NULL ) 
  {
     ERR_FATAL("ftm_gsm_frame_sweep_set_band() chain_vars_ptr NULL for dispatch device %d and chain %d", vars_ptr->dispatch_device, gsm_curr_chain, 0 );
     return;
  }
  
  cmn_vars_ptr = chain_vars_ptr->cmn_vars_ptr;
  if( cmn_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_frame_sweep_set_band() cmn_vars_ptr is NULL for dispatched dev %d", 
              vars_ptr->dispatch_device, 0, 0 );
	return;
  }

  /* Assign ACCESS STRATUM from chain vars sub ID */
  as_id = ftm_gsm_ctl_map_sub_id_to_as_id( chain_vars_ptr->sub_id );

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  if ( rfgsm_core_handle_ptr == NULL )
  {
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "rfgsm_core_handle_ptr is NULL", rfm_dev);
    return;
  }

  switch ( mode )
  {
  case FTM_PHONE_MODE_GSM_850:
    if ( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_CELL_850 )
    {
      change_mode = TRUE;
      gsm_band = RFGSM_BAND_GSM850;
      cmn_vars_ptr->ftm_gl1_hw_gsm_band = SYS_BAND_CELL_850;
      default_channel = 128;
    }
    break;

  case FTM_PHONE_MODE_GSM_900:
    if ( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_EGSM_900 )
    {
      change_mode = TRUE;
      gsm_band = RFGSM_BAND_GSM900;
      cmn_vars_ptr->ftm_gl1_hw_gsm_band = SYS_BAND_EGSM_900;
      default_channel = 1;
    }
    break;

  case FTM_PHONE_MODE_GSM_1800:
    if ( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_DCS_1800 )
    {
      change_mode = TRUE;
      gsm_band = RFGSM_BAND_GSM1800;
      cmn_vars_ptr->ftm_gl1_hw_gsm_band = SYS_BAND_DCS_1800;
      default_channel = 512;
    }
    break;

  case FTM_PHONE_MODE_GSM_1900:
    if ( cmn_vars_ptr->ftm_gl1_hw_gsm_band != SYS_BAND_PCS_1900 )
    {
      change_mode = TRUE;      
      gsm_band = RFGSM_BAND_GSM1900;
      cmn_vars_ptr->ftm_gl1_hw_gsm_band = SYS_BAND_PCS_1900;
      default_channel = 512;
    }
    break;

  default:
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR,"unsupported band %d",mode);
    change_mode = FALSE;
    break;
  }

  if ( change_mode == TRUE )
  {
    sub_vars_ptr   = ftm_gsm_get_sub_vars_ptr(chain_vars_ptr->sub_id);
    /* reset the frame counter for TDMA ISR initialize RX, TX and Synth */
    sub_vars_ptr->ftm_frame_count = 0;

    vars_ptr->ftm_gsm_state.ftm_rf_mode = (word)mode;

    /* Reset GSM burst tags index */
    vars_ptr->tags_rd_index = 0;
    vars_ptr->tags_wr_index = 0;

    /* Set the core data */
    rfgsm_core_data.rf_task = RF_TASK_FTM;
    rfgsm_core_data.rfm_dev = rfm_dev;
    rfgsm_core_data.sub_id = 0; //Default for now
    rfgsm_core_data.as_id = as_id;
    rfgsm_core_data.triple_buffer_id = 0; //Default for now
    rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

    /* set the band */
    rfgsm_mc_set_rx_band(&rfgsm_core_data, (rfgsm_band_type)gsm_band );

    geran_test_mode_api_rf_set_tx_band((int8)gsm_band,as_id);

    /* Set to default channel */
    cmn_vars_ptr->ftm_channel = default_channel;
  }
} /* end ftm_gsm_frame_sweep_set_band */


/*===========================================================================

FUNCTION  ftm_tdma_isr

DESCRIPTION
  this function is a TDMA frame processing handler for FTM
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ftm_tdma_isr( sys_modem_as_id_e_type as_id)
{
  rfgsm_core_data_type rfgsm_core_data;
  boolean tx_is_supported = FALSE;

  ftm_gsm_chain_vars_s * chain_vars_ptr =NULL;
#ifdef FTM_HAS_LOGGING
  ftm_log_gsm_therm_vbatt_type *ftm_log_pkt = NULL;
#endif
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  ftm_gsm_rfctl_vars_s * rx_vars_ptr = NULL;
  ftm_gsm_rfctl_vars_s * tx_vars_ptr  = NULL;
  ftm_gsm_cmn_vars_s* cmn_vars_ptr = NULL;

  ftm_gsm_sub_vars_s * sub_vars_ptr = ftm_gsm_get_sub_vars_ptr((uint8)as_id);

  if (sub_vars_ptr == NULL ) 
  {
     FTM_MSG_1( FTM_ERROR,
                "ftm_tdma_isr() FTM GSM Sub vars ptr"
                " is NULL for FTM SUB : %d ", 
                as_id);
     return;
  }

  chain_vars_ptr = ftm_gsm_get_chain_vars_ptr(sub_vars_ptr->active_gsm_chain);

  if ( chain_vars_ptr->rx_vars_ptr  == NULL ||
       chain_vars_ptr->cmn_vars_ptr == NULL ) 
  {
     ERR_FATAL( "ftm_tdma_isr() rx_vars_ptr/cmn_vars_ptr is NULL"
                " for FTM GSM Chain: %d, rx device %d ", 
                chain_vars_ptr->gsm_chain, chain_vars_ptr->prx_dev,0);
     return;
  }

  /*Obtain the variable pointer for RX device*/
  rx_vars_ptr = chain_vars_ptr->rx_vars_ptr;

  cmn_vars_ptr = chain_vars_ptr->cmn_vars_ptr;

  /*Check if TX is active on the given chain*/
  tx_is_supported  = ftm_gsm_device_is_tx_supported(chain_vars_ptr->associated_tx_dev);

  if (tx_is_supported) 
  {
    if (chain_vars_ptr->tx_vars_ptr == NULL) 
    {
       FTM_MSG_2( FTM_ERROR,
                  "ftm_tdma_isr() tx_vars_ptr is NULL "
                  "for FTM GSM Chain: %d, Tx device %d ", 
                  chain_vars_ptr->gsm_chain, 
                  chain_vars_ptr->associated_tx_dev );
       return;
    }

    /*Obtain the variable pointer for TX device*/
    tx_vars_ptr = chain_vars_ptr->tx_vars_ptr;

    /* Obtain the core handle pointer for TX device for VBatt processing*/
    rfgsm_core_handle_ptr = rfgsm_core_handle_get(chain_vars_ptr->associated_tx_dev);
    if ( rfgsm_core_handle_ptr == NULL )
    {
      FTM_MSG_1( FTM_ERROR,
                 "ftm_tdma_isr()rfgsm_core_handle_ptr NULL! for TX device : %d",
                 chain_vars_ptr->associated_tx_dev);
      return;
    }

  } /* tx_is_supported */


  /* Set the core data for idle frame processing*/
  rfgsm_core_data.rf_task = RF_TASK_FTM;
  rfgsm_core_data.rfm_dev = chain_vars_ptr->associated_tx_dev;
  rfgsm_core_data.sub_id = as_id;
  rfgsm_core_data.as_id = as_id; 
  rfgsm_core_data.triple_buffer_id = 0;
  rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

  /* Increment frame counter */
  sub_vars_ptr->ftm_frame_count++;

  /*Logging every 5000th frame for trace*/
  if ( !(sub_vars_ptr->ftm_frame_count % 5000) ) 
  {
    FTM_MSG_3( FTM_LOW,
              "ftm_tdma_isr() Current Frame %d,"
              " active gsm chain %d, sub id :%d",
              sub_vars_ptr->ftm_frame_count,
              sub_vars_ptr->active_gsm_chain,
              rfgsm_core_data.sub_id );
  }

  geran_test_mode_api_schedule_frame_tick (sub_vars_ptr->ftm_frame_count,as_id);

  //Log the data 
#ifdef FTM_HAS_LOGGING
  ftm_log_pkt = (ftm_log_gsm_therm_vbatt_type *)ftm_log_malloc ((uint16)FTM_LOG_GSM_THERM_VBATT, sizeof( ftm_log_gsm_therm_vbatt_type) );
  if ( ftm_log_pkt != NULL )
  {

    ftm_log_pkt->therm_recorded      = therm_recorded;
    ftm_log_pkt->temp_comp_bin_index = temp_comp_bin_index;
    ftm_log_pkt->temp_comp_rem       = temp_comp_rem;
    ftm_log_pkt->temp_comp_val       = temp_comp_val;
    ftm_log_pkt->vbatt_recorded      = vbatt_recorded;
    ftm_log_pkt->vbatt_backoff       = vbatt_backoff;
  }
  /* send log */
  log_commit( (ftm_log_type*) ftm_log_pkt );
#endif  

  /* initialize RX/TX chain (channel filter and PA power profile etc.) */
  if ( sub_vars_ptr->ftm_frame_count == RX_TX_START_INIT_CNT )
  {
    ftm_gsm_init_rx( chain_vars_ptr->prx_dev );

    if (tx_is_supported) 
    {
       ftm_gsm_init_tx( chain_vars_ptr->associated_tx_dev);
    }

    FTM_MSG_3( FTM_MED,
               "ftm_tdma_isr() RX/TX Initialized on frame %d,"
               " active gsm chain %d, sub id :%d",
               sub_vars_ptr->ftm_frame_count,
               sub_vars_ptr->active_gsm_chain,
               rfgsm_core_data.sub_id );
  }
  
  /* handler for FTM sweep funtionality*/
  ftm_gsm_frame_sweep_handler(chain_vars_ptr);

  /*Only perform tx commands if TX device is supported*/
  if (tx_is_supported) /*tx_is_supported*/
  {
     /* Perform the TX command if any */
     if ( tx_vars_ptr->ftm_gsm_state.ftm_tx_state == ON )
     {
       /* Perform TX if cont TX is set    or   Schedule remaining number of TX bursts  */
       if ( tx_vars_ptr->ftm_cont_tx || 
            ((tx_vars_ptr->ftm_num_bursts_tx) ? tx_vars_ptr->ftm_num_bursts_tx-- : tx_vars_ptr->ftm_num_bursts_tx) )
       {
         /* if ftm_gsm_idle_proc_flag is true , idle processing is enabled*/
         if( ( sub_vars_ptr->ftm_frame_count % FTM_GSM_IDLE_FRAME_NUM == 1 ) && 
               cmn_vars_ptr->ftm_gsm_idle_proc_flag) /* Idle frame*/   
         { 
           /* if the cal is NOT in progress we can do IDLE frame processing */
           if ( cmn_vars_ptr->ftm_gsm_cal_flag == FALSE ) 
           {
             if( !rfgsm_mc_do_idle_frame_processing(&rfgsm_core_data) )
             {
               FTM_MSG( FTM_ERROR,
                        "ftm_tdma_isr() idle frame processing failed!");
               return;
             }
             else
             {
               FTM_MSG_3( FTM_MED,
                          "ftm_tdma_isr() IDLE frame processing on frame %d,"
                          " active gsm chain %d, sub id :%d",
                          sub_vars_ptr->ftm_frame_count,
                          sub_vars_ptr->active_gsm_chain,
                          rfgsm_core_data.sub_id );
             }
           }
         }

         /* do not call TX function on the frame immediately before the idle frame*/
         if( ( sub_vars_ptr->ftm_frame_count % FTM_GSM_IDLE_FRAME_NUM != 0 ) || 
             (! cmn_vars_ptr->ftm_gsm_idle_proc_flag) )
         {
           (*tx_vars_ptr->ftm_tx_funct)(chain_vars_ptr->associated_tx_dev);
         }
       }
     }
     else /*if ftm_tx_state is OFF*/
     {
       ftm_gsm_do_tx_stop_cont( chain_vars_ptr->associated_tx_dev); 
     }
  } /*tx_is_supported*/

  /* Perform the RX command if any */
  if ( rx_vars_ptr->ftm_cont_rx || 
       ( (rx_vars_ptr->ftm_num_bursts_rx) ? rx_vars_ptr->ftm_num_bursts_rx-- : rx_vars_ptr->ftm_num_bursts_rx) )
  {
    (*rx_vars_ptr->ftm_rx_funct)(chain_vars_ptr->prx_dev);
   
  }

  if (tx_is_supported) 
  {
     if( tx_vars_ptr->do_tx_da_cal ) 
     {
         ftm_da_cal_send_tx_cal_cmd( chain_vars_ptr->associated_tx_dev, chain_vars_ptr->sub_id );

     }
  } /* tx_is_supported */


  if ( rx_vars_ptr->ip2_cal_data.do_ip2_cal )
  {

    ftm_gsm_do_ip2_cal(chain_vars_ptr->prx_dev);
  }


  if (TRUE == rx_vars_ptr->ftm_gsm_mode_enter)
  {
  /* Tick the DTM manager - must come after the DL and UL functions */
    geran_test_mode_api_sched_rx_tx(as_id);
  }

}

/*! @brief Calls the ISR fucntion as SUB0 */
void ftm_tdma_isr_sub0( sys_modem_as_id_e_type as_id )
{
  /* This functionality will further change in phase 2*/

  ftm_tdma_isr( as_id);
}

/*! @brief Calls the ISR fucntion as SUB1 */
void ftm_tdma_isr_sub1( sys_modem_as_id_e_type as_id )
{
  /* This functionality will further change in phase 2*/
 

  ftm_tdma_isr( as_id );
}



/*===========================================================================

FUNCTION  ftm_send_FTM_STOP_GSM_MODE_REQ

DESCRIPTION
  This function sends a MPH_STOP_GSM_MODE_REQ to GSM L1
  
DEPENDENCIES
  None

RETURN VALUE
  Error code for 

SIDE EFFECTS
  None

===========================================================================*/
gs_status_T ftm_send_FTM_STOP_GSM_MODE_REQ( sys_modem_as_id_e_type as_id)
{
  gs_status_T status;

#ifdef FEATURE_DUAL_SIM
  status = geran_test_mode_api_send_FTM_STOP_GSM_MODE_REQ( as_id );
#else
  as_id = as_id;
  status =  geran_test_mode_api_send_FTM_STOP_GSM_MODE_REQ();
#endif /* FEATURE_DUAL_SIM */

  return status;

} /* ftm_send_FTM_STOP_GSM_MODE_REQ */

/*----------------------------------------------------------------------------*/
/*
@brief
    ftm_mutex_init() creates a mutex for the FTM Mode.
 
  @param None
    
 
  @retval None
    
 
  @details
  Mutexes are used to allow only one thread to enter the critical section
  of code that is accessing shared data. Thus, a given data structure which
  needs mutual exclusion will need to create a mutex to go with it. 
 
*/
void ftm_mutex_init(void)
{
  /* In Q6 both tasks and ISRs are handled the same way */
  rex_init_crit_sect(&ftm_gsm_lock);
}



/*!
@brief
   ftm_gsm_init_once does one time GSM FTM initialisation
 
  @param None
    
 
  @retval None
    
 
  @details
  Creates and populates the global variables required for FTM operation.
 
*/
void ftm_gsm_init_once( void )
{
  static boolean init_done_once = FALSE;

  if ( init_done_once == FALSE )
  {
    ftm_gsm_init_ftm_slot_mutex();

    ftm_mutex_init();

    init_done_once = TRUE;
  }
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Wait function 

  @details
  Waits for a given signal mask

  @param mask - signals to wait for.
  @param as_id - subscription id, not used

*/  
void ftm_gsm_sig_rex_wait_no_clear_cb
(
  rex_sigs_type   mask,        
  sys_modem_as_id_e_type as_id
)
{
  as_id = as_id;

  ftm_sig_rex_wait_no_clear_cb( mask );

} /* End ftm_gsm_sig_rex_wait_no_clear_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Calls the geran test mode api start command 

  @details
  

  @param chain_vars_ptr - Pointer to chain parameters

*/ 
void ftm_gsm_start_geran( void * vars_ptr )
{
  boolean                         enable_rxd = FALSE;
  uint64                          rxd_dev_band_mask = 0;
  geran_test_mode_api_rxd_type    rxd_control;
  geran_test_mode_api_device_type devices;
  sys_modem_as_id_e_type          as_id;
  ftm_gsm_chains_enum_type        curr_chain = FTM_GSM_INVALID_CHAIN;
  ftm_gsm_sub_vars_s              *sub_vars_ptr = NULL;
  ftm_gsm_chain_vars_s            *chain_vars_ptr = NULL;
  rfcom_gsm_band_type             rfgsm_com_band;
  sys_band_mask_type              bit_mask;

  /* The cast from void is required because of awful dependency chain */
  chain_vars_ptr = (ftm_gsm_chain_vars_s*)vars_ptr;
  if( chain_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_start_geran() chain_vars_ptr NULL", 0, 0, 0);
  }

  sub_vars_ptr = ftm_gsm_get_sub_vars_ptr( chain_vars_ptr->sub_id );
  if( sub_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_start_geran() sub_vars_ptr NULL", 0, 0, 0);
  }

  /*Initialise the gl1_hw_init state to RX only type.*/
  gl1_init_rflm_type gsm_init_rflm_state = GL1_HW_INIT_RX_ONLY;

  /*Obtain the geran as id from ftm sub id*/
  as_id =ftm_gsm_ctl_map_sub_id_to_as_id(chain_vars_ptr->sub_id);

  /* This code needs revisiting*/
  /*Check if rfm_device is tx capable. */
  if( rfgsm_core_get_device_tx_capability( chain_vars_ptr->associated_tx_dev ) )
  {
     /*Introduce a new gsm init rflm state since Non-signalling for TX is confusing.*/
    /*gl1_hw_init state to turn on both RX and TX  from GL1*/
    if (ftm_gsm_is_in_calibration_mode)
    {
      gsm_init_rflm_state = GL1_HW_INIT_RX_TX;
    }
    else
    {
      gsm_init_rflm_state = GL1_HW_INIT_NON_SIGNALLING;
    }
  }

  RF_MSG_4( RF_HIGH, 
            "ftm_gsm_start_geran() gsm_init_rflm_state %d for PRx dev %d, DRx dev %d, Tx dev %d", 
            gsm_init_rflm_state, chain_vars_ptr->prx_dev, chain_vars_ptr->associated_drx_dev, chain_vars_ptr->associated_tx_dev );

  /* Set the ISR function */
  /* This functionality will further change in phase 2*/
  if( !ftm_gsm_update_sub_vars( chain_vars_ptr->gsm_chain, chain_vars_ptr->sub_id ) )
  {
    ERR_FATAL("Failed to update sub variables for FTM GSM subscription: %d, current chain: %d",
              chain_vars_ptr->sub_id, chain_vars_ptr->gsm_chain, 0);
  }

  /*Query BC config NV for DRX to retrieve the band mask information. 
    Query only if the device has any band support in GSM (i.e. is a valid device for GSM) */
  if( rfgsm_core_get_device_rx_capability( chain_vars_ptr->associated_drx_dev ) )
  {
    rxd_dev_band_mask = rfcommon_nv_get_rx_device_bc_config( chain_vars_ptr->associated_drx_dev);
  }

  for (rfgsm_com_band = 0; rfgsm_com_band < RFGSM_MAX_UNIQUE_RFCOM_BANDS; rfgsm_com_band++) 
  {
     bit_mask = rfgsm_core_get_sys_band_mask(rfgsm_com_band);

     /*Update RX Diversity support per band*/
     if (rxd_dev_band_mask & bit_mask) 
     {
        devices.drx_band_support[rfgsm_com_band] = TRUE;
        MSG_1(MSG_SSID_FTM, MSG_LEGACY_HIGH, "ftm_gsm_start_geran() GSM RX Diversity is supported on the band %d", 
              rfgsm_com_band);
        

        /*Confirm if GSM Bands are supported*/
        if( chain_vars_ptr->rx_vars_ptr == NULL )
        {
          ERR_FATAL("ftm_gsm_start_geran() chain_vars_ptr->rx_vars_ptr NULL for remapped dev %d", chain_vars_ptr->prx_dev, 0, 0);
        }
        // Set the enable RxD flag to pass to GL1. Overall switch for diversity control  
        /* Before GL1 supports band-to-band control, need to make enable_rxd band-related. 
           As long as one band is supported, FTM  diversity_on will dominate.
        */
        enable_rxd = (enable_rxd||TRUE) && (chain_vars_ptr->rx_vars_ptr->diversity_on); 

     }
     else
     {
        devices.drx_band_support[rfgsm_com_band] = FALSE;
        MSG_1(MSG_SSID_FTM, MSG_LEGACY_HIGH, "ftm_gsm_start_geran() GSM RX Diversity is not supported on the band %d ", 
                rfgsm_com_band);
     }
  }


  rxd_control.enableRxd = enable_rxd;
  rxd_control.enableDrx = TRUE;
  rxd_control.enablePrx = TRUE;

  devices.prx = chain_vars_ptr->prx_dev;
  devices.drx = chain_vars_ptr->associated_drx_dev;
  devices.tx  = chain_vars_ptr->associated_tx_dev;

  RF_MSG_4( RF_HIGH, 
            "ftm_gsm_start_geran() RxD enabled %d, for PRx dev %d, DRx dev %d, Tx dev %d", 
            enable_rxd, chain_vars_ptr->prx_dev, chain_vars_ptr->associated_drx_dev, chain_vars_ptr->associated_tx_dev ); 

  /* Checks for Victim status. Only gets TRUE if a NPT test is going on.*/
  if ( ftm_device_manager_is_device_npt_victim(chain_vars_ptr->prx_dev) )
  {
    RF_MSG_1( RF_HIGH, "ftm_gsm_start_geran(), victim for dev %d, disable Tx",
              chain_vars_ptr->prx_dev );
    gsm_init_rflm_state = GL1_HW_INIT_RX_ONLY; 
    if( ftm_device_manager_is_device_in_dta_scenario(chain_vars_ptr->prx_dev) ) 
    {
      rxd_control.enableRxd = FALSE;
      RF_MSG( RF_HIGH, "ftm_gsm_start_geran(), disable Drx");      
    }
  }

  /* this sets up rxlm and txlm buffers, requests mcpm resources for FTM, loads MDSP fw.  */
  geran_test_mode_api_start_geran( sub_vars_ptr->ftm_tdma_isr_fnc,
                                   TASK_HANDLE(ftm),
                                   ftm_gsm_sig_rex_wait_no_clear_cb,
                                   FTM_SIG_WAIT_SIG, 
                                   FTM_SIG_WAIT_SIG2,
                                   gsm_init_rflm_state,
                                   rxd_control,
                                   devices,
                                   as_id );

  chain_vars_ptr->rx_vars_ptr->ftm_gsm_mode_enter = TRUE;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Stops any RX and TX activity and calls the geran test mode api stop command 

  @details
  

  @param rfm_dev - Logical RF device

*/ 
void ftm_gsm_stop_geran( rfm_device_enum_type rfm_dev )
{
  ftm_gsm_chains_enum_type gsm_curr_chain = FTM_GSM_INVALID_CHAIN;
  ftm_gsm_chain_vars_s * chain_vars_ptr = NULL;
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_MAX;
  ftm_gsm_rfctl_vars_s * vars_ptr = ftm_gsm_get_vars_ptr(rfm_dev);
  
  if( vars_ptr == NULL ) 
  {
     ERR_FATAL("ftm_gsm_stop_geran() gvars NULL for remapped Tx device %d", rfm_dev, 0, 0 );
     return;
  }
  
  gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( vars_ptr->dispatch_device );
  chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( gsm_curr_chain );
  if( chain_vars_ptr == NULL ) 
  {
     ERR_FATAL("ftm_gsm_stop_geran() chain_vars_ptr NULL for dispatch device %d and chain %d", vars_ptr->dispatch_device, gsm_curr_chain, 0 );
     return;
  }
  
  /* Assign ACCESS STRATUM from chain vars sub ID */
  as_id = ftm_gsm_ctl_map_sub_id_to_as_id( chain_vars_ptr->sub_id );

  /* Disable TX */
  if (((chain_vars_ptr->tx_vars_ptr)!=NULL) && (chain_vars_ptr->tx_vars_ptr->ftm_gsm_state.ftm_tx_state ))
  {
    MSG_2( MSG_SSID_FTM, MSG_LEGACY_ERROR, "ftm_gsm_stop_geran TxStop state %d for device %d", chain_vars_ptr->tx_vars_ptr->ftm_gsm_state.ftm_tx_state ,chain_vars_ptr->tx_vars_ptr->rfm_dev);
    ftm_gsm_set_tx( chain_vars_ptr->tx_vars_ptr->rfm_dev, OFF );
  }

  /* Disable RX */
  ftm_gsm_rx_disable( rfm_dev );

  mdsp_ftm_wait_for_frame_tick(as_id);

  /* Tell the geran test mode API to stop*/
  geran_test_mode_api_stop_geran( TRUE, as_id );

  vars_ptr->ftm_gsm_mode_enter = FALSE;
}


/*===========================================================================

FUNCTION  FTM_GET_TX_FREQUENCY_OFFSET

DESCRIPTION
   This function gets the GSM TX frequency offset set in FTM mode. In online mode it returns 0.
   
DEPENDENCIES
   None.

RETURN VALUE
  GSM TX frequency offset set in FTM mode.

SIDE EFFECTS
   None.

===========================================================================*/
int32 ftm_gsm_get_tx_frequency_offset( void  )
{
  if ( ftm_gsm_vars_created(ftm_gsm_current_tx_cal_dev) )
  {
    return ftm_gsm_get_vars_ptr(ftm_gsm_current_tx_cal_dev)->ftm_gsm_tx_freq_offset;
  }
  else
  {
    /* Not in FTM mode or frequency offset not set */
    return 0;
  }
}

/*===========================================================================

FUNCTION  FTM_SET_TX_FREQUENCY_OFFSET

DESCRIPTION
   This function sets the GSM TX frequency offset set in FTM mode.
   
DEPENDENCIES
   None.

RETURN VALUE
  None.

SIDE EFFECTS
   None.

===========================================================================*/
void ftm_gsm_set_tx_frequency_offset( rfm_device_enum_type rfm_dev, 
                                      int32 offset )
{  
  if( ftm_gsm_vars_created(rfm_dev) )
  {
    ftm_gsm_get_vars_ptr(rfm_dev)->ftm_gsm_tx_freq_offset = offset;

    ftm_gsm_current_tx_cal_dev = rfm_dev;

    MSG_2( MSG_SSID_FTM, MSG_LEGACY_HIGH,
           "GSM TX Freq offset for device %d set to %d", rfm_dev, offset );
  }
}


/*============================================================================

FUNCTION 
  ftm_gsm_program_qfe_scenario

DESCRIPTION
  This API will be called to program the tuner immediately and no script or FW 
  action is required. Inside this API check all the active paths then give a call to
  tuner manager associated with appropriate tuners

DEPENDENCIES
  None

RETURN VALUE
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)

SIDE EFFECTS
  None
=============================================================================*/

void ftm_gsm_program_qfe_scenario(void)
{
  boolean status = FALSE;

  status = rfgsm_mc_program_qfe_scenario();

  if(!status)
  {
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR,"FTM_GSM_PROGRAM_QFE_SCENARIO: Failed ", 0);
  }
}


/*============================================================================

FUNCTION 
  ftm_gsm_overwrite_alt_path

DESCRIPTION
  This API will be store required alt_path for overwritting

DEPENDENCIES
  None

RETURN VALUE
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)

SIDE EFFECTS
  None
=============================================================================*/

boolean ftm_gsm_overwrite_alt_path(rfm_device_enum_type rfm_dev, uint8 alt_path, boolean overwrite)
{
#ifdef FEATURE_RFGSM_ALT_PATH_SUPPORTED
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  if ( rfgsm_core_handle_ptr == NULL )
  {
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "rfgsm_core_handle_ptr is NULL", rfm_dev);
    return FALSE;
  }

  rfgsm_core_handle_ptr->overwrite_alt_path = overwrite;
  rfgsm_core_handle_ptr->alt_path_to_overwrite =  alt_path;

  if(overwrite) MSG_2(MSG_SSID_FTM, MSG_LEGACY_HIGH,"overwrite alt_path to %d on rfm_dev %d for all GSM bands", alt_path, rfm_dev);
  else  MSG(MSG_SSID_FTM, MSG_LEGACY_HIGH,"disable to overwrite alt_path");
#else
  MSG(MSG_SSID_FTM, MSG_LEGACY_ERROR,"ftm_gsm_overwrite_alt_path() alt path not supported");
#endif
  return TRUE;
}


/*============================================================================

FUNCTION 
  ftm_gsm_get_nv_container

DESCRIPTION
  This API get nv container

DEPENDENCIES
  None

RETURN VALUE
  rfgsm_nv_path_type

SIDE EFFECTS
  None
=============================================================================*/

boolean ftm_gsm_get_nv_container(rfm_device_enum_type rx_device, ftm_mode_id_type band, rfgsm_nv_container_device_type nv_cont_type, rfgsm_nv_path_type* nv_container)
{
  /*ftm device manager passes prx device only based on the dispatch device*/
  rfm_device_enum_type rfm_dev = rx_device;
  rfgsm_nv_rxtx_enum_type rx_tx = RFGSM_NV_RX;
  rfcom_gsm_band_type rfcom_band = ftm_gsm_convert_ftm_mode_id_type_to_rfcom(band);

  if(nv_container == NULL)
  {
    MSG(MSG_SSID_FTM, MSG_LEGACY_ERROR, "NULL nv_container! ");
    return FALSE;
  }

  if (nv_cont_type == RFGSM_NV_CONT_DRX)
  {
    /*For DRX path, obtain the drx device from RFC*/
    rfm_dev = ftm_gsm_get_associated_rx_device( rx_device );
  }
  else if (nv_cont_type == RFGSM_NV_CONT_TX)
  {
    /*For TX path, obtain associated TX device from RFC*/
    rfm_dev = ftm_gsm_get_associated_tx_device( rx_device );
    rx_tx = RFGSM_NV_TX;
  }

  *nv_container = rfgsm_nv_get_nv_container(rfm_dev, rfcom_band, rx_tx);

  MSG_4( MSG_SSID_FTM, MSG_LEGACY_HIGH,
         "use NV container %d for rfm_dev %d, band %d, device_type %d",
         *nv_container, rfm_dev, rfcom_band, nv_cont_type);
  
  return TRUE;  
}

#ifdef FEATURE_GERAN_RF_SELF_TEST
/*============================================================================

FUNCTION 
  ftm_gsm_fbrx_meas_tx_params

DESCRIPTION
  This API starts GSM fbrx selftest power measurement
  Requirement 1) turn on Tx before calling the api.
                    2) define the power at 1st slot of frame matrix
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
=============================================================================*/

void ftm_gsm_fbrx_meas_tx_params
(
   rfcom_device_enum_type device,
   rfcom_device_enum_type tx_device,
   uint16 num_averages,
   uint32 capture_offset,
   uint32 tx_measurement_config,
   void* cmd_ptr,
   ftm_rsp_pkt_type *rsp_pkt
)
{
   
   int payload_size_count = 0;
   boolean enable_tx_pwr_meas;
   int16 tx_pwr;
   uint16 evm_value = 0; 
   uint32 rx_rms_mag = 0; 
   uint64 lo_freq=0;
   rfcom_gsm_band_type tx_band;
   uint16 rf_tx_arfcn;
   rfgsm_core_handle_type *rfgsm_core_handle_ptr;
   ftm_common_selftest_fbrxgs_info_type fbrx_gs_info;
   rfcom_band_type_u rfcom_band;
   uint32 samp_rate = -1;
   rf_time_type prof_time = 0;
   rf_time_tick_type prof_tick;      
   int32 *iq_buf_rx = NULL;
   int32 *iq_buf_tx = NULL;
   int32 power_meas_offset = 0;


   prof_tick = rf_time_get_tick();
   rfgsm_core_handle_ptr = rfgsm_core_handle_get(tx_device);

   /* Enable FW Application */
   fws_app_enable(FW_APP_RFCMD);

  /***************** Fill up RSP packet with standard data *****************/
  
  // Fill in the constant fields of the response packet. 
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.cmd_code = 75;
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.subsys_id = FTM;
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.subsys_cmd_code = ((ftm_diagpkt_subsys_header_type*)cmd_ptr)->subsys_cmd_code;
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.ftm_rf_cmd = FTM_RF_MEAS_TX_PARAMS;  

  // Start with sucess flag and change it if we fail any check conditions.
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.ftm_error_code = FTM_FBRX_TEST_SUCCESS;
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.num_averages = num_averages;
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.capture_offset = capture_offset;
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.tx_measurement_config = tx_measurement_config;
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.payload_size = FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE; 

  // Fill in the constant fields of response packet.Size of the packet will be based on the requested Tx measurement config
  rsp_pkt->cmd = FTM_RSP_DO_NEW_PKT;
  rsp_pkt->pkt_payload = &(ftm_rf_fbrx_meas_tx_params_rsp_pkt);
  rsp_pkt->delete_payload = FALSE;  
  rsp_pkt->pkt_len = FTM_RF_MEAS_TX_PARAMS_HDR_SIZE; //need to check the size afterwards 
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.ftm_error_code = FTM_FBRX_TEST_SUCCESS;


  tx_band = rfgsm_core_handle_ptr->tx_band;
  rfcom_band.gsm_band = tx_band;
  fbrx_gs_info.com_band = rfcom_band;
  fbrx_gs_info.override_enable = FALSE;      
  rf_tx_arfcn = rfgsm_core_handle_ptr->tx_log.tx_chan;

  /* FBRX selftest */
  if(!ftm_common_fbrx_iq_capture_processing(
      FTM_RF_TECH_GSM,
      device,
      tx_measurement_config, 
      num_averages,
      &iq_buf_rx, 
      &iq_buf_tx, 
      &evm_value, 
      &rx_rms_mag,
      &samp_rate,
      &fbrx_gs_info,
      tx_device
      ) 
  )
  {
      ftm_rf_fbrx_meas_tx_params_rsp_pkt.ftm_error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
  }

  

  
  rfcommon_iq_efs_save(
   iq_buf_tx, 
   iq_buf_rx, 
   9216);

  /* gsm power compensation over bands */ 
  if(rfgsm_core_handle_ptr->band == RFCOM_BAND_GSM850)
  {
    power_meas_offset = -9100;
  }
  else if(rfgsm_core_handle_ptr->band == RFCOM_BAND_GSM900)
  {  
    power_meas_offset = -9160;
  }  
  else if(rfgsm_core_handle_ptr->band == RFCOM_BAND_GSM1800)
  {    
    power_meas_offset = -8550;
  }  
  else if(rfgsm_core_handle_ptr->band == RFCOM_BAND_GSM1900)  
  {
    power_meas_offset = -8600;
  }
  rx_rms_mag += power_meas_offset;  
  
  // populate the response packet with calculated TX power
  /*Populate Tx power data*/
  memscpy(&ftm_rf_fbrx_meas_tx_params_rsp_pkt.result[payload_size_count],
          (FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count),
          &(rx_rms_mag),
          sizeof(int16));
  payload_size_count += sizeof(int16);

  rsp_pkt->pkt_len +=  payload_size_count; 
  ftm_rf_fbrx_meas_tx_params_rsp_pkt.payload_size = payload_size_count;

    FTM_MSG_4( FTM_HIGH, "GSM debug tx_pwr[0] =  %d, [1] = %d; rsp_pkt->pkt_len = %d, ftm_rf_fbrx_meas_tx_params_rsp_pkt.payload_size = %d",
                ftm_rf_fbrx_meas_tx_params_rsp_pkt.result[0],
                ftm_rf_fbrx_meas_tx_params_rsp_pkt.result[1],
               rsp_pkt->pkt_len,
                ftm_rf_fbrx_meas_tx_params_rsp_pkt.payload_size
               );


  prof_time = rf_time_get_elapsed( prof_tick, RF_MSEC );
  FTM_MSG_1( FTM_HIGH, "Self_test Time profiling GSM  "
                         "ExecTime %d ms", prof_time );

}


/*============================================================================

FUNCTION 
  ftm_gsm_selftest_config

DESCRIPTION
  This API sets up the environments for gsm fbrx power meas
  1) WTR fbrx path setup and de-setup
  2) MSM setup
  3) Coupler gain
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
=============================================================================*/

void ftm_gsm_selftest_config
(
  rfm_device_enum_type tx_device,  
  rfm_device_enum_type rx_device,
  boolean config
)
{
  boolean ret_val = TRUE;
  uint8 instance = 0;
  lm_status_type status;	
  ftm_gsm_cmn_vars_s* cmn_vars_ptr = NULL;
  ftm_gsm_chains_enum_type gsm_curr_chain = FTM_GSM_INVALID_CHAIN;
  ftm_gsm_chain_vars_s* chain_vars_ptr = ftm_gsm_device_get_and_update_chain_vars(rx_device);
  ftm_gsm_rfctl_vars_s * gvars = ftm_gsm_get_vars_ptr(tx_device);
  rxlm_state_cfg_params rxlm_cfg_params = {
       RFLM_DTR_RX_NUM_CARR_DEFAULT, RFLM_DTR_RX_BANDWIDTH_DEFAULT, 
       RFLM_DTR_RX_MODE_DEFAULT, RFLM_DTR_RX_TRX_INVALID, 
       RFLM_DTR_RX_DEBUG_PT_INVALID};

  sys_modem_as_id_e_type as_id = gl1_hw_rf_map_rf_sub_id_to_as_id( chain_vars_ptr->sub_id);

  /*paramaters to send to FW through mdsp */ 
  fw_rf_capture_param_t cap_param;
  fw_rf_capture_config_t cap_config;	  


  rxlm_cfg_params.mode = RFLM_DTR_RX_MODE_SELF_TEST_MODE1;
  rxlm_cfg_params.trx = RFLM_DTR_RX_TRX_WTR4905;	
  rfdevice_rxtx_common_class *device_ptr = NULL;
  
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = rfgsm_core_handle_get(tx_device);

  gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( gvars->dispatch_device );

  if( chain_vars_ptr == NULL ) 
  {
     ERR_FATAL("ftm_gsm_frame_sweep_set_band() chain_vars_ptr NULL for dispatch device %d and chain %d", gvars->dispatch_device, gsm_curr_chain, 0 );
     return;
  }

  cmn_vars_ptr = chain_vars_ptr->cmn_vars_ptr;
  if( cmn_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_frame_sweep_set_band() cmn_vars_ptr is NULL for dispatched dev %d", 
              gvars->dispatch_device, 0, 0 );
    return;
  }

  if (config)
  {

    if (!ftm_gsm_fbrx_path_buffer.is_buf_allocated)
    {
    /* if the xpt fb path not config, go ahead allocate RxLM buffer
       and config the feedback path */
      status = rflm_allocate_rx_handle(LM_GSM,LM_GSM, &ftm_gsm_fbrx_path_buffer.buf_idx); 
      if (status != LM_SUCCESS )
      {
        MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "FBRx path buffer allocate failed, status:%d",status);
        return;
      }
      else
      {
        MSG_1(MSG_SSID_FTM, MSG_LEGACY_HIGH, "FBRx buffer allocated buffer index:%d",
        ftm_gsm_fbrx_path_buffer.buf_idx);
      }
      ftm_gsm_fbrx_path_buffer.is_buf_allocated = TRUE;

    }/* if (!ftm_tdscdma_xpt_fb_path_buffer.is_buf_allocated) */
  
    /* Acquire handle lock */
    if( !rxlm_acquire_lock(ftm_gsm_fbrx_path_buffer.buf_idx) ) 
    {MSG(MSG_SSID_FTM, MSG_LEGACY_LOW,"fail to acquire lock");}

    if( !rxlm_update_static_settings(ftm_gsm_fbrx_path_buffer.buf_idx, (void*)&rxlm_cfg_params)) 
    {MSG(MSG_SSID_FTM, MSG_LEGACY_LOW,"fail to rxlm_update_static_settings");}

    if(!rfgsm_msm_update_modem_chain_rxlm_selftest(ftm_gsm_fbrx_path_buffer.buf_idx, tx_device )) 
    {MSG(MSG_SSID_FTM, MSG_LEGACY_LOW,"fail to update modem chain");}
	  
    /* Release handle lock */
    if( !rxlm_release_lock(ftm_gsm_fbrx_path_buffer.buf_idx)) 
    {MSG(MSG_SSID_FTM, MSG_LEGACY_LOW,"fail to release lock");}
	  	    
    /* Step3: Set WTR ET Path */
    if( !rfc_gsm_setup_fb_device(
          tx_device,
	  rfgsm_core_handle_ptr->tx_band,
	  cmn_vars_ptr->ftm_channel,  TRUE, 3))
    {
      MSG(MSG_SSID_FTM, MSG_LEGACY_ERROR,"fail to setup fb device");
    }

  
    /* populate parameters for capture and configuration in mdsp */ 
    cap_param.first_trig = 1;
    cap_param.last_trig = 1;
    cap_param.samp_idx = 0;
    cap_param.result_idx = 1;
    cap_param.seq_num = 0;		
    cap_config.capture_type = FW_RF_CAP_GSM_PWR_MES; 
    cap_config.num_samp = 9216; 
    cap_config.do_processing = 1;
    cap_config.rxlm_buf_id = ftm_gsm_fbrx_path_buffer.buf_idx;
    cap_config.rx_compact = 1;	
    cap_config.split_samp = 2400;
    cap_config.rx_chain = 0;
    cap_config.tx_chain = 0;
    mdsp_set_selftest_params(as_id,TRUE,ftm_gsm_fbrx_path_buffer.buf_idx,&cap_param,&cap_config);

    /* if (config) */
  }
  else /* xPT Cal Deconfig */
  {
    /* Unvote modem bus clk to 144 MHz */
    //MCPM_MCVSConfig_Release(MCPM_TDSCDMA_TECH);	
    if (ftm_gsm_fbrx_path_buffer.is_buf_allocated)
    {
      status = rflm_deallocate_rx_handle(LM_GSM,LM_GSM, (lm_handle_type)ftm_gsm_fbrx_path_buffer.buf_idx);
      if (status != LM_SUCCESS )
      {
        MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR,"feedback path buffer deallocate failed, status:%d",status);
      } 
      else
      {
        MSG_1(MSG_SSID_FTM, MSG_LEGACY_LOW,"feedback path buffer deallocated for chain:%d",tx_device);
        ftm_gsm_fbrx_path_buffer.is_buf_allocated = FALSE;
      }
    } 
    ret_val &= rfc_gsm_setup_fb_device(tx_device,
                                       rfgsm_core_handle_ptr->tx_band,
                                       cmn_vars_ptr->ftm_channel, FALSE, 3);
    FTM_MSG_MED("xPT Cal Deconfig status %d.",ret_val,0,0);
	
  }/* xPT Cal Deconfig */
  return;	
}

/*============================================================================

FUNCTION 
  ftm_gsm_selftest_write_capture_params_to_commonFW

DESCRIPTION
  This API writes params for capture to cfw
  
DEPENDENCIES
  None

RETURN VALUE
  boolean
  
SIDE EFFECTS
  None
=============================================================================*/

boolean ftm_gsm_selftest_write_capture_params_to_commonFW(rfm_device_enum_type  device, uint8 cap_type_sw)
{
  boolean ret_val = TRUE;
  ftm_gsm_chain_vars_s* chain_vars_ptr = ftm_gsm_device_get_and_update_chain_vars(device);
  sys_modem_as_id_e_type as_id = gl1_hw_rf_map_rf_sub_id_to_as_id( chain_vars_ptr->sub_id);
  boolean enable;
  uint32 buf_index;
  fw_rf_capture_param_t *cap_param;
  fw_rf_capture_config_t *cap_config;
  
  mdsp_get_selftest_params(as_id, &enable, &buf_index, &cap_param, &cap_config);
  ret_val &= rfcommon_mdsp_setup_capture_params(cap_config->tx_chain,                              
                                                cap_config->rx_chain,                              
                                                buf_index, 
                                                cap_type_sw,
                                                cap_config->rx_compact,
                                                cap_config->num_samp,                                 
                                                cap_config->split_samp,
                                                cap_config->do_processing);
  return ret_val;
}
#endif

/*----------------------------------------------------------------------------*/
#endif //FEATURE_GSM
#endif // FTM_HAS_UMTS
#endif //FEATURE_FACTORY_TESTMODE
