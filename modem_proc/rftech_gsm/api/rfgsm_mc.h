#ifndef RFGSM_MC_H
#define RFGSM_MC_H

/*!
   @file
   rfgsm_mc.h

   @brief


   @details

*/

/*===========================================================================
Copyright (c) 1999 - 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:10:17 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_mc.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/08/16   dsm     Report to RFM layer to disable asdiv switching on specific sar code
10/27/15   cs      Mutex protect iRAT meas enter/exit to avoid overlap
09/10/15   sc      Deprecate core handle cmd_buff log and use cmd_proc log
08/21/15   sc      Deprecate subscription 3 reducing memory footprint
08/18/15   ec      Deprecate unused API to update SAWless lin state in GL1 params
07/03/15   sc      Fix LIF and enable optimised Rx burst processing
                   Temporarily compiled out old functionality
07/02/15   ec      Update API declaration to pass GL1 SAWless support to and from MC layer
06/16/15   zbz     Return Tx/Rx wake up CCS time profiling to L1
06/03/15   sg      Code cleanup for edge core
05/28/15   sc      Enable thermistor and vbatt reads
05/13/15   sc      Move Rx burst checks and logic from command proc to mc
05/12/15   zgl     Changes for FR 24616: Algorithm to mitigate VBatt droop (down to 2.5V) via TX backoff
05/07/15   sc      Implement command processing log
04/20/15   sc      Populate RF sequence number passed from L1 in RFLM DM
04/17/15   ggs     Pass back Tx power in  dBm100 to GL1 in Tx confirmation
04/14/15   piy     Fixed bug in Device status read feature for Mon Burst
03/25/15   sc      Add Start/Stop MC APIs
03/25/14   zbz     Update Rx alpha values for every wake up
03/24/15   sc      Moved from rfa to rftech_gsm
03/02/15   piy     Added Macros for Wakeup, Sleep, Rx & Tx burst Time Profiling
02/27/15   sc      Ensure NULL NV error checking when processing Rx/Tx band
01/30/15   sw      GL1->RF handshake mechanism to handle ASD response error cases
12/02/14   sml     Read WTR or RF front end devices status in between Rx/Tx
                   burst along with GRFC status
11/25/14   sw      Disable therm read during WTR dedicated hopping
10/21/14   sc      Reduce code execution limiting use of get qsym API
10/06/14   sc      Deprecate unused functionality
10/01/14   tsr     GSM IDLE Mode RXD support
08/19/14   zbz     Add RF GSM CCS execution time profiling
07/30/14   hoh     Add enhanced rx feature support
07/28/14   ec      Correct mc layer burst processing API
07/08/14   ec      Function prototype for MC layer API to process burst metrics
06/18/14   tsr     RXD diversity call flow changes to enable DRX path along with PRX path
06/10/14   hk      Edge core parameter from GL1.
05/28/14   sw      Move therm read from enter mode to tx enable
05/27/14   sb      Changes to add RFM APIs for GSM arfcn to frequency conversion
03/31/14   tsr     Added support for handling combined Enter Mode for PRX and DRX
03/26/14   sb      Add support for IP2 cal for Dime PM
03/11/14   tsr     Added RXD support for RX burst
03/05/14   sw      Remove GFW MSGR framework as its now replaced with RFLM MSGR
03/05/14   sc      Fix oft build
02/26/14   sc      Pass core data through cmd processing and mc layer
02/18/14   tsr     GSM RX Diversity
02/18/14   sc      Remove unused code and dependencies
02/11/14   sw      Remove therm read Triton featurisation
01/30/14   sw      Add framework to receive msgs from RFLM tech (therm read)
01/08/14   cj      Added rfgsm_mc_update_buffer_id API
07/29/13   sml/svi ASD support for GSM tech
07/24/13   ec    Pass rf_task into MC layer for logging, make parameters consistent
07/12/13   sc      Add process Tx burst API
07/12/13   av      GTA EXIT mode
06/25/13   ec      Make changes to support Therm Read Cancellation
06/20/13   ec      Add support for GSM Tune Away enter mode
05/30/13   tws     Add support for GSM sub 3
05/30/13   tws     Make the GFW msg queue rf task specific
04/09/13   tsr     Added RF APPS->RF Task MSGR framework for temp comp to make
                   temp comp data thread safe
03/12/13   tsr     Added to tag to each therm read event handle to identify the
                   successful retrieval
02/25/13   sc      Added cm enter/exit to init/de-init iRAT during gl1 cm init/terminate
02/19/13   sc      Added APIs to allow GL1 to alloc and free Rx meas buffers
02/12/13   sr      changes to free the memory alloocated during rfm_init()
01/31/13   tsr     Added framework for recieving therm read response from GFW
01/11/13   aka     Added support for simultaneous Tx SAR
01/10/13   av      Create new REQ and CNF ids for subscriber 2 and handle them in rf_Task_2
01/09/13   sr      Made changes to run vbatt/temp comp in rf app task context.
01/03/12   sc      Added therm read full call flow framework
01/3/13    av      Changes to support chain2 for GSM
12/18/12   sc      Added therm read event generation framework
12/14/12   sc      Removed support for RF Task iRAT startup and cleanup
11/21/12   av      Added rfm_device parameters to mc/core/mdsp/msm layers.
10/12/12   sc      Added GSM mc iRAT meas build scripts API prototype
10/11/12   sc      Added prepare Rx burst API for inline GL1 iRAT readiness
10/11/12   sc      Cleanup up unrequired Rx burst & SBI/Non-SBI API burst buffer params
10/05/12   av      Expanded rf task architecture to include more tasks.
09/25/12   tc      Mainline RF_HAS_RFA_GSM_TASK.
08/19/12   sr      Added cplusplus diretive to include in cpp files.
08/06/12   sc      Edit API to update CCS header FW vars
08/02/12   sc      Add num slots to setup tx burst API
07/26/12   sr      Changes to APIS to take rfm_dev as parameter.
07/06/12   sc      Edit API parameters to support CCS buffer format
07/06/12   sc      Remove unrequired num transactions init functionality
06/20/12   sc      Add write to hw flag for set tx band to enable varied scenario calls
06/07/12   sc      Added Fast Scan status prototype for RFGSM MC scope
04/23/12   sc      Support for Rx/Tx Burst, Tx Band, Tx Power shared memory interface
04/04/12   ggs     Added rfgsm_mc_override_pa_range to provide a cleaner top-down
                   call flow to RF driver APIs from FTM mode
02/13/12   ggs     Added RF GSM FTM Task dispatch init/de-init function prototypes
02/07/12   sc      Added RF Task dispatch init/de-init function prototypes
01/24/12   sc      Added GSM msgr registration and deregistration function prototypes
11/29/11   sb      Added functionality for GSM IQ CAPTURE
10/27/11    sr     made changes to trigger SAR state change for GSM.
10/19/11   sb      Apply therm read in idle frame and enable FTM temp comp and vbatt comp during idle frame
03/21/11    sr     Added LM support and timing info access interface.
10/14/10   mkv/whc Integrating Reload RFNV from SCMM into MDM9x00 (CL 1315259)
09/21/10   ggs     Added new api prototype to return rf_warmup time
03/19/10   lcl     Pull in changes from QSC6295
29/01/10   sr      Changed the <tech>_mc_init() signature to fix warnings in
                   <tech> specific builds
01/28/10    sr     added prototypes for power_clock_on/off
11/30/09   bmg    Added common_init_successful parameter to GSM initialization
                  Changed init function return to boolean
03/13/09   sr     code cleanup for SCMM build
10/14/08   sr     Initial version.

============================================================================*/


