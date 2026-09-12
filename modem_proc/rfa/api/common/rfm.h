#ifndef RFM_H
#define RFM_H

/*
   @file
   rfm.h

   @brief
   RF Driver's common external interface file.

   @details

*/

/*===========================================================================
Copyright (c) 2002 - 2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfa.mpss/4.11/api/common/rfm.h#2 $

when       who     what, where, why
-----      --      ------------------------------------------------------
09/02/22   tej     FR69305 To add support for new error codes for QMI_DMS_SET_OPERATING_MODE
02/05/20   hari    [CR 2616173] Removed unwanted qsh client header
01/27/20   hari    [CR 2610528]moved rf_bandset_info_type from rfm.c to rfm.h
01/24/20   moh     RF MDUMP Changes
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
10/30/17   pv      Unified band mask support to Policyman
05/03/16   hm      Added a new API 
                   rfm_get_sys_lte_band_mask_from_rflte_band_mask()
01/15/16   dyc     Added new data types for SAR forced antenna support
09/21/15   sk      Make AFC TxLM Handle based
06/23/15   rp      Added Tabasco Spcific Feature to Dump Code.
06/23/15   rp      Adding API that can be called from FW to dump WTR GRFC
                   RXLM and TXLM regs
05/19/15   zhw     Added rfm_get_aggregated_band_mask() support
05/14/15   stw     Added support for 256 lte band mask check
04/20/15    sk     Inter-band UL CA support 
03/13/15   ska     Add support for DGLNA feature
10/20/14   vbh     Add a debug routine that prints WTR registers
09/24/14   pl      Added get_rx_lo_freq support
09/10/14   vv      Added RFM therm API support
02/18/14   sc      Remove unused code and dependencies
10/30/13   svi     Added support for TxPLL AFC
09/05/13   pl      Added Asdiv init default switch position API
08/14/13   ak      phone enter to "offline" with QRD RP4 RF card on TR1.1 CRM build
07/23/13   aro     Added inteface to do immediate write
07/22/13   aro     Added immediate flag for rfm_set_antenna()
07/18/13   aro     CB Pointer fix
07/17/13   aro     Moved the L1 callback definition to rfm.c
06/05/13   kb      [XPT] Support to update rfm_mode for TDS in online mode
05/31/13   pl      Added API for Antenna Switch Diversity Feature
03/11/13   sar     Updates for APQ, GNSS only target.
01/21/13   nrk     Changed input param type path to device for rfm_get_synth_lock_status
01/04/13   gvn     Move interface to query RF LTE CA bc config to rflte_mc
11/28/12   nrk     Added API to get SYNTH lock  
11/15/12   adk     Backed out Qtuner code
08/16/12   ndb     Change antenna tuner settings api to be customizable 
10/25/12   gvn     Interface for LTE CA bc config
07/26/12   vrb     Move TRM interface prototype to rfm_device_config_type.h
05/25/12   kb/npi  Updated the rfm_init() state using an enum
05/21/12   aca     AFC interface cleanup
04/13/12   kai     Revert the following update_txlm_buffer change since gsm layer1 dependence not ready
04/13/12   kai     Move update_txlm_buffer from enable_tx to init_tx, remove txlm_buf_idx from disable_tx 
03/08/12   sar     Exported rfm_is_rfm_init_success() for RCINIT. 
02/09/12   aca     Tx PLL error correction
12/14/11   sbm     added rfm_get_rx_freq_from_rx_earfcn() function.
11/07/11   aak     Changes to implement Quiet mode  
10/13/11   whc     Adding RFM layer function to return the current PA state. 
09/28/11   sb      Mainline FEATURE_RF_COMMON_LM_RFM_INTERFACE and FEATURE_RF_WCDMA_LM_RFM_INTERFACE.
08/12/11   jhe     Added support for TDS band mask
06/30/11   Saul    Compile error fix for CL 1817866.
06/14/11   aro     Updated TRM interface to return data validity
04/28/11   aro     Removed number of devices in TRM interface as this info
                   is redundant and TRM can deduce this based on other param
04/26/11   aro     Added number of devices in TRM interface
04/22/11   aro     Doxygen Documentation Update
04/22/11   aro     [1] Updated Device Configuration interface to have reduced
                   mask length for tech and antenna.
                   [2] Added enum to perform masking on multi-tech support
04/21/11   aro     Added Interface for TRM to get Device Configuration
04/20/11   dw      Temporarily add back lm buffer indexes
04/19/11   pl      Remove Warnings
04/19/11   bmg     Fixed extern "C" locations
04/19/11   aro     Added prototype for Get Rx Wakeup function
04/07/11   sar     Relocated to rfa/api to conform to CMI, Removed rfm_helper_incs.h 
                   and included files previously exported thru this file.
02/28/11   tnt     Merge to MDM9K PLF110 
02/25/11   ka/dw   Merge support for thermal mitigation  
02/17/11    av     Reverting GP_CLK changes 
02/02/11   av      Disable GP_CLK 
01/28/11    dw     RFM interface change for TxLM
01/05/10    dw     RFM interface change for RxLM
11/29/10   dbz     Added GSM fast scan notification API
10/14/10   mkv/whc Integrating Reload RFNV from SCMM into MDM9x00 (CL 1315259)
09/23/10   sty     Reverted previous change
09/22/10   sty     Added rfm_init()
08/04/10    av     Update to LTE B13 GPS algorithm 
06/30/10    ap     Added support for Power Collapse & Early clock enable 
06/28/10   can     Removing Power collapse changes that cause a crash.
06/24/10   can     Support for LTE BC Config.
06/22/10    ap     Added support for Power Collapse 
05/21/10   lcl     Back out clock changes.  Wasn't broken GSM.
05/21/10   tws     Add API function to convert sys_band to rf_card_band.
04/01/10   adk     Merged branch scmm_mdm9k_merge_final
03/01/10   kma     Added power collapse status API
02/16/10   sty     Added prototype for rfm_get_rx_warmup_time()
10/22/09   dyc     Mode specific build support.
10/22/09   wen     Fix compiling warnings 
10/16/09   jhe     Added rfm_power_clock_on/off
08/31/09   bmg     Moved truly common functions to rfm_common.h
03/12/09   sar     Added changes needed for 1x
10/21/08   rmd     QSC7X30 code clean up REV1: Mainline/Removed features 
                   (FEATURE_..., RF_HAS_.. and T_MSM...)
08/18/08   adk     Moved function prototype rfm_get_tx_carrier_freq() from rf1x.h
07/14/08   adk     Ported DORB changes from the 7800 branch.
06/04/08   ra      move rfm_gps_pause() and rfm_gps_resume() to rfm file
06/11/08   ad      Added desciption for ADC return value in rfm_adc_read()
05/08/08   dw      Fixed two function prototypes.
05/07/08   ad      Export generic ADC read functionality through RFM API
05/07/08   jfc     Fixed prototypes for rfm_get_tx_pwr_limit()
04/24/08   dw      Added rfm_get_cgagc_settling_time_for_tune API to support both tune scenarios.
12/07/07   jfc     Add update_pavmode_burst_data() API for GL1
11/14/07   jfc     Export API for getting switchpoints
08/15/07   Vish    Renamed rfm_get_rx_agc_db256() to rfm_get_rx_agc(), and added
                   rfm_get_tx_[adjust/open_loop/pilot_power/total_power] APIs.
08/09/07   ad      Added support for reading HS LNA switch point NVs during HS calls.
08/06/07   jfc     Featurized code for EDGE-only builds
08/06/07   bmg     Updated rfm_enable_diversity comments and return type
08/01/07   Vish    Added rfm_get_rx_agc_db256() and two associated macros -
                   RFM_GET_RX[0/1]_AGC_DB256().

07/12/07   jfc/lcl Added lna_range param for set_rx_gain()
06/25/07   Vish    Added rfm_enable_diversity().
                   Moved rfm_get_sub_class(), and rfm_get_rx_carrier_freq()
                   from "rf1x.h".
05/08/07   jfc     Fixed rfm_band_supports_rxd() to take rfi, not rfcom band type
02/28/07   ycl     Fix compiler warnings.
02/27/07   jfc     Cleaned up rfm_ioctl() headers
02/12/07   jfc     Added channel type parameter to set_tx_general_ctl()
02/05/07   ycl     Merge to 7200 tip.
01/30/07   jfc     Put rfm_get_rf_warmup_time() back
01/23/05   ra      Add support for rfm_trigger_gps_bbc_update()
01/17/07   jfc     Fixed featurization and cleaned up declarations.
01/17/07   jfc     Wrapped UMTS code with RF_HAS_FEATURE_WCDMA
01/11/06   ra      Added rfm_exit_gps
12/18/06   jfc     Moved rfm_is_sleeping to RFM layer
12/16/06   jfc     Added API for antenna for TRM layer
11/16/06   jfc     Removed warnings about deprecated declarations. Changed
                    rfm_get_band_mask() to take device type
10/31/06   ycl     Added rfm_get_mdsp_tx_power_limit() and 
                   rfm_get_mdsp_tx_agc() functions. 
10/17/06   ycl     Modifications for initial MSM7600 build.
04/12/06    bn     Pass down appropriate dev_ptr based on the device
04/03/06    dp     Add RxD query function to check for diversity capabilities
                   based on band.
03/16/06   jfc     Added rfm_enable_data_mover_clk_for_cm()
03/09/06   adm     Added API for returning RF warmup time to L1.
03/10/06    xw     Deleted rfm_band_enum_type.
03/06/06   rsr/rv  Added support for multislot power back off.
03/06/06   rsr     Mainlined T_MSM6280.
03/06/06    bn     Added multi chain RF API support.                   
02/17/06    xw     Included bsp.h and removed unused rfm_band_enum_type. 
01/17/06    xw     Added rfm_get_max_tx_power_nv_item() and rfm_set_tx_general_ctl().
01/13/06    xw     Added egprs_mode in rfm_set_amam_profile().
01/11/06    xw     Added new APIs to enforce other function area to go thru RFM.
11/15/05   jfc     Added new interfaces for GtoW, WtoG, and WtoG operation
11/04/05   adm     Merged changes from 6275 tip.
07/29/05   ycl     Correct rx agc interface funtion prototypes.
07/14/05   ycl     Added interface for initial prach tx min power.
04/27/05   jtn     Added extern to prevent compiler warning.
04/07/05   ycl     Add interface to get serving and away rx agc values.
03/29/05    xw     Added rfm_get_band_mask().
01/04/05    rv     Updated comments for  rfm_get_tx_pwr_limit().
12/17/04    rv     Added rfm_get_tx_pwr_limit
08/09/04    eh     Added rfm_band_enum_type.
05/11/04   dhh     Added AFC support.
03/10/04    eh     Modified trkloadj and rotator functions.
06/20/03    eh     Wrapped freq. maint. functions under FEATURE_FREQ_VS_TEMP_NV_UPDATE
05/07/03   thh     Include rfgsm.h instead of rfmgsm.h.  Rfmgsm.h is obsolete.
03/27/03   rv      Added MM API for frequency maintenance algorithm in WCDMA.
03/05/03   eh      Modified rfm_init to pass in a state variable. This state
                   indicates whether RF NV is valid or not. If it is invalid
                   then returns false, otherwise returns true.
12/13/02   sd      Removed temporary GSM synth tuning types (MON and PWR) for
                   non CDMA1X builds.
11/04/02   sd      Added support for intersystem measurements.
10/10/02   sar     Added support for cdma 1x mode.
09/10/02   sd      Added support for multimode API that includes GSM drivers.
07/01/02   sd      Initial revision.

============================================================================*/