#include "rfcom.h"
#include "msgr_types.h"
#include "rfm_gsm.h"
#include "rfgsm_msg.h"
#include "rfgsm_core_timing_consts.h"
#include "rfmeas_types.h"
#include "rfgsm_core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/*=============================================================================

                           DEFINITIONS AND TYPES

=============================================================================*/
typedef enum
{
  RFGSM_MC_ERROR,
  RFGSM_MC_SUCCESS,
  RFGSM_MC_INVALID_PARAM,
  RFGSM_MC_CORE_API_FAILURE,
  RFGSM_MC_INVALID_MC_STATE,
  RFGSM_MC_ILLEGAL_STATE_TRANSITION,
  RFGSM_MC_STATE_ERROR,
  RFGSM_MC_STATUS_MAX,
}rfgsm_mc_status_type;

#ifdef FEATURE_RF_ASDIV
/*
  Enum to indicate the antenna which is "forced" to be enabled from SAR
*/
typedef enum
{
  RFGSM_MC_SAR_ANTENNA_NONE = -1,     /* no SAR forced selection */
  RFGSM_MC_SAR_ANTENNA_0,        /* pass through position */
  RFGSM_MC_SAR_ANTENNA_1,        /* swapped position */
  RFGSM_MC_SAR_ANTENNA_MAX,      /* enum limit for ant conditions */
} rfgsm_mc_sar_forced_antenna_enum_type;

#define RFGSM_SAR_FORCED_ANT_VALUE 0xFFFE
#endif /*FEATURE_RF_ASDIV*/

/* To convert USTMR to US without a divide          */
/* 1/19.2 << RFGSM_USTMR_TO_US_SCALE_FACTOR =  3413 */
#define RFGSM_USTMR_TO_US_SCALE_FACTOR    16
#define RFGSM_USTMR_TO_US_SCALED          3413
#define RFGSM_USTMR_COUNT_BITS            22

#define GET_START_TIME_WAKEUP_PROFILING(h,s)         \
    if(h->enable_time_profiling.wakeup == TRUE)      \
    {                                                \
      s = rfgsm_mc_read_ustmr_count();               \
    }                                                \

#define GET_CURRENT_TIME_WAKEUP_PROFILING(h, s, c)   \
    if(h->enable_time_profiling.wakeup == TRUE)      \
    {                                                \
      c = rfgsm_ustmr_count_diff_us_rtn(s);          \
    }                                                \

#define GET_START_TIME_SLEEP_PROFILING(h,s)          \
    if(h->enable_time_profiling.sleep == TRUE)       \
    {                                                \
      s = rfgsm_mc_read_ustmr_count();               \
    }                                                \

#define GET_CURRENT_TIME_SLEEP_PROFILING(h, s, c)    \
    if(h->enable_time_profiling.sleep == TRUE)       \
    {                                                \
      c = rfgsm_ustmr_count_diff_us_rtn(s);          \
    }                                                \

#define GET_START_TIME_RX_BURST_PROFILING(h,s)       \
    if(h->enable_time_profiling.rx_burst == TRUE)    \
    {                                                \
      s = rfgsm_mc_read_ustmr_count();               \
    }                                                \

#define GET_CURRENT_TIME_RX_BURST_PROFILING(h, s, c) \
    if(h->enable_time_profiling.rx_burst == TRUE)    \
    {                                                \
      c = rfgsm_ustmr_count_diff_us_rtn(s);          \
    }                                                \

#define GET_START_TIME_TX_BURST_PROFILING(h,s)       \
    if(h->enable_time_profiling.tx_burst == TRUE)    \
    {                                                \
      s = rfgsm_mc_read_ustmr_count();               \
    }                                                \

#define GET_CURRENT_TIME_TX_BURST_PROFILING(h, s, c) \
    if(h->enable_time_profiling.tx_burst == TRUE)    \
    {                                                \
      c = rfgsm_ustmr_count_diff_us_rtn(s);          \
    }                                                \

/* Read the current USTMR value */
static inline uint32 rfgsm_mc_read_ustmr_count( void )
{
  #if defined( HWIO_STMR_TIME_RD_ADDR )
    return HWIO_IN(STMR_TIME_RD);
  #elif defined( HWIO_UNIV_STMR_MSTMR_ADDR )
    return HWIO_IN(UNIV_STMR_MSTMR);
  #elif defined( HWIO_MSS_STMR_TIME_RD_ADDR )
    return HWIO_IN(MSS_STMR_TIME_RD);
  #else
  #error "USTMR register not defined"
  #endif
}

/* Calculate the difference in us based on USMTR reads */
static inline int32 rfgsm_ustmr_count_diff_us_rtn( uint32 start_ustmr )
{
  int32 diff = ( ( (0x3FFFFF & rfgsm_mc_read_ustmr_count()) - (0x3FFFFF & start_ustmr) ) << //difference
                 ( 32 - RFGSM_USTMR_COUNT_BITS ) ) >> ( 32 - RFGSM_USTMR_COUNT_BITS ); //sign-extension

  return ( ( diff * RFGSM_USTMR_TO_US_SCALED ) >> RFGSM_USTMR_TO_US_SCALE_FACTOR ); //USTMR diff to us
}