#include "rfcom.h"
#include "rfm_types.h"
#ifndef FEATURE_GNSS_ONLY_NO_WWAN
#include "db.h"
#endif
#include "rfumts.h"
#include "rfllgsm.h"
#include "rfm_common.h"
#include "rfm_device_config_type.h"
#include "rfm_init_event.h"
#include "policyman_rf_bandset.h"

//#ifdef FEATURE_QSH_MDUMP
//#include "rfcommon_qsh_client.h"
//#endif


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RFM_MAX_NUM_FREQ_LIST 3

/*----------------------------------------------------------------------------*/
/*! Type defintion for L1 Call back function */
typedef void(*rfm_l1_cb_type)(void *cb_data);

typedef struct
{
  uint8 num_valid_freq;
  uint32 frequency_list[RFM_MAX_NUM_FREQ_LIST];
} rfm_frequency_list_type;

/*----------------------------------------------------------------------------*/
/*
  Enum to indicate whether RFM initialization was done and if so, whether
  this was successful
*/
typedef enum
{
  RFM_INIT_FAILED,                /* rfm_init() ran but failed    */
  RFM_INIT_SUCCEEDED,             /* rfm_init() ran and succeeded */
  RFM_INIT_NOT_RUN,               /* rfm_init() did not run       */
} rfm_init_state_enum_type;

/*----------------------------------------------------------------------------*/
/*
  Enum to indicate the antenna which is "forced" to be enabled from SAR
*/
typedef enum
{
  RFM_SAR_ANTENNA_NONE,                /* no SAR forced selection */
  RFM_SAR_ANTENNA_CONFIG_0,            /* pass through position */
  RFM_SAR_ANTENNA_CONFIG_1,            /* swapped position */
  RFM_SAR_ANTENNA_MAX,                 /* enum limit for ant conditions */
} rfm_sar_forced_antenna_enum_type;

/*----------------------------------------------------------------------------*/
/*! This enum lists the types of Simultaneous RF Operation. */
typedef enum
{

  RFM_SIMULTANEOUS_RX,
  /*!< Simultaneous Receiver */ 

  RFM_SIMULTANEOUS_TX
  /*!< Simultaneous Transkmitter */ 

} rfm_simultaneous_op_enum_type;


/*! therm device handle for external use*/
typedef uint8 rfm_thermistor_handle_type;


/*! temperature result type */
typedef struct
{
  /*variable to store the read temperature in dgree celcius */
  int16 therm_read_deg_C;

}rfm_thermistor_result_type;

/*! This structure contains therm handles and any other associated data that may be used in the future by the thermal algorithm 
    to improve their performance */