/* Calculate the difference in us based on USMTR reads */
static inline int32 rfgsm_ustmr_count_diff_a_b_us_rtn( uint32 start_ustmr, uint32 stop_ustmr )
{
  int32 diff = ( ( (0x3FFFFF & stop_ustmr) - (0x3FFFFF & start_ustmr) ) << //difference
                 ( 32 - RFGSM_USTMR_COUNT_BITS ) ) >> ( 32 - RFGSM_USTMR_COUNT_BITS ); //sign-extension

  return ( ( diff * RFGSM_USTMR_TO_US_SCALED ) >> RFGSM_USTMR_TO_US_SCALE_FACTOR ); //USTMR diff to us
}

extern rf_lock_data_type rfgsm_core_irat_meas_crit_sect;

extern boolean rfgsm_msgr_register( msgr_client_t *client_id, msgr_id_t id );
extern boolean rfgsm_msgr_deregister( msgr_client_t *client_id );
extern void rfgsm_dispatch_init(void);
extern void rfgsm_dispatch_deinit(void);
/*----------------------------------------------------------------------------*/
extern boolean rfgsm_msgr_register_sub2( msgr_client_t *client_id, msgr_id_t id );
extern boolean rfgsm_msgr_deregister_sub2( msgr_client_t *client_id );
extern void rfgsm_dispatch_init_2(void);
extern void rfgsm_dispatch_deinit_2(void);
/*----------------------------------------------------------------------------*/
extern void rfgsm_initialise_rflm_msg_router_q( rf_task_num_type task_num );
extern void rfgsm_rflm_rsp_dispatch(rf_task_num_type task_num);
/*----------------------------------------------------------------------------*/
extern boolean rfgsm_mc_init(boolean common_init_successful,
                             rex_tcb_type *caller_tcb_ptr,
                             rex_sigs_type task_nv_wait_sig,
                             void (*task_wait_func_ptr)( rex_sigs_type ));

/*----------------------------------------------------------------------------*/
boolean rfgsm_mc_deinit(void);

/*----------------------------------------------------------------------------*/

extern rfgsm_mc_status_type rfgsm_mc_start_gsm( uint8 sub_id, sawless_support_table_t sawless_support_table);

/*----------------------------------------------------------------------------*/
extern rfgsm_mc_status_type rfgsm_mc_stop_gsm( uint8 sub_id );

/*----------------------------------------------------------------------------*/
#ifdef FEATURE_RF_ASDIV
  extern rfgsm_mc_status_type rfgsm_mc_process_asd_read_rsp(rfgsm_core_data_type *core_data,
                                                            uint8 ant_position);
#endif

/*----------------------------------------------------------------------------*/

extern rfgsm_mc_status_type rfgsm_mc_process_device_status_read_rsp(uint8 sub_id,
                                                                    uint8 buffer_id,
                                                                    rf_burst_type burst_type,
                                                                    uint8 event_type,
                                                                    uint8 event_tag,
                                                                    uint8 burst_num);

/*----------------------------------------------------------------------------*/

extern rfgsm_mc_status_type rfgsm_mc_update_temp_comp_req(rfgsm_core_data_type *core_data,
                                                          int16 temp_comp_scaled_value,
                                                          uint8 current_temp_bin_16);

/*----------------------------------------------------------------------------*/

extern rfgsm_mc_status_type rfgsm_mc_set_rx_band( rfgsm_core_data_type *core_data,
                                                  rfgsm_band_type band );

/*----------------------------------------------------------------------------*/

extern rfgsm_mc_status_type rfgsm_mc_set_tx_band( rfgsm_core_data_type *core_data,
                                                  rfgsm_band_type tx_band,
                                                  void *shared_mem_ptr );

/*----------------------------------------------------------------------------*/
rfgsm_mc_status_type rfgsm_mc_set_simult_sar_limit( rfgsm_core_data_type *core_data,
                                                    int16 sar_limit_gmsk[5],
                                                    int16 sar_limit_8psk[5]);