typedef struct
{
  rfm_thermistor_handle_type handle;

  /*To identify the thermistor withtin the chip when you have multiple thermistors per chip.*/
  uint8 thermistor_within_chip;
  /*Device info of the therm device*/
  uint16 mfg_id; /*! RFFE Manafacturer ID  */
  uint16 prd_id; /*! RFFE Product ID */
  uint16 rev_id; /*! Revision ID */

}rfm_thermistor_type;

/*! This structure contains generic rfm input paramters */
typedef struct
{
   rfcom_device_enum_type device;
   rfcom_mode_enum_type   rfmode;
}rfm_input_parm_type;

typedef rfm_input_parm_type rfm_sar_forced_antenna_input_param_type;

/*!--------rfm therm APIs for external use----------*/

/*! 
  @brief
  Structure to hold the tech properties in CRAT cases.
*/
typedef struct 
{
  /*! Tech that is active, */
  rfm_mode_enum_type   tech              ;

  /*! Band the tech is on  */
  uint32               band              ;

  /*! RFM device for primary  chain */
  rfm_device_enum_type primary_device    ;

  /*!  RFM device for secondary chain, Set to RFM_INVALID_DEVICE 
    if diversity is inactive */
  rfm_device_enum_type secondary_device  ;

 /*! More devices can be added if needed */
} rfm_tech_descriptor_type ; 

/*----------------------------------------------------------------------------*/

/*!
  @brief
  Structure to hold the tech properties in CRAT cases.
*/
typedef struct
{
  /*! Overall status of rfm initialization
    TRUE if rfm initialization is successful, otherwise FALSE */
  boolean   rfm_init_status;              

  /*! Failure reason for RF initialization  */
  rfm_init_status_type_e_type rfm_init_failure;

 /*! More devices can be added if needed */
} rfm_init_status_type ;


/*----------------------------------------------------------------------------*/


/*! Consolidates rf_bandset items used to communicate supported bands by 
    RF's WWAN techs to external clients . */
typedef enum
{
  RF_BANDSET_INIT_PENDING, /*!< Init not yet done. */
  RF_BANDSET_INIT_SUCCESS,
  RF_BANDSET_INIT_FAILED
} rf_bandset_init_status;

/*----------------------------------------------------------------------------*/
/*! Consolidates rf_bandset items used to communicate supported bands by 
    RF's WWAN techs to external clients . */
typedef struct
{
  /*! Indicates data validity */
  rf_bandset_init_status init_status;

  /*! Contains supported bands data */
  policyman_rf_bandset_t *rf_bandset;
} rf_bandset_info_type;

/*----------------------------------------------------------------------------*/

/*! 
  @brief
  Structure to hold DGLNA attributes. rfm_dglna_set_attributes()
  can be called to set the params and pass it to RF.
*/
typedef struct 
{
  /*! Flag to enable or disable DGLNA feature. If this flag is 
    FALSE/0, all the remainder of the fields are dont care. This
    flag is useful in disabling DGLNA feature. This also can
    tell RF that this is a single sim or a platform where DGLNA
    feature is not used on. rfm_dglna_set_attributes() can be
    called at the reservation time to set/reset this flag. The
    longetivity of this phone is from bootup to power down. By
    default the flag will come up as FALSE at power up.
    rfm_dglna_set_attributes() can then be used to manipulate
    the flag. */
  boolean                   feature_dglna_disable ;

  /*! This flag will be set to indicate is DGLNA is enabled or 
    not. If this flag is enabled, the other fields of these
    structure must be valid. */
  boolean                   enable_dglna ; 

  /*! Tech1 band, PRx, DRx info   */
  rfm_tech_descriptor_type  tech1_info  ;

  /*! Tech2 band, PRx, DRx info   */
  rfm_tech_descriptor_type  tech2_info  ;
} rfm_dglna_descriptor_type ;

/*----------------------------------------------------------------------------*/

/*!
  @brief
  Structure to hold the parameters to be used with rfm_update_maat_info API.

*/
typedef struct
{
  /*! technology providing the maat information*/
  rfcom_mode_enum_type rfmode;

  /*! tech specific data */
  void *tech_payload;
  
}rfm_maat_params_type;


/*----------------------------------------------------------------------------*/
/*!
  @brief
  rfm API to get the active number of thermistors
 
  @details
  rfm API to get the active number of thermistors

  @param   size : out parameter. Returns the number of active thermistors

*/
boolean rfm_get_thermistor_count(uint8* size);


/*----------------------------------------------------------------------------*/
/*!
  @brief
  rfm API to enumerate and return the active thermistors for external use
 
  @details
  rfm API to enumerate and return the active thermistors for external use

  @param   size : in+out parameter. It specifies the number of entries provide in the array by the caller, 
                  and is updated to the number of thermistors that have been filled in after the call
  @param   therm_device_list : out parameter.Array of structures containing handles and any other associated data 
                      that may be used in the future by the thermal algorithm to improve their performance

*/
boolean rfm_enumerate_thermistors(uint8* size, rfm_thermistor_type* therm_device_list);


/*----------------------------------------------------------------------------*/
/*!
  @brief
  rfm API to read and return the temperature for the requested thermistor 
 
  @details
  rfm API to read and return the temperature for the requested thermistor 

  @param  handle : in parameter to specify the thermistor to read temperature
  @param  result : out parameter which will be updated with the therm read value if the read was successfull

*/
boolean rfm_query_thermistor(rfm_thermistor_handle_type handle, rfm_thermistor_result_type* result);

/*!-------------------------------------------------*/


/*----------------------------------------------------------------------------*/
uint32 rfm_tune_to_chan ( rfcom_device_enum_type device, uint16 channel,
                          rfm_synth_tune_enum_type synths,
                          const void *user_data_ptr,
                          rfm_cb_handler_type cb_handler);

/*----------------------------------------------------------------------------*/
uint32 rfm_enable_tx ( rfcom_device_enum_type device,
                       const void *user_data_ptr,rfm_cb_handler_type cb_handler
                       ,uint32 txlm_buf_idx
                       );

/*----------------------------------------------------------------------------*/
void rfm_disable_tx (rfcom_device_enum_type device
                     ,uint32 txlm_buf_idx
                    );

/*----------------------------------------------------------------------------*/
void rfm_set_quiet_mode (rfm_mode_enum_type mode, boolean enable);

/*----------------------------------------------------------------------------*/
boolean rfm_is_sleeping( rfcom_device_enum_type device );

/*----------------------------------------------------------------------------*/
uint32 rfm_sleep( rfcom_device_enum_type device, const void *user_data_ptr,
                  rfm_cb_handler_type cb_handler 
                  ,uint32 rxlm_buf_idx
                  );

/*----------------------------------------------------------------------------*/
uint32 rfm_wakeup ( rfcom_device_enum_type device, rfm_mode_enum_type rf_mode,        
                    const void *user_data_ptr, rfm_cb_handler_type cb_handler
                    ,uint32 rxlm_buf_idx
                  );     