/*----------------------------------------------------------------------------*/
rfgsm_mc_status_type rfgsm_mc_set_simult_vbatt_limit( rfgsm_core_data_type *core_data,
                                                    int16 vbatt_limit_gmsk[5],
                                                    int16 vbatt_limit_8psk[5]);

/*----------------------------------------------------------------------------*/

rfgsm_mc_status_type rfgsm_mc_process_tx_burst( rfgsm_core_data_type *core_data,
                                                rfa_rf_gsm_tx_burst_type *tx_burst_params );

/*----------------------------------------------------------------------------*/

boolean rfgsm_mc_update_tx_power_in_response_message( rfgsm_core_data_type *core_data, int32 *tx_pwr_in_dBm100 );

/*----------------------------------------------------------------------------*/
uint32 rfgsm_mc_get_wakeup_ccs_exe_us(rfm_device_enum_type rfm_dev);

/*----------------------------------------------------------------------------*/

extern boolean rfgsm_mc_reload_nv( rfm_device_enum_type rfm_dev,
                                   rex_tcb_type *caller_tcb_ptr,
                                   rex_sigs_type task_nv_wait_sig,
                                   void (*task_wait_func_ptr)( rex_sigs_type ) );

/*----------------------------------------------------------------------------*/

rfgsm_mc_status_type rfgsm_mc_do_idle_frame_processing( rfgsm_core_data_type *core_data );

/*----------------------------------------------------------------------------*/

boolean rfgsm_mc_update_sar_backoff( rfm_device_enum_type device );

/*----------------------------------------------------------------------------*/

boolean rfgsm_mc_log_iq_data( rfm_device_enum_type rfm_dev, GfwIqSamplesBuffer* data );

/*----------------------------------------------------------------------------*/

rfgsm_mc_status_type rfgsm_mc_cm_enter( rfgsm_core_data_type *core_data,
                                        rfgsm_timing_info_type *timing_info );

/*----------------------------------------------------------------------------*/

rfgsm_mc_status_type rfgsm_mc_cm_exit( rfgsm_core_data_type *core_data );

/*----------------------------------------------------------------------------*/
#ifdef FEATURE_RF_ASDIV
  rfgsm_mc_status_type rfgsm_mc_set_antenna_req(rfgsm_core_data_type *core_data, uint8 ant_pos);
  void rfgsm_mc_asd_rsp_check_stop( rfm_device_enum_type rfm_dev );
  /*----------------------------------------------------------------------------*/
#endif

/*----------------------------------------------------------------------------*/

boolean rfgsm_mc_build_ip2_cal_script(rfgsm_core_data_type *core_data);

/*----------------------------------------------------------------------------*/

boolean rfgsm_mc_msm_init_ip2_cal(rfgsm_core_data_type *core_data, uint32 ip2_rxlm_idx, uint32 ip2_txlm_idx);

/*----------------------------------------------------------------------------*/
uint32 rfgsm_mc_convert_arfcn_to_tx_freq(rfgsm_band_type band, uint16 arfcn);

/*----------------------------------------------------------------------------*/
uint32 rfgsm_mc_convert_arfcn_to_rx_freq(rfgsm_band_type band, uint16 arfcn);

/*----------------------------------------------------------------------------*/
boolean rfgsm_mc_process_burst_metrics(rfm_device_enum_type rfm_dev,
                                       rfa_rf_gsm_rx_burst_metrics_type **metrics_ptr,
                                       uint8 num_entries);

/*----------------------------------------------------------------------------*/

boolean rfgsm_mc_retrieve_sawless_support(boolean support_matrix[RFM_MAX_WAN_DEVICES][RFGSM_MAX_UNIQUE_RFCOM_BANDS]);

/*----------------------------------------------------------------------------*/

boolean rfgsm_mc_program_qfe_scenario(void);


/*!
 @brief
   Function sets/gets sar_forced_antenna if it is forced

 @return
   Return the antenna to be forced if sar value is 0xFFFE
*/

void rfgsm_mc_set_sar_forced_ant(rfgsm_mc_sar_forced_antenna_enum_type sar_ant_type);

rfgsm_mc_sar_forced_antenna_enum_type rfgsm_mc_get_sar_forced_ant(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RFGSM_MC_H */