/*----------------------------------------------------------------------------*/
uint32 rfm_enable_rx ( rfcom_device_enum_type device, const void *user_data_ptr,            
                       rfm_cb_handler_type cb_handler);

/*----------------------------------------------------------------------------*/
void rfm_disable_rx ( rfcom_device_enum_type device );

/*----------------------------------------------------------------------------*/
const policyman_rf_bandset_t *
rfm_get_supported_rf_bandset
(
  void
);

/*----------------------------------------------------------------------------*/
void rfm_multimode_handover (rf_multimode_handover_cmd_type chain_0_cmd,
                             rf_multimode_handover_cmd_type chain_1_cmd);

/*----------------------------------------------------------------------------*/
uint64 rfm_get_band_mask ( rfcom_device_enum_type device );

/*----------------------------------------------------------------------------*/
uint64 rfm_get_aggregated_band_mask ();

#ifdef LTE_BAND_NUM 
/*----------------------------------------------------------------------------*/
sys_lte_band_mask_e_type rfm_get_lte_band_mask ( void ); 
sys_lte_band_mask_e_type 
 rfm_get_sys_lte_band_mask_from_rflte_band_mask ( uint64 rflte_band_mask );

#else 
/*----------------------------------------------------------------------------*/
uint64 rfm_get_lte_band_mask ( void );
#endif 

/*----------------------------------------------------------------------------*/
uint64 rfm_get_tds_band_mask ( rfcom_device_enum_type device );

/*----------------------------------------------------------------------------*/
rfcom_ioctl_return_type rfm_ioctl ( rfcom_device_enum_type device,
                                    rfcom_ioctl_param_type request,
                                    void *pbuf,
                                    int32 length );

/*----------------------------------------------------------------------------*/
rfcom_adc_err_enum_type rfm_adc_read(rfcom_adc_logical_channel_type adc_addr, 
                                     uint16 *adc_val);

/*----------------------------------------------------------------------------*/
#if 0

uint32 
rfm_get_rx_warmup_time    \\Already defined below
( 
  rfm_device_enum_type device, 
  rfm_mode_enum_type   rf_mode
);

#endif
/*----------------------------------------------------------------------------*/
rf_card_band_type rfm_convert_sys_band_to_rf_card_band( sys_band_class_e_type );

/*----------------------------------------------------------------------------*/
void rfm_gnss_tx_band(rfcom_lte_band_type rf_tx_band); 
                                                       
/*----------------------------------------------------------------------------*/                                                       
void rfm_gnss_tx_indicator(boolean enable);

/*----------------------------------------------------------------------------*/
boolean 
rfm_reload_nv 
( 
  rex_tcb_type *caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (*task_wait_func_ptr)( rex_sigs_type )
);

/*----------------------------------------------------------------------------*/
void rfm_reduce_mptl_db_for_therm( uint8 backoff );

/*----------------------------------------------------------------------------*/
uint32 
rfm_get_rx_warmup_time 
( 
  rfm_device_enum_type device, 
  rfm_mode_enum_type   rf_mode
);

/*----------------------------------------------------------------------------*/
uint8 rfm_get_current_pa_state(void);

/*----------------------------------------------------------------------------*/
uint32 rfm_get_rx_freq_from_rx_earfcn(uint16 rx_chan, rfcom_band_type_u band, rfm_mode_enum_type mode);

/*----------------------------------------------------------------------------*/
uint32 rfm_get_tx_freq_from_tx_earfcn(uint16 tx_chan, rfcom_band_type_u band, rfm_mode_enum_type mode);

/*----------------------------------------------------------------------------*/
void rfm_get_txpll_script( uint8 sub_id,
                           rfm_mode_enum_type mode,
                           int32 rx_freq_error_in_hz, 
                           void* txpll_script_buf_ptr);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  rfm API to populate the TxPLL scripts in DM buffer for the frequency error for 
  a particular cell/carrier 
 
  @details
  This is a new interface where each Tech ML1 should pass the UL carrier information
  along with rx frequency error.

  @param  mode : TECH mode for which the api is called
  @param  carrier_idx : Carrier/Cell index for which TxPLL freq error should be compensated
  @param  txlm_handle : TxLM Handle for which AFC correction should happen
  @param  rx_freq_error_in_hz : Rx frequency error on the current carrier/cell
*/
  void rfm_get_cell_txpll_script_v2( uint8   sub_id,
                                     rfm_mode_enum_type mode,
                                     uint32 carrier_idx,
                                     uint32 txlm_handle,
                                     int32 rx_freq_error_in_hz);
  
/*----------------------------------------------------------------------------*/
void rfm_get_cell_txpll_script( uint8   sub_id,
                                rfm_mode_enum_type mode,
                                uint32 carrier_idx,
                                int32 rx_freq_error_in_hz);

/*----------------------------------------------------------------------------*/
void rfm_get_rxpll_script( uint8   sub_id,
                           rfm_mode_enum_type mode,
                           void* rxpll_script_buf_ptr);

/*----------------------------------------------------------------------------*/
void rfm_power_clock_off(void);
/*----------------------------------------------------------------------------*/
void rfm_power_clock_on(void);
/*----------------------------------------------------------------------------*/

extern boolean rfm_is_rfm_init_success
(
  void
);

/*----------------------------------------------------------------------------*/
boolean rfm_get_calibration_state(void);

/*----------------------------------------------------------------------------*/

boolean rfm_deinit (void);

/*----------------------------------------------------------------------------*/
void
rfm_get_synth_lock_status
(
  rfm_device_enum_type device,
  rfcom_mode_enum_type rf_mode,
  rfcom_synth_type synth_type,
  void *is_synth_locked
);

/*----------------------------------------------------------------------------*/
boolean
rfm_set_antenna
(
  rfm_device_enum_type device, 
  uint32 ant_sw_pos,
  rfm_l1_cb_type cb_func,
  void *cb_data
);

/*----------------------------------------------------------------------------*/
boolean
rfm_set_antenna_imm
(
  rfm_device_enum_type device, 
  uint32 ant_sw_pos,
  rfm_l1_cb_type cb_func,
  void *cb_data
);

/*----------------------------------------------------------------------------*/
boolean
rfm_set_antenna_abort
(
  rfm_device_enum_type device, 
  uint32 ant_sw_pos,
  rfm_l1_cb_type cb_func,
  void *cb_data
);

/*----------------------------------------------------------------------------*/
boolean
rfm_init_antenna
(
  uint32 ant_sw_pos
);

/*----------------------------------------------------------------------------*/
void rfm_mode_update(const rfcom_device_enum_type device, const rfcom_mode_enum_type rf_mode);

/*----------------------------------------------------------------------------*/


boolean rfm_is_tech_supported(rfcom_device_enum_type device, rfm_mode_enum_type tech);


boolean rfm_is_coex_backoff_req (rfm_device_enum_type rfm_dev, rfm_mode_enum_type tech);

uint32 rfm_get_rx_lo_frequency(rfm_mode_enum_type tech, rfm_frequency_list_type freq_list);

void rfm_read_wtr_regs(uint8 register_set);

void rfm_read_asm_regs(void);

boolean rfm_dglna_set_attributes (rfm_dglna_descriptor_type dglna_info);

/*----------------------------------------------------------------------------*/
rfm_sar_forced_antenna_enum_type
rfm_get_sar_forced_antenna
(
  rfm_sar_forced_antenna_input_param_type *input_parm
);
/*----------------------------------------------------------------------------*/


extern rfm_init_state_enum_type rfm_init_get_state
(
  void
);
#ifdef FEATURE_QSH_MDUMP
extern rfm_init_state_enum_type rfm_init_get_state_mdump
(
  void
);
#endif

/*----------------------------------------------------------------------------*/
boolean
rfm_bands_bitmask_bit_is_set(
  const rfm_bands_bitmask* mask,
  sys_band_class_e_type band
);

/*----------------------------------------------------------------------------*/
void
rfm_bands_bitmask_set_bit(
  rfm_bands_bitmask* mask,
  sys_band_class_e_type band
);

/*----------------------------------------------------------------------------*/
void
rfm_bands_bitmask_clear_bit(
  rfm_bands_bitmask* mask,
  sys_band_class_e_type band
);

/*----------------------------------------------------------------------------*/
boolean rfm_update_maat_info
(
  rfm_maat_params_type *maat_params
);


#ifdef FEATURE_TABASCO_MODEM

void rfm_read_grfc_regs(void);

void rfm_dump_regs (uint8 dump_mode_mask);

#endif

#ifdef FEATURE_QSH_MDUMP
void rfm_qsh_dump_data(void);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* RFM_H */

